const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogOutPin1 = 10;
const int analogOutPin2 = 5;
const int analogOutPin3 = 6;
const int analogOutPin4 = 8;
unsigned long previousMillis = 0;        // will store last time LED was updated
unsigned long clickMillis;
unsigned long previousBeat;
unsigned long defibrillatorMillis;
unsigned long bpm;
long interval;         // interval at which to blink (milliseconds)

int sensorValue = 0;        // value read from the pot
int outputValue = 0;        // value output to the PWM (analog out)
int t1 = 140;
int t2 = 150;
int R;
int inByte1;

int automatic;
int defibrillator;

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(10, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(8, OUTPUT);
}

void loop() {
// check if data is available on the serial port
  if (Serial.available() > 0) {
    int inByte2 = inByte1;
    inByte1 = Serial.read();
    if (inByte2 == 'x' ) {
      t1 = inByte1;
      //Serial.print(t1);
    }
    //switch case to specify the interval according to character in serial port
    switch (inByte1) {
      case 'a':
        interval = 40;
        clickMillis = millis();
        break;
      case 'b':
        interval = 20;
        clickMillis = millis();
        break;
      case 'c':
        interval = 10;
        clickMillis = millis();
        break;
      case 'd':
        interval = 5;
        clickMillis = millis();
        break;
      case 'e':
        interval = 0;
        break;
      case 'f':
        if (automatic == 0) {
          digitalWrite(10, HIGH);
        }
        break;
      case 'g':
        if (automatic == 0) {
          digitalWrite(10, LOW);
        }
        break;
      case 'h':
        if (automatic == 0) {
          digitalWrite(5, HIGH);
        }
        break;
      case 'i':
        if (automatic == 0) {
          digitalWrite(5, LOW);
        }
        break;
      case 'j':
        if (automatic == 0) {
          digitalWrite(6, HIGH);
        }
        break;
      case 'k':
        if (automatic == 0) {
          digitalWrite(6, LOW);
        }
        break;
      case 'l':
        if (automatic == 0) {
          digitalWrite(8, HIGH);
        }
        break;
      case 'm':
        if (automatic == 0) {
          digitalWrite(8, LOW);
        }
        break;
      case 'n':
        automatic = 1;
        break;
      case 'o':
        automatic = 0;
        break;
    }
  }



  unsigned long currentMillis = millis();

  if (interval > 0 and currentMillis - clickMillis <= 2000) {
    if (currentMillis - previousMillis >= interval) {
      previousMillis = currentMillis;

      sensorValue = analogRead(analogInPin);
      outputValue = map(sensorValue, 0, 1023, 0, 255);
      Serial.write(outputValue);

      if (automatic == 1) {

        if (outputValue > t2 && R == 0) {
          R = 1;
          bpm = 60000 / (currentMillis - previousBeat);
          previousBeat = currentMillis;

          //Serial.println(bpm);

          if (bpm > 160) {
            digitalWrite(10, HIGH);
          }
          else {
            digitalWrite(10, LOW);
          }
          if (bpm > 30 and bpm < 40) {
            digitalWrite(5, HIGH);
          }
          else {
            digitalWrite(5, LOW);
          }
          if (bpm > 10 and bpm < 30) {
            digitalWrite(6, HIGH);
          }
          else {
            digitalWrite(6, LOW);
          }

        }

        if (outputValue < t1 && R == 1) {
          R = 0;
        }
        // activate defibrillator if no beat detected for 10 seconds
        if (currentMillis - previousBeat >= 10000 && defibrillator == 0)  {
          digitalWrite(8, HIGH);
          digitalWrite(10, LOW);
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          defibrillator = 1;
          defibrillatorMillis = millis();
          bpm = 0;
        }
        // deactivate defibrillator after 15 seconds (10s wait + 5s on)
        if (currentMillis - previousBeat >= 15000 && defibrillator == 1) {
          digitalWrite(8, LOW);
          digitalWrite(10, LOW);
          digitalWrite(5, LOW);
          digitalWrite(6, LOW);
          defibrillator = 0;
          defibrillatorMillis = 0;
          previousBeat = currentMillis;
        }

        if (currentMillis - defibrillatorMillis >= 5000 && defibrillator == 1 && bpm != 0) {
          digitalWrite(8, LOW);
          defibrillator = 0;
          defibrillatorMillis = 0;
        }

      }
    }
  }
}
