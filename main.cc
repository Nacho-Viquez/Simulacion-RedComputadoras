#include "Mensaje.h"
#include "Evento.h"

long manejadorEventos(long reloj,vector<long> eventos, long indiceEventoProximo, Evento *evento){
	long relojActual = reloj;
	switch(indiceEventoProximo){
		//Computadora 1
		case 0: //Finalización de procesamiento del mensaje de la computadora 1
			relojActual = evento->FC1(relojActual, eventos);
			break;
		case 1: //Arribo de un mensaje a la computadora 1 de la computadora 3
			relojActual = evento->AMC1C3(relojActual,eventos);
			break;
		case 2://Arribo de un mensaje a la computadora 1 del proc 1 de la computadora 2
			relojActual = evento->AMC1P1C2(relojActual,eventos);
			break;
		case 3://Arribo de un mensaje a la computadora 1 del proc 2 de la computadora 2
			relojActual = evento->AMC1P2C2(relojActual,eventos);
			break;
		//Computadora 2
		case 4: //Arribo de un mensaje a la computadora 2 de la computadora 1
			relojActual = evento->AMC2C1(relojActual,eventos);
			break;
		case 5: //Arribo de un mensaje a la computadora 2 desde afuera
			relojActual = evento->AMC2F(relojActual,eventos);
			break;
		case 6: //Finalizacion de procesamiento del mensaje en el proc 1 de la computadora 2
			relojActual = evento->EMC2P1(relojActual,eventos);
			break;
		case 7: //Finalizacion de procesamiento del mensaje en el proc 2 de la computadora 2
			relojActual = evento->EMC2P2(relojActual,eventos);
			break;
		case 8: //Finalizacion de procesamiento del mensaje de la computadora 3
			relojActual = evento->Emc3(relojActual,eventos);
			break;
		case 9: //Arribo de mensaje a la computadora 3 de la computadora 1
			relojActual = evento->AMC3C1(relojActual,eventos);
			break;
		case 10: //Arribo de mensaje a la computadora 3 desde afuera
			relojActual = evento->AMC3F(relojActual,eventos);
			break;
		default:
			printf("default\n");
	}
	return relojActual;
}


/*
 * main del proyecto 
*/
int main(int argc, char const *argv[])
{
	
	//Pedir datos al usuario
	double X1 = 0.23;
	double X2 = 0.45;
	double X3= 0.78;


	//Inicializacion de instancias de clases
	Evento * evento = new Evento(X1,X2,X3);


	//Variables "globales" simulacion
	long reloj = 0;
	long tiempoDuracionSimulacion = 5000;
	vector<long> eventos; //Vector para los tiempos de ejecucion de los eventos
	eventos.resize(11, 5000*4);	 //Inicializamos todos los eventos en inf

	//Valores iniciales a los eventos inciales
	eventos[9] =  0;
	eventos[10] = 0;

	
	//Ciclo principal de la simulacion

	while(reloj<= tiempoDuracionSimulacion ){
		//Comprobacion de tiempo para siguiente accion
		long eventoProximo = 5000*4; 
		int indiceEventoProximo = 5000*4;
		for (int i = 0; i < eventos.size(); ++i)
		{
			if ( eventos[i] < eventoProximo ){
				eventoProximo = eventos[i];
				indiceEventoProximo = i;
			}
		}
		cout<< "Hola este es el siguiente evento en pasar :"<< indiceEventoProximo<<" que tiene un valor de : "<<eventoProximo <<endl;

		//Se realiza el siguiente evento indicado en indiceEventoProximo
		reloj = manejadorEventos(reloj, eventos, indiceEventoProximo, evento);


	}
	printf("Sali de esta picjaaaaaaaaaaaaaaaaaaaaaaa\n");
	//Calculos finaless

	delete(evento);
	return 0;
}