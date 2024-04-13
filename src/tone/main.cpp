#include <WProgram.h>

#include "notes.h"

// Define the buzzer pin
int buzzerPin = 0;

struct {
    int note;
    int duration;
} melody[] = {
    { NOTE_E4, 8 },
    { NOTE_E4, 8 },
    { NOTE_E4, 4 },

    { NOTE_E4, 8 },
    { NOTE_E4, 8 },
    { NOTE_E4, 4 },

    { NOTE_E4, 8 },
    { NOTE_G4, 8 },
    { NOTE_C4, 8 },
    { NOTE_D4, 8 },
    { NOTE_E4, 2 },

    { NOTE_F4, 8 },
    { NOTE_F4, 8 },
    { NOTE_F4, 8 },
    { NOTE_F4, 8 },

    { NOTE_F4, 8 },
    { NOTE_E4, 8 },
    { NOTE_E4, 8 },
    { NOTE_E4, 16 },
    { NOTE_E4, 16 },

    { NOTE_E4, 8 },
    { NOTE_D4, 8 },
    { NOTE_D4, 8 },
    { NOTE_E4, 8 },

    { NOTE_D4, 4 },
    { NOTE_G4, 4 },
};

#define NUM_NOTES (sizeof(melody) / sizeof(melody[0]))

void play_tune()
{
    // Iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < NUM_NOTES; thisNote++) {
        // To calculate the note duration, take one second divided by the note type.
        int noteDuration = 1000 / melody[thisNote].duration;
        tone(buzzerPin, melody[thisNote].note, noteDuration);

        // To distinguish the notes, set a minimum time between them.
        int pauseBetweenNotes = noteDuration * 1.30;
        delay(pauseBetweenNotes);

        // Stop the tone playing:
        noTone(buzzerPin);
    }
}

void setup()
{
    pinMode(1, INPUT);
    digitalWrite(1, HIGH);
}

void loop()
{
    // while (HIGH == digitalRead(1)) {
    // }
    play_tune();
    // while (LOW == digitalRead(1)) {
    // }
}