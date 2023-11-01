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
    Serial.print("Voltage P1: ");
    Serial.print(meter.getVoltage(1));  // Specify number of phase
    Serial.print(" | Voltage P2: ");
    Serial.print(meter.getVoltage(2));
    Serial.print(" | Voltage P3: ");
    Serial.println(meter.getVoltage(3));
    Serial.print("Current: ");
    Serial.println(meter.getCurrent(0));  // 0 stands for sum, example: P1 + P2 + P3. But we can also specify phase
    Serial.print("Power: ");
    Serial.println(meter.getPower(0));
  }
  else
  {
    Serial.println("Reading error");
  }
}