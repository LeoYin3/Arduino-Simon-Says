
// Global Variables, doesn't initialize arduino parts


int button[] = {10yu,11,12,13};  

int led[] = {4,5,6,7};  


// red is button 10, LED  4 (button[0], LED[0])

// yellow is button 11, LED 5 (button[1], LED[1])

// green is button '12, LED 6 (button[2], LED[2])

// blue is button 13, LED 7 (button[3], LED[3])


int tones[] = {262, 330, 392, 494};  


int roundsToWin = 10;        

int buttonSequence[16];      


int buzzerPin = 2;          


int pressedButton = 4;  

// a variable to remember which button is being pressed. 4 is the value if no buzzer is being pressed. (changes but starts at 4 so if the four doesn't change then you know nothings been pressed)


int roundCounter = 1;         //keeps track of what round the player is on


long startTime = 0;           //time limit on button press

long timeLimit = 2000;        //time limit to hit a button


bool gameStarted = false;      //variable to tell the game whether or not to play the start sequence (should change to true eventually)


void setup() {

//function to run once, doesn't repeat. Initializes buttons.

  pinMode(button[0], INPUT_PULLUP);

  pinMode(button[1], INPUT_PULLUP);

  pinMode(button[2], INPUT_PULLUP);

  pinMode(button[3], INPUT_PULLUP);


  pinMode(led[0], OUTPUT);

  pinMode(led[1], OUTPUT);

  pinMode(led[2], OUTPUT);

  pinMode(led[3], OUTPUT);


  pinMode(buzzerPin, OUTPUT);

}


void loop() {

// loop for if game hasn't started yet

  if (gameStarted == false) {  

    startSequence();            

    roundCounter = 0;          

    delay(1500);              

    gameStarted = true;        

  }


  //loop to start each round by flashing out the sequence to be repeated

  for (int i = 0; i <= roundCounter; i = i + 1) { //go through array up to the current round

    flashLED(buttonSequence[i]);          

    delay(200);                          

    allLEDoff();                          

    delay(200);

  }


  //then start going through the sequence one at a time and see if the user presses the correct button

  for (int i = 0; i <= roundCounter; i = i + 1) { //for each button to be pressed in the sequence


    startTime = millis();                 //record the start time


    while (gameStarted == true) { //loop until the player presses a button or the time limit is up (the time limit check is in an if statement)


      pressedButton = buttonCheck();      //every loop check to see which button is pressed


      if (pressedButton < 4) {            //if a button is pressed... (4 means that no button is pressed)


        flashLED(pressedButton);          //flash the LED for the button that was pressed


        if (pressedButton == buttonSequence[i]) { //if the button matches the button in the sequence

          delay(250);                   //leave the LED light on for a moment

          allLEDoff();                  //then turn off all of the lights and

          break;                        //end the while loop (this will go to the next number in the for loop)


        } else {                          //if the button doesn't match the button in the sequence

          loseSequence();               //play the lose sequence (the loose sequence stops the program)

          break;                        //when the program gets back from the lose sequence, break the while loop so that the game can start over

        }


      } else {                            //if no button is pressed

        allLEDoff();                      //turn all the LEDs off

      }


      //check to see if the time limit is up

      if (millis() - startTime > timeLimit) { //if the time limit is up

        loseSequence();                       //play the lose sequence

        break;                                //when the program gets back from the lose sequence, break the while loop so that the game can start over

      }

    }

  }


  if (gameStarted == true) {

    roundCounter = roundCounter + 1;      //increase the round number by 1


    if (roundCounter >= roundsToWin) {              //if the player has gotten to the 16th round

      winSequence();                      //play the winning song

    }


    delay(500);                           //wait for half a second between rounds

  }


}


//----------HELPER FUNCTIONS------------


//FLASH LED

void flashLED (int ledNumber) {

  digitalWrite(led[ledNumber], HIGH);

  tone(buzzerPin, tones[ledNumber]);

}


//TURN ALL LEDS OFF

void allLEDoff () {

  //turn all the LEDs off

  digitalWrite(led[0], LOW);

  digitalWrite(led[1], LOW);

  digitalWrite(led[2], LOW);

  digitalWrite(led[3], LOW);

  //turn the buzzer off

  noTone(buzzerPin);

}


//CHECK WHICH BUTTON IS PRESSED

int buttonCheck() {

  //check if any buttons are being pressed

  if (digitalRead(button[0]) == LOW) {

    return 0;

  } else if (digitalRead(button[1]) == LOW) {

    return 1;

  } else if (digitalRead(button[2]) == LOW) {

    return 2;

  } else if (digitalRead(button[3]) == LOW) {

    return 3;

  } else {

    return 4; //this will be the value for no button being pressed

  }

}


//START SEQUENCE

void startSequence() {


// if you read something off of a pin with nothing connected, it returns random

  randomSeed(analogRead(A0));


  //populate the buttonSequence array with random numbers from 0 to 3

  for (int i = 0; i <= roundsToWin; i = i + 1) {

    buttonSequence[i] = round(random(0, 4));

  }


  //flash all of the LEDs when the game starts

  for (int i = 0; i <= 3; i = i + 1) {


    tone(buzzerPin, tones[i], 200); //play one of the 4 tones


    //turn all of the leds on

    digitalWrite(led[0], HIGH);

    digitalWrite(led[1], HIGH);

    digitalWrite(led[2], HIGH);

    digitalWrite(led[3], HIGH);


    delay(100);        


    //turn all of the leds off

    digitalWrite(led[0], LOW);

    digitalWrite(led[1], LOW);

    digitalWrite(led[2], LOW);

    digitalWrite(led[3], LOW);


    delay(100);  


  } //this will repeat 4 times

}


//WIN SEQUENCE

void winSequence() {


  //turn all the LEDs on

  for (int j = 0; j <= 3; j = j + 1) {

    digitalWrite(led[j], HIGH);

  }


  //play the 1Up noise

  tone(buzzerPin, 1318, 150);   //E6

  delay(175);

  tone(buzzerPin, 1567, 150);   //G6

  delay(175);

  tone(buzzerPin, 2637, 150);   //E7

  delay(175);

  tone(buzzerPin, 2093, 150);   //C7

  delay(175);

  tone(buzzerPin, 2349, 150);   //D7

  delay(175);

  tone(buzzerPin, 3135, 500);   //G7

  delay(500);


  do {

    pressedButton = buttonCheck();

  } while (pressedButton > 3);

  delay(100);


  gameStarted = false;   //reset the game so that the start sequence will play again.


}


//LOSE SEQUENCE

void loseSequence() {


  //turn all the LEDs on

  for (int j = 0; j <= 3; j = j + 1) {

    digitalWrite(led[j], HIGH);

  }


  //play the 1Up noise

  tone(buzzerPin, 130, 250);   //E6

  delay(275);

  tone(buzzerPin, 73, 250);   //G6

  delay(275);

  tone(buzzerPin, 65, 150);   //E7

  delay(175);

  tone(buzzerPin, 98, 500);   //C7

  delay(500);


  //wait until a button is pressed

  do {

    pressedButton = buttonCheck();

  } while (pressedButton > 3);

  delay(200);


  gameStarted = false;   //reset the game so that the start sequence will play again.

}
