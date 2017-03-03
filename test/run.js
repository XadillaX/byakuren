#! /usr/bin/env node
var os = require("os");
var pixelGetter = require("pixel-getter");
var path = require("path");
var fs = require("fs");
var async = require("async");
var run = require("sync-runner");
var exec = require("child_process").exec;
var opts = require("nomnom")
    .option("url", {
        abbr    : "u",
        default : "/tmp/a.jpg",
        help    : "picture path."
    }).option("algorithm", {
        abbr    : "a",
        default : "all",
        help    : "algorithm of this time."
    }).parse();

var dirname = path.dirname(__filename);

function int32ToRGB(value) {
    var rgb = value.toString(16);
    while(rgb.length < 6) rgb = "0" + rgb;
    return rgb;
}

if(opts.algorithm === "all") {
    async.waterfall([
        function(callback) {
            exec("./test/run.js -u " + opts.url + " -a octree", {
                cwd: path.resolve(dirname, "../")
            }, function(err, stdout, stderr) {
                if(err) return callback(err);
                console.log(stdout);
                console.log(stderr);
                callback();
            });
        },
        function(callback) {
            exec("./test/run.js -u " + opts.url + " -a mindiff", {
                cwd: path.resolve(dirname, "../")
            }, function(err, stdout, stderr) {
                if(err) return callback(err);
                console.log(stdout);
                console.log(stderr);
                callback();
            });
        },
        function(callback) {
            exec("./test/run.js -u " + opts.url + " -a mix", {
                cwd: path.resolve(dirname, "../")
            }, function(err, stdout, stderr) {
                if(err) return callback(err);
                console.log(stdout);
                console.log(stderr);
                callback();
            });
        }
    ], function(err) {
        if(err) {
            console.error(err.message);
            process.exit(4);
        }

        process.exit(0);
    });
} else {
    async.waterfall([
        function(callback) {
            pixelGetter.get(opts.url, callback, 1, 10000);
        },
        function(pixels, callback) {
            pixels = pixels[0];
            var buff = new Buffer(3 * pixels.length);
            for(var i = 0; i < pixels.length; i++) {
                buff[i * 3]     = pixels[i].r;
                buff[i * 3 + 1] = pixels[i].g;
                buff[i * 3 + 2] = pixels[i].b;
            }
    
            callback(undefined, buff);
        },
        function(buff, callback) {
            fs.writeFile(dirname + "/test.rgb", buff, { encoding: "binary" }, callback);
        },
        function(callback) {
            console.log("Making `bkr_test`...");
            console.log(run("make ./test/bkr_test", path.resolve(dirname, "../")));
    
            console.log("Running `bkr_test " + opts.algorithm + "`...");
            exec("./bkr_test " + opts.algorithm, {
                cwd: dirname
            }, function(err, stdout) {
                callback(undefined, stdout);
            });
        },
        function(result, callback) {
            var json;
            try {
                json = JSON.parse(result);
            } catch(e) {
                return callback(new Error(result));
            }
    
            callback(undefined, json);
        },
        function(result, callback) {
            // write a file
            var html = "<h1>" + opts.algorithm + "</h1>";
            html += "<img src=\"" + opts.url + "\" /><br /><br />";
    
            for(var i = 0; i < result.count; i++) {
                html += "<div style=\"width: 50px; height: 21px; float: left; margin-right: 5px; margin-bottom: 5px; background: #" + int32ToRGB(result.colors[i].color) + "; color: #fff; font-size: 12px; text-align: center; padding-top: 9px;\">" + result.colors[i].count + "</div>";
            }
    
            var filename = "/tmp/" + (+new Date()) + ".html";
            fs.writeFile(filename, html, { encoding: "utf8" }, function(err) {
                if(err) return callback(err);
                return callback(undefined, filename);
            });
        }
    ], function(err, filename) {
        if(err) {
            console.error(err.message);
            process.exit(4);
        }
    
        run((os.platform() === "darwin" ? "open " : "google-chrome ") + filename);
    });
}

