/*

Steve Cobb
Hash.h
EE 312
UT - Austin

Note: this hash table uses the car's number as its hash value, because
all car numbers will be unique. 

*/

#include <stdio.h>
#include <stdint.h>

int count = 0;
struct car_record{

	int32_t carID,mean_time,min_lap,start_time,num_laps;

	//basic record constructor, reads in invalid values
	car_record(void){
		carID = -1;
		mean_time = -1;
		min_lap = -1;
		start_time = -1;
		num_laps = -1;
	}
	//custom record constructor, takes int values for each field
	car_record(int32_t carID,int32_t mean_time,int32_t min_lap,int32_t start_time){
		this->carID = carID;
		this->mean_time = mean_time;
		this->min_lap = min_lap;
		this->start_time = start_time;
		this->num_laps = -1;			//starts as -1 because first read takes us up to 0 laps
	}

	//copy constructor, doesn't seem to be necessary though
	car_record(car_record& that){
		this->carID = that.carID;
		this->mean_time = that.mean_time;
		this->min_lap = that.min_lap;
		this->start_time = that.start_time;
		this->num_laps = that.num_laps;
	}

};


#define DEFAULT_SIZE 8


struct HashTable {
private:
	uint32_t length;			//number of elements contained
	uint32_t capacity;			//number of buckets

	struct Node {				// used to make linked lists
		car_record car_record;	//payload
		Node* next;				//pointer to next node
	};
	typedef Node* NodePtr;
	NodePtr* table;				// the pointer to the array on the heap of linked lists

public:

	/*---Constructor-------------------------------------

	Default constructor that constructs an empty table of 
	default size.

	----------------------------------------------------*/

	HashTable(void) {
		table = new NodePtr[DEFAULT_SIZE];
		for (uint32_t k = 0; k < DEFAULT_SIZE; k += 1) {
			table[k] = 0;
		}
		length = 0;
		capacity = DEFAULT_SIZE;
	}

	/*---Custom Constructor--------------------------------

	Constructs a table to maintain a .75 load factor given
	the input number of entries

	-----------------------------------------------------*/
	HashTable(uint32_t num_cars){
		uint32_t num_indices = num_cars * 4 + 2;			//guarantees we get < .75 load factor
		num_indices /= 3;
		table = new NodePtr[num_indices];
		for (uint32_t k = 0; k < num_indices; k +=1){
			table[k] = 0;
		}
		length = 0;
		capacity = num_indices;
	}
	/*---Destructor--------------------------------------

	Cleans up heap allocations from our nodes & hash table

	---------------------------------------------------*/
	~HashTable(void) {
		Node* p;
		Node* s;
		for (int i = 0; i < capacity; i++){			//go until we've looked at all the old buckets
			p = table[i];
			while (p){
				s = p;
				p = p->next;
				delete s;
			}
		}
		delete table;
	}

	/* [] operator ----------------------------------

	returns reference to record with input ID #
	
	input: car's ID number
	output: reference to that car's record

	------------------------------------------------*/
	car_record& operator[](uint32_t carID){
		uint32_t pos = carID % capacity;
		Node* p = table[pos];
		while (p){
			if (p->car_record.carID == carID)
				return p->car_record;
			else 
				p = p->next;
		}
		printf("tried to access car record that didn't exist\n");
	}

	/*---insert-------------------------------------------------

	inserts given record according to carID

	input: record to be inserted
	output: none

	-----------------------------------------------------------*/
	void insert(car_record value) {
		uint32_t hash_code = value.carID;
		uint32_t pos = hash_code % capacity;
		Node* p = new Node;
		p->car_record = value;
		p->next = table[pos];
		table[pos] = p;
		length++;
		if ((float)length/capacity > 1)				//for our purposes this should never happen
			resize_table();
	}

	void resize_table(void){
		uint32_t new_capacity = capacity * 2;
		uint32_t new_length = length;
		Node* get;
		Node* put;
		NodePtr* new_table = new NodePtr[new_capacity];
		int i = 0;						//tracks what bucket we're in
		for (int i = 0; i < capacity; i++){			//go until we've looked at all the old buckets
			get = table[i];
			while (get){
				put = get;
				uint32_t pos = get->car_record.carID % new_capacity;
				get = get->next;
				put->next = new_table[pos];
				new_table[pos] = put;
			}
		}
		length = new_length;
		capacity = new_capacity;
		table = new_table;
	}


	bool isMember(car_record key) {
		uint32_t hash_code = key.carID;
		uint32_t pos = hash_code % capacity;
		Node* p = table[pos];

		while (p != 0) {
			if (p->car_record.carID == key.carID) { return true; }
			p = p->next;
		}

		return false;

	}
};