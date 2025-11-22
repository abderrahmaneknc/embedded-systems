#include <Arduino.h>

char tab[4][3] = {
    {'1', '2', '3'},
    {'4', '5', '6'},
    {'7', '8', '9'},
    {'*', '0', '#'}};

int rows[4] = {(1 << PD2), (1 << PD3), (1 << PD4), (1 << PD5)};
int cols[3] = {(1 << PB1), (1 << PB2), (1 << PB3)};
void usart_init()
{
  UBRR0L = 103; // 9600 bauds pour F_CPU = 16MHz
  UCSR0B = (1 << TXEN0);
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
void usart_send(unsigned char data)
{
  while (!(UCSR0A & (1 << UDRE0)))
    ;
  UDR0 = data;
}

void setup()
{
  Serial.begin(9600);

  DDRD |= 0b00111100;
  PORTD |= 0b00111100;

  DDRB &= ~0b00001110;
  PORTB |= 0b00001110;
  usart_init();
}

void loop()
{
  for (int r = 0; r < 4; r++)
  {
    PORTD &= ~rows[r];

    for (int c = 0; c < 3; c++)
    {
      if ((PINB & cols[c]) == 0)
      {
        Serial.println(tab[r][c]);
        usart_send(tab[r][c]);
        delay(200);
      }
    }

    PORTD |= rows[r];
  }
}
