#include "song_cannon_in_d.h"
#include <SPI.h>
#include <WiFiNINA_Generic.h>


// PINS
const int LED1 = 10;
const int LED2 = 9;
const int LED3 = 7;
const int LED4 = 6;
const int LED5 = 4;
const int LED6 = 3;
const int BUZZER = 17;

// LEDS
int piezoData = 0;

// MUSIC
int thisNote = 0;

// WIFI
char ssid[] = "RIT-Legacy";        // your network SSID (name)
int status = WL_IDLE_STATUS;
IPAddress server(129,21,156,235);  // numeric IP for Google (no DNS)
WiFiClient client;

// FUNCTIONS
void BuzzerIterate(int diyData);
void WifiInit();
void printWiFiStatus();


void setup() {
  // PINS
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(LED6, OUTPUT);
  pinMode(BUZZER, OUTPUT);

  WifiInit();

  
}

void loop() {

  String data;
  // if there are incoming bytes available
  // from the server, read them and print them:
  while (!client.available());
  
  while (client.available())
  {
    char c = client.read();
    if(c == 13) {
      while(client.available()) client.read();
      break;
    }
    if(c != -1){
        data += c;
    }
  }

  // if the server's disconnected, stop the client:
  if (!client.connected())
  {
    Serial.println(F("\nDisconnecting from server."));
    client.stop();

    // do nothing forevermore:
    while (true);
  }


  // parse the data
  int indexOfComma = data.indexOf(',');
  int diyData = data.substring(0, indexOfComma).toInt();
  piezoData += data.substring(indexOfComma + 1).toInt();

  String dataFromServer = String(diyData) + " " + String(piezoData);
  Serial.println(dataFromServer);

  // LEDS
  unsigned long curTime = millis();
  float mappedPiezo = (piezoData / 50.f) + 1;      // cant use map(piezoData, 0, 50, 1, 2);
  analogWrite(LED1, (sin(curTime / 110) + 1) * 15 * mappedPiezo);
  analogWrite(LED2, (sin(curTime / 120) + 1) * 15 * mappedPiezo);
  analogWrite(LED3, (sin(curTime / 130) + 1) * 15 * mappedPiezo);
  analogWrite(LED4, (sin(curTime / 140) + 1) * 15 * mappedPiezo);
  analogWrite(LED5, (sin(curTime / 151) + 1) * 15 * mappedPiezo);
  analogWrite(LED6, (sin(curTime / 160) + 1) * 15 * mappedPiezo);
  piezoData = constrain(piezoData - 50, 0, 500);

  // MUSIC
  if(diyData > 600) {
    BuzzerIterate(diyData);
  }
  else {
    delay(50);
  }



}

// iterate over the notes of the melody. 
// Remember, the array is twice the number of notes (notes + durations)
void BuzzerIterate(int diyData) {
  if(diyData < 600) return;
  // fake ass for loop
  if(thisNote >= notes * 2) thisNote = 0;

  // MUSIC
  // calculates the duration of each note
  divider = melody[thisNote + 1];
  if (divider > 0) {
    // regular note, just proceed
    noteDuration = (wholenote) / divider;
  } else if (divider < 0) {
    // dotted notes are represented with negative durations!!
    noteDuration = (wholenote) / abs(divider);
    noteDuration *= 1.5; // increases the duration in half for dotted notes
  }

  // we only play the note for 90% of the duration, leaving 10% as a pause
  tone(BUZZER, melody[thisNote], noteDuration*0.9);
  
  // Wait for the specief duration before playing the next note.
  delay(noteDuration);
  
  // stop the waveform generation before the next note.
  noTone(BUZZER);

  thisNote = thisNote + 2;
}


void WifiInit() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial);

  Serial.println(WIFININA_GENERIC_VERSION);

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE)
  {
    Serial.println(F("Communication with WiFi module failed!"));
    // don't continue
    while (true);
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION)
  {
    Serial.print(F("Your current firmware NINA FW v"));
    Serial.println(fv);
    Serial.print(F("Please upgrade the firmware to NINA FW v"));
    Serial.println(WIFI_FIRMWARE_LATEST_VERSION);
  }

  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED)
  {
    Serial.print(F("Attempting to connect to SSID: "));
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid);

    // wait 10 seconds for connection:
    delay(10000);
  }

  Serial.println(F("Connected to WiFi"));
  printWiFiStatus();

  Serial.println(F("\nStarting connection to server..."));

  // if you get a connection, report back via serial:
  if (client.connect(server, 2390))
  {
    Serial.println(F("Connected to server"));
  }
}

void printWiFiStatus() 
{
  // print the SSID of the network you're attached to:
  Serial.print(F("SSID: "));
  Serial.println(WiFi.SSID());

  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print(F("IP Address: "));
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print(F("Signal strength (RSSI):"));
  Serial.print(rssi);
  Serial.println(F(" dBm"));
}
