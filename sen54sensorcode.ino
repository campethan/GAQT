#include <Arduino.h>
#include <SensirionI2CSen5x.h>
#include <Wire.h>

#define MAXBUF_REQUIREMENT 48

#if (defined(I2C_BUFFER_LENGTH) &&                 \
     (I2C_BUFFER_LENGTH >= MAXBUF_REQUIREMENT)) || \
    (defined(BUFFER_LENGTH) && BUFFER_LENGTH >= MAXBUF_REQUIREMENT)
#define USE_PRODUCT_INFO
#endif

SensirionI2CSen5x sen5x;

int motor1pin1 = 2;
int motor1pin2 = 3;
int motor2pin1 = 4;
int motor2pin2 = 5;

void setup() {
    // Initialize Serial communication
    Serial.begin(115200); 
    while (!Serial) {
        delay(100);
    }

    // Initialize I2C communication and sensor
    Wire.begin();
    sen5x.begin(Wire);

    uint16_t error;
    char errorMessage[256];

    error = sen5x.deviceReset();
    if (error) {
        Serial.print("Error trying to execute deviceReset(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }

#ifdef USE_PRODUCT_INFO
    printSerialNumber();
    printModuleVersions();
#endif

    // Set temperature offset for the sensor
    float tempOffset = 0.0;
    error = sen5x.setTemperatureOffsetSimple(tempOffset);
    if (error) {
        //Serial.print("Error trying to execute setTemperatureOffsetSimple(): ");
        errorToString(error, errorMessage, 256);
        //Serial.println(errorMessage);
    } else {
        Serial.print("Temperature Offset set to ");
        Serial.print(tempOffset);
        Serial.println(" deg. Celsius (SEN54/SEN55 only)");
    }

    // Start Measurement
    error = sen5x.startMeasurement();
    /*
    if (error) {
        Serial.print("Error trying to execute startMeasurement(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    }
    */
    // Initialize motor pins
    pinMode(motor1pin1, OUTPUT);
    pinMode(motor1pin2, OUTPUT);
    pinMode(motor2pin1, OUTPUT);
    pinMode(motor2pin2, OUTPUT);

    // Optional motor control pins
    pinMode(9, OUTPUT); 
    pinMode(10, OUTPUT);
}

void loop() {
    // Sensor data reading and printing logic
    uint16_t error;
    char errorMessage[256];
    delay(1000);

    float massConcentrationPm1p0;
    float massConcentrationPm2p5;
    float massConcentrationPm4p0;
    float massConcentrationPm10p0;
    float ambientHumidity;
    float ambientTemperature;
    float vocIndex;
    float noxIndex;

    error = sen5x.readMeasuredValues(
        massConcentrationPm1p0, massConcentrationPm2p5, massConcentrationPm4p0,
        massConcentrationPm10p0, ambientHumidity, ambientTemperature, vocIndex,
        noxIndex);
/*
    if (error) {
        Serial.print("Error trying to execute readMeasuredValues(): ");
        errorToString(error, errorMessage, 256);
        Serial.println(errorMessage);
    } else {
      */
        Serial.print("MassConcentrationPm1p0: ");
        Serial.print(massConcentrationPm1p0);
        Serial.print("\tMassConcentrationPm2p5: ");
        Serial.print(massConcentrationPm2p5);
        /*
        Serial.print("\tMassConcentrationPm4p0: ");
        Serial.print(massConcentrationPm4p0);
        Serial.print("\tMassConcentrationPm10p0: ");
        Serial.print(massConcentrationPm10p0);
        Serial.print("\tAmbientHumidity: ");
        Serial.print(isnan(ambientHumidity) ? "n/a" : String(ambientHumidity));
        Serial.print("\tAmbientTemperature: ");
        Serial.print(isnan(ambientTemperature) ? "n/a" : String(ambientTemperature));
        Serial.print("\tVocIndex: ");
        Serial.print(isnan(vocIndex) ? "n/a" : String(vocIndex));
        Serial.print("\tNoxIndex: ");
        Serial.println(isnan(noxIndex) ? "n/a" : String(noxIndex));
    */

     //Motor control logic
    
    analogWrite(9, 100); // ENA pin (Optional)
    analogWrite(10, 200); // ENB pin (Optional)

    digitalWrite(motor1pin1, HIGH);
    digitalWrite(motor1pin2, LOW);
    digitalWrite(motor2pin1, HIGH);
    digitalWrite(motor2pin2, LOW);
    delay(1000);

    digitalWrite(motor1pin1, LOW);
    digitalWrite(motor1pin2, HIGH);
    digitalWrite(motor2pin1, LOW);
    digitalWrite(motor2pin2, HIGH);
    delay(1000);
    
}

