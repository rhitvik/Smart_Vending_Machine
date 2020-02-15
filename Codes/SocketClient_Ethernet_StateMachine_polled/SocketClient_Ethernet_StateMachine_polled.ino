#include <SPI.h>
#include <Ethernet.h>

// Client Lib by Pablo
#include <WebSocketClient.h>
 
EthernetClient client;
WebSocketClient webSocketClient;

// Arduino's mac address
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFD, 0xEF };

// Constants
const char* server = "34.213.220.255";//api.servester.com
const char* path = "/";
const int port = 4500;
int loopDelay = 1000;

// data formatted for communication with server
const String echoString = String("E echo_test");
// payload is machine ID
const String registerString = String("R 4");

// SYNC variables
volatile boolean online = false;//set false in our code
volatile bool hitchhike = false;
boolean waitingForResponse = true;
boolean machineRegistered = false;

/**
   parameter to cck arduino that vm system ready

   IDLE : 0
   PIN INPUT : 1
   ACK : 2

*/
volatile byte startReading = 0;
bool exitParameter = false;//loop exit parameter
int ticktick = 0;//for OTP counting and filling
bool allowRead = false;//to stem multiple read and switch iops
volatile bool dispatch_successful = false;
volatile unsigned char mr_ISR = 0;
unsigned char OTP[5];

bool OTP_read = 0;

// BUS
const byte Bus_Pin1 = 9;
const byte Bus_Pin2 = 8;
const byte Bus_Pin3 = 7;
const byte Bus_Pin4 = 6;
unsigned char Bus_Val = 0;

// PINS
int pinState1 = 0;
int pinState2 = 0;
int pinState3 = 0;
int pinState4 = 0;

int clockPinState = 0;
const byte clockPin = 5;
const byte interruptPin = 3;
const byte micro_ack = 2;

const byte ledPin = 13;

volatile byte state = LOW;
volatile unsigned char value = 0;

// the shelf no
unsigned char product_no = 0;
int orderId = 0;
/**
   function to compare 2 strings TESTED
*/
boolean isEqual(const char* s1, const char* s2) {
  byte len1 = strlen(s1);
  byte len2 = strlen(s2);

  if (len1 != len2)
    return false;

  for (byte i = 0; i < len1; i++) {
    if (s1[i] != s2[i])
      return false;
  }
  //  Serial.println("EQ");
  return true;
}

/**
   Called when dispatch request is received
*/
void dispatch(int shelf, int quant) {
  Serial.println("DISPATCH LOGIC!");

}
/**
 * Initialize LAN
 */
void init_lan() {
  //Serial.println(F("Websocketclient starting"));
 // Serial.println("Attempting to obtain a DHCP lease");
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
   // Serial.println("Failed to configure Ethernet using DHCP");
    while(Ethernet.begin(mac) == 0) {
   //   Serial.println("Attempting to obtain a DHCP lease again");
      delay(1000);
    }
  }
  Serial.println(Ethernet.localIP());
}

/**
 * Initialize websocket connection with server
 */
void init_connection() {
  // Connect to the websocket server
  if (client.connect(server, port)) {
    // Great!
  } else {
    Serial.println("Connection failed.");

    // retry logic
    while (!client.connect(server, port)) {
      // Retry on failure
      Serial.println("Retrying");
      delay(1000);
    }
  }
  Serial.println("Connected");
  // Handshake with the server
  webSocketClient.setPath(path);
  webSocketClient.setHost(server);

  if (webSocketClient.handshake(client)) {
    // Great!
  } else {
    Serial.println("Handshake failed.");
    // Retry on failure
    while (!webSocketClient.handshake(client)) {
      Serial.println("Retrying");
      delay(5000);
    }
  }
  Serial.println("Handshake successful");
}

void reset_Ethernet() {
    
  client.flush();
  client.stop();
  init_lan();
  reset_env();
  init_connection();
}
/**
 * reset globals
 */
void reset_env() {
  waitingForResponse = true;///////////////////TRUE IT
  machineRegistered = false;
//  i = 0;
}
/**
   START
   local communication code
*/

void getPassword(void) { //for retrieving entered otp
  SETlistenMode();
  Serial.println("in getPassword function");
  //bus will be sampled and read
  //ard pins will source and capture logic
  // to replace startReading locally
  while (8) {
    if (exitParameter == true) {
      break;
    }
    else
    if (hitchhike == true) { //if isr is triggered
      hitchhike = false;
       Serial.println("hitchhike TRUE");
      digitalWrite(micro_ack, LOW);
      // Serial.println("entering loop");

      // open loop data transfer
      while (7) {
        clockPinState = digitalRead(clockPin);//read the generated clock

        if (clockPinState == HIGH) { //read when clock sources
          
          if (allowRead == true) {
           // Serial.print("clockPinState = HIGH");
           // delay(1);
            allowRead = false;
            pinState1 = digitalRead(Bus_Pin1);
            pinState2 = digitalRead(Bus_Pin2);
            pinState3 = digitalRead(Bus_Pin3);
            pinState4 = digitalRead(Bus_Pin4);
            recognizeValue();
           // Serial.println(" received value"+ Bus_Val);
            countAndFillFunction();  
              if (ticktick == 5) {                
                digitalWrite(micro_ack, HIGH);
                exitParameter = true;
                Serial.println("ticktick = 5");
                digitalWrite(ledPin, HIGH);
                break;
              }                  
            }         
        } else 
        if (clockPinState == LOW) { //reset read parameters when clk pin sinks
           
          if (allowRead == false) {
            allowRead = true;
            Serial.print("clockPinState = LOW");
            }         
        }
        // END READING WHILE LOOP
      }
      // END IF READING PIN TRUE
    }
    else if (hitchhike == false) {
      Serial.print("hitchhike false in line 162");
      //do something else
    }
    // END OUTER WHILE LOOP
  }
  // END  
}


/**
   this will fill the OTP[] in a sequential way
*/
void countAndFillFunction(void) {
  ticktick++;
 // Serial.println( "ticktick");
  //OTP[ticktick - 1] = Bus_Val;
  OTP[ticktick] = Bus_Val;
}

void SETtalkMode() {
  //set D1-D4 ti output
  pinMode(Bus_Pin1, OUTPUT);
  pinMode(Bus_Pin2, OUTPUT);
  pinMode(Bus_Pin3, OUTPUT);
  pinMode(Bus_Pin4, OUTPUT);
  //pulll down D1-D4
  delay(5);
  digitalWrite(Bus_Pin1, LOW);
  digitalWrite(Bus_Pin2, LOW);
  digitalWrite(Bus_Pin3, LOW);
  digitalWrite(Bus_Pin4, LOW);
  delay(5);
}

void SETlistenMode() {
  pinMode(Bus_Pin1, INPUT_PULLUP);
  pinMode(Bus_Pin2, INPUT_PULLUP);
  pinMode(Bus_Pin3, INPUT_PULLUP);
  pinMode(Bus_Pin4, INPUT_PULLUP);
  //set D1-D4 to input with pullups
}

void recognizeValue(void) {

  if ( (pinState4 == LOW) && (pinState3 == LOW) && (pinState2 == LOW) && (pinState1 == LOW) ) {
    Bus_Val = 0;
  }
  else if ( (pinState4 == LOW) && (pinState3 == LOW) && (pinState2 == LOW) && (pinState1 == HIGH) ) {
    Bus_Val = 1;
  }
  else if ( (pinState4 == LOW) && (pinState3 == LOW) && (pinState2 == HIGH) && (pinState1 == LOW) ) {
    Bus_Val = 2;
  }
  else if ( (pinState4 == LOW) && (pinState3 == LOW) && (pinState2 == HIGH) && (pinState1 == HIGH) ) {
    Bus_Val = 3;
  }
  else if ( (pinState4 == LOW) && (pinState3 == HIGH) && (pinState2 == LOW) && (pinState1 == LOW) ) {
    Bus_Val = 4;
  }
  else if ( (pinState4 == LOW) && (pinState3 == HIGH) && (pinState2 == LOW) && (pinState1 == HIGH) ) {
    Bus_Val = 5;
  }
  else if ( (pinState4 == LOW) && (pinState3 == HIGH) && (pinState2 == HIGH) && (pinState1 == LOW) ) {
    Bus_Val = 6;
  }
  else if ( (pinState4 == LOW) && (pinState3 == HIGH) && (pinState2 == HIGH) && (pinState1 == HIGH) ) {
    Bus_Val = 7;
  }
  else if ( (pinState4 == HIGH) && (pinState3 == LOW) && (pinState2 == LOW) && (pinState1 == LOW) ) {
    Bus_Val = 8;
  }
  else if ( (pinState4 == HIGH) && (pinState3 == LOW) && (pinState2 == LOW) && (pinState1 == HIGH) ) {
    Bus_Val = 9;
  }
  else if ( (pinState4 == HIGH) && (pinState3 == LOW) && (pinState2 == HIGH) && (pinState1 == LOW) ) {
    Bus_Val = 'a';
  }
  else if ( (pinState4 == HIGH) && (pinState3 == LOW) && (pinState2 == HIGH) && (pinState1 == HIGH) ) {
    Bus_Val = 'b';
  }
  else if ( (pinState4 == HIGH) && (pinState3 == HIGH) && (pinState2 == LOW) && (pinState1 == LOW) ) {
    Bus_Val = 'c';
  }
  else if ( (pinState4 == HIGH) && (pinState3 == HIGH) && (pinState2 == LOW) && (pinState1 == HIGH) ) {
    Bus_Val = 'd';
  }
  else if ( (pinState4 == HIGH) && (pinState3 == HIGH) && (pinState2 == HIGH) && (pinState1 == LOW) ) {
    Bus_Val = 'e';
  }
  else if ( (pinState4 == HIGH) && (pinState3 == HIGH) && (pinState2 == HIGH) && (pinState1 == HIGH) ) {
    Bus_Val = 'f';
  }
}

// ISR
void interrupt () {
  //Serial.println(mr_ISR);
   mr_ISR++;   
   if(hitchhike == false){
      hitchhike = true;
    if(online){
      startReading++;     
     }
   // if (startReading == 2){
   //   dispatch_successful = true;
   // }
    //Serial.println(" startReading " + startReading);
    //Serial.println("EXECUTING ISR");
  }
}
  

// For telling machine what to do
void commandMachine (void) {
  SETtalkMode(); //machine pin will be I/P and ard will be O/P
  delay(10);
  //send the values to server to check when this calls...here a function is used to generate accordingly but it will be edited afterwords
  //  process_OTP_request();

  product_instruction();//load the comm bus with the respective logic
  delay(500);//wait for sometime for chip sync
  digitalWrite(micro_ack, LOW); //bus will be sampled and read
  delay(100);
  digitalWrite(micro_ack, HIGH);

  product_no = 0;
  product_instruction();
  delay(1);
  SETlistenMode();//set comm bus to I/P
  delay(1);

  /**
     Dispatching product

     Wait for 25 seconds
  */
}

void product_instruction(void) {
  if (product_no == 0) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, LOW);//0
  }
  else if (product_no == 1) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, HIGH);//1
  }
  else if (product_no == 2) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, LOW);//2
  }
  else if (product_no == 3) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, HIGH);//3
  }
  else if (product_no == 4) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, LOW);//4
  }
  else if (product_no == 5) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, HIGH);//5
  }
  else if (product_no == 6) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, LOW);//6
  }
  else if (product_no == 7) {
    digitalWrite(Bus_Pin4, LOW);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, HIGH);//7
  }
  else if (product_no == 8) {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, LOW);//8
  }
  else if (product_no == 9) {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, HIGH);//9
  }
  else if (product_no == 10) {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, LOW);//A
  }
  else if (product_no == 'B') {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, LOW);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, HIGH);//B
  }
  else if (product_no == 'C') {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, LOW);//C
  }
  else if (product_no == 'D') {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, LOW);
    digitalWrite(Bus_Pin1, HIGH);//D
  }
  else if (product_no == 'E') {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, LOW);//E
  }
  else if (product_no == 'F') {
    digitalWrite(Bus_Pin4, HIGH);
    digitalWrite(Bus_Pin3, HIGH);
    digitalWrite(Bus_Pin2, HIGH);
    digitalWrite(Bus_Pin1, HIGH);//F
  }
}
/**
   END
   local communication code
*/


/*
   dispatchLifeCycle

   NOTHING TO SEND : 0
   SEND OTP PENDING : 1
   SENT : 2
   COMMAND MACHINE : 3
   WAIT FOR DISPATCH : 4
   ACK TO SERVER : 5
*/
byte dispatchLifeCycle = 0;

byte secondsCount = 0;

void resetEverything() {
  orderId = 0;
  product_no = 0;
  dispatchLifeCycle = 0;
  startReading = 0;
  //waitingForDrop = 0;
  secondsCount = 0;
  ticktick = 0;
  exitParameter = false;
  allowRead = false;
  hitchhike = false;
  dispatch_successful = false;
  mr_ISR = 0;
  OTP[0] = 0;
  OTP[1] = 0;
  OTP[2] = 0;
  OTP[3] = 0;
  OTP[4] = 0;
  OTP_read = 0;

  Serial.println("Reset"); 

  clockPinState = digitalRead(clockPin);//read the generated clock
  if (clockPinState == HIGH){
      Serial.println("clock pin is read HIGH");
  }
  else
  if (clockPinState == LOW){
    Serial.println("clock pin is read LOW");
  }
  
  delay(100);
}

/**
   setup
*/
void setup() {

  // LOCAL CIRCUIT SETUP
  pinMode(ledPin, OUTPUT);
  pinMode(micro_ack, OUTPUT);
  digitalWrite(micro_ack, HIGH);
  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), interrupt, FALLING);

  delay(100);

  Serial.begin(9600);
  // END OF SETUP
}

/**
   LOOP
*/
void loop() {

  String data;

  // verify connectivity
  if (client.connected()) {
    online = true;

    // check for first ISR hit
    if (hitchhike == true && dispatchLifeCycle == 0) {
      // get the 4 digits entered by user and process the OTP
       //Serial.print("getting password");
       if (OTP_read == 0){
        //Serial.print("otp_read");
                
        getPassword();
        dispatchLifeCycle = 1;
        OTP_read = 1;
       }
      
    } if(OTP_read == 0){
      Serial.print(".");      
    }

    // SERVER IDLE
    if (waitingForResponse == false) {
      // synchronized send block

      // if we have not registered
      if (machineRegistered != true) {
        Serial.println("sending REGISTER event");
        webSocketClient.sendData(registerString);
        machineRegistered = true;
        waitingForResponse = true;

      } else {
        if (dispatchLifeCycle == 1) {
          // SEND PIN TO SERVER

          String otp = "";
          otp = otp + OTP[4] + OTP[3] + OTP[2] + OTP[1];
         // otp = otp + OTP[1] + OTP[2] + OTP[3] + OTP[4];
          Serial.println("sending OTP: " + otp);
          webSocketClient.sendData("P " + otp);

          // dispatchLifeCycle = 2 : wait for confirmation from server
          dispatchLifeCycle = 2;
          waitingForResponse = true;

        } else if (dispatchLifeCycle == 3) {
          // COMMAND MACHINE
          // we have shelf
          commandMachine();//communicate to the control board for the transaction
          dispatchLifeCycle = 4;
          Serial.println("commanded");
          byte waitingForDrop = 0;
          while (1) { //lock for single transaction and debugging
            digitalWrite(ledPin, LOW);
            delay(500);
            digitalWrite(ledPin, HIGH);
            delay(500);
            waitingForDrop++;
            
            if (mr_ISR >= 4){
              dispatch_successful = true;
              Serial.println("mr_ISR 4");
              dispatchLifeCycle = 5;
              break;
            }
            //else           
           // if (waitingForDrop > 25){
           //   break;
           // }
          }

          if (dispatchLifeCycle == 5) {
            // FAIL
            Serial.println("IS 5");
          }else{
            Serial.println("NOT 5");
            resetEverything();
          }
          

          // wait for dispatch ack from ISR when dispatchLifeCycle == 4

        } else if (dispatchLifeCycle == 5) {
          // SEND ACK TO SERVER
          // D<space><orderId>
          //delay(3000);
          //Serial.println(ack);
          String ack = "D ";
          ack += orderId;          
          webSocketClient.sendData(ack);
          Serial.println("Mr.FIVE active");
         // webSocketClient.sendData("D " + orderId);
           Serial.println("D " + orderId);
          waitingForResponse = true;
          resetEverything();

        } else {
          // IDLE FOR LONG TIME
          if (secondsCount > 15 && dispatchLifeCycle == 0) {
            // for testing, send ECHO event
            data = echoString;
            Serial.println("Sending " + data);
            webSocketClient.sendData(data);
            secondsCount = 0;
            waitingForResponse = true;
          }
        }
        // END DATA SENDING
      }

      // waiting for response to previous event sent
    } else {
      // waitingForResponse == true

      // read data from server
      webSocketClient.getData(data);

      // server sent something
      if (data.length() > 0) {
        Serial.print("Received data: ");
        waitingForResponse = false;
        Serial.println(data);

        char event = data[0];

        if (event == 'E') {
          // ERROR Event
          Serial.println("ERROR");
          if(dispatchLifeCycle == 2){
            product_no = 0;
            Serial.println("err instruction");
            commandMachine();
          }
          resetEverything();
          return;
        }

        byte responseLength = data.length();

        if (responseLength < 2) {
          if (data[0] == 'O') {
            // OK
            Serial.println("OK");
            if (dispatchLifeCycle == 5) {
              // RESET ALL
              resetEverything();
            }
          }
          return;
        }

        if (event == 'O' && data[1] == ' ') {
          // DISPATCH / ORDER event
          byte index1, index2;
          // O<space><orderId><space><quantity><space><shelf>
          index1 = data.indexOf(' ', 2);
          orderId = (data.substring(2, index1)).toInt();
          index2 = data.indexOf(' ', index1 + 1);
          byte quantity = (data.substring(index1 + 1, index2)).toInt();
          product_no = (data.substring(index2 + 1, responseLength)).toInt();
          //delay(3000);
          Serial.println("orderId: ");
          Serial.println(orderId);
          Serial.println("q: ");
          Serial.println(quantity);
          Serial.println("shelf: ");
          Serial.println(product_no);
          //Serial.println("D " + orderId);
          String ack = "D ";
          ack += orderId;
          Serial.println(ack);
          dispatchLifeCycle = 3;
        }
        else {
          // WUT???
        }
      }
      else
        Serial.println("waiting for reply...");
    }

    // END CONNECTED
  } else {
    Serial.println("Client disconnected.");
    
    if(OTP_read == 1){
      product_no = 'F';
      //Serial.println("Failed!!");
      commandMachine();
      resetEverything();
    }
     //reconnection logic
    // not tested
   reset_Ethernet();  
  }

  // wait to fully let the client disconnect
  delay(loopDelay);
  secondsCount++;
}
