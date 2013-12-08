/*Race.h

	Author: Steve Cobb
	EE 312
    UT - Austin
*/

#include <algorithm>
#include <stdint.h>
#include <stdio.h>
#include <vector>

using namespace std;


/*--Race-------------------------------------------------------------

	Binary search tree for car information

--------------------------------------------------------------------*/

class Race {
	
private:
	
	//car data structure will be our base node for BST
	struct car {
		car* parent;
		car* right;
		car* left;
		int num;				//car number
		vector<int> times;		//lap times
		int best_lap;			
	

		//operator < ------------------------------------------------

		//based primarily on # of laps started, and then on time that 
		//last lap was started

		bool operator<(car& that){
			if (this->times.size() == that.times.size()){
				return this->times[this->times.size()-1] < that.times[that.times.size()-1];
			}
			else return this->times.size() > that.times.size();

		}


	};
	

	//********************PRIVATE MEMBERS***************************
	car* root;						//root of our Race BST
	int best_lap;

	//****************PRIVATE MEMBER FUNCTIONS**********************

	/*---merge_sort------------------------------------------------

		Inputs: vector to be merged, index bounds left and right
		Post-condition: vector is merged from index range [left,right)

	--------------------------------------------------------------*/

	void merge_sort(vector<car*>& input, int left, int right){
		if (right - left < 2)
			return;
		int mid = (right+left)/2;

		merge_sort(input,left, mid);
		merge_sort(input,mid,right);

		merge(input, left, mid, right);
	}

	
	
	/*---merge---------------------------------------------------------

		Inputs: reference to vector<car*> to be merged into, left, mid,
				and right indices that separate vector into two mini-vectors
				to be merged.

		Post-Condition: input[left to mid) will be merged with 
				input[mid to right) and returned written back into input

	------------------------------------------------------------------*/
	
	void merge(vector<car*>&input, int left, int mid, int right){

		vector<car*> tmp;
		int get1 = left;
		int get2 = mid;

		while (get1 < mid && get2 < right){
			if (*input[get1]< *input[get2]){
				tmp.push_back(input[get1]);
				get1++;
			}
			else {
				tmp.push_back(input[get2]);
				get2++;
			}
		}

		while (get1 < mid){
			tmp.push_back(input[get1]);
			get1++;
		}

		while (get2 < right){
			tmp.push_back(input[get2]);
			get2++;
		}

		int put = left;
		for (int i = 0; i < tmp.size(); i++,put++){
			input[put] = tmp[i];

		}

	}

	/*---insert-----------------------------------------------------

	Inserts new entry into our race BST

	Inputs: root of BST, car number, lap completion time
	Output: pointer to root of BST

	---------------------------------------------------------------*/

	car* insert(car* root, int num,int laptime){
			
		car* p;

		//empty tree case

		if (root == 0){
			p = new car();
			p->left = 0;
			p-> right = 0;
			p->parent = 0;
			p->num = num;
			p->times.push_back(laptime);
			p->best_lap = LONG_MAX;
			return p;
		}

		p = root;

		//find where p should live

		while (1){
			if (num < p->num){
				if (p->left == 0)
					break;
				p = p->left;
			}
			else if (num > p->num){
				if (p->right == 0)
					break;
				p = p->right;
			}
			else return root;			//someone tried to insert and it was already there
		}

		//make a new car entry

		car* s = new car();
		s->parent = p;
		s->num = num;
		s->right = 0;
		s->left = 0;
		s->times.push_back(laptime);
		s->best_lap = LONG_MAX;
		
		//stitch new entry into tree

		if (num < p->num){	
			p->left = s;
		}
		else {
			p->right = s;
		}

		return root;
	}

	/*---find-------------------------------------------------------

	Finds entry in our BST with the correct car number, returns a 
	pointer to it

	Inputs: root pointer, car number to search for

	Output: pointer to searched car

	---------------------------------------------------------------*/
	car* find(car* root, int num){

		//base case: empty tree or not found

		if (root == 0)
			return 0;

		car* p;

		//found it!
		if (num == root->num){
			p = root;
		}

		//recursive call to left side of tree
		else if (num < root->num){
			p = find(root->left, num);
		}
		//recursive call to right side of tree
		else p = find(root->right, num);

		return p;
	}


	/*---find_smallest----------------------------------------------

	Finds the car with the smallest number in our BST

	Input: root pointer

	Output: pointer to car with smallest number in our BST

	---------------------------------------------------------------*/
	car* find_smallest(car* root){

		//empty tree case
		if (root == 0)
			return 0;
		
		//go left until you can't go left anymore

		car* p = root;
		while (p->left != 0){
			p = p->left;
		}
		return p;
	}
	
	/*---successor---------------------------------------------------
	
	Finds the successor of a given car in our race BST

	Input: pointer to car of whom we want to find the successor

	Output: pointer to successor

	----------------------------------------------------------------*/
	car* successor(car* p){

		//empty tree

		if (p == 0)
			return 0;

		//case if p has a right child (successor will be a descendant)

		if (p->right){
			p = p->right;
			while (p->left){
				p = p->left;
			}
			return p;
		}

		//case if p has no right child (successor will be an ancestor)

		else {
			//go until we take a left branch
			while (p->parent != 0 && p != p-> parent->left)
				p = p->parent;
			return p->parent;
		}
	}



	/*---destroy_tree----------------------------------------------------

		Inputs: pointer to root of tree
		Post-Condition: all nodes of tree deallocated

		Time Complexity: O(n)

	--------------------------------------------------------------------*/
	void destroy_tree(car* root){
		vector<car*> cleanup;
		car* p;
		p = find_smallest(root);

		//load vector with all car pointers
		while (p){
			cleanup.push_back(p);
			p = successor(p);
		}

		//delete each car node

		for (int i = 0; i < cleanup.size(); i++){
			delete cleanup[i];
		}
	}

public:	

	//********************PUBLIC MEMBER FUNCTIONS*******************************

	/*---Constructor------------------------------------------------------------

	Sets initial conditions for our tree

	--------------------------------------------------------------------------*/
	Race(void)
	{
		this->root = 0;
		this->best_lap = LONG_MAX;
	}

	/*---Destructor-------------------------------------------------------------

	Clean up heap allocations

	Time Complexity: O(n)
	---------------------------------------------------------------------------*/

	~Race(void){
		if (root){
			destroy_tree(root);
		}
	}


	/*---lap----------------------------------------------------------------------

		Inputs: car number, time of lap completion

		Post-Condition: If car does not exist in race, add them. If car does exist,
				update their info and global info.

	---------------------------------------------------------------------------*/


	void lap(int num,int laptime){
		car* p = find(root, num);
		if (p){
			p->times.push_back(laptime);

			//update best_lap
			int tmp = p->times[p->times.size()-1] - p->times[p->times.size()-2]; //time of lap just completed
			if ( tmp < p->best_lap){
				p->best_lap = tmp;
			}
			if (p->best_lap < this->best_lap){
				this->best_lap = p->best_lap;
			}
		}

		//didn't find car num, so add them

		else {
			root = insert(root,num,laptime);
			
		}
	}

	/*---print_results------------------------------------------------------------

		Calculates final places using Race information by loading BST into an array
		of vectors based on laps completed and then merge-sorting those vectors by
		differential. Outputs rankings, number of laps completed, and best lap time.

	----------------------------------------------------------------------------*/

	void print_results(void){
		car* p = find_smallest(root);
		vector <car*> results;

		//load BST into a vector of cars

	
		while (p){
			results.push_back(p);
			p = successor(p);
		}

		merge_sort(results,0,results.size());
		printf("There are %d cars in the race\n", results.size());
		for (int i = 0; i < results.size(); i++){
			printf ("Position %d was car %d, completing %d laps ",i+1, results[i]->num,results[i]->times.size()-1);
			if (results[i]->times.size() > 1){
				if (results[i]->best_lap == this->best_lap){
					printf("with the best lap of %d.%03d seconds\n",results[i]->best_lap/ 1000, results[i]->best_lap % 1000);
				}	
				else printf("with a best lap of %d.%03d seconds\n",results[i]->best_lap / 1000, results[i]->best_lap % 1000);
		
			}
			else printf("\n");
		}
	}
};
