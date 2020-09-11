#include "header.h"

//Function to read recv 


void *read_buffer(void *arg){
    
    client_data *args = (client_data *)arg;
    char *reading_buffer = NULL;
    char *reading_line = NULL;
    //Estructura para leer los buffers del server
    struct iovec hola[2];
    char buffo1[1];
    char buffo2[512];
    char *message_from_server1 = NULL;
    char *message_from_server2 = NULL;
    char command_from_server;
    hola[0].iov_base = buffo1;
    hola[0].iov_len = sizeof(buffo1);
    hola[1].iov_base = buffo2;
    hola[1].iov_len = sizeof(buffo2);
    
    while (readv(args->sockid,hola,2) > 0){

        message_from_server1 = hola[0].iov_base;
        command_from_server = message_from_server1[0];
        message_from_server2 = hola[1].iov_base;
        printf("%c \n",command_from_server);
        printf("%s \n",message_from_server2);
        switch (command_from_server)
        {
        case '1':
            printf("Registrado como usuario: %s \n",message_from_server2);
            printf("Exito\n");
            break;
        
        default:
            break;
        }
        //printf("%s\n",reading_line);
        //reading_buffer = strtok(buffo2, "\r\n");
        //printf("%s\n",reading_buffer );

        //reading_line = strtok(reading_buffer, "\r\n");
        
        //printf("%s \n",reading_line);
        printf("Entro mas deep\n");

    sleep(0.1);    
    }
    //recv message from server
    // return commands available
    return NULL;
}

int main(int argc, char** argv) {
 
    char *error;
 
    char message[1024];
    char *send_message[1024];
    int sockid, conn_sock;
    struct sockaddr_in server_address;
    client_data cli;
    client_struct client;
    pthread_t client_thread;
    int start = 0;
 
 
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
    sleep(0.1);
    printf("Creo hilo cliente\n");

    while (1){

        //CHECK IF CLIENT HAS USERNAME
        if(start == 0) {
            char *command_client = "1"; //NEW CLIENT
            printf("Ingrese nickname: ");
            scanf("%s", message);
            struct iovec send_message_struct_connect[2];
            send_message_struct_connect[0].iov_base = command_client;
            send_message_struct_connect[0].iov_len = strlen(command_client);
            send_message_struct_connect[1].iov_base = message;
            send_message_struct_connect[1].iov_len = strlen(message);
            start++;
            writev(cli.sockid, send_message_struct_connect, 2);
            sleep(1);
            printf("\n");

        } else {
            
            char *prueba1;
            printf("Ingrese un adios\n");
            scanf("%s",message);
            struct iovec send_message_struct[2];
            char *command_client = "2";
            send_message_struct[0].iov_base = command_client;
            send_message_struct[0].iov_len = strlen(command_client);            
            send_message_struct[1].iov_base = message;
            send_message_struct[1].iov_len = strlen(message);
            printf("Mensaje que le paso es : %s \n",message);
            prueba1 = send_message_struct[1].iov_base;
            printf("%s \n",prueba1);
            writev(cli.sockid, send_message_struct, 2);
            //SWITCH PARA COMMANDOS
            sleep(1);

            
        }
    
    }
    close(cli.sockid);
 
    return 0;
 
err:
    fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
    exit(1);
 
}


