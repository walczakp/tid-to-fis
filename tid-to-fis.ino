#include "src/opel-tid-display-reader/TIDDisplay.hpp"
#include "src/vag-fis-radio-text-spi/VAGFISRadioText.hpp"

#define MRQ_PIN 2
#define ENA_PIN 5

VAGFISRadioText LCD(ENA_PIN);
TIDDisplay tidDisplay(MRQ_PIN);

void emergencyReset() {
    asm volatile("jmp 0");
}

void dataReceived(const TIDDisplay::DisplayData& data) {
    char l1[8] = { ' ' };
    char l2[8];
    int idx = 0;

    for (int i = 0; i < 8 && idx < 8; i++) {
        if (data.text[i] >= 32 && data.text[i] <= 126) {
            l1[idx++] = data.text[i];
        }
    }

    snprintf(l2, sizeof(l2), " %-3s %-2s ", data.rds ? "RDS" : "", data.stereo ? "ST" : "");
    LCD.showText(l1, l2);
}

void setup() {
  LCD.init();
  tidDisplay.begin();
  tidDisplay.onBusError(emergencyReset);
  tidDisplay.onDataReceived(dataReceived);
}

void loop() {
  tidDisplay.process();
  LCD.sendKeepAlive();
}

