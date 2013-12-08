/*
 * Project5.cpp 
 *	Steve Cobb
	EE 312
    UT - Austin
 
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Set.h"

/*
 * Several of the required functions have been written already
 * You may use these functions as-is or you may modify them in any way. 
 * Please NOTE, you are responsible for ensuring that the functions work correctly in 
 * your project solution. Just because the function works correctly in my solution does 
 * NOT mean that the function will work correctly in yours. 
 */

/*
 * Design NOTES:
 *
 * The design provided in this starter kit assumes
 * (1) empty sets will be represented with length == 0 and elements == nullptr (i.e., address 0)
 * (2) amortized doubling is not used, and capacity is ignored/unused. Functions should assume that 
 * the amount of storage available in the elements[] array is equal to length
 */

int binarySearch(int* x, int n, int v);


/*--binarySearch---------------------------------------------------------

	inputs: pointer to array to be searched, number of elements in array,
			value to be searched for

	output: index of array where v should be, -1 if empty array

-----------------------------------------------------------------------*/
int binarySearch(int* x, int n, int v){
	if (n == 0)
		return -1;
	int low = 0;
	int hi = n;
	while (hi-low > 1){
		int mid = (low+hi)/2;
		if (v < x[mid])
			hi = mid;
		else low = mid;
	}
	return low;
}
//-----------------------------------------------------------------------

/* done for you already */
void destroySet(Set* self) {
	free(self->elements);
}

/* done for you already */
void createEmptySet(Set* self) {
	self->len = 0;
	self->elements = 0;
}

/* done for you already */
void createSingletonSet(Set* self, int x) {
	self->elements = (int*) malloc(sizeof(int));
	self->elements[0] = x;
	self->len = 1;
}

/* done for you already */
void createCopySet(Set* self, const Set* other) {
	self->elements = (int*) malloc(other->len * sizeof(int));
	for (int k = 0; k < other->len; k += 1) {
		self->elements[k] = other->elements[k];
	}
	self->len = other->len;	
}

/* done for you already */
void assignSet(Set* self, const Set* other) {
	if (self == other) { return; }
	
	destroySet(self);
	createCopySet(self, other);
}

/*--isMemberSet-----------------------------------------------------
	
	inputs: pointer to set to be searched, integer to search for
	outputs: 1 if value is in set, 0 if not

	time complexity: O(log(n))
------------------------------------------------------------------*/
bool isMemberSet(const Set* self, int x) {
	int mid =  binarySearch(self->elements, self->len, x);
	if (mid >= 0)
	{
		if (self->elements[mid] == x)
		{
			return true;
		}
	}
	return false;

}

/*--insertSet--------------------------------------------------------------
	inserts a new member into the set, assuming it does not already exist

	inputs: pointer to set, integer value to be inserted
	post-condition: set now contains the member

	time complexity: O(n)
 -------------------------------------------------------------------------*/
void insertSet(Set* self, int x) {
	if (isMemberSet(self,x))               //get out if x is already a member
		return;
	int placed = 0;							//keeps track of whether we've placed the new element
	self->len += 1;
	int* newData = (int*)malloc(sizeof(int)*(self->len));
	int j = 0;
	for (int i = 0; i< self->len; i++){
		if (self->len == 1)					//to handle previously empty arrays
			newData[i] = x;
		else if (!placed && (x < self->elements[j] || j == self->len - 1)){
			newData[i] = x;
			placed = 1;						//we've placed our new element so we won't catch this block anymore
		}
		else {
			newData[i] = self->elements[j];
			j++;
		}
	}
	free(self->elements);					//free the old dataset
	self->elements = newData;				//elements points to new data set
}


/*--removeSet---------------------------------------------------------

	Removes member from set if it exists there already

	Inputs: pointer to set, integer value to be removed
	Post-Condition: value has been removed

	time complexity: O(n)
 --------------------------------------------------------------------*/
void removeSet(Set* self, int x) {

	int mid = binarySearch(self->elements, self->len, x);			//returns where x should be if it's a member

	if (mid < 0)													//array was empty
		return;

	if (self->elements[mid] != x)									//return if x isn't already a member
		return;


	//***update self**********************************

	self->len-=1;
	for (int i = mid; i < self->len; i++){
		self->elements[i] = self->elements[i+1];
	}
}

/* done for you already */
void displaySet(const Set* self) {
	int k;
	
	printf("{");

	if (self->len == 0) { 
		printf("}"); 
	}
	else {
		for (k = 0; k < self->len; k += 1) {
			if (k < self->len - 1) {
				printf("%d,", self->elements[k]);
			} else {
				printf("%d}", self->elements[k]);
			}
		}
	}
}

/*--isEqualToSet--------------------------------------------------

	returns true if self and other have exactly the same elements 

	inputs: two sets to be compared
	outputs: true if the sets match exactly, false otherwise
	time complexity: O(n)

-----------------------------------------------------------------*/

bool isEqualToSet(const Set* self, const Set* other) {

	if (self->len != other->len)
		return false;

	for (int i = 0; i < self->len; i++)
	{
		if (self->elements[i] != other->elements[i])
			return false;
	}
	return true;

}

/*--isSubsetOf-------------------------------------------------------

	returns true if every element of self is also an element of other 
	inputs: pointer to two sets
	outputs: boolean true if every element of first set is also an element
			of the second set.
	time complexity: O(n)

---------------------------------------------------------------------*/
bool isSubsetOf(const Set* self, const Set* other) {
	int i,j;			//i counts for self, j counts for other
	i = 0;

	for (j = 0; j < other->len && i < self->len; j++)
	{
		if (self->elements[i] == other->elements[j])
		{
			i++;
		}
	}

	if (i == self->len)
		return true;

	else return false;

}

/* done for you */
bool isEmptySet(const Set* self) {
	return self->len == 0;
}

/*--intersectFromSet-------------------------------------------------

	remove all elements from self that are not also elements of other

	inputs: pointer to two sets to be intersected
	post-condition: self contains only the elements that occur in both 
					sets.
	time complexity: O(n)


----------------------------------------------------------------------*/

void intersectFromSet(Set* self, const Set* other) {

	
//****locals**************

	int i = 0;	//self
	int j = 0;	//other
	int k = 0;	//new
	int final = self->len;
	int* newData = (int*)malloc(self->len * sizeof(int));

//****implementation******

	while (i < final && j < other->len)
	{
		if (self->elements[i]  == other->elements[j])				//found a match, copy it
		{				
			newData[k] = self->elements[i];
			i++;
			j++;
			k++;
		}
		else if (self->elements[i] < other->elements[j])			//self's element isn't in other
		{			
			i++;													//go to next element of self
		}
		else j++;													//other's element is less than self's
	}

//****update self**********

	free(self->elements);
	self->elements = newData;
	self->len = k;													//k tracks the number of copied elements

}

/*--subtractFromSet----------------------------------------------

	remove all elements from self that are also elements of other 

	inputs: pointers to two sets to be compared

	post-condition: self will no longer contain any elements that
					are contained in other

	time complexity: O(n)

---------------------------------------------------------------*/

void subtractFromSet(Set* self, const Set* other) 
{

//****base case*****************************************

	if (other->len == 0)
		return;

//****locals********************************************

	int i = 0;	//self
	int j = 0;	//other
	int k = 0;	//new
	int final = self->len;
	int final2 = other->len;
	int* newData = (int*)malloc(self->len * sizeof(int));

//****implementation************************************

	while (i < final && j < final2)
	{
		if (self->elements[i] < other->elements[j])			//this element is not contained in other
		{
			newData[k] = self->elements[i];
			i++;
			k++;
		}
		else if (self->elements[i] == other->elements[j])	//this element is contained in other; remove it
		{
			i++;
			j++;
			self->len--;
		}
		else j++;											//this element might be in other, need to check against other's next element
	}

	while (i< final)										//once we're done checking against length, we can fill in the rest from self
	{
		newData[k] = self->elements[i];
		i++;
		k++;
	}

//****update self****************************************

	free(self->elements);
	self->elements = newData;

}

/*---unionInSet-----------------------------------------------------------

	add all elements of other to self without creating duplicate elements 

	inputs: pointers to two sets to take the union of

	post-condition: self points to result of union

	time complexity: O(n)

------------------------------------------------------------------------*/

void unionInSet(Set* self, const Set* other) {

//****local variables****************************************

	int i = 0;	//self index
	int j = 0;	//other index
	int k = 0;	//new index
	int oldLength = self->len;
	int* newData = (int*)malloc((self->len + other->len) * sizeof(int));

//****implementation*****************************************

	while (i < oldLength || j < other->len )
	{
		if (j == other->len || (i< oldLength && self->elements[i] < other->elements[j]))	//either other is empty or self has the smaller element
		{
			newData[k] = self->elements[i];
			k++;
			i++;
		}
		else if (i == oldLength || (j<other->len && other->elements[j] < self->elements[i])) //either self is empty or other has the smaller element
		{
			newData[k] = other->elements[j];
			k++;
			j++;
			self->len++;
		}
		else if (self->elements[i] == other->elements[j]){									//this element is in both sets
			newData[k] = self->elements[i];
			i++;
			j++;
			k++;
		}

	}

//****update self**********************************************

	free(self->elements);
	self->elements = newData;

}