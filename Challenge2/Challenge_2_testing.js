var SerialPort = require("serialport");
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);

/*************VARIABLES DECLARATION**************/
var timeCount = 0;
var tags = new Array();
var realData = new Array(1000); //Number of Devices
var realAvg = 0;
var tagIndex;

var portName = process.argv[2],
portConfig = {
	baudRate: 9600,
	parser: SerialPort.parsers.readline("\n")
};

/*****************MONGO DB SETUP********************/
var mongodb = require('mongodb');
var MongoClient = mongodb.MongoClient;
var url = 'mongodb://localhost:27017/temperature_data';
var sp;


/****************REAL TIME CHART*******************/
var plotly = require('plotly')('lllllouissssss','bhu0y55672');
var initdata = [{x:[], y:[], stream:{token:'bouynjwumy', maxpoints:200}}];
var initlayout = {fileopt : 'overwrite', filename : 'nodenodenode'};

/****************SERIAL PORT CONNECTION**********/
sp = new SerialPort.SerialPort(portName, portConfig);

app.get('/', function(req, res)
{
	res.sendfile('index.html');
});

io.on('connection', function(socket)
{
	console.log('a user connected');
 	socket.on('disconnect', function()
	{});
  	socket.on('chat message', function(msg)
	{
    		io.emit('chat message', msg);
    		sp.write(msg + "\n");
  	});
});

http.listen(3000, function()
{
  	console.log('listening on *:3000');
});

/* 1. OPEN CONNECTION TO XBEE.
   2. DYNAMICALLY ADD AND DROP DEVICES.
   3. IT READS THE TAG FROM THE SENSORS AND KEEPS A RUNNING TALLY OF THE SENSORS.
   4. IF A SENSOR IS NOT CONNECTED TO THE NETWORK, IT WILL NOT WRITE TO THE DATABASE.*/
sp.on("open", function ()
{
  	console.log('open');
    	plotly.plot(initdata, initlayout, function (err, msg)
	{
    		if (err) return console.log(err);
    		console.log(msg);
    		stream1 = plotly.stream('bouynjwumy', function (err, res)
		{
        		if (err) return console.log(err);
        		console.log(res);
		});

  	sp.on('data', function(data)
	{
    		packet = data.split(' ');
   		if (inArray(packet[0],tags) == 0)
		{
			tags.push(packet[0]);
		}

    		tagIndex = tags.indexOf(packet[0]);
    		realData[tagIndex] = parseFloat(packet[1]);
   	
    		/***********WRITINSENSOR DATA TO DB************/
    		MongoClient.connect(url, function (err, db)
		{
			if (err)
			{
            	console.log('Unable to connect to the mongoDB server. Error:', err);
        		} 
			else
			{
				// create a collection based on the tag you get from the Xbee packet
				// if the tag does not exist, this will create a new collection.
				// if the tag does exist, it will insert a timestamp and the temperature for thaft tag.
				var collection = db.collection(packet[0]);
		
				// get Date timestamp
				var d = new Date();
				var hrs = d.getHours();
				var min = d.getMinutes();
				var sec = d.getSeconds();
				var time = hrs.toString().concat(':',min.toString(),':',sec.toString());
				var sens_temp = {'Time' : time, 'Data' : parseFloat(packet[1]).toFixed()};

				collection.insert(sens_temp, function (err, result)
				{
					if (err)
					{
						 console.log(err);
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
		if(timeCount == 10)
		{ 
			total = 0;
			numberOfThingsToTakeAverageOf = 0;
			realAvg = 0;
		
			for(var j = 0; j < tags.length; j++)
			{
				if(!(realData[j] == 0))
				{
					total = total + realData[j];
					numberOfThingsToTakeAverageOf++;
				}
			}
			realAvg = (total/numberOfThingsToTakeAverageOf).toFixed(2);
			//console.log(realAvg);
		
	       		//WRITING AVERAGE TO THE DB
			MongoClient.connect(url, function (err, db)
			{
				if (err)
				{
			        console.log('Unable to connect to the mongoDB server. Error:', err);
			    	}
				else
				{
			     	// create a collection based on the tag you get from the Xbee packet
			        // if the tag does not exist, this will create a new collection.
			        // if the tag does exist, it will insert a timestamp and the temperature for thaft tag.
			        var collection = db.collection("average");
			        
					// get Date timestamp
					var d = new Date();
					var hrs = d.getHours();
					var min = d.getMinutes();
					var sec = d.getSeconds();
					var time = hrs.toString().concat(':',min.toString(),':',sec.toString());
			        var sens_temp = { 'Time': time, 'Data': parseFloat(realAvg) };

			        collection.insert(sens_temp, function (err, result)
					{
			            if (err)
						{
			                console.log(err);
			            		}
			        	});
			        	db.close();
			    	}
			});
			//var f = new Date();
			var d = new Date();
			var hrs = d.getHours();
			var min = d.getMinutes();
			var sec = d.getSeconds();
			var time = hrs.toString().concat(':',min.toString(),':',sec.toString());
			var data = { x : time, y : realAvg };
          	var streamObject = JSON.stringify(data);
          	stream1.write(streamObject+'\n');
		
			for(var q = 0; q < tags.length; q++)
			{
				realData[q] = 0;
			}	 
			timeCount = 0;
		}
    	}); 
});
});

function inArray(tag,tagsArray)
{
	var arrLen = tagsArray.length;
    	var flag = 0;
    	for(var k=0;k<=arrLen;k++)
    	{
		if((tag.localeCompare(tagsArray[k])) == 0)
		{
			flag = 1;
			break;
		}
    	}
   	return flag;
}
