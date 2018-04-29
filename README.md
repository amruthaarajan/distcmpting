# Key Value Store - Client Server using TCP
## Introduction:
This project is an application with Client-Server communication through a socket. The application used here is Key-Value store. 

## Implementation: 
Four operations are done. (1:PUT,2:GET,3:DELETE,4:QUIT). Input is got from the client and sent to server for processing. The processed data is sent back to the client from the server and is displayed in the output.

1. Files : 
We have two files namely 'serverTCP3.c' and 'clientTCP3.c'. 'serverTCP3.c' does various operations in server after reading the buffer and parsing them as characters from char[] array. The operations are done as seperate cases in switch() by identifying the operation integer. 'clientTCP3.c' reads the input and writes it to the buffer in client which is a char[]. Operations, Keys and Values of the user-input are delimited by ':', ';' and '\0'.


2. Operations:
      1. PUT: 
            - looks into buffer
            - opens a file
            - changes the file name as the given key
            - puts the value in the file
            - file close
      2. GET:
            - opens the file which is named after key
            - gets the value in that file
            - closes the file
            - puts the value in the buffer
      3. DELETE: 
            - checks for the availability of the file in the directory. 
            - if found, deletes the file. 
      4. QUIT :
            - quits from the operation

## Usage:
### Server
- For compiling server:  gcc serverTCP3.c -o server
- For starting server :  ./server portno
### Client
- For compiling client:  gcc clientTCP3.c -o client
- For running client  :  ./client hostname portno

/* Server should run before the client. */ 

## Example:
### Terminal 1:
1. gcc serverTCP3.c -o server
2. ./server 9000
### Terminal 2:
1. gcc clientTCP3.c -o client
2. ./client localhost 9000


## Note: 
More documentation can be found inline in code
