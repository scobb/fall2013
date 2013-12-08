/*--Project6.cpp------------------------------------

	Author: Steve Cobb
	TA: Duncan Horn
	Session: F, 1-1:50

----------------------------------------------------*/
#include <stdio.h>
#include "String.h"
#include "Customer.h"
#include "CustomerDB.h"
#include <cassert>

void readString(String&);
void readNum(int&);

CustomerDB database;

int num_bottles = 0;
int num_rattles = 0;
int num_diapers = 0;


/* clear the inventory and reset the customer database to empty */
void reset(void) {
	database.clear();
	num_bottles = 0;
	num_rattles = 0;
	num_diapers = 0;
}

/*
 * selectInventItem is a convenience function that allows you
 * to obtain a pointer to the inventory record using the item type name
 * word must be "Bottles", "Diapers" or "Rattles"
 * for example the expression *selectInventItem("Bottles") returns the 
 * current global variable for the number of bottls in the inventory
 */
int* selectInventItem(String word) {
	if (word == "Bottles") {
		return &num_bottles;
	} else if (word == "Diapers") {
		return &num_diapers;
	} else if (word == "Rattles") {
		return &num_rattles;
	}
	
	/* NOT REACHED */
	return 0;
}

/*
 * this overload of selectInventItem is similar to selectInventItem above, however
 * this overload takes a Customer as a second argument and selects the data member
 * of that Customer that matches "word". The function returns a pointer to one of the three data
 * members from the specified customer.
 */
int* selectInventItem(String word, Customer& cust) {
	if (word == "Bottles") {
		return &cust.bottles;
	} else if (word == "Diapers") {
		return &cust.diapers;
	} else if (word == "Rattles") {
		return &cust.rattles;
	}
	
	/* NOT REACHED */
	return 0;
}


/*
 * findMax searches through the CustomerDB "database" and returns the Customer
 * who has purchased the most items of the specified type.
 * type must be one of "Bottles", "Rattles" or "Diapers".
 * 
 * Note: if two or more Customers are tied for having purchased the most of that item type
 * then findMax returns the first Customer in the CustomerDB who has purchased that maximal
 * quantity.
 *
 * Note: in the special case (invalid case) where there are zero Customers in the 
 * CustomerDB, fundMax returns a null pointer (0)
 */
Customer* findMax(String type) {
	Customer* result = 0;
	int max = 0;
	for (int k = 0; k < database.size(); k += 1) {
		Customer& cust = database[k];
		int *p = selectInventItem(type, cust); 
		if (*p > max) {
			max = *p;
			result = &cust;
		}
	}
	
	return result;
}
/*--ProcessPurchase-------------------------------------------------------------
	
	Inputs: [parsed text file]
	Post-Condition: customer database and inventory updated

------------------------------------------------------------------------------*/


void processPurchase() {

	//**locals**********

	String name;
	String itemType;
	int32_t quantity;

	//**parse***********

	readString(name);
	readString(itemType);
	readNum(quantity);


	//**implementation**

	if (quantity <= 0 || selectInventItem(itemType) == 0)					//safeties on input
		return;

	if (quantity <= *selectInventItem(itemType)){
		*selectInventItem(itemType, database[name]) += quantity;			//update customer quantity
		*selectInventItem(itemType) -= quantity;							//update inventory quantity
	}

	else printf ("Sorry %s, we only have %d %s\n", name.c_str(), *selectInventItem(itemType), itemType);

}

/*--ProcessSummarize----------------------------------------------------------------
	
	Inputs: none
	Outputs: none

	This prints to the screen a summary of remaining inventory, number of customers,
	and the customers who purchased the most of each product (if any were purchased)

-----------------------------------------------------------------------------------*/

void processSummarize() {
	printf("There are %d diapers, %d bottles and %d rattles in inventory\n", num_diapers, num_bottles, num_rattles);
	printf("we have had a total of %d different customers\n", database.length);

	Customer* most_diapers_ptr = findMax("Diapers");
	Customer* most_rattles_ptr = findMax("Rattles");
	Customer* most_bottles_ptr = findMax("Bottles");

	if (most_diapers_ptr){
		printf("%s has purchased the most diapers (%d)\n", most_diapers_ptr->name.c_str(),most_diapers_ptr->diapers);
	}
	else printf("no one has purchased any diapers\n");


	if (most_bottles_ptr){
		printf("%s has purchased the most bottles (%d)\n", most_bottles_ptr->name.c_str(),most_bottles_ptr->bottles);
	}
	else printf("no one has purchased any bottles\n");


	if (most_rattles_ptr){
		printf("%s has purchased the most rattles (%d)\n", most_rattles_ptr->name.c_str(),most_rattles_ptr->rattles);
	}
	else printf("no one has purchased any rattles\n");

}

/*--ProcessInventory-------------------------------------------------------------
	
	Inputs: [parsed text file]
	Post-Condition: inventory updated

------------------------------------------------------------------------------*/

void processInventory() {

	//**locals**********
	int32_t quantity;
	String itemType;

	//**parse***********
	readString(itemType);
	readNum(quantity);

	//**implementation**

	if (quantity <= 0 || selectInventItem(itemType) == 0)			//safeties on input
		return;

	*selectInventItem(itemType) += quantity;

}