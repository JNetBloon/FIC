
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <time.h>
#include <fcntl.h>
#include <sys/stat.h> //Es la de mkdir
#include <dirent.h> //Es para DIR
#include <libgen.h>

#include <pwd.h> //Estas son del grupo y user
#include <grp.h>

#include "listas.h"


#include <sys/utsname.h>


#define MAX 100
#define MAXCN 21
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
    error("Imposible duplicar descriptor");
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
        error("Imposible abrir fichero");
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
        error("Imposible cerrar descriptor");
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
    }
}

//PROCESO GENERAL DEL MAIN

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
    } else if(strcmp(tr[0], "create")==0) { //--------P1
        create(tr);
    } else if(strcmp(tr[0], "stat")==0) {
        STat(tr);
    } else if(strcmp(tr[0], "list")==0) {
        LIst(tr);
    } else if(strcmp(tr[0], "delete")==0) {
        delete(tr[1]);
    } else if(strcmp(tr[0], "deltree")==0) {
        deltree(tr[1]);
    } else {
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

            switch (checker) {
                case 0: //Si retorna 0 es un exit, bye o quit, asi que sale del programa
                    exit=true;
                    break;
                case 1: //Si retorna 1 se borra el contenido de la lista
                    deleteList(&list);
                    break;
                case 2: //Si retorna 2 se uso comand, que repite una instruccion. Esta implementacion esta pensada para evitar el bucle infinito en el que se llama a si mismo
                    strcpy(input, getElement(tr[1], list));
                    maxtr = partir(input, tr);
                    if(strcmp(tr[0], "comand")==0)
                        printf("Comand not valid\n");
                    intruccion(maxtr, tr, list, CoSt, PList);

                default:
                    break;
            }
        }
    }

    //ANTES DE FINALIZAR ELIMINAR TODOS LOS PROCESOS DEL LISTOPEN
    deleteList(&list);

    return 0;
}
