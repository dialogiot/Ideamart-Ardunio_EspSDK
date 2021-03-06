/*
  GENERIC SDK FOR ESP
  Add ESP8266 and PubSubClient libraries into your arduino libraries forlder
  You can find those library files inside the zip file.
  update the send_event function and do_actions function with respect your requirements.
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// Update these with values suitable for your network.
const char* ssid = "Wifi_Name";
const char* password = "Wifi_password";

//Backend credentials
const char* mqtt_server = "mqtt.iot.ideamart.io";
const char* mqtt_port = 1883;

const char* EVENT_TOPIC = "testing/tester/v1/common";
String SUB_TOPIC_STRING = "+/2951212455078424/testing/tester/v1/sub";

WiFiClient espClient;
PubSubClient client(espClient);
char msg[300];

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


//receiving a message
void callback(char* topic, byte* payload, long length) {
  Serial.print("Message arrived [");
  Serial.print(SUB_TOPIC_STRING);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    msg[i] = (char)payload[i];
  }
  do_actions(msg);
  send_event();
  for (int i = 0; i < length; i++) {
    msg[i] = NULL;
  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str(), "rabbit", "rabbit")) {
      Serial.println("connected");
      //subscribe to the topic
      const char* SUB_TOPIC = SUB_TOPIC_STRING.c_str();
      client.subscribe(SUB_TOPIC);
      Serial.print(SUB_TOPIC_STRING);
      publish_message("Sent From ESP");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

void loop() {
  if (WiFi.status() != WL_CONNECTED) {
    setup_wifi();
  }
  if (WiFi.status() == WL_CONNECTED && !client.connected()) {
    reconnect();
  }

  client.loop();

}

void publish_message(const char* message) {
  client.publish(EVENT_TOPIC, message);
}

//====================================================================================================================================================================
void send_event() {
  //create this function to send some events to the backend. You should create a message format like this
 String message  =  "{
         "mac":"6655591876092787",
         "eventName":"eventOne",
         "state":"none",
         "eventOne":{
         "ev1Value1":30
         }
       }";
  

  //Should call publish_message() function to send events. Pass your message as parameter
   publish_message(message);
}
//====================================================================================================================================================================
//====================================================================================================================================================================
void do_actions(const char* message) {
  //Create this function according to your actions. you will receive a message something like this
  /* Eg : {
        "action":"actionOne",
        "param":{
          "ac1Value1":"1110" ,
          "parentMac":"6655591876092787",
          "ac1Value4":"port:03",
          "ac1Value5":"on",
          "mac":"6655591876092787",
          "ac1Value2":"2220",
          "ac1Value3":"567776"
        }
      } */
  Serial.println(message);
}
//=====================================================================================================================================================================
