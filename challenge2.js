var SerialPort = require("serialport");
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var flags = [0,0,0,0];
var temperature = [0,0,0,0];
var sensor_data = [0,0,0,0];
var count=0;
var sum =0;
var average;
var start = 1;
var portName = process.argv[2],
portConfig = {
	baudRate: 9600,
	parser: SerialPort.parsers.readline("\n")
};

// setup the mongodb
var mongodb = require('mongodb');
var MongoClient = mongodb.MongoClient;
var url = 'mongodb://localhost:27017/temperature_data';
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
            var temp = {SensorW: sensor_data[0], SensorX: sensor_data[1], SensorY: sensor_data[2], SensorZ: sensor_data[3], Average: average};
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
        
        var delay = setTimeout(timeOut, 2000);

}

sp.on("open", function () {
  console.log('open');
  
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
    /*
    MongoClient.connect(url, function (err, db) {
        if (err) {
            console.log('Unable to connect to the mongoDB server. Error:', err);
        } else {
            //HURRAY!! We are connected. :)
            //console.log('Connection established to', url);

            // do some work here with the database.
            var collection = db.collection('temperature');
            var temp = {sensor1: temperature[0], sensor2: temperature[1], sensor3: temperature[2], sensor4: temperature[3] };
            collection.insert(temp, function (err, result){
              if (err){
                  console.log(err);
              }else{
                  console.log('written sensors!');
              }
              //Close connection
              db.close();
              }); 
            }
          });*/

      
  }); 
});
