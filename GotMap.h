#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS
//Opción a del menú, introducir un vertice en el grafo
void introducir_vertice(grafo *G);

//Opción b del menú, eliminar un vértice del grafo
void eliminar_vertice(grafo *G);

//Opción c del menú, crear una relación entre dos vértices
void nuevo_arco(grafo *G);

//Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G);

//Opción i del menú, imprimir el grafo
void imprimir_grafo(grafo G);

//Cargar archivo
void cargarArchivoCiudades(char* nombre_archivo, grafo* G);
void cargarArchivoCaminos(char* nombre_archivo, grafo* G);

void camino_mas_corto(grafo G, char opcion);
void min_infraestructura(grafo G, char opcion);


#endif	/* FUNCIONES_H */

