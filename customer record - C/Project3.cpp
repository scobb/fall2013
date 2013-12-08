/* 
 * EE312 Simple CRM Project
 *  Steven Cobb
	scc2448
	Duncan Horn
	F: 1-1:50
	
 */


//---------INCLUDES-------------
#include <stdio.h>
#include <stdint.h>
#include <assert.h>
#include "String.h"
#include "Invent.h"


//-------DEFINITIONS-------------
#define MAX_CUSTOMERS 1000

//---------GLOBALS---------------
Customer customers[MAX_CUSTOMERS];
int32_t num_customers = 0;										
int32_t num_diapers = 0;
int32_t num_rattles = 0;
int32_t num_bottles = 0;


/* Reset------------------------
Inputs: none
Outputs: none

Cleans up global variables
-------------------------------*/
void reset(void) {
	int32_t i;
	for (i=0; i<num_customers; i++){                            //cycle through active customers, destroy their names
		StringDestroy(&customers[i].name);
		customers[i].diapers = 0;
		customers[i].rattles = 0;
		customers[i].bottles = 0;
	}
	num_customers = 0;											//reset globals
	num_diapers = 0;
	num_rattles = 0;
	num_bottles = 0;
}

/*	processSumarize -------------------------------------
	Inputs: none [parsed text file: <Summarize>]
	Outputs: none

	Prints a summary including remaining inventory, number 
	of customers, and customers with the highest-quantity
	purchases
-------------------------------------------------------	*/

void processSummarize() {
	printf("There are %d diapers, %d bottles and %d rattles in inventory\n",num_diapers,num_bottles,num_rattles);
	printf("we have had a total of %d different customers\n",num_customers);
	int32_t i;
	int32_t most_diapers = 0;										//variables to keep track of the index and number of superlatives
	int32_t most_bottles = 0;
	int32_t most_rattles = 0;
	int32_t most_diapers_ind = -1;
	int32_t most_bottles_ind = -1;
	int32_t most_rattles_ind = -1;
	for (i=0;i<num_customers;i++){                                  //loop to search for superlatives
		if (customers[i].diapers > most_diapers){
			most_diapers = customers[i].diapers;
			most_diapers_ind=i;
		}
		if (customers[i].bottles > most_bottles){
			most_bottles = customers[i].bottles;
			most_bottles_ind = i;
		}
		if (customers[i].rattles > most_rattles){
			most_rattles = customers[i].rattles;
			most_rattles_ind = i;
		}
	}
	if (most_diapers_ind >= 0){
		StringPrint(&customers[most_diapers_ind].name);
		printf(" has purchased the most diapers (%d)\n", most_diapers);
	}
	else {
		printf("no one has purchased any diapers\n");
	}
	if (most_bottles_ind >= 0){
		StringPrint(&customers[most_bottles_ind].name);
		printf(" has purchased the most bottles (%d)\n", most_bottles);
	}
	else {
		printf("no one has purchased any bottles\n");
	}
	if (most_rattles_ind >= 0){
		StringPrint(&customers[most_rattles_ind].name);
		printf(" has purchased the most rattles (%d)\n", most_rattles);
	}
	else {
		printf("no one has purchased any rattles\n");
	}
	printf("\n");
	return;
}


/*	processPurchase -------------------------------------------------------------
	Inputs: none [parsed text file, <Purchase> <Customer> <Item> <Quantity>]
	Outputs: none

	Parses purchase information. If command is valid, customer's account either
	created or updated, as is inventory. If order cannot be fulfilled due to 
	inventory constraints, a message is printed to the customer
	----------------------------------------------------------------------------*/
void processPurchase() {
	String Diapers, Bottles, Rattles,customer,item;
	Diapers = StringCreate("Diapers");						//keywords
	Bottles = StringCreate("Bottles");
	Rattles = StringCreate("Rattles");
	int32_t quantity;
	int32_t i;
	uint8_t customerExists = 0;
	readString(&customer);
	readString(&item);
	readNum(&quantity);
	if (quantity <= 0){                                     //if someone tries to enter a negative number or 0, clean up and return
		StringDestroy(&Diapers);
		StringDestroy(&Bottles);
		StringDestroy(&Rattles);
		StringDestroy(&customer);
		StringDestroy(&item);
		return;
	}
	for (i = 0; i< num_customers && customerExists == 0; i++){
		if (StringIsEqualTo(&customer,&(customers[i]).name)){
			customerExists = 1;										//i-1 now stores the index of the customer, unless customer is new
		}
	}
	if (customerExists == 0)											//for new customers
		{
			customers[num_customers].name = StringDup(&customer);	
			customers[num_customers].diapers = 0;
			customers[num_customers].bottles = 0;
			customers[num_customers].rattles = 0;
			num_customers++;
			i++;											//now i-1 stores the index of the customer for everyone
	}
	if (StringIsEqualTo(&item,&Diapers)){
		if (num_diapers >= quantity){
			num_diapers -= quantity;
			customers[i-1].diapers += quantity;
		}
		else {
			printf("Sorry ");
			StringPrint(&customer);
			printf(", we only have %d ",num_diapers);
			StringPrint(&item);
			printf("\n");
			if (customerExists == 0){                                  //if a first-time customer's purchase doesn't go through
				num_customers--;
				StringDestroy(&customers[num_customers].name);
			}
		}
	}
	else if (StringIsEqualTo(&item,&Bottles)){
		if (num_bottles >= quantity){
			num_bottles -= quantity;
			customers[i-1].bottles += quantity;
		}
		else{
			printf("Sorry ");
			StringPrint(&customer);
			printf(", we only have %d ",num_bottles);
			StringPrint(&item);
			printf("\n");
			if (customerExists == 0){                                    //if a first-time customer's purchase doesn't go through
				num_customers--;
				StringDestroy(&customers[num_customers].name);
			}
		}
	}
	else if (StringIsEqualTo(&item,&Rattles)){
		if (num_rattles >= quantity){
			num_rattles -= quantity;
			customers[i-1].rattles += quantity;
		}
		else {
			printf("Sorry ");
			StringPrint(&customer);
			printf(", we only have %d ",num_rattles);
			StringPrint(&item);
			printf("\n");
			if (customerExists == 0){                                    //if a first-time customer's purchase doesn't go through
				num_customers--;
				StringDestroy(&customers[num_customers].name);
			}

		}

	}
	else if (customerExists == 0){                         //new customer purchases inventory item that doesn't exist
		num_customers--;						 
		StringDestroy(&customers[num_customers].name);
		
	}
	StringDestroy(&Diapers);					//cleanup
	StringDestroy(&Bottles);
	StringDestroy(&Rattles);
	StringDestroy(&customer);
	StringDestroy(&item);
	return;
}

/*	processInventory----------------------------------------------------
	Inputs: none [parsed text: <Inventory> <Item> <Quantity>]
	Outputs: none

	Upon receiving a valid command, updates the inventory

	--------------------------------------------------------------------*/
void processInventory() { 
	String item;
	String Diapers = StringCreate("Diapers");	//keywords
	String Bottles = StringCreate("Bottles");
	String Rattles = StringCreate("Rattles");
	int32_t quantity;
	readString(&item);
	readNum(&quantity);
	if (quantity <= 0){
		StringDestroy(&Diapers);				//if quantity <= 0, clean up and get on with it
		StringDestroy(&Bottles);
		StringDestroy(&Rattles);
		StringDestroy(&item);
		return;
	}
	if (StringIsEqualTo(&item,&Diapers)){       //checking for item keywords, then updating proper quantity	
		num_diapers += quantity;
	}
	else if (StringIsEqualTo(&item,&Bottles)){
		num_bottles += quantity;
	}
	else if (StringIsEqualTo(&item,&Rattles)){
		num_rattles += quantity;
	}
	StringDestroy(&Diapers);					//cleanup
	StringDestroy(&Bottles);
	StringDestroy(&Rattles);
	StringDestroy(&item);
	return;
}