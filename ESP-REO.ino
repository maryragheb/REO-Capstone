/* nrf msg format

  || S | parentState | childState ||
  State = 1 -> detected
  State = 0 -> not detected
*/

/* Test mode using LEDs (On = pwr enabled) */


/* GPIO input pin for motion sensor */
int motionPin = 16; /* TODO: GPIO12/13/14/16 pick any one */
int motionDetected = HIGH;

int ac1EnPin = 4; /* TODO: GPIO4 */
int ac2EnPin = 5; /* TODO: GPIO5 */

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

void enableAC() {
  digitalWrite(ac1EnPin, HIGH);
  digitalWrite(ac2EnPin, HIGH); 
}

void disableAC() {
  digitalWrite(ac1EnPin, LOW);
  digitalWrite(ac2EnPin, LOW);
}

void setup() {
  /* Set UART baud rate */
  Serial.begin(115200);

  /* Motion sensor pin */
  pinMode(motionPin, INPUT);
  pinMode(ac1EnPin, OUTPUT);
  pinMode(ac2EnPin, OUTPUT);

  enableAC();
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
          enableAC();             // ON
        } else if (childState == '1') {  // child w/o parent detected
          disableAC();            // OFF
        } else {
          enableAC();             // ON
        }
        break;
      default: 
        break;
    }
  }

  /* Motion detection fail safe */
  motionDetected = digitalRead(motionPin);
  if (parentState == '0' && motionDetected == LOW) { 
    disableAC();  // OFF
  } else if (motionDetected == HIGH && childState == '0') {
    enableAC(); // ON
  }

}
