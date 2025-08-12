#include <PubSubClient.h>
#include <ThingSpeak.h>
#include <WiFi.h>

char ssid[] = "Your_WiFi_SSID";
char pass[] = "Your_WiFi_Password";
const char* server = "mqtt3.thingspeak.com";

int fieldsToPublish[8] = { 1, 1, 0, 0, 0, 0, 0, 0 };
WiFiClient wifiClient;
PubSubClient mqttClient(server, 1883, wifiClient);


int writeChannelID =YOUR_CHANNEL_ID;
const char mqttUserName[] = "YOUR_MQTT_USERNAME"; 
const char mqttPass[] = "YOUR_MQTT_PASSWORD";
char writeAPIKey[] = "Your_Write_API_Key";

#define turbiditypin 34
#define BUZZER_PIN 13  
#define TdsSensorPin 36
#define VREF 5.0              
#define SCOUNT  1            
int analogBuffer[SCOUNT];     
int analogBufferTemp[SCOUNT];
int analogBufferIndex = 0;
int copyIndex = 0;

float averageVoltage = 0;
float tdsValue = 0;
float temperature = 25;       


int getMedianNum(int bArray[], int iFilterLen){
  int bTab[iFilterLen];
  for (byte i = 0; i<iFilterLen; i++)
  bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0){
    bTemp = bTab[(iFilterLen - 1) / 2];
  }
  else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}


void connectWifi()
{
  Serial.print( "Connecting to Wi-Fi..." );
  while ( WiFi.status() != WL_CONNECTED ) 
  {
    WiFi.begin( ssid, pass );
    delay(1000 );
    Serial.print( WiFi.status() ); 
  }
  Serial.println( "Connected to Wi-Fi." );
}

void mqttPublish(long pubChannelID, char* pubWriteAPIKey, int r, int fieldArray[]) {
  String dataString = "";
  dataString = String(r);

  Serial.println(dataString);

  String topicString = "channels/" + String(pubChannelID) + "/publish/" + String(pubWriteAPIKey);
  mqttClient.publish(topicString.c_str(), dataString.c_str());

  Serial.print("Publishing to Channel ID: ");
  Serial.println(pubChannelID);
  Serial.println();
}

void setup() 
{
  Serial.begin( 115200 );
  delay(3000);
  connectWifi();
  ThingSpeak.begin(wifiClient);
  mqttClient.setServer(server, 1883);
  pinMode(TdsSensorPin,INPUT);
  pinMode(turbiditypin,INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, HIGH);
}

void loop() 
{
  if (WiFi.status() != WL_CONNECTED) 
  {
      connectWifi();
  }
  while (mqttClient.connected() != NULL) {
    Serial.println("MQTT client connecting....");
  }
  Serial.println("MQTT client connected!");
  
  mqttClient.loop(); 

  static unsigned long analogSampleTimepoint = millis();
  if(millis()-analogSampleTimepoint > 40U)
  {     //every 40 milliseconds,read the analog value from the ADC
    analogSampleTimepoint = millis();
    analogBuffer[analogBufferIndex] = analogRead(TdsSensorPin);    //read the analog value and store into the buffer
    analogBufferIndex++;
    if(analogBufferIndex == SCOUNT){ 
      analogBufferIndex = 0;
    }
  }   
  
  static unsigned long printTimepoint = millis();
  if(millis()-printTimepoint > 800U)
  {
    printTimepoint = millis();
    for(copyIndex=0; copyIndex<SCOUNT; copyIndex++)
    {
      analogBufferTemp[copyIndex] = analogBuffer[copyIndex];
      
      // read the analog value more stable by the median filtering algorithm, and convert to voltage value
      averageVoltage = getMedianNum(analogBufferTemp,SCOUNT) * (float)VREF / 1024.0;
      
      //temperature compensation formula: fFinalResult(25^C) = fFinalResult(current)/(1.0+0.02*(fTP-25.0)); 
      float compensationCoefficient = 1.0+0.02*(temperature-25.0);
      //temperature compensation
      float compensationVoltage=averageVoltage/compensationCoefficient;
      
      //convert voltage value to tds value
      tdsValue=(133.42*compensationVoltage*compensationVoltage*compensationVoltage - 255.86*compensationVoltage*compensationVoltage + 857.39*compensationVoltage)*0.5;
      
      Serial.print("voltage:");
      Serial.print(averageVoltage,2);
      Serial.print("V   ");
      Serial.print("TDS Value:");
      Serial.print(tdsValue,0);
      Serial.println("ppm");
    }
  }


  int sensorValue = analogRead(turbiditypin);
  Serial.println(sensorValue);
  int turbidity = map(sensorValue, 0, 750, 100, 0);
  Serial.println(turbidity);
  if (turbidity < 20) 
  {
    Serial.println(" its CLEAR ");
  }
  if ((turbidity > 20) && (turbidity < 50)) 
  {
    Serial.println(" its CLOUDY ");
  }
  if (turbidity > 50) 
  {
    Serial.println(" its DIRTY ");
  }

   if(turbidity > 20 || tdsValue>500)
  {
    digitalWrite(BUZZER_PIN, LOW);
    delay(2000); 
    digitalWrite(BUZZER_PIN, HIGH);
    delay(2000);
  }

  int x = ThingSpeak.writeField(writeChannelID, 1, tdsValue , writeAPIKey);
  delay(15000);
  int y = ThingSpeak.writeField(writeChannelID, 2, turbidity , writeAPIKey);
  delay(15000);

   delay(1000);
}