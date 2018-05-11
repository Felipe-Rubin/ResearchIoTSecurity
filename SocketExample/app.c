 
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netdb.h>
#include <string.h>

// #define SIGN_SIZE 256 //Signature Size for buffer
#define SIGN_SIZE 2 //Signature Size for buffer

#define BUFF_SIZE 8 //
//https://vcansimplify.wordpress.com/2013/03/14/c-socket-tutorial-echo-server/

static char msg1[] = "\nFirmware Update Service\n";
static char msg2[] = "\nWrite your Signature and press enter: ";
static char msg3[] = "\nWrite the new Firmware and press enter: ";
static char msg4[] = "\nBeginning Update";
static char msg5[] = "\nSignature Invalid, Disconnecting";
// static char msg2[] = "\nWrite a message and press enter: ";
// static char msg3[] = "\nEcho message: ";


// Wannabe Hypercalls

/*
    Request VM List that matches signature
*/
char* RequestVMList(char sign[SIGN_SIZE])
{
    //..... check signature with each one....
    //Create List ....
    //Send List
}

int checkSignature(char sign[SIGN_SIZE]) //Seria uma hypercall
{
    //
    char validsign[SIGN_SIZE] = "a0";
    return strcmp(validsign,sign);
}

int main(int argc, const char** argv)
{


     char buffer[BUFF_SIZE];
     

     int receiverfd; // file descriptor to read/receive
     int senderfd; //file descriptor to write/send

     struct sockaddr_in receiveraddr;

     //create socket, AF_INET(Ip addressing);
     //receiverfd mapped to receive
     receiverfd = socket(AF_INET, SOCK_STREAM, 0);

     //Clear memory
     bzero(&receiveraddr,sizeof(receiveraddr));


     // Set addressing scheme to AF_INET(IP)
     receiveraddr.sin_family = AF_INET;

     //Who can connect ?
     //Any Ip address == INADDR_ANY 
     //Local only would be 127.0.0.1
     // receiveraddr.sin_addr.s_addr = htons(INADDR_ANY);
     receiveraddr.sin_addr.s_addr = htons(INADDR_ANY);

     //Which port ?
     receiveraddr.sin_port = htons(2612);


     //Prepare to receive conn from ip and port specified
     bind(receiverfd, (struct sockaddr*)&receiveraddr, sizeof(receiveraddr));


     //Start listening for connections, keep 2 at most waiting, if 3 want
     //to request, fail the 3rd
     listen(receiverfd,2);

     //If no one wants to connect at this point, it waits
     //Else it connects to it and senderfd can be used to send messages
     senderfd = accept(receiverfd,(struct sockaddr*)NULL,NULL);
        

     while(1){
        bzero(buffer,BUFF_SIZE);
        write(senderfd,msg1,strlen(msg1)+1);
        write(senderfd,msg2,strlen(msg2)+1);
        // bzero(buffer, &)

        read(senderfd,buffer,SIGN_SIZE*sizeof(char));
        if(!checkSignature(buffer)){ //Invalid Signature
            write(senderfd,msg5,strlen(msg5)+1);
            //Add something, like block IP after 2 tries ?
            close(senderfd);
        }
        write(senderfd,msg3,strlen(msg3)+1);
        bzero(buffer,SIGN_SIZE);
        read(senderfd,buffer,BUFF_SIZE*sizeof(char));



        // write(senderfd,msg2,strlen(msg2)+1);
        printf("Received %s\n",buffer);


     }

     return 0;
}

















