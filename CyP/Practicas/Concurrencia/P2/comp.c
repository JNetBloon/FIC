#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include "compress.h"
#include "chunk_archive.h"
#include "queue.h"
#include "options.h"
#include <pthread.h>

#define CHUNK_SIZE (1024 * 1024)
#define QUEUE_SIZE 20

#define COMPRESS 1
#define DECOMPRESS 0

typedef struct { //El struct necesita mas variables para pasar de argumento a los thread
    queue in;
    queue out;
    struct options optQueue;
    int chunksQueue;
    int fd;
    archive ar;
} QueuePack;

void *worker(void *args) { //Se encarga de insertar en la cola de salida los chunks
    QueuePack *queueArg = (QueuePack *)args;
    chunk ch, res;
    while (1) {
        ch = q_remove(queueArg->in);
        if (ch == NULL) break;
        if (ch->size == 0) {
            free_chunk(ch);
            break; // Chunk especial que indica el fin del proceso
        }
        res = zcompress(ch);
        free_chunk(ch);
        q_insert(queueArg->out, res);
    }
    return NULL;
}

void *lectura(void *args) { //Lee los chunck y los inserta en la cola de entrada
    QueuePack *queueArg = (QueuePack *)args;
    int i, offset;
    for (i = 0; i < queueArg->chunksQueue; i++) {
        chunk ch = alloc_chunk(queueArg->optQueue.size);
        offset = lseek(queueArg->fd, 0, SEEK_CUR);
        ch->size = read(queueArg->fd, ch->data, queueArg->optQueue.size);
        ch->num = i;
        ch->offset = offset;
        q_insert(queueArg->in, ch);
    }

    for (i = 0; i < queueArg->optQueue.num_threads; i++) { //chunk necesario para que el programa funcione
        chunk ch = alloc_chunk(0);
        q_insert(queueArg->in, ch);
    }
    return NULL;
}

void *escritura(void *args){ //Lee los chunk de la cola de salida y lo libera, comprimiendo el archivo en el proceso
    QueuePack *queueArg = (QueuePack *)args;
    int i;
    for (i = 0; i < queueArg->chunksQueue; i++) {
        chunk ch = q_remove(queueArg->out);
        add_chunk(queueArg->ar, ch);
        free_chunk(ch);
    }
    return NULL;
}

void comp(struct options opt) { //Funcion de comprimir
    int fd, chunks, i;
    char comp_file[256];
    struct stat st;
    archive ar;
    queue in, out;
    pthread_t threads[opt.num_threads];
    pthread_t threadsLectura[opt.num_threads];
    pthread_t threadsEscritura[opt.num_threads];
    QueuePack args;

    printf("Starting compression process.\n");

        //PROPIEDADES DEL ARCHIVO
    if ((fd = open(opt.file, O_RDONLY)) == -1) {
        printf("Cannot open %s\n", opt.file);
        exit(0);
    }

    fstat(fd, &st);
    chunks = st.st_size / opt.size + (st.st_size % opt.size ? 1 : 0);

    if (opt.out_file) {
        strncpy(comp_file, opt.out_file, 255);
    } else {
        strncpy(comp_file, opt.file, 255);
        strncat(comp_file, ".ch", 255);
    }

        //PARAMETROS PARA EL ARGUMENTO
    ar = create_archive_file(comp_file);
    in = q_create(opt.queue_size);
    out = q_create(opt.queue_size);

    args.in = in;
    args.out = out;
    args.optQueue = opt;
    args.chunksQueue = chunks;
    args.fd = fd;
    args.ar = ar;

        //LECTURA -> Monohilo
    pthread_create(&threadsLectura[0],NULL,lectura,&args);
        //INSERCION -> Multihilo
    for (i = 0; i < opt.num_threads; i++) {
        pthread_create(&threads[i], NULL, worker, &args);
    }
        //ESCRITURA -> Monohilo
    pthread_create(&threadsEscritura[0],NULL,escritura,&args);


        //FIN DE LOS THREADS
    pthread_join(threadsLectura[0], NULL);
    for (i = 0; i < opt.num_threads; i++) {
        pthread_join(threads[i], NULL);
    }
    pthread_join(threadsEscritura[0], NULL);

    //Cierra el archivo y destruye las colas
    close_archive_file(ar);
    close(fd);
    q_destroy(in);
    q_destroy(out);

    printf("Compression process completed.\n");
}

void decomp(struct options opt) { //Funcion de descomprimir
    int fd, i;
    char uncomp_file[256];
    archive ar;
    chunk ch, res;

    if((ar=open_archive_file(opt.file))==NULL) {
        printf("Cannot open archive file\n");
        exit(0);
    }

    if(opt.out_file) {
        strncpy(uncomp_file, opt.out_file, 255);
    } else {
        strncpy(uncomp_file, opt.file, strlen(opt.file) -3);
        uncomp_file[strlen(opt.file)-3] = '\0';
    }

    if((fd=open(uncomp_file, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH))== -1) {
        printf("Cannot create %s: %s\n", uncomp_file, strerror(errno));
        exit(0);
    }

    for(i=0; i<chunks(ar); i++) {
        ch = get_chunk(ar, i);

        res = zdecompress(ch);
        free_chunk(ch);

        lseek(fd, res->offset, SEEK_SET);
        write(fd, res->data, res->size);
        free_chunk(res);
    }

    close_archive_file(ar);
    close(fd);
}


int main(int argc, char *argv[]) {
    struct options opt;

    opt.compress = COMPRESS;
    opt.num_threads = 3;
    opt.size = CHUNK_SIZE;
    opt.queue_size = QUEUE_SIZE;
    opt.out_file = NULL;

    read_options(argc, argv, &opt);

    if (opt.compress == COMPRESS) {
        comp(opt);
    } else {
        decomp(opt);
    }

    return 0;
}
