#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "main.h"
#include "complex.h"
#include "config.h"
#include "templatearray.h"
#include "errors.h"

using namespace std;

int main (int argc, char const * argv[])
{
	status_t st;
	config_t config;
	Array <Complex> complexArray;
	Array <Complex> transformedComplexArray;
	ifstream inputFile;	
	ofstream outputFile;

	if ((st = validateArguments (argc, argv, config)) != OK )
	{
		printError(st);
		printInstructions ();
		return st;
	}

	if (config.inputFormat == INPUT_FORMAT_FILE)
		inputFile.open ((config.inputFile).c_str ());

	if(!inputFile){
		printError(ERROR_INPUT_FILE);
		return ERROR_INPUT_FILE;
	}	

	if (config.outputFormat == OUTPUT_FORMAT_FILE)
		outputFile.open ((config.outputFile).c_str ());

	st = (config.inputFormat == INPUT_FORMAT_CIN ? complexArray.loadArray () : complexArray.loadArray (inputFile));

	if (st)
	{
		printError(st);
		return st;
	}

	switch (config.method){
		case METHOD_DFT:
			transformedComplexArray = dft (complexArray);
			break;
		case METHOD_IDFT:
			transformedComplexArray = idft (complexArray);
			break;
		case METHOD_FFT:
			transformedComplexArray = fft (complexArray);
			break;
		case METHOD_IFFT:
			transformedComplexArray = ifft (complexArray);
			break;
	}

	config.outputFormat == OUTPUT_FORMAT_COUT ? cout << transformedComplexArray : outputFile << transformedComplexArray;
	
	if (config.inputFormat == INPUT_FORMAT_FILE)
		inputFile.close ();

	if (config.outputFormat == OUTPUT_FORMAT_FILE)
		outputFile.close ();

	return OK;

}

status_t validateArguments (int argc, char const *argv[], config_t & config)
{
	size_t pos;

	config.inputFormat = INPUT_FORMAT_CIN;
	config.outputFormat = OUTPUT_FORMAT_COUT;
	config.method = METHOD_FFT;

	if (argv == NULL)
		return ERROR_NULL_POINTER;

	if (argc > MAX_CMD_ARGS || (argc % 2 == 0))
		return ERROR_PROGRAM_INVOCATION;

	for (pos = 1; pos < (size_t) argc; pos++)
	{
		if (!strcmp (argv[pos], "-i"))
		{
			if (!strcmp (argv[++pos], "-"))
				config.inputFormat = INPUT_FORMAT_CIN;
			else
			{ 
				config.inputFormat = INPUT_FORMAT_FILE;
				config.inputFile = argv[pos];
			}
		}	

		else if (!strcmp (argv[pos], "-o"))
		{
			if (!strcmp (argv[++pos], "-"))
				config.outputFormat = OUTPUT_FORMAT_COUT;			
			else
			{ 
				config.outputFormat = OUTPUT_FORMAT_FILE;
				config.outputFile = argv[pos];
			}		
		}

		else if (!strcmp (argv[pos], "-m"))
		{
			if (!strcmp (argv[++pos], "dft"))
				config.method = METHOD_DFT;			
			else if (!strcmp (argv[pos],"idft"))
				config.method = METHOD_IDFT;
			else if (!strcmp (argv[pos], "fft"))
				config.method = METHOD_FFT;			
			else if (!strcmp (argv[pos],"ifft"))
				config.method = METHOD_IFFT;				
			else
				return ERROR_PROGRAM_INVOCATION_MODE;
		}	
		
		else
			return ERROR_PROGRAM_INVOCATION_FLAGS;
			
	}

	return OK;

}

void printInstructions ()
{
	ifstream ifs (INSTRUCTIONS_FILE);
	string s;
	getline (ifs, s, (char) ifs.eof ());
	cout << s << endl;	
}

