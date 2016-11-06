#include <XBee.h>
#include <SoftwareSerial.h>

uint8_t BEACON_ID = 1;

XBee xbee = XBee();
XBeeResponse response = XBeeResponse();

// create reusable response objects for responses we expect to handle 
ZBRxResponse rx = ZBRxResponse();
ModemStatusResponse msr = ModemStatusResponse();

uint8_t dbCommand[] = {'D','B'};
AtCommandRequest atRequest = AtCommandRequest(dbCommand);
ZBTxStatusResponse txStatus = ZBTxStatusResponse();
AtCommandResponse atResponse = AtCommandResponse();

SoftwareSerial xbeeSerial(2,3);

void setup()
{
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  xbee.setSerial(xbeeSerial);
}

//SEND THE PACKET TO THE DESTINATION ADDRESS WITH ACKNOWLEDGEMENT
void sendTx(ZBTxRequest zbTx)
{
  xbee.send(zbTx);
  if (xbee.readPacket(10))
  {
    if (xbee.getResponse().getApiId() == ZB_TX_STATUS_RESPONSE)
      xbee.getResponse().getZBTxStatusResponse(txStatus);
  } 
  else
  if (xbee.getResponse().isError())
    Serial.print("Error reading packet.  Error code: ");  
}

//COMBINE RSSI AND BEACON_ID PACKET
void sendRSSIValue(XBeeAddress64 targetAddress, int rssiValue)
{
  uint8_t value = (uint8_t) rssiValue;
  uint8_t values[] = {value, BEACON_ID};
  Serial.println(value);
  ZBTxRequest zbTx = ZBTxRequest(targetAddress, values, sizeof(values));
  sendTx(zbTx);
}


// GET RSSI VALUE
int sendATCommand(AtCommandRequest atRequest)
{
  int value = -1;
  xbee.send(atRequest);
  if (xbee.readPacket(10))
  {
    if (xbee.getResponse().getApiId() == AT_COMMAND_RESPONSE)
    {
      xbee.getResponse().getAtCommandResponse(atResponse);
      if (atResponse.isOk())
      {
        if (atResponse.getValueLength() > 0)
        {
          for (int i = 0; i < atResponse.getValueLength(); i++)
            value = atResponse.getValue()[i];
        }
      }
    }
  } 
  return value;
}
                                    

//CHECK IF ZIGBEE AND DATA IS AVAILABLE
void processResponse()
{
  if (xbee.getResponse().isAvailable())
  {
      if (xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
      {
        xbee.getResponse().getZBRxResponse(rx);
        XBeeAddress64 replyAddress = rx.getRemoteAddress64();
        int rssi = sendATCommand(dbCommand);
        sendRSSIValue(replyAddress, rssi);
      }
  } 
}

void loop()
{
    xbee.readPacket();
    processResponse();
}
