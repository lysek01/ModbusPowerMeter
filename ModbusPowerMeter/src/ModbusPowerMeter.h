#ifndef ModbusPowerMeter_h
#define ModbusPowerMeter_h

#if !defined(ARDUINO_ARCH_ESP32)
  #error “This library only supports ESP32 boards.”
#endif

#include <Arduino.h>
#include <ModbusMaster.h>

class ModbusPowerMeter
{
public:
  ModbusPowerMeter(int hwSerialNumber = 1);
  void begin(int txPin, int rxPin, int derePin = -1, int slaveID = 1, long baudrate = 9600, char parity = 'E');

  void printRegisters();

  float intFloat(int highBit, int lowBit);
  int16_t highBits(int inputInt);
  int16_t lowBits(int inputInt);

  bool readParameters();

  bool setBaudrate(int baudrate);
  bool setParity(char parity);
  bool setAddress(int address);

  int getBaudrate();
  char getParity();
  int getID();

  bool readData();

  float getVoltage(int phase = 1);
  float getCurrent(int phase = 1);
  float getPower(int phase = 1);
  float getReactivePower(int phase = 1);
  float getPowerFactor(int phase = 1);
  float getFrequency();
  float getTotalPower();
  float getTotalReactivePower();

private:
  ModbusMaster node;
  HardwareSerial _serialPort;

  int _txPin;
  int _rxPin;
  static int _derePin;
  int _slaveID;
  long _baudrate;
  char _parity;
  float _registerValues[20];
  int _parametersValues[3];

  static void preTransmission();
  static void postTransmission();
};

#endif
