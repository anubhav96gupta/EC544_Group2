var SerialPort = require("serialport");
var app = require('express')();
var xbee_api = require('xbee-api');
var KNN = require('ml-knn');
var fs = require('fs');

<<<<<<< HEAD
var express = require('express');
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.use('/fonts', express.static(__dirname + '/frontend/fonts'));
=======
var express = require('express')();
var http = requre('http').Server(app);
var io = require('socket.io')(http);


app.use('/fonts', express.static(__dirname + 'frontend/fonts'));
>>>>>>> 930f27ca483c553e04c827514f03060a377e8ac1
app.use('/images', express.static(__dirname + '/frontend/images'));
app.use('/', express.static(__dirname + '/frontend'));


app.get('/localization', function(req, res){
  res.sendfile('frontend/index.html');
});

<<<<<<< HEAD
var c1 = 0,  c2 = 0, c3 = 0,c4 = 0;
=======

var fs = require('fs')
>>>>>>> 930f27ca483c553e04c827514f03060a377e8ac1
var C = xbee_api.constants;
var XBeeAPI = new xbee_api.XBeeAPI({
  api_mode: 2
});

var count = 0;
var flag = [0,0,0,0];
var portName = process.argv[2];
var knn = new KNN();

var sampleDelay = 2000;
var dataset = [[0, 0, 0, 0]];

portConfig = {
  baudRate: 9600,
  parser: XBeeAPI.rawParser()
};

io.on('connection', function(socket){
  console.log('a user connected');
  socket.on('disconnect', function(){
  });
});
<<<<<<< HEAD
=======

http.listen(4000, function(){
  //listen on localhost port 4000
  console.log('listening on *:4000');
});

var train = [[45,40,67.6,74],
[49.2,40.8,69.8,74.6],
[46.8,43.8,65.6,74],
[47.2,40.4,69.6,76],
[54.4,52.8,65.8,77.8],
[52.8,47.6,64,77.8],
[56.4,42.8,62,74.2],
[53.8,45,63.2,74.6],
[56.4,48.8,62.2,73.4],
[57,49.2,64,74.8],
[53.4,52.2,60.8,78.2],
[54.2,51.2,62,74.8],
[56.8,55,59.6,73.2],
[57.4,45.4,66.4,75.4],
[58.2,45.6,61.2,75.8],
[55.4,49,64.2,75.2],
[57.8,58.4,59.4,72],
[58.2,50,62.2,73.4],
[61.6,54,61.4,72.4],
[59.6,52.4,61.6,76.2],
[66.2,57.2,61,76.2],
[60.6,55.8,60,72.2],
[62.4,55.6,64.4,75.8],
[60.2,56.6,58.4,76.4],
[67.2,60,62.2,73.8],
[67.8,58,62.4,72.6],
[64,59.2,61.8,69.8],
[63,58.4,64,74],
[69.8,63.4,58,69],
[67.4,57.8,60.4,72.6],
[62.8,58.2,61.6,70.8],
[67.4,63.6,58.2,73],
[67.6,61.4,57.6,66.8],
[67.6,60.8,58,71.4],
[61.2,60.6,59.2,69.2],
[68,63,54.2,72.6],
[71.4,61.2,52.6,66.8],
[65.6,61.6,57.8,72.4],
[67.2,62.6,57.2,69.2],
[68,64.4,51,65.6],
[69.2,64.8,52.4,73.2],
[67.2,61.4,56.6,68.4],
[71.6,66,53.8,71],
[76.6,65.4,51.2,69],
[72.8,68,52,64.2],
[72.6,58.6,55.4,67.6],
[70,65.2,54.6,67.8],
[70.8,63.2,56.4,64],
[76.4,70.6,53,65.2],
[68,59.8,51.8,68.6],
[68.8,62.8,51,61],
[70.2,65.6,53,67.4],
[72.8,65.8,45,61.2],
[67.8,58.4,55.8,62.8],
[73.4,66.8,53,58.2],
[71.6,60,53.8,63.2],
[75.2,68,47.8,64],
[71.6,60.8,50.4,61.2],
[72.6,68.6,52,62.4],
[70.6,64.8,51.6,59.2],
[72.8,69,45.8,55.2],
[69,62,46.4,59.2],
[70.4,63.2,47,60],
[73.4,66.4,42.8,58.8],
[73,63.8,45.2,62],
[74,62.4,41.2,57],
[71.6,63.2,44,60.8],
[75.8,63.4,42,60],
[73,68.4,39,48.8],
[70,70.4,39,56],
[77.8,72.6,40.2,51],
[78.8,72.2,37.2,50.4],
[79,72.4,40.2,45.4],
[73.2,68.4,36.6,55.6],
[72.6,64.8,40.6,52.2],
[76.2,70.2,40.2,46.4],
[79.8,77.6,48.6,46],
[80.2,74,43.8,45.6],
[77.8,72.6,43.4,48.8],
[77.2,77.6,43.6,49.2],
[80,76.8,45.6,48.2],
[77.2,77.6,44.4,48],
[78.2,78,44.2,50.4],
[83.4,78.6,50,44.6],
[79,76.6,49.4,42],
[79,78.8,46,45.4],
[80,78.6,48.4,46],
[80.4,79.8,47.4,46.4],
[78.8,82.8,46.8,43.8],
[81,78,47.2,39.2],
[80.8,79.6,50.2,49.4],
[79.4,82.4,50,40.2],
[76.6,80.8,51.2,44.2],
[75.2,78.6,50.2,39.6],
[76.6,78.4,50.4,41.4],
[73.2,78.8,51.6,43.4],
[70,71.8,50.6,37],
[65.4,75.2,51,37.4],
[67,70,49.4,40.4],
[68.6,70.8,47,41.8],
[64.4,72.6,52.2,39.6],
[68.8,71.6,55.6,38],
[63.8,68.4,52.2,41],
[62.2,71.4,56.6,44.2],
[62.4,71.2,63.4,45.6],
[66.4,71,58,46.4],
[61.8,68,62.4,43.6],
[66.8,70.8,59,45.8],
[68.8,71.6,60.6,49.6],
[67,75.2,62.4,48.8],
[64.4,69.2,59.8,47],
[64.4,69.2,59.8,47],
[66.6,66.8,61.6,48.6],
[65.4,69.2,60.4,47.6],
[65.6,68.4,64.6,51.4],
[65.6,66.8,63.2,49.8],
[62.8,68.4,62.4,56.4],
[57.2,69.4,60.6,49.8],
[60.6,69.2,67,51],
[63.4,67.4,62.8,57],
[64.6,65,69.6,53.4],
[65.6,76.2,65.2,54],
[61.4,70.8,66.6,53.2],
[65,62.2,70.8,56.4],
[55.8,65.8,69.2,53.2],
[63.8,66.6,65.8,58],
[64.2,71.6,69,55.2],
[61.6,72.8,69.4,57.4],
[55.8,65.8,69.2,53.2],
[63.8,66.6,65.8,58],
[64.2,71.6,69,55.2],
[61.6,72.8,69.4,57.4],
[56.8,65.6,71.8,61.2],
[63.2,74,68.6,61.8],
[69.4,66.8,73.4,59],
[61.8,63.8,68.8,60.2],
[59.4,71.2,67.6,63.2],
[58.6,63.8,73,58],
[61,65,70.8,58.8],
[67.4,66.8,70,57.4],
[57.4,66.2,76.6,63.6],
[60.2,61.8,73.4,61.2],
[58.2,60.2,72.2,67.4],
[62.2,61.8,74.8,61],
[54,66.4,73.6,64.4],
[56.6,61.8,72.4,59.4],
[59.4,65,71.8,64.6],
[54,67.8,73,67.6],
[56.4,65,76.2,64.2],
[56.2,61.8,76.8,64.6],
[55.8,62.4,76.2,64.8],
[61.6,62,74.2,70.6],
[57,59.4,77.2,62.4],
[49.8,59.6,77,63.2],
[57.6,59.2,71.4,60.2],
[54.4,57,76.6,63.6],
[51.4,57,75.4,63.8],
[54.4,58.2,74.6,60.2],
[53.6,57.4,75,65.6],
[46.6,55.2,73.6,68.2],
[50.2,53.2,74.2,64.4],
[50.4,54.2,73.2,65.6],
[50.4,54.2,73.6,64.2],
[50.8,52.4,76.2,64],
[48,51.6,77.6,63.2],
[44.4,52.4,74.4,66.4],
[48.6,48.2,72.8,64.2],
[49,53,74.8,64.6],
[48.2,44.8,76.8,61.6],
[43.6,47.4,73,65],
[44.2,53,78.4,61.8],
[46.2,46.2,74.2,66.4],
[38,42,70.8,68.2],
[45.2,45.6,73.8,62.2],
[40.8,46.6,68.8,64.2],
[41.2,40.8,73.2,59.6],
[44.2,40.2,73.2,72.8],
[41.6,49.6,76.4,74],
[39,43.6,77.6,68],
[41.8,40.2,71,68.8],
[45.4,41.8,70.2,73.8],
[43.2,43.4,74.2,73.6],
[39.8,40.6,74.8,70.6],
[42.4,39.6,69,76.8],
[48,43.6,74,77.8],
[41,39.2,68.8,75],
[40.6,42.6,70.6,72],
[46.4,41,65.2,73.2],
[42.2,43.6,69.6,75.8],
[44.4,37.6,72.8,77.4],
[40.6,40,67.8,75.2],
[49.6,46.4,66,75.8]];
>>>>>>> 930f27ca483c553e04c827514f03060a377e8ac1

http.listen(4000, function(){
  console.log('listening on *:4000');
});

var train = [[54.3,43.9,74.2,93.4],
[52.5,38.5,75,93.5],
[45.4,45.1,65.5,80.5],
[43.1,43.3,63.5,86.7],
[51.7,40.8,71.8,94.4],
[53.1,37.6,70.2,88.9],
[51.1,52.9,66.1,84.3],
[47.4,47.2,69.5,86.6],
[54.1,53.3,59.4,84.1],
[49.9,47.9,58.3,88.5],
[51.5,52.5,59.7,76.0],
[45,48.1,69.6,81.3],
[55.7,53,66.2,80.9],
[48.7,55.8,65.5,91.8],
[54.5,57.3,61.7,83.0],
[57.7,52.9,65.6,91.4],
[60.6,57.5,65.4,89.0],
[56.7,59.9,69.5,90.5],
[59.7,54.9,66.5,90.3],
[65.3,63.8,61,86.6],
[62.3,56.2,63.1,79.6],
[57.9,59.7,58.1,89.1],
[64,61.2,62,83.0],
[66.4,62.5,66.9,74.5],
[67.2,65.5,59.5,84.8],
[64.2,58.1,63.8,81.7],
[69.1,59.7,65,86.8],
[72,57.8,59.5,76.5],
[69.5,62.9,66.6,72.5],
[67.7,65.6,62.3,85.3],
[67.8,67.5,64.8,77.9],
[74.2,68.9,60.3,79.6],
[69.2,67,63.8,83.6],
[70,68.8,56.7,78.7],
[67,68.9,68.5,75.0],
[74.4,70.5,65.3,75.0],
[71,68.7,64.8,73.4],
[76.1,68.4,63.2,69.2],
[73.6,69.2,55.2,66.1],
[73.6,68,56.8,76.8],
[71.2,62.7,55.1,70.3],
[74,72.1,57.9,71.3],
[73.7,69.7,60.1,70.7],
[75.4,72.2,56,69.9],
[75.5,69.7,56.7,72.1],
[74.2,72.1,51.6,66.2],
[74,67.8,51.4,74.6],
[75.7,68.5,51.6,66.3],
[73.6,72.5,44.2,66.3],
[78,72,47.5,68.6],
[75.7,71.8,49.6,71.0],
[77.1,71,52.1,64.1],
[79,69.5,48.3,64.6],
[82.7,76.8,41.1,60.3],
[73.6,73.8,44,68.1],
[75.8,74.8,42.1,60.5],
[77.1,68.8,38.3,51.3],
[76.9,67,41.6,55.3],
[74.6,70.5,42.4,50.2],
[75.6,72.7,46.6,64.6],
[80.5,77.2,43.8,60.0],
[83.4,76.4,46.4,67.0],
[81,80.2,47.1,51.1],
[80.4,78.4,51.4,56.3],
[79.9,81.4,47,64.2],
[82.1,76.5,49.7,53.8],
[77.6,82.1,51.3,58.6],
[78.4,81.4,57.2,55.1],
[78.9,83.2,58.1,51.0],
[75.6,82,53.6,68.7],
[73.2,82.2,54.2,45.3],
[76,80.7,55.5,51.6],
[73.9,83.1,57.3,48.2],
[71.8,80.1,57.9,61.5],
[72,83.7,59.5,48.0],
[73.7,80.6,57.1,49.3],
[72.8,78.4,56.5,44.6],
[75.1,78.6,53.9,48.0],
[71.4,76.2,55.4,47.2],
[68.5,76.1,45.2,53.9],
[66.4,75.9,49.3,50.2],
[64.1,75.7,49.6,43.4],
[65.9,75,51.3,50.3],
[63.7,77.5,45.3,51.8],
[69.4,70.9,60.3,56.7],
[72,77.9,63.1,56.6],
[69.6,75.8,60.9,48.4],
[65.7,79.9,61.4,52.1],
[69.9,69.7,62.5,67.7],
[62.9,72.7,66.9,66.2],
[68.2,70.3,61.9,63.0],
[67.6,78.2,65.6,54.9],
[67,75.2,65.1,64.0],
[69.4,76.5,66.4,69.4],
[67.4,69.9,73.4,60.8],
[64.5,70.8,70.2,74.4],
[62,70.5,74.4,63.8],
[67.9,76.1,71.6,68.0],
[64.9,65.7,73.6,75.9],
[64.4,69.4,74.2,73.5],
[63.4,66.4,72.2,69.7],
[65.3,66.4,73.7,69.4],
[59.3,64.1,76.9,65.5],
[62.6,65.8,73.5,72.4],
[65.9,67.5,75.7,72.7],
[65.7,72,74.2,76.6],
[61.7,69.4,78.6,71.4],
[62.1,70.3,75.5,69.7],
[65.2,70.9,77.2,72.2],
[58.6,72.2,79.5,78.1],
[58.6,68.3,79.3,74.1],
[59.5,63.9,81,74.2],
[64,64.6,79.5,74.3],
[59.6,63.5,81.4,78.5],
[53.2,63.9,77.8,73.2],
[54.2,63,77.1,80.7],
[48.9,63,79.5,77.6],
[52.1,63.6,84,76.5],
[57.5,67.4,76.5,84.5],
[55.3,56.1,80.9,80.7],
[54.7,64.5,76,77.7],
[53.2,61.9,81.5,90.2],
[47.8,57.5,80.5,85.2],
[48,57.8,80.4,88.4],
[61.4,50.6,76.4,89.9],
[44.6,50.9,82,84.6],
[50,52.2,79.4,75.8],
[42.4,53.8,79.1,82.2],
[44.1,50.3,79.2,76.3],
[42.5,44.2,76.1,84.4],
[40.3,55.7,79.7,83.4],
[50.2,54.3,80.9,79.7],
[41.2,42.5,79.8,82.1],
[49.5,44.3,81.4,81.4],
[44.6,43.7,77.5,70.9],
[42.6,48.1,75,80.3],
[37.9,44.5,79.2,76.5],
[41.1,41.3,78.1,78.6],
[41.4,44.5,79.4,80.9],
[44.1,43.3,75.8,83.0],
[45.7,43.3,78.4,73.5],
[45.8,42.1,75.9,77.7],
[45.1,43.2,77.9,87.1],
[50.8,45.1,81.2,91.3],
[43.3,50.8,78,90.3],
[44.5,41.1,76.6,84.4],
[48.8,43.4,71.1,90.7],
[51,42.3,72.6,92.7],
[48.7,45.6,73.9,84.6],
[42.7,44.3,72.9,87.7],
[54.3,43.9,74.2,93.4],
[52.5,38.5,75,93.5],
[45.4,45.1,65.5,80.5],
[43.1,43.3,63.5,86.7],
[51.7,40.8,71.8,94.4],
[53.1,37.6,70.2,88.9]];

var prediction = [1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,
                  8,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,13,13,
                  13,13,13,13,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,17,17,17,17,17,17,
                  18,18,18,18,18,18,19,19,19,19,19,19,20,20,20,20,20,20,21,21,21,21,21,21,22,22,22,22,
                  22,22,23,23,23,23,23,23,24,24,24,24,24,24,25,25,25,25,25,25,26,26,26,26,26,26];

//knn.k = 27;
knn.train(train, prediction);
var sp;
sp = new SerialPort.SerialPort(portName, portConfig);

var RSSIRequestPacket = {
  type: C.FRAME_TYPE.ZIGBEE_TRANSMIT_REQUEST,
  destination64: "000000000000ffff",
  broadcastRadius: 0x01,
  options: 0x00,
  data: "test"
}

var requestRSSI = function(){
  sp.write(XBeeAPI.buildFrame(RSSIRequestPacket));
}

sp.on("open", function () {
  console.log('open');
  requestRSSI();
  setInterval(requestRSSI, sampleDelay);
});


XBeeAPI.on("frame_object", function(frame)
{
  if (frame.type == 144)
  {
    switch(frame.data[1])
    {
      case 1:
              if(flag[0] != 1)
              {
                dataset[0][0] += frame.data[0];
                c1++;
                if(c1 == 2)
                {
                  dataset[0][0] = dataset[0][0]/2;
                  count++
                  flag[0] = 1;
                  c1 = 0;
                }
              }
              break;
      case 2:
              if(flag[1] != 1)
              {
                dataset[0][1] += frame.data[0];
                c2++;
                if(c2 == 2)
                {
                  dataset[0][1] = dataset[0][1]/2;
                  count++
                  flag[1] = 1;
                  c2 = 0;
                }
              }
              break;
      case 3:
              if(flag[2] != 1)
              {
                dataset[0][2] += frame.data[0];
                c3++;
                if(c3 == 2)
                {
                  dataset[0][2] = dataset[0][2]/2;
                  count++
                  flag[2] = 1;
                  c3 = 0;
                }
              }     
              break;           
      case 4:
              if(flag[3] != 1)
              {
                dataset[0][3] += frame.data[0];
                c4++;
                if(c4 == 2)
                {
                  dataset[0][3] = dataset[0][3]/2;
                  count++
                  flag[3] = 1;
                  c4 = 0;
                }
              }
              break;
    }
    if(count == 4)
    {
      console.log(dataset);
      var ans = knn.predict(dataset);
      console.log(ans);
      if(ans!= -1)
      {
        io.emit('location', ans);
      }
      dataset = [[0, 0, 0, 0]];
      flag = [0,0,0,0];
      count = 0;
    }
  }
});