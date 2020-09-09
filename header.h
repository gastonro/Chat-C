#include<stdio.h>
#include<stdlib.h>
#include <pthread.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<errno.h>
#include<sys/socket.h>
#include<netinet/in.h>
 
#define BUF_SIZE 1024
#define SERVER_PORT 8000
#define MAX_CLIENTS 10

// CLIENT STRUCTURE

static int client_id = 0;

typedef struct {
    struct sockaddr_in address;
    int fd;
    int client_id;
    char username[20];
    int state;
    int chat;
} client_struct;

client_struct *clients[MAX_CLIENTS];

typedef struct {
    int sockid;
    int chat;
    char nickname[50];
} client_data;
