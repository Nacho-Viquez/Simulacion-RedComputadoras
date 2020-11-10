#include "Mensaje.h"
#include "Evento.h"
#include <map>

long manejadorEventos(long reloj,vector<long> *eventos, long indiceEventoProximo, Evento *evento){
	long relojActual = eventos->at(indiceEventoProximo);

	switch(indiceEventoProximo){
		//Computadora 1
		case 0: //Finalización de procesamiento del mensaje de la computadora 1
			relojActual = evento->FC1(relojActual,eventos);
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
			relojActual = evento->FC2P1(relojActual,eventos);
			break;
		case 7: //Finalizacion de procesamiento del mensaje en el proc 2 de la computadora 2
			relojActual = evento->FC2P2(relojActual,eventos);
			break;
		case 8: //Finalizacion de procesamiento del mensaje de la computadora 3
			relojActual = evento->FC3(relojActual,eventos);
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


void obtencionParametros(int escogencia, map<int, vector<double>> * map, int valorD){
	double miu, varianza,a,b, k, lambda;

	switch (escogencia){
		case 1 : // Se ocupan dos parametros
			
			cout << "Digite el valor para mui: ";
			cin>>miu ;
			cout << "Digite el valor para la varianza: ";
			cin>>varianza;
			map->insert(pair< int, vector<double> > (valorD+1 , {1,miu,varianza}));
			break;
		case 2: 
			
			cout << "Digite el valor para mui: ";
			cin>>miu ;
			cout << "Digite el valor para la varianza: ";
			cin>>varianza;
			map->insert(pair< int, vector<double> > (valorD+1 , {2,miu,varianza}));
			break;
		case 3: 
			
			cout << "Digite el valor para a: ";
			cin>>a ;
			cout << "Digite el valor para la b: ";
			cin>>b;
			map->insert(pair< int, vector<double> > (valorD+1 , {3,a,b}));
			break; 
		case 4: 
			
			cout << "Digite el valor para lambda: ";
			cin>>lambda ;
			map->insert(pair< int, vector<double> > (valorD+1 , {4,lambda}));
			break;
		case 5:
			
			cout << "Digite el valor para k: ";
			cin>>k ;
			cout << "Digite el valor para a: ";
			cin>>a;
			cout << "Digite el valor para b: ";
			cin>>b;
			map->insert(pair< int, vector<double> > (valorD+1 , {5, k, a,b}));
			break;
		default:
			printf("default\n");
	}

}

void intervaloConfianza(vector<double>* v){
	
	double varianzaMuestral = 0.0;
	double mediaMuestral = 0.0;
	double a , b ;

	for (int i = 0; i < v->size(); ++i){
	 	mediaMuestral += v->at(i);
	} 
	mediaMuestral = mediaMuestral / 10;

	for (int i = 0; i < v->size(); ++i)
	{
		varianzaMuestral += (v->at(i) - mediaMuestral) * (v->at(i) - mediaMuestral);
	}
	varianzaMuestral = varianzaMuestral / 9;

	a = (varianzaMuestral / 10 );
	a = pow(a,0.5);
	a = mediaMuestral - 2.26 * a;
	b = (varianzaMuestral / 10 );
	b = pow(b,0.5);
	b = mediaMuestral + 2.26 * b;

	v->clear();
	v->push_back(a);
	v->push_back(b);

}


/*
 * main del proyecto 
*/
int main(int argc, char const *argv[])
{
	double X1 = 0.2;
	double X2 = 0.5;
	double X3 = 0.5;
	double tiempoTotal = 0.0;
	int corridas = 0;


	//Pedir datos al usuario
	cout << "Indique cuanto tiempo va a durar la simulacion:";
	cin>> tiempoTotal;
	cout <<endl;
	cout << "Indique cuantas veces quiere de la simulacion:";
	cin>> corridas;
	cout <<endl;
	//Probas
	
	cout << "Digite el valor para X1: "<< endl;
	cin>>X1 ;
	cout << "Digite el valor para X2: "<< endl;
	cin>>X2 ;
	cout << "Digite el valor para X3: "<< endl;
	cin>>X3 ;
	
	map<int , vector<double>> mapaDistros;
	//Seleccion de distros
	int escogencia ; 
	for (int i = 0; i <= 5; ++i)
	{
		escogencia = 0;
		cout << "Ingrese la opcion que desea para D"<<i+1<<": "<<endl;
		cout <<endl;
		cout << "Opcion 1: Normal Metodo directo"<< endl;
		cout << "Opcion 2: Normal Metodo TLC"<< endl;
		cout << "Opcion 3: Uniforme"<< endl;
		cout << "Opcion 4: Exponencial"<< endl;
		cout << "Opcion 5: Densidad"<< endl;
		cout <<endl;
		cout << "Su seleccion: ";
		cin >> escogencia;

		obtencionParametros(escogencia, &mapaDistros, i);
	}

	//Inicializacion de instancias de clases
	Evento * evento = new Evento(X1,X2,X3, mapaDistros,tiempoTotal);


	//Variables "globales" simulacion
	long reloj = 0;
	long tiempoDuracionSimulacion = tiempoTotal;
	vector<long> eventos; //Vector para los tiempos de ejecucion de los eventos
	eventos.resize(11, tiempoTotal*4);	 //Inicializamos todos los eventos en inf

	//Valores iniciales a los eventos inciales
	eventos[5] = 0;
	eventos[10] = 0;

	//Valores para las estadisticas acumuladas
	double porcentajeOcupadoProc1Total = 0,porcentajeOcupadoProc2Total= 0,porcentajeOcupadoProc3Total= 0,porcentajeOcupadoProc4Total= 0;
	double porcentajeP1RechazadoTotal= 0, porcentajeP4RechazadoTotal= 0, porcentajeMensajesRechazdosTotal= 0, promedioTiempoSistemaMensajeTotal= 0;
	double numeroVecesDevueltoTotal= 0, tiempoPromedioColasTotal= 0 ,tiempoPromedioTransTotal= 0,promedioTrabajoRealTotal= 0;
	vector<double> valoresIntervalo;

	//Ciclo principal de la simulacion
	for (int i = 0; i < corridas; ++i)
	{
		while(reloj<= tiempoDuracionSimulacion ){
			//Comprobacion de tiempo para siguiente accion
			long eventoProximo = tiempoTotal*4; 
			int indiceEventoProximo = tiempoTotal*4;
			for (int i = 0; i < eventos.size(); ++i)
			{
				if ( eventos[i] < eventoProximo ){
					eventoProximo = eventos[i];
					indiceEventoProximo = i;
				}
			}
			//Se realiza el siguiente evento indicado en indiceEventoProximo
			reloj = manejadorEventos(reloj, &eventos, indiceEventoProximo, evento );

		}


		printf("Sali de la corrida no. %d---------------------------------------------------------------------\n",i);


		//Calculos finales estadisticos 
		//1.a
		double porcentajeOcupadoProc1 = (evento->tiemposProcesadores[0]*100)/ tiempoTotal; 
		porcentajeOcupadoProc1Total += porcentajeOcupadoProc1;

		double porcentajeOcupadoProc2 = (evento->tiemposProcesadores[1] *100)/ tiempoTotal; 
		porcentajeOcupadoProc2Total += porcentajeOcupadoProc2;

		double porcentajeOcupadoProc3 = (evento->tiemposProcesadores[2] *100)/ tiempoTotal;
		porcentajeOcupadoProc3Total += porcentajeOcupadoProc3;

		double porcentajeOcupadoProc4 = (evento->tiemposProcesadores[3]*100)/ tiempoTotal;
		porcentajeOcupadoProc4Total += porcentajeOcupadoProc4;
		cout<< "Porcentaje del tiempo que pasa ocupado el procesador 1(compu1): "<<porcentajeOcupadoProc1 <<endl;
		cout<< "Porcentaje del tiempo que pasa ocupado el procesador 2(compu2): "<<porcentajeOcupadoProc2 <<endl;
		cout<< "Porcentaje del tiempo que pasa ocupado el procesador 3(compu3): "<<porcentajeOcupadoProc3 <<endl;
		cout<< "Porcentaje del tiempo que pasa ocupado el procesador 4(compu4): "<<porcentajeOcupadoProc4 <<endl;

		//1.b
		double porcentajeP1Rechazado = (evento->tiempoProc1Perdidos *100)/ evento->tiemposProcesadores[0]+ 0.0 ;
		porcentajeP1RechazadoTotal += porcentajeP1Rechazado;
		double porcentajeP4Rechazado = (evento->tiempoProc4Perdidos*100)/ evento->tiemposProcesadores[3]+ 0.0 ;
		porcentajeP4RechazadoTotal += porcentajeP4Rechazado;
		cout<< "Porcentaje del tiempo que pasa ocupado el procesador 1(compu1) con mensajes desechados: "<<porcentajeP1Rechazado <<endl;
		cout<< "Porcentaje del tiempo que pasa ocupado el procesador 4(compu3) con mensajes desechados: "<<porcentajeP4Rechazado <<endl;

		//1.c
		double porcentajeMensajesRechazdos = (evento->mensajesEliminados *100+ 0.0 )/ evento->mensajes.size();
		porcentajeMensajesRechazdosTotal += porcentajeMensajesRechazdos;
		cout<< "Porcentaje del total de mensajes rechazados: "<<porcentajeMensajesRechazdos <<endl;

		//1.d
		double promedioTiempoSistemaMensaje = evento->sumatoriaTiemposMensajes /tiempoTotal+ 0.0;
		valoresIntervalo.push_back(promedioTiempoSistemaMensaje);
		promedioTiempoSistemaMensajeTotal += promedioTiempoSistemaMensaje;
		cout<< "Tiempo promedio en el sistema por cada mensaje: "<<promedioTiempoSistemaMensaje <<endl;

		//1.e
		double cantidad = 0+ 0.0;
		for (int i = 0; i < evento->mensajes.size(); ++i)
		{
			if( !(evento->mensajes[i].estado == 1)){

				cantidad++;			
			}
		}
		
		double numeroVecesDevuelto = evento->sumatoriaVecesDevuelto  / cantidad;
		numeroVecesDevueltoTotal += numeroVecesDevuelto;
		cout<< "Numero promedio de veces que un mensaje fue devuelto por la computadora 1:  "<<numeroVecesDevuelto <<endl;

		//1.f 
		double tiempoPromedioColas = evento->tiempoColas /tiempoTotal+ 0.0;
		tiempoPromedioColasTotal += tiempoPromedioColas;
		cout<< "Tiempo promedio en colas: "<<tiempoPromedioColas <<endl;

		//1.g
		double tiempoPromedioTrans = evento->tiempoTransmicion / tiempoTotal+ 0.0;
		tiempoPromedioTransTotal += tiempoPromedioTrans;
		cout<< "Tiempo promedio en trasnmicion: "<<tiempoPromedioTrans <<endl;

		//1.h
		double promedioTrabajoReal = (evento->sumatoriaTiempoReal *100.0)/ evento->sumatoriaTiemposMensajes+ 0.0;
		promedioTrabajoRealTotal += promedioTrabajoReal;
		cout<< "Porcentaje del tiempo total de cada mensaje que fue utilizado en procesamiendo real: "<<promedioTrabajoReal <<endl;
		//Reseteamos valores para la siguiente corrida
		for (int i = 0; i < eventos.size(); ++i)
		{
			eventos[i] = tiempoTotal*4;
		}
		eventos[5] = 0;
		eventos[10] = 0;
		evento->LimpiarSimulacion();
		reloj = 0;

	}
	
	cout << " \n******************Promedio de estadisticas de las corridas**********************************"<< endl;
	cout<< "Porcentaje del tiempo que pasa ocupado el procesador 1(compu1): "<<porcentajeOcupadoProc1Total / corridas <<endl;
	cout<< "Porcentaje del tiempo que pasa ocupado el procesador 2(compu2): "<<porcentajeOcupadoProc2Total/ corridas  <<endl;
	cout<< "Porcentaje del tiempo que pasa ocupado el procesador 3(compu3): "<<porcentajeOcupadoProc3Total/ corridas  <<endl;
	cout<< "Porcentaje del tiempo que pasa ocupado el procesador 4(compu4): "<<porcentajeOcupadoProc4Total/ corridas  <<endl;
	cout<< "Porcentaje del tiempo que pasa ocupado el procesador 1(compu1) con mensajes desechados: "<<porcentajeP1RechazadoTotal/ corridas  <<endl;
	cout<< "Porcentaje del tiempo que pasa ocupado el procesador 4(compu3) con mensajes desechados: "<<porcentajeP4RechazadoTotal/ corridas  <<endl;
	cout<< "Porcentaje del total de mensajes rechazados: "<<porcentajeMensajesRechazdosTotal/ corridas  <<endl;
	cout<< "Tiempo promedio en el sistema por cada mensaje: "<<promedioTiempoSistemaMensajeTotal / corridas <<endl;
	cout<< "Numero promedio de veces que un mensaje fue devuelto por la computadora 1:  "<<numeroVecesDevueltoTotal/ corridas  <<endl;
	cout<< "Tiempo promedio en colas: "<<tiempoPromedioColasTotal/ corridas  <<endl;
	cout<< "Tiempo promedio en trasnmicion: "<<tiempoPromedioTransTotal/ corridas  <<endl;
	cout<< "Tiempo promedio en trasnmicion: "<<tiempoPromedioTransTotal/ corridas  <<endl;
	cout<< "Porcentaje del tiempo total de cada mensaje que fue utilizado en procesamiendo real: "<<promedioTrabajoRealTotal/ corridas  <<endl;
	
	if(corridas == 10){
		intervaloConfianza(&valoresIntervalo);
	}
	cout <<"El intervalo de confianza con una proba de 0.95 comprende de : "<<valoresIntervalo[0]<<", "<<valoresIntervalo[1]<<endl;

	cout << "*******************Final de la simulacion, hasta la proxima********************************** "<<endl;



	delete(evento);
	return 0;
}