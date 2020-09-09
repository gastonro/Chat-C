#include "header.h"
//ADD CLIENT

void add_client(client_struct *added_client) {
    for (int i=0; i< MAX_CLIENTS; i++) {
        if (!clients[i]) {
            clients[i] = added_client;
            return;
        }
    }

}

void *chat(void * arg ){

    // MESSAGE TIME
    time_t t;
    struct tm *tm;
    char chat_date[100], chat_hour[100];
    t=time(NULL);
    tm=localtime(&t);
    strftime(chat_date, 128, "%d/%m/%Y %H:%M:%S", tm);
    printf("%s", chat_date);

    // ID CLIENT
    client_struct *client = (client_struct *) arg;
    printf("Client connected");
    printf(" ID %d\n", client -> client_id);
    client->state = 0;

    //read with the socket send from client
    //write to client
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
        sleep(3);

    }
  
    close(sockid);
 
    return 0;
 
err:
    fprintf(stderr,"%d %s %s\n",errno,error,strerror(errno));
    exit(1);
 
}


