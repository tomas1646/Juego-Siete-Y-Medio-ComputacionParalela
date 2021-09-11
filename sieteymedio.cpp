#include <iostream>
#include<sys/wait.h>
#include<unistd.h>
using namespace std;


/*
Para correr en LINUX
Comandos
Compilar: g++ sieteymedio.cpp -o sieteymedio
Ejecutar: ./sieteymedio 5
*/

int main(int argc, char *argv[]) {
	
	srand( (unsigned)time( NULL ) );
	
	system("clear");
	
	const int n = atoi(argv[1]);
	
	int cartas[10]= {1,2,3,4,5,6,7,10,11,12};
	
	int pipes[n][2][2];

	int* Id1 = new int[n];
	int* estados = new int[n];
	
	/*Creo los pipes*/
	for(int i=0; i<n; i++){
		if (pipe(pipes[i][0]) == -1){
			cout<<"Error al crear Pipe"<<endl;
		}
		if (pipe(pipes[i][1]) == -1){
			cout<<"Error al crear Pipe"<<endl;
		}
	}
	
	/*Creo los jugadores*/
	for(int i=0; i<n; i++){
		Id1[i] = fork();
		if(Id1[i] == -1){
			cout<<"Error creando el Jugador (Proceso Hijo)"<<endl;
		}
		if(Id1[i] == 0){
			
			/*Codigo del proceso hijo, el jugador*/
			float puntaje= 0;
			int carta;
			int estado = 0;
			/* 
			estado = 0 -> Pide otra carta
			estado = 1 -> Sa planta
			estado = 2 -> Abandona
			
			Aca es donde 'juega' el proceso*/
			do{
				if(read(pipes[i][0][0], &carta, sizeof(int)) == -1){
					cout<<"Error al leer pipe del proceso"<<i<<endl;
				}

				if(carta == 10 || carta == 11 || carta == 12){
					puntaje += 0.5;
				}else{
					puntaje += carta;
				}
				/*El que suma mas de 7.5 abandona*/
				if(puntaje > 7.5){
					estado = 2;
				}
				/*El que suma entre 5 y 7.5 se planta*/
				if(puntaje > 5 && puntaje <= 7.5){
					estado = 1;
				}
				/*Esto es por si quiero seguir el juego
				cout<<"Jugador"<<i+1<<" Lee carta: "<<carta<<" Puntos: "<<puntaje<<endl;*/
				
				if(write(pipes[i][1][1], &estado, sizeof(int)) == -1){
					cout<<"Error al leer pipe del proceso"<<i<<endl;
				}
				
			}while(estado == 0);
			
			/*AHora solo le pasa los puntos finales*/
			if(read(pipes[i][0][0], &carta, sizeof(int)) == -1){
				cout<<"Error al leer pipe del proceso"<<i<<endl;
			}
			if(write(pipes[i][1][1], &puntaje, sizeof(float)) == -1){
				cout<<"Error al leer pipe del proceso"<<i<<endl;
			}
			
			return 0;
		}
	}
	
	int cartaAl;
		
	/*Primer ronda, da cartas a todos*/
	for(int j=0; j<n; j++){
		cartaAl = cartas[rand() % 10];
		if(write(pipes[j][0][1], &cartaAl, sizeof(int)) == -1){
			cout<<"Error Proceso Padre escribir en Pipe"<<endl;
		}
	}
	
	bool QuedaGEnteJugando = true;
	/*Va entregando cartas a los que piden*/
	do{
		QuedaGEnteJugando = false;
		for(int i=0; i<n; i++){
			if(estados[i] == 0){
				if(read(pipes[i][1][0], &estados[i], sizeof(int)) == -1){
					cout<<"Error Proceso Padre escribir en Pipe"<<endl;
				}
				if(estados[i] == 0){
					QuedaGEnteJugando = true;
					cartaAl = cartas[rand() % 10];
					if(write(pipes[i][0][1], &cartaAl, sizeof(int)) == -1){
						cout<<"Error Proceso Padre escribir en Pipe"<<endl;
					}
				}
			}

		}
	}while(QuedaGEnteJugando);
	
	cout<<"********Tabla Posiciones*********"<<endl;
	
	float puntaje;
	int ganador = -1;
	float distancia;
	float distanciaG = 7.5;
	
	for(int j=0; j<n; j++){
		puntaje = 0;
		if(write(pipes[j][0][1], &puntaje, sizeof(int)) == -1){
			cout<<"Error Proceso Padre escribir en Pipe"<<endl;
		}
		if(read(pipes[j][1][0], &puntaje, sizeof(float)) == -1){
			cout<<"Error Proceso Padre escribir en Pipe"<<endl;
		}

		if(estados[j] == 1){
			distancia = 7.5 - puntaje;
			if(distancia < distanciaG){
				ganador = j;
				distanciaG = distancia;
			}
		}
		cout<<"Jugador "<<j+1<<" -> ";
	
		if(estados[j] == 1){
			cout<<"Se planto";
		}else{
			cout<<"Abandono ";
		}
		cout<<"  Puntos: "<<puntaje<<endl;
	}
	
	cout<<endl<<"******Ganador*******"<<endl;
	
	if(ganador == -1){
		cout<<"Ningun jugador gano, todos abandonaron"<<endl;
	}else{
		cout<<"     Jugador "<<ganador+1<<endl;
	}
	
	for(int i=0; i<n; i++){
		wait(NULL);
	}	
	
	return 0;
}

