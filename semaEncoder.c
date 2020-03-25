/*Mr.Bit Quadrature Encoder Teset with lcd interface
** LCD used:SainSmart I2C LCD2004 adapter for HD44780 LCD screens
** The LCD2004 module appears to be identical to one marketed by YwRobot
**Author: Mr. Bit
** LICENSE: GNU General Public License, version 3 (GPL-3.0)
** LCD interface quadrature incremental encoder tester.ino
** NOTE: I2C pins are A4 SDA, A5 SCL
** Don't forget to use the new LiquidCrystal_I2C Library.
* Encoder: A,B channel connected on pins 2 and 4, on the same pins A/ and B/ channels ,or index channel N and N/ can be connected
* index channel not yet added
* 
*/
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display


const int encoder1PinA = 2;  // encoder channel A and B on pins 2 and 4
const int encoder1PinB = 4;
volatile int encoder1Pos = 0; //ticks of encoder will change, initial number is zero

const int transmitTimer = 100; // milliseconds
const int keepAliveTransmitTimer = 5000; //milliseconds
float prevTransmission;
float prevTransmitVal;


void setup()
{

  Serial.begin(38400);
  prevTransmission = 0;
  prevTransmitVal = 0;

  
    attachInterrupt(0, runEncoder1, RISING );  // encoder pin on interrupt 0 >>(pin 2 on arduino)
    
    
  lcd.init();                      // initialize the lcd 
  lcd.init();
  lcd.backlight();
  lcd.setCursor(3,0);


  lcd.setBacklight(HIGH); //turn on lcd backlight
  lcd.home ();
  lcd.print("Encoder Tester.");  // Print a message to the LCD.
  lcd.setCursor( 0, 3);//Set the cursor to line 3 (first line is line 0)
  lcd.print("Please plug encoder.");
  delay(1500);

}

void loop()
{
  double Pos1;
 uint8_t oldSREG = SREG; 
 
 cli();
 Pos1 = encoder1Pos;  
 SREG = oldSREG;
 static double oldPos1;
 if(Pos1 != oldPos1){
  lcd.setCursor ( 0, 2 );
    lcd.print("A,B CH="); 
    oldPos1 = Pos1;


 }

      // Position cursor and write some text
  lcd.home();// go to the first line, first character
  lcd.print("Detecting...        ");  
  
  lcd.setCursor ( 8, 2 );        // go to the 2nd line
  lcd.print(Pos1/10, DEC);  
   if (Pos1 < 100000) lcd.print(" ");
  if (Pos1 < 10000) lcd.print(" ");
  if (Pos1 < 1000) lcd.print(" ");
if (Pos1 < 100) lcd.print(" ");
if (Pos1 < 10) lcd.print(" "); 
                 

  lcd.setCursor ( 0, 3 );        // go to the fourth line
  lcd.print("                    ");

  if (((prevTransmission + transmitTimer) < millis()) && prevTransmitVal != Pos1) {
      //Serial.print("X-pos: ");
      Serial.println(Pos1/10);
      prevTransmitVal = Pos1;
      prevTransmission = millis();
  } else if ((prevTransmission + keepAliveTransmitTimer) < millis()) {
      Serial.println(Pos1/10);
      prevTransmitVal = Pos1;
      prevTransmission = millis();
  }
}
void runEncoder1()
{
 if (digitalRead(encoder1PinA) == digitalRead(encoder1PinB))  { 
   encoder1Pos++;    // count up if both encoder pins are HIGH on pin change interrupt
 } else {                                       
   encoder1Pos--;    //count down if pins are HIGH and LOW
 }
}
