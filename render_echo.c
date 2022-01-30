// Jack Tarantino - jtarant3
// Weina Dai - wdai11

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"
#include "wave.h"
#include <math.h>


/* 
 *need to open the input file, call read_wave_header to read the WAVE header an *d get the number of (stereo) samples, then read the sample data into an array *Adding the echo effe * ct can be accomplished by adding attenuated sample values *to the sample value at a later audio position
 */

int main(int argc, char *argv[]) {

  if (argc < 5) {   // Check for proper number of command line inputs
    fatal_error("Invalid number of inputs");
    
  }

  FILE * wavefilein = fopen(argv[1], "rb"); // Open input wavefile and do proper checks
  if (wavefilein == NULL) {
    fatal_error("Cannot open input file");
  }

  int delay;
  if (sscanf(argv[3], "%d", &delay) != 1) {  // Check that int was read in for delay value
    fatal_error("Invalid delay number");
  }

  float echoamp;
  if (sscanf(argv[4], "%f", &echoamp) != 1) {  // Check that float was entered for the echoamp value
    fatal_error("Invalid amplitude");
  }

  unsigned numsamples;
  read_wave_header(wavefilein, &numsamples);  // Obtain number of samples from the wave file header

  int16_t * buf = calloc((size_t) (numsamples * 2), sizeof(int16_t));  // Allocate memory and initialize buf array to zero
  read_s16_buf(wavefilein, buf, numsamples * 2);  // Read in values to buf array

  int16_t * temp = calloc((size_t) (numsamples * 2), sizeof(int16_t)); // Allocate memory and intitialize temporary array to zero

  for (unsigned i = delay * 2; i < numsamples * 2; ++i) { // Adjust temp values for the echo delay

    temp[i] += (echoamp / 1.0) * buf[i - delay * 2];
   
  }

  for (unsigned i = 0; i < numsamples * 2; ++i) {  // Assign buf values to the updated temp values

    buf[i] += temp[i];
    
  }

  FILE * wavefileout = fopen(argv[2], "wb");  // Open wave file and do the proper checks
  if (wavefileout == NULL) {
    free(temp);
    free(buf);
    fatal_error("Cannot open output file");
  }

  // Write_wave_header(FILE *out, unsigned num_samples);
  write_wave_header(wavefileout, numsamples);

  // Write_s16_buf(FILE *out, const int16_t buf[], unsigned n)                  
  write_s16_buf(wavefileout, buf, (numsamples * 2));

  // Free the memory and close the files
  fclose(wavefileout);
  fclose(wavefilein);
  free(buf);
  free(temp);
  
  return 0;
}
