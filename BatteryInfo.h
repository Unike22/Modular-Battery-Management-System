#ifndef BATTERYINFO_H
#define BATTERYINFO_H

#include "Config.h"

struct BatteryInfo {
  float cellVoltages[CELL_COUNT];

  int strongestCell;
  int weakestCell;

  float maxVoltage;
  float minVoltage;

  float imbalance;
  float previousImbalance;

  String trend;

  float averageVoltage;
  float stateOfCharge;

  float adaptiveThreshold;

  bool imbalanceWarning;
};

#endif