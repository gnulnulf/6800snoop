
/*
#define E_PIN 32
#define VMA_PIN 31
#define RW_PIN 26

#define D0_PIN 16
#define D1_PIN 17
#define D2_PIN 19
#define D3_PIN 18
#define D4_PIN 0
#define D5_PIN 1
#define D6_PIN 29
#define D7_PIN 30

#define LATCH_PIN 24
#define A0_PIN 33
#define A1_PIN 34
*/




uint8_t cra,crb,ddra,ddrb,pdra,pdrb,lampreadready;
uint8_t swcra,swcrb,swddra,swddrb,swpdra,swpdrb,switchreadready;


void snoopInit() {
   pinMode(E_PIN, INPUT); 
   pinMode(VMA_PIN, INPUT); 
   pinMode(RW_PIN, INPUT);
   pinMode(D0_PIN, INPUT); 
   pinMode(D1_PIN, INPUT); 
   pinMode(D2_PIN, INPUT); 
   pinMode(D3_PIN, INPUT); 
   pinMode(D4_PIN, INPUT); 
   pinMode(D5_PIN, INPUT); 
   pinMode(D6_PIN, INPUT); 
   pinMode(D7_PIN, INPUT); 

   pinMode(A0_PIN, INPUT); 
   pinMode(A1_PIN, INPUT); 
   pinMode(A2_PIN, INPUT); 
   pinMode(A3_PIN, INPUT); 
   pinMode(A4_PIN, INPUT); 
   pinMode(A5_PIN, INPUT); 
   pinMode(A6_PIN, INPUT); 
   pinMode(A7_PIN, INPUT); 
   pinMode(A8_PIN, INPUT); 
   pinMode(A9_PIN, INPUT); 
   pinMode(A10_PIN, INPUT); 
   pinMode(A11_PIN, INPUT); 
   pinMode(A12_PIN, INPUT); 
   pinMode(A13_PIN, INPUT); 
   pinMode(A14_PIN, INPUT); 
   pinMode(A15_PIN, INPUT); 

//SYST_CSR &= ~SYST_CSR_TICKINT;


   //pinMode(LATCH_PIN, OUTPUT); 
   //pinMode(A0_PIN, OUTPUT); 
   //pinMode(A1, OUTPUT); 
   pinMode(DEBUG_PIN, OUTPUT); 
//   attachInterruptvector(E_PIN, snoop_isr, CHANGE); 
//...hardware\teensy\avr\cores\teensy3\kinetis.h
CORE_PIN55_CONFIG = (1<<24) | (0b1011 <<16) | (0b001 <<8) ; 
//CORE_PIN55_CONFIG = (1<<24) | (0b1001 <<16) | (0b001 <<8) ; 
attachInterruptVector(IRQ_PORTD, snoop_isr); 
NVIC_SET_PRIORITY(IRQ_PORTD, 0);
//Serial.println(IRQ_PORTD);
//   attachInterrupt(E_PIN, snoop_isr, RISING); 

}

uint8_t snoop_isr_active=0;
uint8_t vmapin=0;
uint8_t rwpin=0;
uint8_t readdataondown=0;
uint32_t inputb;
uint32_t inputd;

uint8_t data=0;
uint8_t swdata=0;
volatile uint16_t address=0;
volatile uint16_t addresssave=0;


FASTRUN void snoop_isr() {

  // do not allow more then one isr
 // if ( snoop_isr_active ) {return;}
  //cli();
  //snoop_isr_active=1;

  uint32_t inputd=GPIOD_PDIR;
  uint32_t inputb=GPIOB_PDIR;
  //uint8_t epin = digitalReadFast( E_PIN );
  //uint8_t epin = (inputd & (1<<11))?1:0;

  // e-pin high?
  if ( inputd & (1<<11) ) {
    // up
    //get vma
//    vmapin = digitalReadFast( VMA_PIN );
    //vmapin = (inputd & (1<<8))?1:0;
//    if ( ! (inputd & (1<<8))  ) {snoop_isr_active=0;sei();return;}
    if ( ! (inputd & (1<<8))  ) {
          CORE_PIN55_CONFIG |= (1<<24);
          sei();
          return;
     }
//    rwpin = digitalReadFast( RW_PIN );
    //rwpin = (inputd & (1<<9))?1:0;
// 0b00000000 11111111 00001100 00111111
    uint16_t address= (inputb & 0x3f) | (inputb & 0x0c00 )>>4 |(inputb & 0xFF0000 )>>8;   
    //latch
    //set 138-a0
    //read addrh
    //set 138-a1
    //read addrl

    // write?
    if ( ! (inputd & (1<<9)) ) {
      //set 138-a2
      //read data
       uint8_t data = inputd & 0xff;
      //write data to ram
      if ( address < RAMSIZE ) {
          ram[ address ] = data;
      }
    
  /*    if ( 0 && address > 0x2F00 && address < 0x31000) {
//      if ( (address & 0xFFF8)==0x2400 ) {
        if ( snoophead >= snooptail ) {
        //IO
        snoopbuffer[snoophead] = (address>>8);
        snoopbuffer[snoophead+1] = (address&0xff);
        snoopbuffer[snoophead+2] = data;
        snoopbuffer[snoophead+3] = 0;
        snoophead+=4;
        //overflow does not realy matter yet
        if (snoophead > SNOOP_BUFFER_SIZE ) snoophead=0;
        }
      }
*/
// LAMPS PIA
    if ( address == 0x2400 ) {
        //if ( data != 0xff ) {
          if ( cra & (1<<DDRA_XS)) {
            if ( lampreadready) {
              lampreadready=0;
            if ( pdrb == 0x80 ) lamps[7] = data;
            if ( pdrb == 0x40 ) lamps[6] = data;
            if ( pdrb == 0x20 ) lamps[5] = data;
            if ( pdrb == 0x10 ) lamps[4] = data;
            if ( pdrb == 0x08 ) lamps[3] = data;
            if ( pdrb == 0x04 ) lamps[2] = data;
            if ( pdrb == 0x02 ) lamps[1] = data;
            if ( pdrb == 0x01 ) lamps[0] = data;
            }
          }
        //if ( cra & (1<<DDRA_XS)) pdra = data;
      
      }
     //2402 is the row
     if ( address == 0x2402 ) {
      
        //if ( data != 0xff ) {
        if ( crb & (1<<DDRB_XS)) {
          // we want only the first write
          lampreadready=1;
          pdrb = data;
        }
     }
     if ( address == 0x2401 ) cra=data;
     if ( address == 0x2403 ) crb=data;







     //if ( address &FFF0 == 0x2400 ) {
//  lamps pia
//cra,crb,ddra,ddrb,pdra,pdrb

    //cra=addr+1;
  //crb=addr+3;
  //ddra=addr;
  //ddrb=addr+2;
  //pdra=ddra;
  //pdrb=ddrb;
  
    //}


//switch PIA
   if ( address == 0x3000 ) {
      if ( swcra & (1<<DDRA_XS)) {
         swpdra = data;
      } else {
         swddra= data;
      }
   }       

    //3002 is the row
     if ( address == 0x3002 ) {
        if ( swcrb & (1<<DDRB_XS)) {
              swpdrb = data;
          } else {
            swddrb= data;
          }
     }
           if ( address == 0x3001 ) swcra=data;
           if ( address == 0x3003 ) swcrb=data;




    
    } else {
      //read
   readdataondown=1;
    addresssave=address;
     // inputd=GPIOD_PDIR;
      //data = inputd & 0xff;
      //write data to ram

 
    }
    
  } else {
    //down
  
    if ( readdataondown ) {
      readdataondown=0;
      //latch
      //read data
      uint8_t data = inputd & 0xff;
      //write data to ram
      if ( addresssave < RAMSIZE ) ram[ address ] = data;

      //switchdata
      
   if ( addresssave == 0x3001 ) swcra=data;
   if ( addresssave == 0x3003 ) swcrb=data;

   if ( addresssave == 0x3000 ) {
      if ( swcra & (1<<DDRA_XS)) {
         swpdra = data;
            if ( swpdrb == 0x80 ) switches[7] = data;
            if ( swpdrb == 0x40 ) switches[6] = data;
            if ( swpdrb == 0x20 ) switches[5] = data;
            if ( swpdrb == 0x10 ) switches[4] = data;
            if ( swpdrb == 0x08 ) switches[3] = data;
            if ( swpdrb == 0x04 ) switches[2] = data;
            if ( swpdrb == 0x02 ) switches[1] = data;
            if ( swpdrb == 0x01 ) switches[0] = data;
      
  } else {
         swddra= data;
      }
   }       
 if ( addresssave == 0x3002 ) {
        if ( swcrb & (1<<DDRB_XS)) {
              swpdrb = data;
          } else {
              swddrb= data;
          }
     }
      /*
      if ( 0 && address > 0x2900 && address < 0x3100) {
        //IO
        snoopbuffer[snoophead] = (address>>8);
        snoopbuffer[snoophead+1] = (address&0xff);
        snoopbuffer[snoophead+2] = data;
        snoopbuffer[snoophead+3] = 1;
        snoophead+=4;
        //overflow does not realy matter yet
        if (snoophead > SNOOP_BUFFER_SIZE ) snoophead=0;
        
      }
      */
    } //readdataondown
    } //else



/*
      switches[0] = swddra;
      switches[1] = swpdra;
      switches[2] = swcra;
      
      switches[3] = swdata;
      switches[4] = swcra;
      switches[5] = swcrb;
      switches[6] = swpdra;
      switches[7] = swpdrb;
*/



  //snoop_isr_active=0;

/*  const uint32_t pinISF = 1 << 24;
const uint8_t interruptPin = 3;

void setup() {
  volatile uint32_t *pinConfigRegPtr;
  uint32_t pinConfigRegValue;

  pinConfigRegPtr = portConfigRegister(interruptPin);
  pinConfigRegValue = *pinConfigRegPtr;
  pinConfigRegValue |= pinISF;
  *pinConfigRegPtr = pinConfigRegValue;
} */
//CORE_PIN55_CONFIG &= ~(1<<24);
CORE_PIN55_CONFIG |= (1<<24);

  sei();
}
