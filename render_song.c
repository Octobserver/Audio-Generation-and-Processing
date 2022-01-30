//Jack Tarantino - jtarant3
//Weina Dai - wdai11

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"
#include "wave.h"
#include <math.h>


/*                                                                             
 * This program renders a song
 * with the input text file that describes a song and 
 * write the song to the output .wav file
 * Returns: -1 for failed run, 0 for successful run.                           
 */
int main(int argc, char *argv[]) {

  if (argc < 3) {  // Check if the user enters correct number of command line arguements
    fatal_error("Invalid number of inputs");
  }

  FILE * songinput = fopen(argv[1], "r"); // Open input textfile and do proper checks
  if (songinput == NULL) {
    fatal_error("Cannot open input file");
  }
   
  unsigned numsamples; // Read the numer of samples from the songinput
  if (fscanf(songinput, "%u ", &numsamples) != 1) {
    fclose(songinput);
    fatal_error("Cannot parse sample number");
  } 

  unsigned beat; // Read the number of beats from songinput
  if (fscanf(songinput, "%u ", &beat) != 1) {
    fclose(songinput);
    fatal_error("Cannot parse beat length");
  }

  int16_t * buf = calloc((size_t) (numsamples * 2), sizeof(int16_t));  // Allocate memory and initialize buf array to zero
 
  char cur;  // Switch case value
  int curvoice = 0; // Current voice
  float curamp = 0.1; // Current amplitude
  int i = 0; // Index

  float b; // Beat
  int n; // MIDI note number 
  int length; // Partition of a sample
  float freq; // Frequency
  
  while((cur = fgetc(songinput)) != EOF && cur != '\n') {  // While we have input

    switch (cur) {  // Switch case to take in data from file and make proper sound/pause

    case 'N':  // Note Case
      if (fscanf(songinput, "%f", &b) != 1) {  // Check for valid beat input
	free(buf);
        fatal_error("Cannot parse beat");  
      } 
      if (fscanf(songinput,"%d", &n) != 1) {  // Check for valid note input
	free(buf);
        fatal_error("Cannot parse MIDI note number");
      }
      
      length = (int)(b * beat);  // Make proper adjustments for length
      freq = (float)(440 * pow(2, (double)((n - 69.0) / 12.0)));  // Adjust the frequency
      render_voice_stereo(&buf[i], length, freq, curamp, curvoice);

      i += 2 * length;  // Update index value

      break;

    case 'C':  // Chord Case
      if (fscanf(songinput, "%f", &b) != 1) {  // Check for valid beat input
	free(buf);
      	fatal_error("Cannot parse beat");
      }
      
      int temp;
      length = (int)(b * beat);  // Adjust the length
      while (fscanf(songinput, "%d", &temp) == 1 && temp != 999) {  // Loop to adjust the frequency and call render_voice_stereo
      
        freq = (float)(440 * pow(2, (double)((temp - 69.0) / 12.0)));
        render_voice_stereo(&buf[i], length, freq, curamp, curvoice);	
              
      }

      i	+= 2 * length;

      break;

    case 'P':  // Pause Case
      if (fscanf(songinput, "%f", &b) != 1) {  // Check for valid input
	free(buf);
      	fatal_error("Cannot parse beat");
      }
      
      length = (int)(b * beat);  // Adjust the length for the pause
      i += 2 * length;
      
      
      break;

    case 'V':  // Voice Case
      if (fscanf(songinput, "%d", &curvoice) != 1) {  // Check for valid voice input
	free(buf);
	fatal_error("Cannot parse voice");
      } 
      
      break;

    case 'A':  // Amplitude Case
      if (fscanf(songinput, "%f", &curamp) != 1) {  // Check for a valid amplitude input
	free(buf);
	fatal_error("Cannot parse amplitude");
      } 
      break;

    }

    if((cur = fgetc(songinput)) == '\n') {  // Check for new line
      continue;
    }
    else if(cur != EOF) {  // Check for end of file
      free(buf);
      fatal_error("Incorrect song format");
    }
    

  }
  if (ferror(songinput)) {
    fatal_error("Error indicatior was set for the input file");
  }
  
  FILE * waveoutput = fopen(argv[2], "wb");  // Open wave file to write to and do the proper checks
  if (waveoutput == NULL) {
    free(buf);
    fatal_error("Cannot open output file");
  }

  // Call write_wave_header(FILE *out, unsigned num_samples);
  write_wave_header(waveoutput, numsamples);

  // Call write_s16_buf(FILE *out, const int16_t buf[], unsigned n)
  write_s16_buf(waveoutput, buf, (2 * numsamples));

  // Free memory and close files
  fclose(songinput);
  fclose(waveoutput);
  free(buf);
  
  return 0;

}
