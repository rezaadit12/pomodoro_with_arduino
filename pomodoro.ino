#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Inisialisasi objek LiquidCrystal_I2C dengan alamat I2C 0x27 dan jumlah kolom dan baris LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

int power = HIGH;  // Asumsi awalnya HIGH
const int button2 = 8;
const int ledPutih = 3;
const int ledBiru = 4;
const int ledMerah = 2;
const int buzzer = 5;

int lap = 0;

unsigned long previousMillis = 0;
unsigned long currentMillis = 0;
unsigned long interval = 1000;     // Interval waktu dalam milidetik (1000 ms = 1 detik)
const long buzzDuration = 300;     // Durasi untuk menyalakan buzzer (ms)
const long silentDuration = 2000;  // Durasi untuk diamkan buzzer setelah berdering (ms)

byte hours = 0;
byte minutes = 0;
byte seconds = 0;

void setup() {
  Serial.begin(9600);
  pinMode(ledPutih, OUTPUT);
  pinMode(ledBiru, OUTPUT);
  pinMode(ledMerah, OUTPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(9, INPUT);  // Gantilah 8 dengan pin yang sesuai untuk input power
  pinMode(button2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);  // Mengatur LED bawaan sebagai output
  lcd.init();
  lcd.backlight();
}

int counter = 0;
int timeMode = 0;
int buzzerStatus = 0;
int statusWaktu = 0;

void loop() {
  digitalWrite(LED_BUILTIN, LOW);  // Matikan LED

  power = digitalRead(9);
  int confirmButton = digitalRead(button2);

  if (confirmButton == LOW) {
    lcd.clear();
    timeMode = 2;
    buzzerStatus = 0;
    digitalWrite(ledPutih, LOW);
  }



  if (lap >= 6) {

    lcd.setCursor(3, 0);
    lcd.print("Your task");
    lcd.setCursor(3, 1);
    lcd.print("is finished");
    digitalWrite(ledMerah, HIGH);
    digitalWrite(ledBiru, HIGH);
    digitalWrite(ledPutih, HIGH);


  } else {
    if (buzzerStatus == 1) {
      confBuzzer();
    }
    if (counter == 1) {

      if (timeMode == 1) {

        mytime();
        work();
      }

      if (statusWaktu == 0) {
        if (timeMode == 2) {
          mytime();
          work();
        }
      } else if (statusWaktu == 1) {
        if (timeMode == 2) {
          mytime();
          rest();
        }
      }


    } else {
      lcd.setCursor(3, 0);
      lcd.print("Hello sir!");
      lcd.setCursor(1, 1);
      lcd.print("ready to start?");
      if (power == LOW) {
        delay(1000);
        lcd.clear();
        counter = 1;
        timeMode = 1;
      }
    }
  }
}

void mytime() {
  currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;

    seconds++;

    if (seconds >= 60) {
      seconds = 0;
      minutes++;

      if (minutes >= 60) {
        minutes = 0;
        hours++;

        if (hours >= 24) {
          hours = 0;
        }
      }
    }
    updateDisplay();
  }
}


void confBuzzer() {
  unsigned long currentMillis = millis();

  // Menyalakan buzzer
  if (currentMillis - previousMillis < buzzDuration) {
    analogWrite(buzzer, 6);  // Mengatur volume buzzer
  }
  // Mematikan buzzer setelah durasi tertentu
  else if (currentMillis - previousMillis < buzzDuration + silentDuration) {
    analogWrite(buzzer, 0);  // Mematikan buzzer
  } else {
    // Reset waktu penundaan
    previousMillis = currentMillis;
  }
}


void work() {
  digitalWrite(ledMerah, LOW);
  digitalWrite(ledBiru, HIGH);


  if (hours == 0 && minutes == 25 && seconds == 1) {
    hours = 0;
    minutes = 0;
    seconds = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Istirahat!");

    digitalWrite(ledPutih, HIGH);
    buzzerStatus = 1;

    statusWaktu = 1;
    timeMode = 0;
    if (lap >= 5) {
      lcd.clear();
    }

    lap++;
  }
}

void rest() {
  digitalWrite(ledBiru, LOW);
  digitalWrite(ledMerah, HIGH);

  if (hours == 0 && minutes == 5 && seconds == 1) {
    hours = 0;
    minutes = 0;
    seconds = 0;

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Produktif!");

    digitalWrite(ledPutih, HIGH);
    buzzerStatus = 1;
    statusWaktu = 0;
    timeMode = 0;
  }
}


void updateDisplay() {
  lcd.setCursor(4, 0);
  if (hours < 10) lcd.print("0");
  lcd.print(hours);
  lcd.print(":");
  if (minutes < 10) lcd.print("0");
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10) lcd.print("0");
  lcd.print(seconds);
}
