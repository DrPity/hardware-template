#include <main.h>
#include <ArduinoJson.h>
#include <stdio.h>

// keep up here; debug define needs to be before the network include
#define DEBUG
#include "debug.h"

#include "helpers.h"
#include "network.h"
#include "config.h"

Network net;
DynamicJsonDocument doc(2048);

long publishTime = 0;


void mqttMessage(String &topic, String &payload) 
{
	//Handle MQTT messages
	Serial.println("incoming: " + topic + " - " + payload);
}


void subscribe()
{
	net.subscribeMQTT("/in");
}

void setup()
{
	Serial.begin(115200);
	net.setup();
	net.receiveMessageMQTT(mqttMessage);
	doc["Key"] = "Value - Hello from feather";
}

void loop()
{

#ifdef MQTT
	if(net.updateMQTT()){
		subscribe();		
	}
	if (millis() - publishTime >= 5000){
		net.publishMQTT("/out", doc);
		publishTime = millis();
	}
#endif
}

