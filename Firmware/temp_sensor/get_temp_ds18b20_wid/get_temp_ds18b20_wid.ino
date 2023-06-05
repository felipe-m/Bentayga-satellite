// Get the temperature of the two sensors DS18B20, providing the sensor ID (address)

#include <OneWire.h>
#include <DallasTemperature.h>

// Pin were the 1-Wire bus with the temp sensors DS18B20
const int PIN_ONEWIRE = 9;

// Instance to classes OneWire y DallasTemperature
OneWire OneWireObj(PIN_ONEWIRE);
DallasTemperature sensorDS18B20(&OneWireObj);

// These are the address of the 2 sensors, the one on the board and the other waterproof
DeviceAddress board_tsens_addr  = {0x28, 0xFF, 0x10, 0x4B, 0x20, 0x18, 0x01, 0x10};
DeviceAddress waterp_tsens_addr = {0x28, 0x55, 0xB3, 0x95, 0xF0, 0x01, 0x3C, 0xEE};

// compare two One Wire device addressess
// a DeviceAddress is a 8 byte array
bool comp_dev_addr(DeviceAddress addr1, DeviceAddress addr2){
 for (uint8_t i = 0; i < 8; i++){
    if (addr1[i] != addr2[i]) {
      return false;
    }
 }
 return true; // if here, all are equal
}

// print a One Wire device address
// a DeviceAddress is a 8 byte array
void prnt_dev_addr(DeviceAddress addr){
  for (uint8_t i = 0; i < 8; i++){
    // If only one digit, fill it wit a zero on the left
    if (addr[i] < 16) Serial.print("0");
      // show in HEX
    Serial.print(addr[i], HEX);
  }
  Serial.println("");
}


void setup() {
    // Init serial monitor y and temperature sensor DS18B20
    Serial.begin(9600);
    sensorDS18B20.begin();

    // Searching sensors
    Serial.println("Searching temp sensors...");
    Serial.print("Found: ");
    int numSensorsFound = sensorDS18B20.getDeviceCount();
    Serial.print(numSensorsFound);
    Serial.println(" sensors");

    // If found show address
    if(numSensorsFound>=1){
      for (int sens_i=0; sens_i<numSensorsFound; sens_i++) { 
        DeviceAddress sens_temp_addr; // 8 byte array (uint8_t)
        // get adddres of the sensor
        sensorDS18B20.getAddress(sens_temp_addr, sens_i);

        if (comp_dev_addr(sens_temp_addr, waterp_tsens_addr)) {
          Serial.print("Waterproof sensor address: ");
        }
        if (comp_dev_addr(sens_temp_addr, board_tsens_addr)) {
          Serial.print("Board sensor address:      ");
        }
        // print the address
        prnt_dev_addr (sens_temp_addr);
      }  
   }    
}
 
void loop() {
  Serial.println("Requesting temperatures to sensors");
  sensorDS18B20.requestTemperatures();
 
  // Read and print the temperature data from the address of the DS18B20 sensors
  Serial.print("Board sensor:     ");
  Serial.print(sensorDS18B20.getTempC(board_tsens_addr));
  Serial.println(" C");
  Serial.print("Waterprof sensor: ");
  Serial.print(sensorDS18B20.getTempC(waterp_tsens_addr));
  Serial.println(" C\n");
 
  delay(4000);
}