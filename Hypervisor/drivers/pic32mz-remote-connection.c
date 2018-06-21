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
#include <globals.h>
#include <scheduler.h>
#include <hypercall_defines.h>
#include <libc.h>
#include <driver.h>
#include <mips_cp0.h>

/**
 * @brief Check Signature, Returns list of VM's to choose from
 * Input: a0: Firmware Update buffer pointer
 * Input: a1: Firmware size
 * Output: v0: List of VM 
 */
static int verifyPublicKey(vm_t *vm, uint8_t* public)
{
	uint32_t i;
	int countPubKey = 0;
	int countSign = 0;
    long sizeVm = vm->vmconf->flash_size;
	unsigned char *lAddrVm = (unsigned char*) vm->vmconf->flash_base_add;
	long sizeHash = sizeVm - 128; //only the size to calculate hash of vm

   	uint8_t testkey[64];
    for (countPubKey = 0; countPubKey < 64; countPubKey++) {
    	testkey[countPubKey] = lAddrVm[(sizeVm - 128) + countPubKey];
    	if(public[countPubKey] != testkey[countPubKey]) return 0;
	}

	if (!uECC_valid_public_key(testkey, uECC_secp256k1())) return 0;

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

	int verified = 0;
	verified = uECC_verify(testkey, buf, sizeof (buf), sigReceived, uECC_secp256k1());

	
	
	/* This would mean that the key from flash is invalid */
	/* Which also means that in the next hw reset this VM won't boot */
	if(!verified) return 0;

	return 1;
}

static void get_owned_list(void){

	printf("Trying to verify\n");

	uint8_t *testkey = (uint8_t*)tlbCreateEntry(
	(uint32_t) MoveFromPreviousGuestGPR(REG_A0),
	 vm_executing->base_addr,sizeof(uint8_t)*64,//sizeof(uint32_t) * 16,
	 0xf,CACHEABLE);
	/*Received Public Key*/


	/* Prints it */
	uint32_t i;
	printf("\n{");
	for(i = 0; i < 64;i++){
		printf("%x",(uint8_t*)testkey[i]);
	}
	printf("}\n");
	

	


	/*Check for each VM*/
	
	vcpu_t *vcpu = NULL;
	vm_t* vm = NULL;
	char c0 = ':';
	char c1 = '\n';
	struct list_t* aux = scheduler_info.virtual_machines_list;
	int index = 0;
	int ret = 0;
	while(aux){
		vm = (vm_t*)aux->elem; //acquire vm_t pointer
		if(verifyPublicKey(vm,testkey)) ret++; //Check Vm's that have this key
		aux = aux->next;
	}
	
	/* Returns how many Vms have this key */
	MoveToPreviousGuestGPR(REG_V0, ret);
}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */

static void remote_connection_init(){
	if(register_hypercall(get_owned_list,HCALL_GET_OWNED_LIST) < 0){
		ERROR("Error registering the HCALL_GET_OWNED_LIST hypercall.");
		return;
	}
	INFO("Driver for Remote Connection Enabled.");

}
driver_init(remote_connection_init);













