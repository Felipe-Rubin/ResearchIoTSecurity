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

/**
 * @brief Update the VM chosen
 * Input a0: VM ID
 * Input a1: Firmware Update buffer pointer
 * Input a2: Firmware size
 * Output v0: 1 = success or 0 = failure
 */
void update_chosen_vm(){

	/* Get arguments from regs */
	vcpu_t *vcpu;
	uint32_t vmid = MoveFromPreviousGuestGPR(REG_A0);
	
	/* Check again if the chosen VM signature matches the firmware*/
	

	 
	 /*Stops VM */
 	struct list_t *aux;
 	
 	aux = scheduler_info.vcpu_ready_list;

 	if(((vcpu_t*)aux->elem)->id != vmid){ //Se for Head ja tenho, n preciso procurar
 		while(aux->next != NULL && ((vcpu_t*)aux->next->elem)->id != vmid){
 			aux = aux->next;
		}
 	}

	if(((vcpu_t*)aux->elem)->id == vmid){ //It was the Head Element
		printf("Era Head\n");
		aux = aux->next;
		scheduler_info.vcpu_ready_list = aux;
	}else if(aux->next != NULL){ //Wasn't Head Element
		aux->next = aux->next->next;
		printf("N era Head\n");
	}

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

void get_allowed_vm(){
	vcpu_t *vcpu;

	//Char or UINT8_T ?
	char* updt_buffr = (char*)MoveFromPreviousGuestGPR(REG_A0);

	/* Read Last part of buffer and saves VM signature */
	
	/* Reads For each VCPU with same signature, add it to the list...*/
	uint32_t i;

	for(i = 0; i < NVMACHINES; i++){
		vcpu = get_vcpu_from_id(i+1);
		// vm_t
	}

}

/**
 * @brief Driver init call.  Registers the hypercalls. 
 */
void vm_update_init(){
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













