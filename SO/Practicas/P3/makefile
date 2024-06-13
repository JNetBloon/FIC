# Angel Berreco Kolan angelkozlan@udc.es
# Jaime Camiña Caneda j.caneda@udc.es

##-----COMANDOS-----
## make -> Crea el ejecutable ./p3
## make valgrind -> Ejecuta la practica y muestra errores
## make clean -> Limpia la carpeta dejando los archivos originales

TARGET = p3

SOURCES = listas.c p3.c

OBJECTS = $(SOURCES:.c=.o)

VALGRIND_OPTIONS = --leak-check=full

#Esto crea un ejecutable (./p3)
executable: $(OBJECTS)

	gcc -Wall -o p3 listas.c p3.c #Esto es para que suelte todos los warnings correctamente
	gcc -Wall -o $(TARGET) $(OBJECTS) #Esto es para que cree el ejecutable

all: execute

execute: executable
	./$(TARGET)

#Esto ejecuta el programa y luego te informa de la cantidad de fugas de memoria (make valgrind)
valgrind: executable
	valgrind $(VALGRIND_OPTIONS) ./$(TARGET)

#Borra los -o y el ejecutable (make clean)
clean:
	rm -f $(TARGET) $(OBJECTS)

.PHONY: clean

