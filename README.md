# Juego-Siete-Y-Medio-ComputacionParalela

Siete y medio es un juego que consiste en dar una carta a cada jugador, y ellos deciden si piden otra, o se plantan.
Esta decision la toman de acuerdo al valor que consiguen sumando sus cartas.

Las cartas son 1,2,3,4,5,6,7,10,11,12. Todas las cartas del 1 al 7 suman el valor de la carta. Luego 10,11 y 12, suman 0.5 puntos.

El jugador que se pasa de 7.5 puntos abandona. Y el ganador es el que primero llega a 7.5. O el que mas cerca esta de este valor.
En caso de empate, gana el primero que se acerco mas al valor.

## Implementacion

Cada jugador es un proceso distinto, y el repartidor, es el proceso padre. La comunicacion se realiza por medio de pipes, mecanismos unidireccionales, y por ahi el repartidor envia la carta. Y el jugador le dice si se planta, o abandona. Tambien sirven los pipes, para la sincronizacion de la partida.
