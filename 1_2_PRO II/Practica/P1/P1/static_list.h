
#ifndef STATIC_LIST_H
#define STATIC_LIST_H

#define MAX 25
#include "types.h"

#define LNULL -1
typedef int tPosL;

typedef struct{
    tItemL content[MAX];
    tPosL lastposition;
}tList;

//FUNCIONES

void createEmptyList(tList* L);
/*FUNCIÓN DE CREACIÓN DE UNA LISTA
 * Objetivo: Preparar una lista para poder recibir elementos
 * Entradas: Una lista con contenido indefinido
 * Salidas: Una lista inicializada
 */
bool isEmptyList(tList L);
/*FUNCIÓN DE PREGUNTA DE LISTA VACÍA
 * Objetivo: Comprueba si existe algún elemento para saber si está vacía o no
 * Entradas: Una lista con contenido indefinido
 * Salidas: Booleano true/false
 */
tPosL first(tList L);
/*FUNCIÓN PRIMER ELEMENTO
 * Objetivo: Devolver la primera posición de la lista
 * Entradas: Una lista de tipo tList
 * Salidas: La posición del primer elemento
 */
tPosL last(tList L);
/*FUNCIÓN ÚLTIMO ELEMENTO
 * Objetivo: Devolver la última posición de la lista
 * Entradas: Una lista de tipo tList
 * Salidas: La posición del último elemento
 */
tPosL next(tPosL p, tList L);
/*FUNCIÓN ELEMENTO SIGUIENTE
 * Objetivo: Devolver la siguiente posición en el array
 * Entradas: La posición actual en el array y la lista
 * Salidas: Posición siguiente
 */
tPosL previous(tPosL p, tList L);
/*FUNCIÓN ELEMENTO ANTERIOR
 * Objetivo: Devolver la anterior posición en el array
 * Entradas: La posición actual en el array y la lista
 * Salidas:Posición anterior
 */
bool insertItem(tItemL d, tPosL p, tList* L);
/*FUNCIÓN INSERTAR ITEM
 * Objetivo: Insertar un item y saber si fue posible su inserción o no
 * Entradas: Un elemento del array, la posición en el array, y la lista a modificar
 * Salidas: Un booleano que afirma si fue posible o no la inserción
 * Precondición: Que la lista no esté llena
 * Postcondición: Si introduces un NULL lo guarda en la primera posición que pueda
 */
void deleteAtPosition(tPosL p, tList* L);
/*FUNCIÓN ELIMINAR ELEMENTO
 * Objetivo: Eliminar un elemento de un array, y mover los siguientes una posición atrás (Ahorro de memoria)
 * Entradas: La posición en el array y la lista
 */
tItemL getItem(tPosL p, tList L);
/*FUNCIÓN OBTENER CONTENIDO DE LA LISTA
 * Objetivo: Obtener la información de un item (La posición del array)
 * Entradas: La posición a buscar y la lista
 * Salidas: El item con el que se va a tratar
 */
void updateItem(tItemL d, tPosL p, tList* L);
/*FUNCIÓN ACTUALIZAR ITEM
 * Objetivo: Actualizar la información modificada en la posición del array
 * Entradas: Un elemento del array, la posición en el array, y la lista a modificar
 */
tPosL findItem(tParticipantName, tList);
/*FUNCIÓN BUSCAR ITEM
 * Objetivo: Conocer la posición en la que se encuentre el item
 * Entradas: Nombre del participante y la lista
 * Salidas: Posición el array donde esta el item
 * Precondición: La lista no debe estar vacía
 */

#endif