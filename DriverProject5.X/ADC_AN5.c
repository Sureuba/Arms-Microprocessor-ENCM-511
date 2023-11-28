AD1PCFG = 0xFFFE; // Configure A/D port
// AN0 input pin is analog
AD1CON1 = 0x2202; // Configure sample clock source
// and conversion trigger mode.
// Unsigned Fraction format (FORM<1:0>=10),
// Manual conversion trigger (SSRC<2:0>=000),
// Manual start of sampling (ASAM=0),
// No operation in Idle mode (ADSIDL=1),
// S/H in Sample (SAMP = 1)
AD1CON2 = 0; // Configure A/D voltage reference
// and buffer fill modes.
// Vr+ and Vr- from AVdd and AVss (VCFG<2:0>=000),
// Inputs are not scanned,
// Interrupt after every sample
AD1CON3 = 0x0100; // Configure sample time = 1Tad,
// A/D conversion clock as Tcy
AD1CHS = 0; // Configure input channels,
// S/H+ input is AN0,
// S/H- input is Vr- (AVss).
AD1CSSL = 0; // No inputs are scanned.
IFS0bits.AD1IF = 0; // Clear A/D conversion interrupt.
// Configure A/D interrupt priority bits (AD1IP<2:0>) here, if
// required. Default priority level is 4.
IEC0bits.AD1IE = 1; // Enable A/D conversion interrupt
AD1CON1bits.ADON = 1; // Turn on A/D
AD1CON1bits.SAMP = 1; // Start sampling the input
Delay(); // Ensure the correct sampling time has elapsed
// before starting conversion.
AD1CON1bits.SAMP = 0; // End A/D sampling and start conversion
// Example code for A/D ISR:



void __attribute__ ((__interrupt__)) _ADC1Interrupt(void)
{
IFS0bits.AD1IF = 0;
}