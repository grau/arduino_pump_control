#include "lcd.h"

/** I2C LCD */
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 40 chars and 4 line display

uint8_t LCD_CUSTOM_ROOF[] = {
  B00011,
  B00001,
  B01000,
  B11100,
  B11110,
  B11111,
  B11110,
  B11110
};
uint8_t LCD_CUSTOM_BUF_TOP[] = {
  B00000,
  B01110,
  B11111,
  B11111,
  B10001,
  B10001,
  B10001,
  B11111,
};

uint8_t LCD_CUSTOM_BUF_MID[] = {
  B00000,
  B01110,
  B10001,
  B11111,
  B11111,
  B11111,
  B10001,
  B11111,
};

uint8_t LCD_CUSTOM_BUF_LOW[] = {
  B00000,
  B01110,
  B10001,
  B10001,
  B10001,
  B11111,
  B11111,
  B11111,
};

uint8_t LCD_CUSTOM_DEG[] = {
  B01110,
  B01010,
  B01110,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

void initLcd() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(LCD_ROOF, LCD_CUSTOM_ROOF);
  lcd.createChar(LCD_BUF_TOP, LCD_CUSTOM_BUF_TOP);
  lcd.createChar(LCD_BUF_MID, LCD_CUSTOM_BUF_MID);
  lcd.createChar(LCD_BUF_LOW, LCD_CUSTOM_BUF_LOW);
  lcd.createChar(LCD_DEG, LCD_CUSTOM_DEG);
}

void lcdOutput() {
  lcd.setCursor(0,0);
  lcd.write(LCD_ROOF);
  lcdPrintNum(SENSOR_ROOF);

  lcd.setCursor(0, 1);
  lcd.write(LCD_BUF_TOP);
  lcdPrintNum(SENSOR_BUFFER_TOP);

  lcd.setCursor(0, 2);
  lcd.write(LCD_BUF_MID);
  lcdPrintNum(SENSOR_BUFFER_MID);

  lcd.setCursor(0, 3);
  lcd.write(LCD_BUF_LOW);
  lcdPrintNum(SENSOR_BUFFER_LOW);

  lcd.setCursor(10, 0);
  lcd.print("Pumpen:");
  lcd.setCursor(10, 1);
  for (int i = 0; i < 2; i++) {
    if (outputStates[i]) {
      lcd.print("(+|+)");
    } else {
      lcd.print("(   )");
    }
  }

  lcd.setCursor(10, 2);
  lcd.print("Status:");
  lcd.setCursor(10, 3);
  switch (systemState) {
    case STATE_BUFFER_TOO_HOT:
      lcd.print("Buff heiss");
      break;
    case STATE_ROOF_TOO_COLD:
      lcd.print("Dach kalt ");
      break;
    case STATE_PUMP_ON:
      lcd.print("Pumpen an ");
      break;
    case STATE_PUMP_OFF:
      lcd.print("Pumpen aus");
      break;
    case STATE_ROOF_FROST:
      lcd.print("De-Frost  ");
      break;
  }
}

void lcdPrintNum(int num) {
  lcd.print(" ");
  if (num < -99) { 
    lcd.print("--!");
  } else if (num < -9) {
    lcd.print(num);
  } else if (num < 0) {
    lcd.print(" ");
    lcd.print(num);
  } else if (num < 10) {
    lcd.print("  ");
    lcd.print(num);
  } else if (num < 100) {
    lcd.print(" ");
    lcd.print(num);
  } else if (num < 1000) {
    lcd.print(num);
  } else {
    lcd.print("++!");
  }

  lcd.write(LCD_DEG);
  lcd.print("C   ");
}