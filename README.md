# Key Value Store- Client Server using TCP
Introduction:
          This project is an application with Client-Server communication through a socket. The application used here is Key-Value store. 

Implementation: Four operation are done. (1:PUT,2:GET,3:DELETE,4:QUIT). Input is got from the client and sent to server for processing. The processed data is sent back to the client from the server and is displayed in the output.

1. Files : 
          We have two files namely serverTCP3.c and clientTCP3.c. serverTCP3.c does various operations after reading the buffer and parsing them as characters from char[] array. The operations are done in seperate cases in switch() by identifying the operation integer. clientTCP3.c reads the input and writes it to the buffer which is a char[]. Operations, Keys and Values of the user-input are delimited by ':', ';' and '\0'.


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
            - quits from the operations

Usage:
- For running server -   ./server portno
- For running client -   ./client hostname portno

/* Server should run before the client. */ 

Example:
- ./server 9000
- ./client localhost 9000
Enter your message: DistComputing is awesome
1. PUT
2. GET
3. DELETE
4. QUIT


Note: More documentation can be found inline in code
