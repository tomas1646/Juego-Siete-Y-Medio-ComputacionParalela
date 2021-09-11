#include <iostream>
#include<sys/wait.h>
#include<unistd.h>
#include <sys/types.h>         
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/un.h>

using namespace std;


/*
Para correr en LINUX
Comandos
g++ sieteymedioSocket.cpp -o sieteymedioSocket
para compilar y despues para ejecutar
./sieteymedioSocket 5
*/

#define MY_SOCK_PATH "/tmp/server"
#define FALSE 0


int main(int argc, char *argv[]) {
	
	srand( (unsigned)time( NULL ) );
	
	system("clear");
	
	const int n = 1;
	
	int* Id1 = new int[n];
	
	/*Creo los jugadores*/
	for(int i=0; i<n; i++){
		Id1[i] = fork();
		if(Id1[i] == -1){
			cout<<"Error creadno el proceso"<<endl;
		}
		if(Id1[i] == 0){
			/*Codigo Jugador (Hijo)*/
			int Jugador;
			Jugador = socket(AF_UNIX, SOCK_STREAM, 0);
			if(Jugador == -1){
				cout<<"No se creo el socket ERROR"<<endl;
			}
			
			
			struct sockaddr_un serveraddr;
			
			memset(&serveraddr, 0, sizeof(serveraddr));
			serveraddr.sun_family = AF_UNIX;
			if (argc > 1)
				strcpy(serveraddr.sun_path, argv[1]);
			else
				strcpy(serveraddr.sun_path, MY_SOCK_PATH);
			
			int rc = connect(Jugador, (struct sockaddr *)&serveraddr, SUN_LEN(&serveraddr));
			if (rc < 0)
			{
				perror("connect() failed");
				break;
			}
			char buffer[100];
			memset(buffer, 'AL FINNNN', sizeof(buffer));
			rc = send(Jugador, buffer, sizeof(buffer), 0);
			if (rc < 0)
			{
				perror("send() failed");
				break;
			}
			cout<<"Conecto"<<endl;
			
			return 0;
		}
	}
	
	struct sockaddr_un servidor;
	int repartidor;
	
	do{
	/*Creo socket*/
	repartidor = socket(AF_UNIX, SOCK_STREAM, 0);
	if(repartidor == -1){
		cout<<"No se creo el socket ERROR"<<endl;
	}
	
	/*Bind*/
	memset(&servidor, 0, sizeof(servidor));
	servidor.sun_family = AF_UNIX;
	strncpy(servidor.sun_path, MY_SOCK_PATH, sizeof(servidor.sun_path)-1);
	
	if (bind(repartidor, (struct sockaddr *) &servidor, SUN_LEN(&servidor)) == -1){
		perror("bind() failed");
		break;
	}
	
	int rc;
	
	rc= listen(repartidor, 10);
	if (rc< 0)
	{
		perror("listen() failed");
		break;
		
	}
	
	cout<<"Listo para cliente"<<endl;
	
	int sd2;
	sd2 = accept(repartidor, NULL, NULL);
	if (sd2 < 0)
	{
		perror("accept() failed");
		break;
	}
	
	cout<<"Paso el accept"<<endl;
	
	
	}while(FALSE);
	
	wait(NULL);
	
	unlink(MY_SOCK_PATH);
	
	return 0;
}

