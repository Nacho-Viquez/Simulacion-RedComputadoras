#include "Evento.h"
#include "Mensaje.h"



Evento::Evento(){
	
}

Evento::~Evento(){}


//Finalización de procesamiento del mensaje de la computadora 1
long Evento::FC1 (  long reloj, long tiempoEvento, vector<long> eventos){
	long relojEvento = tiempoEvento ; // Revisar -----------------------------------------------------
	srand(time(NULL));
	double num = (rand()%101)/100 ;
	if (mensajes[this->idMensajeP1].tiempoProc4 == 0){
		//El mensaje proviene de la compu 2 
		if (num <= this->X1){
			//El mensaje se va a reenviar a la compu 2 
			long tiempollegada = tiempoEvento + 3;
			mensajes[this->idMensajeP1].tiempoLlegada = tiempollegada;
			mensajes[this->idMensajeP1].vecesDevuelto++;
			colaTransmicion2.push_back(this->idMensajeP1);
			if (eventos[4] == 5000*4){
				//Ponemos que el primero de la cola de transicion es el que debe de llegar
				eventos[4] = mensajes[colaTransmicion2[0]].tiempoLlegada;
				this->arriboCompu2 = colaTransmicion2[0];
				colaTransmicion2.erase(colaTransmicion2.begin());
			}

		}else {
			//Se envia al destino(sale del sistema todo meco)
			mensajes[this->idMensajeP1].estado = 2;
		}
	}else if (mensajes[this->idMensajeP1].tiempoProc2 == 0 && mensajes[this->idMensajeP1].tiempoProc3 == 0){
		//El mensaje proviene de la compu 3
		if (num <= this->X3){
			//Reenvia a la compu 3
			long tiempollegada = tiempoEvento + 3;
			mensajes[this->idMensajeP1].tiempoLlegada = tiempollegada;
			mensajes[this->idMensajeP1].vecesDevuelto++;
			colaTransmicion3.push_back(this->idMensajeP1);
			if (eventos[3] == 5000*4){
				//Ponemos que el primero de la cola de transicion es el que debe de llegar
				eventos[3] = mensajes[colaTransmicion3[0]].tiempoLlegada;
				this->arriboCompu3 = colaTransmicion3[0];
				colaTransmicion3.erase(colaTransmicion3.begin());
			}

		}else {
			//Se envia al destino (sale del sistema todo meco)
			mensajes[this->idMensajeP1].estado = 2;
		}
	}
	//Calculos probabilisticos
	long tiempoRealProc = relojEvento - mensajes[this->idMensajeP1].tiempoInicioTrabajo;
	mensajes[this->idMensajeP1].tiempoRealProc += tiempoRealProc;
	mensajes[this->idMensajeP1].tiempoProc1 += tiempoRealProc;
	
	eventos[0] = 5000*4;//Desprograma evento 1
	
	this->proc1 = false;
	mensajes[this->idMensajeP1].tiempoInicioTrabajo = 0;
	this->idMensajeP1 = 5000*4; // Ahora el proc1 no tiene a nadie como io ;(

	//Revisar la cola(proc) para atender a un mensaje que espera a ser atendido
	if (this->colaProc1.size() != 0){
		//Existen mensajes esperando en la cola de la computadora 1
		int tiempoEnCola = tiempoEvento - mensajes[colaProc1[0]].tiempoEntradaCola;
		mensajes[colaProc1[0]].tiempoEnColas = tiempoEnCola;
		this->proc1 = true; //El proc1 vuelve a estar ocupado
		this->idMensajeP1 = colaProc1[0];
		eventos[0] = relojEvento + D6();
		mensajes[colaProc1[0]].tiempoInicioTrabajo = relojEvento; 
		colaProc1.erase(colaProc1.begin()); // quitamos el consumido
	}


	

	return relojEvento;
}

//Arribo de un mensaje a la computadora 1 del proc 1 de la computadora 2
int Evento::AMC1P1C2(){}

//Arribo de un mensaje a la computadora 1 del proc 2 de la computadora 2
int Evento::AMC1P2C2(){}

//Arribo de un mensaje a la computadora 1 de la computadora 3 
long Evento::AMC1C3(long reloj, long tiempoEvento, vector<long> eventos){
	long relojEvento = tiempoEvento;
	//Se revisa para ver si el proc1 esta ocupado o no
	if (proc1){
		//El proc esta ocupado
		colaProc1.push_back(this->arriboCompu1);
		mensajes[arriboCompu1].tiempoEntradaCola = tiempoEvento;
	}else{
		//El proc1 no esta ocupado
		proc1 = true;
		this->idMensajeP1 = arriboCompu1; // Asignamos el mensaje en que esta siendo atendido por P1
		eventos[0] = tiempoEvento + D6();
		mensajes[idMensajeP1].tiempoInicioTrabajo = relojEvento;

	}
	eventos[1] = 5000*4; // Se desprograma este evento

	//Se revisa la cola de transicion para ver si hay mensajes que vienen de camino y si los hay ponemos ese valor como evento[1]
	if (!colaTransmicion1.empty()){
		eventos[1] = mensajes[colaTransmicion1[0]].tiempoLlegada; // RePrograma el arribo 
		this->arriboCompu1 = colaTransmicion1[0]; // Se asigna id del mensaje para poder trabajar
		colaTransmicion1.erase(colaTransmicion1.begin());
	}


	return relojEvento;
}

//Arribo de un mensaje a la computadora 2 de la computadora 1
int Evento::AMC2C1(){}

//Arribo de un mensaje a la computadora 2 desde afuera
int Evento::AMC2F(){}

//Finalizacion de procesamiento del mensaje en el proc 1 de la computadora 2
int Evento::EMC2P1(){}

//Finalizacion de procesamiento del mensaje en el proc 2 de la computadora 2
int Evento::EMC2P2(){}

//Arribo de mensaje a la computadora 3 de la computadora 1
long Evento::AMC3C1(long reloj, long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento; // Adelantamos el relo
	//Revisar el proc4
	if (proc4){
		//Esta ocupado el proc4 debemos meter el mensaje en la cola de espera 
		colaProc4.push_back(this->arriboCompu3);
		mensajes[this->arriboCompu3].tiempoEntradaCola = relojEvento;

	}else{
		//Si encuentra el proc desocupado lo ponemos a trabajar
		proc4 = true;
		this->idMensajeP4 = arriboCompu3; // guardamos el valor del id del mensaje que esta en el procesador
		eventos[8]= relojEvento + D5(); // Programamos la salida
		mensajes[idMensajeP4].tiempoInicioTrabajo = relojEvento; // Iniciamos una toma del tiempo para el tiempo real y otros 

	}
	eventos[9] = 5000*4;
	//Se revisa la cola de transicion para ver si hay mensajes que vienen de camino y si los hay ponemos ese valor como evento[1]
	if (!colaTransmicion3.empty()){
		eventos[9] = mensajes[colaTransmicion3[0]].tiempoLlegada; // RePrograma el arribo 
		this->arriboCompu3 = colaTransmicion3[0];// Se asigna id del mensaje para poder trabajar
		colaTransmicion3.erase(colaTransmicion3.begin());
	}

	return relojEvento;
}

//Arribo de mensaje a la computadora 3 desde afuera
long Evento::AMC3F(long tiempoEvento, long idMensaje,vector<long> eventos){
	//A este evento se le pasa un idMensaje que no habia estado nunca en el sistema. 
	long relojEvento = tiempoEvento;
	//Revisamos el proc4
	if (proc4){
		//El procesador esta ocupado hemos de poner el mensaje en cola
		colaProc4.push_back(idMensaje);
		mensajes[idMensaje].tiempoEntradaCola = relojEvento;
	}else{
		//Si el proc4 esta libre lo ponemos a trabajar
		proc4 = true;
		this->idMensajeP4 = idMensaje;
		eventos[8] = relojEvento + D5();
		mensajes[idMensajeP4].tiempoInicioTrabajo = relojEvento;
	}
	eventos[10] = relojEvento +D4(); //Reprograma este mismo evento


	return relojEvento;
}

//Finalizacion de procesamiento del mensaje de la computadora 3
int Evento::Emc3(){}

long Evento::D6(){}

long Evento::D5(){}

long Evento::D4(){}