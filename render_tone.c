// Jack Tarantino - jtarant3
// Weina Dai - wdai11

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"
#include "wave.h"
#include <math.h>


/*
 * This program renders a continuous tone with inputed 
 * voice, frequency, amplitude, and duration from the command
 * line and then writes it to a WAVE file.
 * Returns: -1 for failed run, 0 for successful run.
 */
int main(int argc, char *argv[]) {
  if (argc < 6) {  // Check to see if proper number of inputs were entered
    fatal_error("Invalid number of inputs");
  }

  FILE *wave = fopen(argv[5], "wb");  // Read in the wave file name and check to see if it was opened properly
  if (wave == NULL) {
    fatal_error("File could not be opened");
  }

  unsigned voice;
  if (sscanf(argv[1], "%u", &voice) != 1) { // Check if unsigned int was entered for voice value
    fatal_error("Invalid voice");
  }
  
  float frequency;
  if (sscanf(argv[2], "%f", &frequency) != 1) {  // Check if float was entered for frequency value
    fatal_error("Invalid frequency");
  }
  
  float amplitude;
  if (sscanf(argv[3], "%f", &amplitude) != 1) { // Check if float was entered for amplitude value
    fatal_error("Invalid amplitude");
  }

  unsigned numsamples;
  if (sscanf(argv[4], "%u", &numsamples) != 1) {  // Check if unsigned was entered for numsamples value
    fatal_error("Invalid sample number");
  }
  
  int16_t *buf = calloc((2 * numsamples), sizeof(int16_t));  // Allocate an array of (numsamples * 2) int16_t and initialize to zero

  if ((voice != 0) && (voice != 1) && (voice != 2)) {  // Check if proper voice value was inputed
    fatal_error("Invalid value for voice input");
  }

  if ((amplitude < 0.00) || (amplitude > 1.00)) {  // Check if proper amplitude value was inputed
    fatal_error( "Invalid value for amplitude input");
  }

  render_voice_stereo(buf, numsamples, frequency, amplitude, voice);  // Render with the input values

  write_wave_header(wave, numsamples);  // Write the wave header

  write_s16_buf(wave, buf, (2 * numsamples));  // Write the values to a wave file

  free(buf);  // Free memory
  fclose(wave);
  
  return 0;

}
