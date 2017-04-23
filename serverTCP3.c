#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

void error(char *msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
    int sockfd, newsockfd, portno, clilen;
    char buffer[256];
    char file_name[256],op[256], c;
    struct sockaddr_in serv_addr, cli_addr;
    int n = 0,rm;
    char num[256];
    time_t mytime;
            char w[256];

    if (argc < 2)
    {
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

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

    // TODO: while loop begin
    if (newsockfd < 0)
    {
        error("ERROR on accept");
    }

      mytime = time(NULL);

    while(1){
        char src[] = "/home/radhika/Documents/Radhika/";
        bzero(buffer,256);
        n = read(newsockfd,buffer,255);
        printf("Service requested at: %s\n",ctime(&mytime));
    //    printf("Request from client: %s\n",buffer);
        if (n <= 0)
        {
            error("ERROR reading from socket");
        }

        bzero(op,256);
        bzero(file_name,256);
      //  printf("second reading\n");
        int i,j=0,k,l=0;


        k = buffer[0] - '0';

        for(i=2;i<strlen(buffer);i++){
            if(buffer[i] == ';'){
//                printf("\n");
                break;
            }
            op[j] = buffer[i];
            j++;
        }
  //      printf("File name : %s\n",op);

        i=i+1;
        for(;i<strlen(buffer);i++){
            if(buffer[i] == '\0'){
                break;
            }
            file_name[l] = buffer[i];
            l++;
        }
  //      printf("Contents: %s\n",file_name);

        int q;
        strcat(src, op);
        strcat(src,".txt");
        FILE *fptr;
        switch(k){
            case 1:
  //              printf("PUT request case\n");
                fptr = fopen(src,"w");
                if(fptr == NULL)
                {
                    printf("Error!");
                    exit(1);
                }
                fprintf(fptr,"%s",file_name);
                bzero(num, 256);
                sprintf(num,"Ack");
                q = write(newsockfd,num,strlen(num));
                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                fclose(fptr);
                break;

            case 2:
                //FILE *fptr;
//                printf("GET request case\n");
                if ((fptr = fopen(src,"r")) == NULL){
                  //  printf("Error! opening file");
                    //exit(1);
                    printf("File not found\n");
                    char *r = "FILE NOT FOUND";
                    write(newsockfd, r, strlen(r));
                    break;
                }
                bzero(num, 256);
                fgets(num, 256, fptr);
//                printf("read from file, contents: %s\n", num);
                // TODO error check for read
                q = write(newsockfd,num,strlen(num));
                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                fclose(fptr);
                break;

            case 3:
                bzero(w, 255);
//                printf("DELETE request case\n");
//                printf("File location of delete file : %s\n",src);
                rm = remove(src);
                if(rm==0){
//                    printf("File deleted successfully\n");
                    strcpy(w, "FILE IS DELETED SUCCESSFULLY");
                }
                else{
                    printf("File not deleted successfully\n");
                    printf("File not found\n");
                    strcpy(w, "FILE NOT FOUND AND IS NOT DELETED");
                }
                q=  write(newsockfd, w, strlen(w));

                if (q < 0)
                {
                    error("ERROR writing to socket");
                }
                  printf("%s\n",w);
                break;
        }
    }

    return 0;
}
