#include <iostream>
#include "errors.h"
#include "config.h"

static string error_dictionary [] = {

	"Ok",
	"Error: puntero nulo",
	"Error: invocación inválida",
	"Error: memoria insuficiente",
	"Error: no hay muestras",
	"Error: se detectó un valor erroneo, no se proceso",
	"Error: se detectó un valor erroneo, proceso abortado",
	"Error: subíndice fuera de rango",
	"Ocurrió una excepción: "

};

void printError (status_t error)
{
	cerr << error_dictionary[error] << endl;
} 