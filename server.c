#include "header.h"

void add_client(client_struct *added_client) {
    for (int i=0; i< MAX_CLIENTS; i++) {
        if (!clients[i]) {
            clients[i] = added_client;
            return;
        }
    }

}

void who_i_am(char *s, int fd){
    char *command_to_client = "1";
    struct iovec message_sent_server[2];
    message_sent_server[0].iov_base = command_to_client;
    message_sent_server[0].iov_len = strlen(command_to_client);
    message_sent_server[1].iov_base = s;
    message_sent_server[1].iov_len = strlen(s);
    writev(fd, message_sent_server, 2);
} 

void send_list_clients(int fd) {
    int i = 0;
    char s[64];
    char *command_to_client = "2";
    struct iovec message_sent_server[2];
    for (i = 0; i < MAX_CLIENTS; i++){
        if (clients[i] && (clients[i]->state == 0)){
            sprintf(s, "ID: %d | Name: %s | \n\r", clients[i]->client_id, clients[i] ->username);
            message_sent_server[0].iov_base = command_to_client;
            message_sent_server[0].iov_len = strlen(command_to_client);
            message_sent_server[1].iov_base = s;
            message_sent_server[1].iov_len = strlen(s);
            writev(fd, message_sent_server, 2);

        }

    }
}
void client_log_off(int fd) {
    int i;
    for (i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i]) {
            if(clients[i]->fd == fd){
                clients[i] = NULL;
            }
        }
    }
}

void *chat(void * arg ){


    char buff_out[1024];
    char buff_in[1024];
    int start = 0;


    // MESSAGE TIME
    time_t t;
    struct tm *tm;
    char chat_date[100];
    t=time(NULL);
    tm=localtime(&t);
    strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
    printf("%s ", chat_date);

    // ID CLIENT
    client_struct *client = (client_struct *) arg;
    printf("Client connected ");
    printf(" ID %d\n", client -> client_id);
    client->state = 0; //Idle

    client_data *args = (client_data *)arg;
    struct iovec message_recieved_client[2];
    char buffo1[1];
    char buffo2[512];
    char *message_from_client = NULL;
    char *get_client_value1 = NULL;
    char *get_client_value2 = NULL;
    char command_from_client;
    
    message_recieved_client[0].iov_base = buffo1;
    message_recieved_client[0].iov_len = sizeof(buffo1);
    message_recieved_client[1].iov_base = buffo2;
    message_recieved_client[1].iov_len = sizeof(buffo2);
    while (readv(client->fd, message_recieved_client, 2) > 0 ){
        
        printf("\n");
        get_client_value1 = message_recieved_client[0].iov_base;
        get_client_value2 = message_recieved_client[1].iov_base;
        
        command_from_client = get_client_value1[0];
        message_from_client = message_recieved_client[1].iov_base;
        printf("Mensaje de cliente recibido\n");
        printf("Comando: \n");
        printf("%c \n",command_from_client);
        printf("Entrada: \n");
        printf("%s \n",message_from_client);
        


        switch (command_from_client)
        {
            case '1':
                strcpy(client->username,message_from_client);
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                printf("%s Usuario: %s asignado nickname \n",chat_date,client->username);
                who_i_am(message_from_client,client->fd);
                memset(message_recieved_client[1].iov_base,0,strlen(message_recieved_client[1].iov_base));
                memset(message_recieved_client[0].iov_base,0,strlen(message_recieved_client[0].iov_base));
                break;
            
            case '2':
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                printf("%s Usuario: %s solicito lista \n",chat_date,client->username);
                send_list_clients(client->fd);
                break;
            
            case '4':
                t=time(NULL);
                tm=localtime(&t);
                strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
                printf("%s Usuario: %s solicito desconexion \n",chat_date,client->username);
                client_log_off(client->fd);
                break;
            default:
                break;
            }
    printf("\n");
    }
    close (client->fd);

} 
int main(int argc, char** argv) {
 
    char *error;
 
    int sockid, conn_sock;
    struct sockaddr_in server_address;
    struct sockaddr_in client_address;
    pthread_t thread;
 
    //Socket creation
    if((sockid=socket(AF_INET,SOCK_STREAM,0))<0){
        error="socket";
        goto err;
    }
 
 
    //Socket configuration
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family=AF_INET; //address family
    server_address.sin_port=htons(SERVER_PORT);  //atoi ascii to integer
    server_address.sin_addr.s_addr=htonl(INADDR_ANY); //0.0.0.0
 
 
    //Bind the address to the socket
    if ((bind(sockid, (struct sockaddr *)&server_address, sizeof(server_address)))<0 )    {
        error="bind";
        goto err;
    }
 
    // Listen to queue
    if (listen(sockid, MAX_CLIENTS) < 0)    {
        error="listen";
        goto err;

    }
    printf("SERVER ONLINE \n");
    printf("WAITING FOR USERS \n");

    while(1){
        socklen_t client_len = sizeof(client_address);
        
        conn_sock = accept(sockid, (struct sockaddr *)&client_address, &client_len);
        if (conn_sock < 0){
            error="accept";
            goto err;
        }

        //ADD CLIENT CONFIGURATION
        client_struct *client = (client_struct *) malloc(sizeof(client_struct));
        client->address = client_address;
        client->client_id = client_id++;
        client->fd = conn_sock;
        sprintf(client->username, "%d", client->client_id);

        // ADD CLIENT TO QUEUE AND CREATE THREAD
        add_client(client);
        pthread_create(&thread, NULL, &chat, (void *)client);
        sleep(1);

    }
  
 
err:
    fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
    exit(1);
 
}


