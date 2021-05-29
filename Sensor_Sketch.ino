//Libraries for Arduino's Ethernet Shield and MQTT
#include <SPI.h>
#include <Ethernet.h>
#include <PubSubClient.h>

//Library for JSON
#include <ArduinoJson.h>

//Library for the temperature and relative humidity sensor
#include <dht.h>

//MAC, IP and server addresses should be set
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 160);
const char* server = "test.mosquitto.org";
int port = 1883;
 
//Ethernet Shield and MQTT interface
EthernetClient ethClient;
PubSubClient mqttClient(server, port, ethClient);

//PINS
const int LONG_PIN = A0;
const int TRAN_PIN = A1;
const int WIND_PIN = A2;
const int DHT_PIN = 3;
const int RAIN_PIN = A3;

//Sensor specific data
int sensor_id = 1;
DHT temp_relh_sensor(DHT_PIN,DHT22);


void setup()
{ 
  //Start the ethernet connection (with a delay to give it time to start)
  Ethernet.begin(mac, ip);              
  delay(3000);                          
  String s = "Sensor"
  String connectTo = s + sensor_id
  //Attempt to connect to the server
  if (!mqttClient.connect(connectTo)) 
  {
    //IF IT FAILS TROUBLESHOOTING WILL BE NEEDED
  }
  
  temp_relh_sensor.begin();
}

void loop()
{
  //Ready the MQTT loop
  mqttClient.loop();
  
  //Sensor readings
  //Rotations
  float Vlong_rot = analogRead(LONG_PIN);
  float Vtran_rot = analogRead(TRAN_PIN);
  //Values found to match the possible (-90°, 90°) range
  float long_rot = map(Vlong_rot, 292.0, 701.0, -90.0, 90.0);
  float tran_rot = map(Vtran_rot, 292.0, 701.0, -90.0, 90.0);

  //Wind speed
  float wind_val = analogRead(WIND_PIN);
  //Values found to match the possible (0 m/s, 120 m/s) range
  float wind = 120.0 * wind_val / 1023.0;
  
  //Temperature and relative humidity readings
  float relh_air = temp_relh_sensor.readHumidity();
  float temp_air = temp_relh_sensor.readTemperature();
  
  //Rain amount
  float rain_val = analogRead(RAIN_PIN);
  //Values found to match the possible (0 mm, 100 mm) range
  float rain = 100.0 * rain_val / 1023.0;

  //Create the JSON
  StaticJsonDocument<64> doc;
  JsonObject payload = doc.createNestedObject("payload");
  payload["sensor_id"] = sensor_id;
  
  //IsMalfunctioning is a theoretical function that would return
  //true when the sensor pin returns invalid results when read
  //It is however difficult to be able to tell when a result is
  //invalid, one ideas might be setting up the circuit so that
  //there is always a baseline voltage and when the sensor is
  //damaged, the voltage on the pin would drop to 0
  if(isMalfunctioning(LONG_PIN)){
  	payload["long_rot"] = null;
  }else{
  	payload["long_rot"] = long_rot;
  }
  if(isMalfunctioning(TRAN_PIN)){
  	payload["tran_rot"] = null;
  }else{
  	payload["tran_rot"] = tran_rot;
  }
  if(isMalfunctioning(WIND_PIN)){
  	payload["wind"] = null;
  }else{
  	payload["wind"] = wind;
  }
  if(isMalfunctioning(DHT_PIN)){
  	payload["temp_air"] = null;
  	payload["relh_air"] = null;
  }else{
    payload["temp_air"] = temp_air;
    payload["relh_air"] = relh_air;
  }
  if(isMalfunctioning(RAIN_PIN)){
  	payload["rain"] = null;
  }else{
  	payload["rain"] = rain;
  }

  //Send the JSON as a string
  String data = payload.printTo(data);
  
  //Attempt to publish the readings
  if(!mqttClient.publish("status", data))
  {
    //IF IT FAILS TROUBLESHOOTING WILL BE NEEDED
  }
 
  //Send data every hour
  delay(3600000);
}