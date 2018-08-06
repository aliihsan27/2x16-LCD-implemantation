#ifndef LCDFunction
#define LCDFunction
#include "stm32f4xx.h"	//change this header file with respect to your device
#include "stdio.h"
/*
	@brief :defines for connection pin and port
*/
#define LCDD0PIN 15
#define LCDD0Port GPIOB
#define LCDD1PIN 8
#define LCDD1Port GPIOD
#define LCDD2PIN 9
#define LCDD2Port GPIOD
#define LCDD3PIN 10
#define LCDD3Port GPIOD
#define LCDD4PIN 	11
#define LCDD4Port GPIOD
#define LCDD5PIN  13
#define LCDD5Port GPIOE
#define LCDD6PIN  15
#define LCDD6Port GPIOE
#define LCDD7PIN  10
#define LCDD7Port GPIOB
#define LCDEnablepin 14
#define LCDEnableport GPIOB
#define LCDReadwritepin 13
#define LCDReadwriteport GPIOB
#define LCDRegisterselectpin 12
#define LCDRegisterselectport GPIOB
#define LCDcontrastpin 11
#define LCDcontrastport GPIOB

/*
	@brief :Time delay for writing or reading to LCD
*/

#define timedelaybeforeenable 4000
#define timedelaybeforedisable 8000
/*
	@brief : Next line instruction
*/
#define LCDnextline(); 	LCDsendacommand(0xc0);

/*
	@brief :Setting port and pin for General Purpose Output
	@param -> port : indicates port of device which we used to set for output
	@param -> pin  : indicates pin number of device which we used to set for output 
	@retval : None
*/

void SetpinAndPortForOutput(GPIO_TypeDef * port,int pin){
	if(port==GPIOA){
		RCC->AHB1ENR  |=RCC_AHB1ENR_GPIOAEN;
	}
	if(port==GPIOB){
		RCC->AHB1ENR  |=RCC_AHB1ENR_GPIOBEN;
	}
	if(port==GPIOC){
		RCC->AHB1ENR  |=RCC_AHB1ENR_GPIOCEN;
	}
	if(port==GPIOD){
		RCC->AHB1ENR  |=RCC_AHB1ENR_GPIODEN;
	}
	if(port==GPIOE){
		RCC->AHB1ENR  |=RCC_AHB1ENR_GPIOEEN;
	}
	// General purpose output mode
	port->MODER &=~(1<<((2*pin)+1));
	port->MODER |=(1<<((2*pin)));
	
	// Output push-pull (reset state)
	port->OTYPER &=~(1<<pin);
	
	// Very high speed
	port->OSPEEDR|=(1<<((2*pin)+1));
	port->OSPEEDR |=(1<<(2*pin));
	// No pull-up, pull-down
	port->PUPDR &=~(1<<pin);
}


/*
	@brief set to port and pin for HIGH and LOW
	@Param->port indicates port of device which we used to set
	@Param->pin indicates pin number of the device which we used to set HIGH or LOW
	@Param->state indicates state of pin(HIGH=1,LOW=0)
	@retval  : None
*/
void sendBitPortandBit(GPIO_TypeDef * port,int pin,int state)
{
	if(state)
	{
		port->BSRR |=(1<<pin);
	}
	else
	{
		port->BSRR |=(1<<(2*pin+(16-pin)));
	}
}
/*
	@brief  :nonexact time delay function
	@param  : a :specified time amount delay
	@retval : None
*/
void delay(uint32_t a){
	volatile int i=0;
	while(i<a){
	i++;
	}
}

/* 
	@brief  :send 8 bit to LCD
	@param  :sentchar is the charcter that we send to LCD
	@retval : none
*/
void LCDsendAbytetoLCD(char sentchar){
	sendBitPortandBit(LCDD0Port,LCDD0PIN,sentchar &0x1);
	sendBitPortandBit(LCDD1Port,LCDD1PIN,sentchar &0x2);
	sendBitPortandBit(LCDD2Port,LCDD2PIN,sentchar &0x4);
	sendBitPortandBit(LCDD3Port,LCDD3PIN,sentchar &0x8);
	sendBitPortandBit(LCDD4Port,LCDD4PIN,sentchar &0x10);
	sendBitPortandBit(LCDD5Port,LCDD5PIN,sentchar &0x20);
	sendBitPortandBit(LCDD6Port,LCDD6PIN,sentchar &0x40);
	sendBitPortandBit(LCDD7Port,LCDD7PIN,sentchar &0x80);
	delay(timedelaybeforedisable);
	sendBitPortandBit(LCDEnableport,LCDEnablepin,0);
}
/*
	@brief  : initialize all pins for LCD
	@param  : none
	@retval : none
*/
void initializePortforLCD(){
	SetpinAndPortForOutput(LCDD0Port,LCDD0PIN);
	SetpinAndPortForOutput(LCDD1Port,LCDD1PIN);
	SetpinAndPortForOutput(LCDD2Port,LCDD2PIN);
	SetpinAndPortForOutput(LCDD3Port,LCDD3PIN);
	SetpinAndPortForOutput(LCDD4Port,LCDD4PIN);
	SetpinAndPortForOutput(LCDD5Port,LCDD5PIN);
	SetpinAndPortForOutput(LCDD6Port,LCDD6PIN);
	SetpinAndPortForOutput(LCDD7Port,LCDD7PIN);
	SetpinAndPortForOutput(LCDEnableport,LCDEnablepin);
	SetpinAndPortForOutput(LCDReadwriteport,LCDReadwritepin);
	SetpinAndPortForOutput(LCDRegisterselectport,LCDRegisterselectpin);
	SetpinAndPortForOutput(LCDcontrastport,LCDcontrastpin);
	sendBitPortandBit(LCDcontrastport,LCDcontrastpin,1);

}
/*
	@brief  : enable the LCD with sending HIGH signal to LCDEnable pin
	@param  : none
	@retval : none
*/
void LCDEnable(){
	delay(timedelaybeforeenable);
	sendBitPortandBit(LCDEnableport,LCDEnablepin,1);
}  

/*
	@brief  : set to LCD to write mode with sending LOW signal to LCDReadwritepin
	@param  : none
	@retval : none
*/

void LCDsettoWrite(){
	sendBitPortandBit(LCDReadwriteport,LCDReadwritepin,0);
}

/*
	@brief  : set to LCD to read mode with sending HIGH signal to LCDreadwritepin
	@param  : none
	@retval : none
*/
void LCDsettoRead(){
	sendBitPortandBit(LCDReadwriteport,LCDReadwritepin,1);
}

/*
	@brief  : set to LCD to command mode with sending LOW signal to LCDregisterselectpin
	@param  : none
	@retval : none
*/
void LCDCommandMode(){
	sendBitPortandBit(LCDRegisterselectport,LCDRegisterselectpin,0);
}
/*
	@brief  : set to LCD to character mode with sending HIGH signal to LCDregisterselectpin
	@param  : none
	@retval : none
*/

void LCDCharacterMode(){
	sendBitPortandBit(LCDRegisterselectport,LCDRegisterselectpin,1);
}



/*
	@brief  : sending a character to LCD
	@param  -> (char)a:character that we want to send to LCD
	@retval : none
*/
void LCDsendacharacter(char a){
	LCDsettoWrite();
	LCDCharacterMode();
	LCDEnable();
	LCDsendAbytetoLCD(a);
}

/*
	@brief  : sending a command to LCD
	@param  -> (char)a : (char)a:character that we want to send to LCD
	@retval : none
*/


void LCDsendacommand(char a){
	LCDsettoWrite();
	LCDCommandMode();
	LCDEnable();
	LCDsendAbytetoLCD(a);
}

/*
	@brief  : sending a command to initialize to LCD
	@param  : none
	@retval : none
*/

void sendcommandforinitLCD(){
	LCDsendacommand(0x1);	//Clear screen
	LCDsendacommand(0xC);	//Entery mode
	LCDsendacommand(0xE);	//Control
  LCDsendacommand(0x38); 	//Function
}

/*
	@brief  : initialize to LCD and set to LCD for writing
	@param  -> (char)a : (char)a:character that we want to send to LCD
	@retval : none
*/


void LCDsetup(){
	initializePortforLCD();
	sendcommandforinitLCD();
	
}

/*
	@brief  : sending a string to LCD
	@param  -> (char*)st :string that we want to send to LCD
	@retval : none
*/

void sendaStringtoLCD(char * st){	
	while(*st){	
		LCDsendacharacter(*st++);
	}
}

/*
	@brief  : sending a integer to LCD
	@param  -> (char) a       : integer that we want to send to LCD
			->(int) maxlength : max length of digits
	@retval : none
*/

void sendaintLCD(int displayednumber,int maxlength){
	char s[maxlength];
	snprintf(s,maxlength+1,"%d",displayednumber);
	sendaStringtoLCD(s);
}

/*
	@brief  : sending a float to LCD
	@param  -> (char) a       : float that we want to send to LCD
			->(int) maxlength : max length of digits
	@retval : none
*/
void sendafloatLCD(float displayednumber,int maxlength){
	char s[maxlength];
	snprintf(s,maxlength+1,"%f",displayednumber);
	sendaStringtoLCD(s);
}
	/*
	@brief  : setting cursor position within 2 row and 16 column 
	@param  -> (int)row    : row number
			->  (int)column : column number
	@retval : none
*/
void setcursorposition(int row,int column){
	if(row==1){
		LCDsendacommand(127+column);
	}
	else if(row==2){
			LCDsendacommand(191+column);
	}
	else{
		sendaStringtoLCD("Invalid position");
	}
}
/*
	@brief  : setting cursor position within 2 row and 16 column and send this position a string
	@param -> (int)r    : row number
		   ->(int)c     : column number
		   ->(char*s)   : string that we want to send to LCD
	@retval : none
*/
void sendastringtopositionLCD(unsigned int r,unsigned int c,char * s){
	setcursorposition(r,c);
	sendaStringtoLCD(s);
}


#endif
