/*
 * Project9.c
 *
 * Steve Cobb
 * EE 312
 * UT - Austin
 * Some recursive solutions to exhaustive search problems
 */


#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <limits>
#include "MazeParams.h"
int whatLetter(char c);
#define MAX_INT 0xcFFF


/*---solveMazeRec------------------------------------------------------------
	Solves a maze contained in a global variable integer two-dimensional matrix
	named maze that is of size MATRIX_SIZE x MATRIX_SIZE
	
	inputs: row and column for current position in maze
	outputs: 1 for maze solved, 0 for dead end

----------------------------------------------------------------------------*/

int solveMazeRec(int row, int col) {
	int bestpath = 0;
	maze[row][col] = 2;

	if (row == MATRIX_SIZE - 1)											//found the exit!
		return 1;

	if (!bestpath && row > 0 && maze[row-1][col] == 0)					//if best path hasn't been found, look above
		bestpath = solveMazeRec(row-1, col);

	if (!bestpath && row < MATRIX_SIZE - 1 && maze[row+1][col] == 0)	//if best path hasn't been found, look below
		bestpath = solveMazeRec(row+1,col);

	if (!bestpath && col > 0 && maze[row][col-1] == 0)					//if best path hasn't been found, look left
		bestpath = solveMazeRec(row, col-1);

	if (!bestpath&& col < MATRIX_SIZE - 1 && maze[row][col+1] == 0)		//if best path hasn't been found, look right
		bestpath = solveMazeRec(row, col+1);

	if (!bestpath)														//if this is not the best path, pick up the bread crumb
		maze[row][col] = 0;

	return bestpath;													//unless the path found the exit, this should be a 0
}




//--------Martian struct------------------------

struct Martian {
	int pennies;	//1 cent
	int nicks;		//5 cents
	int dodeks;		//12 cents
};

//----------private function prototypes-------------

int numCoins(Martian guy);
int totalValue(Martian guy);

/*----------------------change-------------------------------
	This is a recursive subroutine that determines the fewest
	coins that can be used to sum to a certain total. Dodeks 
	are worth 12 cents, Nicks 5, and Pennies 1.

	Inputs: integer value total to be obtained

	Outputs: Martian structure that contains the most efficient
			coin combination of the requested value total
 ------------------------------------------------------------*/

Martian change(int cents) {

//------------inits--------------
	
	Martian result1,result2,result3;
	result1.pennies = 0;
	result1.nicks = 0;
	result1.dodeks = 0;
	result2.pennies = 0;
	result2.nicks = 0;
	result2.dodeks = 0;
	result3.pennies = 0;
	result3.nicks = 0;
	result3.dodeks = 0;

//-------base case -------------------------

	if (cents < 0)										//just in case the testers are jerks.
		return result1;


	if (cents < 5){										//for less than five cents, we must use pennies
		result1.pennies += cents;
		return result1;
	}													

//-------result 2 - dodeks------------------

	if (cents > 11){									//result2 follows what happens if we add as many dodeks as possible
	result2 = change(cents-12);
	result2.dodeks++;
	}

//-------result 3 - nicks--------------------

	if (cents > 4){										//result3 follows what happens if we add as many nicks as possible
	result3 = change(cents-5);
	result3.nicks ++;
	}

//--------evaluation--------------------------

	if (numCoins(result2) < numCoins(result3)			//result 2 is most efficient and has the right amount
		&& totalValue(result2) == cents)
	{
		return result2;
	}

	else if (totalValue(result3) != cents)				//result 2 is the only solution that has the right amount
		return result2;

	else return result3;								//result3 is the most efficient and has the right amount, or is the only one
														//with the right amount
}


/*------------numCoins---------------------
	This function returns the total number of 
	coins a Martian has.

	Inputs: Martian

	Outputs: integer number of coins
------------------------------------------*/


int numCoins(Martian guy)
{								
	int result = 0;
	result += guy.dodeks;
	result += guy.nicks;
	result += guy.pennies;
	return result;
}

/*------------totalValue---------------------
	This function returns the total value of 
	the coins a Martian has

	Inputs: Martian

	Outputs: integer value of coins
------------------------------------------*/

int totalValue(Martian guy)
{							
	return 12*guy.dodeks + 5*guy.nicks + guy.pennies;
}

