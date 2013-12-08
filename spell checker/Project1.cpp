/*
 * Project1.c
 * Please insert the standard EE312 header here!
 * 
 * EE312 Project 1 (Spell Check)
 * Steven Cobb
 * Duncan Horn
 * F: 1-1:50
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

int32_t wordLength(char text[]);
int8_t stringCompare(char word1[], char word2[], int32_t n);
int32_t nextWord(char text[]);
void allLowerCase(char text[]);
int32_t dictionaryCount(char text[]);
int8_t binarySearch(char word[], char dictionary[], int32_t left, int32_t right, int32_t positionArray[], int8_t length);
void populatePosition(char dictionary[], int32_t positionArray[]);

//spellCheck:
//inputs: pointer to a null-terminated string to be spell-checked, pointer to a null-terminated, \n-separated, alphabetized dictionary string
//outputs: groups of 2+ consecutive letters that do not match any of the dictionary's entries will be printed, one to a line

void spellCheck(char article[], char dictionary[]) {
	int32_t fp_article = 0;									//pointer to front of current word in article
	allLowerCase(article);									//convert to all lower case text
	allLowerCase(dictionary);								//convert to all lower case text
	int* positionArray;										//pointer that will hold point to position array
	int32_t total = dictionaryCount(dictionary);			//total number of words in dictionary
	if (total > 0){
		positionArray = (int*)malloc(total*sizeof(int));	//position array made to fit total # of dict words
		populatePosition(dictionary,positionArray);			//position array populated with position of first letter of each word			
	}
	else{													//given an array with 0 entries, only position is 0, which should point to either \n or 0
		positionArray = (int*)malloc(1*sizeof(int));
		positionArray[0] = 0;
	}
	while (article[fp_article] != 0){						//go until each word of the article is checked
		int8_t found = binarySearch(article+fp_article,dictionary,0,total,positionArray, wordLength(article+fp_article));
		if (!found){                                        //print the misspelled word if it isn't found in the dictionary
			for (int i = fp_article; i < fp_article + wordLength(article + fp_article); i++){
				putchar(article[i]);
			}
			putchar('\n');
		}
		fp_article += nextWord(article + fp_article);		//to the next word in the article!
	
	}
    free(positionArray);
	
}

//binarySearch:
//inputs: word to be searched for, dictionary to be searched in, left and rightmost position indices, position array, and searched word length
//output: 1 if found, 0 if not

int8_t binarySearch(char word[], char dictionary[], int32_t left, int32_t right, int32_t positionArray[], int8_t length){
	while (left <= right){
		int32_t middle = (left+right)/2;		//middle index
		int8_t result = stringCompare(word,dictionary+positionArray[middle],length);	//comparing to word at middle index
		switch(result){
		case 0:
			return 1;							//return if word is found
		case 1:
			right = middle-1;					//search earlier half
			break;
		case 2:
			left = middle+1;					//search later half
			break;
		}
	}
	return 0;
}

//wordLength:
//input: a string, with words separated by non-letters
//op: length of 'word'

int32_t wordLength(char text[]){
	int32_t finder = 0;
	while (text[finder] >= 'a' && text[finder] <= 'z'){ //increment until non-letter is found
		finder++;
	}
	return finder;
}

//nextWord:
//input: a string, with words separated by non-letters
//op: index of beginning of next word

int32_t nextWord(char text[]){
	int32_t result = wordLength(text);	//start just after end of current word
	while ((text[result] < 'a' || text[result] > 'z') && text[result] != 0){  //increment until a letter is found
		result++;
	}
	return result;
}

//stringCompare:
//inputs: word 1 to reference against n letters of word 2 from \n separated dictionary
//outputs: 0 for strings the same or <2 letters, 1 for w1 earlier alpha than w2, 2 for w2 later alpha than w2

int8_t stringCompare(char word1[], char word2[], int32_t n){
	int8_t result = 0;
	if (n<2)				//special case: length less than 2
	{
		return 0;
	}
	for (int32_t i = 0; i<n && result == 0; i++){  
		if (word1[i] > word2[i]){
			return 2;			//code for BEYOND
		}
		else if (word1[i] < word2[i]){
			return 1;			//code for BEFORE
		}
	}
	if (word2[n] != '\n')	//special case: word1 a partial match for word2, but word2 extends
	{
		result = 1;			//code for BEFORE (partial match)
	}
	return result;
}

//allLowerCase:
//inputs: pointer to null-terminated string
//post-condition: all letters in string are lower case

void allLowerCase(char text[]){
	int32_t i = 0;
	while (text[i] != 0){
		if (text[i] >= 'A' && text[i] <= 'Z'){
			text[i] += 32;
		}
		i++;
	}
}

//dictionaryCount
//inputs: text array of dictionary (null terminated, \n separated)
//outputs: number of total words

int32_t dictionaryCount(char text[]){
	int32_t count = 0;							//tracks number of words
	int32_t i = 0;
	while (text[i] != 0){
		if (text[i] == '\n'){
			if (text[i-1] >= 'a' && text[i-1] <= 'z'){
				count++;
			}
		}
		i++;
	}
	return count;
}

//populatePosition
//inputs: pointer to dictionary (null terminated, \n separated, alphabetized), pointer to positionArray
//post-condition: entries of positionArray are the indices of the first letters of each word of the dictionary, in alphabetical order

void populatePosition(char dictionary[], int32_t positionArray[]){
	int32_t i = 0;
	int32_t j = 1;
	positionArray[0] = 0;
	while (dictionary[i+1] != 0){
		if (dictionary[i] == '\n'){									//find new line char
			if (dictionary[i+1] >= 'a' && dictionary[i+1] <= 'z'){  //if new line is followed by a letter
				positionArray[j] = i+1;								//record the position of beginning of next word
				j++;												//next index of the positionArray
			}
		}
		i++;														//search dictionary for \n
	}
}



