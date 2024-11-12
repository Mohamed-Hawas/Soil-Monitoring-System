#include "WiFi.h"           // WiFi library for ESP32
#include "ThingSpeak.h"      // ThingSpeak library to send data to the ThingSpeak platform

// RS485 control pins
#define RE 2                 // Pin to control RS485 receiver
#define DE 4                 // Pin to control RS485 driver
#define RXD2 16              // Serial RX pin for RS485
#define TXD2 17              // Serial TX pin for RS485

WiFiClient client;           // Create a WiFi client
unsigned long myChannelNumber = 2730816;   // ThingSpeak channel number
const char * myWriteAPIKey = "FW5K3ZW9P8FUVL8S"; // ThingSpeak write API key

// Modbus frames for each sensor reading request
const byte nitro[] = {0x01, 0x03, 0x00, 0x1e, 0x00, 0x01, 0xe4, 0x0c};  // Nitrogen
const byte phos[] = {0x01, 0x03, 0x00, 0x1f, 0x00, 0x01, 0xb5, 0xcc};   // Phosphorus
const byte pota[] = {0x01, 0x03, 0x00, 0x20, 0x00, 0x01, 0x85, 0xc0};   // Potassium
const byte moist[] = {0x01, 0x03, 0x00, 0x12, 0x00, 0x01, 0x24, 0x0f}; // Soil moisture
const byte temp[] = {0x01, 0x03, 0x00, 0x13, 0x00, 0x01, 0x75, 0xCF};   // Temperature
const byte cond[] = {0x01, 0x03, 0x00, 0x15, 0x00, 0x01, 0x95, 0xce};     // Conductivity
const byte ph[] = {0x01, 0x03, 0x00, 0x06, 0x00, 0x01, 0x64, 0x0B};     // pH
byte values[11]; // Array to store received data

// Field numbers in ThingSpeak
#define NUM_FIELDS 8
#define HUMIDITY_FIELD 1
#define TEMPERATURE_FIELD 2
#define CONDUCTIVITY_FIELD 3
#define pH_FIELD 4
#define NITROGEN_FIELD 5
#define PHOSPHORUS_FIELD 6
#define POTASSIUM_FIELD 7
#define RSSI_FIELD 8

// WiFi credentials
const char* ssid = "Hack me if you can !!";      // Network SSID
const char* password = "Mohamed Hawas";  // Network password

void enable_transmitter(){
  digitalWrite(RE, HIGH);
  digitalWrite(DE, HIGH);
}

void enable_reciever(){
  digitalWrite(RE, LOW);
  digitalWrite(DE, LOW);
}

void setup() {
  Serial.begin(9600);               // Serial monitor for debugging
  Serial1.begin(9600, SERIAL_8N1, RXD2, TXD2); // RS485 communication setup

  WiFi.mode(WIFI_STA);              // Set WiFi to station mode
  ThingSpeak.begin(client);         // Initialize ThingSpeak client
  pinMode(RE, OUTPUT);              // Set RS485 control pin as output
  pinMode(DE, OUTPUT);              // Set RS485 control pin as output
  enable_reciever();            // Set RE to low to receive
  delay(1000);
}

void loop() {
  // WiFi connection
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect");
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, password);
      delay(5000);
    }
    Serial.println("\nConnected.");
  }

  float val1, val2, val3, val4, val5, val6, val7, val8, val9;

  // Humidity measurement
  Serial.print("Humidity: ");
  enable_transmitter();          // Enable transmitter
  delay(10);
  for (uint8_t i = 0; i < sizeof(moist); i++ ) Serial1.write(moist[i]); // Send request frame
  Serial1.flush();
  enable_reciever();           // Enable receiver
  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial1.read();
  }
  val1 = int(values[3]<<8|values[4]) / 10.0;
  Serial.print(" = ");
  Serial.print(val1,1);
  Serial.println("  %");
  delay(200);

  // Temperature measurement
  Serial.print("Temperature: ");
  enable_transmitter();
  delay(10);
  for (uint8_t i = 0; i < sizeof(temp); i++ ) Serial1.write(temp[i]);
  Serial1.flush();
  enable_reciever();
  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial1.read();
  }
  val2 = int(values[3]<<8|values[4]) / 10.0;
  Serial.print(" = ");
  Serial.print(val2,1);
  Serial.println(" deg.C");
  delay(200);

  // Conductivity measurement
  Serial.print("Conductivity: ");
  enable_transmitter();
  delay(10);
  for (uint8_t i = 0; i < sizeof(cond); i++ ) Serial1.write(cond[i]);
  Serial1.flush();
  enable_reciever();
  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial1.read();
  }
  val3 = int(values[3]<<8|values[4]);
  Serial.print(" = ");
  Serial.print(val3);
  Serial.println(" uS/cm");
  delay(200);

  // pH measurement
  Serial.print("pH: ");
  enable_transmitter();
  delay(10);
  for (uint8_t i = 0; i < sizeof(ph); i++ ) Serial1.write(ph[i]);
  Serial1.flush();
  enable_reciever();
  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial1.read();
  }
  val4 = int(values[3]<<8|values[4]) / 100.0;
  Serial.print(" = ");
  Serial.println(val4,1);
  delay(200);

  // Nitrogen measurement
  Serial.print("Nitrogen: ");
  enable_transmitter();
  delay(10);
  for (uint8_t i = 0; i < sizeof(nitro); i++ ) Serial1.write(nitro[i]);
  Serial1.flush();
  enable_reciever();
  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial1.read();
  }
  val5 = int(values[3]<<8|values[4]);
  Serial.print(" = ");
  Serial.print(val5);
  Serial.println(" mg/L");
  delay(200);

  // Phosphorus measurement
  Serial.print("Phosphorus: ");
  enable_transmitter();
  delay(10);
  for (uint8_t i = 0; i < sizeof(phos); i++ ) Serial1.write(phos[i]);
  Serial1.flush();
  enable_reciever();
  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial1.read();
  }
  val6 = int(values[3]<<8|values[4]);
  Serial.print(" = ");
  Serial.print(val6);
  Serial.println(" mg/L");
  delay(200);

  // Potassium measurement
  Serial.print("Potassium: ");
  enable_transmitter();
  delay(10);
  for (uint8_t i = 0; i < sizeof(pota); i++ ) Serial1.write(pota[i]);
  Serial1.flush();
  enable_reciever();
  delay(100);
  for (byte i = 0; i < 7; i++) {
    values[i] = Serial1.read();
  }
  val7 = int(values[3]<<8|values[4]);
  Serial.print(" = ");
  Serial.print(val7);
  Serial.println(" mg/L");
  delay(200);

  Serial.println(WiFi.RSSI());

  // Send data to ThingSpeak
  ThingSpeak.setField(1, val1);
  ThingSpeak.setField(2, val2);
  ThingSpeak.setField(3, val3);
  ThingSpeak.setField(4, val4);
  ThingSpeak.setField(5, val5);
  ThingSpeak.setField(6, val6);
  ThingSpeak.setField(7, val7);
  ThingSpeak.setField(8, (WiFi.RSSI()));

  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  delay(20000); // Delay 10 minutes before next data transmission
}


