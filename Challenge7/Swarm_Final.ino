#include <XBee.h>
#include <SoftwareSerial.h>

/******************COMMANDS EXCHANGED BETWEEN NODES***********************/
const uint8_t ELECTION = 0xA0, ACK = 0xA1, WIN = 0xA2, INFECTION = 0xA3, CLEAR = 0xA4, DISCOVERY = 0xA5, LIVE = 0xA6;

/***********************PIN ASSIGNMENTS AND DEFAULT TIMINGS FOR THE DIFFERENT PROCESSES**********************/
#define BLUE_PIN              6                  // LEADER STATUS PIN
#define RED_PIN               4                  // INFECTED NODE PIN
#define GREEN_PIN             5                  // NON-INFECTED STATUS PIN
#define BUTTON_PIN            8                  // BUTTON TO INFECT OR CLEAR NODES
#define ELECTION_REPLY_WAIT_PERIOD      1000
#define WINING_WAIT_PERIOD              3000
#define ELECTION_BETWEEN_WAIT_PERIOD    5000
#define LEADER_LIVE                     6000
#define IMMUNITY_PERIOD                 3000
#define INF_REBROADCAST                 4000

const uint8_t slCommand[] = {'S', 'L'};
XBee xbee = XBee();
SoftwareSerial xbeeSerial(2, 3);
ZBRxResponse rxResponse = ZBRxResponse();
ZBTxRequest txRequest;
AtCommandRequest atRequest = AtCommandRequest((uint8_t*)slCommand);
AtCommandResponse atResponse;
uint32_t ownAddr, leaderAddr, remoteAddr, electionTimeout, LeaderLiveTimeout, betweenElectionTimeout, immunityTimeout, infectionRebroadcastTimeout;
uint32_t Addr_Array[10];                             // SCALING THE NETWORK
uint8_t Devices = 0 , BeatLost = 0;
bool isInfected = false, isElecting = false, isAcknowledged = false;
int Button_State = LOW, Previous_Button_State = HIGH, Switch_Timestamp = 0, Debounce_Delay = 50;

/*****************************SETUP FUNCTION*************************/
void setup()
{
  Serial.begin(9600);
  xbeeSerial.begin(9600);
  xbee.begin(xbeeSerial);
  delay(2000);

  PINS_INIT();
  SelfAddr();
  leaderAddr = ownAddr;
  SendPacket(0x0000FFFF, (uint8_t*)&DISCOVERY, 1);
  LeaderLiveTimeout = millis() + LEADER_LIVE;
}

/**********************INITIALIZE LEDS*************************/
void PINS_INIT(void)
{
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);

  digitalWrite(BLUE_PIN, HIGH);
  digitalWrite(RED_PIN, LOW);
  digitalWrite(GREEN_PIN, HIGH);
}

/*************************SET LED STATUS***********************/
void LED_STATUS(void)
{
  if (ownAddr == leaderAddr)
  {
    digitalWrite(BLUE_PIN, HIGH);
    digitalWrite(GREEN_PIN, LOW);
    digitalWrite(RED_PIN, LOW);
  } 
  else
  {
    digitalWrite(BLUE_PIN, LOW);
    if (isInfected)
    {
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(RED_PIN, HIGH);
    } 
    else
    {
      digitalWrite(GREEN_PIN, HIGH);
      digitalWrite(RED_PIN, LOW);
    }
  }
}

/**********************FUNCTION TO GET ITS OWN ADDRESS******************/
void SelfAddr(void)
{
  do
  {
    do    
      xbee.send(atRequest);
    while (!xbee.readPacket(5000) || xbee.getResponse().getApiId() != AT_COMMAND_RESPONSE);

    xbee.getResponse().getAtCommandResponse(atResponse);
  }
  while (!atResponse.isOk());
  for (int i = 0; i < 4; i++)
  {
    uint32_t tempVal = atResponse.getValue()[i];
    ownAddr |= tempVal << 8 * (3 - i);
  }
}

/*****************************SEND FUNCTION********************************/
void SendPacket(uint32_t DestnAddr, uint8_t* payload, uint8_t PayloadLength)
{
  if (payload[0] == DISCOVERY || payload[0] == WIN)
  {
    txRequest = ZBTxRequest(XBeeAddress64(0x00000000, 0x0000FFFF), payload, PayloadLength);
    xbee.send(txRequest);
  }
  else
  {
    if (DestnAddr == 0x0000FFFF)
    {
      for (int i = 0; i < Devices; i++)
      {
        txRequest = ZBTxRequest(XBeeAddress64(0x0013A200, Addr_Array[i]), payload, PayloadLength);
        xbee.send(txRequest);
      }
    }
    else
    {
      txRequest = ZBTxRequest(XBeeAddress64(0x0013A200, DestnAddr), payload, PayloadLength);
      xbee.send(txRequest);
    }
  }
}


/*************************START THE ELECTION**********************/
void BeginElection(void)
{
  if (isElecting)  return;
  if (millis() < betweenElectionTimeout) return;
  else isElecting = true;
  isAcknowledged = false;
  uint8_t countDevices = 0;
  for (int i = 0; i < Devices; i++)
  {
    if (Addr_Array[i] > ownAddr)
    {
      SendPacket(Addr_Array[i], (uint8_t*) &ELECTION, 1);
      countDevices++;
    }
  }
  if (countDevices > 0) electionTimeout = millis() + ELECTION_REPLY_WAIT_PERIOD;
  else electionTimeout = millis();
}

/*******************READ FUNCTION**********************/
void ReadPacket(void)
{
  if (xbee.readPacket(1) && xbee.getResponse().getApiId() == ZB_RX_RESPONSE)
  {
    xbee.getResponse().getZBRxResponse(rxResponse);
    remoteAddr = rxResponse.getremoteAddr().getLsb();

    bool Addr_Present = false;
    for (int i = 0; i < Devices; i++)
      if (Addr_Array[i] == remoteAddr)
        Addr_Present = true;
    if (!Addr_Present) Addr_Array[Devices++] = remoteAddr;
    switch (rxResponse.getData(0))
    {
      case DISCOVERY:
        if (rxResponse.getDataPayloadLength() > 1)
        {
          memcpy(&leaderAddr, rxResponse.getData() + 1, sizeof(leaderAddr));
          if (leaderAddr < ownAddr) BeginElection();
        }
        else
        {
          uint8_t PayLoad[5];
          PayLoad[0] = DISCOVERY;
          memcpy(PayLoad + 4, &leaderAddr, sizeof(leaderAddr));
          SendPacket(remoteAddr, PayLoad, 5);
        }
        break;

      case ELECTION:
        SendPacket(remoteAddr, (uint8_t*)&ACK, 1);
        BeginElection();
        break;

      case ACK:
        electionTimeout = millis() + WINING_WAIT_PERIOD;
        isAcknowledged = true;
        break;

      case WIN:
        if (remoteAddr > ownAddr)
        {
          leaderAddr = remoteAddr;
          isElecting = false;
          LeaderLiveTimeout = millis() + LEADER_LIVE;
          betweenElectionTimeout = millis() + ELECTION_BETWEEN_WAIT_PERIOD;
        }
        else
          BeginElection();
        break;

      case LIVE:
        if (ownAddr == leaderAddr)
        {
          if (remoteAddr > ownAddr)
          {
            leaderAddr = remoteAddr;
            LeaderLiveTimeout = millis() + LEADER_LIVE;
          }
        }
        else
          LeaderLiveTimeout = millis() + LEADER_LIVE;
        break;

      case INFECTION:
        if (millis() > immunityTimeout && leaderAddr != ownAddr)
          isInfected = true;
        break;

      case CLEAR:
        isInfected = false;
        LED_STATUS();
        immunityTimeout = millis() + IMMUNITY_PERIOD;
        delay(IMMUNITY_PERIOD);
        break;
    }
  }
}

/********************************MAIN FUNCTION***************************/
void loop()
{
  int reading = digitalRead(BUTTON_PIN);
  if (reading != Previous_Button_State)
    Switch_Timestamp = millis();
  if (millis() - Switch_Timestamp > Debounce_Delay)
  {
    if (reading != Button_State)
    {
      Button_State = reading;
      if (Button_State == LOW)
      {
        if (leaderAddr == ownAddr)
        {
          SendPacket(0x0000FFFF, (uint8_t*)&CLEAR, 1);
          isInfected = false;
        }
        else isInfected = true;
      }
    }
  }
  Previous_Button_State = reading;

  LED_STATUS();
  ReadPacket();
  if (isElecting && millis() > electionTimeout)
  {
    isElecting = false;
    LeaderLiveTimeout = millis() + LEADER_LIVE;
    betweenElectionTimeout = millis() + ELECTION_BETWEEN_WAIT_PERIOD;
    if (isAcknowledged)
      BeginElection();
    else
    {
      SendPacket(0x0000FFFF, (uint8_t*)&WIN, 1);
      leaderAddr = ownAddr;
    }
  }
  if (!isElecting)
  {
    if (millis() > LeaderLiveTimeout)
    {
      if (leaderAddr == ownAddr)
      {
        SendPacket(0x0000FFFF, (uint8_t*) &LIVEs, 1);
        LeaderLiveTimeout = millis() + LEADER_LIVE / 3;
      }
      else
      {
        BeginElection();
      }
    }
    if (leaderAddr != ownAddr && isInfected && millis() > infectionRebroadcastTimeout)
    {
      SendPacket(0x0000FFFF, (uint8_t*) &INFECTION, 1);
      infectionRebroadcastTimeout = millis() + INF_REBROADCAST;
    }
  }
}