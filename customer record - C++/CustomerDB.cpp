/*--CustomerDB.cpp------------------------------------

	Author: Steve Cobb
	EE 312
    UT - Austin

----------------------------------------------------*/

#include <iostream>
#include <cassert>
#include "String.h"
#include "CustomerDB.h"

using namespace std;

const int default_capacity = 1;

Customer::Customer(String name) { // done, please do not edit
	this->bottles = 0;
	this->rattles = 0;
	this->diapers = 0;
	this->name = name;
}

CustomerDB::CustomerDB(void) { // done, please do not edit
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

int CustomerDB::size(void) {
	return this->length;
}

CustomerDB::~CustomerDB(void) { // done, please do not edit
	delete[] this->data;
}

void CustomerDB::clear(void) { // done, please do not edit
	delete[] this->data;
	this->capacity = default_capacity;
	this->data = new Customer[this->capacity];
	this->length = 0;
}

Customer& CustomerDB::operator[](int k) { // done, please do not edit
	assert(k >= 0 && k < this->length);
	return this->data[k];
}

/*---CustomerDB::operator[](String)------------------------------------
	
	Inputs: implicit CustomerDB structure, String of name to be searched
	Output: reference to Customer structure with searched name

	Three behaviors:

	1.  If a customer whose name is searched is already in the database, 
	a reference to their Customer struct is returned. 
	
	2.  If the customer is not in the database and there is space to add 
	another customer, database's length is incremented, a new customer 
	record is created in the database with name from the string parameter, 
	and a reference to that structure is returned.

	3.  If the customer is not in the database and there is not enough space
	to add another customer, a new database data set is created with twice
	the storage space of the previous set. The previous data is copied, the 
	database's length is incremented, and then a new customer structure is 
	instantiated with name from the string parameter. The return value is a 
	reference to the new customer structure.

	Time Complexity: O(n); in applications where it is called n times, the 
	time complexity of the entire operation would be O(n) due to amortized
	doubling

---------------------------------------------------------------------*/


Customer& CustomerDB::operator[](String name) { 

	//1.  Customer already in the database.

	for (int i = 0; i< this->length; i++){
		if (this->data[i].name == name){
			return this->data[i];
		}
	}

	//2. Customer not in the database, but we have room to expand. 

	if (this->length < this->capacity){
		this->data[this->length] = Customer(name);
		this->length++;
		return this->data[length-1];
	}

	//3. Customer not in the database, and we need to expand.

	else {
		this->capacity *= 2;								//amortized doubling
		Customer* newData = new Customer[this->capacity];	//new array of customers

		for (int i = 0; i < this-> length; i++){			//populate with the old data
			newData[i] = this->data[i];
		}

		newData[this->length] = Customer(name);				//add the new customer to the db
		delete[] this->data;								//clean up old data
		this->data = newData;								//brand new and twice as nice!
		this->length++;										//update the length
		return this->data[length-1];						//return reference to new struct
	}

}

/*--CustomerDB::isMember-------------------------------------

	Inputs: implicit DB, String value of name to be searched
	Output: Boolean of whether a customer with this name exists

------------------------------------------------------------*/

bool CustomerDB::isMember(String name) {				
	for (int i = 0; i < this->length; i++){
		if (this->data[i].name == name)
			return true;
	}
	return false;
}


