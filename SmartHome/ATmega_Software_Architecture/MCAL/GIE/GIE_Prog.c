/*
 * GIE_Prog.c
 *
 * Created: 10/7/2023 8:57:16 PM
 *  Author: Beast
 */ 

void GIE_vidEnable(){
	
	asm("sei");					// Enables the Global Interrupt Enabler bit in the states register
	
}

void GIE_vidDisable(){
	
	asm("cli");					// Disables the Global Interrupt Enabler bit in the states register
	
}