#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

//Error print function//
void error(char *msg)
{
    perror(msg);
    exit(1);
}

//Main function//
int main(int argc, char *argv[])
{
    //variables declaration
    int sockfd, newsockfd, portno, clilen;
    char socket_buffer[256];
    char value[256],key[256], c;
    struct sockaddr_in serv_addr, cli_addr;
    int n = 0,rm;
    char put_buffer[256];
    time_t mytime;
    char get_buffer[256];

    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    //socket creation and restoring communication//
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
    {
        error("ERROR opening socket");
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        error("ERROR on binding");
    }

    listen(sockfd,5);
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);

    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }

      //Time as in Epoch//
      mytime = time(NULL);

    while(1){
	
	//Reading from buffer by clearing the buffer//
        bzero(socket_buffer,256);
        n = read(newsockfd,socket_buffer,255);

        //printing the current time//
        printf("Service requested at: %s\n",ctime(&mytime));

        //Error printing//
        if (n <= 0)
        {
            error("ERROR reading from socket");
        }

        //clearing key and value buffers//
        bzero(key,256);
        bzero(value,256);

        //initialization of variable to use in a loop//
        int i,j=0,k,l=0;

        //converting socket_buffer character to integer to get operation//
        k = socket_buffer[0] - '0';

        //getting key from the buffer: index starts from 2nd position and ends with ';'. First position has option. From kvsput in clientTCP.c//
        for(i=2;i<strlen(socket_buffer);i++){
            if(socket_buffer[i] == ';'){
                break;
            }
            key[j] = socket_buffer[i];
            j++;
        }

        //getting value from the buffer in a similar way//
        i=i+1;
        for(;i<strlen(socket_buffer);i++){
            if(socket_buffer[i] == '\0'){
                break;
            }
            value[l] = socket_buffer[i];
            l++;
        }

        //concatinating strings of directory and keyname into a file name directory//
        int q;
	char src[] = "/tmp/";
        strcat(src, key);
        strcat(src,".txt");
        FILE *fptr;

        //Switch starts here//
        switch(k){
            case 1:
                
                //PUT operation//
                fptr = fopen(src,"w");
                if(fptr == NULL)
                {
                    printf("Error!");
                    exit(1);
                }
                fprintf(fptr,"%s",value);
                bzero(put_buffer, 256);
                sprintf(put_buffer,"Ack");
                q = write(newsockfd,put_buffer,strlen(put_buffer));
                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                fclose(fptr);
                break;

            case 2:

                //GET operation//
                if ((fptr = fopen(src,"r")) == NULL){
                    printf("File not found\n");
                    char *r = "FILE NOT FOUND";
                    write(newsockfd, r, strlen(r));
                    break;
                }
                bzero(put_buffer, 256);
                fgets(put_buffer, 256, fptr);
                q = write(newsockfd,put_buffer,strlen(put_buffer));
                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                fclose(fptr);
                break;

            case 3:
              
                //DELETE operation//
                bzero(get_buffer, 255);
                rm = remove(src);
                if(rm==0){
                    strcpy(get_buffer, "FILE IS DELETED SUCCESSFULLY");
                }
                else{
                    printf("File not deleted successfully\n");
                    printf("File not found\n");
                    strcpy(get_buffer, "FILE NOT FOUND AND IS NOT DELETED");
                }
                q=  write(newsockfd, get_buffer, strlen(get_buffer));

                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                  printf("%s\n",get_buffer);
                break;
        }
    }

    return 0;
}
