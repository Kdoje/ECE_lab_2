#include <msp430.h>
#include <stdlib.h>
#include "peripherals.h"

/*
unsigned long gallons=0;

void swDelay(char numLoops)
{
  volatile unsigned int i,j;

  for (j=0; j<numLoops; j++)
  {
    i = 50000 ;
    while (i > 0)
    i--;
  }
}

//configure the msp buttons
void config_msp_buttons(void)
{
  P2SEL &= ~(BIT1);
  P1SEL &= ~(BIT1);

  P2DIR &= ~(BIT1);
  P1DIR &= ~(BIT1);

  P2REN |= (BIT1);
  P1REN |= (BIT1);

  P2OUT |= (BIT1);
  P1OUT |= (BIT1);
}

//configure the board buttons
void config_board_buttons(void)
{
  P7SEL &= ~(BIT0|BIT4);
  P3SEL &= ~(BIT6);
  P2SEL &= ~(BIT2);

  P7DIR &= ~(BIT0|BIT4);
  P3DIR &= ~(BIT6);
  P2DIR &= ~(BIT2);

  P7REN |= (BIT0|BIT4);
  P3REN |= (BIT6);
  P2REN |= (BIT2);

  P7OUT |= (BIT0|BIT4);
  P3OUT |= (BIT6);
  P2OUT |= (BIT2);
}

//return the state of the buttons
unsigned char buttons_state(void)
{
  unsigned char val=0;
  if(~P7IN&BIT0){
    val|=BIT0; //S1
  }
  if(~P3IN&BIT6){
    val|=BIT1; //S2
  }
  if(~P2IN&BIT2){
    val|=BIT2; //S3
  }
  if(~P7IN&BIT4){
    val|=BIT3; //S4
  }
  return val;
}

//determine and display the cost
void display_cost(unsigned char priceString[], unsigned char grade)
{
  unsigned long price;

  if(grade == 'D')
  {
    price = (gallons * 299)/100;
  }
  else if(grade == 'S')
  {
    price = (gallons * 289)/100;
  }
  else if(grade == 'P')
  {
    price = (gallons * 269)/100;
  }
  else
  {
    price = (gallons * 249)/100;
  }

  convert_price(priceString, price);

  Graphics_clearDisplay(&g_sContext);

  Graphics_drawStringCentered(&g_sContext, "TOTAL:", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, priceString, AUTO_STRING_LENGTH, 48, 55, TRANSPARENT_TEXT);

  Graphics_flushBuffer(&g_sContext);
}


//convert the price
void convert_price(unsigned char priceString[], unsigned long price)
{
  int digits = 0;
  int num;
  int dot = 1;
  int one = 1;
  int i = 1;
  int j;

  priceString[0] = '$';

  num = price;

  //determine the number of digits in the input number
  while(num >= 10)
  {
    num = num/10;
    digits++;
  }


  for(j=0;j<digits;j++)
  {
    one = one*10;
  }

  while(digits >= 0)
  {
    if((digits == 1)&&(dot == 1))
    {
      priceString[i] = '.';
      dot = 0;      //make sure decimal cannot be added again
      i++;
    }
    else
    {
      num = price/one;
      priceString[i] = '0' + num; //starts at 0x30
      price -= (one*num);
      one /= 10;
      digits--;
      i++;
    }
  }
}


void welcome_screen(void)
{
  Graphics_clearDisplay(&g_sContext);
  Graphics_drawStringCentered(&g_sContext, "Welcome", AUTO_STRING_LENGTH, 48, 35, TRANSPARENT_TEXT);
  Graphics_drawStringCentered(&g_sContext, "Press * to begin", AUTO_STRING_LENGTH, 48, 45, TRANSPARENT_TEXT);
  Graphics_flushBuffer(&g_sContext);
}


//return grade as D, S, P, or R
unsigned char select_grade(unsigned char grade)
{
  //set grade = 'D'
}

//start timer
void runtimerA2(void)
{
  // This function configures and starts Timer A2
  // Timer is counting ~0.01 seconds
  //
  // Input: none, Output: none
  //
  // smj, ECE2049, 17 Sep 2013
  //
  // Use ACLK, 16 Bit, up mode, 1 divider
  TA2CTL = TASSEL_1 + MC_1 + ID_0;
  TA2CCR0 = 327; // 327+1 = 328 ACLK tics = ~1/100 seconds
  TA2CCTL0 = CCIE; // TA2CCR0 interrupt enabled
}

//Timer Interrupt
#pragma vector=TIMER2_A0_VECTOR
__interrupt void TimerA2_ISR(void)
{
  //increment gallons, probably want leap counter
}

// This function stops Timer A2 andresets the global gallons variable
void stoptimerA2(int reset)
{

  TA2CTL = MC_0; // stop timer
  TA2CCTL0 &= ~CCIE; // TA2CCR0 interrupt disabled
  if(reset){
  gallons=0;
}
}

//display gallons
void display_gallons(unsigned int gallons)
{
  unsigned char display[6];

  display[5] = (gallons % 10) + 48;
  display[4] = ((gallons/10) % 10) + 48;
  display[3] = '.';
  display[2] = ((gallons/100) % 10) + 48;
  display[1] = ((gallons/1000) % 10) + 48;
  display[0] = ((gallons/10000) % 10) + 48;

  Graphics_drawStringCentered(&g_sContext, display,6,45,50,OPAQUE_TEXT);
  Graphics_flushBuffer(&g_sContext);
}

//gets values from pump to be ready
void pump_ready(unsigned char grade)
{
  //can call display gallons function
  //probably want to use interrupt with counter to call display gallons
  //need to get time button has been pressed
  //allow for topping off
}

void pay(unsigned char pin[], unsigned char currKey)
{
  unsigned char pin1[8] = {'1', '2', '3', '4', '5', '6', '7', '8'};
  unsigned char pin2[8] = {'8', '7', '6', '5', '4', '3', '2', '1'};
  int i;
  int count = 0;
  int x = 15;
  int correctPin1;
  int correctPin2;

  Graphics_drawStringCentered(&g_sContext, "ENTER PIN", AUTO_STRING_LENGTH, 48, 65, TRANSPARENT_TEXT);
  Graphics_flushBuffer(&g_sContext);

  while(1)
  {
    gallons = 0;
    runtimerA2();
    correctPin1 = 1;
    correctPin2 = 1;
    i = 0;

    while(i<8)
    {
      if((gallons>100)|(count>2))
      {
        if((gallons%4)==0)
        {
          x = x^15;
          BuzzerOn();
          //setLeds(x);
        }
      }
      currKey = getKey();
      if((currKey >= '0') && (currKey <= '9'))
      {
        pin[i] = currKey;
        i++;
        while(getKey() != 0){}
        currKey = 0;
        gallons = 0;
      }
    }

    for(i=0;i<8;i++)
    {
      if(pin[i] != pin1[i])
      {
        correctPin1 = 0;
      }

      if(pin[i] != pin2[i])
      {
        correctPin2 = 0;
      }
    }

    if(correctPin1|correctPin2)
    {
      BuzzerOff();
      setLeds(0);
      break;
    }
    else
    {
      Graphics_drawStringCentered(&g_sContext, "INCORRECT", AUTO_STRING_LENGTH, 48, 65, OPAQUE_TEXT);
      Graphics_drawStringCentered(&g_sContext, "ENTER AGAIN", AUTO_STRING_LENGTH, 48, 75, OPAQUE_TEXT);
      Graphics_flushBuffer(&g_sContext);
      count++;
    }
  }
}



void main(void)
{
  unsigned char pin[8];
  unsigned char priceString[10];
  unsigned char currKey=0;
  unsigned char grade=0;
  int i;
  WDTCTL = WDTPW | WDTHOLD;

  configDisplay();
  //initLeds();
  configKeypad();
  config_board_buttons();
  config_msp_buttons();

  _BIS_SR(GIE);


  while(1)
  {
    welcome_screen();

    //wait_for_response(currKey);

    grade = select_grade(grade);
    pump_ready(grade);

    for(i=0;i<10;i++)
    {
      priceString[i] = NULL;  //initialize to be null
    }

    display_cost(priceString, grade);
    pay(pin, currKey);
    Graphics_clearDisplay(&g_sContext);
  }
}
*/
int main(){
    WDTCTL = WDTPW | WDTHOLD;
    initLeds();
    setLeds(0x01);
    configDisplay();
    while (1)
    {
        Graphics_drawStringCentered(&g_sContext, "INCORRECT",
                                    AUTO_STRING_LENGTH, 48, 65, OPAQUE_TEXT);
        Graphics_drawStringCentered(&g_sContext, "ENTER AGAIN",
                                    AUTO_STRING_LENGTH, 48, 75, OPAQUE_TEXT);
        Graphics_flushBuffer(&g_sContext);
    }

    return 0;
}
