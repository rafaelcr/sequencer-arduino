#define SEQ_START 13
#define SEQ_COUNT 4

#define NOTE_START 9
#define NOTE_COUNT 4

#define TEMPO_PIN A0
#define TEMPO_MIN 60
#define TEMPO_MAX 300

#define NOTE_C3 0x30
#define NOTE_Eb3 0x33
#define NOTE_G3 0x37
#define NOTE_C4 0x3C


void setup() {
  // Sequence pins as an output.
  for (int i = 0; i < SEQ_COUNT; i++) {
    pinMode(SEQ_START - i, OUTPUT);
  }

  // Note pins as input.
  for (int i = 0; i < NOTE_COUNT; i++) {
    pinMode(NOTE_START - i, INPUT);
  }

  // Set to MIDI baud rate.
  Serial.begin(31250);
}

void loop() {
  // Do the sequence on tempo.
  for (int i = 0; i < SEQ_COUNT; i++) {
    // Get sequence and note pins.
    int seq_pin = SEQ_START - i;
    int note_pin = NOTE_START - i;

    // Play note if enabled.
    digitalWrite(seq_pin, HIGH);
    float tempo_delay = tempoDelay();
    if (digitalRead(note_pin) == HIGH) {
      playNote(i, tempo_delay);
    } else {
      // Delay to keep on tempo.
      delay(tempo_delay);
    }

    // Move to next note.
    digitalWrite(seq_pin, LOW);
  }
}

// Get tempo delay based on analog pot read.
float tempoDelay() {
  float tempo_input = analogRead(TEMPO_PIN);
  float tempo_mult = tempo_input / (float)1023;
  float tempo = TEMPO_MIN + tempo_mult * (TEMPO_MAX - TEMPO_MIN);
  float delay_ms = 60 / tempo * 1000;

  return delay_ms;
}

// Plays a MIDI note given an input pin.
void playNote(int note, float tempo_delay) {
  int note_value = NOTE_C3;
  switch(note) {
    case 0: note_value = NOTE_C3; break;
    case 1: note_value = NOTE_Eb3; break;
    case 2: note_value = NOTE_G3; break;
    case 3: note_value = NOTE_C4; break;
  }

  Serial.write(0x90);
  Serial.write(note_value);
  Serial.write(0x45);
  delay(tempo_delay);
  Serial.write(0x90);
  Serial.write(note_value);
  Serial.write(0x00);
}
