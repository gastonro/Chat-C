#include "header.h"

void menu(){
    printf("Menu chat \n");
    printf("1-List\n");
    printf("2-Chatear\n");
    printf("3-Bye\n");
}

void *read_buffer(void *arg){
    
    client_data *args = (client_data *)arg;
    char *reading_buffer = NULL;
    char *reading_line = NULL;
    //Estructura para leer los buffers del server
    struct iovec message_recieve_server[2];
    char buffo1[1];
    char buffo2[512];
    char *message_from_server1 = NULL;
    char *message_from_server2 = NULL;
    char command_from_server;
    message_recieve_server[0].iov_base = buffo1;
    message_recieve_server[0].iov_len = sizeof(buffo1);
    message_recieve_server[1].iov_base = buffo2;
    message_recieve_server[1].iov_len = sizeof(buffo2);
    
    while (readv(args->sockid,message_recieve_server,2) > 0){

        message_from_server1 = message_recieve_server[0].iov_base;
        command_from_server = message_from_server1[0];
        message_from_server2 = message_recieve_server[1].iov_base;
        printf("Comando del servidor: %c \n",command_from_server);
        //printf("%s \n",message_from_server2);
        switch (command_from_server)
        {
        case '1':
            printf("Registrado como usuario: %s \n",message_from_server2);
            break;
        case '2':
            printf("%s \n", message_from_server2);
        default:
            break;
        }


    sleep(0.1);    
    }

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
    int exit_client = 0;
 
 
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

    while (1){

        //CHECK IF CLIENT HAS USERNAME
        if(start == 0) {
            char *command_client = "1";
            printf("Ingrese nickname: ");
            scanf("%s", message);
            strcpy(cli.nickname, message);
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
            
            // limpiar consola printf("\e[1;1H\e[2J");
            int command_for_client;
            printf("Bienvenido %s \n",cli.nickname);
            menu();
            printf("Seleccione commando: \n");
            scanf("%d", &command_for_client);
            printf("Comando seleccionado es : %d \n", command_for_client);
            switch (command_for_client)
            {
            case 1: ;
                char *command_client_list = "2";
                printf("Solicito lista \n");
                memset(message,0,strlen(message));
                struct iovec send_message_struct_connect[2];
                send_message_struct_connect[0].iov_base = command_client_list;
                send_message_struct_connect[0].iov_len = strlen(command_client_list);
                send_message_struct_connect[1].iov_base = message;
                send_message_struct_connect[1].iov_len = strlen(message);
                writev(cli.sockid, send_message_struct_connect, 2);
                sleep(1);
                printf("\n");
                break;
            case 2: ;
                //char *command_client_chat = "3";
                //aca va para chatear
                break;
            case 3: ;
                char *command_client_log = "4";
                printf("Bye \n");
                memset(message,0,strlen(message));
                struct iovec send_message_struct_connect2[2];
                send_message_struct_connect2[0].iov_base = command_client_log;
                send_message_struct_connect2[0].iov_len = strlen(command_client_log);
                send_message_struct_connect2[1].iov_base = message;
                send_message_struct_connect2[1].iov_len = strlen(message);
                writev(cli.sockid, send_message_struct_connect2, 2);
                sleep(1);
                printf("\n");
                exit_client++;
                break;
            
            default:
                printf("Comando incorrecto \n");
                break;
            }

            if (exit_client == 1) {
                printf("Log off\n");
                break;
            }
        }
    
    }
    close(cli.sockid);
 
    return 0;
 
err:
    fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
    exit(1);
 
}


