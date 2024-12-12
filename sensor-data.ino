#include <Arduino.h>
#if defined(ESP32)
#include <WiFi.h>
#elif defined(ESP8266)
#include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>
#include <Wire.h>
#include "Adafruit_SCD30.h"
#include <TimeLib.h>  // Include the TimeLib library

#include <WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>


// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

// Variables to save date and time
String formattedDate;
String dayStamp;
String timeStamp;


// Firebase includes
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "MYPC 0617"
#define WIFI_PASSWORD "U2<y4160"

// Insert Firebase project API Key
#define API_KEY "AIzaSyDRfqBnf_vz0clPPxrEcUjNUnqtY2YABkE"

// Insert RTDB URL
#define DATABASE_URL "https://workshop-3395b-default-rtdb.europe-west1.firebasedatabase.app/"

// Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
const unsigned long interval = 30000; // Interval to send data (30 seconds)
bool signupOK = false;

// SCD30 sensor
Adafruit_SCD30 scd30;

// Hardcoded room name
String roomName = "1.2"; 


void setup() {
  Serial.begin(115200);
  while (!Serial);

  // Initialize Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Configure Firebase
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println("Firebase sign-up successful");
    signupOK = true;
  } else {
    Serial.printf("Firebase sign-up failed: %s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Initialize the SCD30 sensor
  if (!scd30.begin()) {
    Serial.println(F("Could not find a valid SCD30 sensor, check wiring!"));
    while (1); // Halt if the sensor is not found
  }
  scd30.setMeasurementInterval(2); // Set measurement interval to 2 seconds

  // Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(3600);
}


void loop() {
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
  // Check if Firebase is ready and enough time has passed
  if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > interval || sendDataPrevMillis == 0)) {
    sendDataPrevMillis = millis();

    // Check if the SCD30 sensor has new data
    if (scd30.dataReady()) {
      if (!scd30.read()) {
        Serial.println(F("Failed to read data from SCD30 sensor!"));
        return;
      }

      // Read values from the sensor
      float co2 = scd30.CO2;
      float temperature = scd30.temperature;
      float humidity = scd30.relative_humidity;

      
      // The formattedDate comes with the following format:
      // 2018-05-28T16:00:13Z
      // We need to extract date and time
      formattedDate = timeClient.getFormattedDate();
      Serial.println(formattedDate);

      // Extract date
      int splitT = formattedDate.indexOf("T");
      dayStamp = formattedDate.substring(0, splitT);
      Serial.print("DATE: ");
      Serial.println(dayStamp);

      // Extract time
      timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
      Serial.print("HOUR: ");
      Serial.println(timeStamp);

      // Extract date in "JJ/MM/AAAA" format
      int year = formattedDate.substring(0, 4).toInt();
      int month = formattedDate.substring(5, 7).toInt();
      int day = formattedDate.substring(8, 10).toInt();

      String dateJJMMAAAA = String(day) + "/" + String(month) + "/" + String(year);
      Serial.print("Date (JJ/MM/AAAA): ");
      Serial.println(dateJJMMAAAA);


      // Extract date and time in the format DD/MM/YY HH:MM:SS
      String nvdatetime = dateJJMMAAAA + "_" + timeStamp ;
      Serial.println("date and time: "+ nvdatetime );

      // Log sensor values to the Serial Monitor
      Serial.print(F("CO2: "));
      Serial.print(co2);
      Serial.println(F(" ppm"));

      Serial.print(F("Temperature: "));
      Serial.print(temperature);
      Serial.println(F(" Â°C"));

      Serial.print(F("Humidity: "));
      Serial.print(humidity);
      Serial.println(F(" %"));


      formattedDate.replace("T"," ");
      formattedDate.replace("Z","");

      // Prepare data in JSON format for Firebase
      FirebaseJson json;

      json.set("co2", co2);
      json.set("temperature", temperature);
      json.set("humidity", humidity);
      json.set("date", formattedDate);

      // Send data to Firebase using timestamp as the key
      // Replace any invalid characters
      nvdatetime.replace(":", "_");
      nvdatetime.replace("/", "_");
      roomName.replace(".", "_");

      // Construct a sanitized Firebase path
      String path = "/dcCampus/" + roomName + "/";
      Serial.println("Constructed Firebase Path: " + path);


      if (Firebase.RTDB.pushJSON(&fbdo, path, &json)) {
        Serial.println("Data sent successfully with timestamp");
      } else {
        Serial.println("Failed to send data");
        Serial.println("REASON: " + fbdo.errorReason());
      }
    } else {
      Serial.println(F("Waiting for new data from SCD30..."));
    }
  }

  delay(1000); // Short delay to avoid overwhelming the loop
}
