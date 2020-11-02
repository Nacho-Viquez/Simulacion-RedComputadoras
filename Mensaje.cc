#include "Mensaje.h"


Mensaje::Mensaje(long id){
	this->idMensaje = id;
	this->tiempoProc1 = 0;
	this->tiempoProc2 = 0;
	this->tiempoProc3 = 0;
	this->tiempoProc4 = 0;
	this->estado = 0; //0 = rechazado 1 = en sistema 2 = destino
	this->vecesDevuelto  = 0; 
	this->tiempoRealProc = 0 ; 
	this->tiempoTransmicion  = 0;
	this->tiempoEnColas = 0 ;
}

Mensaje::~Mensaje(){}