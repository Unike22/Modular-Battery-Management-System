#include "BMSEngine.h"


BMSEngine bms;


const unsigned long UPDATE_INTERVAL_MS = 2000;


unsigned long previousUpdateTime = 0;


void displayBatteryInfo(const BatteryInfo& battery) {
  Serial.println();

  for (int i = 0; i < CELL_COUNT; i++) {
    Serial.print("Cell ");
    Serial.print(i + 1);
    Serial.print(" Voltage: ");
    Serial.print(battery.cellVoltages[i], 2);
    Serial.println(" V");
  }

  Serial.println("--------------------------------");

  Serial.print("Strongest Cell: Cell ");
  Serial.print(battery.strongestCell + 1);
  Serial.print(" (");
  Serial.print(battery.maxVoltage, 2);
  Serial.println(" V)");

  Serial.print("Weakest Cell: Cell ");
  Serial.print(battery.weakestCell + 1);
  Serial.print(" (");
  Serial.print(battery.minVoltage, 2);
  Serial.println(" V)");

  Serial.print("Average Voltage: ");
  Serial.print(battery.averageVoltage, 2);
  Serial.println(" V");

  Serial.print("Voltage Imbalance: ");
  Serial.print(battery.imbalance, 2);
  Serial.println(" V");

  Serial.print("Imbalance Trend: ");
  Serial.println(battery.trend);

  Serial.print("Estimated SoC: ");
  Serial.print(battery.stateOfCharge, 1);
  Serial.println(" %");

  Serial.print("Adaptive Threshold: ");
  Serial.print(battery.adaptiveThreshold, 2);
  Serial.println(" V");

  Serial.print("BMS Status: ");

  if (battery.imbalanceWarning) {
    Serial.println("IMBALANCE WARNING");
  } else {
    Serial.println("NORMAL");
  }

  Serial.println("--------------------------------");
}

void setup() {
  Serial.begin(115200);

  
  analogReadResolution(12);

  Serial.println("Modular Battery Management System");
  Serial.println("--------------------------------");

  
  previousUpdateTime = millis() - UPDATE_INTERVAL_MS;
}

void loop() {
  unsigned long currentTime = millis();

  
  if (currentTime - previousUpdateTime >= UPDATE_INTERVAL_MS) {
    previousUpdateTime = currentTime;

    
    bms.update();

   
    BatteryInfo battery = bms.getBatteryInfo();

    
    displayBatteryInfo(battery);
  }


}
