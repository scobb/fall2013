/*Steve Cobb
  Project 7
  TA: Duncan Horn
  Friday, 1-1:50
*/

//**************INCLUDES************************

#include "Bugs.h"
#include <assert.h>

//***************GLOBALS************************

int time_step = 0;
int average_age = 0;
int average_generation = 0;
int percent_straight;
int percent_right;
int percent_left;
int percent_back;
int total_left = 0;
int total_right = 0;
int total_straight = 0;
int total_back = 0;


//**********************IMPLEMENTATION**************************************


/* DONE: you don't need to use this function in your solution
 * however this function is used by createWorld to generate random bugs */
Bug createBug(int x, int y) {
	Bug b;
	b.age = 0;
	b.generation = 0;
	b.x = x;
	b.y = y;
	b.dir = rand() % 8;
	b.health = START_HEALTH;
	for (int k = 0; k < 8; k += 1) {
		if (DISTRIBUTED_GENES) {
			b.genes[k] = GENE_TOTAL / 8;
		} else {
			if (k == 0) {
				b.genes[k] = GENE_TOTAL;
			} else {
				b.genes[k] = 0;
			}
		}
	}
	total_straight += b.genes[0];
	total_right += b.genes[1] + b.genes[2] + b.genes[3];
	total_left += b.genes[5] + b.genes[6] + b.genes[7];
	total_back += b.genes[4];
	return b;
}

/* DONE: */
void createWorld(void) {
	/* empty the world */
	for (int i = 0; i < WORLD_SIZE; i += 1) {
		for (int j = 0; j < WORLD_SIZE; j += 1) {
			world[i][j] = EMPTY;
		}
	}

	/* create food */
	for (int k = 0; k < INITIAL_FOOD; k += 1) {
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		world[x][y] = FOOD;
	}
	
	/* create bugs */
	while (bug_list.size() < INITIAL_BUGS) {
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		if (world[x][y] == EMPTY) {
			world[x][y] = bug_list.size();
			bug_list.push_back(createBug(x, y));
		}
	}	
}

/*
 * DONE: keep in mind that bugs that "fall off" the edge of the world
 * automatically reappear on the opposite edge (i.e, the world is round,
 * well, if you really want to get topographical, our bugs live on a donut.
 */ 
int wrap(int y) {
	if (y < 0) { return WORLD_SIZE - 1; }
	else if (y == WORLD_SIZE) { return 0; }
	else { return y; }
}

/* DONE: you can use this function if you want, it's not required that you use it */
int newX(int x, int dir) {
	switch (dir) {
		case 0: // move up
		case 1:
		case 7:
			return wrap(x - 1);
			break; // not reached
		case 3: // move down
		case 4:
		case 5:
			return wrap(x + 1);
			break; // not reached
		case 2: // move left or right
		case 6:
			return x;
	}
	/* NOT REACHED */
	return 0;
}


/* DONE: you can use this function if you want, it's not required that you use it */
int newY(int y, int dir) {
	switch (dir) {
	case 1:
	case 2:
	case 3:
		return wrap(y + 1);
	case 5:
	case 6:
	case 7:
		return wrap(y - 1);
	case 0:
	case 4:
		return y;
	}
	/* NOT REACHED */
	return 0;
}


/* DONE: 
 * Add NUM_FOOD food objects in random locations
 * if you drop food on a bug, be sure to feed the bug
 */
void addFood(void) {
	for (int food = 0; food < NUM_FOOD; food += 1) {
		int x = rand() % WORLD_SIZE;
		int y = rand() % WORLD_SIZE;
		switch (world[x][y]) {
		case EMPTY: // empty, add the food
			world[x][y] = FOOD;
			break;
		case FOOD: // already has food, do nothing
			break;
		default: // bug, feed the bug
			bug_list[world[x][y]].health += EAT_HEALTH;
		}
	}	
	
}

/*---moveBugs---------------------------------------------------------------

	Updates state of global variables bug_list and world to move the bugs 
	according to their current direction and position and select their new 
	direction using a random probably based on their genes. Bug health will be 
	updated due to movement and also if a bug lands on a space in the world 
	array that contains food.

	Inputs: none
	Outputs: none

	Time Complexity: O(n*GENE_TOTAL) -> O(n), with the understanding that a large
	gene total could indicate a large prefactor

 --------------------------------------------------------------------------*/
void moveBugs(void) {
	int total_age = 0;
	int total_gen = 0;
	
	for (int k = 0; k < bug_list.size(); k += 1) {

		//update world where bug is moving from to EMPTY

		if (world[bug_list[k].x][bug_list[k].y] == k)
			world[bug_list[k].x][bug_list[k].y] = EMPTY;

		//new position in each bug's x and y
		bug_list[k].x = wrap(newX(bug_list[k].x,bug_list[k].dir));
		bug_list[k].y = wrap(newY(bug_list[k].y,bug_list[k].dir));

		// see if new position contains food, update health

		if (world[bug_list[k].x][bug_list[k].y] == FOOD){
			bug_list[k].health+=EAT_HEALTH;
		}

		//after checking for food, bug now occupies this spot in the world

		world[bug_list[k].x][bug_list[k].y] = k;

		//bug loses some health from moving
		
		bug_list[k].health -= MOVE_HEALTH;

		//bug's new direction determined according to his genes


		vector <int> prob_array(GENE_TOTAL);
		int i2 = 0;												//will iterate from 0 -> GENE_TOTAL

		/*this nested loop populates a probability array with GENE_TOTAL elements. Each element
		is assigned a direction to turn based on the bug's genes and represents 1/GENE_TOTAL 
		chance of the bug turning that direction*/

		for (int j = 0; j < 8; j++){							//goes through the 8 genes
			for (int i = 0; i < bug_list[k].genes[j]; i++){		
				prob_array[i2] = j;								//bug_list[k].genes[j] elements will be given value j
				i2++;
			}
		}

		int tmp = rand() % GENE_TOTAL;
		bug_list[k].dir = (bug_list[k].dir + prob_array[tmp])%8; //new direction as a function of old direction

		//update stats

		bug_list[k].age += 1;
		total_age += bug_list[k].age;
		total_gen += bug_list[k].generation;

	}


	if (bug_list.size()){
		average_age = total_age / bug_list.size();
		average_generation = total_gen / bug_list.size();
	}
}

/*---killDeadBugs----------------------------------------------------------

	Updates global variables bug_list and world to remove bugs whose health 
	are below 0

	Inputs: none
	Outputs: none

	Time Complexity: O(n)
-------------------------------------------------------------------------*/

void killDeadBugs(void) {
	int i = 0;
	while (i < bug_list.size()){
		if (bug_list[i].health <= 0){

			//bug is dead, update world

			world[bug_list[i].x][bug_list[i].y] = EMPTY;

			//update stats
			total_straight -= bug_list[i].genes[0];
			total_right -= (bug_list[i].genes[1] + bug_list[i].genes[2] + bug_list[i].genes[3]);
			total_back -= bug_list[i].genes[4];
			total_left -= (bug_list[i].genes[5] + bug_list[i].genes[6] + bug_list[i].genes[7]);

			//swap bug to end if necessary and update world
			if (i != bug_list.size() - 1){
				swap(bug_list[i],bug_list[bug_list.size()-1]);
				world[bug_list[i].x][bug_list[i].y] = i;
			}

			//delete last bug in list

			bug_list.pop_back();

		}
		else i++; //only iterate if bug is alive
	}
}

/*---reproduceBugs-----------------------------------------

	If a bug has reached REPRODUCE_HEALTH amount of health,
	it will reproduce. The new bug will "steal" half the old
	bug's health, undergo a gene mutation, and be a generation
	older than the old bug, but otherwise will be a copy.

	Inputs: none
	Outputs: none

	Time Complexity: O(n) worst case, as worst case would involve 
	calling push_back n times, which has an amortized constant time
	complexity.
 --------------------------------------------------------------*/
void reproduceBugs(void) {
	for (int k = 0; k < bug_list.size(); k += 1) {
		if (bug_list[k].health >= REPRODUCE_HEALTH) {

			//create new Bug using half of bug k's health

			Bug b; 
			b = bug_list[k];
			b.generation++;
			b.age = 0;
			bug_list[k].health /= 2;
			b.health = bug_list[k].health;

			//mutation 

			//increment a gene
			int tmp = rand() % 8;
			b.genes[tmp]++;
			
			//find non-zero gene to decrement
			do {
				tmp = rand() % 8;
			} while (b.genes[tmp] == 0);

			b.genes[tmp]--;

			//statistics update

			total_straight += b.genes[0];
			total_right += b.genes[1] + b.genes[2] + b.genes[3];
			total_left += b.genes[5] + b.genes[6] + b.genes[7];
			total_back += b.genes[4];

			
			//add b to bug_list
			bug_list.push_back(b);

		}
	}	
}

/*---timeStep---------------------------------------------------------

	Updates bug_list and world through one time unit, calling each of the
	non-initial functions above.

	Inputs: none
	Outputs: none


-------------------------------------------------------------------------*/
void timeStep() {
	time_step += 1;
	addFood();
	moveBugs();
	killDeadBugs();
	reproduceBugs();
	if (bug_list.size()){
		percent_straight = 100 * total_straight / bug_list.size() / GENE_TOTAL;
		percent_left = 100 * total_left / bug_list.size() / GENE_TOTAL;
		percent_right = 100 * total_right / bug_list.size() / GENE_TOTAL;
		percent_back = 100 * total_back / bug_list.size() / GENE_TOTAL;
	}

}	
