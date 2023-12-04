# TallerEstructura

Connet4- Juego conecta 4 en C++

Este juego ha sido desarrollado como proyecto de aprendizaje, en este ReadMe encontrarás informacion sobre la implementacion,desiciones de diseños y resultados de pruebas solicitados.

Connet4.hpp:Contiene la implementacion la cual gestiona el estado de juego, la logica de IA y las funciones principales.

main.cpp:Inicia el juego, configura la dificultad y comienza la partida.

Guardado y carga de partida: Se implementa la funcionalidad de guardar y cargar partidas para entregar flexibilidad en cuanto juego al jugador, puedes guardar el juego despues de cada movimiento (despues de movimiento jugador/IA) al igual que cargar un archivo (partida).

Niveles:

El juego Connect4, se implemento con tres niveles de dificultad: facil,medio y dificil. Estos niveles se ajustan a la profundidad del logaritmo Minimax, proporcionando asi un aumento de dificultad y una experiencia desafiante acorde a la eleccion del jugador

Algoritmo Minimax: Es el nucleo fundamental de la IA implementada en este juego, ya que realiza la exploracion de arboles de juego para determinar la mejor jugada posible. Se implemento funciones de evaluacion heurística (utilizando un valor numerico o estado) para la eficiencia de dicho algoritmo.

Comparacion de Minimax sin poda y con poda: Al realizar las pruebas comparativas entre con/sin poda alfa-beta, se llego al resultado de una mejora significativa al usar este algoritmo, reduciendo el numero de nodos explorados y acelarando la toma de desiciones de la IA

Compilacion y ejecucion:
g++ -o Connect4Game main.cpp Connect4.cpp
