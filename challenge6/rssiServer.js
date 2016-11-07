var SerialPort = require("serialport");
var app = require('express')();
var xbee_api = require('xbee-api');

var csvWriter = require('csv-write-stream')
var fs = require('fs')
var writer = csvWriter()
writer.pipe(fs.createWriteStream('47.csv'));

var C = xbee_api.constants;
var XBeeAPI = new xbee_api.XBeeAPI({
  api_mode: 2
});

var c = 1;
var count = 0;
var flag = [0,0,0,0];
var portName = process.argv[2];

var sampleDelay = 2000;
var dataset = [[0, 0, 0, 0]]

//Note that with the XBeeAPI parser, the serialport's "data" event will not fire when messages are received!
portConfig = {
	baudRate: 9600,
  parser: XBeeAPI.rawParser()
};

var sp;
sp = new SerialPort.SerialPort(portName, portConfig);


//Create a packet to be sent to all other XBEE units on the PAN.
// The value of 'data' is meaningless, for now.
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


XBeeAPI.on("frame_object", function(frame) {
  if (frame.type == 144)
  {
    switch(frame.data[1])
    {
      case 1:
              if(flag[0] != 1)
              {
                dataset[0][0] = frame.data[0];
                count++;
              }
              break;
      case 2:
              if(flag[1] != 1)
              {
                dataset[0][1] = frame.data[0];
                count++;
              }
              break;
      case 3:
              if(flag[2] != 1)
              {
                dataset[0][2] = frame.data[0];
                count++;
              }     
              break;           
      case 4:
              if(flag[3] != 1)
              {
                dataset[0][3] = frame.data[0];
                count++;
              }
              break;
    }
    if(count == 4)
    {
      console.log(c + " " + dataset);
      writer.write({data: dataset});
      flag = [0,0,0,0];
      count = 0;
      c++
    }
  }
});