int clockPin = 5;
int latchPin = 4;
int dataPin = 6;
int octave = 5;

int speakerPin = 3;

uint16_t prevInput; //Stores the last input to change the note when a new key is pressed

float hertz[12] = {
  61.74, // B
  58.27, // A#
  55.00, // A
  51.91, // G#
  49.00, // G
  46.25, // F#
  43.65, // F
  41.20, // E
  38.89, // D#
  36.71, // D
  34.65, // C#
  32.70  // C
};



uint16_t keys;

void setup() {
  // put your setup code here, to run once:
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, INPUT);
  Serial.begin(115200);

  octave = octave -1; //fix obo error, makes it easier to input

}

uint16_t readKeys() {
  uint16_t data = 0;            //reset key data
  digitalWrite(latchPin, LOW);  //load keys into shift register
  delayMicroseconds(5);
  digitalWrite(latchPin, HIGH);  //Lock data into the register
  for (int i = 0; i < 16; i++) {
    data <<= 1;
    data |= digitalRead(dataPin);
    digitalWrite(clockPin, HIGH);  //shift bits
    delayMicroseconds(20);
    digitalWrite(clockPin, LOW);  //reset for next loop
  }

  return data;
}

void printBinary16(uint16_t value) {
  for (int i = 15; i >= 0; i--) {
    Serial.print((value >> i) & 1);
  }
  Serial.println();
}

void playNote(uint16_t value){
  if (prevInput == value){}
  else{
    for(int i = 11; i>=0; i--){
      if(bitRead(value, i + 4)){ // +4 skips the empty inputs
        tone(speakerPin, hertz[i] * pow(2, octave));
        Serial.println(hertz[i] * pow(2, octave));
      }
    }
  }
  if(value == 0){noTone(speakerPin);}
  prevInput = value;
}

void loop() {
  // put your main code here, to run repeatedly:
  keys = readKeys();

  printBinary16(keys);
  playNote(keys);
  }
