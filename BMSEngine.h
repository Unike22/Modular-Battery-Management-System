#ifndef BMSENGINE_H
#define BMSENGINE_H

#include <Arduino.h>
#include "BatteryInfo.h"

class BMSEngine {
  private:
    BatteryInfo info;

    float readCellVoltage(int pin);
    float calculateSoC(float averageVoltage);
    float calculateAdaptiveThreshold(float stateOfCharge);
    String detectTrend(float currentImbalance, float previousImbalance);

  public:
    BMSEngine();

    void update();
    BatteryInfo getBatteryInfo();
};

#endif