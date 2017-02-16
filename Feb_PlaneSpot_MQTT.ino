/*  See more at http://blog.squix.ch

  WIRING
  ------

  7735 -> ESP8266

  VCC ->  3v3
  GND ->  GND
  LED ->  3v3
  CLK ->  D5
  SDI ->  D7
  RS  ->  D1
  RST ->  D2
  CS  ->  D0

*/
#include <Arduino.h>
#include <ESP8266WiFi.h>

//MQTT
#include <PubSubClient.h>

#include <ESP8266mDNS.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>

// ST7735
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#define TFT_CS     16
#define TFT_RST    4  // you can also connect this to the Arduino reset
// in which case, set this #define pin to 0!
#define TFT_DC     5

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS,  TFT_DC, TFT_RST);

// MQTT
const char* mqtt_server = "m20.cloudmqtt.com";
const char* mqtt_user = "gbaihvhp";
const char* mqtt_pass = "nRp3hsrLBqVm";

// Please read http://blog.squix.org/weatherstation-getting-code-adapting-it
// for setup instructions

// Setup
const int UPDATE_INTERVAL_SECS_LONG = 15; // Update every 15 seconds if no airplanes around
const int UPDATE_INTERVAL_SECS_SHORT = 10; // Update every 3 seconds if there are airplanes

int currentUpdateInterval = UPDATE_INTERVAL_SECS_LONG;
long lastUpdate = 0;

// save data points for display
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


// Variables to save
int visibleCrafts;

// Nearest
String nearestFrom = "";
String nearestTo = "";
String nearestMan = "";
String nearestMdl = "";
String nearestAlt = "";
String nearestSpd = "";
String nearestDst = "";
String nearestOp = "";
String nearestOpcode = "";
String nearestYear = "";
// Highest
String highestFrom = "";
String highestTo = "";
String highestMan = "";
String highestMdl = "";
String highestAlt = "";
String highestSpd = "";
String highestDst = "";
String highestOp = "";
String highestOpcode = "";
String highestYear = "";
// Fastest
String fastestFrom = "";
String fastestTo = "";
String fastestMan = "";
String fastestMdl = "";
String fastestAlt = "";
String fastestSpd = "";
String fastestDst = "";
String fastestOp = "";
String fastestOpcode = "";
String fastestYear = "";

//Weather
String temperature = "";
String humidity = "";
String summary = "";
String hsummary = "";
String windSpeed = "";
String icon = "";

//animation
int progression = 0;

//scene number
int startScreen = 0;
int transition = 1;
//int counter;
String espName = "";
/***************************
   End Settings
 **************************/


//declaring prototypes
void configModeCallback (WiFiManager *myWiFiManager);


void setup() {
  Serial.begin(115200);

  //ST7735
  tft.initR(INITR_144GREENTAB);   // initialize a ST7735S chip, black tab
  Serial.println("Initialized");

  uint16_t time = millis();
  tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  startScreen = random(0,1);

  if (startScreen == 1) {
    // startScreen 1
    tft.fillScreen(ST7735_RED);
    testdrawtext("PLANE", ST7735_WHITE, 10, 50, 2);
    testdrawtext("SPOTTER", ST7735_WHITE, 10, 70, 2);
    tft.fillRect(0, 90, tft.width(), tft.height(), ST7735_YELLOW);
    delay(500);
    testdrawtext("by tinkr", ST7735_BLACK, 10, 95, 1);
    delay(2000);
  } else {
    // startScreen 2
    tft.fillScreen(ST7735_CYAN);
    testdrawtext("PLANE", ST7735_BLACK, 10, 50, 2);
    testdrawtext("SPOTTER", ST7735_BLACK, 10, 70, 2);
    tft.fillRect(0, 90, tft.width(), tft.height(), ST7735_BLUE);
    delay(500);
    testdrawtext("by tinkr", ST7735_WHITE, 10, 95, 1);
    delay(2000);
  }

  WiFiManager wifiManager;

  //  wifiManager.resetSettings(); // Uncomment for testing wifi manager
  wifiManager.setAPCallback(configModeCallback);

  //or use this for auto generated name ESP + ChipID
  wifiManager.autoConnect();

  int counter = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    tft.fillScreen(ST7735_BLACK);
    testdrawtext("WIFI CONNECTING..", ST7735_WHITE, 20, 30, 1);
    delay(1000);
    counter++;
  }


  client.setServer(mqtt_server, 16877);
  client.setCallback(callback);
}

//MQTT
void callback(char* topic, byte* payload, unsigned int length) {

  //  Serial.print("Message arrived [");
  //  Serial.print(topic);
  //  Serial.print("] ");
  //  for (int i = 0; i < length; i++) {
  //    Serial.print((char)payload[i]);
  //  }
  //  Serial.println();

  payload[length] = '\0';
  //  String s = String((char*)payload);
  //  float f = s.toFloat();
  //  int visibleCrafts = s.toInt();
  if (strcmp(topic, "planespotter/info") == 0)
  {
    String s = String((char*)payload);

    Serial.print("Info: ");
    Serial.println(s);
  }

  if (strcmp(topic, "planespotter/number") == 0)
  {
    String s = String((char*)payload);
    visibleCrafts = s.toInt();
    Serial.print("Visible Crafts: ");
    Serial.println(visibleCrafts);
  }

  if (strcmp(topic, "planespotter/nearest/from") == 0)
  {
    String from = String((char*)payload);
    nearestFrom = from.substring(5, from.length());
    Serial.print("NearestFrom: ");
    Serial.println(nearestFrom);
  }

  if (strcmp(topic, "planespotter/nearest/to") == 0)
  {
    String to = String((char*)payload);
    nearestTo = to.substring(5, to.length());
    Serial.print("NearestTo: ");
    Serial.println(nearestTo);
  }

  if (strcmp(topic, "planespotter/nearest/man") == 0)
  {
    nearestMan = String((char*)payload);
    //    nearestFrom = "";
    //    nearestTo = "";
    Serial.print("NearestMan: ");
    Serial.println(nearestMan);
  }

  if (strcmp(topic, "planespotter/nearest/mdl") == 0)
  {
    nearestMdl = String((char*)payload);
    Serial.print("NearestMdl: ");
    Serial.println(nearestMdl);
  }

  if (strcmp(topic, "planespotter/nearest/alt") == 0)
  {
    nearestAlt = String((char*)payload);
    Serial.print("NearestAlt: ");
    Serial.println(nearestAlt);
  }

  if (strcmp(topic, "planespotter/nearest/spd") == 0)
  {
    nearestSpd = String((char*)payload);
    Serial.print("NearestSpd: ");
    Serial.println(nearestSpd);
  }

  if (strcmp(topic, "planespotter/nearest/dst") == 0)
  {
    nearestDst = String((char*)payload);
    Serial.print("NearestDst: ");
    Serial.println(nearestDst);
  }

  if (strcmp(topic, "planespotter/nearest/op") == 0)
  {
    nearestOp = String((char*)payload);
    Serial.print("NearestOp: ");
    Serial.println(nearestOp);
  }

  if (strcmp(topic, "planespotter/nearest/opcode") == 0)
  {
    nearestOpcode = String((char*)payload);
    Serial.print("NearestOpCode: ");
    Serial.println(nearestOpcode);
  }

  if (strcmp(topic, "planespotter/nearest/year") == 0)
  {
    nearestYear = String((char*)payload);
    Serial.print("NearestYear: ");
    Serial.println(nearestYear);
  }

  //Highest
  if (strcmp(topic, "planespotter/highest/from") == 0)
  {
    String from = String((char*)payload);
    highestFrom = from.substring(5, from.length());
    Serial.print("HighestFrom: ");
    Serial.println(highestFrom);
  }

  if (strcmp(topic, "planespotter/highest/to") == 0)
  {
    String to = String((char*)payload);
    highestTo = to.substring(5, to.length());
    Serial.print("HighestTo: ");
    Serial.println(highestTo);
  }

  if (strcmp(topic, "planespotter/highest/man") == 0)
  {
    highestMan = String((char*)payload);
    Serial.print("HighestMan: ");
    Serial.println(highestMan);
  }

  if (strcmp(topic, "planespotter/highest/mdl") == 0)
  {
    highestMdl = String((char*)payload);
    Serial.print("HighestMdl: ");
    Serial.println(highestMdl);
  }

  if (strcmp(topic, "planespotter/highest/alt") == 0)
  {
    highestAlt = String((char*)payload);
    Serial.print("HighestAlt: ");
    Serial.println(highestAlt);
  }

  if (strcmp(topic, "planespotter/highest/spd") == 0)
  {
    highestSpd = String((char*)payload);
    Serial.print("HighestSpd: ");
    Serial.println(highestSpd);
  }

  if (strcmp(topic, "planespotter/highest/dst") == 0)
  {
    highestDst = String((char*)payload);
    Serial.print("HighestDst: ");
    Serial.println(highestDst);
  }

  if (strcmp(topic, "planespotter/highest/op") == 0)
  {
    highestOp = String((char*)payload);
    Serial.print("highestOp: ");
    Serial.println(highestOp);
  }

  if (strcmp(topic, "planespotter/highest/opcode") == 0)
  {
    highestOpcode = String((char*)payload);
    Serial.print("highestOpCode: ");
    Serial.println(highestOpcode);
  }

  if (strcmp(topic, "planespotter/highest/year") == 0)
  {
    highestYear = String((char*)payload);
    Serial.print("highestYear: ");
    Serial.println(highestYear);
  }

  //Fastest
  if (strcmp(topic, "planespotter/fastest/from") == 0)
  {
    fastestFrom = String((char*)payload);
    Serial.print("FastestFrom: ");
    Serial.println(fastestFrom);
  }

  if (strcmp(topic, "planespotter/fastest/to") == 0)
  {
    fastestTo = String((char*)payload);
    Serial.print("FastestTo: ");
    Serial.println(fastestTo);
  }

  if (strcmp(topic, "planespotter/fastest/man") == 0)
  {
    fastestMan = String((char*)payload);
    Serial.print("FastestMan: ");
    Serial.println(fastestMan);
  }

  if (strcmp(topic, "planespotter/fastest/mdl") == 0)
  {
    fastestMdl = String((char*)payload);
    Serial.print("FastestMdl: ");
    Serial.println(fastestMdl);
  }

  if (strcmp(topic, "planespotter/fastest/alt") == 0)
  {
    fastestAlt = String((char*)payload);
    Serial.print("FastestAlt: ");
    Serial.println(fastestAlt);
  }

  if (strcmp(topic, "planespotter/fastest/spd") == 0)
  {
    fastestSpd = String((char*)payload);
    Serial.print("FastestSpd: ");
    Serial.println(fastestSpd);
  }

  if (strcmp(topic, "planespotter/fastest/dst") == 0)
  {
    fastestDst = String((char*)payload);
    Serial.print("FastestDst: ");
    Serial.println(fastestDst);
  }

  if (strcmp(topic, "planespotter/fastest/op") == 0)
  {
    fastestOp = String((char*)payload);
    Serial.print("FastestOp: ");
    Serial.println(fastestOp);
  }

  if (strcmp(topic, "planespotter/fastest/opcode") == 0)
  {
    fastestOpcode = String((char*)payload);
    Serial.print("FastestOpCode: ");
    Serial.println(fastestOpcode);
  }

  if (strcmp(topic, "planespotter/fastest/year") == 0)
  {
    fastestYear = String((char*)payload);
    Serial.print("FastestYear: ");
    Serial.println(fastestYear);
  }

  if (strcmp(topic, "planespotter/weather/temp") == 0)
  {

    String t = String((char*)payload);
    temperature = t.substring(0, 2);
    Serial.print("Temp: ");
    Serial.println(temperature);
  }

  if (strcmp(topic, "planespotter/weather/humidity") == 0)
  {
    String h = String((char*)payload);
    humidity = h.substring(2, h.length());
    Serial.print("Humidity: ");
    Serial.println(humidity);
  }

  if (strcmp(topic, "planespotter/weather/windspeed") == 0)
  {
    String s = String((char*)payload);
    windSpeed = s.substring(0, 2);
    Serial.print("Wind speed: ");
    Serial.println(windSpeed);
  }

  if (strcmp(topic, "planespotter/weather/icon") == 0)
  {
    icon = String((char*)payload);
    Serial.print("Icon: ");
    Serial.println(icon);
  }

  if (strcmp(topic, "planespotter/weather/summary") == 0)
  {
    summary = String((char*)payload);
    Serial.print("Summary: ");
    Serial.println(summary);
  }

  if (strcmp(topic, "planespotter/weather/hsummary") == 0)
  {
    hsummary = String((char*)payload);
    Serial.print("Hourly Summary: ");
    Serial.println(hsummary);
  }

}

//MQTT
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_pass)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("info", "planeSpotter is Online");
      // ... and resubscribe
      client.subscribe("planespotter/nearest/#");
      client.subscribe("planespotter/highest/#");
      client.subscribe("planespotter/fastest/#");
      client.subscribe("planespotter/info");
      client.subscribe("planespotter/number");

      client.subscribe("planespotter/weather/#");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  //  if ((millis() % (10 * 1000)) == 0) {
  //    listNearest();
  //  } else if ((millis() % (20 * 1000)) == 0) {
  //    visible();
  //  } else if ((millis() % (30 * 1000)) == 0) {
  //    listHighest();
  //  }



  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 45;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    // if ( secondHand >=  10 && secondHand <=  19) {
    //   // listHighest();
    //   // delay(3000);
    // }
    if (secondHand == 5) {
      visible();
      // delay(3000);
    }
    if (secondHand ==  15) {
      listNearest();
      // delay(3000);
    }

    if (secondHand ==  40) {
      showWeather();
      // delay(3000);
    }
  }
}

// List Visible number of aircrafts
void visible() {

  if (visibleCrafts > 9) {
    tft.setCursor(tft.width() / 2 - 24, 28);
  } else {
    tft.fillScreen(ST7735_BLACK);
    tft.fillRect(0, 0, tft.width(), 80, ST7735_WHITE);
    tft.setCursor(tft.width() / 2 - 14, 28);
    tft.setTextColor(ST7735_BLACK);

    tft.setTextSize(5);
    tft.print(visibleCrafts);

    testdrawtext("AIRCRAFTS", ST7735_WHITE, 10, 90, 2);
    testdrawtext("CURRENTLY OVERHEAD", ST7735_WHITE, 10, 110, 1);
  }
}

// List Highest
void listNearest() {

  uint8_t secondHand = (millis() / 1000) % 60;
  static uint8_t lastSecond = 40;

  tft.setTextWrap(true);
  if (nearestFrom.length() < 1)
  {
    tft.fillRect(0, 0, tft.width(), 20, ST7735_BLACK);
    testdrawtext("YEAR", ST7735_WHITE, 10, 5, 1);
    tft.fillRect(0, 15, tft.width(), 20, ST7735_WHITE);
    tft.setTextColor(ST7735_BLACK);
    tft.setCursor(10, 22);
    tft.print(nearestYear);
  }
  else
  {
    tft.fillRect(0, 0, tft.width(), 20, ST7735_BLACK);
    testdrawtext("FROM", ST7735_WHITE, 10, 5, 1);
    tft.fillRect(0, 15, tft.width(), 20, ST7735_WHITE);
    tft.setTextColor(ST7735_BLACK);
    tft.setCursor(10, 22);
    tft.print(nearestFrom);
  }

  if (nearestTo.length() < 1)
  {
    tft.fillRect(0, 35, tft.width(), 20, ST7735_BLACK);
    testdrawtext("OP", ST7735_WHITE, 10, 40, 1);
    tft.fillRect(0, 50, tft.width(), 20, ST7735_WHITE);
    tft.setCursor(10, 56);
    tft.setTextColor(ST7735_BLACK);
    tft.print(nearestOp);
  }
  else
  {
    tft.fillRect(0, 35, tft.width(), 20, ST7735_BLACK);
    testdrawtext("TO", ST7735_WHITE, 10, 40, 1);
    tft.fillRect(0, 50, tft.width(), 20, ST7735_WHITE);
    tft.setCursor(10, 56);
    tft.setTextColor(ST7735_BLACK);
    tft.print(nearestTo);
  }

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if (secondHand == 10) {
      tft.setTextColor(ST7735_WHITE);
      tft.setCursor(10, 80);
      tft.fillRect(0, 70, tft.width(), 26, ST7735_BLACK);
      tft.print(nearestOp);
    }
  }

  tft.setTextColor(ST7735_WHITE);
  tft.setCursor(10, 80);
  tft.fillRect(0, 70, tft.width(), 26, ST7735_BLACK);
  tft.print(nearestMdl);
  
  tft.fillRect(60, 96, tft.width(), 108, ST7735_YELLOW);
  testdrawtext("ALTITUDE", ST7735_BLACK, 70, 99, 1);

  tft.fillRect(0, 96, 60, 108, ST7735_RED);
  tft.setCursor(10, 105);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print(nearestOpcode);

  tft.setTextSize(2);
  tft.setCursor(70, 110);
  tft.setTextColor(ST7735_BLACK);
  tft.print(nearestAlt);
}

// List Highest
void listHighest() {

  tft.setTextWrap(true);
  if (highestFrom.length() < 1)
  {
    tft.fillRect(0, 0, tft.width(), 20, ST7735_BLACK);
    testdrawtext("YEAR", ST7735_WHITE, 10, 5, 1);
    tft.fillRect(0, 15, tft.width(), 20, ST7735_RED);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(10, 22);
    tft.print(highestYear);
  }
  else
  {
    tft.fillRect(0, 0, tft.width(), 20, ST7735_BLACK);
    testdrawtext("FROM", ST7735_WHITE, 10, 5, 1);
    tft.fillRect(0, 15, tft.width(), 20, ST7735_RED);
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(10, 22);
    tft.print(highestFrom);
  }

  if (highestTo.length() < 1)
  {
    tft.fillRect(0, 35, tft.width(), 20, ST7735_BLACK);
    testdrawtext("OP", ST7735_WHITE, 10, 40, 1);
    tft.fillRect(0, 50, tft.width(), 20, ST7735_WHITE);
    tft.setCursor(10, 56);
    tft.setTextColor(ST7735_BLACK);
    tft.print(highestOp);
  }
  else
  {
    tft.fillRect(0, 35, tft.width(), 20, ST7735_BLACK);
    testdrawtext("TO", ST7735_WHITE, 10, 40, 1);
    tft.fillRect(0, 50, tft.width(), 20, ST7735_WHITE);
    tft.setCursor(10, 56);
    tft.setTextColor(ST7735_BLACK);
    tft.print(highestTo);
  }

  if ((millis() % (10 * 1000)) == 0) {
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(10, 80);
    tft.fillRect(0, 70, tft.width(), 26, ST7735_BLACK);
    tft.print(highestOp);
  }

  else {
    tft.setTextColor(ST7735_WHITE);
    tft.setCursor(10, 80);
    tft.fillRect(0, 70, tft.width(), 26, ST7735_BLACK);
    tft.print(highestMdl);
  }

  tft.fillRect(60, 96, tft.width(), 108, ST7735_YELLOW);
  testdrawtext("ALTITUDE", ST7735_BLACK, 70, 99, 1);

  tft.fillRect(0, 96, 60, 108, ST7735_RED);
  tft.setCursor(10, 105);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_WHITE);
  tft.print(highestOpcode);

  tft.setTextSize(2);
  tft.setCursor(70, 110);
  tft.setTextColor(ST7735_BLACK);
  tft.print(highestAlt);
}

void showWeather() {
  //if icon has "night", invert display
    // tft.invertDisplay(true);

    tft.setCursor(tft.width() / 2 - 24, 28);
    // tft.fillScreen(ST7735_BLACK);
    // tft.fillRect(0, 0, tft.width(), 80, ST7735_WHITE);
    tft.fillRect(0, 0, tft.width(), 80, ST7735_WHITE);
    tft.setCursor(tft.width() / 2 - 26, 20);
    tft.setTextColor(ST7735_MAGENTA);
    tft.setTextSize(5);
    tft.print(temperature);
    testdrawtext("o", ST7735_MAGENTA, tft.width() / 2 + temperature.length()*16, 18, 2);

    //summary
    tft.setTextSize(1);
    tft.setTextColor(ST7735_BLACK);
    tft.setCursor(tft.width() / 2 - summary.length()*3, 65);
    tft.print(summary);

    tft.fillRect(0, 80, tft.width(), 84, ST7735_RED);

    //humidity & windspeed
    // testdrawtext("WIND SPEED", ST7735_WHITE, 10, 90, 1);
    // tft.setCursor(84, 90);
    // tft.print(windSpeed);
    // testdrawtext("HUMIDITY", ST7735_WHITE, 10, 104, 1);
    // tft.setCursor(84, 104);
    // tft.print(humidity);
    // testdrawtext("%", ST7735_WHITE, 84 + humidity.length()*6, 104, 1);
    // tft.fillRect(0, 10, progression, 14, ST7735_MAGENTA);

    tft.fillRect(0, 86, tft.width()/2, 108, ST7735_BLUE);
    tft.fillRect(tft.width()/2, 86, tft.width(), 108, ST7735_CYAN);

    testdrawtext("WINDSPEED", ST7735_WHITE, 4, 90, 1);
    testdrawtext("HUMIDITY", ST7735_BLACK, tft.width()/2 + 10, 90, 1);
    tft.setTextSize(3);
    tft.setCursor(10, 102);
    tft.setTextColor(ST7735_WHITE);
    tft.print(windSpeed);
    tft.setTextSize(3);
    // testdrawtext("kmph", ST7735_BLACK, 10 + windSpeed.length(), 112, 1);
    tft.setCursor(tft.width()/2 + 14, 102);
    tft.setTextColor(ST7735_BLACK);
    tft.print(humidity);
    testdrawtext("%", ST7735_BLACK, tft.width()/2 + 28 + humidity.length()*11, 102, 1);


}

//ST7735
void testdrawtext(char *text, uint16_t color, uint16_t x, uint16_t y, uint16_t s) {
  tft.setCursor(x, y);
  tft.setTextColor(color);
  tft.setTextSize(s);
  tft.setTextWrap(true);
  tft.print(text);
}

void configModeCallback (WiFiManager *myWiFiManager) {
  Serial.println("Entered config mode");
  Serial.println(WiFi.softAPIP());

  //Step 1
  tft.fillScreen(ST7735_BLACK);
  tft.fillRect(10, 12, 40, 12, ST7735_WHITE);
  testdrawtext("STEP 1", ST7735_BLACK, 12, 14, 1);
  testdrawtext("CONNECT TO DEVICE", ST7735_WHITE, 10, 30, 1);
  tft.setCursor(10, 40);
  tft.setTextSize(2);
  tft.setTextColor(ST7735_YELLOW);
  tft.print(myWiFiManager->getConfigPortalSSID());
  testdrawtext("THROUGH WIFI ", ST7735_WHITE, 10, 58, 1);

  tft.fillRect(10, 80, 40, 12, ST7735_WHITE);
  testdrawtext("STEP 2", ST7735_BLACK, 12, 82, 1);
  testdrawtext("GO TO", ST7735_WHITE, 10, 96, 1);
  tft.setCursor(10, 106);
  tft.setTextSize(1);
  tft.setTextColor(ST7735_YELLOW);
  tft.print(WiFi.softAPIP());

  //if you used auto generated SSID, print it
  Serial.println(myWiFiManager->getConfigPortalSSID());
}
