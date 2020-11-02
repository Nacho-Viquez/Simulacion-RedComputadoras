#include <iostream>
using namespace std;

class Mensaje
{
public:
	Mensaje(long id);
	~Mensaje();
	long getIDMensaje();


	//Atributos no necesarios para estadisticas

//private: 
	//Atributos para los calculos estadisticos
	long idMensaje ; 
	long tiempoProc1 ; //Computadora 1
	long tiempoProc2 ; //Computadora 2 
	long tiempoProc3 ;
	long tiempoProc4 ; //Computadora 3
	int estado ; //0 = rechazado 1 = en sistema 2 = destino
	long vecesDevuelto ; 
	long tiempoRealProc ; 
	long tiempoTransmicion ;
	long tiempoEnColas ; 
	long tiempoLlegada;
	long tiempoInicioTrabajo;
	long tiempoEntradaCola;


};