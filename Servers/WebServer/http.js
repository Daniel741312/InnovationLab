/*
    *基于nodejs写的一个简单的httpServer
*/

/*引入依赖*/
const fs = require("fs");
const http = require("http");

var server = http.createServer();

server.listen(9527, function () {
    console.log("请访问localhost:9527");
});

server.on("request", function (request, response) {
    console.log(request.method + "\t" + request.url);

    if (request.url == "/") {
        /*如果请求的是根目录，则将主页发送给客户端*/
        fs.readFile("./index.html", "utf8", function (err, data) {
            response.end(data);
        });
    } else if (request.url == "/getAllTrashesInfo") {
        /*如果发送过来的是ajax请求，则将发送文件allTrashesInfo.json给客户端*/
        fs.readFile("../allTrashesInfo.json", "utf-8", function (err, data) {
            response.end(data);
        });
    } else if (request.url == "/favicon.ico") {
        fs.readFile("./img/favicon.ico", function (err, data) {
            response.end(data);
        })

    } else if (request.url == "/map.js") {
        fs.readFile("./map.js", "utf-8", function (err, data) {
            response.end(data);
        });
    } else {
        /*如果请求不同的垃圾桶Icon，将对应的图片发给前端*/
        fs.readFile("./img" + request.url, "binary", function (err, data) {
            response.write(data, "binary");
            response.end(data);
        });
    }
});