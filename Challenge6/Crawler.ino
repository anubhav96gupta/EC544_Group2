#include <XBee.h>
#include <SoftwareSerial.h>

XBee xbee = XBee();

SoftwareSerial xbeeSerial(2, 3);

uint8_t payload[2] = {0, 0};
uint8_t flag[4] = {0, 0, 0, 0};
uint8_t packet[4] = {0, 0, 0, 0};
int count = 0;

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
  delay(4000);
  packetrecieve();
  packetrecieve();
  packetrecieve();
  packetrecieve();
}

void packetSend()
{
  ZBTxRequest zbTx = ZBTxRequest(0xFFFF, payload, sizeof(payload));
  xbee.send(zbTx);
}

void packetrecieve()
{
  xbee.readPacket(200);
  Serial.println("inside");
  uint8_t value = 0;
  if (xbee.getResponse().isAvailable())
  {
    if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
    {
      xbee.getResponse().getZBRxResponse(rx);

      if (rx.getData(0) != 255)
      {
        switch (rx.getData(1))
        {
          case 1:
            if (flag[0] != 1)
            {
              flag[0] = 1;
              packet[0] = rx.getData(0);
              count++;
              //Serial.println("1");
            }
            break;
          case 2:
            if (flag[1] != 1)
            {
              flag[1] = 1;
              packet[1] = rx.getData(0);
              count++;
              //Serial.println("2");
            }
            break;
          case 3:
            if (flag[2] != 1)
            {
              flag[2] = 1;
              packet[2] = rx.getData(0);
              count++;
              //Serial.println("3");
            }
            break;
          case 4:
            if (flag[3] != 1)
            {
              flag[3] = 1;
              packet[3] = rx.getData(0);
              count++;
              //Serial.println("4");
            }
            break;
        }
      }
    }
  }
  if (count == 4)
  {
    //ZBTxRequest zbTx = ZBTxRequest(0xFFFF, packet, sizeof(packet));
    //xbee.send(zbTx);
    for (int i = 0; i < 4; i++)
    {
      Serial.print(i);
      Serial.print("\t");
      Serial.println(packet[i]);
      flag[i] = 0;
      count = 0;
    }
  }
}

