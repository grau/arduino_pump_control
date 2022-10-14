#include "lcd.h"

#define MILLIS_SECOND 1000ul
#define MILLIS_MINUTE (60ul * MILLIS_SECOND)
#define MILLIS_HOUR (60ul * MILLIS_MINUTE)
#define MILLIS_DAY (24ul * MILLIS_HOUR)

// millis() will overflow after ~50 days. So after 30 days we no longer look at time.
#define MAX_MILLIS (30ul * MILLIS_DAY)

/** I2C LCD */
LiquidCrystal_I2C lcd(0x27, 20, 4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

/** If uptime is bigger than 30 days */
bool max_uptime = false;

/** Status for showing, we are alive */
int acitivity_indicator = 0;
/** Icons to iterate over to show activity */
char acitivity_icons[] = {' ', '.', 'o', 'O', 'o', '.'};
#define ACTIVITY_COUNT 6;

/**
 * Just for testing the uptime code
 * @see https://forum.arduino.cc/t/can-i-forcibly-set-the-system-uptime-value-of-the-millis-counter/889844/4
 */
// extern unsigned long timer0_millis;


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

void printUptime() {
  unsigned long uptime = max_uptime ? MAX_MILLIS : millis();
  lcd.setCursor(10, 0);
  if (max_uptime) {
    lcd.print("1+ Mon ");
  } else if (uptime > MILLIS_DAY) {
    lcd.print(uptime / MILLIS_DAY);
    lcd.print(" Tag ");
  } else if (uptime > MILLIS_HOUR) {
    lcd.print(uptime / MILLIS_HOUR);
    lcd.print(" Std ");
  } else if (uptime > MILLIS_MINUTE) {
    lcd.print(uptime / MILLIS_MINUTE);
    lcd.print(" Min ");
  } else {
    lcd.print(uptime / MILLIS_SECOND);
    lcd.print(" Sek ");
  }
  lcd.setCursor(19, 0);
  
  acitivity_indicator = (acitivity_indicator + 1) % ACTIVITY_COUNT;
  lcd.print(acitivity_icons[acitivity_indicator]);
}

void initLcd() {
  lcd.init();
  lcd.backlight();
  lcd.createChar(LCD_ROOF, LCD_CUSTOM_ROOF);
  lcd.createChar(LCD_BUF_TOP, LCD_CUSTOM_BUF_TOP);
  lcd.createChar(LCD_BUF_MID, LCD_CUSTOM_BUF_MID);
  lcd.createChar(LCD_BUF_LOW, LCD_CUSTOM_BUF_LOW);
  lcd.createChar(LCD_DEG, LCD_CUSTOM_DEG);

  /* Just for testing the uptime code */
  // uint8_t oldSREG = SREG;
  // cli();
  // timer0_millis = MILLIS_DAY * 29 + MILLIS_HOUR * 23 + MILLIS_MINUTE * 59 + MILLIS_SECOND * 50;
  // timer0_millis = 4294967295ul - MILLIS_SECOND * 10;
  // SREG = oldSREG;
}

void lcdOutput() {
  if (! max_uptime && MAX_MILLIS < millis()) {
    max_uptime = true;
  }

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

  printUptime();
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
