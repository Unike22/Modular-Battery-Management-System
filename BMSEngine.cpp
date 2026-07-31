#include "BMSEngine.h"


BMSEngine::BMSEngine() : info{} {
  info.previousImbalance = 0.0;
}


float BMSEngine::readCellVoltage(int pin) {
  int adcValue = analogRead(pin);

  return MIN_CELL_VOLTAGE +
         (adcValue / 4095.0) *
         (MAX_CELL_VOLTAGE - MIN_CELL_VOLTAGE);
}


float BMSEngine::calculateSoC(float averageVoltage) {
  float soc =
      ((averageVoltage - MIN_CELL_VOLTAGE) /
       (MAX_CELL_VOLTAGE - MIN_CELL_VOLTAGE)) *
      100.0;

  
  if (soc > 100.0) {
    soc = 100.0;
  }

  if (soc < 0.0) {
    soc = 0.0;
  }

  return soc;
}


float BMSEngine::calculateAdaptiveThreshold(float stateOfCharge) {
  if (stateOfCharge >= 80.0) {
    return 0.05;
  } else if (stateOfCharge >= 30.0) {
    return 0.10;
  } else {
    return 0.15;
  }
}


String BMSEngine::detectTrend(
    float currentImbalance,
    float previousImbalance
) {
  const float tolerance = 0.01;

  if (currentImbalance > previousImbalance + tolerance) {
    return "Increasing";
  } else if (currentImbalance < previousImbalance - tolerance) {
    return "Decreasing";
  } else {
    return "Stable";
  }
}


void BMSEngine::update() {
  float totalVoltage = 0.0;

  
  for (int i = 0; i < CELL_COUNT; i++) {
    info.cellVoltages[i] = readCellVoltage(CELL_PINS[i]);
    totalVoltage += info.cellVoltages[i];
  }

  
  info.maxVoltage = info.cellVoltages[0];
  info.minVoltage = info.cellVoltages[0];

  info.strongestCell = 0;
  info.weakestCell = 0;

  
  for (int i = 1; i < CELL_COUNT; i++) {
    if (info.cellVoltages[i] > info.maxVoltage) {
      info.maxVoltage = info.cellVoltages[i];
      info.strongestCell = i;
    }

    if (info.cellVoltages[i] < info.minVoltage) {
      info.minVoltage = info.cellVoltages[i];
      info.weakestCell = i;
    }
  }

  
  info.imbalance = info.maxVoltage - info.minVoltage;

  
  info.trend = detectTrend(
    info.imbalance,
    info.previousImbalance
  );

  
  info.averageVoltage = totalVoltage / CELL_COUNT;

  
  info.stateOfCharge =
      calculateSoC(info.averageVoltage);

  
  info.adaptiveThreshold =
      calculateAdaptiveThreshold(info.stateOfCharge);

  
  info.imbalanceWarning =
      info.imbalance > info.adaptiveThreshold;

  
  info.previousImbalance = info.imbalance;
}


BatteryInfo BMSEngine::getBatteryInfo() {
  return info;
}
