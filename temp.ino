
#define MOTION 0
char parentState = '0';
char childState = '0';

int state = 0;

char start1;

void setup() {
  Serial.begin(115200);
  pinMode(0, OUTPUT);
  digitalWrite(0, LOW);
}

void loop() { 
  if (Serial.available() > 0) {
    switch(state) {
      case 0: 
        start1 = Serial.read();
        Serial.print(start1);
        if (start1 == 'S') state++;
        break;
      case 1:
        parentState = Serial.read();
        Serial.print(parentState);
        state++;
        break;
      case 2:
        childState = Serial.read();
        Serial.println(childState);
        state = 0;

        /* Determine power state */
        if (parentState == '1') { // parent detected
          digitalWrite(0, LOW); // ON
        } else if (childState == '1') { // child w/o parent detected
          digitalWrite(0, HIGH); // OFF
        } else {
          digitalWrite(0, LOW); // ON
        }
        break;
      default: break;
    }
  }

  if (parentState == '0' && MOTION) {
    digitalWrite(0, HIGH); // OFF
  }

}
