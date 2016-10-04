var SerialPort = require("serialport");
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var flags = [0,0,0,0];
var temperature = [0,0,0,0];
var sensor_data = [0,0,0,0];
var Time;
var time;
var count=0;
var sum =0;
var average;
var start = 1;
var stream1;
var i = 0;
var portName = process.argv[2],
portConfig = {
	baudRate: 9600,
	parser: SerialPort.parsers.readline("\n")
};

// setup the mongodb
var mongodb = require('mongodb');
var MongoClient = mongodb.MongoClient;
var url = 'mongodb://localhost:27017/temperature_data';
// plot the realTime 
var plotly = require('plotly')('holmekun','zyyom3il8v');
var initdata = [{x:[], y:[], stream:{token:'nyivgp46rr', maxpoints:200}}];
var initlayout = {fileopt : 'overwrite', filename : 'realTime'};

var sp;

//var config = require('./config.json'),
var  username = "holmekun",
var  apikey = "zyyom3il8v",
var  tokens = ["nyivgp46rr", "hzh0biwwqg", "g4yiqa8mx4", "lq27ne3yre", "isachqzo46"],
  //var  Plotly = require('../.')(username, apikey),
var  Signal = require('random-signal');



sp = new SerialPort.SerialPort(portName, portConfig);

app.get('/', function(req, res){
  res.sendfile('index.html');
});

io.on('connection', function(socket){
  console.log('a user connected');
  socket.on('disconnect', function(){
  });
  socket.on('chat message', function(msg){
    io.emit('chat message', msg);
    sp.write(msg + "\n");
  });
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});


var layout = {
    filename: "stream-multiple-traces",
    fileopt: "overwrite",
    layout: {
      title: "streaming mock sensor data"
    },
    world_readable: true
};

function timeOut(){
  
  for(i=0;i<4;i++)
      {
         sensor_data[i] = temperature[i];
         sum = sum + temperature[i];
         flags[i] = 0;
         temperature[i] = 0;
      }
      average = (sum/count).toFixed(2);
      sum = 0;
      count = 0;
      var time = new Date();
      var month = time.getMonth();
      var date = time.getDate();
      var hour = time.getHours();
      var minute = time.getMinutes();
      var second = time.getSeconds();
      Time = month.toString() + "/" + date.toString() + "/" + hour.toString() + ":" + minute.toString() + ":" + second.toString();
      console.log(Time);
      console.log('Average Temperature: ' + average + " C");
      // connect to the database
      MongoClient.connect(url, function (err, db) {
        if (err) {
            console.log('Unable to connect to the mongoDB server. Error:', err);
        } else {
            //HURRAY!! We are connected. :)
            //console.log('Connection established to', url);

            // do some work here with the database.
            var collection = db.collection('SensorW');
            var temp1 = {SensorW: sensor_data[0], Time: Time};
            collection.insert(temp1, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('SensorX');
            var temp2 = {SensorX: sensor_data[1], Time: Time};
            collection.insert(temp2, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('SensorY');
            var temp3 = {SensorY: sensor_data[2], Time: Time};
            collection.insert(temp3, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('SensorZ');
            var temp4 = {SensorZ: sensor_data[3], Time: Time};
            collection.insert(temp4, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('Average');
            var temp5 = {Average: average, Time: Time};
            collection.insert(temp5, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  console.log('written average!');
              }
              //Close connection
              }); 
            db.close();
            }
          });
        /*
        var data0 = {
            'x':Time   // empty arrays since we will be streaming our data to into these arrays
          , 'y':average
          , 'type':'scatter'
          , 'mode':'lines+markers'
          , marker: {
              color: "rgba(31, 119, 180, 0.96)"
            }
          , line: {
              color:"rgba(31, 119, 180, 0.31)"
            }
          , stream: {
              "token": token[0]
          , "maxpoints": 100
            }
          }
        */
        function initTrace(i) {
          return {
            x: Time,  // init. data arrays
            y: Average,
            type: 'scatter',
            mode: 'lines+markers',
            stream: {
              "token": tokens[0],
              "maxpoints": 100
            }
          };
        }
        /*
        function initTrace(1) {
          return {
            x: Time,  // init. data arrays
            y: sensor_data[0],
            type: 'scatter',
            mode: 'lines+markers',
            stream: {
              "token": tokens[1],
              "maxpoints": 100
            }
          };
        }

        function initTrace(2) {
          return {
            x: Time,  // init. data arrays
            y: sensor_data[1],
            type: 'scatter',
            mode: 'lines+markers',
            stream: {
              "token": tokens[2],
              "maxpoints": 100
            }
          };
        }

        function initTrace(3) {
          return {
            x: Time,  // init. data arrays
            y: sensor_data[2],
            type: 'scatter',
            mode: 'lines+markers',
            stream: {
              "token": tokens[3],
              "maxpoints": 100
            }
          };
        }

        function initTrace(4) {
          return {
            x: Time,  // init. data arrays
            y: sensor_data[3],
            type: 'scatter',
            mode: 'lines+markers',
            stream: {
              "token": tokens[4],
              "maxpoints": 100
            }
          };
        }
        */
        //var data1 = { x : Time, y : average};
        //var data2 = { x : Time, y : sensor_data[1]};
        var data = [0, 1].map(initTrace)

        [0, 1].forEach(function(i){
        var plotlystream = Plotly.stream(tokens[i], function(){}),
          singalstream = Signal({tdelta: 100});

          plotlystream.on("error", function(err){singalstream.destroy();});
          singalstream.pipe(plotlystream);
        });
        //var streamObject1 = JSON.stringify(data1);
        //var streamObject2 = JSON.stringify(data2);
        //stream1.write(streamObject1+'\n');
        //stream2.write(streamObject2+'\n');
        var delay = setTimeout(timeOut, 2000);
        /*
        if (second == 0){
          var data = { x : second + 60 * i, y : average };
          var streamObject = JSON.stringify(data);
          stream1.write(streamObject+'\n');
          i++;
          var delay = setTimeout(timeOut, 2000);
        }
        else{
          var data = { x : second + 60 * i, y : average };
          var streamObject = JSON.stringify(data);
          stream1.write(streamObject+'\n');
          var delay = setTimeout(timeOut, 2000);
        }*/
}

function calculateAvg(){
  for(i=0;i<4;i++)
      {
         sensor_data[i] = temperature[i];
         sum = sum + temperature[i];
         flags[i] = 0;
         temperature[i] = 0;
      }
      average = (sum/count).toFixed(2);
      sum = 0;
      count = 0;
}

function getTheTime(){
  var time = new Date();
      var date = time.getDate();
      var hour = time.getHours();
      var minute = time.getMinutes();
      var second = time.getSeconds();
      Time = date.toString() + " " + hour.toString() + ":" + minute.toString() + ":" + second.toString();
} 

function writeToDatabase(){
  MongoClient.connect(url, function (err, db) {
        if (err) {
            console.log('Unable to connect to the mongoDB server. Error:', err);
        } else {
            //HURRAY!! We are connected. :)
            //console.log('Connection established to', url);

            // do some work here with the database.
            var collection = db.collection('SensorW');
            var temp1 = {SensorW: sensor_data[0], Time: Time};
            collection.insert(temp1, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('SensorX');
            var temp2 = {SensorX: sensor_data[1], Time: Time};
            collection.insert(temp2, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('SensorY');
            var temp3 = {SensorY: sensor_data[2], Time: Time};
            collection.insert(temp3, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('SensorZ');
            var temp4 = {SensorZ: sensor_data[3], Time: Time};
            collection.insert(temp4, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  //console.log('written average!');
              }
              }); 
            var collection = db.collection('Average');
            var temp5 = {Average: average, Time: Time};
            collection.insert(temp5, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  console.log('written average!');
              }
              //Close connection
              }); 
            db.close();
            }
          });
}

sp.on("open", function () {
  console.log('open');
  plotly.plot(data, layout, function (err, msg) {
    if (err) return console.log(err);
    console.log(msg);
    
  });
    /*stream1 = plotly.stream('nyivgp46rr', function (err, res) {
        if (err) return console.log(err);
        console.log(res);
        //clearInterval(loop); // once stream is closed, stop writing
      });
    stream2 = plotly.stream('g4yiqa8mx4', function (err, res) {
        if (err) return console.log(err);
        console.log(res);
        //clearInterval(loop); // once stream is closed, stop writing
      });*/

    
  sp.on('data', function(data) {
    
    if (start == 1){
      start = 0;
      var delay = setTimeout(timeOut, 2000);
      console.log('1');
    }
    packet = data.split(' ');
    if(packet[0] == 'W' && flags[0] == 0)
    {
	     flags[0] = 1;
       count++;
	     temperature[0] = parseFloat(packet[1]);
	     //console.log('Data Received W: ' + temperature[0]);
    }
    if(packet[0] == 'X' && flags[1] == 0)
    {
	     flags[1] = 1;
	     count++;
       temperature[1] = parseFloat(packet[1]);	
	     console.log('Data Received X: ' + temperature[1]);
    }
    if(packet[0] == 'Y' && flags[2] == 0)
    {
	      flags[2] = 1;
        count++;
	      temperature[2] = parseInt(packet[1]);
	      console.log('Data Received Y: ' + temperature[2]);
    }
    if(packet[0] == 'Z' && flags[3] == 0)
    {
	      flags[3] = 1;
        count++;
	      temperature[3] = parseInt(packet[1]);
	      //console.log('Data Received Z: ' + temperature[3]);
    }
    
    //io.emit("chat message", "Average Temperature:" + average + " C");
    
      
  }); 
});








