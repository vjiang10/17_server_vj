#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <errno.h> 
#include <signal.h>

int fd, fd2;

static void sighandler(int signo) {
    if (signo == SIGINT) {
        printf("program exited\n");
        exit(0);
    }
}

void handshake_procedure() {
    char str[256];
    // private FIFO 
    sprintf(str, "%d", getpid());
    mkfifo(str, 0666);

    // opens and writes to WKP
    fd = open("WKP", O_WRONLY);

    // Client: sends message to Server
    printf("Client sending message to Server.\n");
    write(fd, str, sizeof(str));
    
    // Client: checking message from Server
    fd2 = open(str, O_RDONLY);
    char msg[256];
    // reads message from server
    read(fd2, msg, sizeof(msg));
    printf("Awknowledgement from Server to Client: %s\n", msg);

    // Client: sends message to Server
    printf("Client sending message to Server.\n");
    write(fd, str, sizeof(str));
    
    // removes private FIFO
    remove(str);
}

int main()
{
    signal(SIGINT, sighandler);
    handshake_procedure();
    // operation 
    char input[256];
    sleep(0.5);
    printf("\nReturns all UPPERCASE of str input:\n");
    
    while(1) {
        sleep(0.5);
        printf("Input str: ");
        int size = sizeof(input);
        fgets(input, size, stdin);
        // write/pass input str to server
        write(fd, input, size);
        // reads output str from server
        read(fd2, input, size);
        printf("Output str: %s\n", input);
    }
    
    close(fd);
    close(fd2);

    return 0;
}
