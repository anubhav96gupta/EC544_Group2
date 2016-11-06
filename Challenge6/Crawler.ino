#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();

SoftwareSerial xbeeSerial(2,3);

uint8_t payload[4] = {'T','E','S','T'};

ZBRxResponse rx = ZBRxResponse();
ZBTxStatusResponse txStatus = ZBTxStatusResponse();

void setup()
{
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  xbee.setSerial(xbeeSerial);
}

void loop()
{       
    packetSend();
    packetrecieve();
}

void packetSend()
{ 
  ZBTxRequest zbTx = ZBTxRequest(0xFFFF, payload, sizeof(payload));
  xbee.send(zbTx);
}

void packetrecieve()
{
  xbee.readPacket(500);
  uint8_t value = 0;
  if (xbee.getResponse().isAvailable())
  {          
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
      {
        xbee.getResponse().getZBRxResponse(rx);
        for (int i = 0; i < rx.getDataLength(); i++)
          Serial.println( rx.getData(i) );
      }
  }
}

