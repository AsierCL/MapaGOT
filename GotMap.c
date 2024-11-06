#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

//Opción a del menú, introducir un vertice en el grafo

void introducir_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce el nombre de la ciudad: ");
    scanf(" %[^\n\r]", &v1.nombre);
    printf("Introduce la region a la que pertenece: ");
    scanf(" %[^\n\r]", &v1.region);

    if (existe_vertice(*G, v1))
        printf("Ese vertice ya esta en el grafo\n");
    else
        insertar_vertice(G, v1);
}
//Opción b del menú, eliminar un vértice del grafo

void eliminar_vertice(grafo *G) {
    tipovertice v1;
    printf("Introduce la ciudad que quieres eliminar: ");
    scanf(" %[^\n\r]", &v1);
    if (existe_vertice(*G, v1))
        borrar_vertice(G, v1);
    else
        printf("Ese vertice no existe en el grafo\n");
}

//Opción c del menú, crear una relación entre dos vértices

void nuevo_arco(grafo *G) {
    tipovertice v1, v2;
    float dist;
    char tipo;
    //Insertamos una nueva relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Nueva relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf(" %[^\n\r]", &v1.nombre);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.nombre);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf(" %[^\n\r]", &v2.nombre);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.nombre);
        return;
    }
    //Distancia
    printf("Introduce la distancia: ");
    scanf("%f",&dist);
    if(dist<=0){
        printf("No puedes usar una distancia negativa\n");
        return;
    }
    printf("Introduce el tipo (t/m): ");
    scanf(" %c", &tipo);
        if(tipo != 't' && tipo != 'm'){
            printf("Tipo de dato incorrecto");
            return;
        }
    //Creación del arco
    if (!son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        crear_arco(G, posicion(*G, v1), posicion(*G, v2), dist, tipo);
}

//Opción d del menú, eliminar una relación entre dos vértices
void eliminar_arco(grafo *G) {
    tipovertice v1, v2;
    //Eliminamos una relación pidiendo los datos al usuario controlando que existan los vértices
    printf("Eliminar relacion vertice1-->vertice2\n");
    //Vértice origen del arco
    printf("Introduce vertice origen: ");
    scanf(" %[^\n\r]", &v1.nombre);
    if (!existe_vertice(*G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.nombre);
        return;
    }
    //Vértice destino del arco
    printf("Introduce vertice destino: ");
    scanf(" %[^\n\r]", &v2.nombre);
    if (!existe_vertice(*G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.nombre);
        return;
    }
    //Eliminación del arco
    if (son_adyacentes(*G, posicion(*G, v1), posicion(*G, v2)))
        borrar_arco(G, posicion(*G, v1), posicion(*G, v2));
        borrar_arco(G, posicion(*G, v2), posicion(*G, v1));
}

//Opción i del menú, imprimir el grafo
//Función que imprime el grafo utilizando num_vertices para saber cuántos vértices tiene
//y array_vertices para recuperar el vector de vértices y recorrerlo

void imprimir_grafo(grafo G) {
    tipovertice *VECTOR; //Para almacenar el vector de vértices del grafo
    int N; //número de vértices del grafo

    //Para recorrerla, simplemente vamos a recorrer la matriz de adyacencia
    N = num_vertices(G);
    VECTOR = array_vertices(G);

    int i, j;
    printf("El grafo actual es:\n");
    for (i = 0; i < N; i++) {
        //Imprimo el vértice
        printf("Vertice(%d): %s\n", i, VECTOR[i].nombre);
        //Chequeo sus arcos
        for (j = 0; j < N; j++)
            if (son_adyacentes(G, i, j))
                if(conexion_matriz(G,i,j).tipo=='t'){
                    printf("\t%s-->%.2f-->%s\n", VECTOR[i].nombre, conexion_matriz(G,i,j).dist, VECTOR[j].nombre);
                }else{
                    printf("\t%s~~>%.2f~~>%s\n", VECTOR[i].nombre, conexion_matriz(G,i,j).dist, VECTOR[j].nombre);
                }
    }
}

//////////////////////////////////////////////////////////

void procesar_linea_ciudades(char* linea, grafo* G) {
    tipovertice vertice;
    char* token;
    char* rest = linea;  // Variable auxiliar para strtok_r

    // Token para el nombre
    token = strtok_r(rest, "\t", &rest);
    strcpy(vertice.nombre, token);
    strcpy(vertice.region, rest);

    insertar_vertice(G, vertice);
}


void cargarArchivoCiudades(char* nombre_archivo, grafo* G){
    // Abro el archivo
    FILE *archivo_personajes = fopen(nombre_archivo, "r");
    if (archivo_personajes == NULL){
        perror("Fallo al intentar abrir el archivo\n");
        exit(1);
    }

    char linea[1024];
    while (fgets(linea, sizeof(linea), archivo_personajes)) {
        // Eliminar el salto de línea si existe
        linea[strcspn(linea, "\n")] = 0;
        procesar_linea_ciudades(linea, G);
    }

    // Cierro el archivo
    fclose(archivo_personajes);
}


void procesar_linea_caminos(char* linea, grafo* G) {
    tipoconexiones conexion;
    tipovertice vertice;
    int pos1, pos2;
    char* token;
    char* rest = linea;  // Variable auxiliar para strtok_r

    token = strtok_r(rest, "\t", &rest);
    if (token != NULL) {
        strcpy(vertice.nombre, token);
        pos1 = posicion(*G, vertice);
    }
    
    token = strtok_r(rest, "\t", &rest);
    if (token != NULL) {
        strcpy(vertice.nombre, token);
        pos2 = posicion(*G, vertice);
    }
    
    // Token para si es de la realeza (royal)
    token = strtok_r(rest, "\t", &rest);
    if (token != NULL) {
        conexion.dist = atof(token);  // Convertimos el string a float
    }

    token = strtok_r(rest, "\t", &rest);
    if (token != NULL) {
        if(strcmp(token,"land")==0){
            conexion.tipo = 't';
        }else if(strcmp(token,"sea")==0){
            conexion.tipo = 'm';
        }else{
            perror("Error al añadir tipo de conexion");
        }
    }


    crear_arco(G, pos1, pos2,conexion.dist, conexion.tipo);
}


void cargarArchivoCaminos(char* nombre_archivo, grafo* G){
    // Abro el archivo
    FILE *archivo_personajes = fopen(nombre_archivo, "r");
    if (archivo_personajes == NULL){
        perror("Fallo al intentar abrir el archivo\n");
        exit(1);
    }

    char linea[1024];
    while (fgets(linea, sizeof(linea), archivo_personajes)) {
        // Eliminar el salto de línea si existe
        linea[strcspn(linea, "\n")] = 0;
        procesar_linea_caminos(linea, G);
    }

    // Cierro el archivo
    fclose(archivo_personajes);
}