#include "configuration.h"
#include "loop_functions.h"
#include "loop_functions_extern.h"

#include <Arduino.h>

#include <Wire.h>

#include <mcu811.h>

#define I2C_ADDRESS_A 0x5A
#define I2C_ADDRESS_B 0x5B

uint8_t mcu811_i2c_address = 0x00;

#include <Adafruit_SSD1306.h>
#include "Adafruit_CCS811.h"

Adafruit_CCS811 ccs;

unsigned long MCU811TimeWait = 0;

float fTVOC = 0.0;
float ftCO2 = 0.0;
float feCO2 = 0.0;

int maxMCU811ValideCount=0;

bool setupMCU811()
{  
    if(!bMCU811ON)
    {
        meshcom_settings.node_co2=0.0;
        return false;
    }

    if(!ccs.begin(0x5A))
    {
        mcSerial.println("[INIT]... Failed to start sensor MCU-811! Please check your wiring.");
        return false;
    }

    //calibrate temperature sensor
    maxMCU811ValideCount=0;
    while(!ccs.available())
    {
        maxMCU811ValideCount++;
        if(maxMCU811ValideCount > 10)
        {
            mcSerial.println("[INIT]...MCU-811 not found");
            bMCU811ON=false;
            meshcom_settings.node_sset2 = meshcom_settings.node_sset2 & 0x7FF7; // MCU811 off
            save_settings();
            return false;
        }

        delay(500);
    }

    float temp = ccs.calculateTemperature();
    ccs.setTempOffset(temp - 25.0);

    mcSerial.println("[INIT]...MCU-811 set");

    return true;
}


bool loopMCU811()
{
    if(!bMCU811ON)
        return false;

    maxMCU811ValideCount=0;
    while(!ccs.available())
    {
        maxMCU811ValideCount++;
        if(maxMCU811ValideCount > 10)
        {
            mcSerial.println("[LOOP]...MCU-811 not available");
            bMCU811ON=false;
            meshcom_settings.node_sset2 = meshcom_settings.node_sset2 & 0x7FF7; // MCU811 off
            save_settings();
            return false;
        }

        delay(500);
        return false;
    }

    ftCO2 = ccs.calculateTemperature();
    
    uint8_t i = ccs.readData();

    if(bWXDEBUG)
        mcSerial.printf("ftCO2:%f retcod:%i\n", ftCO2, i);
    
    if(i > 0)
    {
        if(bWXDEBUG)
        {
            mcSerial.print("CO2temp: ");
            mcSerial.print(ftCO2);
            mcSerial.print(" °C, eCO2: ");
        }

        feCO2 = ccs.geteCO2();

        if(bWXDEBUG)
        {
            mcSerial.print(feCO2);
            mcSerial.print(" ppm, TVOC: ");      
        }

        fTVOC = ccs.getTVOC();
        
        if(bWXDEBUG)
        {
            mcSerial.println(fTVOC);
        }
    }
    else
    {
        mcSerial.println("[LOOP]...ERROR readData from MCU-811!");
        return false;
    }

    return true;
}

float geteCO2()
{
	return feCO2;
}

float getGasTemp()
{
	return ftCO2;
}

float getTVOC()
{
	return fTVOC;
}
