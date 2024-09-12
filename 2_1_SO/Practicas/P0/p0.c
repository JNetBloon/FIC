
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>

#include "listas.h"


#include <sys/utsname.h>


#define MAX 100
#define MAXCN 16
#define MAXOPEN 100

void error(const char* caderror) {
    perror(caderror);
    usleep(100000); // Espera 100.000 microsegundos (0.1 segundos) para que el mensaje de error se muestre antes de continuar.
}

struct ProcessList { //Definicion del struct ProcessList. Sera el encargado de almacenar el nombre y el modo de los procesos que se estan ejecutando
    char name[256];
    int mode;
};

void startProcessList(struct ProcessList PList[]){ //Inicializa el struct mencionado en la anterior funcion para evitar errores en la ejecucion.
    int fd=0, i=3;

    PList[0].mode = fcntl(fd, F_GETFL);    //Aqui asiganmos los 3 procesos básicos, que son la entrada estandar, salida estandar y error estandar
    strcpy(PList[0].name, "entrada estandar");

    fd++;
    PList[1].mode = fcntl(fd, F_GETFL);
    strcpy(PList[1].name, "salida estandar");

    fd++;
    PList[2].mode = fcntl(fd, F_GETFL);
    strcpy(PList[2].name, "error estandar");

    while (i < MAXOPEN) {
        strcpy(PList[i].name, ""); //En nuestro programa, trataremos un proceso como inexistente si en la posicion x no hay un nombre asignado
        i++;
    }
}

const char* whichMode(int mode) { //Con esta funcion identificamos el mode, es decir, en base a la cadena de numeros que recibimos, las comparamos e imprimimos el mode correspondiente en texto

    char* str;

    if (mode & O_WRONLY) {
        str = "O_WRONLY";
    } else if (mode & O_RDWR) {
        str = "O_RDWR";
    } else if (mode & O_APPEND) {
        str = "O_APPEND";
    } else if (mode & O_CREAT) {
        str = "O_CREAT";
    } else if (mode & O_EXCL) {
        str = "O_EXCL";
    } else if (mode & O_TRUNC) {
        str = "O_TRUNC";
    } else if (!(mode & O_WRONLY) && !(mode & O_RDWR)) { //Solo lectura es cuando no se cumple ninguna de estas dos condiciones
        str = "O_RDONLY";
    } else {
        str="Unknown";
    }

    return str;
}

void listopen(struct ProcessList PList[]) {  //Funcion que nos abre la lista mediante un bucle while
    int i=0;


    while(i<MAXOPEN){
        if(strcmp(PList[i].name, "")!=0){ //Si existe un nombre asignado a esa posicion, imprime su informacion
            printf("Descriptor %d -> %s %s\n", i, PList[i].name, whichMode(PList[i].mode));
        }
        i++;
    }
}

int thisFileExists(const char* filename) { //Comprueba la existencia de un archivo
    return access(filename, F_OK) != -1;
}

void add_open_file(int mode, char *filename, struct ProcessList PList[]) { //Con esta funcion añadimos el proceso que queramos abrir a la lista de procesos abriertos
    int i = 3;

    if (!thisFileExists(filename)) { //Comprueba si no existe el archivo
        printf("El archivo %s no existe.\n", filename);
        return;
    }

    while (i < MAXOPEN) { //Añadimos el proceso a la lista
        if (strcmp(PList[i].name, "") == 0) {
            strncpy(PList[i].name, filename, strlen(filename)); //strncy sirve para copiar de forma segura, para ello reservamos la memoria que vaya a ocupar la cadena
            PList[i].name[strlen(filename)] = '\0'; //Nos aseguramos que al final de la cadena haya un \0 para que deje de leer al llegar a este caracter
            PList[i].mode = mode; //Por ultimo mete el modo
            printf("Añadida entrada a la tabla ficheros abiertos: %s\n", PList[i].name); //Notifica al usuario si hubo exito
            return;
        }
        i++;
    }
    error("Imposible abrir fichero: No such file or directory");
}


void remove_open_file(int df, struct ProcessList PList[]){ //Funcion que elimina los preocesos que deseemos de la lista de procesos

    if (strcmp(PList[df].name, "")!=0) { //Si el proceso que se quiere eliminar no existe en el struct, no hay nada que borrar
        strcpy(PList[df].name, "");
        PList[df].mode = 0;
    }
}

void dup_open_file(int mode, char *filename, struct ProcessList PList[]){ //Fucion que duplica los procesos que seleccionemos.
    int i = 3;

    while (i < MAXOPEN) {
        if(strcmp(PList[i].name, "")==0) { //Al igual que en open_fil
            strncpy(PList[i].name, filename, strlen(filename) + 1);
            PList[i].name[strlen(PList[i].name)] = '\0';
            PList[i].mode = mode;
            printf("Duplicada la entrada a la tabla ficheros abiertos\n");
            return;
        }
        i++;

    }
    error("Imposible duplicar descriptor: Bad file descriptor");
}

void Cmd_open (char * tr[], struct ProcessList PList[]) { //Comando "open", abre un nuevo proceso
    int i, df, mode = 0;
    if (tr[1] == NULL) { //Si no hay ningun argumento se interpreta que se quiere imprimir la lista
        listopen(PList);
        return;
    }
    for (i = 1; tr[i] != NULL; i++) //Si existe un argumento, lo comparamos con un OR con los modos para ver si es uno de ellos
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;

    df = open(tr[0], mode, 0777); //Abre el proceso
    if (df == -1) //df descriptor del fichero que pones por terminal
        error("Imposible abrir fichero: No such file or directory");
    else {
        add_open_file(mode, tr[1], PList); //Si todo va bien, metemos la informacion al struct
    }
}

void Cmd_close (char *tr[],struct ProcessList PList[]) {
    int df;

    if (tr[1]==NULL || (df=atoi(tr[1]))<0) {
        listopen(PList);
        return;
    }

    if (close(df)==-1)
        error("Imposible cerrar descriptor: Bad file descriptor");
    else
        remove_open_file(df, PList); //tr[0] es el nombre del archivo
}

void Cmd_dup (char * tr[], struct ProcessList PList[]) {
    int df;
    char aux[200], *p;

    if (tr[1]==NULL || (df=atoi(tr[1]))<0) {
        listopen(PList);
        return;
    }

    if (!thisFileExists(PList[df].name)) {
        error("Imposible duplicar descriptor: Bad file descriptor");
        return;
    }

    if(strcmp(PList[df].name,"")!=0) {
        p = PList[df].name;
        sprintf(aux, "dup %d (%s)", df, p);
        df = open(PList[df].name, PList[df].mode, 0777);
        dup_open_file(PList[df].mode, aux, PList);
    }
}

void infosys() {
    struct utsname unameData;
    if (uname(&unameData) != -1) {
        printf("Usuario %s [%s]\nSistema %s\nKernel %s\nArquitectura de maquina %s\n", unameData.nodename, unameData.sysname, unameData.version, unameData.release, unameData.machine);
        //LD PRELOAD=../../fake/libfakearmy71.so ./shell.out
    } else {
        printf("No se pudo acceder a los datos del sistema");
    }
}

void CHdir(char *argumento){

    char directorio[216];
    char resultado[512];
    int size = sizeof(directorio);

    getcwd(directorio,size);

    if(argumento != NULL){
        snprintf(resultado, sizeof(resultado), "%s/%s", directorio, argumento);


        if(chdir(resultado)==0){
            strcpy(directorio,resultado);
        }else if(strcmp(argumento, "..")==0){
            for (int i = size-1; i>=0 ; --i) {
                if(directorio[i]=='/'){
                    memmove(directorio + i, directorio + i +1, size-1);
                    directorio[size-1]='\0';
                    i=-1;
                }


            }
            printf("%s\n",directorio);
        } else {
            printf("Imposible cambiar directorio: No such file or directory\n");
        }

    }else{
        getcwd(directorio,size);
        printf("%s\n",directorio);
    }
}

struct CommandsStruct {
    char commands[MAXCN][10]; // Un array de números con un tamaño máximo de 10 elementos
    char info[MAXCN][200]; // Un array de cadenas de caracteres con un tamaño máximo de 10 elementos y cada cadena puede contener hasta 50 caracteres
};

void StartStruct(struct CommandsStruct *CoSt){

    strcpy(CoSt->commands[0], "authors");
    strcpy(CoSt->info[0], "Prints the names and logins of the program authors. authors -l prints only the logins and authors -n prints only the names");
    strcpy(CoSt->commands[1], "pid");
    strcpy(CoSt->info[1], "Prints the pid of the process executing the shell. pid -p rints the pid of the shell s parent process.");
    strcpy(CoSt->commands[2], "chdir");
    strcpy(CoSt->info[2], "Changes the current working directory of the shell to dir (using the chdir system call). When invoked without auguments it prints the current working directory.");
    strcpy(CoSt->commands[3], "date");
    strcpy(CoSt->info[3], "Prints the current date in the format DD/MM/YYYY");
    strcpy(CoSt->commands[4], "time");
    strcpy(CoSt->info[4], "Prints the current time in the format hh:mm:ss.");
    strcpy(CoSt->commands[5], "hist");
    strcpy(CoSt->info[5], "Shows/clears the historic of commands executed by this shell. -hist Prints all the comands that have been input with their order number -hist -c Clears (empties) the list of historic commands -hist -N Prints the first N comands");
    strcpy(CoSt->commands[6], "comand");
    strcpy(CoSt->info[6], "Repeats command number N (from historic list)");
    strcpy(CoSt->commands[7], "open");
    strcpy(CoSt->info[7], "Opens a file and adds it (together with the file descriptor and the opening mode to the list of shell open files");
    strcpy(CoSt->commands[8], "close");
    strcpy(CoSt->info[8], "Closes the df file descriptor and eliminates the corresponding item from the list");
    strcpy(CoSt->commands[9], "dup");
    strcpy(CoSt->info[9], "Duplicates the df file descriptor (using the dup system call, creating the corresponding new entry on the file list");
    strcpy(CoSt->commands[10], "listopen");
    strcpy(CoSt->info[10], "Lists the shell open files. For each file it lists its descriptor, the file name and the opening mode. The shell will inherit from its parent process open descriptors 0, 1 and 2.");
    strcpy(CoSt->commands[11], "infosys");
    strcpy(CoSt->info[11], "Prints information on the machine running the shell (as obtained via the uname system call/library function)");
    strcpy(CoSt->commands[12], "quit");
    strcpy(CoSt->info[12], "Ends the shell");
    strcpy(CoSt->commands[13], "exit");
    strcpy(CoSt->info[13], "Ends the shell");
    strcpy(CoSt->commands[14], "bye");
    strcpy(CoSt->info[14], "Ends the shell");
    strcpy(CoSt->commands[15], "help");
    strcpy(CoSt->info[15], "Help displays a list of available commands. help cmd gives a brief help on the usage of comand cmd");
}

void help(char *argumento, struct CommandsStruct CoSt){
    bool exist=false;
    if(argumento==NULL){
        for(int i=0; i<MAXCN; i++){
            printf("%s  ", CoSt.commands[i]);
        }
        printf("\n");
    } else {
        for(int i=0; i<MAXCN; i++){
            if(strcmp(argumento, CoSt.commands[i])==0){
                printf("%s\n", CoSt.info[i]);
                exist=true;
            }
        }
        if(!exist)
            printf("%s", "Comando no encontrado. Usa help para ver la lista.\n");
    }
}

void hist(char *argumento, tList list, bool *resetList) {
    int num;

    if (argumento != NULL) {
        if(strcmp("-c", argumento) == 0)
            *resetList = true;
        else if (argumento[0] == '-'){
            for (int i = 1; i<= strlen(argumento); i++) {
                argumento[i-1] = argumento[i];
            }
            num = (int)strtol(argumento, NULL, 10);
            if(num>0)
                PrintNList(num, list);
        }
    } else {
        PrintList(list);
    }
}


void DateTime(char *comando){
    if(strcmp(comando,"date")==0){
        time_t tac;// Declaramos la variable "tac" del tipo "time_t" para almacenar informacion sobre el tiempo
        struct tm *inft; //Declaramos un puntero a la estructura "tm" llamado "inft". La estructura se utiliza para descomponer la fecha en sus componentes individuales (dia,mes,año), aunque tambien las horas, segundos etc, pero aqui no nos interesan
        char cadena[11]; //Tamaño para dd/mm/yyyy + caracter nulo

        time(&tac); //Se utiliza la función time() para obtener el tiempo actual en segundos desde el 1 de enero de 1970 (época Unix) y se almacena en la variable tac
        inft = localtime(&tac); //Se utiliza la función localtime() para convertir el tiempo en tac en una estructura tm local y se almacena en el puntero inft
        strftime(cadena, sizeof(cadena), "%d/%m/%Y", inft); //Se utiliza la función strftime() para formatear la fecha y hora almacenada en inft en la cadena "cadena" . El formato "%d/%m/%Y" indica que se desea obtener la fecha en el formato "dd/mm/yyyy".
        printf("%s\n", cadena);

    } else {
        time_t tac;
        struct tm *inft;

        time(&tac);
        inft = localtime(&tac);
        int hora = inft->tm_hour;  //Guardamos la variables hora, minutos y segundos en sus respectivos sitios
        int minutos = inft->tm_min;
        int segundos = inft->tm_sec;
        printf("Hora actual: %02d:%02d:%02d\n", hora, minutos, segundos);

    }
}

void pid(char *argumento){
    if((argumento != NULL) && (strcmp(argumento, "-p")==0)) { //Si no tiene argumento imprime el PID del shell, sino imprime el del padre
        printf("PID PADRE\n");
        //printf("El PID de este proceso es: %d\n", getppid());     SOLO LINUX
    } else {
        printf("El PID de este proceso es: %d\n", getpid());
    }
}

void authors(char *argumento){
    if(argumento != NULL){ //Si no hay argumento imprime los nombre y el login
        if(strcmp(argumento, "-l")==0){ //Imprimir login
            printf("Usuario\n");
        } else if(strcmp(argumento, "-n")==0){ //Imprimir los nombres
            printf("Correo\n");
        }
    } else {
        printf("Usuario\n");
        printf("Correo\n");
    }
}

int intruccion(int maxtr, char * tr[maxtr], tList list, struct CommandsStruct CoSt, struct ProcessList PList[]) {

    bool resetList = false; //Auxiliar para saber si hay que borrar la lista. Se hace en el main para evitar errores

    if(strcmp(tr[0], "exit")==0 ||strcmp(tr[0], "bye")==0||strcmp(tr[0], "quit")==0) { //Salir del programa
        return 0;
    } else if(strcmp(tr[0], "authors")==0){ //Autores del programa
        authors(tr[1]);
    } else if(strcmp(tr[0], "comand")==0) { //Historial del programa, usa LISTA
        //comand(tr[1], list);
        return 2;
    } else if(strcmp(tr[0], "hist")==0) { //
        hist(tr[1], list, &resetList);
        if(resetList)
            return 1;
    } else if(strcmp(tr[0], "pid")==0) { //Pide el numero del proceso actual del sheel y del padre
        pid(tr[1]);
    } else if(strcmp(tr[0], "date")==0 || strcmp(tr[0], "time")==0) { //Imprime date/time dependiendo del comando, las dos estan en la misma funcion
        DateTime(tr[0]);
    } else if(strcmp(tr[0], "help")==0) { //Pide el numero del proceso actual del sheel y del padre
        help(tr[1], CoSt);
    }  else if(strcmp(tr[0], "infosys")==0) { //Pide el numero del proceso actual del sheel y del padre
        infosys();
    } else if(strcmp(tr[0], "chdir")==0) {
        CHdir(tr[1]);
    } else if(strcmp(tr[0], "open")==0) {
        Cmd_open(tr, PList);
    } else if(strcmp(tr[0], "close")==0) {
        Cmd_close(tr, PList);
    } else if(strcmp(tr[0], "dup")==0) {
        Cmd_dup(tr, PList);
    } else if(strcmp(tr[0], "listopen")==0) {
        listopen(PList);
    } else {
        error("No ejecutado: No such file or directory");
    }

    return 3; //Retorna un numero que no ejecuta nada al salir para que el programa siga
}

int partir(char input[MAX], char * tr[]){ //Sacado de la ayuda de SO
    int i=1;
    if ((tr[0]=strtok(input," \n\t"))==NULL)
        return 0;
    while ((tr[i]=strtok(NULL," \n\t"))!=NULL)
        i++;
    return i;
}


int main() {
    char input[MAX];
    bool exit = false;
    char * tr[MAX];
    int maxtr;

    tList list;
    createList(&list); //Todo lo referido a la lista

    int checker; //Interpreta el numero que retorna la instruccion
    struct CommandsStruct CoSt;
    StartStruct(&CoSt); //Crea un struct con todos los comandos

    struct ProcessList PList[MAXOPEN];
    startProcessList(PList);

    while (exit==false) {

        printf("--%c", (char) 62);
        fgets(input, sizeof(input), stdin); //Pide introducir en teclado la instruccion

        if (strcmp(input, "\n") != 0) { //Evita meter nulls al programa o cadenas vacias

            if(!insertElement(input, &list))
                error("No ejecutado: No such file or directory");

            maxtr = partir(input, tr); //Parte la instruccion en cachos tr[MAX]


            checker = intruccion(maxtr, tr, list, CoSt, PList);
            if (checker == 0) { //Si retorna 0 es un exit, bye o quit, asi que sale del programa
                exit = true;
            } else if (checker == 1) { //Si retorna 1 se borra el contenido de la lista
                deleteList(&list);
            } else if (checker == 2) {
                strcpy(input, getElement(tr[1], list));
                maxtr = partir(input, tr);
                if(strcmp(tr[0], "comand")==0)
                    printf("Comand not valid\n");
                intruccion(maxtr, tr, list, CoSt, PList);
            }

        }
    }

    //ANTES DE FINALIZAR ELIMINAR TODOS LOS PROCESOS DEL LISTOPEN
    deleteList(&list);

    return 0;
}
