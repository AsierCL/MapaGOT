#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "grafo.h"

//FUNCIONES DEL PROGRAMA DE PRUEBA DE GRAFOS

typedef struct{
    int prev;
    char nombre[MAXNOMBRES];
    char tipo;
} tipoprevio;

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


/////////////////////////////////////////////////////////////////////////////

/* 
DESDE k=0; k<N; k++ //Analizamos matriz Dk
    DESDE i=0;i<N;i++ //arco ik
        DESDE j=0;j<N;j++ //arco kj
            SI D(i,j)>D(i,k)+D(k,j) ENTONCES
                D(i,j)=D(i,k)+D(k,j)
                P(i,j)=P(k,j)
            FIN_SI
        FIN_DESDE
    FIN_DESDE
FIN_DESDE */



void floyd_warshall_distancia(grafo G, tipoconexiones dist[MAXVERTICES][MAXVERTICES], tipoprevio pred[MAXVERTICES][MAXVERTICES], char opcion) {
    int i, j, k, vertices = num_vertices(G);
    tipovertice *arrayvertices = array_vertices(G);
    float tierra, mar;
    switch (opcion)
    {
    case 'n':
        tierra=5.5;
        mar=11.25;
        break;
    case 'd':
        tierra=80.0;
        mar=80.0;
        break;
    case 'c':
        tierra=1.0;
        mar=1.0;
        break;
    default:
        printf("ERROR\n");
        break;
    }
    
    // Inicialización de las matrices de distancias y predecesores
    for (i = 0; i < vertices; i++) {
        for (j = 0; j < vertices; j++) {
            if (i == j) {
                dist[i][j].dist = 0; // La distancia a sí mismo es 0
                dist[i][j].tipo = '-';

                pred[i][j].prev = i; // Predecesor de sí mismo
                strcpy(pred[i][j].nombre,arrayvertices[i].nombre); // Predecesor de sí mismo
                pred[i][j].tipo = '-'; // Predecesor de sí mismo
            } else {
                tipoconexiones conexion = conexion_matriz(G, i, j);
                if (conexion.dist > 0) { // Existe una conexión
                    dist[i][j] = conexion;
                    if(dist[i][j].tipo!='t'){
                        dist[i][j].dist /= tierra;
                    }else{
                        dist[i][j].dist /= mar;
                    }
                    pred[i][j].prev = i;
                    strcpy(pred[i][j].nombre,arrayvertices[i].nombre);
                    pred[i][j].tipo = conexion.tipo;
                } else { // No hay conexión
                    dist[i][j].dist = INFINITY;
                    dist[i][j].tipo = '-';
                    pred[i][j].prev = -1;
                }
            }
        }
    }

    // Algoritmo de Floyd-Warshall
    for (k = 0; k < vertices; k++) {
        for (i = 0; i < vertices; i++) {
            for (j = 0; j < vertices; j++) {
                if (dist[i][k].dist + dist[k][j].dist < dist[i][j].dist) {
                    dist[i][j].dist = dist[i][k].dist + dist[k][j].dist;
                    pred[i][j] = pred[k][j];
                }
            }
        }
    }
}

// Imprime el camino mínimo desde 'start' hasta 'end' usando la matriz de predecesores 'pred'.
// Devuelve 1 si hay un camino y 0 si no lo hay.
int imprimir_camino(int pred[MAXVERTICES][MAXVERTICES], int start, int end) {
    if (pred[start][end] == -1) { // No hay camino entre start y end
        printf("No existe un camino entre los nodos %d y %d.\n", start, end);
        return 0;
    }

    // Array para almacenar el camino mínimo en orden inverso
    int camino[MAXVERTICES];
    int n = 0; // Índice para el camino

    // Recorremos desde el destino hacia el origen
    int actual = end;
    while (actual != start) {
        camino[n++] = actual;
        actual = pred[start][actual];
    }
    camino[n++] = start; // Añadimos el origen al final del camino

    // Imprimir el camino en el orden correcto (desde 'start' a 'end')
    printf("Camino mínimo de %d a %d: ", start, end);
    for (int i = n - 1; i >= 0; i--) {
        printf("%d ", camino[i]);
        if (i > 0) printf("-> ");
    }
    printf("\n");
    return 1;
}

// Función auxiliar para reconstruir el camino a partir de la matriz de predecesores
void reconstruir_camino(tipoprevio pred[MAXVERTICES][MAXVERTICES], int inicio, int fin) {
    if (inicio != fin){
        reconstruir_camino(pred, inicio, pred[inicio][fin].prev);
        if(pred[pred[inicio][fin].prev][fin].tipo = 't'){
            printf("-> ");
        }else if(pred[pred[inicio][fin].prev][fin].tipo = 'm'){
            printf("~> ");
        }else{
            printf("Error");
        }
    }
    printf("%s ", pred[fin][fin].nombre);
}

void camino_mas_corto(grafo G, char opcion) {
    tipovertice v1, v2;
    int pos1, pos2;

    printf("Buscar el camino más corto:\n");
    
    // Ciudad origen
    printf("Introduce la ciudad origen: ");
    scanf(" %[^\n\r]", v1.nombre);
    if (!existe_vertice(G, v1)) {
        printf("El vertice %s no existe en el grafo\n", v1.nombre);
        return;
    }
    
    // Ciudad destino
    printf("Introduce la ciudad destino: ");
    scanf(" %[^\n\r]", v2.nombre);
    if (!existe_vertice(G, v2)) {
        printf("El vertice %s no existe en el grafo\n", v2.nombre);
        return;
    }

    // Obtenemos las posiciones de los vértices en el grafo
    pos1 = posicion(G, v1);
    pos2 = posicion(G, v2);
    
    // Matriz de distancias y predecesores
    tipoconexiones dist[MAXVERTICES][MAXVERTICES];
    tipoprevio pred[MAXVERTICES][MAXVERTICES];

    ///////////////////////OPCION AQUI//////////////////////////

    // Calculamos el camino más corto para todas las parejas de vértices
    floyd_warshall_distancia(G, dist, pred, opcion);

    // Si no hay un camino entre las ciudades
    if (dist[pos1][pos2].dist == INFINITY) {
        printf("No existe un camino entre %s y %s.\n", v1.nombre, v2.nombre);
        return;
    }

    // Mostramos la distancia más corta
    printf("La distancia más corta entre %s y %s es %.2f.\n", v1.nombre, v2.nombre, dist[pos1][pos2].dist);

    // Mostramos el camino más corto utilizando la matriz de predecesores
    printf("Ruta: ");
    reconstruir_camino(pred, pos1, pos2);
    printf("\n");
}