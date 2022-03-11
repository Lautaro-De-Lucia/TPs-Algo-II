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

int main (int argc, char const * argv[]){

	status_t st;
	config_t config;
	Array <Complex> complexArray;
	Array <Complex> & transformedComplexArray = complexArray;
	ifstream inputFile;	
	ofstream outputFile;

	if ((st = validateArguments (argc, argv, config)) != OK )
	{
		printError(st);
		printInstructions ();
		return st;
	}

	if (config.inputFormat == INPUT_FORMAT_FILE){
		if(!(inputFile.open ((config.inputFile).c_str ()))){
			printError(st);
			return ERROR_INPUT_FILE;
		}
	}

	if (config.outputFormat == OUTPUT_FORMAT_FILE)
		outputFile.open ((config.outputFile).c_str ());

	st = (config.inputFormat == INPUT_FORMAT_CIN ? complexArray.loadArray () : complexArray.loadArray (inputFile));

	if (st) 
	{
		printError(st);
		return st;
	}

	if (config.method == METHOD_DFT)
		transformedComplexArray = dft (complexArray);
	else
		transformedComplexArray = idft (complexArray);

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
	config.method = METHOD_DFT;

	if (argv == NULL)
		return ERROR_NULL_POINTER;

	if (argc > MAX_CMD_ARGS || (argc % 2 == 0))
		return ERROR_PROGRAM_INVOCATION;

	for (pos = 1; pos < argc; pos++)
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
			else
				return ERROR_PROGRAM_INVOCATION;
		}	
		
		else
			return ERROR_PROGRAM_INVOCATION;
			
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
