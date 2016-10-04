var SerialPort = require("serialport");
var app = require('express')();
var http = require('http').Server(app);
var io = require('socket.io')(http);
var flags = [0,0,0,0];
var temperature = [0,0,0,0];
var count=0;
var sum =0;
var average;
var portName = process.argv[2],
portConfig = {
	baudRate: 9600,
	parser: SerialPort.parsers.readline("\n")
};
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

sp.on("open", function () {
  console.log('open');
  sp.on('data', function(data) {
    console.log('data received: ' + data);
    packet = data.split(' ');
    if(packet[0] == 'W')
    {
	flags[0] = 1;
        count++;
	temperature[0] = parseFloat(packet[1]);
    }
    if(packet[0] == 'X')
    {
	flags[1] = 1;
	count++;
        temperature[1] = parseFloat(packet[1]);
    }
    if(packet[0] == 'Y')
    {
	flags[2] = 1;
        count++;
	temperature[2] = parseInt(packet[1]);
    }
    if(packet[0] == 'Z')
    {
	flags[3] = 1;
        count++;
	temperature[3] = parseInt(packet[1]);
    }
/*    if(count == 1)
    {
         for(i=0;i<1;i++)
	{
		sum = sum + temperature[i];
    		flags[i] = 0;
	}
	average = (sum).toFixed(2);
	sum = 0;
	count = 0;
    }
    if(count == 2)
    {
         for(i=0;i<2;i++)
	{
		sum = sum + temperature[i];
    		flags[i] = 0;
	}
	average = (sum/2).toFixed(2);
	sum = 0;
	count = 0;
    }
    if(count == 3)
    {
         for(i=0;i<3;i++)
	{
		sum = sum + temperature[i];
    		flags[i] = 0;
	}
	average = (sum/3).toFixed(2);
	sum = 0;
	count = 0;
    }*/
    if(count == 4)
    {
         for(i=0;i<4;i++)
	{
		sum = sum + temperature[i];
    		flags[i] = 0;
	}
	average = (sum/4).toFixed(2);
	sum = 0;
	count = 0;
    }
    io.emit("chat message", "An XBee says: " + average);
  });
});

