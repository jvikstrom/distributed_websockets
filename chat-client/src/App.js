import React, { Component } from 'react';
import { Container, Button, Input, Grid } from 'semantic-ui-react';
import _ from 'lodash';
//import WebSocket from 'ws';
import './App.css';
import Login from './Login.js';

const url = 'ws://localhost:8010/ws/';

class App extends Component {
  constructor(props){
    super(props);
    console.log('websocket: ' + url);
    let ws = new WebSocket(url);
    ws.onopen = () => {
      console.log('Opened websocket');
      this.setState({open: true});
    };
    ws.onmessage = (data) => this.messageHandler(data);

    this.state = {
      ws: ws,
      open: false,
      users: [],
      messages: [],
      selected: ''
    }

    /*this.state.ws.on('open', () => {
      console.log('Opened websocket');
      this.setState({open: true});
    });*/
    /*this.state.ws.onopen = () => {
      console.log('Opened websocket');
      this.setState({open: true});
    };
    this.state.ws.onmessage = (data) => this.messageHandler(data);
*/
    //this.state.ws.on('open', () => this.setState({open: true}));
    //this.state.ws.on('message', this.messageHandler);
  }

  onLogin(user){
    this.state.ws.send(JSON.stringify(Object.assign({}, {cmd: 'login'}, user)));
    this.setState({user: user});
  }

  render() {
    console.log(this.state);
    if(!this.state.open){
      console.log('Render 1');
      return (<div>Opening websocket...</div>);
    }

    if(!this.state.user){
      console.log('Render 2');
      return (<Login ws={this.state.ws} onLogin={(user) => this.onLogin(user)}/>);
    }

    console.log('Render 3');
    console.log(this.state.users);
    return (
      <Grid columns={2}>
        <Grid.Column>
          <List ws={this.state.ws} users={this.state.users} onSelect={(usr) => this.setState({selected: usr})}/>
        </Grid.Column>
        <Grid.Column>
          <ChatBox ws={this.state.ws} target={this.state.selected} messages={this.state.messages} onSendMessage={(e) => this.addMessage(e)}/>
        </Grid.Column>
      </Grid>
    );
  }

  addMessage(msg){
    let messages = this.state.messages;
    if(!messages[msg.to]){
      messages[msg.to] = [];
    }
    messages[msg.to].push(msg);
  }

  messageHandler(data) {
    console.log('Message: ', data.data);
    try{
      data = JSON.parse(data.data);
    }catch(e){
      console.log('Exception on JSON: ', data.data);
    }

    if(data.cmd === 'list'){
      console.log('Set state');
      return this.setState({users: data.users});
    }//KingdomGame Deliverence

    if(data.cmd === 'login'){
      return this.setState({user: {username: data.username, password: data.password}});
    }

    if(data.cmd === 'receive'){

      let messages = this.state.messages;
      if(!messages[data.from]){
        messages[data.from] = [];
      }
      console.log('messages: ', messages);
      messages[data.from].push(data);
      console.log(messages[data.from]);
      return this.setState({messages: messages});
    }
  }
}

class ChatBox extends Component{
  constructor(props){
    super(props);
    this.state = {
      msg: ''
    }
  }

  sendMessage(){
    this.props.ws.send(JSON.stringify({cmd: 'send', target: this.props.target, msg: this.state.msg}));
    this.props.onSendMessage({to: this.props.target, msg: this.state.msg});
  }
  render(){
    if(!this.props.target){
      return <Container>Select a user to chat with</Container>
    }

    let it = 0;
    return (
      <Container>
      {_.map(this.props.messages[this.props.target], (value) => {
        if(value.from){
          return <Container key={it++}>From: {value.from}. Message: {value.msg}</Container>
        }
        return <Container key={it++}>To: {value.to}. Message: {value.msg}</Container>
      })}
      <Container><Input onChange={(e) => this.setState({msg: e.target.value})} placeholder='Message'/><Button onClick={() => this.sendMessage()}>Send</Button></Container>
      </Container>
    )
  }
}

class List extends Component{
  render(){
    return (
      <Container>
        <Container>
          <Button onClick={(e) => {console.log('Sending....');
          this.props.ws.send(JSON.stringify({cmd: 'list'}))}}>Update</Button>
        </Container>
        <Container>
          {_.map(this.props.users, (value, key) =>{console.log('val: ', value); return (<Button key={value} onClick={() => this.props.onSelect(value)}>{value}</Button>)})}
        </Container>
      </Container>
    )
  }
}
/*
<div className="App">
  <div className="App-header">
    <img src={logo} className="App-logo" alt="logo" />
    <h2>Welcome to React</h2>
  </div>
  <p className="App-intro">
    To get started, edit <code>src/App.js</code> and save to reload.
  </p>
</div>
*/

export default App;
