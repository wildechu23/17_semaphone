#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <fcntl.h>

#define KEY 23001
#define KEY2 23002

union semun {
    int              val;    /* Value for SETVAL *\/ */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET *\/ */
    unsigned short  *array;  /* Array for GETALL, SETALL *\/ */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO */
/*                               (Linux-specific) *\/ */
 };


int main(int argc, char*argv[]) {
    if(argc > 1) {
        int shmd;
        int *data;

        int semd = semget(KEY2, 1, IPC_CREAT | IPC_EXCL | 0644);
        if(strcmp(argv[1], "-c") == 0) {
            shmd = shmget(KEY, sizeof(int), IPC_CREAT | 0600);
            union semun us;
            us.val = 1;
            int r = semctl(semd, 0, SETVAL, us);

            int f = open("story.txt", O_CREAT | O_WRONLY | O_TRUNC, 0644);
        } else if (strcmp(argv[1], "-r") == 0) {
            shmd = shmget(KEY, 0, 0);
            semd = semget(KEY2, 1, 0);

            shmctl(shmd, IPC_RMID, 0);
            semctl(semd, IPC_RMID, 0);

            char buffer[1000];
            int f = open("story.txt", O_RDONLY);
            int bytes_written = read(f, buffer, 1000);
            printf("%s", buffer);
        } else {
            printf("Invalid flag\n");
        }
    } else {
        printf("Flag needed\n");
    }

    return 0;
}
