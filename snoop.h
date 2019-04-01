#ifndef Snoop_h
#define Snoop_h

#define E_PIN 55
#define VMA_PIN 48
#define RW_PIN 47

#define D0_PIN 2
#define D1_PIN 14
#define D2_PIN 7
#define D3_PIN 8
#define D4_PIN 6
#define D5_PIN 20
#define D6_PIN 21
#define D7_PIN 5

//#define LATCH_PIN 24
#define A0_PIN 16
#define A1_PIN 17
#define A2_PIN 19
#define A3_PIN 18
#define A4_PIN 49
#define A5_PIN 50
#define A6_PIN 31
#define A7_PIN 32
#define A8_PIN 0
#define A9_PIN 1
#define A10_PIN 29
#define A11_PIN 30
#define A12_PIN 43
#define A13_PIN 46
#define A14_PIN 44
#define A15_PIN 45
#define DEBUG_PIN 27

#define CA1_CTL0   0
#define CA1_CTL1  1
#define DDRA_XS   2
#define CA2_CTL0  3
#define CA2_CTL1  4
#define CA2_CTL2  5
#define IRQA2     6
#define IRQA1     7

#define CB1_CTL0  0
#define CB1_CTL1  1
#define DDRB_XS   2
#define CB2_CTL0  3
#define CB2_CTL1  4
#define CB2_CTL2  5
#define IRQB2     6
#define IRQB1     7

//! structure of a frame
typedef struct Snoop {
  uint16_t addr;    //!< address lines
  uint8_t data;    //!< data lines
  uint8_t ctrl;    //!< ctrl lines
} Snoop;

#define SNOOP_BUFFER_SIZE (16384*8)

volatile uint8_t snoopbuffer[SNOOP_BUFFER_SIZE];
volatile long snoophead=0;
volatile long snooptail=0;

volatile bool snoop_data_available=0;

/*

class Snoop6800 {
  public:
  Snoop6800();
  

  
}

*/

#endif
