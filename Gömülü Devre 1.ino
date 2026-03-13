#include <LiquidCrystal.h>
#include <Servo.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// Pin Tanımlamaları
const int trigPin = 13;
const int echoPin = 10;
const int servoPin = 6;
const int motorEn = 9;
const int motorIn1 = 8;
const int motorIn2 = 7;
const int ledR = A1;
const int ledG = A2;
const int ledB = A3;

Servo ayirici;

// Sayaçlar
int kirmiziSayisi = 0;
int yesilSayisi = 0;
int maviSayisi = 0;

const int sabitHiz = 130; 
int renkSirasi = 1; // 1=Yeşil, 2=Mavi, 3=Kırmızı

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  ayirici.attach(servoPin);
  ayirici.write(0);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(motorEn, OUTPUT);
  pinMode(motorIn1, OUTPUT);
  pinMode(motorIn2, OUTPUT);
  pinMode(ledR, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledB, OUTPUT);

  lcd.print("SISTEM HAZIR");
  delay(1500);
  lcd.clear();
}

void loop() {
  long mesafe = mesafeOlc();

  if (mesafe > 0 && mesafe < 10) {
    motorDurdur();
    lcd.clear();
    lcd.print("RENK OKUNUYOR...");

    // Renk döngüsü (simülasyon için)
    int renkKodu = renkOku();

    motorCalistir();
    delay(1200);
    motorDurdur();

    lcd.clear();
    if (renkKodu == 1) {
      lcd.print("RENK: YESIL");
      yesilSayisi++;
      ayirici.write(90);
      digitalWrite(ledG, HIGH); // Yeşil LED yanar
      delay(500);
      digitalWrite(ledG, LOW);
    } else if (renkKodu == 2) {
      lcd.print("RENK: MAVI");
      maviSayisi++;
      ayirici.write(135);
      digitalWrite(ledB, HIGH); // Mavi LED yanar
      delay(500);
      digitalWrite(ledB, LOW);
    } else if (renkKodu == 3) {
      lcd.print("RENK: KIRMIZI");
      kirmiziSayisi++;
      ayirici.write(45);
      digitalWrite(ledR, HIGH); // Kırmızı LED yanar
      delay(500);
      digitalWrite(ledR, LOW);
    }

    delay(1500);
    ayirici.write(0);
    delay(500);
    lcd.clear();
  } else {
    motorCalistir();

    lcd.setCursor(0, 0);
    lcd.print("BANT CALISIYOR  ");

    lcd.setCursor(0, 1);
    lcd.print("K:"); lcd.print(kirmiziSayisi);
    lcd.print(" Y:"); lcd.print(yesilSayisi);
    lcd.print(" M:"); lcd.print(maviSayisi);
    lcd.print("    ");
  }
}

// --- Fonksiyonlar ---

void motorCalistir() {
  digitalWrite(motorIn1, HIGH);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEn, sabitHiz);
}

void motorDurdur() {
  digitalWrite(motorIn1, LOW);
  digitalWrite(motorIn2, LOW);
  analogWrite(motorEn, 0);
}

long mesafeOlc() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long sure = pulseIn(echoPin, HIGH);
  return (sure * 0.034) / 2;
}

// --- Simülasyon için renk döngüsü ---
int renkOku() {
  int renk = renkSirasi;

  // Döngüyü sırayla değiştir
  renkSirasi++;
  if (renkSirasi > 3) renkSirasi = 1;

  return renk;
}