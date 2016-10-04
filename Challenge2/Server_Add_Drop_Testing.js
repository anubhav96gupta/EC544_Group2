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
var timeCount = 0;
var tags = new Array();
var realData = new Array(1000);
var realAvg = 0;
var tagIndex;
// Scaling devices
var numDevices = 4

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


// Set up connections to plotly
var plotly = require('plotly')('holmekun','zyyom3il8v');
var initdata = [{x:[], y:[], stream:{token:'nyivgp46rr', maxpoints:200}}];
var initlayout = {fileopt : 'overwrite', filename : 'nodenodenode'};

// Set up graph schema for plotly



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



// Open connection to the Xbee
// We can dynamically add and drop sensors
// It reads the tag from the sensors, and keeps a running tally of the connected sensors
// If a sensor is not connected to the network, it will not write to the databse.
sp.on("open", function () {
  console.log('open');
 

    plotly.plot(initdata, initlayout, function (err, msg) {
    if (err) return console.log(err);
    console.log(msg);
    stream1 = plotly.stream('nyivgp46rr', function (err, res) {
        if (err) return console.log(err);
        console.log(res);
	});

 
  sp.on('data', function(data) {
    
    packet = data.split(' ');
   if (inArray(packet[0],tags) == 0){
	tags.push(packet[0]);
	}

    tagIndex = tags.indexOf(packet[0]);
    realData[tagIndex] = parseFloat(packet[1]);
   	
    //wRITING TO THE DB
    MongoClient.connect(url, function (err, db) {
	if (err) {
            console.log('Unable to connect to the mongoDB server. Error:', err);
        } else {
		
		// create a collection based on the tag you get from the Xbee packet
		// if the tag does not exist, this will create a new collection.
		// if the tag does exist, it will insert a timestamp and the temperature for thaft tag.
		var collection = db.collection(packet[0]);
		// get Date timestamp
		var d = new Date();
		d.getHours();
		d.getMinutes();
		d.getSeconds();

		var dateStr = d.toString();

		var sens_temp = {'Date' : dateStr, 'Data' : parseFloat(packet[1])};


		collection.insert(sens_temp, function (err, result) {
			if (err) { console.log(err);
			}
		});
	db.close();
	}
});

timeCount++;	
	

	var total;
	var numberOfThingsToTakeAverageOf	


	// Once we reach the designated time count
	// Calculate the average
	// and reset the temp data arrays to 0
	if(timeCount == 10){ 
		
		total = 0;
		numberOfThingsToTakeAverageOf = 0;
		realAvg = 0;
		
		for(var j = 0; j < tags.length; j++) {
			if(!(realData[j] == 0)) {
				total = total + realData[j];
				numberOfThingsToTakeAverageOf++;
			}
			}
			realAvg = total/numberOfThingsToTakeAverageOf;
			console.log(realAvg);
		var f = new Date();
		var data = { x : f.getSeconds(), y : realAvg };
          	var streamObject = JSON.stringify(data);
          	stream1.write(streamObject+'\n');
		
		for(var q = 0; q < tags.length; q++) {
			realData[q] = 0;
		} 
		timeCount = 0;
	}
    
  }); 
});
});

function inArray(tag,tagsArray) {
    var arrLen = tagsArray.length;
    var flag = 0;
    for(var k=0;k<=arrLen;k++)
    {
	if((tag.localeCompare(tagsArray[k])) == 0) {
		 flag = 1;
		break;
	}
    }
   return flag;
}


/*function getTheTime(){
  var time = new Date();
      var date = time.getDate();
      var hour = time.getHours();
      var minute = time.getMinutes();
      var second = time.getSeconds();
      Time = date.toString() + " " + hour.toString() + ":" + minute.toString() + ":" + second.toString();
} */
