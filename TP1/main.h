#ifndef MAIN_H
#define MAIN_H

#include <iostream>

#include "complex.h"
#include "config.h"
#include "templatearray.h"
#include "errors.h"

#define MAX_CMD_ARGS 7

#define INSTRUCTIONS_FILE "instrucciones.txt"

status_t validateArguments (int argc, char const *argv[], config_t & config);
void printInstructions ();

#endif

