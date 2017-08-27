process.env.NODE_ENV = 'test';

var expect = require('chai').expect,
    should = require('chai').should,
    asyncInterval = require('../lib/asyncInterval');

describe('asyncInterval cases', function() {

    it('returns object', function(done) {

        var interval = asyncInterval(function(next) {}, 5);
        expect(interval).to.be.a('object');
        done();
    });

    it('basic', function(done) {

        var count = 0;
        var interval = asyncInterval(function(next) {

            setTimeout(function(n) {
                count++;
                if (count === 2) {
                    interval.clear();
                    expect(count).to.equal(2);
                    done();
                } else {
                    n();
                }
            }, 5, next);
        }, 10);
    });

    it('clear', function(done) {

        var count = 0;
        var interval = asyncInterval(function(next) {

            count++;
            next();
        }, 10);
        interval.clear();

        setTimeout(function() {

            expect(count).to.be.equal(0);
            done();
        }, 20);
    });

    it('does wait for previous call to complete', function(done) {

        var working = 0;
        var completed = 0;
        var interval = asyncInterval(function(next) {

            working++;
            setTimeout(function(n) {
                expect(working - completed).to.equal(1);
                completed++;
                if (completed === 3) {
                    interval.clear();
                    done();
                } else {
                    n();
                }
            }, 2, next);
        }, 10);
    });

    it('timeout', function(done) {

        var interval = asyncInterval(function(next) {

        }, 5, 10);

        interval.onTimeout(function() {
            interval.clear();
            done();
        });
    });
});
