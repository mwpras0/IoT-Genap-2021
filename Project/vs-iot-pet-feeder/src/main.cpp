#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Servo.h>
#include <time.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

#define MSG_BUFFER_SIZE (50)
char msg[MSG_BUFFER_SIZE];

const long utcOffsetInSeconds = 25200;

// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

const char *ssid = "vivo 1919";       //silakan disesuaikan sendiri
const char *password = "12345678"; //silakan disesuaikan sendiri

const char *mqtt_server = "ec2-3-95-217-175.compute-1.amazonaws.com";

WiFiClient espClient;
PubSubClient client(espClient);

Servo servo;

long now = millis();
long lastMeasure = 0;
int pos = 0;

String nowTime;
String setTime;
String status;

void setup_wifi()
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}

void openLid() //cek status makan

{
  servo.write(90);
  delay(3000);
  servo.write(0);
  int hh = timeClient.getHours();
  int mm = timeClient.getMinutes();

  if ( hh > 0 && hh < 12)
  {
    status = "Sudah Makan Pagi";
  }

  else if (hh > 12 && hh < 18)
  {
    status = "Sudah Makan Siang";
  }

  else
  {
    status = "Sudah Makan Malam";
  }
}

void closeLid() // manggil servo
{
  servo.write(0);
}

void callback(char *topic, byte *payload, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if (String(topic) == "pet/motor")
  {
    Serial.print("Changing Lid status to ");
    if (messageTemp == "on")
    {
      openLid();
      Serial.println("open");
      Serial.print(status);
    }
    else if (messageTemp == "off")
    {
      closeLid();
      Serial.println("close");
    }
  }

  else if (String(topic) == "pet/time")
  {
    setTime = messageTemp;
    Serial.println(setTime);
  }
  Serial.println();
}

void reconnect()
{
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");

    // Attempt to connect
    if (client.connect("ESP8266Client", "jti", "123")) //disesuakan dengan username dan password
    {
      Serial.println("connected");
      client.subscribe("pet/motor");
      client.subscribe("pet/time");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo.attach(D1);
  closeLid();
  Serial.println("Mqtt Pet Feeder");
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  timeClient.begin();
}

void loop()
{
  timeClient.update();

  //get tanggal hari ini
  unsigned long epochTime = timeClient.getEpochTime();
  struct tm *ptm = gmtime((time_t *)&epochTime);
  int monthDay = ptm->tm_mday;
  int currentMonth = ptm->tm_mon + 1;
  int currentYear = ptm->tm_year + 1900;
  String currentDate = String(monthDay) + "/" + String(currentMonth) + "/" + String(currentYear);

  // put your main code here, to run repeatedly:
  if (!client.connected())
  {
    reconnect();
  }
  client.loop();
  now = millis();
  const char *c_status = status.c_str();

  if (now - lastMeasure > 1000)
  {
    nowTime = String(timeClient.getHours()) + ":" + String(timeClient.getMinutes()) + ":" + String(timeClient.getSeconds());
    Serial.println(nowTime);
    //atur waktu makan
    client.publish("pet/feedStatus", c_status);
  }

  if (setTime == nowTime)
  {
    openLid();
    Serial.println("open");
    Serial.print(status);
  }
  else
  {
    closeLid();
    Serial.println("close");
  }
}