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
var initlayout = {fileopt : 'overwrite', filename : 'nodenodenode'};

var sp;



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
      var date = time.getDate();
      var hour = time.getHours();
      var minute = time.getMinutes();
      var second = time.getSeconds();
      Time = date.toString() + " " + hour.toString() + ":" + minute.toString() + ":" + second.toString();
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
            var collection = db.collection('temperature');
            var temp = {SensorW: sensor_data[0], SensorX: sensor_data[1], SensorY: sensor_data[2], SensorZ: sensor_data[3], Average: average, Time: Time};
            collection.insert(temp, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  console.log('written average!');
              }
              //Close connection
              db.close();
              }); 
            }
          });

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
        }
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
            var collection = db.collection('temperature');
            var temp = {SensorW: sensor_data[0], SensorX: sensor_data[1], SensorY: sensor_data[2], SensorZ: sensor_data[3], Average: average, Time: Time};
            collection.insert(temp, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  console.log('written average!');
              }
              //Close connection
              db.close();
              }); 
            }
          });
}

sp.on("open", function () {
  console.log('open');
  plotly.plot(initdata, initlayout, function (err, msg) {
    if (err) return console.log(err);
    console.log(msg);
    stream1 = plotly.stream('nyivgp46rr', function (err, res) {
        if (err) return console.log(err);
        console.log(res);
        //clearInterval(loop); // once stream is closed, stop writing
      });

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
});




