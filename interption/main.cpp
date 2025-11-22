#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define LED1 13
#define LED2 12
#define LED3 11
#define BTN 4

volatile int LED1_state = LOW;
volatile int brightness = 0;
volatile bool print_flag = false;
volatile unsigned int timer2_count = 0;
void setup()
{
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(BTN, INPUT_PULLUP);

  Serial.begin(9600);

  cli();

  // LED1 : Timer1 1s
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;
  OCR1A = 15625;                       // 16MHz / 1024 ≈ 1s
  TCCR1B |= (1 << WGM12);              // CTC
  TCCR1B |= (1 << CS12) | (1 << CS10); // prescaler 1024
  TIMSK1 |= (1 << OCIE1A);             // enable interrupt

  //  Timer2 : flag impression brightness toutes les 5s
  TCCR2B = 0;
  TCNT2 = 0;
  TCCR2B |= (1 << CS22) | (1 << CS21) | (1 << CS20);
  TIMSK2 |= (1 << TOIE2);

  sei();
}

void loop()
{
  //  LED2 : bouton
  if (digitalRead(BTN) == LOW)
  {
    digitalWrite(LED2, HIGH);
  }
  else
  {
    digitalWrite(LED2, LOW);
  }

  //  LED3 : Serial
  if (Serial.available())
  {
    String input = Serial.readStringUntil('\n');
    int val = input.toInt();
    if (val >= 0 && val <= 255)
    {
      brightness = val;
      analogWrite(LED3, brightness);
    }
  }

  //  Impression brightness toutes les 5s
  if (print_flag)
  {
    Serial.print("Brightness: ");
    Serial.println(brightness);
    print_flag = false;
  }
}

//  ISR Timer1 : LED1 toggle every 1s
ISR(TIMER1_COMPA_vect)
{
  LED1_state = !LED1_state;
  digitalWrite(LED1, LED1_state);
}

//  ISR Timer2 : flag impression brightness every 5s
ISR(TIMER2_OVF_vect)
{
  timer2_count++;
  if (timer2_count >= 312)
  {
    print_flag = true;
    timer2_count = 0;
  }
}
