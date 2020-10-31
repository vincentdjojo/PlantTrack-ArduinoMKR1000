//Reference: Github: https://github.com/mobizt
//Program connects to Firebase's Realtime database and updates or pushes to it.
//Required WiFi101 Library for Arduino from https://github.com/arduino-libraries/WiFi101

#include "Firebase_Arduino_WiFi101.h"
#define FIREBASE_HOST "soil-moisture-sensor-b10f9.firebaseio.com"
#define FIREBASE_AUTH "JSsjbQmUZIfe8AW0GwwKyLXmLcLRfcd1wmiiH6E6"
#define WIFI_SSID "HMT-Guest" //This will change. Currently living in Airbnb. Will have to change the network.
#define WIFI_PASSWORD "hmt14675" //Will change too.

//Define Firebase data object
FirebaseData firebaseData;
int status = WL_IDLE_STATUS;
WiFiServer server(80);


int sensorPin = A0;
int sensorValue = 0;

void setup()
{

  Serial.begin(115200);
  delay(100);
  Serial.println();

  Serial.print("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;
  while (status != WL_CONNECTED)
  {
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);
  Serial.println();


  String path = "/Soil_Moisture";
  String jsonStr;

  //Provide the autntication data
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH, WIFI_SSID, WIFI_PASSWORD);
  Firebase.reconnectWiFi(true);

  String state;
  String color;
  Serial.println("-----------------------------------");
  Serial.println("----------Begin Push Test----------");
  Serial.println("-----------------------------------");
  Serial.println();
  for (long i = 0; i > -1; i++)
  {
    sensorValue = analogRead(sensorPin);
    Serial.print("Moisture = " );
    Serial.println(sensorValue);
    delay(1000);

    if ( sensorValue < 300 && sensorValue >= 0 ) {
      state = "Dry";
      color = "#ff0000";
    }
    else if ( sensorValue >= 300 && sensorValue < 600 )
    {
      color = "#008000";
      state = "Moist";
    }
    else if ( sensorValue > 600 )
    {
      color = "#FFFF00";
      state = "Wet";
    }
    delay (1000);

    unsigned long time;

    Serial.print("Time: ");
    time = millis();

    Serial.println(time); //prints time since program started
    //Append many data (multiple keys included nest data) to the database at "/test/append"
    //Just for reference on the Json syntax: //String jsonData = "{\"Moisture2\": \"" +  + "\"}";
    //Just for reference on the Json syntax: //String jsonData = "{\"Moisture\": \"TEST\",\"MoistureState\": \"TEST\", \"ID\": \"TEST\"}";
    
    String jsonData = "{\"Moisture\": \"" + String(sensorValue)  + "\", \"color\": \"" + String(color) + "\",\"MoistureState\": \"" + String(state) + "\", \"ID\": \"" + String(i) + "\"}";
    if (Firebase.pushJSON(firebaseData, path + "/append", jsonData)) {

      //Success, then read the payload value

      //Database path to be appended
      Serial.println(firebaseData.dataPath()); //Should be "/test/append"

      //New created key/name
      Serial.println(firebaseData.pushName());

      //Absolute path of new appended data
      Serial.println(firebaseData.dataPath() + "/" + firebaseData.pushName());


    } else {
      //Failed, then print out the error detail
      Serial.println(firebaseData.errorReason());
    }
    delay(30000);//Record moisture every 30 seconds.
  }

}

void loop()
{


}
