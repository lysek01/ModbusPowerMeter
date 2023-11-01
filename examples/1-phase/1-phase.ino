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
    Serial.print("Voltage ");
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