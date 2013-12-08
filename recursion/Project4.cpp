/*
 * Project9.c
 *
 * Steve Cobb
 * Duncan Horn
 * F: 1-1:50
 *
 *
 */


#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <limits>
#include "MazeParams.h"
int whatLetter(char c);
#define MAX_INT 0xcFFF

/* -----------minIt------------------------------
	Iterative function that returns the minimum value in 
	an array

	inputs: integer array to be searched, length of array

	output: minimum value in array
------------------------------------------------- */


int minIt(int x[], int n) {
	int min = MAX_INT;					//initialize to max value
	int i;
	for (i=0;i<n;i++){
		if (x[i] < min){
			min = x[i];
		}
	}
	return min;

}

/* ----------minRec1---------------------------------
	An n-1 recursive implementation of min function

	inputs: integer array to be searched, array length

	output: minimum value in array
---------------------------------------------------- */


int minRec1(int x[], int n) 
{
	if (n > 1)
	{
		if (x[0] < x[n-1])
		{
			return minRec1 (x, n - 1);
		}
		else if (x[0] > x[n-1]){
			return minRec1 (x+1, n-1);
		}
	}
	else return x[0];
}


/* ----------minRec2---------------------------------
	An n/2 recursive implementation of min function

	inputs: integer array to be searched, array length

	output: minimum value in array
---------------------------------------------------- */


int minRec2(int x[], int n) {
	int high, low;
	if (n>1){
		high = minRec2(x + n/2, (n+1)/2);				//search the upper half 
		low = minRec2(x, (n+1)/2);						//and the lower half
	}
	else return x[0];
	if (high < low){
		return high;
	}
	else return low;

}


/*----------------sqrtIt--------------------------------------
	Returns the square root to within 15 decimal places of 
	precision

	inputs: number to take square root of, low guess, and high 
	guess. All are doubles

	output: square root of first parameter
------------------------------------------------------------ */


double sqrtIt(double x, double low_guess, double high_guess) {
	if (x<0 || low_guess > high_guess)											//just in case.
		return 0;
	double myGuess = (low_guess+high_guess)/2;
	double myGuessSquared = myGuess * myGuess;
	while (myGuessSquared != x){            
		if (myGuess == low_guess)
			return low_guess;
		if (myGuessSquared > x){
			high_guess = myGuess;
		}
		
		else if (myGuessSquared < x){
			low_guess = myGuess;
		}

		myGuess = (low_guess+high_guess)/2;
		myGuessSquared = myGuess*myGuess;
		
	}
	return myGuess;
}

/*----------------sqrtRec--------------------------------------
	Returns the square root to within 15 decimal places of 
	precision; recursive implementation

	inputs: number to take square root of, low guess, and high 
	guess. All are doubles

	output: square root of first parameter
------------------------------------------------------------ */

double sqrtRec(double x, double low_guess, double high_guess) {
	if (x < 0 || low_guess > high_guess)			//to protect against infinite loop
		return 0;
	double myGuess = (low_guess+high_guess)/2;
	double myGuessSquared = myGuess * myGuess;
	double answer;

	if (x == myGuessSquared){					   //if we're in this conditional, we found it exactly
		return myGuess;
	}
	if (low_guess == myGuess){                     //if we're in this conditional, that means we're within 15 decimal places

		return low_guess;
	}

	else if (myGuessSquared > x){                  //if we're here, we're too high
		answer = sqrtRec(x,low_guess,myGuess);
	}

	else if (myGuessSquared < x){                  //if we're here, we're too low
		answer = sqrtRec(x,myGuess,high_guess);
	}

	return answer;
}


/*----------strCompare-------------------------------------------------

Recursive program that compares two strings based on ASCII value

inputs: two character pointers to null-terminated strings to be compared

outputs: -1 if first parameter is less than second, 0 if the two strings
		are equal, 1 if first parameter is greater than the second.

------------------------------------------------------------------------*/

int strCompare(char* str1, char* str2) {

	if (str1[0] == 0 && str2[0] == 0)				//base case: null strings
		return 0;

	if (str1[0] == str2[0])							//smaller problem: if this letter is equal, but not a null, go to the next
		return strCompare(str1+1,str2+1);

	if (str1[0] < str2[0])							//case: str1's char is < str2's char
		return -1;

	else return 1;									//case: str1's char is > str2's char

}

/* if c is null, return 0
 * if c is not a letter return -1
 * if c is a letter, return the position of c in the alphabet 
 * where 'A' (and 'a') are position 1 and 'Z' (and 'z') are position 26
 *
 * This code is rather ugly as I'm exploiting some detailed knowledge of the ASCII table
 */
int whatLetter(char c) {
	if (c == 0) return 0;				//this makes one comparison easier below
	if (c < 'A') { return -1; }
	if (c > 'z') { return -1; }
	if (c > 'Z' && c < 'a') { return -1; }
	return c & ~32 - 64;
}



/*----------strCompare2-------------------------------------------------

Recursive program that compares two strings based on ASCII value. Ignores
non-letters and case.

inputs: two character pointers to null-terminated strings to be compared

outputs: -1 if first parameter is less than second, 0 if the two strings
		are equal, 1 if first parameter is greater than the second.

------------------------------------------------------------------------*/


int strCompare2(char* str1, char* str2) {

	if (str1[0] == 0 && str2[0] == 0)				//base case: null strings
		return 0;

	if (whatLetter(str1[0]) < 0)					//str1[0] is not a letter or 0, ignore and go on
		return strCompare2(str1+1,str2);

	if (whatLetter(str2[0]) < 0)					//str2[0] is not a letter or 0, ignore and go on
		return strCompare2(str1,str2+1);

	if (whatLetter(str1[0]) == whatLetter(str2[0]))	//str1[0] = str2[0], go to next letter on each
		return strCompare2(str1+1,str2+1);

	if (whatLetter(str1[0]) < whatLetter(str2[0]))	//str1[0] < str2[0], return -1
		return -1;

	else return 1;									//str1[0] > str2[0], return 1
	
}

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

