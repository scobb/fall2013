/*--Project 8------------------------------------------------------

	Steve Cobb
	TA: Duncan Horn
	Friday, 1-1:50

-----------------------------------------------------------------*/


using namespace std;

//**************INCLUDES********************************************
#include <stdio.h>
#include "Race.h"


//***************GLOBALS********************************************

FILE* input_stream;



//************IMPLEMENTATION****************************************


bool readNum(int& x) {
	return EOF != fscanf(input_stream, "%d", &x);
}

int main(void) {

	//create race 
	Race race;

	//set up input
	input_stream = fopen("input.txt", "r");

	//integers to store our input
	int num1,num2;

	while (readNum(num1) && readNum(num2)){

		race.lap(num1,num2);

	}

	race.print_results();
}
