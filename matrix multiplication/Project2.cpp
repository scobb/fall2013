/*
 * Project2.c
 *
 * Steven Cobb
 * Duncan Horn
 * F: 1-1:50
 *
 */
 
#include <stdio.h>
#include <stdint.h>
#include <assert.h> 
#include <math.h>
#include "MatrixMultiply.h"
 

/* multiplyMatrices
 * inputs: a, b, and c matrices stored row-major as arrays of doubles sized ST [A]x[B] = [C],
           integer-formated information about the number of columns in a and b
		   and the numbe	r of rows in a
   post-condition: [C] contains AB stored row-major as an array of doubles
 */
void multiplyMatrices(double a[], const uint32_t a_rows, const uint32_t a_cols,
										double b[], const uint32_t b_cols, // b_rows == a_cols
										double c[]) { // c_rows = a_rows, c_cols = b_cols 
	const uint32_t b_rows = a_cols; // for readability
	const uint32_t c_rows = a_rows; 
	const uint32_t c_cols = b_cols;
	int32_t i=0;
	while (i < c_rows){             //i loop will populate each row of c
		int32_t j = 0;
		while (j < c_cols){         //j loop will populate each col position the ith row
			int32_t k = 0;
			c[i*c_cols+j] = 0;		//zero out c's entry
			while (k < a_cols){     //this loop adds the scalar multiplications
				c[i*c_cols + j] += a[i*a_cols+k]*b[k*b_cols + j]; //term-multiplication
				k++;
			}
			j++;
		}
		i++;
	}
}

#define READY_FOR_STAGE2
#ifdef READY_FOR_STAGE2

/* these three variables are used in Stage 2 to implement allocateSpace */
#define TOTAL_SPACE 10000
double memory_pool[TOTAL_SPACE];
uint32_t top_of_pool = 0;

/* allocateSpace
   input: number of doubles to be allocated
   output: pointer to the first index of the allocated space
 */

PtrDouble allocateSpace(uint32_t size) {
	uint32_t current_top = top_of_pool;			//record return address
	top_of_pool += size;						//allocate the memory
	return memory_pool + current_top;			//return recorded address
}

/* multiplyMatrixChain
   inputs: array of pointers to double matrices to be added, integer arrays containing information
           about the number of rows and columns of each matrix, the number of matrices to be added,
		   an out matrix to store the result in, and integers with information about the number of
		   rows and columns of the out matrix
   post-condition: out points to the result of the matrix multiplication chain
 */
void multiplyMatrixChain(PtrDouble matrix_list[], // an array of pointers to matrices
												 uint32_t rows[], // rows[i] is the number of rows in matrix_list[i]
												 uint32_t cols[], // cols[i] is the number of cols in matrix_list[i]
												 uint32_t num_matrices, // the number of matrices in matrix_list
												 double out[], // the output matrix
												 uint32_t out_rows, uint32_t out_cols) { // the dimensions of the output matrix

	
	double* a_mat = matrix_list[0];					//a is the first matrix from the list
	uint32_t a_rows = rows[0];						// number of rows in a matrix
	uint32_t a_cols = cols[0];						// number of cols in a matrix
	uint32_t next_src = 1;							// index of next matrix to multiply by
	double* b_mat;
	double* c_mat;
	uint32_t c_rows;
	uint32_t c_cols;
	uint32_t b_cols;
	while (next_src < num_matrices) {
		b_mat = matrix_list[next_src];				//b's info pulled from matrix list		
		b_cols = cols[next_src];
		if (next_src + 1 < num_matrices  ){			//for all but the final stage, use intermediate matrix c 
			c_rows = a_rows;						//c's rows will depend on a	
			c_cols = b_cols;						//c's columns will depend on b
			c_mat = allocateSpace(c_rows*c_cols);	//allocating space for c according to its size
			multiplyMatrices(a_mat, a_rows, a_cols,
										 b_mat, b_cols,
										 c_mat);
			a_mat = c_mat;							// a matrix points to our result
			a_rows = c_rows;						// and takes on its characteristics
			a_cols = c_cols;						// for next iteration
		}
		else {										//for final stage, store result in out matrix
			multiplyMatrices(a_mat, a_rows, a_cols, b_mat, b_cols, out);
		}
		next_src += 1;						
	}	
	top_of_pool = 0;								//memory deallocate
}

#endif 