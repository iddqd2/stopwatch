/*
  Stopwatch
  Run stopwatch with arduino.

  Code based on: http://danthompsonsblog.blogspot.com/2008/11/timecode-based-stopwatch.html
  Coded by: arduinoprojects101.com
*/

// include the library code:
#include "Arduino.h"
#include "LiquidCrystal\LiquidCrystal.h"


// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 10, 9, 8, 7);

int ledPin = 13	;                    // LED connected to digital pin 13
int buttonPin = 5;                  // button on pin 2
int stopButtonPin = 3;	// Reset button

int value = LOW;                    // previous value of the LED
int buttonState;                    // variable to store button state
int stopButtonState;
int lastStopButtonState=HIGH;
int lastButtonState=HIGH;                // variable to store last button state
int buttonHold;					// button hold
int blinking;                       // condition for blinking - timer is timing
int frameRate = 100;                // the frame rate (frames per second) at which the stopwatch runs - Change to suit
long interval = (1000/frameRate);   // blink interval
long previousMillis = 0;            // variable to store last time LED was updated
long startTime ;                    // start time for stop watch
long elapsedTime ;                  // elapsed time for stop watch
long fractional;                     // variable used to store fractional part of Frames
int fractionalSecs;                 // variable used to store fractional part of Seconds
int fractionalMins;                 // variable used to store fractional part of Minutes
long  elapsedFrames;                  // elapsed frames for stop watch
long elapsedSeconds;                 // elapsed seconds for stop watch
long elapsedMinutes;                 // elapsed Minutes for stop watch
char buf[10];                       // string buffer for itoa function
int blinkInterval1=100, blinkInterval2=300;
unsigned long currentTimer=0;
unsigned long TotalTime, LapTime, PreviousLapTime;
int LapNumber=0;
long buttonTime, buttonPressTime=0;

void printTimerRunning ()
{
	if (millis() - currentTimer < blinkInterval1 && blinking == true)
	{
	lcd.setCursor (15,0);
	lcd.print("x");

	}
	else if (millis() - currentTimer < blinkInterval2 && blinking == true)
	{
	lcd.setCursor (15,0);
	lcd.print(" ");
	}
	else
	{
		currentTimer = millis();
	}

}


/*  Prints elapsed time
 * Outputs given countTime in stopwatch format and given Title in "Title:mm:ss:ff" format
 */
void printTime (long countTime, int cursorRow, int cursorLine, char *title)
{
	// Routine to report elapsed time
//	   countTime =   millis() - startTime;                // store elapsed time
	   elapsedMinutes = (countTime / 60000L);
	   elapsedSeconds = (countTime / 1000L);              // divide by 1000 to convert to seconds - then cast to an int to print
	   elapsedFrames = (countTime / interval);            // divide by 100 to convert to 1/100 of a second - then cast to an int to print
	   fractional = (int)(elapsedFrames % frameRate);       // use modulo operator to get fractional part of 100 Seconds
	   fractionalSecs = (int)(elapsedSeconds % 60L);        // use modulo operator to get fractional part of 60 Seconds
	   fractionalMins = (int)(elapsedMinutes % 60L);        // use modulo operator to get fractional part of 60 Minutes
	   //lcd.clear();                                         // clear the LDC
	   lcd.setCursor(cursorRow, cursorLine);
	   lcd.print(title);
	   if (fractionalMins < 10){                            // pad in leading zeros
	      lcd.print("0");                                 // add a zero
	      }

	    lcd.print(itoa(fractionalMins, buf, 10));       // convert the int to a string and print a fractional part of 60 Minutes to the LCD
	      lcd.print(":");                                 //print a colan.

	 if (fractionalSecs < 10){                            // pad in leading zeros
	      lcd.print("0");                                 // add a zero
	      }

	 lcd.print(itoa(fractionalSecs, buf, 10));          // convert the int to a string and print a fractional part of 60 Seconds to the LCD
	   lcd.print(":");                                    //print a colan.

	 if (fractional < 10){                                // pad in leading zeros
	      lcd.print("0");                                 // add a zero
	      }

	 lcd.print(itoa(fractional, buf, 10));              // convert the int to a string and print a fractional part of 25 Frames to the LCD

}

/*	Stop timer function.
 *  Stops timer with the last lap time and display total time for the last lap.
 */
void StopTimer()
{
		elapsedTime = millis() - startTime;
		blinking = false;
		lcd.setCursor (15,0);
		lcd.print(" ");
		lcd.setCursor(0,1);
		lcd.print("                ");
		if ( TotalTime == 0) {
			printTime (elapsedTime, 0, 1, "Total:");
		}
		else printTime (TotalTime, 0, 1, "Total:");
		LapNumber=0;
		TotalTime = 0;
}


void StartTimer()
{
	  startTime = millis();                             // store the start time
	  elapsedTime = millis() - startTime;
	  PreviousLapTime = elapsedTime;
	  blinking = true;                                  // turn on blinking while timing
	  delay(500);                                         // short delay to debounce switch
	  lcd.setCursor(0,0);
	  lcd.print("                ");
}

void StartSingle() {
	lcd.clear();
	lcd.setCursor(1,0);
	lcd.print("Get Ready in...");
	delay(300);
	for (int i=0; i<5; i++) {
		lcd.setCursor(8,1);
		lcd.print(5-i);
		delay(1000);
	}
	lcd.clear();
	lcd.setCursor(6,0);
	lcd.print("GO!");
	StartTimer();

}

void DisplayLapTime()
{

	   //blinking = false;                                    // turn off blinking, all done timing
	   elapsedTime = millis() - startTime;
	   LapTime = elapsedTime - PreviousLapTime;
	   PreviousLapTime = elapsedTime;
	   TotalTime = elapsedTime;
	   LapNumber++;
	   printTime (LapTime, 0, 0, "Lap# :");
	   lcd.setCursor (4,0);
	   lcd.print(LapNumber);

}

void setup()
{
  lcd.begin(16, 2);                // initialise the LCD.
  pinMode(ledPin, OUTPUT);         // sets the digital pin as output
  pinMode(buttonPin, INPUT);       // not really necessary, pins default to INPUT anyway
  digitalWrite(buttonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.
  digitalWrite(stopButtonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.
  lcd.setCursor(0,0);
  lcd.print("System Ready...");
  lcd.setCursor(0,1);
}

void loop(){
  digitalWrite(ledPin, LOW);            // Initiate LED and Step Pin States
  printTimerRunning();

//  printTime (elapsedTime, 0, 1, "Total: ");

//  DisplayLapTime();
  buttonState = digitalRead(buttonPin); // Check for button press, read the button state and store
  stopButtonState = digitalRead(stopButtonPin);
if (buttonState != lastButtonState) {
	if (buttonState == LOW && lastButtonState == HIGH) {			//start button hold
		buttonPressTime = millis();
		lastButtonState = buttonState;
		delay(70);
		buttonTime = 0;
	}
	else if (buttonState == HIGH && lastButtonState == LOW){		// action on button release
		buttonTime = millis() - buttonPressTime;
		if (buttonTime < 3000)
			if (blinking == false)	StartTimer();					// start timer
			else DisplayLapTime();									// Lap
		else if (buttonTime > 5000)
			if (blinking == false) StartSingle();						// Single Start routine
			else StopTimer();										// Stop routine
		lastButtonState = buttonState;
		delay(70);
	}

}


if (stopButtonState == LOW && lastStopButtonState == HIGH && blinking == true) {
	StopTimer();
}

if (blinking == true) {
	printTime (millis()-startTime, 0, 1, "Elapsed:");
}


// run commands at the specified time interval
// blink routine - blink the LED while timing
// check to see if it's time to blink the LED; that is, the difference
// between the current time and last time we blinked the LED is larger than
// the interval at which we want to blink the LED.

// if ( (millis() - previousMillis > interval) ) {
//
//    if (blinking == true){
//       previousMillis = millis();                    // remember the last time we blinked the LED
//
//       digitalWrite(ledPin, HIGH);                   // Pulse the LED for Visual Feedback
//
//       elapsedTime =   millis() - startTime;         // store elapsed time
//       //printTime (elapsedTime, 0, 0, "");
//    }
//
//    else{
//          digitalWrite(ledPin, LOW);                 // turn off LED when not blinking
//          }
// }

}
