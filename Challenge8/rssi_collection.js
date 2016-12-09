var SerialPort = require("serialport");
var app = require('express')();
var xbee_api = require('xbee-api');
var KNN = require('ml-knn');
var csvWriter = require('csv-write-stream')
var fs = require('fs')


var writer = csvWriter()
writer.pipe(fs.createWriteStream('23.csv'))


var C = xbee_api.constants;
var XBeeAPI = new xbee_api.XBeeAPI({
  api_mode: 2
});

var count = 0;
var c1 = 0, c2 = 0, c3 = 0, c4 = 0;
var flag = [0,0,0,0];
var portName = process.argv[2];
var knn = new KNN();

var sampleDelay = 2000;
var dataset = [[0, 0, 0, 0]];

portConfig = {
  baudRate: 9600,
  parser: XBeeAPI.rawParser()
};

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


XBeeAPI.on("frame_object", function(frame) {
  if (frame.type == 144)
  {
    switch(frame.data[1])
    {
      case 1:
              if(flag[0] != 1)
              {
                dataset[0][0] += frame.data[0];
                c1++;
                if(c1  == 10)
                {
                  dataset[0][0] = dataset[0][0]/10;
                  c1 =  0;
                  flag[0] = 1;
                  count++;
                }
              }
              break;
      case 2:
              if(flag[1] != 1)
              {
                dataset[0][1] += frame.data[0];
                c2++;
                if(c2  == 10)
                {
                  dataset[0][1] = dataset[0][1]/10;
                  c2 =  0;
                  flag[1] = 1;
                  count++;
                }
              }
              break;
      case 3:
              if(flag[2] != 1)
              {
                dataset[0][2] += frame.data[0];
                c3++;
                if(c3  == 10)
                {
                  dataset[0][2] = dataset[0][2]/10;
                  c3 =  0;
                  flag[2] = 1;
                  count++;
                }
              }     
              break;           
      case 4:
              if(flag[3] != 1)
              {
                dataset[0][3] += frame.data[0];
                c4++;
                if(c4  == 10)
                {
                  dataset[0][3] = dataset[0][3]/10;
                  c4 =  0;
                  flag[3] = 1;
                  count++;
                }
              }
              break;
    }//writer.write({data: data});
    if(count == 4)
    {
      console.log(dataset);
      writer.write({data:'['+dataset+'],'});
      dataset = [[0, 0, 0, 0]];

      flag = [0,0,0,0];
      count = 0;
    }
  }
});