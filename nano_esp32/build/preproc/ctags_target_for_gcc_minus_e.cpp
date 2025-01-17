# 1 "/home/gomofob/Projects/Arduino/nano_esp32/nano_esp32.ino"
/*

  Rui Santos

  Complete project details at Complete project details at https://RandomNerdTutorials.com/esp32-http-get-post-arduino/



  Permission is hereby granted, free of charge, to any person obtaining a copy

  of this software and associated documentation files.



  The above copyright notice and this permission notice shall be included in all

  copies or substantial portions of the Software.

*/
# 12 "/home/gomofob/Projects/Arduino/nano_esp32/nano_esp32.ino"
# 13 "/home/gomofob/Projects/Arduino/nano_esp32/nano_esp32.ino" 2
# 14 "/home/gomofob/Projects/Arduino/nano_esp32/nano_esp32.ino" 2
# 15 "/home/gomofob/Projects/Arduino/nano_esp32/nano_esp32.ino" 2

# 17 "/home/gomofob/Projects/Arduino/nano_esp32/nano_esp32.ino" 2

const char *ssid = "O2-Internet-052";
const char *password = "dCPCCPGBAX";

// Your Domain name with URL path or IP address with path
const char *serverName = "http://ergast.com/api/f1/2004/1/results.json";

// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastTime = 0;
// Timer set to 10 minutes (600000)
// unsigned long timerDelay = 600000;
// Set timer to 5 seconds (5000)
unsigned long timerDelay = 5000;

String sensorReadings;
float sensorReadingsArr[3];

void setup()
{
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Timer set to 5 seconds (timerDelay variable), it will take 5 seconds before publishing the first reading.");
}

void loop()
{
  // Send an HTTP POST request every 10 minutes
  if ((millis() - lastTime) > timerDelay)
  {
    // Check WiFi connection status
    if (WiFi.status() == WL_CONNECTED)
    {

      sensorReadings = httpGETRequest(serverName);
      Serial.println(sensorReadings);
      JSONVar myObject = JSON.parse(sensorReadings);

      // JSON.typeof(jsonVar) can be used to get the type of the var
      if (JSON.typeof_(myObject) == "undefined")
      {
        Serial.println("Parsing input failed!");
        return;
      }

      Serial.print("JSON object = ");
      Serial.println(myObject);

      // myObject.keys() can be used to get an array of all the keys in the object
      JSONVar keys = myObject.keys();

      for (int i = 0; i < keys.length(); i++)
      {
        JSONVar value = myObject[keys[i]];
        Serial.print(keys[i]);
        Serial.print(" = ");
        Serial.println(value);
        sensorReadingsArr[i] = double(value);
      }
      Serial.print("1 = ");
      Serial.println(sensorReadingsArr[0]);
      Serial.print("2 = ");
      Serial.println(sensorReadingsArr[1]);
      Serial.print("3 = ");
      Serial.println(sensorReadingsArr[2]);
    }
    else
    {
      Serial.println("WiFi Disconnected");
    }
    lastTime = millis();
  }
}

String httpGETRequest(const char *serverName)
{
  WiFiClient client;
  HTTPClient http;

  // Your Domain name with URL path or IP address with path
  http.begin(client, serverName);

  // If you need Node-RED/server authentication, insert user and password below
  // http.setAuthorization("REPLACE_WITH_SERVER_USERNAME", "REPLACE_WITH_SERVER_PASSWORD");

  // Send HTTP POST request
  int httpResponseCode = http.GET();

  String payload = "{}";

  if (httpResponseCode > 0)
  {
    Serial.print("HTTP Response code: ");
    Serial.println(httpResponseCode);
    payload = http.getString();
  }
  else
  {
    Serial.print("Error code: ");
    Serial.println(httpResponseCode);
  }
  // Free resources
  http.end();

  return payload;
}
