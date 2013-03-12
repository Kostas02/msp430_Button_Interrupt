#include "msp430.h"

#define RED_LED BIT0
#define GREEN_LED BIT6
#define BUTTON BIT3


void init_clocks()
{
    // Disable WDT
    WDTCTL = WDTPW + WDTHOLD; // watchdog timer setup
    // Set the calibrating things for the DCO
    BCSCTL1 = CALBC1_1MHZ; // Set range
    DCOCTL = CALDCO_1MHZ; // Set DCO step + modulation
    // ACLK runs from VLO
    BCSCTL3 |= LFXT1S_2;
    // Select DCO as MCLK and the /8 divider (125KHz)
    BCSCTL2 |= SELM_0 + DIVM_3;
}

void init_gpio()
{
    // P1.0 and 1.6 as outputs and leds off
    P1DIR |= RED_LED + GREEN_LED;
    P1OUT &= ~(RED_LED + GREEN_LED);
    // Button as input, output high for pullup, and pullup on
    P1DIR &= ~BUTTON;
    P1OUT |= BUTTON;
    P1REN |= BUTTON;
    // P1 interrupt enabled with P1.3 as trigger
    P1IE |= BUTTON;
    // P1.3 interrupt flag cleared.
    P1IFG &= ~BUTTON;
}


int main()
{
    init_clocks();
    init_gpio();

    // Enable global interrupts (and LPM if needed)
    //_BIS_SR(LPM0_bits + GIE);
    _BIS_SR(GIE);

    while(1)
    {
        // Flashing green led once in main program loop
        P1OUT ^= GREEN_LED;
        __delay_cycles(50000);
        P1OUT ^= GREEN_LED;
        // And then go in low power mode
        LPM0;
    }
    return 0;
}


__attribute__( (interrupt (PORT1_VECTOR)) )
void PORT1_ISR()
{
    // PORT1 interrupt ISR
    // Just toggling red led to show interrupt occuring
    P1OUT ^= RED_LED;
    //Clearing IFG. This is required.
    P1IFG &= ~BUTTON;
    // Toggle interrupt edge (makes it always trigger)
    P1IES ^= BUTTON;
    // Exit lpm on ISR exit (green led flashing once again)
    __bic_SR_register_on_exit(LPM0_bits);
}

