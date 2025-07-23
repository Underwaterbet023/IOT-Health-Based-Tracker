#define BLYNK_TEMPLATE_ID "TMPL3Mdks8xkR"
#define BLYNK_TEMPLATE_NAME "Arogya Track" w
#define BLYNK_AUTH_TOKEN "o2X2vibreQqco1WFktpeccFANJsP7cZ_" // Replace with your Auth Token

#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include "MAX30100_PulseOximeter.h"

#include <Wire.h>
#include <BlynkSimpleEsp8266.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS 1000

PulseOximeter pox;
uint32_t tsLastReport = 0;

// WiFi credentials
char ssid[] = ""; // Replace with your WiFi SSID
char pass[] = ""; // Replace with your WiFi password

// Callback function for sensor readings
void onBeatDetected()
{
    Serial.println("Beat detected!");
}

void setup()
{
    // Initialize Serial Monitor
    Serial.begin(115200);
    Serial.println("Initializing...");

    // Connect to Blynk
    Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

    // Initialize MAX30100
    if (!pox.begin())
    {
        Serial.println("FAILED to initialize MAX30100. Check wiring/power.");
        while (true)
            ;
    }
    else
    {
        Serial.println("MAX30100 initialized.");
    }

    // Set the callback for heartbeat detection
    pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
    // Blynk communication handler
    Blynk.run();

    // Update sensor readings
    pox.update();

    // Send data to Blynk every second
    if (millis() - tsLastReport > REPORTING_PERIOD_MS)
    {
        tsLastReport = millis();

        float heartRate = pox.getHeartRate();
        float spO2 = pox.getSpO2();

        Serial.print("Heart Rate: ");
        Serial.print(heartRate);
        Serial.print(" bpm / SpO2: ");
        Serial.print(spO2);
        Serial.println(" %");

        // Push data to Blynk
        Blynk.virtualWrite(V1, heartRate); // Heart Rate to Virtual Pin V1
        Blynk.virtualWrite(V2, spO2);      // SpO2 to Virtual Pin V2
    }
}