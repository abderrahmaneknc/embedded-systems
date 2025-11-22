
// #include <Wire.h>

// void setup()
// {
//   Wire.begin(); // Master
//   Serial.begin(9600);
// }

// void loop()
// {
//   // Allumer LED
//   Wire.beginTransmission(8); // adresse Arduino LED
//   Wire.write(1);             // 1 = ON
//   Wire.endTransmission();
//   delay(1000);

//   // Éteindre LED
//   Wire.beginTransmission(8);
//   Wire.write(0); // 0 = OFF
//   Wire.endTransmission();
//   delay(1000);
// }

// #include <Wire.h>

// void setup()
// {
//   pinMode(13, OUTPUT);
//   Wire.begin(8);
//   Wire.onReceive(receiveEvent);
// }

// void loop()
// {
//   delay(1000);
// }

// void receiveEvent(int bytes)
// {
//   if (Wire.available())
//   {
//     int cmd = Wire.read();
//     if (cmd == 1)
//       digitalWrite(13, HIGH); // Allumer LED
//     else
//       digitalWrite(13, LOW); // Éteindre LED
//   }
// }

// #include <Wire.h>
// #include <Keypad.h>

// const byte ROWS = 4;
// const byte COLS = 4;

// char keys[ROWS][COLS] = {
//     {'1', '2', '3', 'A'},
//     {'4', '5', '6', 'B'},
//     {'7', '8', '9', 'C'},
//     {'*', '0', '#', 'D'}};

// byte rowPins[ROWS] = {9, 8, 7, 6};
// byte colPins[COLS] = {5, 4, 3, 2};

// Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// void setup()
// {
//   Wire.begin(); // Master mode
//   Serial.begin(9600);
// }

// void loop()
// {
//   char key = keypad.getKey();
//   if (key)
//   {
//     Serial.print("Touche appuyée : ");
//     Serial.println(key);

//     Wire.beginTransmission(9); // adresse du slave
//     Wire.write(key);           // envoie la touche
//     Wire.endTransmission();
//     delay(200); // petit délai pour éviter les rebonds
//   }
// }

// #include <Wire.h>
// #include <Arduino.h>
// int a = 2;
// int b = 3;
// int c = 4;
// int d = 5;
// int e = 6;
// int f = 7;
// int g = 8;
// void afficherChiffre(int n)
// {
//   // tableau de chiffres de 0 à 9
//   bool chiffres[10][7] = {
//       {1, 1, 1, 1, 1, 1, 0}, // 0
//       {0, 1, 1, 0, 0, 0, 0}, // 1
//       {1, 1, 0, 1, 1, 0, 1}, // 2
//       {1, 1, 1, 1, 0, 0, 1}, // 3
//       {0, 1, 1, 0, 0, 1, 1}, // 4
//       {1, 0, 1, 1, 0, 1, 1}, // 5
//       {1, 0, 1, 1, 1, 1, 1}, // 6
//       {1, 1, 1, 0, 0, 0, 0}, // 7
//       {1, 1, 1, 1, 1, 1, 1}, // 8
//       {1, 1, 1, 1, 0, 1, 1}  // 9
//   };

//   for (int i = 0; i < 7; i++)
//   {
//     digitalWrite(a + i, chiffres[n][i]);
//   }
// }

// void receiveEvent(int bytes)
// {
//   char key = Wire.read();
//   if (key >= '0' && key <= '9')
//   {
//     afficherChiffre(key - '0');
//   }
// }

// void setup()
// {
//   Wire.begin(9); // adresse du slave
//   Wire.onReceive(receiveEvent);

//   pinMode(a, OUTPUT);
//   pinMode(b, OUTPUT);
//   pinMode(c, OUTPUT);
//   pinMode(d, OUTPUT);
//   pinMode(e, OUTPUT);
//   pinMode(f, OUTPUT);
//   pinMode(g, OUTPUT);
// }

// void loop()
// {
//   delay(100);
// }

//--------------------------------------------------------------------- the master code--------------------------------------------------------------------------------------------------------------------------------------
#include <Wire.h>
#include <Keypad.h>

// Keypad configuration
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};
byte rowPins[ROWS] = {9, 8, 7, 6};
byte colPins[COLS] = {5, 4, 3};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables pour LED
unsigned long previousMillis = 0;
const long interval = 1000;
bool ledState = false;

void setup()
{
  Wire.begin();
  Serial.begin(9600);
}

void loop()
{
  unsigned long currentMillis = millis(); // obtenir le temps actuel
  //  Allumer / éteindre LED sur Slave1 toutes les 1s  
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    ledState = !ledState; // alterne LED
    Wire.beginTransmission(8);
    Wire.write(ledState ? 1 : 0);
    Wire.endTransmission();
  }

  //  Lire Keypad et envoyer à Slave2 immédiatement
  char key = keypad.getKey();
  if (key)
  {
    Serial.print("Touche appuyée : ");
    Serial.println(key);
    Wire.beginTransmission(9); // adresse Slave2 7-segment
    Wire.write(key);
    Wire.endTransmission();
  }
}

// ------------------------------------------------------led code---------------------------------------------------------------------------------------------------------------------------------------

#include <Wire.h>
#include <Arduino.h>
void receiveEvent(int bytes)
{
  if (Wire.available())
  {
    int cmd = Wire.read();
    if (cmd == 1)
      digitalWrite(13, HIGH); // Allumer LED
    else
      digitalWrite(13, LOW); // Éteindre LED
  }
}

void setup()
{
  pinMode(13, OUTPUT);
  Wire.begin(8);
  Wire.onReceive(receiveEvent);
}

void loop()
{
  delay(1000);
}

// ------------------------------------------------------7-segment code---------------------------------------------------------------------------------------------------------------------------------------

#include <Wire.h>
#include <Arduino.h>
int a = 2;
int b = 3;
int c = 4;
int d = 5;
int e = 6;
int f = 7;
int g = 8;
void afficherChiffre(int n)
{
  // tableau de chiffres de 0 à 9
  bool chiffres[10][7] = {
      {1, 1, 1, 1, 1, 1, 0}, // 0
      {0, 1, 1, 0, 0, 0, 0}, // 1
      {1, 1, 0, 1, 1, 0, 1}, // 2  
      {1, 1, 1, 1, 0, 0, 1}, // 3
      {0, 1, 1, 0, 0, 1, 1}, // 4
      {1, 0, 1, 1, 0, 1, 1}, // 5
      {1, 0, 1, 1, 1, 1, 1}, // 6
      {1, 1, 1, 0, 0, 0, 0}, // 7
      {1, 1, 1, 1, 1, 1, 1}, // 8
      {1, 1, 1, 1, 0, 1, 1}  // 9
  };

  for (int i = 0; i < 7; i++)
  {
    digitalWrite(a + i, chiffres[n][i]);
  }
}

void receiveEvent(int bytes)
{
  char key = Wire.read();
  if (key >= '0' && key <= '9')
  {
    afficherChiffre(key - '0');
  }
}

void setup()
{
  Wire.begin(9); // adresse du slave
  Wire.onReceive(receiveEvent);

  pinMode(a, OUTPUT);
  pinMode(b, OUTPUT);
  pinMode(c, OUTPUT);
  pinMode(d, OUTPUT);
  pinMode(e, OUTPUT);
  pinMode(f, OUTPUT);
  pinMode(g, OUTPUT);
}

void loop()
{
  delay(100);
}
