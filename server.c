#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
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
    printf("Server connecting to Client...\n");
    // creates WKP
    mkfifo("WKP", 0666);

    // opens and reads WKP
    fd = open("WKP", O_RDONLY);
    char str[256]; 
    read(fd, str, sizeof(str));

    // Server: receives message from Client
    printf("Client to Server message received: %s. Connection made.\n", str);

    // removes WKP
    remove("WKP");

    // Server: sending message to Client
    fd2 = open(str, O_WRONLY);
    char check[256] = "Checking Server to Client";
    write(fd2, check, sizeof(check));

    // Server: receiving message from Client
    char check2[256];
    read(fd, check2, sizeof(check2));
    printf("Handshake complete. Client to Server message received: %s\n", check2); 
}

int main() {
    signal(SIGINT, sighandler);
    handshake_procedure();
    // operation
    char input[256];
    while(1) {
        int size = sizeof(input);
        // reads input str from client
        read(fd, input, size);
        // changes str input to all UPPERCASE
        int i;
        for (i = 0; input[i]; i++) input[i] = input[i] - 32;
        // write back to client
        write(fd2, input, size);
    }

    close(fd);
    close(fd2);

    return 0;
}