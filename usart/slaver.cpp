#include <Arduino.h>

unsigned char digits[10]{
    0b00111111,
    0b00000110,
    0b01011011,
    0b01001111,
    0b01100110,
    0b01101101,
    0b01111101,
    0b00000111,
    0b01111111,
    0b01101111

};

void usart_init()
{
    UBRR0L = 103; // 9600 bauds pour F_CPU = 16MHz
    UCSR0B = (1 << RXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}
unsigned char usart_receive()
{
    while (!(UCSR0A & (1 << RXC0)))
        ;
    return UDR0;
}

void setup()
{
    DDRD |= 0b11111100;
    PORTD &= ~0b11111100;
    DDRB |= 0b00000010;
    PORTB &= ~0b00000010;
    usart_init();
}

void loop()
{
    unsigned char data = usart_receive();

    if (data >= '0' && data <= '9')
    {
        unsigned char val = digits[data - '0'];

        PORTD = (PORTD & 0b00000011) | (val << 2);

        if (val & 0b01000000)
            PORTB |= (1 << PB1);
        else
            PORTB &= ~(1 << PB1);
    }
}
