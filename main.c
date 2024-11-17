#include <stdio.h>
#include <stdlib.h>
#include "GotMap.h"

/*
 * Programa que muestra el uso del TAD grafo de números enteros
 */

int main(int argc, char** argv) {
    //Grafo de números enteros
    grafo G; //grafo
    char opcion;

    //Creo el grafo
    crear_grafo(&G);
    cargarArchivoCiudades("ciudades.csv", &G);
    cargarArchivoCaminos("caminos.csv", &G);
    do {
        printf("\n\na. Insertar nuevo vertice\n");
        printf("b. Eliminar vertice\n");
        printf("c. Crear arco\n");
        printf("d. Eliminar arco\n");
        printf("i. Imprimir grafo\n");
        printf("m. Distancia más corta entre 2 ciudades\n");
        printf("n. Tiempo menor entre 2 ciudades\n");
        printf("s. Salir\n");

        printf("Opcion: ");
        scanf(" %c", &opcion); 

        switch (opcion) {
            case 'a':case'A':
                introducir_vertice(&G);
                break;
            case 'b':case 'B':
                eliminar_vertice(&G);
                break;
            case 'c': case 'C':
                nuevo_arco(&G);
                break;
            case 'd': case 'D':
                eliminar_arco(&G);
                break;
            case 'i': case 'I':
                imprimir_grafo(G);
                break;
            case 'm': case 'M':
                camino_mas_corto(G,'c');
                break;
            case 'n': case 'N':
                char dragon;
                printf("El viaje será en dragón? (y/n): ");
                scanf(" %c",&dragon);
                if(dragon=='y'){
                    camino_mas_corto(G,'d');
                }else if(dragon=='n'){
                    camino_mas_corto(G,'n');
                }else{
                    printf("Opcion incorrecta");
                }
                break;
            case 's': case 'S':
                opcion='s';
                break;
            default:
                printf("Opción equivocada\n");
        }
        while (getchar() != '\n');
    } while (opcion != 'S' && opcion !='s');

    //Al salir, liberamos la memoria del TAD, lo destruimos
    borrar_grafo(&G);

    return (EXIT_SUCCESS);
}

