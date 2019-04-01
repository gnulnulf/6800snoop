

// This Teensy3 native optimized version requires specific pins
//
#define sclk 13  // SCLK can also use pin 14
#define mosi 11  // MOSI can also use pin 7
#define cs   10  // CS & DC can use pins 2, 6, 9, 10, 15, 20, 21, 22, 23
#define dc   9   //  but certain pairs must NOT be used: 2+10, 6+9, 20+23, 21+22
#define rst  25   // RST can use any pin
#define sdcs 4   // CS for SD card, can use any pin

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>
//#include <String.h>
#include "snoop.h"

#if defined(__SAM3X8E__)
    #undef __FlashStringHelper::F(string_literal)
    #define F(string_literal) string_literal
#endif

// Option 1: use any pins but a little slower
//Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, mosi, sclk, rst);
Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, rst);

#define VERSION "1.1"
#define RAMSIZE 0x800
volatile uint8_t ram[RAMSIZE];
volatile uint8_t testa[256];
volatile uint8_t testb[256];

//volatile uint8_t [RAMSIZE];

volatile uint8_t lamps[64];
volatile uint8_t switches[64];

uint32_t looptime=0;
uint32_t looptimescore=0;
void setup(void) {
  pinMode(sdcs, INPUT_PULLUP);  // don't touch the SD card
  Serial.begin(9600);
  Serial.print("Snoop 6800");

  // Our supplier changed the 1.8" display slightly after Jan 10, 2012
  // so that the alignment of the TFT had to be shifted by a few pixels
  // this just means the init code is slightly different. Check the
  // color of the tab to see which init code to try. If the display is
  // cut off or has extra 'random' pixels on the top & left, try the
  // other option!
  // If you are seeing red and green color inversion, use Black Tab

  // If your TFT's plastic wrap has a Black Tab, use the following:
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab
  // If your TFT's plastic wrap has a Red Tab, use the following:
  //tft.initR(INITR_REDTAB);   // initialize a ST7735R chip, red tab
  // If your TFT's plastic wrap has a Green Tab, use the following:
  //tft.initR(INITR_GREENTAB); // initialize a ST7735R chip, green tab

  Serial.println("init");

  uint16_t time = millis();
 // tft.fillScreen(ST7735_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
//  tft.fillScreen(ST7735_BLACK);
 // testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST7735_WHITE);
 // delay(1000);

  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE );
  tft.setTextWrap(true);
  tft.println("Snoop 6800");
  tft.print("Version:");
  tft.println(VERSION);


  snoopInit();
  looptime=millis();
  looptimescore=millis();
  tft.fillScreen(ST7735_BLACK);
}


void loop() {

  // #############################################################################
  // Dump snoop data to Serial
  // #############################################################################
  // this generates so much traffic the Teensy will crash
  // You will get nice results
  // AAAADDCC address, data, control
  // 2400FF00
  // 24020200 write row
  // 24008100 write data
  // 2400FF00 clear lamps

  if ( snooptail<snoophead) {
    uint16_t address = snoopbuffer[snooptail]<<8 |snoopbuffer[snooptail+1];
    uint16_t data = snoopbuffer[snooptail+2];
    uint16_t ctrl = snoopbuffer[snooptail+3];

    Serial.print(( address & 0xf000 )>>12 ,HEX);
    Serial.print(( address & 0xf00 )>>8 ,HEX);
    Serial.print(( address & 0xf0 )>>4 ,HEX);
    Serial.print(( address & 0xf ) ,HEX);

    Serial.print(( data & 0xf0 )>>4 ,HEX);
    Serial.print(( data & 0xf ) ,HEX);

    Serial.print(( ctrl & 0xf0 )>>4 ,HEX);
    Serial.print(( ctrl & 0xf ) ,HEX);
    Serial.println("");
    snooptail += 4;
    if (snooptail > SNOOP_BUFFER_SIZE ) snooptail=0;
  }


/*
      
// ram
ROM ram_u25(&Cpubus, 0x00,0x800 , "U25 RAM");

// roms
ROM rom_u26(&Cpubus, 0x4000,0x4000, "U26 ROM");
ROM rom_u27(&Cpubus, 0x8000,0x8000, "U27 ROM");

//PIA initialisation
MC6821 PIAU10( &Cpubus , 0x2100 , "U10 SOLENOID/SOUND"); 
MC6821 PIAU38( &Cpubus , 0x3000 , "U38 SWITCH"); 
MC6821 PIAU41( &Cpubus , 0x2c00 , "U41 ALFADISPLAY" ); 
MC6821 PIAU42( &Cpubus , 0x3400 , "U42 DISPLAY/WIDGET" ); 
MC6821 PIAU51( &Cpubus , 0x2800 , "U51 DISPLAY" ); 
MC6821 PIAU54( &Cpubus , 0x2400 , "U54 LAMPS" ); 











*/

// #############################################################################
// Dump lamp data to Serial
// #############################################################################
if ( 0 ) {
  Serial.print("LAMPS:");
  for  (int j=0;j<16;j++){
    uint8_t value=lamps[j];
    //byte=ram[i*8+j];
    Serial.print(( value & 0x80 )>>7 ,BIN);
    Serial.print(( value & 0x40 )>>6 ,BIN);
    Serial.print(( value & 0x20 )>>5 ,BIN);
    Serial.print(( value & 0x10 )>>4 ,BIN);
    Serial.print(( value & 0x08 )>>3 ,BIN);
    Serial.print(( value & 0x04 )>>2 ,BIN);
    Serial.print(( value & 0x02 )>>1 ,BIN);
    Serial.print(( value & 0x01 ) ,BIN);
  }
  Serial.println("");
}


//delay(50);

//  uint16_t base=0x1F00;
  uint16_t base=0x00;
/*

uint16_t base=0;
//for ( base=0;base<2048;base+=0x100) {
for ( base=0;base<0x100;base+=0x100) {

  tft.fillScreen(ST7735_BLACK);
  tft.setCursor(0, 0);
  tft.setTextColor(ST7735_WHITE );
  tft.setTextWrap(false);
  for (int i=0;i<20;i++) {
//    tft.print(i);
    uint16_t addr=base+i*8;
    tft.print(( addr & 0xf000 )>>12 ,HEX);
    tft.print(( addr & 0xf00 )>>8 ,HEX);
    tft.print(( addr & 0xf0 )>>4 ,HEX);
    tft.print(( addr & 0xf ) ,HEX);
 
    
    tft.print(":");
      for  (int j=0;j<8;j++){
//    tft.print(String::format("0x%02X", ram[i*8+j]));
    uint8_t value=ram[i*8+j];
    //byte=ram[i*8+j];
    tft.print(( value & 0xf0 )>>4 ,HEX);
    tft.print(( value & 0xf ) ,HEX);
    
  }
    tft.println("");

  }
  //ram[1]++;
  delay(1000);
}
*/
if (false  ) {
    Serial.println("------------");
    for (int i=0;i<(RAMSIZE/64);i++) {
    uint16_t addr=base+i*64;
    Serial.print(( addr & 0xf000 )>>12 ,HEX);
    Serial.print(( addr & 0xf00 )>>8 ,HEX);
    Serial.print(( addr & 0xf0 )>>4 ,HEX);
    Serial.print(( addr & 0xf ) ,HEX);
 
    
    Serial.print(":");
      for  (int j=0;j<64;j++){
//    tft.print(String::format("0x%02X", ram[i*8+j]));
    uint8_t value=ram[base+i*64+j];
    //byte=ram[i*8+j];
    Serial.print(( value & 0xf0 )>>4 ,HEX);
    Serial.print(( value & 0xf ) ,HEX);
    
  }
    Serial.println("");
  }
} //if

if (0) {
delay(1000);
    Serial.println("-testa-----------");
    for (int i=0;i<(256/16);i++) {
    uint16_t addr=i*16;
    Serial.print(( addr & 0xf000 )>>12 ,HEX);
    Serial.print(( addr & 0xf00 )>>8 ,HEX);
    Serial.print(( addr & 0xf0 )>>4 ,HEX);
    Serial.print(( addr & 0xf ) ,HEX);
 
    
    Serial.print(":");
      for  (int j=0;j<16;j++){
//    tft.print(String::format("0x%02X", ram[i*8+j]));
    uint8_t value=testa[i*16+j];
    //byte=ram[i*8+j];
    Serial.print(( value & 0xf0 )>>4 ,HEX);
    Serial.print(( value & 0xf ) ,HEX);
    
  }
    Serial.println("");
  }
} //if

if (0  ) {
delay(1000);
    Serial.println("-testb-----------");
    for (int i=0;i<(256/16);i++) {
    uint16_t addr=i*16;
    Serial.print(( addr & 0xf000 )>>12 ,HEX);
    Serial.print(( addr & 0xf00 )>>8 ,HEX);
    Serial.print(( addr & 0xf0 )>>4 ,HEX);
    Serial.print(( addr & 0xf ) ,HEX);
 
    
    Serial.print(":");
      for  (int j=0;j<16;j++){
//    tft.print(String::format("0x%02X", ram[i*8+j]));
    uint8_t value=testb[i*16+j];
    //byte=ram[i*8+j];
    Serial.print(( value & 0xf0 )>>4 ,HEX);
    Serial.print(( value & 0xf ) ,HEX);
    
  }
    Serial.println("");
  }
} //if


    if ( millis() > looptime ) {
      looptime+=100;

  // Serial.println("1 sec timer");
  //tft.fillScreen(ST7735_BLACK);
 // tft.setCursor(0, 0);
//  tft.setTextColor(ST7735_WHITE );
//  tft.setTextWrap(false);
  
   // tft.println("LAMPS:");
   //LAMPS from PIA 
      for  (int j=0;j<8;j++){
    uint8_t value=lamps[j];
    //byte=ram[i*8+j];
 //   tft.print(( value & 0xF0 )>>4 ,HEX);
  //  tft.print(( value & 0x0f ) ,HEX);
    tft.drawPixel( j*2+10,  0*2+10, (( value & 0x80 )>>7)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+10,  1*2+10, (( value & 0x40 )>>6)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+10,  2*2+10, (( value & 0x20 )>>5)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+10,  3*2+10, (( value & 0x10 )>>4)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+10,  4*2+10, (( value & 0x08 )>>3)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+10,  5*2+10, (( value & 0x04 )>>2)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+10,  6*2+10, (( value & 0x02 )>>1)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+10,  7*2+10, (( value & 0x01 ))?ST7735_BLACK:ST7735_WHITE);
    /*
    tft.print(( value & 0x80 )>>7 ,BIN);
    tft.print(( value & 0x40 )>>6 ,BIN);
    tft.print(( value & 0x20 )>>5 ,BIN);
    tft.print(( value & 0x10 )>>4 ,BIN);
    tft.print(( value & 0x08 )>>3 ,BIN);
    tft.print(( value & 0x04 )>>2 ,BIN);
    tft.print(( value & 0x02 )>>1 ,BIN);
    tft.println(( value & 0x01 ) ,BIN);
    */
  }
  // LAMPS from ram
      for  (int j=0;j<8;j++){
     uint8_t value=~ram[j+0x10];
    //byte=ram[i*8+j];
 //   tft.print(( value & 0xF0 )>>4 ,HEX);
  //  tft.print(( value & 0x0f ) ,HEX);
    tft.drawPixel( j*2+40,  0*2+10, (( value & 0x80 )>>7)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+40,  1*2+10, (( value & 0x40 )>>6)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+40,  2*2+10, (( value & 0x20 )>>5)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+40,  3*2+10, (( value & 0x10 )>>4)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+40,  4*2+10, (( value & 0x08 )>>3)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+40,  5*2+10, (( value & 0x04 )>>2)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+40,  6*2+10, (( value & 0x02 )>>1)?ST7735_BLACK:ST7735_WHITE);
    tft.drawPixel( j*2+40,  7*2+10, (( value & 0x01 ))?ST7735_BLACK:ST7735_WHITE);
    /*
    tft.print(( value & 0x80 )>>7 ,BIN);
    tft.print(( value & 0x40 )>>6 ,BIN);
    tft.print(( value & 0x20 )>>5 ,BIN);
    tft.print(( value & 0x10 )>>4 ,BIN);
    tft.print(( value & 0x08 )>>3 ,BIN);
    tft.print(( value & 0x04 )>>2 ,BIN);
    tft.print(( value & 0x02 )>>1 ,BIN);
    tft.println(( value & 0x01 ) ,BIN);
    */
  }

//switches
      for  (int j=0;j<8;j++){
     uint8_t value=switches[j];
    //byte=ram[i*8+j];
 //   tft.print(( value & 0xF0 )>>4 ,HEX);
  //  tft.print(( value & 0x0f ) ,HEX);
    tft.drawPixel( j*2+60,  0*2+10, (( value & 0x80 )>>7)?ST7735_GREEN:ST7735_RED);
    tft.drawPixel( j*2+60,  1*2+10, (( value & 0x40 )>>6)?ST7735_GREEN:ST7735_RED);
    tft.drawPixel( j*2+60,  2*2+10, (( value & 0x20 )>>5)?ST7735_GREEN:ST7735_RED);
    tft.drawPixel( j*2+60,  3*2+10, (( value & 0x10 )>>4)?ST7735_GREEN:ST7735_RED);
    tft.drawPixel( j*2+60,  4*2+10, (( value & 0x08 )>>3)?ST7735_GREEN:ST7735_RED);
    tft.drawPixel( j*2+60,  5*2+10, (( value & 0x04 )>>2)?ST7735_GREEN:ST7735_RED);
    tft.drawPixel( j*2+60,  6*2+10, (( value & 0x02 )>>1)?ST7735_GREEN:ST7735_RED);
    tft.drawPixel( j*2+60,  7*2+10, (( value & 0x01 ))?ST7735_GREEN:ST7735_RED);
    /*
    tft.print(( value & 0x80 )>>7 ,BIN);
    tft.print(( value & 0x40 )>>6 ,BIN);
    tft.print(( value & 0x20 )>>5 ,BIN);
    tft.print(( value & 0x10 )>>4 ,BIN);
    tft.print(( value & 0x08 )>>3 ,BIN);
    tft.print(( value & 0x04 )>>2 ,BIN);
    tft.print(( value & 0x02 )>>1 ,BIN);
    tft.println(( value & 0x01 ) ,BIN);
    */
  }

    
    
    tft.println("");
      
    }
//  tft.setCursor(4, 0);
 // Serial.println(snoophead-snooptail);
  //tft.println(" ");

   if ( millis() > looptimescore ) {
      looptimescore+=1000;
      tft.setCursor(0, 32);
      //x,y,w,h,col
      tft.fillRect(0, 32, 128, 64, ST7735_BLACK);

      
      for  (int player=0;player<4;player++){
      tft.print(player+1);
      tft.print(": ");

      for  (int j=0;j<4;j++){
//    tft.print(String::format("0x%02X", ram[i*8+j]));
    uint8_t value=ram[0x200+j+player*4];
    tft.print(( value & 0xf0 )>>4 ,HEX);
    tft.print(( value & 0xf ) ,HEX);
      }
    tft.println("  ");
      }
//Ball
    tft.print("Ball:");
//      for  (int j=0;j<4;j++){
    uint8_t value=ram[0x38];
    tft.print(( value & 0xf0 )>>4 ,HEX);
    tft.print(( value & 0xf ) ,HEX);
  //    }
    tft.println(" ");

//playercount
    tft.print("Player:");
//      for  (int j=0;j<4;j++){
     value=ram[0xa9]+1;
    tft.print(( value & 0xf0 )>>4 ,HEX);
    tft.print(( value & 0xf ) ,HEX);
  //    }
    tft.println(" ");


//playercount
    tft.print("Players:");
//      for  (int j=0;j<4;j++){
   value=ram[0xa8]+1;
    tft.print(( value & 0xf0 )>>4 ,HEX);
    tft.print(( value & 0xf ) ,HEX);
  //    }
    tft.println(" ");



//credits
    tft.print("Credits:");
//      for  (int j=0;j<4;j++){
     value=ram[0x39];
    tft.print(( value & 0xf0 )>>4 ,HEX);
    tft.print(( value & 0xf ) ,HEX);
  //    }
    tft.println(" ");



//RAMDUMP
if ( 0  ) {
    Serial.println("------------");
    for (int i=0;i<(RAMSIZE/64);i++) {
    uint16_t addr=base+i*64;
    Serial.print(( addr & 0xf000 )>>12 ,HEX);
    Serial.print(( addr & 0xf00 )>>8 ,HEX);
    Serial.print(( addr & 0xf0 )>>4 ,HEX);
    Serial.print(( addr & 0xf ) ,HEX);
 
    
    Serial.print(":");
      for  (int j=0;j<64;j++){
//    tft.print(String::format("0x%02X", ram[i*8+j]));
    uint8_t value=ram[base+i*64+j];
    //byte=ram[i*8+j];
    Serial.print(( value & 0xf0 )>>4 ,HEX);
    Serial.print(( value & 0xf ) ,HEX);
    
  }
    Serial.println("");
  }
} //if


      
   }
  
//delay(1000);
}
