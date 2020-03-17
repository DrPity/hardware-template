//Some common helper functions
#ifndef NETWORK_H
#define NETWORK_H
#include <Arduino.h>
#include <MQTT.h>
#include <Ethernet.h>
#include <SPI.h>
#include <Dhcp.h>
#include <ArduinoJson.h>

#include "config.h"

#define blink(hz)      \
	pinMode(13, OUTPUT); \
	digitalWrite(13, (millis() % (1000 / hz)) > ((1000 / hz) / 2))
#define MQTT

EthernetClient client;
const String randomizedMqttClientID = (String(MQTT_CLIENTID) + "_" + String(MAC2) + String(MAC3) + String(MAC4));

MQTTClient mqtt;

class Network
{
private:
	bool _isNetworkSetup = false;
	unsigned long _ethernetMaintainTimer = 0;
	unsigned int _ethernetMaintainInterval = 5000;
	byte _mac[6] = {0xFE, (byte)MAC2, (byte)MAC3, (byte)MAC4, (byte)MAC5, (byte)MAC6};
	void _setNetwork(bool _isNetworkSetup, uint8_t mac);

public:
	//--------------------------------------------------------------------------------------------------

	void init()
	{
		checkCableConnection();
		setNetwork();
		mqtt.begin(AIO_SERVER, AIO_SERVERPORT, client);
	}

	//--------------------------------------------------------------------------------------------------

	void setNetwork()
	{
		if (_isNetworkSetup == true)
		{
			return;
		}

#if defined(MAC2) && defined(MAC3) && defined(MAC4) && defined(MAC5) && defined(MAC6)
		Serial.print("Using randomized MAC Addres: ");
#else
		Serial.print("WARNING: Using preset MAC Adress: ");
#endif
		for (uint16_t i = 0; i < sizeof(_mac) / sizeof(_mac[0]); i++)
		{
			Serial.print(_mac[i], HEX);
			Serial.print(":");
		}
		Serial.println();

		if (Ethernet.begin(_mac) == 1)
		{
			_isNetworkSetup = true;
			DEBUG_PRINTLN("network setup complete");
		}
		else
		{
			Serial.println("Failed to connect to DHCP server");
		}

		delay(5000);

		DEBUG_PRINTLN(" ... done ");
		DEBUG_PRINT(_isNetworkSetup);
		DEBUG_PRINT(" - ");

		DEBUG_PRINTLN(Ethernet.localIP());
	}

	//--------------------------------------------------------------------------------------------------
	bool maintainingDHCPLease()
	{
		if (_ethernetMaintainTimer <= millis())
		{
			byte dhcpStatus = Ethernet.maintain();
			if (dhcpStatus == 1 || dhcpStatus == 3)
			{
				Serial.println("Renewing the dhcp lease failed");
				_isNetworkSetup = false;
			}
			_ethernetMaintainTimer = millis() + _ethernetMaintainInterval;
			return true;
		}
		return false;
	}
	//--------------------------------------------------------------------------------------------------

	void checkCableConnection()
	{
		if (Ethernet.linkStatus() == LinkON)
		{
			return;
		}
		else if (Ethernet.linkStatus() == LinkOFF)
		{
			_isNetworkSetup = false;
			while (Ethernet.linkStatus() == LinkOFF)
			{
				blink(5);
				// print status over serial every 5 seconds
				if (micros() % 5000000 < 50)
				{
					Serial.println("Cable status: Disconnected");
					Serial.println("Plug in an Ethernet cable to continue");
				}
			}
			digitalWrite(13, LOW);
		}
	}

	//--------------------------------------------------------------------------------------------------

	void setup()
	{
		DEBUG_PRINT(_isNetworkSetup);
		DEBUG_PRINTLN(" connect mqtt");
		checkCableConnection();
		if (!_isNetworkSetup)
		{
			init();
		}

		if (mqtt.connected())
		{
			maintainingDHCPLease();
			return;
		}
	}

	//--------------------------------------------------------------------------------------------------

	void connectMQTT(const char clientId[], const char username[], bool skip = false)
	{
		mqtt.connect(clientId, username, skip);
	}
	
	//--------------------------------------------------------------------------------------------------

	void receiveMessageMQTT(MQTTClientCallbackSimple cb)
	{
		mqtt.onMessage(cb);
	}

	//--------------------------------------------------------------------------------------------------

	bool updateMQTT()
	{	
		bool connected = mqtt.loop();
		if(!connected)
		{
			Serial.println("Not connected");
			mqtt.connect(MQTT_CLIENTID, MQTT_USERNAME);
			delay(2000);
		}
		return connected;
	}

	//--------------------------------------------------------------------------------------------------

	void publishMQTT(String topic, DynamicJsonDocument &obj)
	{
		String jsonString;
		serializeJson(obj, jsonString);
		Serial.println(jsonString.c_str());
		mqtt.publish(topic, jsonString.c_str());
	}


	//--------------------------------------------------------------------------------------------------

	void subscribeMQTT(String topic){
		mqtt.subscribe(topic);
	}
};
#endif
