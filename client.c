#include "header.h"

//Function to read recv 


void *read_buffer(void *arg){
    
    //recv message from server
    // return commands available
}

int main(int argc, char** argv) {
 
    char *error;
 
    char message[1024];
    char send_message[1024];
    int sockid, conn_sock;
    struct sockaddr_in server_address;
    client_data cli;
    pthread_t client_thread;
 
 
    //CREATION OF SOCKET
    if((cli.sockid=socket(AF_INET,SOCK_STREAM,0))<0){
        error="socket";
        goto err;
    }
 
 
    //seteamos la direccion en la que va a escuchar
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(SERVER_PORT);
    server_address.sin_addr.s_addr=htonl(INADDR_ANY);

 
    //CONNECT TO SERVER 3 WAY HANDSHAKE TCP
    if(connect(cli.sockid, (struct sockaddr *)&server_address, sizeof(server_address))<0){
        error="connect";
        goto err;
    }
    printf("CONNECTED TO THE SERVER\n");
    pthread_create(&client_thread, NULL, read_buffer, (void*)&cli);
    sleep(1);

    while (1){
        //CHECK IF CLIENT HAS USERNAME
        if(cli.nickname[0] == '\0') {
            printf("Ingrese nickname: ");
            scanf("%s", message);
            strcpy(send_message, message);
            strcpy(cli.nickname,message);

        } else {
            //SWITCH PARA COMMANDOS
        }

    }
        send(cli.sockid, send_message, strlen(send_message), 0);
        sleep(1);

    close(sockid);
 
    return 0;
 
err:
    fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
    exit(1);
 
}

