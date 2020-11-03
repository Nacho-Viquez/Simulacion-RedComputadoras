#include <iostream>
#include<stdlib.h>
#include<time.h>
#include <vector>

using namespace std;
class Mensaje;
class Evento
{
public:
	Evento(double X1, double X2, double X3);
	~Evento();
	//Declaracion de los eventos de la simulacion

	//Finalización de procesamiento del mensaje de la computadora 1
	long FC1 (long tiempoEvento, vector<long> eventos);

	//Arribo de un mensaje a la computadora 1 del proc 1 de la computadora 2
	long AMC1P1C2(long tiempoEvento,vector<long> eventos);

	//Arribo de un mensaje a la computadora 1 del proc 2 de la computadora 2
	long AMC1P2C2(long tiempoEvento,vector<long> eventos);

	//Arribo de un mensaje a la computadora 1 de la computadora 3 
	long AMC1C3(long tiempoEvento,vector<long> eventos);

	//Arribo de un mensaje a la computadora 2 de la computadora 1
	long AMC2C1(long tiempoEvento,vector<long> eventos);

	//Arribo de un mensaje a la computadora 2 desde afuera
	long AMC2F(long tiempoEvento,vector<long> eventos);

	//Finalizacion de procesamiento del mensaje en el proc 1 de la computadora 2
	long EMC2P1(long tiempoEvento,vector<long> eventos);

	//Finalizacion de procesamiento del mensaje en el proc 2 de la computadora 2
	long EMC2P2(long tiempoEvento,vector<long> eventos);

	//Arribo de mensaje a la computadora 3 de la computadora 1
	long AMC3C1(long tiempoEvento,vector<long> eventos);

	//Arribo de mensaje a la computadora 3 desde afuera
	long AMC3F(long tiempoEvento, vector<long> eventos);
	//Finalizacion de procesamiento del mensaje de la computadora 3
	long Emc3(long tiempoEvento,vector<long> eventos);

	long D6();

	long D5();

	long D4();

	long D3();

	long D2();

	long D1();

	//Mensajes de la simulacion 
	vector<Mensaje> mensajes;
	long idMensajeGlobal;

	//IdMensjaes que van a llegar a cierta compu
	long arriboCompu1C3;
	long arriboCompu1P1;
	long arriboCompu1P2;
	long arriboCompu2;
	long arriboCompu3;



	//Atibutos de la simulacion 
	double X1; 
	double X2; 
	double X3; 
	bool proc1; 
	long idMensajeP1;// id del mensaje en el procesador 1 en este momento
	bool proc2;
	long idMensajeP2;// id del mensaje en el procesador 2 en este momento
	bool proc3;
	long idMensajeP3;// id del mensaje en el procesador 3 en este momento
	bool proc4;
	long idMensajeP4;// id del mensaje en el procesador 4 en este momento
	// Colas de transmicion
	vector<long> colaTransmicion1;
	vector<long> colaTransmicion2;
	vector<long> colaTransmicion3;
	//Colas de mensajes en espera
	vector<long> colaProc1;
	vector<long> colaProc2;
	vector<long> colaProc4;
	
};