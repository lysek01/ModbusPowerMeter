# ModbusPowerMeter
ModbusPowerMeter is a simple library for reading data from single-phase and three-phase power meters. It is designed to work with only two specific devices.
## Description
- [single-phase power meter](https://www.aliexpress.com/item/4000786924751.html)
- [three-phase power meter](https://www.aliexpress.com/item/1005001544742036.html)
## Installation
### Library Manager
The simplest way to install a library is using the Library Manager.  
Open Arduino IDE -> Sketch -> Include Library -> Manage Libraries and search for `ModbusPowerMeter` and click install.
### ZIP
Download and save [library](https://github.com/lysek01/ModbusPowerMeter/archive/refs/heads/main.zip) in .zip format.  
Open Arduino IDE -> Sketch -> Include Library -> Add .ZIP Library... -> Select downloaded file.
## Connection
### Hardware
Library was tested with MAX485 module, but should work with every RS485 transceiver.
### Schematic
> work in progress
## Functions
### Setup
- Begin(TX Pin, RX Pin, DE/RE Pin, Slave ID, Baudrate, Parity)
  - DE/RE Pin – default not used, can be set to any output pin
  - Slave ID – default 1, accepted 1 - 247
  - Baudrate – default 9600, accepted 1200, 2400, 4800, 9600
  - Parity – default even, accepted 'E' - even, 'O' - odd, 'N' - none

### Reading
In order to get parameters or data, you must first read them. This does not apply to the total data consumed, which are read invidually.
- readParameters() - reads communication parameters of power meter
  - getBaudrate() - returns baudrate ex: 9600
  - getParity() - return parity ex: E
  - getID() - returns slave ID ex: 1
- readData() - reads electrical data
  - getVoltage(phase) - returns voltage in Volts of specific phase, default is 1, 0 = sqrt(3) * V1
  - getCurrent(phase) - returns current in Amps of specific phase, default is 1, 0 = I1 + I2 + I3
  - getPower(phase) - returns power in kW of specific phase, default is 1, 0 = P1 + P2 + P3
  - getReactivePower(phase) - returns reactive power in VAR of specific phase, default is 1, 0 = Q1 + Q2 + Q3
  - getPowerFactor(phase) - returns power factor of specific phase, default is 1
  - getFrequency() - returns frequency
- getTotalPower() - returns total consumed power in kWh
- getTotalReactivePower() - returns total consumed reactive power in VARh
### Settings
- setBaudrate(baudrate) - sets baudrate of power meter, accepted 1200, 2400, 4800, 9600
- setParity(parity) - sets parity, accepted 'E' - even, 'O' - odd, 'N' - none
- setAddress(address) - sets address, accepted 1 - 247
> New version update: setAddress() --> setID() 
## Example
Example code reads communication parameters of power meter in setup and basic data like Voltage and Current in loop.
### Code
```c++
#include <ModbusPowerMeter.h> 

ModbusPowerMeter meter;  

void setup() {
  meter.begin(17, 16, 32, 1);
  Serial.begin(9600);
  delay(1000);

  if (meter.readParameters()) {
    Serial.println("");
    Serial.print("Baudrate: ");
    Serial.println(meter.getBaudrate());
    Serial.print("Parity: ");
    Serial.println(meter.getParity());
    Serial.print("Slave ID: ");
    Serial.println(meter.getID());
  }
  else {
    Serial.println("Boot error");
  }
}

void loop() {
  if (meter.readData()) {
    Serial.println("========================");
    Serial.print("Voltage: ");
    Serial.println(meter.getVoltage());
    Serial.print("Current: ");
    Serial.println(meter.getCurrent());
    Serial.print("Power: ");
    Serial.println(meter.getPower());
  }
  else
  {
    Serial.println("Reading error");
  }
}
```
### Output
Communication parameters are printed once at the beginning and electrical data are printed in a loop.
```
Baudrate: 9600
Parity: E
Slave ID: 1
========================
Voltage: 239.90
Current: 0.92
Power: 212.50
========================
Voltage: 238.10
Current: 0.90
Power: 207.20
```
## Credits
- Based on [ModbusMaster](https://github.com/4-20ma/ModbusMaster) library
## License
```
Copyright:: 2023 Martin Lysek

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
```
