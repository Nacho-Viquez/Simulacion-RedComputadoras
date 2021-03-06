#include <iostream>
#include<stdlib.h>
#include<time.h>
#include <vector>
#include <map>
#include <math.h> 

using namespace std;
class Mensaje;

/*
 Esta clase se engarga de manipular los eventos que se llevan a cabo en la simulacion
 asi como los calculos para la generacion de los valores aleatorios que se usa en 
 ciertos eventos.
*/
class Evento
{
public:
	Evento(double X1, double X2, double X3, map<int, vector<double> > map, long tiempo);
	~Evento();

	void LimpiarSimulacion();

	long FC1 (long tiempoEvento, vector<long> *eventos);

	long AMC1P1C2(long tiempoEvento,vector<long> *eventos);

	long AMC1P2C2(long tiempoEvento,vector<long> *eventos);

	long AMC1C3(long tiempoEvento,vector<long> *eventos);

	long AMC2C1(long tiempoEvento,vector<long> *eventos);

	long AMC2F(long tiempoEvento,vector<long> *eventos);

	long FC2P1(long tiempoEvento,vector<long> *eventos);

	long FC2P2(long tiempoEvento,vector<long> *eventos);

	long AMC3C1(long tiempoEvento,vector<long> *eventos);

	long AMC3F(long tiempoEvento, vector<long> *eventos);

	long FC3(long tiempoEvento,vector<long> *eventos);

	double DistribucionNormalMetodoDirecto(int miu, int varianza );

	double DistribucionNormalMetodoTLC(int miu, int varianza);

	double DistribucionUniforme(int a, int b);

	double DistribucionExponencialParametro(double lambda); 

	double DistribucionDensidad(double k, int a , int b);

	double Manejador(vector<double> v);


	//Mensajes de la simulacion 
	vector<Mensaje> mensajes;
	long idMensajeGlobal;
	long tiempoSimulacion ;

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
	map<int, vector<double> > mapaD;

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

	//Estructuras para estadisticas
	vector<long> tiemposProcesadores;
	long tiempoProc1Perdidos ; 
	long tiempoProc4Perdidos ; 
	long mensajesEliminados ;
	long sumatoriaTiemposMensajes ; 
	long sumatoriaVecesDevuelto;
	long tiempoColas ;
	long tiempoTransmicion;
	long sumatoriaTiempoReal;
};