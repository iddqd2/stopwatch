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
int buttonPin = 2;                  // button on pin 2
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

long currentTimer=0;
long TotalTime, LapTime, PreviousLapTime;
int LapNumber=0, MaxLap=0;
long lapsTime[99];
int LapsCount;
int WarmUpLap=true, LapsQuantity=2;

int blinkInterval1=200, blinkInterval2=400;
long buttonTime=0, buttonPressTime=0;
int screenCleared = false, screenStopped = false;



void printTimerRunning ()
{
	if (millis() - currentTimer < (unsigned long)blinkInterval1 && blinking == true)
	{
	lcd.setCursor (15,0);
	lcd.print("x");

	}
	else if (millis() - currentTimer < (unsigned long)blinkInterval2 && blinking == true)
	{
	lcd.setCursor (15,0);
	lcd.print(" ");
	}
	else
	{
		currentTimer = millis();
	}
	if (blinking == false) {
		lcd.setCursor (15,0);
//		lcd.print(" ");
	}

}


/*
 * Prints elapsed time
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

//	   if (fractionalMins < 10){                            // pad in leading zeros
//		   lcd.print("0");                                 // add a zero
//	   }

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

/*
 * Prints elapsed time
 * Outputs given countTime in stopwatch format and given Title in "Title:mm:ss:ff" format
 */
void serialprintTime (long countTime, int cursorRow, int cursorLine, char *title)
{
	// Routine to report elapsed time
	   elapsedMinutes = (countTime / 60000L);
	   elapsedSeconds = (countTime / 1000L);              // divide by 1000 to convert to seconds - then cast to an int to print
	   elapsedFrames = (countTime / interval);            // divide by 100 to convert to 1/100 of a second - then cast to an int to print
	   fractional = (int)(elapsedFrames % frameRate);       // use modulo operator to get fractional part of 100 Seconds
	   fractionalSecs = (int)(elapsedSeconds % 60L);        // use modulo operator to get fractional part of 60 Seconds
	   fractionalMins = (int)(elapsedMinutes % 60L);        // use modulo operator to get fractional part of 60 Minutes

	   Serial.print(title);
	   if (fractionalMins < 10){                            // pad in leading zeros
	      Serial.print("0");
	   }

	   Serial.print(itoa(fractionalMins, buf, 10));       // convert the int to a string and print a fractional part of 60 Minutes to the LCD
	      Serial.print(":");


	 if (fractionalSecs < 10){                            // pad in leading zeros
	      Serial.print("0");

	 }

	 Serial.print(itoa(fractionalSecs, buf, 10));          // convert the int to a string and print a fractional part of 60 Seconds to the LCD
      Serial.print(":");


	 if (fractional < 10){                                // pad in leading zeros
	      Serial.print("0");
	 }

     Serial.print(itoa(fractional, buf, 10));
     Serial.println("");

}

/*	Stop timer function.
 *  Stops timer with the last lap time and display total time for the last lap.
 */
void StopTimer()
{
		elapsedTime = buttonPressTime - startTime;
		lcd.setCursor (15,0);
		lcd.print(" ");
		lcd.setCursor(0,1);
		lcd.print("                ");
		if ( TotalTime == 0) {
			printTime (elapsedTime, 0, 1, (char*)"Total:");
			serialprintTime (elapsedTime, 0, 1, (char*)"Total:");

		}
		else {
			printTime (TotalTime, 0, 1, (char*)"Total:");

		serialprintTime (elapsedTime, 0, 1, (char*)"Total:");
		}

		LapNumber=0;
		TotalTime = 0;

}

/* Start timer function
 *
 */
void StartTimer()
{

	  startTime = buttonPressTime;                             // store the start time since button press
//   	  startTime = millis();                             // store the start time
//	  elapsedTime = millis() - startTime;
	  memset(lapsTime, 0, sizeof(lapsTime));
	  LapsCount = 0;
	  PreviousLapTime = 0;
	  blinking = true;                                  // turn on blinking while timing
	  //delay(500);                                         // short delay to debounce switch

	    Serial.print ("START startTime=");
	  	Serial.print (startTime);
		Serial.println("");


	  lcd.setCursor(0,0);
	  lcd.print("                ");
}

/*
 * Countdown start function. Timer counts from 5 to 0
 */
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
	buttonPressTime = millis(); //не учитывать время удержания кнопки
	StartTimer();


}


/*
 * Lap time display function. Prints number of laps and last lap time.
 * Отсечка по нажатию кнопки.
 */
void DisplayLapTime()
{

	//blinking = false;                                    // turn off blinking, all done timing
	//		startTime = buttonPressTime;
	elapsedTime = buttonPressTime - startTime;		//since button press, not release
	LapTime = elapsedTime - PreviousLapTime;
	if ( LapNumber <99) {
		lapsTime[LapNumber+1] = LapTime;
	};
	if (LapNumber > LapsQuantity) {
		StopTimer();
	}
	else {
		PreviousLapTime = elapsedTime;
		TotalTime = elapsedTime;
		LapNumber++;
		MaxLap=LapNumber;

//		Serial.print ("buttonpresstime=");
//		Serial.print (buttonPressTime);
//		Serial.println("");
//		Serial.print ("startTime=");
//		Serial.print (startTime);
//		Serial.println("");
//		Serial.print ("elapsedTime=");
//		Serial.print (elapsedTime);
//		Serial.println("");
//		Serial.print ("LapTime=");
//		Serial.print (LapTime);
//		Serial.println("");
		if (LapNumber == 0) {
			lcd.print ("WarmUp Lap");
			Serial.print ("WarmUp Lap");
		}
		printTime (LapTime, 0, 0, (char*)"L :");
		Serial.print ("Lap#");
		Serial.print(LapNumber);
		serialprintTime (LapTime, 0, 0, (char*)":");
		lcd.setCursor (1,0);
		lcd.print(LapNumber);
	}

}

void setup()
{
  lcd.begin(16, 2);                // initialise the LCD.
  pinMode(ledPin, OUTPUT);         // sets the digital pin as output
  pinMode(buttonPin, INPUT);       // not really necessary, pins default to INPUT anyway
  digitalWrite(buttonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.
  digitalWrite(stopButtonPin, HIGH);   // turn on pullup resistors. Wire button so that press shorts pin to ground.
  Serial.begin(9600);
  lcd.setCursor(0,5);
  lcd.print("Ready");
  lcd.setCursor(0,1);
  Serial.println("Ready");
}

void loop(){
  digitalWrite(ledPin, LOW);            // Initiate LED and Step Pin States
  printTimerRunning();

  buttonState = digitalRead(buttonPin); 			// Check for button press, read the button state and store
  stopButtonState = digitalRead(stopButtonPin);
if (buttonState != lastButtonState) {
	if (buttonState == LOW && lastButtonState == HIGH) {			//start button hold
		buttonPressTime = millis();

		Serial.print (buttonPressTime);
		Serial.println("");

		lastButtonState = buttonState;
		//delay(70);
		buttonTime = 0;
	}

	else if (buttonState == HIGH && lastButtonState == LOW){		// action on button release
		buttonTime = millis() - buttonPressTime;
		if (buttonTime < 5000 && buttonTime > 100)
			if (blinking == false)	StartTimer();					// start timer
			else DisplayLapTime();									// Lap
		else if (buttonTime > 5000) {
			if (blinking == false) StartSingle();						// Single Start routine
			else {
//				StopTimer();
				blinking = false;// Stop routine
				buttonTime = 0;
			}
		}
		lastButtonState = buttonState;
		screenCleared = false;
		screenStopped = false;
		//delay(70);
	}

}

else if (buttonState == LOW && (millis() - buttonPressTime) > 5000) {
	if (blinking == false)
	{
		if (screenCleared == false){
			lcd.clear();
			lcd.print("   Go back to");
			lcd.setCursor(0,1);
			lcd.print(" start position!");
			screenCleared = true;
		}
	}
	else if (blinking == true)
	{
		if (screenStopped ==false){
//			lcd.clear();
			StopTimer();										// Stop routine
//			DisplayLapTime();
//			   printTime (LapTime, 0, 0, (char*)"Lap# :");
//			   lcd.setCursor (4,0);
//			   lcd.print(LapNumber);
			//buttonTime = 0;
			//lcd.print("Stopped");
			screenStopped = true;
		}
	}

}

if (stopButtonState == LOW && lastStopButtonState == HIGH && blinking == true) {
	StopTimer();
	blinking = false;// Stop routine
	LapsCount = 1;
}

if (stopButtonState == LOW && lastStopButtonState == HIGH && blinking == false) {
	if (LapsCount <= MaxLap) {
		lcd.setCursor(0,0);
		printTime (lapsTime[LapsCount], 0, 0, (char*)"L :");
		lcd.setCursor (1,0);
		lcd.print(LapsCount);
		LapsCount++;
		delay (300);
	}
	else {
		LapsCount = 1;
		lcd.setCursor(0,0);
		printTime (lapsTime[LapsCount], 0, 0, (char*)"L :");
		lcd.setCursor (1,0);
		lcd.print(LapsCount);
		LapsCount++;
		delay(300);
	}
}

if (blinking == true && screenCleared == false && screenStopped == false) {
	printTime (millis()-startTime, 0, 1, (char*)"Elapsed:");
}

}
