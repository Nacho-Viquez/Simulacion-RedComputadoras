#include <iostream>
using namespace std;

/* Esta clase simula el mensaje que se pasara entre los procesadores del sistema. Ademas contiene
	los atributos necesarios para calcular las estadisticas. 
*/

class Mensaje
{
public:
	Mensaje(long id);
	~Mensaje();
	long getIDMensaje();

	//Atributos para los calculos estadisticos
	long idMensaje ; 
	long tiempoProc1 ; //Computadora 1
	long tiempoProc2 ; //Computadora 2 
	long tiempoProc3 ;
	long tiempoProc4 ; //Computadora 3
	int estado ; //0 = eliminado 1 = en sistema 2 = destino
	long vecesDevuelto ; 
	long tiempoRealProc ; 
	long tiempoTransmicion ;
	long tiempoEnColas ; 
	long tiempoLlegada;
	long tiempoInicioTrabajo;
	long tiempoEntradaCola;
	


};