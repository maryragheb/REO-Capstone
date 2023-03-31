/* nrf msg format

  || S | parentState | childState ||
  State = 1 -> detected
  State = 0 -> not detected
*/

/* Test mode using LEDs (On = pwr enabled) */


/* GPIO input pin for motion sensor */
int motionPin = 2;
int motionDetected = HIGH;

/* Parent bluetooth signal (1 = detected) */
char parentState = '0';

/* Child bluetooth signal (1 = detected) */
char childState = '0';

/* 
  UART comms state 
    0 -> start 'S'
    1 -> parentState
    2 -> childState
*/
int state = 0;

/* Start char of UART msg ('S') */
char comms_start;

void setup() {
  /* Set UART baud rate */
  Serial.begin(115200);

  /* Motion sensor pin */
  pinMode(motionPin, INPUT);

  if (LED_MODE) {
    pinMode(0, OUTPUT);
    digitalWrite(0, LOW);
  }
}

void loop() {
  /* Read updated bluetooth state */
  if (Serial.available() > 0) {
    switch (state) {
      case 0:
        comms_start = Serial.read();
        if (comms_start == 'S') state++;
        break;
      case 1:
        parentState = Serial.read();
        state++;
        break;
      case 2:
        childState = Serial.read();
        state = 0;

        /* Determine power state */
        if (parentState == '1') { // parent detected
          digitalWrite(0, LOW);   // ON
        } else if (childState == '1') {  // child w/o parent detected
          digitalWrite(0, HIGH);  // OFF
        } else {
          digitalWrite(0, LOW);   // ON
        }
        break;
      default: 
        break;
    }
  }

  /* Motion detection fail safe */
  motionDetected = digitalRead(motionPin);
  if (parentState == '0' && motionDetected == LOW) { 
    if (LED_MODE) digitalWrite(0, HIGH);  // OFF
  }

}
