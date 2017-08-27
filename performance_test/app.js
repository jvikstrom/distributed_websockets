'use strict';
var _ = require('lodash');
var async = require('async');
var uuid = require('uuid');
var config = require('./config.json');
var WebSocket = require('ws');
var asyncInterval = require('asyncinterval');

var conns = [];
var connsByUsername = {};
var usernames = [];

var expects = {};
var n = 0;
var avg = 0;
var low = 100000000;
var high = 0;
function addExpect(addUsername, msg, sendTime){
  if(!expects[addUsername]){
    expects[addUsername] = {};
  }
  expects[addUsername][msg] = sendTime;
}

async.series([
  openAll,
  loginAll,
  sendRequests.bind(null, config.numRequestsLow),
  function(cb){
    setTimeout(cb, 500);
  },
  sendRequests.bind(null, config.numRequestMedium),
  function(cb){
    setTimeout(cb, 2000);
  },
  sendRequests.bind(null, config.numRequestHigh),
  function(cb){
    setTimeout(cb, 30000); //Allow messages to be sent
  },
  sendRequests.bind(null, config.numRequestHigh),
  function(cb){
    setTimeout(cb, 30000); //Allow messages to be sent
  },
  sendRequests.bind(null, config.numRequestHigh),
  function(cb){
    setTimeout(cb, 30000); //Allow messages to be sent
  },
  sendRequests.bind(null, config.numRequestVeryHigh),
  function(cb){
    setTimeout(cb, 30000); //Allow messages to be sent
  },
  sendRequests.bind(null, config.numRequestVeryHigh),
  function(cb){
    setTimeout(cb, 30000); //Allow messages to be sent
  },
  sendRequests.bind(null, config.numRequestHigh),
  function(cb){
    setTimeout(cb, 30000); //Allow messages to be sent
  },
  sendRequests.bind(null, config.numRequestHigh),
  function(cb){
    setTimeout(cb, 30000); //Allow messages to be sent
  },
  closeAll

], function(err){
  if(err){
    throw err;
  }
  console.log('Done!');
  console.log('Number of sent messages: ', n);
  console.log('Avarage response time: ', avg);
});

function openAll(callback){
  async.timesSeries(config.numConnections, function(n, cb){
    var ws = new WebSocket('ws://' + config.host + ':' + config.port + '/ws');
    ws.on('open', function(){
      cb();
    });
    ws.on('error', function(err){
      console.log('Error on websocket: ' + n);
      throw err;
    });
    conns.push(ws);
  }, function(err){
    console.log('Opened all websockets...');
    callback(err);
  });
}

function sendRequests(numRequests, callback){
  console.log('Sending requests number: ' + numRequests);
  async.forEach(conns, function(ws, cb){
    function sendMessage(){
      var index = Math.floor(Math.random()*(usernames.length - 1));
      var username = usernames[index];
      var msg = uuid();
      var payload = {
        cmd: 'send',
        target: username,
        msg: msg
      };
      ws.send(JSON.stringify(payload));
      addExpect(username, msg, new Date().getTime());
    }

    var delay = 0;
    for(var i = 0; i < numRequests; i++){
      setTimeout(sendMessage, delay);
      delay += 1;
    }
    setTimeout(cb, 20);
  }, callback);
}

function closeAll(callback){
  console.log('CLosing all connections...');
  conns.forEach(function(ws){
    ws.close();
  });
  callback();
}

function loginAll(callback){
  console.log('Login all');
  async.parallelLimit(_.map(conns, function(sock){return login.bind(null, sock);}), 10, function(err){
    console.log('Logged in all!');
    callback(err);
  });
}

function login(ws, cb){
  var username = uuid();
  var password = '';
  ws.send(JSON.stringify({cmd: 'login', username: username, password: password}));
  ws.once('message', function(data){
    connsByUsername[username] = ws;
    usernames.push(username);
    cb();
  });
  ws.on('message', function(data){
    data = data.toString('utf8');
    var json = JSON.parse(data);
    if(json.cmd === 'receive'){
      if(expects[username] && expects[username][json.msg]){
        var t = new Date().getTime() - expects[username][json.msg];
        avg = (avg * n + (t)) / (n + 1);
        n++;
        high = high > t ? high : t;
        low = low > t ? t : low;
        expects[username][json.msg] = undefined;
      }
    }
  })
}

function countMissed(){
  var N = 0;
  _.forEach(expects, function(messages, key){
    _.forEach(messages, function(value, innerKey){
      if(value){
        N++;
      }
    });
  });
  return N;
}
