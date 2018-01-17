#include<avr/io.h>
#include<avr/interrupt.h>
#include "display.h"



uint8_t time[4];
uint8_t curr_digit=0;

//the seven segments display table
uint8_t numbers[]={
//0b0gfedcba
  0b01111111, //zero
  0b00000110, //one
  0b01011011, //two
  0b01001111, //three
  0b01100110, //four
  0b01101101, //five
  0b01111101, //six
  0b00000111, //seven
  0b01111111, //eight
  0b01101111  //nine
};

void writeByte(uint8_t data){
  for(uint8_t i=0;i<8;i++){
    displayPort&=~(1<<CLK);
    if(data&(1<<(7-i))){
      displayPort|=(1<<DIN);
    }else{
      displayPort&=~(1<<DIN);
    }
      displayPort|=(1<<CLK);
  }
}
void maxWrite(uint8_t reg, uint8_t data){
  displayPort&=~(1<<LOAD);
  writeByte(reg);
  writeByte(data);
  displayPort|=(1<<LOAD);
}

//interrupt code, sketch
ISR(TIMER0_COMP_vect ){
  // curr_digit++;
  // if (curr_digit==4) curr_digit=0;
  // //put number onto digit
  // segmentPort=0x00;
  // segmentPort=numbers[time[curr_digit]];
  //
  // //enable digit
  // digitPort&=~(0xFF);
  // digitPort=(1<<(curr_digit+4)); //each digit is connected to pin 4 onwards
}

void displayinit(void){
  displayDDR=(1<<LOAD)|(1<<CLK)|(1<<DIN);
  displayPort=(1<<LOAD)|(1<<CLK)|(1<<DIN);

  maxWrite(max7219_reg_scanLimit,0x03);
  maxWrite(max7219_reg_decodeMode,0x0F);
  maxWrite(max7219_reg_shutdown,0x01);
  maxWrite(max7219_reg_displayTest,0x00);
  maxWrite(max7219_reg_intensity,0x0f);

  //displayWrite(0,0);
  maxWrite(max7219_reg_digit0,0);
  maxWrite(max7219_reg_digit1,1);
  maxWrite(max7219_reg_digit2,2);
  maxWrite(max7219_reg_digit3,3);
  TCCR0=(1<<WGM01); //CTC mode, pin disconnected
  OCR0=125;
  TCNT0=0;
  TIMSK=OCIE0; //Enable interrup on compare match
  //sei();
}
void displayWrite(uint8_t hours, uint8_t minutes){
  time[1]=minutes/10;      //digit for sets of ten
  maxWrite(max7219_reg_digit1,time[1]);
  time[0]=minutes-time[1]*10; //digit for set of ones
  maxWrite(max7219_reg_digit0,time[0]|0x80);
  time[3]=hours/10;      //digit for sets of ten
  maxWrite(max7219_reg_digit3,time[3]);
  time[2]=hours-time[3]*10; //digit for set of ones
  maxWrite(max7219_reg_digit2,time[2]);
}
