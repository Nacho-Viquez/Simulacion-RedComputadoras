#include "Mensaje.h"



//Declaracion de los eventos de la simulacion

//Finalización de procesamiento del mensaje de la computadora 1
int FC1 (){}

//Arribo de un mensaje a la computadora 1 del proc 1 de la computadora 2
int AMC1P1C2(){}

//Arribo de un mensaje a la computadora 1 del proc 2 de la computadora 2
int AMC1P2C2(){}

//Arribo de un mensaje a la computadora 1 de la computadora 3 
int AMC1C3(){}

//Arribo de un mensaje a la computadora 2 de la computadora 1
int AMC2C1(){}

//Arribo de un mensaje a la computadora 2 desde afuera
int AMC2F(){}

//Finalizacion de procesamiento del mensaje en el proc 1 de la computadora 2
int EMC2P1(){}

//Finalizacion de procesamiento del mensaje en el proc 2 de la computadora 2
int EMC2P2(){}

//Arribo de mensaje a la computadora 3 de la computadora 1
int AMC3C1(){}

//Arribo de mensaje a la computadora 3 desde afuera
int AMC3F(){}

//Finalizacion de procesamiento del mensaje de la computadora 3
int Emc3(){}

/*
 * main del proyecto 
*/
int main(int argc, char const *argv[])
{
	/* code */
	Mensaje * MensajeParaPera = new Mensaje();
	delete(MensajeParaPera);
	return 0;
}