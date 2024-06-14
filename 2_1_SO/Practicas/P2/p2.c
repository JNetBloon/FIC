
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h> //Es la de mkdir
#include <dirent.h> //Es para DIR
#include <libgen.h>
#include <stdlib.h>

#include <pwd.h> //Estas son del grupo y user
#include <grp.h>
#include <sys/utsname.h> //Obtener grupo y usuario del sistema

#include <dlfcn.h> //Para obtener la direccion de memoria de las funciones

//Librerias P2
#include <errno.h>
#include <sys/mman.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#define TAMANO 2048

//Definiciones base
#include "listas.h"
#define MAX 100
#define MAXCN 30
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
    error("Imposible abrir fichero");
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
    error("Imposible duplicar descriptor\n");
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
        error("Imposible abrir fichero\n");
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
        error("Imposible cerrar descriptor\n");
    else
        remove_open_file(df, PList); //tr[0] es el nombre del archivo
}

void Cmd_dup (char * tr[], struct ProcessList PList[]) {
    int df;
    char *p;
    char aux[300]; // Asegúrate de definir BUF_SIZE con el tamaño necesario


    if (tr[1]==NULL || (df=atoi(tr[1]))<0) {
        listopen(PList);
        return;
    }

    if (!thisFileExists(PList[df].name)) {
        error("Imposible duplicar descriptor");
        return;
    }

    if(strcmp(PList[df].name,"")!=0) {
        p = PList[df].name;
        snprintf(aux, sizeof(aux), "dup %d (%s)", df, p);
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
            error("Imposible cambiar directorio");
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
    //P1
    strcpy(CoSt->commands[16], "create");
    strcpy(CoSt->info[16], "create [-f] [name]    Crea un directorio o un fichero (-f)");
    strcpy(CoSt->commands[17], "stat");
    strcpy(CoSt->info[17], "stat [-long][-link][-acc] name1 name2 ..    lista ficheros;\n\t-long: listado largo\n-acc: acesstime\n\t-link: si es enlace simbolico, el path contenido");
    strcpy(CoSt->commands[18], "list");
    strcpy(CoSt->info[18], "list [-reca] [-recb] [-hid][-long][-link][-acc] n1 n2 ..    lista contenidos de directorios\n\t-hid: incluye los ficheros ocultos\n\t-recb: recursivo (antes)\n\t-reca: recursivo (despues)\nresto parametros como stat");
    strcpy(CoSt->commands[19], "delete");
    strcpy(CoSt->info[19], "delete [name1 name2 ..]    Borra ficheros o directorios vacios");
    strcpy(CoSt->commands[20], "deltree");
    strcpy(CoSt->info[20], "deltree [name1 name2 ..]    Borra ficheros o directorios no vacios recursivamente");
    //P2
    strcpy(CoSt->commands[21], "malloc");
    strcpy(CoSt->info[21], "malloc [-free] [tam]\tasigna un bloque memoria de tamano tam con malloc\n\t-free: desasigna un bloque de memoria de tamano tam asignado con malloc");
    strcpy(CoSt->commands[22], "shared");
    strcpy(CoSt->info[22], "shared [-free|-create|-delkey] cl [tam]\tasigna memoria compartida con clave cl en el programa\n\t-create cl tam: asigna (creando) el bloque de memoria compartida de clave cl y tamano tam\n\t-free cl: desmapea el bloque de memoria compartida de clave cl\n\t-delkey clelimina del sistema (sin desmapear) la clave de memoria cl");
    strcpy(CoSt->commands[23], "mmap");
    strcpy(CoSt->info[23], "mmap [-free] fich prm\tmapea el fichero fich con permisos prm\n\t-free fich: desmapea el ficherofich");
    strcpy(CoSt->commands[24], "read");
    strcpy(CoSt->info[24], "read fiche addr cont \tLee cont bytes desde fich a la direccion addr");
    strcpy(CoSt->commands[25], "write");
    strcpy(CoSt->info[25], "write [-o] fiche addr cont \tEscribe cont bytes desde la direccion addr a fich (-o sobreescribe)");
    strcpy(CoSt->commands[26], "memdump");
    strcpy(CoSt->info[26], "memdump addr cont \tVuelca en pantallas los contenidos (cont bytes) de la posicion de memoria addr");
    strcpy(CoSt->commands[27], "memfill");
    strcpy(CoSt->info[27], "memfill addr cont byte \tLlena la memoria a partir de addr con byte");
    strcpy(CoSt->commands[28], "mem");
    strcpy(CoSt->info[28], "mem [-blocks|-funcs|-vars|-all|-pmap] ..\tMuestra muestra detalles de la memoria del proceso\n\t\t-blocks: los bloques de memoria asignados\n\t\t-funcs: las direcciones de las funciones\n\t\t-vars: las direcciones de las variables\n\t\t:-all: todo\n\t\t-pmap: muestra la salida del comando pmap(o similar)");
    strcpy(CoSt->commands[29], "recurse");
    strcpy(CoSt->info[29], "recurse [n]\tInvoca a la funcion recursiva n veces");

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
                puts(CoSt.info[i]);
                exist=true;
            }
        }
        if(!exist)
            printf("Comando no encontrado. Usa help para ver la lista.\n");
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
        printf("El PID padre del proceso es: %d\n", getppid());
    } else {
        printf("El PID de este proceso es: %d\n", getpid());
    }
}

void authors(char *argumento){
    if(argumento != NULL){ //Si no hay argumento imprime los nombre y el login
        if(strcmp(argumento, "-l")==0){ //Imprimir login
            printf("j.caneda@udc.es\n");
            printf("angel.kozlan@udc.es\n");
        } else if(strcmp(argumento, "-n")==0){ //Imprimir los nombres
            printf("Jaime Caminha Caneda\n");
            printf("Angel Berreco Kozlan\n");
        }
    } else {
        printf("Jaime Caminha Caneda: j.caneda@udc.es\n");
        printf("Angel Berreco Kozlan: angel.kozlan@udc.es\n");
    }
}

//-------------------------------------------------------------------- P1

char LetraTF (mode_t m) {
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}

char * ConvierteModo2 (mode_t m) {
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

bool isEmptyDir(const char *nombre){ //Funcion auxiliar para saber si un directorio esta vacio o no
    int n = 0;
    struct dirent *d;
    DIR *dir = opendir(nombre);

    if (dir == NULL) { //Si es nulo es porque no consiguio abrirlo
        perror("Error al intentar acceder al directorio");
        return false;
    }

    while ((d = readdir(dir)) != NULL) {
        if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) { //"." es la carpeta en si y ".." la carpeta padre, asi que no las contamos
            n++;
        }
    }

    closedir(dir); //Se cierra el directorio al acabar de leerlo
    return (n==0); //Si no leyo ningun elemento, deberia devolver 0, true
}


void create(char *tr[]) { //Crea un archivo o directorio dependiendo del argumento
    char directorio[216];
    char *ruta;

    if(tr[1] != NULL){
        if(strcmp(tr[1], "-f")==0) //En este caso es un fichero
            if(tr[2] != NULL){
                FILE *archivo = fopen(tr[2], "w"); //abre el archivo y en caso de error lo notifica
                if (archivo == NULL)
                    error("Creacion de fichero fallida");
            } else
                CHdir(NULL); //Si no le especificas el nombre hace un chdir
        else {
            ruta = getcwd(directorio,sizeof(directorio));             //Proceso de obtencion de ruta + nombre directorio
            strcat(ruta, "/");
            strcat(ruta, tr[1]);

            if (mkdir(ruta, 0755) != 0) //Crea el directorio
                error("Creacion de carpeta fallida");
        }
    } else
        CHdir(NULL); //Si no tiene argumentos hace un chdir
}

void statAux(char tr[], bool isLink, bool isAcc, bool isLong, bool STAT){ //Función auxiliar a stat para poder ser usada de forma más sencilla con list
    struct stat archivo;

    if (stat(tr, &archivo) != 0) { //stat comprueba que se pueda acceder al directorio
        error("Archivo/directorio no encontrado");
        return;
    }

    if (!STAT) //Si la funcion es llamada desde stat hace una cosa, si no otra
        tr = basename(tr); //Borra todos los /home/... hasta el ultimo nombre

    if (isLong) { //Si es long Acc y Link funcionan de diferente forma
        char tiempo[30];
        nlink_t nlink = archivo.st_nlink;
        ino_t inodo = archivo.st_ino;
        gid_t grupo = archivo.st_gid;
        uid_t usuario = archivo.st_uid;
        struct group *gr = getgrgid(grupo);
        struct passwd *pw = getpwuid(usuario);
        char *username = pw->pw_name;
        char *groupname = gr->gr_name;

        if (!isLink && !isAcc) {
            time_t time = archivo.st_ctime;
            strcpy(tiempo, ctime(&time));
            tiempo[strlen(tiempo) - 1] = '\0'; //PARA QUE NO HAGA SALTO DE LINEA
            printf("%s %lu (%lu) %s %s %s %ld\t%s", tiempo, nlink, inodo, groupname, username, ConvierteModo2(archivo.st_mode), archivo.st_blksize, tr);
        } else if (isLink && !isAcc) {
            time_t time = archivo.st_ctime;
            strcpy(tiempo, ctime(&time));
            tiempo[strlen(tiempo) - 1] = '\0'; //PARA QUE NO HAGA SALTO DE LINEA
            printf("%s %lu (%lu) %s %s %s %ld\t%s", tiempo, nlink, inodo, groupname, username, ConvierteModo2(archivo.st_mode), archivo.st_blksize, tr);
        } else if (!isLink) {
            time_t time = archivo.st_atime;
            strcpy(tiempo, ctime(&time));
            tiempo[strlen(tiempo) - 1] = '\0'; //PARA QUE NO HAGA SALTO DE LINEA
            printf("%s %lu (%lu) %s %s %s %ld\t%s", tiempo, nlink, inodo, groupname, username, ConvierteModo2(archivo.st_mode), archivo.st_blksize, tr);
        } else { //Las dos son true
            time_t time = archivo.st_atime;
            strcpy(tiempo, ctime(&time));
            tiempo[strlen(tiempo) - 1] = '\0'; //PARA QUE NO HAGA SALTO DE LINEA
            printf("%s %lu (%lu) %s %s %s %ld\t%s", tiempo, nlink, inodo, groupname, username, ConvierteModo2(archivo.st_mode), archivo.st_blksize, tr);
        }
    } else if (isLink && !isAcc) { //Ya sabemos que long es false, asi que estos son los casos restantes
        blksize_t block_size = archivo.st_blksize;
        printf("\t%ld\t%s", block_size, tr);
    } else if (isAcc && !isLink) {
        blksize_t block_size = archivo.st_blksize;
        printf("\t%ld\t%s", block_size, tr);
    } else {
        blksize_t block_size = archivo.st_blksize;
        printf("\t%ld\t%s", block_size, tr);
    }
    printf("\n");
}



void STat(char * tr[]) { //Muestra información detallada sobre uno o más archivos o directorios
    int i, l;
    bool isLink = false, isAcc = false, isLong = false;
    struct stat archivo;

    if (tr[1] == NULL) {
        CHdir(NULL);
        return;
    }

    for (i = 1; tr[i] != NULL; i++) { //Almacena en booleanos los argumentos de la entrada
        if (strcmp(tr[i], "-link") == 0) {
            isLink = true;
        } else if (strcmp(tr[i], "-acc") == 0) {
            isAcc = true;
        } else if (strcmp(tr[i], "-long") == 0) {
            isLong = true;
        } else {
            break;
        }
    }

    l = i; //Almacena el punto donde se deja de poner parametros con "-"

    while (tr[i] != NULL) {
        if (stat(tr[i], &archivo) != 0) { //Si stat no puede acceder devuelve error
            error("Archivo/directorio no encontrado");
            return;
        }
        statAux(tr[i], isLink, isAcc, isLong, true); //Usamos la auxiliar
        i++;
    }
    if(l==i){ //Si l = i entonces no metimos ningun directorio. Tanto si tiene argumentos opcion como si no, muestra el chdir
        CHdir(NULL);
    }
}

void listAux(char tr[], char * nombre, bool isLink, bool isAcc, bool isLong, bool isHid, bool isReca, bool isRecb){
    char tipoArchivo; // Almacena el tipo de archivo (regular, directorio, etc.)
    char temp[250]; // Almacena la ruta canónica del directorio o archivo actual
    char subruta[250]; // Almacena la ruta completa del archivo o directorio a procesar
    char trtemp[250]; // Almacena la ruta temporal utilizada para llamadas recursivas

    int i=0;
    struct stat archivo;

    if (realpath(tr, temp) == NULL) { //Obtiene la ruta hasta el directorio
        perror("Error al obtener la ruta canónica");
        return;
    }

    if (stat(tr, &archivo) != 0) { //Si no puede acceder al archivo devuelve error
        error("Archivo/directorio no encontrado");
        return;
    }
    tipoArchivo = LetraTF(archivo.st_mode); //Obtiene un caracter que indica que tipo de archivo es

    if(isReca){ //Recursividad desde antes
        if (tipoArchivo == 'd') { //Directorio

            DIR *dir = opendir(temp);
            struct dirent *d;
            char * carpetas[MAX];
            int pos = 0;
            printf("************%s\n", nombre); //Imprime el nombre del directorio

            while ((d = readdir(dir)) != NULL) { //Recorre el directorio
                strcpy(subruta, temp); //Obtener ruta hasta el archivo que esta leyendo en ese momento
                strcat(subruta, "/");
                strcat(subruta, d->d_name);

                stat(subruta, &archivo); //Accede al archivo
                tipoArchivo = LetraTF(archivo.st_mode); //Obtiene su tipo

                if (isHid) { //Si quieres que muestre los archivos ocultos
                    if (tipoArchivo == 'd' && strcmp(d->d_name, "..")!=0 && strcmp(d->d_name, ".")!=0) { //Aun siendo ".." (Carpeta padre) y "." (Carpeta en si) archivos ocultos, no podemos incluirlos porque entraria en un bucle infinito padre-hijo
                        carpetas[pos] = strdup(d->d_name);
                        pos++;
                    }
                    statAux(subruta, isLink, isAcc, isLong, false); //Usa el auxiliar de stat para poder imprimir sus datos
                } else {
                    if(d->d_name[0] != '.'){ //Si no es oculto, ya que no queremos los archivos ocultos
                        if (tipoArchivo == 'd') { //Directorio
                            carpetas[pos] = strdup(d->d_name); //Almacena el nombre de la carpeta para hacer luego la recursion
                            pos++;
                        }
                        statAux(subruta, isLink, isAcc, isLong, false);
                    }
                }
            }

            while(i < pos) { //Recorre de forma recursiva todas las carpetas del directorio
                strcpy(trtemp, tr);
                strcat(trtemp, "/");
                strcat(trtemp, carpetas[i]);

                char tempNombre[150]; // Variable temporal para el nombre
                strcpy(tempNombre, nombre);
                strcat(tempNombre, "/");
                strcat(tempNombre, carpetas[i]);

                listAux(trtemp, tempNombre, isLink, isAcc, isLong, isHid, isReca, isRecb);
                i++;
            }

            closedir(dir); //Cierra el directorio luego de usarlo
        }
    } else if (isRecb) { //Recusividad desde despues
        if (tipoArchivo == 'd') { //Directorio

            DIR *dir = opendir(temp);
            struct dirent *d;
            char * carpetas[MAX];
            char * ficheros[MAX];
            int pos = 0;
            int pos2 = 0;

            while ((d = readdir(dir)) != NULL) { //HAce lo mismo que en -reca
                strcpy(subruta, temp);
                strcat(subruta, "/");
                strcat(subruta, d->d_name);

                stat(subruta, &archivo);
                tipoArchivo = LetraTF(archivo.st_mode);

                if (isHid) { //Pero esta vez almacena tanto carpetas como archivos
                    if (tipoArchivo == 'd' && strcmp(d->d_name, "..")!=0 && strcmp(d->d_name, ".")!=0) { //Directorio
                        carpetas[pos] = strdup(d->d_name);
                        pos++;
                    } else {
                        ficheros[pos2] = strdup(d->d_name);
                        pos2++;
                    }
                } else {
                    if(d->d_name[0] != '.'){
                        if (tipoArchivo == 'd') { //Directorio
                            carpetas[pos] = strdup(d->d_name);
                            pos++;
                        }
                        ficheros[pos2] = strdup(d->d_name);
                        pos2++;
                    }
                }
            }

            while(i < pos) { //De esta forma entra de forma recursiva en las carpetas, y si esta no tiene ninguna pasa al siguiente while
                strcpy(trtemp, tr);
                strcat(trtemp, "/");
                strcat(trtemp, carpetas[i]);

                char tempNombre[150]; // Variable temporal para el nombre
                strcpy(tempNombre, nombre);
                strcat(tempNombre, "/");
                strcat(tempNombre, carpetas[i]);

                listAux(trtemp, tempNombre, isLink, isAcc, isLong, isHid, isReca, isRecb);
                i++;
            }
            i=0;
            printf("************%s\n", nombre);
            while(i < pos2){ //Aqui sabemos que estamos en los mas bajo, asi que imprimimos el nombre del directorio y los elementos que contiene con todos sus datos (Con statAux)
                strcpy(trtemp, tr);
                strcat(trtemp, "/");
                strcat(trtemp, ficheros[i]);
                statAux(trtemp, isLink, isAcc, isLong, false);
                i++;
            }

            closedir(dir);
        }
    }
}

void LIst(char * tr[]) { //Lista los directorios
    int i;
    bool isLink = false, isAcc = false, isLong = false, isHid = false;
    bool isReca = false, isRecb = false;
    struct stat archivo;

    char tipoArchivo;
    char temp[250];
    char subruta[250];

    if (tr[1] == NULL) {
        CHdir(NULL);
        return;
    }

    for (i = 1; tr[i] != NULL; i++) { //Almacena en booleanos todos los argumentos
        if (strcmp(tr[i], "-link") == 0) {
            isLink = true;
        } else if (strcmp(tr[i], "-acc") == 0) {
            isAcc = true;
        } else if (strcmp(tr[i], "-long") == 0) {
            isLong = true;
        } else if (strcmp(tr[i], "-hid") == 0) {
            isHid = true;
        } else if (strcmp(tr[i], "-reca") == 0) {
            isReca = true;
            isRecb = false; //Escoge el ultimo que lee
        } else if (strcmp(tr[i], "-recb") == 0) {
            isRecb = true;
            isReca = false;
        } else {
            break;
        }
    }

    if (!isReca && !isRecb) {
        while (tr[i] != NULL) { //En el momento en el que deja de leer argumentos "-" pasa a leer los archivos/directorios
            if (stat(tr[i], &archivo) != 0) { //Si no puede acceder devuelve error
                error("Archivo/directorio no encontrado");
                return;
            }
            tipoArchivo = LetraTF(archivo.st_mode); //Almacena en un caracter el tipo de archivo

            if (tipoArchivo == 'd') { //Directorio
                if (realpath(tr[i], temp) == NULL) { //Obtiene la ruta
                    perror("Error al obtener la ruta canónica");
                }

                DIR *dir = opendir(temp);
                struct dirent *d;

                if(dir == NULL) { //Si es nulo es porque no lo abrio
                    error("Error durante la lectura (Imposible acceder directorio)");
                    return;
                }
                printf("************%s\n", tr[i]);
                while ((d = readdir(dir)) != NULL) { //Entra al directorio y lista todos sus elementos
                    strcpy(subruta, temp);
                    strcat(subruta, "/");
                    strcat(subruta, d->d_name);
                    if (isHid) { //Como solo lista, podemos acceder al padre y a la carpeta en si ".." y "."
                        statAux(subruta, isLink, isAcc, isLong, false);
                    } else { //Si no queremos los archivos ocultos, se los salta
                        if (d->d_name[0] != '.') {
                            statAux(subruta, isLink, isAcc, isLong, false);
                        }
                    }
                }
                closedir(dir); //Cierra el directorio luego de leerlo
            } else {
                strcat(temp, "/"); //Si no es un directorio, imprimimos sus datos sacando su ruta y mandandola a la auxiliar
                strcat(temp, tr[i]);
                statAux(temp, isLink, isAcc, isLong, false);
            }
            printf("\n");
            i++;
        }
    } else {
        while (tr[i] != NULL) { //Si no es una llamada recursiva, recorre el directorio e imprime los datos de sus archivos/directorios
            if (stat(tr[i], &archivo) != 0) {
                error("Archivo/directorio no encontrado");
                return;
            }
            tipoArchivo = LetraTF(archivo.st_mode);

            if (tipoArchivo == 'd') { // Directorio
                if (realpath(tr[i], temp) == NULL) {
                    perror("Error al obtener la ruta canónica");
                }
                listAux(temp, basename(temp), isLink, isAcc, isLong, isHid, isReca, isRecb);
            } else {
                statAux(tr[i], isLink, isAcc, isLong, false);
            }
            printf("\n");
            i++;
        }
    }

}


void delete(char *nombre) {
    // Esta función elimina un archivo o directorio

    struct stat archivo; // Estructura para almacenar información del archivo o directorio
    char tipoArchivo; // Almacena el tipo de archivo (regular, directorio, etc.)

    if (nombre != NULL) {
        if (lstat(nombre, &archivo) == 0) {
            tipoArchivo = LetraTF(archivo.st_mode); // Obtiene el tipo de archivo

            if (tipoArchivo == '-') {
                // Es un archivo regular
                off_t tamano = archivo.st_size; // Obtiene el tamaño del archivo
                if (tamano != 0) {
                    error("El archivo no está vacío"); // Verifica si el archivo está vacío
                    return;
                }
                if (unlink(nombre) != 0) {
                    error("Error al eliminar el archivo"); // Elimina el archivo regular
                }
            } else if (tipoArchivo == 'd') {
                // Es un directorio
                if (isEmptyDir(nombre)) {
                    if (rmdir(nombre) != 0) {
                        error("Error al eliminar el directorio"); // Verifica si el directorio está vacío y lo elimina
                    }
                } else {
                    error("El directorio no está vacío"); // Si el directorio no está vacío, muestra un mensaje de error
                }
            } else if (tipoArchivo == 'l') {
                // Es un link simbolico
                if (unlink(nombre) != 0) {
                    error("Error al eliminar el archivo"); // Elimina el link
                }

            } else {
                error("Archivo no reconocido"); // Si el tipo de archivo no es válido, muestra un mensaje de error
            }
        } else {
            error("Archivo/directorio no encontrado"); // Si el archivo o directorio no existe, muestra un mensaje de error
        }
    } else {
        CHdir(NULL); // Si el nombre es nulo, vuelve al directorio raíz
    }
}


void deltree(char *nombre) {
    // Esta función elimina un directorio y su contenido de manera recursiva.

    struct stat archivo; // Estructura para almacenar información del archivo o directorio
    char tipoArchivo; // Almacena el tipo de archivo (regular, directorio, etc.)
    char ruta[300]; // Almacena la ruta del directorio a eliminar

    if (nombre == NULL) {
        CHdir(NULL); // Si el nombre es nulo, devuelve al directorio actual
        return;
    }

    if (realpath(nombre, ruta) == NULL) {
        perror("Error al obtener la ruta canónica"); // Obtiene la ruta canónica del directorio a eliminar
    }

    if (stat(ruta, &archivo) != 0) {
        error("Error durante la eliminación (Acceso inválido)"); // Verifica si el archivo o directorio existe y es accesible
        return;
    }

    tipoArchivo = LetraTF(archivo.st_mode); // Obtiene el tipo de archivo

    if (tipoArchivo == 'd') {
        // Es un directorio
        DIR *dir = opendir(ruta); // Abre el directorio actual

        struct dirent *d;

        if (dir == NULL) {
            error("Error durante la eliminación (Imposible acceder directorio)"); // Verifica si se puede acceder al directorio
            return;
        }

        while ((d = readdir(dir)) != NULL) {
            if (strcmp("..", d->d_name) != 0 && strcmp(".", d->d_name) != 0) {
                // El primer elemento es el directorio padre y el segundo es el directorio actual, que se eliminará de forma recursiva
                char ruta_completa[PATH_MAX]; // Utiliza PATH_MAX para obtener la longitud máxima de una ruta
                snprintf(ruta_completa, PATH_MAX, "%s/%s", ruta, d->d_name);
                deltree(ruta_completa); // Llama recursivamente para eliminar archivos y subdirectorios
            }
        }

        closedir(dir); // Cierra el directorio después de procesarlo

        if (isEmptyDir(ruta)) {
            if (rmdir(ruta) != 0) {
                error("Error durante la eliminación (Imposible borrar directorio)"); // Verifica si el directorio está vacío y lo elimina
            }
        }

    } else if (tipoArchivo == '-') {
        // Es un archivo regular
        if (unlink(ruta) != 0) {
            error("Error durante la eliminación (Imposible borrar archivo)"); // Elimina el archivo regular
            return;
        }
    } else if (tipoArchivo == 'l') {
        // Es un link simbolico
        if (unlink(nombre) != 0) {
            error("Error durante la eliminación (Imposible borrar link)"); // Elimina el link
        }

    }
}

//-------------------------------------------------------------------- P2

void Recursiva (int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        Recursiva(n-1);
}

struct MemoryStruct {
    void* DirMem;    // Dirección de memoria asignada
    size_t size;             // Tamaño del bloque de memoria
    char allocTime[20];  // Hora de asignación
};

void freeAllMallocs(struct MemoryStruct MemStr[]) {
    for(int i=0; i<MAXOPEN; i++){
        if(MemStr[i].size != 0)
            free(MemStr[i].DirMem);
    }
}

void listMallocs(struct MemoryStruct MemStr[]){
    for (int i = 0; i < 10; ++i) {
        if(MemStr[i].size != 0) {
            printf("\t%p\t\t%zu %s %s\n", MemStr[i].DirMem, MemStr[i].size, MemStr[i].allocTime, "malloc");
        }
    }
}

void MAlloc (char * tr[], struct MemoryStruct MemStr[]) {
    int i;

    if (tr[1]==NULL || (strcmp(tr[1],"-free")==0 && tr[2]==NULL)) {
        printf("*****Lista de bloques asigandos malloc para el proceso %d\n", getpid());
        listMallocs(MemStr);
    } else if (strcmp(tr[1],"-free")==0) {
        int buscar = atoi(tr[2]);
        for(i=0; i<MAXOPEN; i++){
            if(MemStr[i].size == buscar) {
                free(MemStr[i].DirMem);
                MemStr[i].size = 0; // Marcar el bloque como no asignado
                break;
            }
        }
        if (i == MAXOPEN) {
            printf("Tamaño de memoria no encontrado\n");
        }
    } else {
        int tamano = atoi(tr[1]);
        if (tamano <= 0)
            printf("No se asignan bloques de 0 bytes");
        else {
            for(i=0; MemStr[i].size != 0; i++);
            MemStr[i].DirMem = malloc(tamano);
            if(MemStr[i].DirMem == NULL){
                error("Error al intentar asignar memoria");
                return;
            }
            MemStr[i].size = tamano;
            time_t t;
            struct tm *inft;

            time(&t);
            inft = localtime(&t);
            char fecha[50];
            strftime(fecha, sizeof(fecha), "%B %H:%M", inft);
            strcpy(MemStr[i].allocTime, fecha);
            printf("Asignados %d bytes en %p\n", tamano, MemStr[i].DirMem);
        }
    }
}

struct MmapStruct {
    void *address;
    size_t size;
    int fileDescriptor;
    char *fileName;
    time_t timestamp; // Add timestamp field
};


void insertMapStruct(struct MmapStruct mapStruct[], void *address, size_t size, int fileDescriptor, char *fileName) {
    int i;
    for (i = 0; i < MAXOPEN; i++) {
        if (mapStruct[i].address == NULL) {
            mapStruct[i].address = address;
            mapStruct[i].size = size;
            mapStruct[i].fileDescriptor = fileDescriptor;
            mapStruct[i].fileName = strdup(fileName);
            mapStruct[i].timestamp = time(NULL); // Record the timestamp
            break;
        }
    }
    if (i == MAXOPEN) {
        printf("ARRAY LLENO\n");
    }
}

void printMapStruct(struct MmapStruct *mapStruct) {
    for (int i = 0; i < MAXOPEN; i++) {
        if (mapStruct[i].address != NULL) {
            struct tm *tm_info = localtime(&mapStruct[i].timestamp);
            char time_str[20];

            strftime(time_str, sizeof(time_str), "%y %b %d %H:%M", tm_info);

            printf("\t%p\t\t%s\t%s  (descriptor %d)\n",
                   mapStruct[i].address,
                   time_str,
                   mapStruct[i].fileName,
                   mapStruct[i].fileDescriptor);
        }
    }
}

void DesmapearFichero(char *fichero, struct MmapStruct MapStr[]) {
    int i;
    for (i = 0; i < MAXOPEN; i++) {
        if (MapStr[i].address != NULL && strcmp(MapStr[i].fileName, fichero) == 0) {
            // Desmapear la región
            if (munmap(MapStr[i].address, MapStr[i].size) == 0) {
                // Liberar la entrada en la estructura
                free(MapStr[i].fileName);
                memset(&MapStr[i], 0, sizeof(struct MmapStruct));
            }
            return;
        }
    }printf("Fichero %s no encontrado\n", fichero);
}

void * MapearFichero (char * fichero, int protection, struct  MmapStruct MapStr[]) {
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;
    insertMapStruct(MapStr,p,s.st_size,df,fichero);


    return p;
}

void CmdMmap(char *arg[], struct MmapStruct MapStr[],struct ProcessList PList[]){

    if (arg[1] != NULL && strcmp(arg[1], "-free") == 0 && arg[2] != NULL) {
        DesmapearFichero(arg[2], MapStr);
        return;
    }
    char *perm;
    void *p;
    int protection=0;

    if ((arg[1]==NULL || (strcmp(arg[1],"-free")==0 && arg[2]==NULL)))
    {   printf("******Lista de bloques asignados mmap para el proceso %d: \n", getpid());
        printMapStruct(MapStr);
        return;
    }
    if ((perm=arg[2])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MapearFichero(arg[1],protection,MapStr))==NULL)
        perror ("Imposible mapear fichero");
    else
        printf ("fichero %s mapeado en %p\n", arg[1], p);
}

void Mmap(char *tr[], struct MmapStruct MapStr[],struct ProcessList PList[]){
    CmdMmap(tr,MapStr,PList);
}

struct SharedStruct {
    void *p;
    size_t tam;
    key_t clave;
    char time[20];
};

void listarBloquesShared(struct SharedStruct ShaStr[]){
    for (int i = 0; i < 10; ++i) {
        if (ShaStr[i].p != NULL) {
            printf("\t%p\t\t%zu %s shared (key %d)\n", ShaStr[i].p, ShaStr[i].tam, ShaStr[i].time, ShaStr[i].clave);
        }
    }
}

void SharedDelkey (char *args[]) {
    key_t clave;
    int id;
    char *key=args[0];

    if (key==NULL || (clave=(key_t) strtoul(key,NULL,10))==IPC_PRIVATE){
        printf ("      delkey necesita clave_valida\n");
        return;
    }
    if ((id=shmget(clave,0,0666))==-1){
        error ("shmget: imposible obtener memoria compartida");
        return;
    }
    if (shmctl(id,IPC_RMID,NULL)==-1)
        error ("shmctl: imposible eliminar id de memoria compartida");
}

void SharedFree(char *tr[], struct SharedStruct ShaStr[]) {
    key_t clave;
    int id;

    if (tr[2] == NULL) {
        printf("******Lista de bloques asignados shared para el proceso %d\n", getpid());
        listarBloquesShared(ShaStr);
        return;
    }

    char *key = tr[2];

    if (key == NULL || (clave = (key_t)strtoul(key, NULL, 10)) == IPC_PRIVATE) {
        printf("      free necesita clave válida\n");
        return;
    }

    if ((id = shmget(clave, 0, 0666)) == -1) {
        error("shmget: imposible obtener memoria compartida");
        return;
    }

    if (shmdt(shmat(id, NULL, 0)) == -1) {
        error("shmdt: imposible desmapear memoria compartida");
        return;
    }

    int i, buscar = atoi(tr[2]);
    for(i=0; i<MAXOPEN; i++){
        if(ShaStr[i].tam == buscar) {
            ShaStr[i].p = NULL; //Se marca como NULL para que deje de apuntar al shared y lo reconozca como vacio al listar
            break;
        }
    }
    if (i == MAXOPEN) {
        printf("Tamaño de memoria no encontrado\n");
    }
}

bool insertarNodoShared(void*p, key_t clave, size_t tam, struct SharedStruct ShaStr[]) {
    int pos = -1;
    for (int i = 0; i < MAXOPEN; i++) {
        if (ShaStr[i].p == NULL) {
            pos = i;
            break;
        }
    }

    if (pos != -1) {
        ShaStr[pos].p = p;
        ShaStr[pos].tam = tam;
        ShaStr[pos].clave = clave;

        time_t t;
        struct tm *inft;

        time(&t);
        inft = localtime(&t);
        strftime(ShaStr[pos].time, sizeof(ShaStr[pos].time), "%B %H:%M", inft);
    } else {
        return false;
    }
    return true;
}

void *ObtenerMemoriaShmget(key_t clave, size_t tam, struct SharedStruct ShaStr[], bool create) {
    int aux, id, flags = 0777;
    struct shmid_ds s;
    void *p;

    if (create) {
        // Si estamos creando, intentamos obtener el segmento existente primero
        if (tam)     /*tam distito de 0 indica crear */
            flags=flags | IPC_CREAT | IPC_EXCL;
        if (clave==IPC_PRIVATE)  /*no nos vale*/
        {errno=EINVAL; return NULL;}
        if ((id=shmget(clave, tam, flags))==-1)
            return (NULL);
        if ((p=shmat(id,NULL,0))==(void*) -1){
            aux=errno;
            if (tam)
                shmctl(id,IPC_RMID,NULL);
            errno=aux;
            return (NULL);
        }
        shmctl (id,IPC_STAT,&s);

        //PROCESO DE INSERCION
        if(insertarNodoShared(p, clave, tam, ShaStr)==false){
            return NULL;
        }
    } else {
        if ((id = shmget(clave, tam, 0666)) == -1) {
            return NULL;
        }

        p = shmat(id, NULL, 0);

        //PROCESO DE INSERCION
        if(insertarNodoShared(p, clave, tam, ShaStr)==false){
            return NULL;
        }
    }

    return p;
}

void SharedCreate (char *tr[], struct SharedStruct ShaStr[]) {
    key_t cl;
    size_t tam;
    void *p;

    if (tr[2]==NULL || tr[3]==NULL) {
        printf("******Lista de bloques asignados shared para el proceso %d\n", getpid());
        listarBloquesShared(ShaStr);
        return;
    }

    cl=(key_t)  strtoul(tr[2],NULL,10);
    tam=(size_t) strtoul(tr[3],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam, ShaStr, true))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void shared (char * tr[], struct SharedStruct ShaStr[]) {
    if (tr[1] == NULL) {
        printf("******Lista de bloques asignados shared para el proceso %d\n", getpid());
        listarBloquesShared(ShaStr);
    } else if (strcmp(tr[1], "-free") == 0) { //LO elimina de la lista pero no lo elimina
        SharedFree(tr, ShaStr);
    } else if (strcmp(tr[1], "-create") == 0) {
        SharedCreate(tr, ShaStr);
    } else if (strcmp(tr[1], "-delkey") == 0) { //Lo elimina pero no de la lista
        char *key = tr[2];
        SharedDelkey(&key);
    } else {
        bool existe = false;
        for (int i = 0; i < MAXOPEN; i++) {
            if (ShaStr[i].clave == (key_t) atoi(tr[1])) {
                void *p = ObtenerMemoriaShmget(ShaStr[i].clave, ShaStr[i].tam, ShaStr, false);
                if(p==NULL){
                    printf("Algo salio mal al asignar\n");
                }
                existe = true;
                break;
            }
        }
        if(!existe) {
            printf("Imposible asignar memoria compartida clave %d\n", atoi(tr[1]));
        }
    }
}

ssize_t EscribirFichero (char *f, void *p, size_t cont,int overwrite) {
    ssize_t  n;
    int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags=O_CREAT | O_WRONLY | O_TRUNC;

    if ((df=open(f,flags,0777))==-1)
        return -1;

    if ((n=write(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

ssize_t LeerFichero (char *f, void *p, size_t cont) {
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   // si pasamos -1 como bytes a leer lo leemos entero
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void *cadtop(char *cad) {
    void *p;
    sscanf(cad, "%p", &p);
    return p;
}

void CmdRead (char *ar[]) {
    void *p;
    size_t cont=-1;  // -1 indica leer todo el fichero
    ssize_t n;
    if (ar[0]==NULL || ar[1]==NULL || ar[2]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p=cadtop(ar[2]);
    if (ar[3]!=NULL)
        cont=(size_t) atoll(ar[3]);

    if ((n=LeerFichero(ar[1],p,cont))==-1)
        error ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}

void WRite(char *ar[]) {
    void *p;
    size_t cont = 0;
    int overwrite = 0;  // 0 indica no sobrescribir por defecto
    ssize_t n;
    int i = 1;

    // Verificar argumentos
    if (ar[0] == NULL || ar[1] == NULL || ar[2] == NULL || ar[3] == NULL) {
        printf("Faltan parametros\n");
        return;
    }

    if(strcmp(ar[1], "-o")==0){
        if(ar[4]==NULL){
            printf("Faltan parametros\n");
            return;
        }
        else{
            overwrite = 1;
            i++;
        }
    }

    p = cadtop(ar[i+1]);
    cont=(size_t) atoll(ar[i+2]);

    n = EscribirFichero(ar[i], p, cont, overwrite);

    if (n == -1)
        error("Imposible escribir en el fichero");
    else
        printf("escritos %lld bytes en %s desde %p\n", (long long)n, ar[i], p);
}


void memdump(char *tr[]) {
    int size;
    char c[3];  // Se cambió la longitud de c a 3 para incluir el carácter nulo '\0'
    int i = 0, j = 0;
    if (tr[1] != NULL) {
        if (tr[2] == NULL) {
            size = 25;
        } else {
            size = atoi(tr[2]);
        }

        void *direccion = (void *)strtol(tr[1], NULL, 16);
        unsigned char *p = (unsigned char *)direccion;

        printf("Volcando %d bytes desde la direccion %s\n", size, tr[1]);
        while (i < size) {
            for (int k = 0; k < 25 && i < size; k++) {
                if (*(p + i) > 31 && *(p + i) < 127) {
                    snprintf(c, sizeof(c), "%c", *(p + i));  //Tuve que usar el snprintf para evitar lo de desbordamiento
                } else if (*(p + i) == '\t') {
                    snprintf(c, sizeof(c), "\\t");
                } else if (*(p + i) == '\n') {
                    snprintf(c, sizeof(c), "\\n");
                } else {
                    strcpy(c, "");
                }
                printf("%2s ", c);
                i++;
            }
            printf("\n");

            for (int k = 0; k < 25 && j < size; k++) {
                printf("%02x ", *(p + j));
                j++;
            }
            printf("\n");
        }
    }
}

void LlenarMemoria (void *p, size_t cont, unsigned char byte) {
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}

void memfill(char * tr[], struct MemoryStruct MemStr[]){
    if(tr[1] != NULL && tr[2] != NULL && tr[3] != NULL){ //memfill 0x0 10 'a'
        void *direccion = (void *)strtol(tr[1], NULL, 16); //ATOI EN BASE 16
        unsigned char *p = (unsigned char *)direccion;
        size_t tam = atoi(tr[2]);
        int carac;

        int i;
        for (i = 0; i < MAXOPEN; i++) {
            if(MemStr[i].DirMem == p){
                break;
            }
            if(i==MAXOPEN-1) {
                printf("¡SOBREESCRIBIENDO UNA MEMORIA NO INICIALIZADA!\n");
            }
        }

        if(tam > MemStr[i].size) {
            printf("No puedes asignar %zu bytes a una memoria a %zu bytes\n", tam, MemStr[i].size);
            return;
        }

        if (tr[3][0] == '\'' && tr[3][2] == '\'') {
            carac = (int) tr[3][1];
        } else if(tr[3][0] == '0' && tr[3][1] == 'x')  {
            carac = (int)strtol(tr[3], NULL, 16);
        } else {
            carac = atoi(tr[3]);
        }
        LlenarMemoria(p, tam, (unsigned char) carac);
        printf("Llenando %zu bytes de memoria con el byte (%d) a partir de la direccion %p\n", tam, carac, p);
    }
}

void memblocks(struct  MemoryStruct MemStr[], struct SharedStruct ShaStr[], struct MemoryStruct MapStr[]){
    printf("******Lista de bloques asignados para el proceso %d\n", getpid());
    listMallocs(MemStr);
    listarBloquesShared(ShaStr);
    printMapStruct((struct MmapStruct *) MapStr);
}

void f1() { printf("Soy la función 1\n"); }
void f2() { printf("Soy la función 2\n"); }
void f3() { printf("Soy la función 3\n"); }

void memfuncs(){
    void (*f1)() = (void (*)()) &f1;
    void (*f2)() = (void (*)()) &f2;
    void (*f3)() = (void (*)()) &f3;

    printf("%15s\t%18p,\t%15p,\t%15p\n", "Funciones del programa", f1, f2, f3);
    printf("%15s\t%18p,\t%15p,\t%15p\n", "Funciones libreria", (void *) strcpy, (void *) strcmp, (void *) strlen);
}

int intgNI;
float floatgNI;
char chargNI;

int intg = 1;
float floatg = 3.14f;
char charg = 'a';

void memvars() {
    int intl;
    float floatl;
    char charl;
    
    static int intsNI;
    static float floatsNI;
    static char charsNI;

    static int ints = 2;
    static float floats = 1.7f;
    static char chars = 'b';

    printf("%15s\t%15p,\t%15p,\t%15p\n", "Variables locales", (void*)&intl, (void*)&floatl, (void*)&charl);
    printf("%15s\t%15p,\t%15p,\t%15p\n", "Variables globales", (void*)&intg, (void*)&floatg, (void*)&charg);
    printf("%15s\t%15p,\t%15p,\t%15p\n", "Var (N.I.)globales", (void*)&intgNI, (void*)&floatgNI, (void*)&chargNI);
    printf("%15s\t%15p,\t%15p,\t%15p\n", "Variables staticas", (void*)&ints, (void*)&floats, (void*)&chars);
    printf("%15s\t%15p,\t%15p,\t%15p\n", "Var (N.I.)staticas", (void*)&intsNI, (void*)&floatsNI, (void*)&charsNI);
}

void Do_MemPmap (void) { //No tiene argumentos
    pid_t pid;       //hace el pmap (o equivalente) del proceso actual
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){ //proceso hijo
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) //probamos vmmap Mac-OS
            perror("cannot execute vmmap (Mac-OS)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)//No hay pmap, probamos procstat FreeBSD
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  //probamos procmap OpenBSD
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid(pid,NULL,0);
}

void mem(char * tr[], struct  MemoryStruct MemStr[], struct SharedStruct ShaStr[], struct MemoryStruct MapStr[]){
    if (tr[1] == NULL || strcmp(tr[1], "-all")==0) {
        memvars();
        memfuncs();
        memblocks(MemStr, ShaStr, MapStr);
    } else if(strcmp(tr[1], "-blocks")==0) {
        memblocks(MemStr, ShaStr, MapStr);
    } else if(strcmp(tr[1], "-funcs")==0) {
        memfuncs();
    } else if(strcmp(tr[1], "-vars")==0) {
        memvars();
    } else if(strcmp(tr[1], "-pmap")==0) {
        Do_MemPmap();
    } else {
        printf("Opcion %s no contemplada", tr[1]);
    }
}

//...................................................................PROCESO GENERAL DEL MAIN

int intruccion(int maxtr, char * tr[maxtr], tList list, struct CommandsStruct CoSt, struct MemoryStruct MemStr[], struct SharedStruct ShaStr[], struct MmapStruct MapStr[], struct ProcessList PList[]) {

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
    }

    else if(strcmp(tr[0], "create")==0) { //--------P1
        create(tr);
    } else if(strcmp(tr[0], "stat")==0) {
        STat(tr);
    } else if(strcmp(tr[0], "list")==0) {
        LIst(tr);
    } else if(strcmp(tr[0], "delete")==0) {
        delete(tr[1]);
    } else if(strcmp(tr[0], "deltree")==0) {
        deltree(tr[1]);
    }

    else if(strcmp(tr[0],"malloc")==0){
        MAlloc(tr, MemStr);
    } else if(strcmp(tr[0], "shared")==0) {
        shared(tr, ShaStr);
    } else if(strcmp(tr[0], "mmap")==0) {
        Mmap(tr, MapStr, PList);
    } else if(strcmp(tr[0], "read")==0) {
        CmdRead(tr);
    } else if(strcmp(tr[0], "write")==0) {
        WRite(tr);
    } else if(strcmp(tr[0], "memdump")==0) {
        memdump(tr);
    } else if(strcmp(tr[0], "memfill")==0) {
        memfill(tr, MemStr);
    } else if(strcmp(tr[0], "mem")==0) {
        mem(tr, MemStr, ShaStr, (struct MemoryStruct *) MapStr);
    } else if(strcmp(tr[0], "recurse")==0) {
        Recursiva(atoi(tr[1]));
    }

    else {
            error("No ejecutado");
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

    struct ProcessList PList[MAXOPEN]; //Struct de los procesos
    startProcessList(PList);

    struct MemoryStruct MemStr[MAXOPEN]; //Hace falta inicializar
    for (int i = 0; i < MAXOPEN; ++i) {
        MemStr[i] = (struct MemoryStruct){NULL, 0, ""};
    }
    struct SharedStruct ShaStr[MAXOPEN];
    for (int i = 0; i < MAXOPEN; ++i) {
        ShaStr[i] = (struct SharedStruct){NULL, 0, 0, ""};
    }

    struct MmapStruct MapStr[MAXOPEN];
    for (int i = 0; i < MAXOPEN; ++i) {
        MapStr[i] = (struct MmapStruct){NULL, 0, 0, "", 0};
    }

    while (exit==false) {

        printf("-(ツ)--> ");
        fgets(input, sizeof(input), stdin); //Pide introducir en teclado la instruccion

        if (strcmp(input, "\n") != 0) { //Evita meter nulls al programa o cadenas vacias

            if(!insertElement(input, &list))
                error("No ejecutado: No such file or directory");

            maxtr = partir(input, tr); //Parte la instruccion en cachos tr[MAX]


            checker = intruccion(maxtr, tr, list, CoSt, MemStr, ShaStr, MapStr, PList);

            switch (checker) {
                case 0: //Si retorna 0 es un exit, bye o quit, asi que sale del programa
                    exit=true;
                    printf("\n(ツ)ノ -BYE-\n\n");
                    break;
                case 1: //Si retorna 1 se borra el contenido de la lista
                    deleteList(&list);
                    break;
                case 2: //Si retorna 2 se uso comand, que repite una instruccion. Esta implementacion esta pensada para evitar el bucle infinito en el que se llama a si mismo
                    if(tr[1] != NULL) {
                        strcpy(input, getElement(tr[1], list));
                        maxtr = partir(input, tr);
                        if(strcmp(tr[0], "comand")==0)
                            printf("Comand not valid\n");
                        intruccion(maxtr, tr, list, CoSt, MemStr, ShaStr, MapStr, PList);
                    }

                default:
                    break;
            }
        }
    }

    //ANTES DE FINALIZAR ELIMINAR TODOS LOS PROCESOS DEL LISTOPEN
    freeAllMallocs(MemStr);
    deleteList(&list);

    return 0;
}
