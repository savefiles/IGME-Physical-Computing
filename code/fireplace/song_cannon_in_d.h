#include "notes_define.h"

//https://github.com/robsoncouto/arduino-songs/blob/master/cannonind/cannonind.ino

// change this to make the song slower or faster
int tempo = 100;

// change this to whichever pin you want to use
int buzzer = 11;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {

  // Cannon in D - Pachelbel
  // Score available at https://musescore.com/user/4710311/scores/1975521
  // C F
  NOTE_FS4,2, NOTE_E4,2,
  NOTE_D4,2, NOTE_CS4,2,
  NOTE_B3,2, NOTE_A3,2,
  NOTE_B3,2, NOTE_CS4,2,
  NOTE_FS4,2, NOTE_E4,2,
  NOTE_D4,2, NOTE_CS4,2,
  NOTE_B3,2, NOTE_A3,2,
  NOTE_B3,2, NOTE_CS4,2,
  NOTE_D4,2, NOTE_CS4,2,
  NOTE_B3,2, NOTE_A3,2,
  NOTE_G3,2, NOTE_FS3,2,
  NOTE_G3,2, NOTE_A3,2,

  NOTE_D4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,4, NOTE_FS4,8, NOTE_G4,8, 
  NOTE_A4,4, NOTE_B3,8, NOTE_CS4,8, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, 
  NOTE_FS4,4, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,4, NOTE_FS3,8, NOTE_G3,8,
  NOTE_A3,8, NOTE_G3,8, NOTE_FS3,8, NOTE_G3,8, NOTE_A3,2,
  NOTE_G3,4, NOTE_B3,8, NOTE_A3,8, NOTE_G3,4, NOTE_FS3,8, NOTE_E3,8, 
  NOTE_FS3,4, NOTE_D3,8, NOTE_E3,8, NOTE_FS3,8, NOTE_G3,8, NOTE_A3,8, NOTE_B3,8,

  NOTE_G3,4, NOTE_B3,8, NOTE_A3,8, NOTE_B3,4, NOTE_CS4,8, NOTE_D4,8,
  NOTE_A3,8, NOTE_B3,8, NOTE_CS4,8, NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2,
  NOTE_A4,4, NOTE_FS4,8, NOTE_G4,8, NOTE_A4,4,
  NOTE_FS4,8, NOTE_G4,8, NOTE_A4,8, NOTE_A3,8, NOTE_B3,8, NOTE_CS4,8,
  NOTE_D4,8, NOTE_E4,8, NOTE_FS4,8, NOTE_G4,8, NOTE_FS4,4, NOTE_D4,8, NOTE_E4,8,
  NOTE_FS4,8, NOTE_CS4,8, NOTE_A3,8, NOTE_A3,8,

  NOTE_CS4,4, NOTE_B3,4, NOTE_D4,8, NOTE_CS4,8, NOTE_B3,4,
  NOTE_A3,8, NOTE_G3,8, NOTE_A3,4, NOTE_D3,8, NOTE_E3,8, NOTE_FS3,8, NOTE_G3,8,
  NOTE_A3,8, NOTE_B3,4, NOTE_G3,4, NOTE_B3,8, NOTE_A3,8, NOTE_B3,4,
  NOTE_CS4,8, NOTE_D4,8, NOTE_A3,8, NOTE_B3,8, NOTE_CS4,8, NOTE_D4,8, NOTE_E4,8,
  NOTE_FS4,8, NOTE_G4,8, NOTE_A4,2,  
   
  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes = sizeof(melody) / sizeof(melody[0]) / 2;

// this calculates the duration of a whole note in ms
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;
