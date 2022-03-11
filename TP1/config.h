#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <cstring>

using namespace std;

typedef enum {

	OK,
	ERROR_NULL_POINTER,
	ERROR_INPUT_FILE,
	ERROR_PROGRAM_INVOCATION,
	ERROR_PROGRAM_INVOCATION_MODE,
	ERROR_PROGRAM_INVOCATION_FLAGS,
	ERROR_MEMORY,
	ERROR_NO_SAMPLES,
	ERROR_VALUE_PROCESS_RESUMED,
	ERROR_VALUE_PROCESS_ABORTED,
	ERROR_OUT_OF_RANGE,
	ERROR_EXCEPTION

} status_t;

typedef enum {

	INPUT_FORMAT_CIN,
	INPUT_FORMAT_FILE,
	OUTPUT_FORMAT_COUT,
	OUTPUT_FORMAT_FILE	

} io_format_t;

typedef enum {

	METHOD_DFT,
	METHOD_IDFT,
	METHOD_FFT,
	METHOD_IFFT

} method_t; 

typedef struct {

	io_format_t inputFormat;
	io_format_t outputFormat;
	method_t method;
	string inputFile;
	string outputFile;

} config_t;

#endif

