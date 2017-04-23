#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void error(char *msg)
{
    perror(msg);
    exit(0);
}

//Global variable declaration//
void inputCharacter(char *name, int len);

void kvsPut(char *key, char *value);
void kvsGet(char *key2);
void kvsDelete(char *key1);
int sockfd;
char buffer1[256];
time_t mytime;

int main(int argc, char *argv[])
{
    //variable declaration//
    int portno, n, option;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    mytime = time(NULL);
    int flag = 1;

    //Checking if argument count is less than 3 because we receive three arguments: ./client is 1, hostname is 2, portno is 3//
    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        exit(0);
    }

    //Converting portno received as char in argument to integer//
    portno = atoi(argv[2]);

    //Creating a socket, AF_INET refers to ip address, SOCK_STREAM refers to TCP reliable connection, 0 enables TCP connection//
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    //Printing error message//
    if (sockfd < 0){
        error("Error opening socket");
    }

    //Getting host(ip) by name (server) which is in argv[1]//
    server = gethostbyname(argv[1]);

    //Error message//
    if (server == NULL)
    {
        fprintf(stderr,"Error,no such host\n");
        exit(0);
    }

    //Clearing the serv_addr bzero is to make all the locations of serv_addr 0//
    bzero((char *) &serv_addr,sizeof(serv_addr));
    
    //Initializing the family of serv_address as AF_INET
    serv_addr.sin_family = AF_INET;

    //Copying the serv_address to server variable//
    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);
    
    //Converting the integer format of portno to network format//
    serv_addr.sin_port = htons(portno);

    //Error message//
    if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR connecting");
    }

    //Prepopulating the server//
    kvsPut("Amruthaa", "Radhika");
    kvsPut("Radhika", "Amruthaa");
    kvsPut("Seattle", "Washington");
    kvsPut("DS", "Collection of independent computers appears as a single coherent systems");
    kvsPut("Tacoma", "A place in the state of Washington");
    kvsPut("UW", "A research university");
    kvsPut("Mascot", "Harry the husky");
    kvsPut("Spring", "Begins on March 1st");
    kvsPut("Summer", "Begins on June 1st");
    kvsPut("Autumn","Begins on September 1st");
    kvsPut("Winter","Begins on December 1st");
    kvsPut("Rainier","A mountain in Washington state");

    //Printing in terminal//
    printf("key-value pairs are populated\n");
    printf("Set of keys: Amruthaa, Radhika, Seattle, DS, Tacoma, UW, Mascot, Spring, Summer, Autumn, Winter, Rainier\n");

    //Getting values for keys//
    printf("Get values for keys\n");
    kvsGet("Tacoma");
    kvsGet("Spring");
    kvsGet("DS");
    kvsGet("Summer");
    kvsGet("Amruthaa");
    kvsGet("Radhika");
    kvsGet("Seattle");
    kvsGet("Mascot");
    kvsGet("Autumn");
    kvsGet("Winter");

    //Deleting key value pairs//
    printf("Delete key value pairs\n");
    kvsDelete("Tacoma");
    kvsDelete("Spring");
    kvsDelete("DS");
    kvsDelete("Rainier");
    kvsDelete("Summer");


    while(flag == 1){
        char k[256],v[256],m[256];
        printf("Enter the operation\n");
        printf("1: PUT, 2: GET, 3:DELETE, 4:QUIT\n");
        
        //Function call//
        inputCharacter(k, 256);

        //converting character to integer//
        option = atoi(k);
        printf("Option: %d %s\n", option, k);
        
        //intializing all the spaces in the buffers as 0//
        bzero(k,256);
        bzero(v,256);
        bzero(m,256);
        int l;

        //switch starts//
        switch(option)
        {
            case 1:
            //For PUT function//
                printf("Enter the key\n");
                inputCharacter(k, 256);
                printf("Key entered: %s\n", k);
                printf("Enter the value\n");
                inputCharacter(v, 256);
                printf("Value is %s\n",v);
                kvsPut(k,v);
               break;

            case 2:
	    //For GET function//
                printf("Enter the key\n");
                inputCharacter(m, 256);
                kvsGet(m);
               break;

            case 3:
            //For DELETE function//
                printf("Enter the key\n");
                inputCharacter(m, 256);
                kvsDelete(m);
               break;

            case 4:
                flag = 0;
                printf("QUIT operation is requested at: %s\n",ctime(&mytime));
                break;
            default:
                printf("Invalid option entered\n");
        }
    }

    return 0;
}

//user input read helper function (key or value for PUT, GET, DELETE functions//
void inputCharacter(char *name, int len){
    char ch;
    int i = 0;
    bzero(name, len);
    while (1) {
        ch = getchar();
        if (ch == '\n' || i >= len) {
            break;
        }
        name[i++] = ch;
    }
    name[i] = '\0';
}

//PUT helper function in client//
void kvsPut(char *key, char *value){
    int c = 1,l;
   bzero(buffer1, 256);
    printf("PUT service is requested at: %s\n",ctime(&mytime));
    sprintf(buffer1,"%d:%s;%s",c,key,value);
    l = write(sockfd,buffer1,strlen(buffer1));
   if (l < 0)
    {
        error("ERROR reading from socket");
    }
      bzero(buffer1, 256);
      read(sockfd,buffer1,255);
      printf("%s\n",buffer1);
      printf("PUT service response is received at: %s\n",ctime(&mytime));

}

//GET helper function in client//
void kvsGet(char *key2){
  int c = 2, p;
  printf("Key: %s\n",key2);
  char buffer2[256];
  char buffer4[256];
  bzero(buffer2, 256);
  printf("GET service is requested at: %s",ctime(&mytime));
  sprintf(buffer2,"%d:%s",c,key2);
  p = write(sockfd,buffer2,strlen(buffer2));
 if (p < 0)
  {
      error("ERROR reading from socket");
  }
  bzero(buffer4,256);
  read(sockfd,buffer4,255);
  printf("Value: %s\n", buffer4);
  printf("GET service response is received at: %s\n",ctime(&mytime));
}

//DELETE helper function in client//
void kvsDelete(char *key1){
  int c = 3, l;
  char buffer3[256];
  char buffer5[256];
  bzero(buffer3,256);
  printf("DELETE service is requested at: %s",ctime(&mytime));
  sprintf(buffer3,"%d:%s",c,key1);
  l = write(sockfd,buffer3,strlen(buffer3));
  printf("Key: %s\n", key1);
  if (l < 0)
  {
      error("ERROR reading from socket");
  }
  bzero(buffer5,256);
  read(sockfd,buffer5,255);
  printf("%s\n",buffer5);
  printf("DELETE service response is received at: %s",ctime(&mytime));
}
