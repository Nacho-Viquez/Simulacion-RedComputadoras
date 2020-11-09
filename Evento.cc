#include "Evento.h"
#include "Mensaje.h"

#define PI 3.14159265

Evento::Evento(double X1, double X2, double X3, map<int, vector<double> > mapaDistros, long tiempo){
	this->tiempoSimulacion = tiempo;
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
	this->mapaD = mapaDistros;

	//Estadisticas
	this->tiemposProcesadores.resize(4,0);
	this->tiempoProc1Perdidos = 0; 
	this->tiempoProc4Perdidos = 0;
	this->mensajesEliminados = 0;
	this->sumatoriaTiemposMensajes = 0;
	this->sumatoriaVecesDevuelto = 0;
	this->tiempoColas = 0;
	this->tiempoTransmicion = 0;
	this->sumatoriaTiempoReal = 0;

	srand(time(0));
}

Evento::~Evento(){}


/*@Funcion: limpia todas las variables utilizadas en la simulacion para cada vez que se corre y no 
			queden variables con contenido de corridas anteriores que afecten los calculos
*/
void Evento::LimpiarSimulacion(){
	this->idMensajeGlobal = 0;
	this->arriboCompu1C3  = 0;
	this->arriboCompu1P1  = 0;
	this->arriboCompu1P2  = 0; 
	this-> arriboCompu2 = 0;
	this->arriboCompu3 = 0;

	this->proc1 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP1 = -1; 

	this->proc2 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP2 = -1; 

	this->proc3 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP3 = -1; 

	this->proc4 = false; //El proc1 esta desocupado al inicio 
	this->idMensajeP4 = -1; 

	//Estadisticas
	//this->tiemposProcesadores.resize(4,0);
	for (int i = 0; i < tiemposProcesadores.size(); ++i)
	{	
		tiemposProcesadores[i] = 0;	
	}
	this->tiempoProc1Perdidos = 0; 
	this->tiempoProc4Perdidos = 0;
	this->mensajesEliminados = 0;
	this->sumatoriaTiemposMensajes = 0;
	this->sumatoriaVecesDevuelto = 0;
	this->tiempoColas = 0;
	this->tiempoTransmicion = 0;
	this->sumatoriaTiempoReal = 0;
	this->mensajes.clear();
	// Colas de transmicion
	this->colaTransmicion1.clear();
	this->colaTransmicion2.clear();
	this->colaTransmicion3.clear();
	//Colas de mensajes en espera
	this->colaProc1.clear();
	this->colaProc2.clear();
	this->colaProc4.clear();
}


/* @Funcion: simula el evento "Finalización de procesamiento del mensaje de la computadora 1"
			donde se toma en cuenta cuando se tiene que regresar tanto a la computadora 2 como 
			a la 3 o si se tiene que devolver a su destino. Ademas revisa si hay algún mensaje esperando
			en su cola para atenderlo.
	@Param tiempoEvento: reloj principal del sistema
	@Param eventos: es un vector donde se van modificando los tiempos en los que pasa un determinado 
					evento.
	@Return: retorna la hora que tomara el reloj principal de la simulación
*/
long Evento::FC1 ( long tiempoEvento, vector<long> *eventos){
	long relojEvento = tiempoEvento ; 
	//srand(time(NULL));
	//Calculos probabilisticos
	long tiempoRealProc = relojEvento - mensajes[this->idMensajeP1].tiempoInicioTrabajo;
	mensajes[this->idMensajeP1].tiempoRealProc += tiempoRealProc;
	mensajes[this->idMensajeP1].tiempoProc1 += tiempoRealProc;
	//cout<<"------------- soy el mensaje: "<<this->idMensajeP1<<"-------------------" <<endl;

	double num = drand48();
	if (mensajes[this->idMensajeP1].tiempoProc4 == 0){
		//printf("Me llego un mensaje de la cpmputadora 2\n");
		//cout<<"Tiempo  proc4: "<< mensajes[this->idMensajeP1].tiempoProc4<<"Tiempo  proc2: "<< mensajes[this->idMensajeP1].tiempoProc2 <<" Tiempo  proc3: "<< mensajes[this->idMensajeP1].tiempoProc3 <<endl;
		//El mensaje proviene de la compu 2 
		if (num <= this->X1){
			//El mensaje se va a reenviar a la compu 2 
			long tiempollegada = tiempoEvento + 3;
			mensajes[this->idMensajeP1].tiempoTransmicion += 3;
			mensajes[this->idMensajeP1].tiempoLlegada = tiempollegada;
			mensajes[this->idMensajeP1].vecesDevuelto++;
			colaTransmicion2.push_back(this->idMensajeP1);
			if (eventos->at(4) == this->tiempoSimulacion*4){
				//Ponemos que el primero de la cola de transicion es el que debe de llegar
				eventos->at(4) = mensajes[colaTransmicion2[0]].tiempoLlegada;
				this->arriboCompu2 = colaTransmicion2[0];
				colaTransmicion2.erase(colaTransmicion2.begin());
			}

		}else {
			//Se envia al destino(sale del sistema todo meco)
			mensajes[this->idMensajeP1].estado = 2;
			this->tiemposProcesadores[0] += mensajes[this->idMensajeP1].tiempoProc1; // Almacena tiempo en proc1
			this->tiemposProcesadores[1] += mensajes[this->idMensajeP1].tiempoProc2; // Almacena tiempo en proc2
			this->tiemposProcesadores[2] += mensajes[this->idMensajeP1].tiempoProc3; // Almacena tiempo en proc3
			this->sumatoriaTiemposMensajes += (mensajes[this->idMensajeP1].tiempoTransmicion + mensajes[this->idMensajeP1].tiempoRealProc + mensajes[this->idMensajeP1].tiempoEnColas);
			this->sumatoriaVecesDevuelto += mensajes[this->idMensajeP1].vecesDevuelto;
			this->tiempoColas += mensajes[this->idMensajeP1].tiempoEnColas;
			this->tiempoTransmicion += mensajes[this->idMensajeP1].tiempoTransmicion;
			this->sumatoriaTiempoReal += mensajes[this->idMensajeP1].tiempoRealProc;
		}
	}else if (mensajes[this->idMensajeP1].tiempoProc2 == 0 && mensajes[this->idMensajeP1].tiempoProc3 == 0){
		//printf("Me llego un mensaje de la computadora 3\n");
		//cout<<"Tiempo  proc2: "<< mensajes[this->idMensajeP1].tiempoProc2 <<" Tiempo  proc3: "<< mensajes[this->idMensajeP1].tiempoProc3 <<endl;
		//El mensaje proviene de la compu 3
		if (num <= this->X3){
			//Reenvia a la compu 3
			//printf("Voy a reenviar el mensaje a la compu 3\n");
			mensajes[this->idMensajeP1].tiempoTransmicion += 3;
			long tiempollegada = tiempoEvento + 3;
			mensajes[this->idMensajeP1].tiempoLlegada = tiempollegada;
			mensajes[this->idMensajeP1].vecesDevuelto++;
			colaTransmicion3.push_back(this->idMensajeP1);
			if (eventos->at(9) == this->tiempoSimulacion*4){
				//Ponemos que el primero de la cola de transicion es el que debe de llegar
				eventos->at(9) = mensajes[colaTransmicion3[0]].tiempoLlegada;
				this->arriboCompu3 = colaTransmicion3[0];
				colaTransmicion3.erase(colaTransmicion3.begin());
			}

		}else {
			//Se envia al destino (sale del sistema todo meco)
			//printf("Voy a enviar el mensaje a su destino\n");
			mensajes[this->idMensajeP1].estado = 2;
			this->tiemposProcesadores[0] += mensajes[this->idMensajeP1].tiempoProc1; // Almacena tiempo en proc1
			this->tiemposProcesadores[3] += mensajes[this->idMensajeP1].tiempoProc4; // Almacena tiempo en proc4
			this->sumatoriaTiemposMensajes += (mensajes[this->idMensajeP1].tiempoTransmicion + mensajes[this->idMensajeP1].tiempoRealProc + mensajes[this->idMensajeP1].tiempoEnColas);
			this->sumatoriaVecesDevuelto += mensajes[this->idMensajeP1].vecesDevuelto;
			this->tiempoColas += mensajes[this->idMensajeP1].tiempoEnColas;
			this->tiempoTransmicion += mensajes[this->idMensajeP1].tiempoTransmicion;
			this->sumatoriaTiempoReal += mensajes[this->idMensajeP1].tiempoRealProc;
		}
	}
	
	
	eventos->at(0) =this->tiempoSimulacion*4;//Desprograma evento 1
	
	this->proc1 = false;
	mensajes[this->idMensajeP1].tiempoInicioTrabajo = 0;
	this->idMensajeP1 =-1; 

	//Revisar la cola(proc) para atender a un mensaje que espera a ser atendido
	if (this->colaProc1.size() != 0){
		//Existen mensajes esperando en la cola de la computadora 1
		/*if (relojEvento < mensajes[colaProc1[0]].tiempoEntradaCola ){
			cout << "tiempo evento : "<<tiempoEvento << ", tiempoEntradaCola : "<< mensajes[colaProc1[0]].tiempoEntradaCola<<endl;
		}*/
		long tiempoEnCola = tiempoEvento - mensajes[colaProc1[0]].tiempoEntradaCola;
		mensajes[colaProc1[0]].tiempoEnColas += tiempoEnCola;
		this->proc1 = true; //El proc1 vuelve a estar ocupado
		this->idMensajeP1 = colaProc1[0];
		map<int , vector<double> >::iterator map = mapaD.find(6);
		eventos->at(0)  = relojEvento +	Manejador((*map).second);
		mensajes[colaProc1[0]].tiempoInicioTrabajo = relojEvento; 
		colaProc1.erase(colaProc1.begin()); // quitamos el consumido
	}


	

	return relojEvento;
}

/* @Funcion: simula el evento "Arribo de un mensaje a la computadora 1 del proc 1 de la computadora 2"
			en donde se si el procesador 1 esta ocupado lo agrega a la cola y si no lo procesa donde se
			considera la distribucion indicada por el usuario. Ademas se revisa la cola de transicion del
			procesador 1 en caso de que no este vacia se asigna el proximo en llegar al sistema
	@Param tiempoEvento: reloj principal del sistema
	@Param eventos: es un vector donde se van modificando los tiempos en los que pasa un determinado 
					evento.
	@Return: retorna la hora que tomara el reloj principal de la simulación
*/
//
long Evento::AMC1P1C2(long tiempoEvento,vector<long> *eventos){
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
		map<int , vector<double> >::iterator map = mapaD.find(6);
		eventos->at(0) = relojEvento + Manejador((*map).second); // Programamos el fin de trabajo
		mensajes[idMensajeP1].tiempoInicioTrabajo = relojEvento;
	}
	eventos->at(2) = this->tiempoSimulacion*4;// Desprograma el evento

	//Debemos revisar la cola de transicion para poder asignar el proximo arribo
	if (!colaTransmicion1.empty()){
		this->arriboCompu1P1 = colaTransmicion1[0]; // asignamos cual sera el proximo en llegar
		eventos->at(2) = mensajes[colaTransmicion1[0]].tiempoLlegada ; // programamos la llegada a este evento por parte de un mensajeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		colaTransmicion1.erase(colaTransmicion1.begin());
	}

	return relojEvento;
}

//Arribo de un mensaje a la computadora 1 del proc 2 de la computadora 2
long Evento::AMC1P2C2(long tiempoEvento,vector<long> *eventos){
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
		map<int , vector<double> >::iterator map = mapaD.find(6);
		eventos->at(0) = relojEvento + Manejador((*map).second); // Programamos el fin de trabajo
		mensajes[idMensajeP1].tiempoInicioTrabajo = relojEvento;
	}
	eventos->at(3) = this->tiempoSimulacion*4;// Desprograma el evento

	//Debemos revisar la cola de transicion para poder asignar el proximo arribo
	if (!colaTransmicion1.empty()){
		this->arriboCompu1P2 = colaTransmicion1[0]; // asignamos cual sera el proximo en llegar
		eventos->at(3) = mensajes[colaTransmicion1[0]].tiempoLlegada; // programamos la llegada a este evento por parte de un mensajeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee
		colaTransmicion1.erase(colaTransmicion1.begin());
	}


	return relojEvento;

}

//Arribo de un mensaje a la computadora 1 de la computadora 3 
long Evento::AMC1C3( long tiempoEvento, vector<long> *eventos){
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
		map<int , vector<double> >::iterator map = mapaD.find(6);
		eventos->at(0) = tiempoEvento + Manejador((*map).second);
		mensajes[idMensajeP1].tiempoInicioTrabajo = relojEvento;

	}
	eventos->at(1) = this->tiempoSimulacion*4; // Se desprograma este evento

	//Se revisa la cola de transicion para ver si hay mensajes que vienen de camino y si los hay ponemos ese valor como evento[1]
	if (!colaTransmicion1.empty()){
		eventos->at(1) = mensajes[colaTransmicion1[0]].tiempoLlegada; // RePrograma el arribo 
		this->arriboCompu1C3 = colaTransmicion1[0]; // Se asigna id del mensaje para poder trabajar
		colaTransmicion1.erase(colaTransmicion1.begin());
	}


	return relojEvento;
}

//Arribo de un mensaje a la computadora 2 de la computadora 1
long Evento::AMC2C1(long tiempoEvento,vector<long> *eventos){
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
		map<int , vector<double> >::iterator map = mapaD.find(3);
		eventos->at(7) = relojEvento + Manejador((*map).second); // Alistamos el evento de salida
		mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento; 
	}else if (proc3 && !proc2){
		//El proc2 esta desocupado el mensaje se puede meter ahi 
		proc2 = true; 
		this->idMensajeP2 = this->arriboCompu2; // asignamos el mensaje
		map<int , vector<double> >::iterator map = mapaD.find(2);
		eventos->at(6) = relojEvento + Manejador((*map).second);
		mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento;
	}else {
		//Si ambos estan desocupados se debe decidir a cual de los dos va a entrar
		//srand(time(NULL));
		double num = drand48() ;
		if (num <= 0.5 ){
			//Lo ponemos en el proc2
			proc2 = true; 
			this->idMensajeP2 = this->arriboCompu2; // asignamos el mensaje
			map<int , vector<double> >::iterator map = mapaD.find(2);
			eventos->at(6) = relojEvento + Manejador((*map).second);
			mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento;
		}else{
			//lo ponemos en el proc3
			proc3 = true; // El proc3 ahora esta ocupado
			this->idMensajeP3 = this->arriboCompu2; // almacenamos el idMensaje
			map<int , vector<double> >::iterator map = mapaD.find(3);
			eventos->at(7)= relojEvento + Manejador((*map).second); // Alistamos el evento de salida
			mensajes[this->arriboCompu2].tiempoInicioTrabajo = relojEvento; 
		}
	}
	eventos->at(4) = this->tiempoSimulacion*4; // Desprogramamos el arribo

	//Revisamos la cola de transmicion para "reprogramar el arribo desde la comp1"
	if (colaTransmicion2.size() != 0){
		// La cola no esta vacia o sea podemos reprogramar un arribo
		eventos->at(4) = mensajes[colaTransmicion2[0]].tiempoLlegada; // Reprogramamos el arribo
		this->arriboCompu2 = colaTransmicion2[0];
		colaTransmicion2.erase(colaTransmicion2.begin()); // elimnamos de la cola de transmicion
	}
	return relojEvento;
}

//Arribo de un mensaje a la computadora 2 desde afuera
long Evento::AMC2F(long tiempoEvento,vector<long> *eventos){
	//printf("Entre al evento 5\n");
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
		map<int , vector<double> >::iterator map = mapaD.find(3);
		eventos->at(7) = relojEvento + Manejador((*map).second); // Alistamos el evento de salida
		mensajes[idMensaje].tiempoInicioTrabajo = relojEvento; 
	}else if (proc3 && !proc2){
		//El proc2 esta desocupado el mensaje se puede meter ahi 
		proc2 = true; 
		this->idMensajeP2 = idMensaje; // asignamos el mensaje
		map<int , vector<double> >::iterator map = mapaD.find(2);
		eventos->at(6) = relojEvento + Manejador((*map).second);
		mensajes[idMensaje].tiempoInicioTrabajo = relojEvento;
	}else {
		//Si ambos estan desocupados se debe decidir a cual de los dos va a entrar
		//srand(time(NULL));
		double num = drand48();
		if (num <= 0.5 ){
			//Lo ponemos en el proc2
			proc2 = true; 
			this->idMensajeP2 = idMensaje; // asignamos el mensaje
			map<int , vector<double> >::iterator map = mapaD.find(2);
			eventos->at(6) = relojEvento + Manejador((*map).second);
			mensajes[idMensaje].tiempoInicioTrabajo = relojEvento;
		}else{
			//lo ponemos en el proc3
			proc3 = true; // El proc3 ahora esta ocupado
			this->idMensajeP3 = idMensaje; // almacenamos el idMensaje
			map<int , vector<double> >::iterator map = mapaD.find(3);
			eventos->at(7) = relojEvento + Manejador((*map).second); // Alistamos el evento de salida
			mensajes[idMensaje].tiempoInicioTrabajo = relojEvento; 
		}
	}
	map<int , vector<double> >::iterator map = mapaD.find(1);
	eventos->at(5) = relojEvento + Manejador((*map).second);
	//cout<<"El evento 5 volvera a pasar en el tiempo: "<<eventos->at(5)<<endl;
	//cout <<"El reloj es de : "<< relojEvento<< endl;
	return relojEvento;
}

//Finalizacion de procesamiento del mensaje en el proc 1 de la computadora 2
long Evento::FC2P1
(long tiempoEvento,vector<long> *eventos){
	long relojEvento = tiempoEvento;
	//Se envia un mensaje a la computadora 1
	mensajes[idMensajeP2].tiempoTransmicion += 20; // estadisico
	colaTransmicion1.push_back(idMensajeP2);
	mensajes[idMensajeP2].tiempoLlegada = relojEvento +20; // Importanteeeee
	
	//Comprobamos si ya se programo el arribo a la computadora 1
	if (eventos->at(2) == this->tiempoSimulacion*4) {
		//Hemos de progrmar el siguiete arribo
		eventos->at(2) = mensajes[colaTransmicion1[0]].tiempoLlegada; // Programamos
		this->arriboCompu1P1 = colaTransmicion1[0];
		colaTransmicion1.erase(colaTransmicion1.begin());
	}

	//Calculos estadisticos
	long tiempoRealProc = relojEvento - mensajes[idMensajeP2].tiempoInicioTrabajo;
	mensajes[idMensajeP2].tiempoRealProc += tiempoRealProc;
	mensajes[idMensajeP2].tiempoProc2 += tiempoRealProc;

	//Desprogramos valores de la simulacion
	eventos->at(6) = this->tiempoSimulacion*4; // Desprogrmamos el evento
	proc2 = false; // ya no esta ocupado 
	mensajes[idMensajeP2].tiempoInicioTrabajo = 0; // por si acaso
	this->idMensajeP2 = -1; // quitamos el mensaje actual

	//Revisamos la cola de la compu2 a ver si hay algun mensaje en espera
	if(colaProc2.size() != 0){
		//existen mensajes esperando a ser atendidos
		/*if (relojEvento < mensajes[colaProc2[0]].tiempoEntradaCola ){
			cout << "tiempo evento : "<<tiempoEvento << ", tiempoEntradaCola : "<< mensajes[colaProc2[0]].tiempoEntradaCola<<endl;
		}*/
		long tiempoEnCola = relojEvento - mensajes[colaProc2[0]].tiempoEntradaCola;
		mensajes[colaProc2[0]].tiempoEnColas += tiempoEnCola;


		proc2 = true; // Se ocupa el proc2
		this->idMensajeP2 = colaProc2[0];
		map<int , vector<double> >::iterator map = mapaD.find(2);
		eventos->at(6) = relojEvento +  Manejador((*map).second);
		mensajes[idMensajeP2].tiempoInicioTrabajo = relojEvento;
		colaProc2.erase(colaProc2.begin()); 

	}

	return relojEvento;
}

//Finalizacion de procesamiento del mensaje en el proc 2 de la computadora 2
long Evento::FC2P2
(long tiempoEvento,vector<long> *eventos){
	long relojEvento = tiempoEvento;
	//Se envia un mensaje a la computadora 1
	mensajes[idMensajeP3].tiempoTransmicion += 20; // estadisico
	colaTransmicion1.push_back(idMensajeP3);
	mensajes[idMensajeP3].tiempoLlegada = relojEvento +20; // Importanteeeee
	
	//Comprobamos si ya se programo el arribo a la computadora 1
	if (eventos->at(3) == this->tiempoSimulacion*4) {
		//Hemos de progrmar el siguiete arribo
		eventos->at(3) = mensajes[colaTransmicion1[0]].tiempoLlegada; // Programamos
		this->arriboCompu1P2 = colaTransmicion1[0];
		colaTransmicion1.erase(colaTransmicion1.begin());
	}

	//Calculos estadisticos
	long tiempoRealProc = relojEvento - mensajes[idMensajeP3].tiempoInicioTrabajo;
	mensajes[idMensajeP3].tiempoRealProc += tiempoRealProc;
	mensajes[idMensajeP3].tiempoProc3 += tiempoRealProc;

	//Desprogramos valores de la simulacion
	eventos->at(7) = this->tiempoSimulacion*4; // Desprogrmamos el evento
	proc3 = false; // ya no esta ocupado 
	mensajes[idMensajeP3].tiempoInicioTrabajo = 0; // por si acaso
	this->idMensajeP3 = -1; // quitamos el mensaje actual

	//Revisamos la cola de la compu2 a ver si hay algun mensaje en espera
	if(colaProc2.size() != 0){
		//existen mensajes esperando a ser atendidos
		/*if (relojEvento < mensajes[colaProc2[0]].tiempoEntradaCola ){
			cout << "tiempo evento : "<<tiempoEvento << ", tiempoEntradaCola : "<< mensajes[colaProc2[0]].tiempoEntradaCola<<endl;
		}*/
		
		long tiempoEnCola = relojEvento - mensajes[colaProc2[0]].tiempoEntradaCola;
		mensajes[colaProc2[0]].tiempoEnColas += tiempoEnCola;


		proc3 = true; // Se ocupa el proc2
		this->idMensajeP3 = colaProc2[0];
		map<int , vector<double> >::iterator map = mapaD.find(2);
		eventos->at(7) = relojEvento +  Manejador((*map).second);
		mensajes[idMensajeP3].tiempoInicioTrabajo = relojEvento;
		colaProc2.erase(colaProc2.begin()); 

	}

	return relojEvento;

}

//Arribo de mensaje a la computadora 3 de la computadora 1
long Evento::AMC3C1( long tiempoEvento,vector<long> *eventos){
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
		map<int , vector<double> >::iterator map = mapaD.find(5);
		eventos->at(8)= relojEvento + Manejador((*map).second); // Programamos la salida
		mensajes[idMensajeP4].tiempoInicioTrabajo = relojEvento; // Iniciamos una toma del tiempo para el tiempo real y otros 

	}
	eventos->at(9) = this->tiempoSimulacion*4;
	//Se revisa la cola de transicion para ver si hay mensajes que vienen de camino y si los hay ponemos ese valor como evento[1]
	if (!colaTransmicion3.empty()){
		eventos->at(9) = mensajes[colaTransmicion3[0]].tiempoLlegada; // RePrograma el arribo 
		this->arriboCompu3 = colaTransmicion3[0];// Se asigna id del mensaje para poder trabajar
		colaTransmicion3.erase(colaTransmicion3.begin());
	}

	return relojEvento;
}

//Arribo de mensaje a la computadora 3 desde afuera
long Evento::AMC3F(long tiempoEvento,vector<long> *eventos){
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
		//printf("Entre en donde es pa\n");
		proc4 = true;
		this->idMensajeP4 = idMensaje;
		map<int , vector<double> >::iterator map = mapaD.find(5);
		eventos->at(8) = relojEvento + Manejador((*map).second);
		mensajes[idMensajeP4].tiempoInicioTrabajo = relojEvento;
	}
	map<int , vector<double> >::iterator map = mapaD.find(4);
	eventos->at(10) = relojEvento +Manejador((*map).second); //Reprograma este mismo evento

	//cout <<"Cantidad de mensajes en el vector"<<this->mensajes.size()<<endl;
	//cout <<"El tiempo en el que volvere a pasar es : "<< eventos->at(10) <<endl;

	return relojEvento;
}

//Finalizacion de procesamiento del mensaje de la computadora 3
long Evento::FC3
(long tiempoEvento,vector<long> *eventos){
	//cout<<"------------- soy el mensaje: "<<this->idMensajeP4<<"-------------------" <<endl;

	long relojEvento = tiempoEvento;
	//srand(time(NULL));
	//Calculos estadisticos
	long tiempoRealProc = relojEvento - mensajes[idMensajeP4].tiempoInicioTrabajo;
	//cout<<"Valores tiempoRealProc:  "<<relojEvento<<", "<<mensajes[idMensajeP4].tiempoInicioTrabajo <<endl;
	mensajes[idMensajeP4].tiempoRealProc += tiempoRealProc;
	mensajes[idMensajeP4].tiempoProc4 += tiempoRealProc;

	double num = drand48();
	if(num <= this->X2){ 
		//printf("Eliminado\n");
		//Se elimina el mensaje
		mensajes[idMensajeP4].estado = 0;

		this->tiemposProcesadores[0] += mensajes[this->idMensajeP4].tiempoProc1; // Almacena tiempo en proc1
		//cout<<"Tiempo que pase en proc1: "<<this->mensajes[this->idMensajeP4].tiempoProc1 <<"----------------------------------------------------------------------------" <<endl;
		this->tiemposProcesadores[3] += mensajes[this->idMensajeP4].tiempoProc4; // Almacena tiempo en proc3
		this->tiempoProc1Perdidos += mensajes[this->idMensajeP4].tiempoProc1; //Almacena tiempo desperdiciado por proc1
		//cout<<"Tiempo perdido  proc1: "<<mensajes[this->idMensajeP4].tiempoProc1 <<"------------- soy el mensaje: "<<this->idMensajeP4<<"-------------------" <<endl;
		this->tiempoProc4Perdidos += mensajes[this->idMensajeP4].tiempoProc4; //Almacena tiempo desperdiciado por proc4
		this->mensajesEliminados++; // Aumentamos la cantidad de mensajes eliminados
		this->sumatoriaTiemposMensajes += (mensajes[this->idMensajeP4].tiempoTransmicion + mensajes[this->idMensajeP4].tiempoRealProc + mensajes[this->idMensajeP4].tiempoEnColas);
		this->sumatoriaVecesDevuelto += mensajes[this->idMensajeP4].vecesDevuelto;
		this->tiempoColas += mensajes[this->idMensajeP4].tiempoEnColas;
		this->tiempoTransmicion += mensajes[this->idMensajeP4].tiempoTransmicion;
		this->sumatoriaTiempoReal += mensajes[this->idMensajeP4].tiempoRealProc;
	}else{
		//Se envia a el mensaje a la computadora 1
		//printf("A la comp1\n");
		mensajes[idMensajeP4].tiempoTransmicion += 20; 
		mensajes[idMensajeP4].tiempoLlegada = relojEvento +20; // a esta hora va a llegar
		colaTransmicion1.push_back(idMensajeP4); //Ponemos el mensaje en la cola de envios "canal"
		if (eventos->at(1) == this->tiempoSimulacion*4 ){
			//Se debe asignar el tiempo del primer elemento de la cola de transicion1 a este evento
			eventos->at(1) = mensajes[colaTransmicion1[0]].tiempoLlegada;
			this->arriboCompu1C3 = colaTransmicion1[0]; // el mensaje que va a llegar sera el que este de primero
			colaTransmicion1.erase(colaTransmicion1.begin());
		}

	}

	


	//Desprogramamos valores de la simulacion
	eventos->at(8) = this->tiempoSimulacion*4;
	this->proc4 = false ; //el proc4 ya no esta ocupado
	mensajes[idMensajeP4].tiempoInicioTrabajo = 0; 
	this->idMensajeP4 = -1; // Se desprogama el id

	// Revision de la cola de mensajes en espera por ser atendidos a ver si podemos sacar a uno
	if (colaProc4.size() != 0){
		//Existen mensajes esperando a ser atendidos por el prov4
		/*if (relojEvento < mensajes[colaProc4[0]].tiempoEntradaCola ){
			cout << "tiempo evento : "<<tiempoEvento << ", tiempoEntradaCola : "<< mensajes[colaProc4[0]].tiempoEntradaCola<<endl;
		}*/
		long tiempoEnCola = relojEvento - mensajes[colaProc4[0]].tiempoEntradaCola;
		mensajes[colaProc4[0]].tiempoEnColas += tiempoEnCola;

		//Programamos valores utiles para el funcionamiento de la simulacion
		proc4 = true; // el proc4 esta ocupado
		this->idMensajeP4 = colaProc4[0]; 
		map<int , vector<double> >::iterator map = mapaD.find(5);
		eventos->at(8) = relojEvento + Manejador((*map).second);
		mensajes[this->idMensajeP4].tiempoInicioTrabajo = relojEvento;
		colaProc4.erase(colaProc4.begin());
	}


	return relojEvento;
}

//Si da un valor negativo reintertar
double Evento::DistribucionNormalMetodoDirecto(int miu, int varianza ){

	double num1 = drand48() ;
	//cout <<"num1"<< num1 <<endl;
	double num2 = drand48() ;
	//cout <<"num1"<< num2 <<endl;
	long z = (-2* log(num1)) * cos( (num2 * 2*PI)* PI /180.0) ; 
	//cout <<"z: "<< z <<endl;
	double retorno = sqrt(varianza) * z + miu ;

	return retorno;
}

//Si da un valor negativo reintertar
double Evento::DistribucionNormalMetodoTLC(int miu, int varianza){
	double retorno ; 
	double z = 0;
	//srand(time(NULL));
	//Sumatoria
	for (int i = 1; i <= 12; ++i)
	{
		double num1 = drand48()  ;
		
		z += num1  ;
	}
	//cout<<"El valor de la sumatoria es de :  "<< z <<endl;
	z = z -6;
	retorno = sqrt(varianza) * z + miu;
	//cout<<"Este es el valor del metodo TLC: "<< retorno <<endl;
 	return retorno;
}

double Evento::DistribucionUniforme(int a, int b) {
	//srand(time(NULL));
	double num1 =drand48() ;
	double retorno = num1*(b - a)+a; 
	//cout<<"Este es el valor de la uniforme "<< retorno <<endl;
	return retorno;
}

double Evento::DistribucionExponencialParametro(double lambda){
	double retorno ;
	//srand(time(NULL));
	double num1 = drand48() ;
	if(num1 != 1){
		retorno =  (-log(1-num1)) / lambda;
	}else if (num1 != 0){
		retorno = (-log(num1)) / lambda;
	}
	return retorno;
}

double Evento::DistribucionDensidad(double k, int a , int b){
	//srand(time(NULL));
	double num1 = drand48() ;
	//cout<<"Este es el valor de num1 del de densidad : "<< num1 <<endl;
	double division = 2* num1 / k;
	//cout<<"Este es el valor de de la division del de densidad : "<< division <<endl;
	double suma = division + (a*a);
	//cout<<"Este es el valor de suma del de densidad : "<< suma <<endl;
	double retorno = sqrt(suma);
	//cout<<"Este es el valor de retorno del de densidad : "<< retorno <<endl;
	return retorno;
}

double Evento::Manejador(vector<double> v){
	int caso = v[0];
	double valor;
	switch(caso){
		case 1:
			valor = DistribucionNormalMetodoDirecto(v[1],v[2]);
			//cout<<"Este es el valor del metodo directo: "<< valor <<endl;
			while (valor < -1){
				//cout<<"Este es el valor del metodo directo: "<< valor <<endl;
				valor = DistribucionNormalMetodoDirecto(v[1],v[2]);

			}
			
			break;
		case 2: 
			valor = DistribucionNormalMetodoTLC(v[1], v[2]);
			//cout<<"Este es el valor del metodo TLC: "<< valor <<endl;
			while(valor < -1){
				//cout<<"Este es el valor del metodo TLC: "<< valor <<endl;
				valor = DistribucionNormalMetodoTLC(v[1], v[2]);
				
			}
			
			break;
		case 3: 
			valor = DistribucionUniforme(v[1], v[2]);
			break;
		case 4 : 
			valor = DistribucionExponencialParametro(v[1]);
			break;
		case 5 : 
			valor = DistribucionDensidad(v[1],v[2],v[3]);
			//cout<<"Este es el valor del metodo de densidad: "<< valor <<endl;
			//exit(0);
			break;
		default:
			printf("default\n");
	}

	return valor;
}

