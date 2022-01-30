// Jack Tarantino - jtarant3
// Weina Dai - wdai11

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"
#include <math.h>

/* 
 * This function takes in an error message and prints
 * it to stderr. The function then quits the program.
 */
void fatal_error(const char *message) {
  fprintf(stderr, "Error: %s\n", message);  // Print the inputed message
  exit(-1);   // Exit the program
}


/*
 * This function takes a character and an open FILE
 * and writes the byte to said file.
 */
void write_byte(FILE *out, char val) {
  if (out == NULL) {    // Check if file was opened properly
    fatal_error("File is NULL");
  }
  else {
    fputc(val, out);      // Write the single byte to the input file
  }
}

/* 
 * This function takes in an array of character values 
 * of size n and utilizes the other write functions
 * to write its values to an open FILE.
 */
void write_bytes(FILE *out, const char data[], unsigned n) {
  if (out == NULL) {   // Check if the file was opened properly
    fatal_error("File is NULL");
  }
  else {
    for (int i = 0; i < (int)n; i++) {   // Loop to continuously write bytes to the file n times
      write_byte(out, data[i]);
    }
  }
}

/* 
 *This function writes a uint16_t value to the open
 * FILE stream in little endian format.
 */
void write_u16(FILE *out, uint16_t value) {
  if (out == NULL) {   // Check if the file was opened properly
    fatal_error("File is NULL");
  }
  else {
    uint16_t var1 = value & 0xFF;   // Least significant byte
    uint16_t var2 = (value >> 8) & 0xFF; // Most significant byte
    fputc(var1, out);  // Write the least significant byte
    fputc(var2, out);  // Write the most significant byte
  }
}

/* 
 * This function writes a uint32_t value to the open
 * FILE stream in little endian format.
 */
void write_u32(FILE *out, uint32_t value) {
  if (out == NULL) {   // Check if the file was opened properly
    fatal_error("File is NULL");
  }
  else {
    for (int i = 0; i < 4; i++) {  // Loop to calculate the least to most significant byte and write them to file
      uint32_t var = ((uint32_t) (value / pow(256, (double)i))) % 256;  
      fputc(var, out);
    }
  }
}

/*
 * This function writes an int16_t value to the 
 * open FILE stream in little endian format.
 */
void write_s16(FILE *out, int16_t value) {
  if (out == NULL) {   // check if the file was opened properly
    fatal_error("File is NULL");
  }
  else {
    int16_t var1 = value & 0xFF;  // Calculate least significant byte
    int16_t var2 = (value >> 8) & 0xFF;  // Calculate most significant byte
    fputc(var1, out);  // Write the least significant byte
    fputc(var2, out);  // Write the most significant byte
  }
}

/* This function makes a series of calls to write_s16
 * to write the arguments of the array buf[] of size n to the 
 * FILE stream.
 */
void write_s16_buf(FILE *out, const int16_t buf[], unsigned n) {
  if (out == NULL) {  // Check if the file was opened properly
    fatal_error("File is NULL");
  }
  else {
    for (unsigned i = 0; i < n; i++) {   // Loop to call write_s16 for each array index
      write_s16(out, buf[i]);
    }
  }
}

/*
 * This function reads in a value from an 
 * open FILE stream.
 */
void read_byte(FILE *in, char *val) {
  char val1;
  val1 = fgetc(in);   // Read in byte

  if (val1 == EOF) {  // Check if value is EOF
    fatal_error("Nothing to be read from file");
  }
  else {
    *val = val1;  // Set pointer equal to the read in value
  }
}

/*
 * This function calls read_byte to store multiple items in
 * an array of size n.
 */
void read_bytes(FILE *in, char data[], unsigned n) {
  for (int i = 0; i < (int)n; i++) {  // Loop to read in a byte from a file n times and store it in the array
    read_byte(in, &data[i]);
  }
}

/*
 * This function reads in a piece of data of type 
 * uint16_t from the FILE stream.
 */
void read_u16(FILE *in, uint16_t *val) {
  int var1;  
  int var2;
  
  var1 = fgetc(in);  // Least significant byte
  var2 = fgetc(in);  // Most significant byte

  if ((var1 == EOF) || (var2 == EOF)) {  // Check to see if either bytes are EOF
    fatal_error("Nothing to be read from file");
  }
  else {
    *val = var1 + (var2 * 256);  // Put bytes together and store into the pointer variable
  }
}

/*
 * This function reads in a value of uint32_t
 * from the open FILE stream and reconstructs it.
 */
void read_u32(FILE *in, uint32_t *val) {
  int var1 = fgetc(in);  // First byte
  int var2 = fgetc(in);  // Second byte
  int var3 = fgetc(in);  // Third byte
  int var4 = fgetc(in);  // Fourth byte

  if ((var1 == EOF) || (var2 == EOF)) {  // Check to see if bytes are EOF
    fatal_error("Nothing to be read from file");
  }
  else if ((var3 == EOF) || (var4 == EOF)) {
    fatal_error("Nothing to be read from file");
  }
  else {
    *val = var1 + (var2 * 256) + (var3 * pow(256, 2)) + (var4 * pow(256, 3));  // Put the bytes together and store into pointer variable
  }
}

/*
 * This function reads in a piece of data of type
 * int16_t from the FILE stream.
 */
void read_s16(FILE *in, int16_t *val) {
  int var1;
  int var2;

  var1 = fgetc(in);  // First byte
  var2 = fgetc(in);  // Second byte

  if ((var1 == EOF) || (var2 == EOF)) {  // Check if bytes are EOF
    fatal_error("Nothing to be read from file");
  }
  else {
    *val = var1 + (var2 * 256);  // Put the bytes together and store into pointer variable
  }
}

/* 
 * This function makes calls to read_s16 in order to fill the 
 * array buf[] of size n with the read in data values.
 */
void read_s16_buf(FILE *in, int16_t buf[], unsigned n) {
  for (int i = 0; i < (int)n; i++) {  // Read bytes and input them into each array index
    read_s16(in, &buf[i]);
  }
}
