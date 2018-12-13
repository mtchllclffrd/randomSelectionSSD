// Proof of concept code written for a client needing a push button to randomly select a choice on a vending machine
// In this code LEDs blink in a slow pattern when nothing happens
// When the button is depressed, the LEDs blink in a faster pattern
// Then when the button is released an LED is selected based off of a continuous counter to blink
// The randomness is based on the timing of a person pressing and releasing the button
// Written by Mitchell Clifford (mtchllclffrd.com)
// Proof read by Nik Lockwood (nikolardo.com)

class Blinking{ // Creates an object for the LEDs that will blink using a timing feature
  int ledPin;   // Initial LED pin                                          

  int ledState; // The State LOW or HIGH of that LED
  unsigned long previousMillis; // Time elapsed since last update of the LED

  long on;
  long off;

  public:
  Blinking(int pin){ // Set generic LED pin to OUTPUT
    ledPin = pin;
    pinMode(ledPin, OUTPUT);

    ledState = LOW; // Set initial LED State is LOW
    previousMillis = 0; // Set initial time to 0
  }

  int OnTime(int delayTime){ // If the delay time is HIGH chose the active time for the slow LED blinking pattern
    if (delayTime == HIGH){
      return 250;
    }
    else{ // Else chose the fast active time for the LED blinking pattern
      return 125;
    }
  }

  int OffTime(int delayTime){ //Same as above but for the delay for the slow LED blinking pattern
    if (delayTime == HIGH){
      return 750;
    }
    else{ // Else chose the fast delay for the LED blinking pattern
      return 375;
    }
  }
  
  void Update(int reset, int delayTime){  // Creates an Update function that is uses a reset and a HIGH or LOW delay time to determine the state of the LED
   unsigned long currentMillis = millis(); // Set the current time

   on = OnTime(delayTime); //Set the active time
   off = OffTime(delayTime); //Set the delay time

   if (reset == HIGH){ // If the reset is HIGH turn off the LED and reset the previous time to 0
    ledState = LOW;
    digitalWrite(ledPin, ledState);
    previousMillis = 0; 
   }
   else if ((ledState == HIGH) && (currentMillis - previousMillis >= on)){ // If the LED state is HIGH and the time elapsed is equal or greater than the LED on time, change the LED state to LOW
     ledState = LOW;
     previousMillis = currentMillis;
     digitalWrite(ledPin, ledState);
   }
   else if ((ledState == LOW) && (currentMillis - previousMillis >= off)){
      ledState = HIGH;
      previousMillis = currentMillis;
     digitalWrite(ledPin, ledState);
    }
  }

  void Blink(){ // Creates a blink function that will blink with specified delays
    long timer = 500;
    for (int blinkAmount = 0; blinkAmount <= 3; blinkAmount ++){ //Change the blinkAmount inequality to alter the amount of blinks that time that the LED blinks
      digitalWrite(ledPin, HIGH);
      delay(timer);
      digitalWrite(ledPin, LOW);
      delay(timer);
    }
  }
};

int buttonPin = 7; // Sets the button pin
int lastButtonState = 0; // Sets the last button state
int currentButtonState = 0;
long counter = 0; // Initializes a counter variable that is used in the randomization

Blinking led0(3); // Initializes LED0 on pin 3
Blinking led1(4); // Initializes LED1 on pin 4
Blinking led2(5); // Initializes LED2 on pin 5

int ledAmount = 3; //Indicates how many LEDs are being used
Blinking ledArray[] = {led0, led1, led2}; //Creates an array of LED objects

int ledIndex = 0;

void setup() {

  pinMode(buttonPin, INPUT); //Sets up button as an input
  
  Serial.begin(9600); // Serial used for debugging

}

void loop() {
  currentButtonState = digitalRead(buttonPin); // Read if the button is pressed

  counter ++;
  if (counter > ledAmount){ //reset the counter if it goes above the amount of LEDs
    counter = 0;
  }

  if (currentButtonState == 1){ // If the button is pressed start a counter and change LED behavior
    for (ledIndex = 0; ledIndex < ledAmount; ledIndex ++){ //Iterates through the LED array applying the update function for the fast pattern
      ledArray[ledIndex].Update(LOW, LOW);
    }
  }
  else if (currentButtonState == 0 && lastButtonState == 1){ // If the button is released, reset the other LEDs, blink the led that corresponds to the current counter and then reset that LED
     for (ledIndex = 0; ledIndex < ledAmount; ledIndex ++){ //Resets the LED objects
      ledArray[ledIndex].Update(HIGH, LOW);
     }
     ledArray[counter].Blink(); //Blinks the random LED
  }
  else if (currentButtonState == 0 && lastButtonState == 0){ // If neither happens blink the LEDs at a slow speed
    for (ledIndex = 0; ledIndex < ledAmount; ledIndex ++){ //Iterates through the LED array applying the update function for the slow pattern
      ledArray[ledIndex].Update(LOW, HIGH);
    }
  }
  else{ // If the button is pressed and released throw an error
    Serial.println("error in button press state changes");
  }
  lastButtonState = currentButtonState;
  delay(10);
}
