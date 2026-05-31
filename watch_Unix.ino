#include <TimerOne.h>
#include <EEPROM.h>

int years = 1970, months = 0, day = 1, hours = 0, minuts = 0, seconds = 0; 
unsigned long tSecond = 0, calcTime, longUnix, timeSave;


void setup() {
  Serial.begin(9600);

  EEPROM.get(0, longUnix);

  if (longUnix <= 0) {

    Serial.println("");
    Serial.print("Введите дату в формате UNIX timestamp: ");


    while (Serial.available() == 0) {
      
    }

    longUnix = Serial.parseInt();

    if (longUnix > 0) {
      Serial.println(longUnix);
      Serial.println();
      Serial.println("***_ Дата установлена _***");
      Serial.println();

      calcTime = longUnix;
      calculation();

      Timer1.initialize(1000000);
      Timer1.attachInterrupt(oneSecond);  
    }  
  } else {
    Serial.println("Время загружено из буфера");
    Serial.println(longUnix);

    Timer1.initialize(1000000);
    Timer1.attachInterrupt(oneSecond);  
  }



}

void oneSecond() {
  tSecond += 1;
}

void calculation(){
  int i = 0;
  years = 1970;

  long calcDays = (calcTime) / 86400; // Считаем общее кол-во дней
  long calcSeconds = (calcTime) % 86400; // Считаем Общее кол-во секунд за вычетом дней

  hours = calcSeconds / 3600; // Кол-во часов на часах
  minuts = (calcSeconds % 3600) / 60; // Кол-во минут на часах
  seconds = calcSeconds % 60; // Кол-во секунд на часах


  int daysPerMonth[12] {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

  while (calcDays > daysPerMonth[i]) {


    calcDays -= daysPerMonth[i];
    i += 1;
    if (i == 12) {
      years += 1;
      i = 0;
    }

    if (years % 4 == 0) daysPerMonth[1] = 29;
    else daysPerMonth[1] = 28;
  }


  int months = i + 1;
  int days = calcDays + 1;

  Serial.println("Дата: ");
  Serial.print("years:");
  Serial.print(years);
  Serial.print(" ");
  Serial.print("months:");
  Serial.print(months);
  Serial.print(" ");
  Serial.print("days:");
  Serial.println(days);

  Serial.println("Время:");
  Serial.print(hours);
  Serial.print(" : ");
  Serial.print(minuts);
  Serial.print(" : ");
  Serial.print(seconds);  
}



void loop() {

  if (millis() - timeSave > 600000) {
    EEPROM.put(0, longUnix + tSecond);
    timeSave = millis();
    Serial.println();
    Serial.println("Дата сохранена в буфер");
  }

  if (Serial.available() > 0) {
    char incoming = Serial.read();

    if (incoming == 'G') {
      Serial.println();
      Serial.println("-------------------------------");
      Serial.println();
      Serial.print("Пройденное кол-во секунд:");
      Serial.println(tSecond);
      calcTime = longUnix + tSecond;
      Serial.println();
      Serial.println("Часы показывают, что сейчас:");
      Serial.println();
      calculation();
    } 

    if (incoming == 'S') {
      Serial.println();
      Serial.println("-------------------------------");
      Serial.println();
      Serial.print("Введите новую дату в формате UNIX timestamp: ");

      while (Serial.available() == 0) {
        
      }

      longUnix = Serial.parseInt();

      if (longUnix > 0) {
        Serial.println(longUnix);
        Serial.println();
        Serial.println("***_ Дата установлена _***");
        Serial.println();

        calcTime = longUnix;
        calculation();
        tSecond = 0;
      }
    }

    if (incoming == 'O') {
      long resetValue = 0;
      EEPROM.put(0, resetValue);
      Serial.println();
      Serial.println("Буфер сброшен");
    }
  }
}
