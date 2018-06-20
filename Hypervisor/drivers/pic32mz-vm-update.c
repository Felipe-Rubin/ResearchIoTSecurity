/*
	Author: Felipe Pfeifer Rubin

 */

/**
 * @file pic32mz-vm-update.c
 * @section DESCRIPTION
 * 	Add the function to update a single VM
 *
 * 
 */
#include <uECC.h>
#include <sha256.h>
#include <security.h>
//
#include <types.h>
#include <pic32mz.h>
#include <hal.h>
#include <globals.h>
#include <scheduler.h>
#include <hypercall_defines.h>
#include <hypercall.h>
#include <libc.h>
#include <driver.h>
#include <mips_cp0.h>
#include <globals.h>
#include <interrupts.h>

#include <linkedlist.h>
#include <vcpu.h>






// static  unsigned char testkeymapped[] = {
//         0x61,0x6e,0x2b,0xcf,0xf1,0x95,0x41,0x24,
//         0x21,0xb3,0xf6,0xfc,0xbc,0x11,0x90,0xbe,
//         0x35,0xec,0xb0,0xbd,0x12,0x45,0xfb,0x45,
//         0x3c,0x0f,0x09,0xf7,0xee,0x2b,0x5e,0x3e,
//         0xe0,0xd6,0xa6,0x68,0x9b,0xde,0xb1,0x54,
//         0xe0,0x5a,0x1d,0x4a,0xd1,0xd9,0x4f,0x83,
//         0x87,0x5d,0xe8,0x81,0x00,0x2f,0x19,0x90,
//         0x1b,0xd2,0x12,0x8d,0xc9,0x6a,0xc1,0x23
//     };

	unsigned char testkeymapped[] = {
        0x61,0x6e,0x2b,0xcf,0xf1,0x95,0x41,0x24,
        0x21,0xb3,0xf6,0xfc,0xbc,0x11,0x90,0xbe,
        0x35,0xec,0xb0,0xbd,0x12,0x45,0xfb,0x45,
        0x3c,0x0f,0x09,0xf7,0xee,0x2b,0x5e,0x3e,
        0xe0,0xd6,0xa6,0x68,0x9b,0xde,0xb1,0x54,
        0xe0,0x5a,0x1d,0x4a,0xd1,0xd9,0x4f,0x83,
        0x87,0x5d,0xe8,0x81,0x00,0x2f,0x19,0x90,
        0x1b,0xd2,0x12,0x8d,0xc9,0x6a,0xc1,0x23
    };

/**
 * @brief Update the VM chosen
 * Input a0: VM ID
 * Input a1: Firmware Update buffer pointer
 * Input a2: Firmware size
 * Output v0: 1 = success or 0 = failure
 */
static void update_chosen_vm(){

	/* Get arguments from regs */
	vcpu_t *vcpu;
	uint32_t vmid = MoveFromPreviousGuestGPR(REG_A0);
	
	/* Check again if the chosen VM signature matches the firmware*/


	 /*Stops VM */
 // 	struct list_t *aux;
 	
 // 	aux = scheduler_info.vcpu_ready_list;
 	
 // 	if(((vcpu_t*)aux->elem)->id != vmid){ //No need to search if it Head elem
 // 		while(aux->next != NULL && ((vcpu_t*)aux->next->elem)->id != vmid){
 // 			aux = aux->next;
	// 	}
 // 	}

	// if(((vcpu_t*)aux->elem)->id == vmid){ //It was the Head Element
	// 	printf("Era Head\n");
	// 	aux = aux->next;
	// 	scheduler_info.vcpu_ready_list = aux;
	// }else if(aux->next != NULL){ //Wasn't Head Element
	// 	aux->next = aux->next->next;
	// 	printf("N era Head\n");
	// }

	//Begin Test READ FLASH
	
	// uint32_t ret;
	
	// uint8_t *dest = uint8_t * dest = (uint8_t *) tlbCreateEntry((uint32_t) MoveFromPreviousGuestGPR(REG_A0), vm_executing->base_addr, sizeof(uint8_t) * 1024, 0xf, CACHEABLE);
	
	//END Test READ FLASH

	/* Write on Flash*/

	/*Restart VM*/

	/*Return success*/
}

/**
 * @brief Check Signature, Returns list of VM's to choose from
 * Input: a0: Firmware Update buffer pointer
 * Input: a1: Firmware size
 * Output: v0: List of VM 
 */

static int verifyPublicKey(vm_t *vm, uint8_t* public)
{
	uint32_t i;	
	if (!uECC_valid_public_key(public, uECC_secp256k1())) {
		return 0;
    }
	int countPubKey = 0;
	int countSign = 0;
    long sizeVm = vm->vmconf->flash_size;
	unsigned char *lAddrVm = (unsigned char*) vm->vmconf->flash_base_add;
	long sizeHash = sizeVm - 128; //only the size to calculate hash of vm
    
 //    for (countPubKey = 0; countPubKey < 64; countPubKey++) {
 //    	testkey[countPubKey] = lAddrVm[(sizeVm - 128) + countPubKey];
	// }
	
	// Read Signature From Flash 
	uint8_t sigReceived[64];
	for (countSign = 0; countSign < 64; countSign++) {
        sigReceived[countSign] = lAddrVm[(sizeVm - 64) + countSign];
    }

	SHA256_CTX contextHash;
	BYTE buf[SHA256_BLOCK_SIZE];

	sha256_init(&contextHash);
	sha256_update(&contextHash, lAddrVm, sizeHash);
	sha256_final(&contextHash, buf);


	int verified = uECC_verify(public, buf, sizeof (buf), sigReceived, uECC_secp256k1());

	return verified;
	// return 0;
}


static void get_allowed_vm(void){
	// vcpu_t *vcpu;

	//Char or UINT8_T ?
	/* uint32_t publicKey[16] */
	// uint32_t* updt_buffr = (char*)MoveFromPreviousGuestGPR(REG_A0);
	 /* Public key to test */
	//Need to use TLB
	// uint8_t *testkey = (uint8_t*) tlbCreateEntry(
	// (uint32_t) MoveFromPreviousGuestGPR(REG_A0),
	//  vm_executing->base_addr,sizeof(uint8_t) * 64,
	//  0xf,CACHEABLE);
	
	// *testkey  = (uint8_t *)MoveFromPreviousGuestGPR(REG_A0);
	// uint8_t *testkeymapped = (uint8_t*) tlbCreateEntry(
	// (uint32_t) MoveFromPreviousGuestGPR(REG_A0),
	//  vm_executing->base_addr,sizeof(uint8_t) * 64,
	//  0xf,CACHEABLE);


	// uint32_t *testkeymappedaux =  tlbCreateEntry(
	// (uint32_t) MoveFromPreviousGuestGPR(REG_A0),
	//  vm_executing->base_addr,sizeof(uint32_t) * 16,
	//  0xf,CACHEABLE);


	   // printf("uint8_t PubKey[2*NUM_ECC_DIGITS] = {");
	   // vli_print(testkeymappedaux, sizeof(uint8_t) * 64);
	   // printf("};\n");
	   // printf("\n\n");

	uint8_t *testkey = (uint8_t*)tlbCreateEntry(
	(uint32_t) MoveFromPreviousGuestGPR(REG_A0),
	 vm_executing->base_addr,sizeof(uint8_t)*64,//sizeof(uint32_t) * 16,
	 0xf,CACHEABLE);



	uint32_t i;

	printf("\n{");
	for(i = 0; i < 64;i++){
		printf("%x,",(uint8_t*)testkey[i]);
	}
	printf("}\n");



	/* Returns Ok!*/

	/*Received Public Key*/
	/*Check for each VM*/
	
	vcpu_t *vcpu = NULL;
	vm_t* vm = NULL;

	struct list_t* aux = scheduler_info.virtual_machines_list;
	while(aux){
		vm = (vm_t*)aux->elem; //acquire vm_t pointer
		if(!verifyPublicKey(vm,testkey))printf("Fail\n");
		else printf("Success\n");
		aux = aux->next;
		//read flash
			
	}
	



	/* Read Last part of buffer and saves VM signature */
	
	/* Reads For each VCPU with same signature, add it to the list...*/
	// uint32_t i;
	uint32_t ret = 1;
	MoveToPreviousGuestGPR(REG_V0, ret);


}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */
static void vm_update_init(){
	if(register_hypercall(update_chosen_vm,HCALL_UPDATE_VM_ID) < 0){
		ERROR("Error registering the HCALL_UPDATE_VM_ID hypercall.");
		return;
	}

	if(register_hypercall(get_allowed_vm,HCALL_UPDATE_GET_ALLOWED) < 0){
		ERROR("Error registering the HCALL_GET_ALLOWED hypercall.");
		return;
	}

	INFO("Driver for VM Update enabled.");

}
driver_init(vm_update_init);













