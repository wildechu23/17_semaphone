#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#define KEY 23001
#define KEY2 23002

int main() {
    int shmd = shmget(KEY, 0, 0);
    int semd = semget(KEY, 1, 0);

    struct sembuf sb;
    sb.sem_num = 0;
    sb.sem_op = -1;

    semop(semd, &sb, 1);
//    printf("have semaphore\n");

    int f = open("story.txt", O_RDWR);
    
    int *data;
    data = shmat(shmd, 0, 0);

    printf("%d\n", -1 * (*data));
    lseek(f, -1 * (*data), SEEK_END);

    char buffer[*data];
    int bytes_read = read(f, buffer, *data);
    buffer[*data] = '\0';
    if(bytes_read == -1) printf("error %d: %s\n", errno, strerror(errno));
    
    printf("%d\n", bytes_read);
    char input[1000];
    printf("Last line: %s\n", buffer);
    printf("New Line: ");
    fgets(input, 1000, stdin);
    int bytes_written = write(f, input, strlen(input));
    *data = strlen(input);
    shmdt(data);

    sb.sem_op = 1;
    semop(semd, &sb, 1);
    
    close(f);

    return 0;
}
