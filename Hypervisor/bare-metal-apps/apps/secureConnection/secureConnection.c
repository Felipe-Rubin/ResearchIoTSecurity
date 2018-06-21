/*
Copyright (c) 2016, prpl Foundation

Permission to use, copy, modify, and/or distribute this software for any purpose with or without 
fee is hereby granted, provided that the above copyright notice and this permission notice appear 
in all copies.

THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS SOFTWARE
INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE 
FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, 
ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.

This code was written by Carlos Moratelli at Embedded System Group (GSE) at PUCRS/Brazil.

This code was modified by Felipe Pfeifer Rubin

*/

/* Simple TCP listener server using picoTCP stack 

   To compile this application, first download the picoTCP sources from:
   https://github.com/tass-belgium/picotcp/releases/tag/prpl-v0.1. Then, compile with:
      
	make CROSS_COMPILE=mips-mti-elf- PLATFORM_CFLAGS="-EL -Os -c -Wa,-mvirt -mips32r5 -mtune=m14k \
	-mno-check-zero-division -msoft-float -fshort-double -ffreestanding -nostdlib -fomit-frame-pointer \
	-G 0" DHCP_SERVER=0 SLAACV4=0 TFTP=0 AODV=0 IPV6=0 NAT=0 PING=1 ICMP4=1 DNS_CLIENT=0 MDNS=0 DNS_SD=0 \
	SNTP_CLIENT=0 PPP=0 MCAST=1 MLD=0 IPFILTER=0 ARCH=pic32

   The compiled picoTCP directory tree must be at the same directory level of the prpl-hypervisor, 
   example:
   
   ~/hyper
        /prp-hypervisor
        /picotcp
        
   Once the application is compiled and uploaded to the board, you can use telnet or nc (netcat)
   to interact with this demo connecting to the 192.168.0.2 port 80. 
*/


#include <pico_defines.h>
#include <pico_stack.h>
#include <pico_ipv4.h>
#include <pico_tcp.h>
#include <pico_socket.h>

#include <arch.h>
#include <eth.h>
#include <guest_interrupts.h>
#include <hypercalls.h>
#include <platform.h>
#include <libc.h>
#include <eth.h>

#define CON_LISTENING_PORT 80
#define CON_MAX_CONNECTIONS 1
#define CON_IP_ADDRESS "192.168.0.2"
#define CON_MASK "255.255.255.0"

/* Max Tries to use a password */
#define MAX_TRIES 3
static char welcome_msg[] = "This is a restricted area\n";
static char askpubkey_msg[] = "\nPassword: ";
static char ok_msg[] = "\nConnection Established.\n";
static char fail_msg[] = "\nWrong Password, your ip has been registered.\n";
static char dummy_msg[] = "\nDummy msg.\n";

#define PASS_BUFFER_SIZE 128
static char passbuffer[PASS_BUFFER_SIZE];

static struct pico_socket *socket = NULL;
static struct pico_ip4 my_eth_addr, netmask;

volatile unsigned int pico_ms_tick = 0;
static uint32_t cp0_ms_ticks = CPU_SPEED/2/1000;

static uint32_t alreadyverified = 0;

void swap_endian(unsigned int *x)
{
    *x = (*x>>24) | ((*x<<8) & 0x00FF0000) |((*x>>8) & 0x0000FF00) | (*x<<24);
}



/* Returns 0 if password is wrong, 1 if right */
/* FIXME: change this please */
static void convert_input(uint8_t* data)
{
    uint32_t i = 0;
    uint32_t j = 0;
    for(j = 0; j < 128; j+=2){
        char c = passbuffer[j];
        char c2 = passbuffer[j+1];

        if(c == '0') data[i] = 0x00;
        else if(c == '1')data[i] = 0x10;
        else if(c == '2')data[i] = 0x20;
        else if(c == '3')data[i] = 0x30;
        else if(c == '4')data[i] = 0x40;
        else if(c == '5')data[i] = 0x50;
        else if(c == '6')data[i] = 0x60;
        else if(c == '7')data[i] = 0x70;
        else if(c == '8')data[i] = 0x80;
        else if(c == '9')data[i] = 0x90;
        else if(c == 'a')data[i] = 0xA0;
        else if(c == 'b')data[i] = 0xB0;
        else if(c == 'c')data[i] = 0xC0;
        else if(c == 'd')data[i] = 0xD0;
        else if(c == 'e')data[i] = 0xE0;
        else if(c == 'f')data[i] = 0xF0;
        else if(c == 'A')data[i] = 0xA0;
        else if(c == 'B')data[i] = 0xB0;
        else if(c == 'C')data[i] = 0xC0;
        else if(c == 'D')data[i] = 0xD0;
        else if(c == 'E')data[i] = 0xE0;
        else if(c == 'F')data[i] = 0xF0;
        else data[i] = 0x00;       


        if(c2 == '0') data[i] += 0x00;
        else if(c2 == '1')data[i] += 0x01;
        else if(c2 == '2')data[i] += 0x02;
        else if(c2 == '3')data[i] += 0x03;
        else if(c2 == '4')data[i] += 0x04;
        else if(c2 == '5')data[i] += 0x05;
        else if(c2 == '6')data[i] += 0x06;
        else if(c2 == '7')data[i] += 0x07;
        else if(c2 == '8')data[i] += 0x08;
        else if(c2 == '9')data[i] += 0x09;
        else if(c2 == 'a')data[i] += 0x0A;
        else if(c2 == 'b')data[i] += 0x0B;
        else if(c2 == 'c')data[i] += 0x0C;
        else if(c2 == 'd')data[i] += 0x0D;
        else if(c2 == 'e')data[i] += 0x0E;
        else if(c2 == 'f')data[i] += 0x0F;
        else if(c2 == 'A')data[i] += 0x0A;
        else if(c2 == 'B')data[i] += 0x0B;
        else if(c2 == 'C')data[i] += 0x0C;
        else if(c2 == 'D')data[i] += 0x0D;
        else if(c2 == 'E')data[i] += 0x0E;
        else if(c2 == 'F')data[i] += 0x0F;
        else data[i] += 0x00;        
        i++;
    }
}

static int verifyPassword()
{
    uint8_t actualdata[64];
    memset(actualdata,0x0,64);
    convert_input(actualdata);
    uint32_t i;
    printf("0=%c.%x\n",passbuffer[0],(uint8_t*)actualdata[0]);

    if(!get_allowed_update(actualdata))
        return 0;
    return 1;
}
//FROM Moratelli's tcp-listener examples
static uint32_t calculate_ms_passed(uint32_t old_ticks, uint32_t new_ticks)
{
    uint32_t diff_ticks;
    if (new_ticks >= old_ticks)
        diff_ticks = new_ticks - old_ticks;
    else
        diff_ticks = 0xffffffff - (old_ticks - new_ticks);

    return diff_ticks / cp0_ms_ticks;
}


//FROM Moratelli's tcp-listener example
void irq_timer()
{
    static int prev_init = 0;
    static uint32_t prev_count = 0;
    uint32_t cur_count = mfc0(CP0_COUNT, 0);

    if (!prev_init){
        prev_count = mfc0(CP0_COUNT, 0);
        prev_init = 1;
    }

    /* pico_ms_tick is not 100% accurate this way but at this point it's not required
     * currently there's a 10% accuracy loss(1000ms only produces 900 pico_ms_ticks) */
    if (cur_count >= prev_count + cp0_ms_ticks){
        pico_ms_tick += calculate_ms_passed(prev_count, cur_count);
        prev_count = cur_count;
    }

}

static void start_listening(uint16_t ev, struct pico_socket *socket)
{
    int validread = 0; //To read a message
    uint16_t port; //port of who is accessing
    uint16_t peer; //who is accessing
    int validwrite = 0; //TO write a message
    int i;
    static struct pico_socket *socket_client = NULL;
    

    /* Incomming connection */
    if(ev & PICO_SOCK_EV_CONN) {
        socket_client = pico_socket_accept(socket,&peer,&port);
        printf("Incomming Connection\n");
        //sends message
        validwrite = pico_socket_write(socket_client,welcome_msg,strlen(welcome_msg));
        if(validwrite < 0) printf("Error0.0 writing message\n");
        validwrite = pico_socket_write(socket_client,askpubkey_msg,strlen(askpubkey_msg));
        if(validwrite < 0) printf("Error0.1 writing message\n");
    }

    /* Read messages from the socket */
    if(ev & PICO_SOCK_EV_RD) {
        /* Read client message */

        if(!alreadyverified){
            memset(passbuffer, 0, sizeof(passbuffer));
            validread = pico_socket_read(socket_client, passbuffer, PASS_BUFFER_SIZE);
            if (validread < 0) printf("Error2.0 while reading from socket!\n");
            
            /* Check if received public key is valid */
            int validpass = verifyPassword();

            if(!validpass || 1){ //It is not
                validwrite = pico_socket_write(socket_client,fail_msg,strlen(fail_msg));
                if(validwrite < 0) printf("Error2.1 writing message\n");
            }else{//It is 
                validwrite = pico_socket_write(socket_client,ok_msg,strlen(ok_msg));
                if(validwrite < 0) printf("Error2.2 writing message\n");
                alreadyverified = 1;
            }
        }else{//Already verified
            validwrite = pico_socket_write(socket_client,dummy_msg,strlen(dummy_msg));
            if(validwrite < 0) printf("Error3.1 writing message\n");
        }
    }


    /* process error event, socket error occured */
    if(ev & PICO_SOCK_EV_ERR){
        printf("Socket error!\n");
    }
    
    /* Client closed the connection. */
    if(ev & PICO_SOCK_EV_CLOSE){
        printf("Client Closed Connection\n");
    }

}

int main()
{
    printf("Starting secureConnection...\n");

    uint8_t macaddress[6];
    uint32_t timer = 0;

    /* Get Mac Address */
    eth_mac(macaddress);

    uint16_t listen_port = 0;

    interrupt_register(irq_timer,GUEST_TIMER_INT);

    /*Select output serial 2 = UART2, 6 = UART6 */
    serial_select(UART2);

    /* Configure the virtual ethernet driver */
    struct pico_device* device = PICO_ZALLOC(sizeof(struct pico_device));
    if(!device) {
        printf("Device not found\n");
        return 0;
    }

    device->send = eth_send;
    device->poll = eth_poll;
    device->link_state = eth_link_state;



    
    if(0 != pico_device_init((struct pico_device *)device, "virt-eth", macaddress)){
        printf ("Device init failed.\n");
        PICO_FREE(device);
        return 0;
    }


    /* picoTCP initialization */
    printf("Starting picoTCP...\n");
    pico_stack_init();

    pico_string_to_ipv4(CON_IP_ADDRESS, &my_eth_addr.addr);
    pico_string_to_ipv4(CON_MASK, &netmask.addr);
    pico_ipv4_link_add(device, my_eth_addr, netmask);
    listen_port = short_be(CON_LISTENING_PORT);
    printf("Opening socket...\n");

    socket = pico_socket_open(PICO_PROTO_IPV4, PICO_PROTO_TCP, &start_listening);

    if (!socket)
        printf("Failed to open socket\n");
    if (pico_socket_bind(socket, &my_eth_addr.addr, &listen_port) != 0)
        printf("Failed to bind socket!\n");

    if (pico_socket_listen(socket,CON_MAX_CONNECTIONS) != 0)
        printf("Failed to listen on socket!\n");

    printf("Secure Connection is Up\n");
    while (1){
        eth_watchdog(&timer, 500);
        /* pooling picoTCP stack */
        pico_stack_tick(); /* Do the pooling */
    }



}



























