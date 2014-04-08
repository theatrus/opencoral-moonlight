#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

volatile char ticks;
volatile uint32_t clock;
#define PWM_MIN 3
#define PWM_MAX 32768

void timer_init(void)
{
    TIMSK0 = 1 | (1 << 1);
    TCCR0B = (1 << 2); /* / 256 from prescaler */
    OCR0A = 0xff;
}

ISR(TIMER0_OVF_vect)
{
    ticks++;
    if (ticks >= 122) {
        clock++;
        ticks = 0;
    }
}

void pwm_init(void)
{
    ICR1 = PWM_MAX;
    OCR1A = PWM_MIN;
    OCR1B = PWM_MIN;
    //OCR1B = 0x10;
    TCCR1A = (1 << 7) | (1 << 5) | (1 << 1); // Table 15-4
    // Set for fast pwm, OCR1A TOP
    TCCR1B = (1 << 4) | (1 << 3) |  (1 << 0);
    /* /1 from prescaler, WGM13 set */
}

int main()
{
    unsigned int i = 0;
    DDRB = (1 << 1) | (1 << 2);
    PORTB = 0;//(1 << 1) | ( 1 << 2);
    pwm_init();
    timer_init();
    for (i = 3; i < 10384; i+=10) {
        OCR1A = i;
        OCR1B = i;
        _delay_ms(1);
    }

    while(1) { asm volatile ("nop");}
}
