// SPDX-License-Identifier: GPL-3.0-or-later
/*
 * Copyright (C) 2023-2024 Mathieu Carbou
 */

#include <ArduinoOTA.h>
#include "ElegantOTA.h"
#include <WiFi.h>

#include <ESPAsyncWebServer.h>

#include <esp_ota_ops.h>
#include <esp_partition.h>

#include <Preferences.h>

#define TAG "SafeBoot"


AsyncWebServer webServer(80);
Preferences preferences;
String hostname = "MeshCom-OTA";


void wifiConnect() {

  // read wlan credentials
  preferences.begin("Credentials", true); // read only mode
  const char* ssid = preferences.getString("node_ssid", "none").c_str();
  const char* pass = preferences.getString("node_pwd", "none").c_str();

  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
	delay(500);

  // Scan for AP with best RSSI
	int nrAps = WiFi.scanNetworks();
  int best_rssi = -200;
  int best_idx = 0;
  for (int i = 0; i < nrAps; ++i)
  {
     if(strcmp(WiFi.SSID(i).c_str(), ssid) == 0)
     {
        if(WiFi.RSSI(i) > best_rssi)
        {
          best_rssi = WiFi.RSSI(i);
          best_idx = i;
        }
     }
  }	
 
  
	WiFi.begin(ssid, pass, WiFi.channel(best_idx), WiFi.BSSID(best_idx),true);
	delay(500);

  int iWlanWait = 0;

  while(WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    iWlanWait++;

    if(iWlanWait > 20)
    {

      // Start AP
      WiFi.mode(WIFI_AP);
      WiFi.softAP(hostname);

      return;
    }
  }


  return;
}


void setup() {

  // Set next boot partition
  const esp_partition_t* partition = esp_partition_find_first(esp_partition_type_t::ESP_PARTITION_TYPE_APP, esp_partition_subtype_t::ESP_PARTITION_SUBTYPE_APP_OTA_0, nullptr);
  if (partition) {
    esp_ota_set_boot_partition(partition);
  }


  // Connect to saved ssid or as fallback spawn an AP
  wifiConnect();

  // Start ElegantOTA
  ElegantOTA.setAutoReboot(true);
  ElegantOTA.begin(&webServer);

  // Start web server
  webServer.rewrite("/", "/update");
  webServer.onNotFound([](AsyncWebServerRequest* request) {
    request->redirect("/");
  });
  webServer.begin();

  // Start OTA
  ArduinoOTA.setHostname(hostname.c_str());
  ArduinoOTA.setRebootOnSuccess(true);
  ArduinoOTA.begin();
}

void loop() {
  ElegantOTA.loop();
  ArduinoOTA.handle();
}
