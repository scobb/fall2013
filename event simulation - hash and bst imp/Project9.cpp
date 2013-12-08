/* Project 9
Steve Cobb
EE 312
UT - Austin
*/

#include <stdio.h>
#include <stdint.h>
#include "BSTQueue.h"
#include "Hash.h"


/* A Mersenne Twister (code adapted from Wikipedia: http://en.wikipedia.org/wiki/Mersenne_twister */
struct RandomMT {
private:
	static const uint32_t size = 624;
	uint32_t data[size]; 
	uint32_t index; 
public:
	RandomMT(uint32_t seed = 42) { // pick your own seed, or use 42 by default
		index = 0;
		data[0] = seed;
		for (uint32_t k = 1; k < size; k += 1) {
			data[k] = data[k-1] >> 30;
			data[k] = (data[k] + k) ^ data[k-1];
			data[k] = data[k] * 1812433253; 
		}
	}

	uint32_t next(void) {
		if (index == 0) { regenerate(); }
		uint32_t result = data[index];
		result ^= result >> 11;
		result ^= (result << 7) & 0x9d2c5680;
		result ^= (result << 15) & 0xefc60000;
		result ^= result >> 18;

		index = (index + 1) % size;
		return result;
	}

private:
	void regenerate(void) {
		for (uint32_t k = 0; k < size; k += 1) {
			uint32_t val = data[k] & 0x80000000  
				 | data[(k + 1) % size] & 0x7fffffff;
			data[k] = data[(k + 397) % size] ^ (val >> 1);
			if (val & 1) {
				data[k] = data[k] ^ 0x9908b0df;
			}
		}
	}
};


FILE* output_stream = stdout;
FILE* input_stream;

bool readNum(int& x) {
	return EOF != fscanf(input_stream, "%d", &x);
}

bool readNum(uint32_t& x) {
	return EOF != fscanf(input_stream, "%d", &x);
}


int main(void) {
	input_stream = fopen("race.txt", "r");

	output_stream = fopen("laps.txt", "w");

	RandomMT random(13);						//RNG with my favorite seed
	BST queue;									//Binary Search Tree that will be priority queue
	bool inProgress = true;						//boolean that tracks if the race is in progress
	int32_t num_cars,num_laps;


	readNum(num_cars);
	readNum(num_laps);
	HashTable records(num_cars);				//hash table with capacity of num_cars * 4/3 for .75 load factor

	//setting up our records for each car

	for (int i = 0; i < num_cars; i++){
		int32_t carID,mean_time,min_lap,start_time;
		readNum(carID);
		readNum(mean_time);
		readNum(min_lap);
		readNum(start_time);

		//start time for each racer into priority queue
		Event lap(carID,start_time);
		start_time = queue.insert(lap);

		//store record of each racer in hash table
		car_record record(carID,mean_time,min_lap,start_time);
		records.insert(record);

	}


	//simulation

	while (!queue.isEmpty()){
		uint32_t current = queue.smallest_node->value.num;				//carID of the car being updated
		uint32_t current_time = queue.smallest_node->value.timestamp;	//timestamp of car being updated

		records[current].num_laps++;									//update record to indicate correct number of laps
		
		queue.remove_smallest();										//delete smallest event from queue

		fprintf(output_stream, "%d %d\n", current, current_time);		//write to file: carID and timestamp
		
		if (records[current].num_laps == num_laps)						//race is over when current car has finished
			inProgress = false;

		if (inProgress){												//if race isn't over, generate this car's next event!

			//random will have a radius of mean-min and surround mean

			uint32_t range = 2 * (records[current].mean_time - records[current].min_lap);	
			uint32_t laptime;

			if (range)
				laptime = random.next() % range + records[current].min_lap + records[current].start_time;
			else laptime = records[current].mean_time + records[current].start_time;			//handle divide-by-0 error
			
			//construct new event, enter in queue

			Event lap(current, laptime);
			laptime = queue.insert(lap);

			//update record with new start time
			records[current].start_time = laptime;
		}


	}







}
