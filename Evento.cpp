#include "Evento.h"
#include "Mensaje.h"



Evento::Evento(double X1, double X2, double X3){
	this->idMensajeGlobal = 0;
	this->arriboCompu1C3  = 0;
	this->arriboCompu1P1  = 0;
	this->arriboCompu1P2  = 0; 
	this-> arriboCompu2 = 0;
	this->arriboCompu3 = 0;
	this->X1 = X1; 
	this->X2 = X2; 
	this->X3 = X3;

	this->proc1 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP1 = -1; 

	this->proc2 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP2 = -1; 

	this->proc3 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP3 = -1; 

	this->proc4 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP4 = -1; 

	//Inicializacion para las probas
}

Evento::~Evento(){}


//Finalización de procesamiento del mensaje de la computadora 1
long Evento::FC1 ( long tiempoEvento, vector<long> eventos){
	long relojEvento = tiempoEvento ; // Revisar -----------------------------------------------------
	srand(time(NULL));
	double num = (rand()%101)/100 ;
	if (mensajes[this->idMensajeP1].tiempoProc4 == 0){
		//El mensaje proviene de la compu 2 
		if (num <= this->X1){
			//El mensaje se va a reenviar a la compu 2 
			long tiempollegada = tiempoEvento + 3;
			mensajes[this->idMensajeP1].tiempoTransmicion += 3;
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
			mensajes[this->idMensajeP1].tiempoTransmicion += 3;
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
	this->idMensajeP1 =-1; // Ahora el proc1 no tiene a nadie como io ;(

	//Revisar la cola(proc) para atender a un mensaje que espera a ser atendido
	if (this->colaProc1.size() != 0){
		//Existen mensajes esperando en la cola de la computadora 1
		int tiempoEnCola = tiempoEvento - mensajes[colaProc1[0]].tiempoEntradaCola;
		mensajes[colaProc1[0]].tiempoEnColas += tiempoEnCola;
		this->proc1 = true; //El proc1 vuelve a estar ocupado
		this->idMensajeP1 = colaProc1[0];
		eventos[0] = relojEvento + D6();
		mensajes[colaProc1[0]].tiempoInicioTrabajo = relojEvento; 
		colaProc1.erase(colaProc1.begin()); // quitamos el consumido
	}


	

	return relojEvento;
}

//Arribo de un mensaje a la computadora 1 del proc 1 de la computadora 2
long Evento::AMC1P1C2(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	//Revisamos el proc1 a ver su estado
	if(proc1){
		//El proc1 esta ocupado por lo que el mensaje que llega lo metemos en la cola
		colaProc1.push_back(this->arriboCompu1P1); // añadimos el id a la cola 
		mensajes[this->arriboCompu1P1].tiempoEntradaCola = relojEvento; 

	}else{
		//El proc1 estaba disponible 
		proc1 = true;
		this->idMensajeP1 = arriboCompu1P1;
		eventos[0] = relojEvento + D6(); // Programamos el fin de trabajo
		mensajes[idMensajeP1].tiempoInicioTrabajo = relojEvento;
	}
	eventos[2] = 5000*4;// Desprograma el evento

	//Debemos revisar la cola de transicion para poder asignar el proximo arribo
	if (!colaTransmicion1.empty()){
		this->arriboCompu1P1 = colaTransmicion1[0]; // asignamos cual sera el proximo en llegar
		eventos[2] = mensajes[colaTransmicion1[0]].tiempoLlegada; // programamos la llegada a este evento por parte de un mensajeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		colaTransmicion1.erase(colaTransmicion1.begin());
	}
	return relojEvento;
}

//Arribo de un mensaje a la computadora 1 del proc 2 de la computadora 2
long Evento::AMC1P2C2(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	//Revisamos el proc1 a ver su estado
	if(proc1){
		//El proc1 esta ocupado por lo que el mensaje que llega lo metemos en la cola
		colaProc1.push_back(this->arriboCompu1P2); // añadimos el id a la cola 
		mensajes[this->arriboCompu1P2].tiempoEntradaCola = relojEvento; 

	}else{
		//El proc1 estaba disponible 
		proc1 = true;
		this->idMensajeP1 = arriboCompu1P2;
		eventos[0] = relojEvento + D6(); // Programamos el fin de trabajo
		mensajes[idMensajeP1].tiempoInicioTrabajo = relojEvento;
	}
	eventos[2] = 5000*4;// Desprograma el evento

	//Debemos revisar la cola de transicion para poder asignar el proximo arribo
	if (!colaTransmicion1.empty()){
		this->arriboCompu1P2 = colaTransmicion1[0]; // asignamos cual sera el proximo en llegar
		eventos[2] = mensajes[colaTransmicion1[0]].tiempoLlegada; // programamos la llegada a este evento por parte de un mensajeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		colaTransmicion1.erase(colaTransmicion1.begin());
	}
	return relojEvento;

}

//Arribo de un mensaje a la computadora 1 de la computadora 3 
long Evento::AMC1C3( long tiempoEvento, vector<long> eventos){
	long relojEvento = tiempoEvento;
	//Se revisa para ver si el proc1 esta ocupado o no
	if (proc1){
		//El proc esta ocupado
		colaProc1.push_back(this->arriboCompu1C3);
		mensajes[arriboCompu1C3].tiempoEntradaCola = relojEvento;
	}else{
		//El proc1 no esta ocupado
		proc1 = true;
		this->idMensajeP1 = arriboCompu1C3; // Asignamos el mensaje en que esta siendo atendido por P1
		eventos[0] = tiempoEvento + D6();
		mensajes[idMensajeP1].tiempoInicioTrabajo = relojEvento;

	}
	eventos[1] = 5000*4; // Se desprograma este evento

	//Se revisa la cola de transicion para ver si hay mensajes que vienen de camino y si los hay ponemos ese valor como evento[1]
	if (!colaTransmicion1.empty()){
		eventos[1] = mensajes[colaTransmicion1[0]].tiempoLlegada; // RePrograma el arribo 
		this->arriboCompu1C3 = colaTransmicion1[0]; // Se asigna id del mensaje para poder trabajar
		colaTransmicion1.erase(colaTransmicion1.begin());
	}


	return relojEvento;
}

//Arribo de un mensaje a la computadora 2 de la computadora 1
long Evento::AMC2C1(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	//revisamos los proc2 y proc3
	if (proc2 && proc3){
		//Ambos proc estan ocupados entonces el mensaje ah de ir a la cola
		colaProc2.push_back(this->arriboCompu2); 
		mensajes[this->arriboCompu2].tiempoEntradaCola = relojEvento;

	}else if (proc2 && !proc3){
		//El proc3 esta desocupado el mensaje se puede meter ahi 
		proc3 = true; // El proc3 ahora esta ocupado
		this->idMensajeP3 = this->arriboCompu2; // almacenamos el idMensaje
		eventos[7] = relojEvento + D3(); // Alistamos el evento de salida
		mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento; 
	}else if (proc3 && !proc2){
		//El proc2 esta desocupado el mensaje se puede meter ahi 
		proc2 = true; 
		this->idMensajeP2 = this->arriboCompu2; // asignamos el mensaje
		eventos[6] = relojEvento + D2();
		mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento;
	}else {
		//Si ambos estan desocupados se debe decidir a cual de los dos va a entrar
		srand(time(NULL));
		double num = (rand()%101)/100 ;
		if (num <= 0.5 ){
			//Lo ponemos en el proc2
			proc2 = true; 
			this->idMensajeP2 = this->arriboCompu2; // asignamos el mensaje
			eventos[6] = relojEvento + D2();
			mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento;
		}else{
			//lo ponemos en el proc3
			proc3 = true; // El proc3 ahora esta ocupado
			this->idMensajeP3 = this->arriboCompu2; // almacenamos el idMensaje
			eventos[7] = relojEvento + D3(); // Alistamos el evento de salida
			mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento; 
		}
	}
	eventos[4] = 5000*4; // Desprogramamos el arribo

	//Revisamos la cola de transmicion para "reprogramar el arribo desde la comp1"
	if (colaTransmicion2.size() != 0){
		// La cola no esta vacia o sea podemos reprogramar un arribo
		eventos[4] = mensajes[colaTransmicion2[0]].tiempoLlegada; // Reprogramamos el arribo
		this->arriboCompu2 = colaTransmicion2[0];
		colaTransmicion2.erase(colaTransmicion2.begin()); // elimnamos de la cola de transmicion
	}
	return relojEvento;
}

//Arribo de un mensaje a la computadora 2 desde afuera
long Evento::AMC2F(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	//Se revisan los procesadores a ver si alguno esta desocupado
	Mensaje mensaje(this->idMensajeGlobal);
	long idMensaje = idMensajeGlobal;
	this->idMensajeGlobal++; // Se creo un mensaje nuevo
	this->mensajes.push_back(mensaje);

	if (proc2 && proc3){
		//Ambos proc estan ocupados entonces el mensaje ah de ir a la cola
		colaProc2.push_back(idMensaje); 
		mensajes[idMensaje].tiempoEntradaCola = relojEvento;

	}else if (proc2 && !proc3){
		//El proc3 esta desocupado el mensaje se puede meter ahi 
		proc3 = true; // El proc3 ahora esta ocupado
		this->idMensajeP3 = idMensaje; // almacenamos el idMensaje
		eventos[7] = relojEvento + D3(); // Alistamos el evento de salida
		mensajes[idMensaje].tiempoInicioTrabajo = relojEvento; 
	}else if (proc3 && !proc2){
		//El proc2 esta desocupado el mensaje se puede meter ahi 
		proc2 = true; 
		this->idMensajeP2 = idMensaje; // asignamos el mensaje
		eventos[6] = relojEvento + D2();
		mensajes[idMensaje].tiempoInicioTrabajo = relojEvento;
	}else {
		//Si ambos estan desocupados se debe decidir a cual de los dos va a entrar
		srand(time(NULL));
		double num = (rand()%101)/100 ;
		if (num <= 0.5 ){
			//Lo ponemos en el proc2
			proc2 = true; 
			this->idMensajeP2 = idMensaje; // asignamos el mensaje
			eventos[6] = relojEvento + D2();
			mensajes[idMensaje].tiempoInicioTrabajo = relojEvento;
		}else{
			//lo ponemos en el proc3
			proc3 = true; // El proc3 ahora esta ocupado
			this->idMensajeP3 = idMensaje; // almacenamos el idMensaje
			eventos[7] = relojEvento + D3(); // Alistamos el evento de salida
			mensajes[idMensaje].tiempoInicioTrabajo = relojEvento; 
		}
	}
	eventos[5] = relojEvento + D1();
	return relojEvento;
}

//Finalizacion de procesamiento del mensaje en el proc 1 de la computadora 2
long Evento::EMC2P1(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	//Se envia un mensaje a la computadora 1
	mensajes[idMensajeP2].tiempoTransmicion += 20; // estadisico
	colaTransmicion1.push_back(idMensajeP2);
	mensajes[idMensajeP2].tiempoLlegada = relojEvento +20; // Importanteeeee
	
	//Comprobamos si ya se programo el arribo a la computadora 1
	if (eventos[2] == 5000*4) {
		//Hemos de progrmar el siguiete arribo
		eventos[2] = mensajes[colaTransmicion1[0]].tiempoLlegada; // Programamos
		this->arriboCompu1P1 = colaTransmicion1[0];
		colaTransmicion1.erase(colaTransmicion1.begin());
	}

	//Calculos estadisticos
	long tiempoRealProc = relojEvento - mensajes[idMensajeP2].tiempoInicioTrabajo;
	mensajes[idMensajeP2].tiempoRealProc += tiempoRealProc;
	mensajes[idMensajeP2].tiempoProc2 += tiempoRealProc;

	//Desprogramos valores de la simulacion
	eventos[6] = 5000*4; // Desprogrmamos el evento
	proc2 = false; // ya no esta ocupado 
	mensajes[idMensajeP2].tiempoInicioTrabajo = 0; // por si acaso
	this->idMensajeP2 = -1; // quitamos el mensaje actual

	//Revisamos la cola de la compu2 a ver si hay algun mensaje en espera
	if(colaProc2.size() != 0){
		//existen mensajes esperando a ser atendidos
		long tiempoEnCola = relojEvento - mensajes[colaProc2[0]].tiempoEntradaCola;
		mensajes[colaProc2[0]].tiempoEnColas += tiempoEnCola;


		proc2 = true; // Se ocupa el proc2
		this->idMensajeP2 = colaProc2[0];
		eventos[6] = relojEvento +  D2();
		mensajes[idMensajeP2].tiempoInicioTrabajo = relojEvento;
		colaProc2.erase(colaProc2.begin()); 

	}

	return relojEvento;
}

//Finalizacion de procesamiento del mensaje en el proc 2 de la computadora 2
long Evento::EMC2P2(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	//Se envia un mensaje a la computadora 1
	mensajes[idMensajeP3].tiempoTransmicion += 20; // estadisico
	colaTransmicion1.push_back(idMensajeP3);
	mensajes[idMensajeP3].tiempoLlegada = relojEvento +20; // Importanteeeee
	
	//Comprobamos si ya se programo el arribo a la computadora 1
	if (eventos[3] == 5000*4) {
		//Hemos de progrmar el siguiete arribo
		eventos[3] = mensajes[colaTransmicion1[0]].tiempoLlegada; // Programamos
		this->arriboCompu1P2 = colaTransmicion1[0];
		colaTransmicion1.erase(colaTransmicion1.begin());
	}

	//Calculos estadisticos
	long tiempoRealProc = relojEvento - mensajes[idMensajeP3].tiempoInicioTrabajo;
	mensajes[idMensajeP3].tiempoRealProc += tiempoRealProc;
	mensajes[idMensajeP3].tiempoProc3 += tiempoRealProc;

	//Desprogramos valores de la simulacion
	eventos[7] = 5000*4; // Desprogrmamos el evento
	proc3 = false; // ya no esta ocupado 
	mensajes[idMensajeP3].tiempoInicioTrabajo = 0; // por si acaso
	this->idMensajeP3 = -1; // quitamos el mensaje actual

	//Revisamos la cola de la compu2 a ver si hay algun mensaje en espera
	if(colaProc2.size() != 0){
		//existen mensajes esperando a ser atendidos
		long tiempoEnCola = relojEvento - mensajes[colaProc2[0]].tiempoEntradaCola;
		mensajes[colaProc2[0]].tiempoEnColas += tiempoEnCola;


		proc3 = true; // Se ocupa el proc2
		this->idMensajeP3 = colaProc2[0];
		eventos[7] = relojEvento +  D2();
		mensajes[idMensajeP3].tiempoInicioTrabajo = relojEvento;
		colaProc2.erase(colaProc2.begin()); 

	}

	return relojEvento;

}

//Arribo de mensaje a la computadora 3 de la computadora 1
long Evento::AMC3C1( long tiempoEvento,vector<long> eventos){
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
long Evento::AMC3F(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	Mensaje mensaje(this->idMensajeGlobal);
	long idMensaje = idMensajeGlobal;
	this->idMensajeGlobal++; // Se creo un mensaje nuevo
	this->mensajes.push_back(mensaje);
	
	//Revisamos el proc4
	if (proc4){
		//El procesador esta ocupado hemos de poner el mensaje en cola
		colaProc4.push_back(idMensaje);
		mensajes[idMensaje].tiempoEntradaCola = relojEvento;
	}else{
		//Si el proc4 esta libre lo ponemos a trabajar
		printf("Entre en donde es pa\n");
		proc4 = true;
		this->idMensajeP4 = idMensaje;
		eventos[8] = relojEvento + D5();
		mensajes[idMensajeP4].tiempoInicioTrabajo = relojEvento;
	}
	eventos[10] = relojEvento +D4(); //Reprograma este mismo evento

	cout <<"Cantidad de mensajes en el vector"<<this->mensajes.size()<<endl;
	cout <<"El tiempo en el que volvere a pasar es : "<< eventos[10] <<endl;

	return relojEvento;
}

//Finalizacion de procesamiento del mensaje de la computadora 3
long Evento::Emc3(long tiempoEvento,vector<long> eventos){
	long relojEvento = tiempoEvento;
	srand(time(NULL));
	double num = (rand()%101)/100 ;
	if(num <= this->X2){
		//Se elimina el mensaje
		mensajes[idMensajeP4].estado = 0;

	}else{
		//Se envia a el mensaje a la computadora 1
		mensajes[idMensajeP4].tiempoTransmicion += 20; 
		mensajes[idMensajeP4].tiempoLlegada = relojEvento +20; // a esta hora va a llegar
		colaTransmicion1.push_back(idMensajeP4); //Ponemos el mensaje en la cola de envios "canal"
		if (eventos[1] == 5000*4 ){
			//Se debe asignar el tiempo del primer elemento de la cola de transicion1 a este evento
			eventos[1] = mensajes[colaTransmicion1[0]].tiempoLlegada;
			this->arriboCompu1C3 = colaTransmicion1[0]; // el mensaje que va a llegar sera el que este de primero
			colaTransmicion1.erase(colaTransmicion1.begin());
		}

	}

	//Calculos estadisticos
	long tiempoRealProc = relojEvento - mensajes[idMensajeP4].tiempoInicioTrabajo;
	mensajes[idMensajeP4].tiempoRealProc += tiempoRealProc;
	mensajes[idMensajeP4].tiempoProc4 += tiempoRealProc;

	//Desprogramamos valores de la simulacion
	eventos[8] = 5000*4;
	this->proc4 = false ; //el proc4 ya no esta ocupado
	mensajes[idMensajeP4].tiempoInicioTrabajo = 0; 
	this->idMensajeP4 = -1; // Se desprogama el id

	// Revision de la cola de mensajes en espera por ser atendidos a ver si podemos sacar a uno
	if (colaProc4.size() != 0){
		//Existen mensajes esperando a ser atendidos por el prov4
		long tiempoEnCola = relojEvento - mensajes[colaProc4[0]].tiempoEntradaCola;
		mensajes[colaProc4[0]].tiempoEnColas += tiempoEnCola;

		//Programamos valores utiles para el funcionamiento de la simulacion
		proc4 = true; // el proc4 esta ocupado
		this->idMensajeP4 = colaProc4[0]; 
		eventos[8] = relojEvento + D5();
		mensajes[this->idMensajeP4].tiempoInicioTrabajo = relojEvento;
		colaProc4.erase(colaProc4.begin());
	}


	return relojEvento;
}

long Evento::D6(){}

long Evento::D5(){}

long Evento::D4(){


	return 200;
}

long Evento::D3(){}

long Evento::D2(){}

long Evento::D1(){}