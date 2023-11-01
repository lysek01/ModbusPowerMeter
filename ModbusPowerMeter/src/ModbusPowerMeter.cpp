#include "ModbusPowerMeter.h"

int ModbusPowerMeter::_derePin = -1;

ModbusPowerMeter::ModbusPowerMeter(int hwSerialNumber) : _serialPort(hwSerialNumber)
{
}

void ModbusPowerMeter::begin(int txPin, int rxPin, int derePin, int slaveID, long baudrate, char parity)
{
  _txPin = txPin;
  _rxPin = rxPin;
  _derePin = derePin;
  _slaveID = slaveID;
  _baudrate = baudrate;
  _parity = parity;

  const int parityMap[] = {
      SERIAL_8E1,
      SERIAL_8O1,
      SERIAL_8N1};

  int parityIndex = 0;
  if (_parity == 'O')
  {
    parityIndex = 1;
  }
  else if (_parity == 'N')
  {
    parityIndex = 2;
  }

  if (!_serialPort)
  {
    _serialPort.begin(_baudrate, parityMap[parityIndex], _rxPin, _txPin);
  }

  if (_derePin != -1) {
    pinMode(_derePin, OUTPUT);
    digitalWrite(_derePin, LOW);
    node.preTransmission(preTransmission);
    node.postTransmission(postTransmission);
  }

  node.begin(_slaveID, _serialPort);
}

void ModbusPowerMeter::preTransmission()
{
  if (_derePin != -1) {
    digitalWrite(_derePin, HIGH);
  }
}

void ModbusPowerMeter::postTransmission()
{
  if (_derePin != -1) {
    digitalWrite(_derePin, LOW);
  }
}

bool ModbusPowerMeter::readParameters()
{
  uint8_t result = node.readHoldingRegisters(0, 9);

  if (result == node.ku8MBSuccess)
  {
    _parametersValues[0] = intFloat(node.getResponseBuffer(0), node.getResponseBuffer(1));
    _parametersValues[1] = intFloat(node.getResponseBuffer(2), node.getResponseBuffer(3));
    _parametersValues[2] = intFloat(node.getResponseBuffer(8), node.getResponseBuffer(9));
    return true;
  }
  else
  {
    return false;
  }
}

bool ModbusPowerMeter::readData()
{
  if (node.readInputRegisters(0, 31) == node.ku8MBSuccess)
  {
    for (int i = 0; i < 16; i++)
    {
      _registerValues[i] = intFloat(node.getResponseBuffer(i * 2), node.getResponseBuffer(i * 2 + 1));
    }
    if (node.readInputRegisters(42, 13) == node.ku8MBSuccess)
    {
      for (int i = 0; i < 3; i++)
      {
        _registerValues[i + 16] = intFloat(node.getResponseBuffer(i * 2), node.getResponseBuffer(i * 2 + 1));
      }
      _registerValues[19] = intFloat(node.getResponseBuffer(12), node.getResponseBuffer(13));
      return true;
    }
    else
    {
      return false;
    }
  }
  else
  {
    return false;
  }
}

void ModbusPowerMeter::printRegisters()
{
  for (int i = 0; i < sizeof(_registerValues) / sizeof(_registerValues[0]); i++)
  {
    Serial.print("Register ");
    Serial.print(i);
    Serial.print(": ");
    Serial.println(_registerValues[i]);
  }
}

float ModbusPowerMeter::getVoltage(int phase)
{
  if (phase >= 0 && phase <= 3)
  {
    if (phase == 0)
    {
      return _registerValues[phase] * sqrt(3);
    }
    else
    {
      return _registerValues[phase - 1];
    }
  }
  else
  {
    return -1;
  }
}

float ModbusPowerMeter::getCurrent(int phase)
{
  if (phase >= 0 && phase <= 3)
  {
    if (phase == 0)
    {
      return _registerValues[4] + _registerValues[5] + _registerValues[6];
    }
    else
    {
      return _registerValues[phase + 3];
    }
  }
  else
  {
    return -1;
  }
}

float ModbusPowerMeter::getPower(int phase)
{
  if (phase >= 0 && phase <= 3)
  {
    return _registerValues[phase + 8];
  }
  else
  {
    return -1;
  }
}

float ModbusPowerMeter::getReactivePower(int phase)
{
  if (phase >= 0 && phase <= 3)
  {
    return _registerValues[phase + 12];
  }
  else
  {
    return -1;
  }
}

float ModbusPowerMeter::getPowerFactor(int phase)
{
  if (phase >= 1 && phase <= 3)
  {
    return _registerValues[phase + 15];
  }
  else
  {
    return -1;
  }
}

float ModbusPowerMeter::getFrequency()
{
  return _registerValues[19];
}

float ModbusPowerMeter::getTotalPower()
{
  uint8_t result = node.readInputRegisters(256, 2);

  if (result == node.ku8MBSuccess)
  {
    return intFloat(node.getResponseBuffer(0), node.getResponseBuffer(1));
  }
  else
  {
    return false;
  }
}

float ModbusPowerMeter::getTotalReactivePower()
{
  uint8_t result = node.readInputRegisters(1024, 2);

  if (result == node.ku8MBSuccess)
  {
    return intFloat(node.getResponseBuffer(0), node.getResponseBuffer(1));
  }
  else
  {
    return false;
  }
}

int ModbusPowerMeter::getBaudrate()
{
  return _parametersValues[0];
}

char ModbusPowerMeter::getParity()
{
  if (_parametersValues[1] == 0)
  {
    return 'E';
  }
  else if (_parametersValues[1] == 1)
  {
    return 'O';
  }
  else
  {
    return 'N';
  }
}

int ModbusPowerMeter::getID()
{
  return _parametersValues[2];
}

bool ModbusPowerMeter::setBaudrate(int baudrate)
{
  if (baudrate % 1200 == 0 && baudrate <= 9600 && baudrate >= 1200)
  {
    node.setTransmitBuffer(0, highBits(baudrate));
    node.setTransmitBuffer(1, lowBits(baudrate));
    return (node.writeMultipleRegisters(0, 2) == node.ku8MBSuccess);
  }
  else
  {
    return false;
  }
}

bool ModbusPowerMeter::setParity(char parity)
{
  int parityNumber;
  if (parity == 'E')
  {
    parityNumber = 0;
  }
  else if (parity == 'O')
  {
    parityNumber = 1;
  }
  else if (parity == 'N')
  {
    parityNumber = 2;
  }
  else
  {
    return false;
  }
  node.setTransmitBuffer(0, highBits(parityNumber));
  node.setTransmitBuffer(1, lowBits(parityNumber));
  return (node.writeMultipleRegisters(2, 2) == node.ku8MBSuccess);
}

bool ModbusPowerMeter::setAddress(int address)
{
  if (address >= 1 && address <= 247)
  {
    node.setTransmitBuffer(0, highBits(address));
    node.setTransmitBuffer(1, lowBits(address));
    return (node.writeMultipleRegisters(8, 2) == node.ku8MBSuccess);
  }
  else
  {
    return false;
  }
}

float ModbusPowerMeter::intFloat(int highBit, int lowBit)
{
  uint16_t data[2] = {lowBit, highBit};
  float num;
  memcpy(&num, data, 4);
  return num;
}

int16_t ModbusPowerMeter::highBits(int inputInt)
{
  float num = static_cast<float>(inputInt);
  uint32_t floatAsInt = *reinterpret_cast<uint32_t *>(&num);
  return static_cast<int16_t>((floatAsInt >> 16) & 0xFFFF);
}

int16_t ModbusPowerMeter::lowBits(int inputInt)
{
  float num = static_cast<float>(inputInt);
  uint32_t floatAsInt = *reinterpret_cast<uint32_t *>(&num);
  return static_cast<int16_t>(floatAsInt & 0xFFFF);
}
