/*
 * T00043x example
 * 
 * status: proof of concept
 * done: reads the MLX90614 over I2C using the standard Wire library
 *
 * TODO: make a proper library including all the functions coming in the sensor's datasheet
 */

#include <Wire.h>

void setup(){
	Serial.begin(9600);
	Serial.println("Setup...");
	
        Wire.begin();
}

void loop(){
    int dev = 0x5A<<1;
    int data_low = 0;
    int data_high = 0;
    int pec = 0;
    
    Wire.beginTransmission(0x5A);
    Wire.write(0x07);
     Wire.endTransmission(false);

    Wire.requestFrom(0x5A, 3);    // request 6 bytes from slave device #2

    int i = 0;
    char c[] = {0, 0, 0};
    while(Wire.available())    // slave may send less than requested
    { 
    c[i++] = Wire.read();    // receive a byte as character
    }    

    data_low = c[0];
    data_high = c[1];
    pec = c[2];
    
    //This converts high and low bytes together and processes temperature, MSB is a error bit and is ignored for temps
    double tempFactor = 0.02; // 0.02 degrees per LSB (measurement resolution of the MLX90614)
    double tempData = 0x0000; // zero out the data
    int frac; // data past the decimal point
    
    // This masks off the error bit of the high byte, then moves it left 8 bits and adds the low byte.
    tempData = (double)(((data_high & 0x007F) << 8) + data_low);
    tempData = (tempData * tempFactor)-0.01;
    
    float celsius = tempData - 273.15;
    float fahrenheit = (celsius*1.8) + 32;

    Serial.print("Celsius: ");
    Serial.print(celsius);

    Serial.print("\tFahrenheit: ");
    Serial.println(fahrenheit);

    delay(1000); // wait a second before printing again
}
