
#ifndef DYNAMIC_LIST_H
#define DYNAMIC_LIST_H

#include <stdio.h>
#include <stdbool.h>
#include "types.h"

#define LNULL NULL

typedef struct tNode* tPosL;

struct tNode{
    tItemL content;
    tPosL next;
};

typedef tPosL tList;

void createEmptyList(tList* L);
/*FUNCIÓN DE CREACIÓN DE UNA LISTA
 * Objetivo: Preparar una lista para poder recibir elementos
 * Entradas: Una lista con contenido indefinido
 * Salidas: Una lista preparada para operar con los datos
 */
bool isEmptyList(tList L);
/*FUNCIÓN DE PREGUNTA DE LISTA VACÍA
 * Objetivo: Comprueba si existe algún elemento para saber si está vacía o no
 * Entradas: Una lista de tipo tList
 * Salidas: Booleano true/false
 */
tPosL first(tList L);
/*FUNCIÓN PRIMER ELEMENTO
 * Objetivo: Devolver el primer puntero al que apunta la lista
 * Entradas: Una lista de tipo tList
 * Salidas: El puntero con el primer elemento
 */
tPosL last(tList L);
/*FUNCIÓN ÚLTIMO ELEMENTO
 * Objetivo: Devolver el último puntero al que apunta la lista
 * Entradas: Una lista de tipo tList
 * Salidas: El puntero con el último elemento
 */
tPosL next(tPosL p, tList L);
/*FUNCIÓN ELEMENTO SIGUIENTE
 * Objetivo: Devolver el siguiente puntero al que apunta el puntero actual
 * Entradas: La posición actual a la que apunta y la lista
 * Salidas: Posición siguiente
 */
tPosL previous(tPosL p, tList L);
/*FUNCIÓN ELEMENTO ANTERIOR
 * Objetivo: Devolver el anterior puntero al que le apunta el actual en la lista
 * Entradas: La posición actual a la que apunta y la lista
 * Salidas:Posición anterior
 */
bool insertItem(tItemL d, tPosL p, tList* L);
/*FUNCIÓN INSERTAR ITEM
 * Objetivo: Insertar un item y saber si fue posible su inserción o no
 * Entradas: Un elemento a introducir, la posición del puntero, y la lista a modificar
 * Salidas: Un booleano que afirma si fue posible o no la inserción
 * Precondición: Que la lista no esté llena
 * Postcondición: Si introduces un NULL lo guarda en la primera posición que pueda
 */
void deleteAtPosition(tPosL p, tList* L);
/*FUNCIÓN ELIMINAR ELEMENTO
 * Objetivo: Eliminar un nodo de forma segura haciendo que el anterior apunte al siguiente para poder liberar memoria del actual
 * Entradas: La posición del puntero y la lista
 */
tItemL getItem(tPosL p, tList L);
/*FUNCIÓN OBTENER CONTENIDO DE LA LISTA
 * Objetivo: Obtener la información de un item (El contenido a donde apuntamos)
 * Entradas: La posición a buscar y la lista
 * Salidas: El item con el que se va a tratar
 */
void updateItem(tItemL d, tPosL p, tList* L);
/*FUNCIÓN ACTUALIZAR ITEM
 * Objetivo: Actualizar la información modificada en la posición del puntero
 * Entradas: Un elemento del contenido de un puntero, la posición del puntero, y la lista a modificar
 */
tPosL findItem(tParticipantName d, tList L);
/*FUNCIÓN BUSCAR ITEM
 * Objetivo: Conocer la posición en la que se encuentre el item
 * Entradas: Nombre del participante y la lista
 * Salidas: Posición del puntero donde esta el item
 * Precondición: La lista no debe estar vacía
 */

#endif
