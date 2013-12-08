#include <stdint.h>


/* BSTQueue.h

Steve Cobb
EE 312
UT - Austin

 */


struct Event{

	int32_t num;					//carID number
	int32_t timestamp;				//timestamp for the event

	//default constructor with invalid values

	Event(void){
		this->num = -1;
		this->timestamp = -1;
	}

	//useful constructor

	Event(int32_t num, int32_t timestamp){
		this->num = num;
		this->timestamp=timestamp;
	}


	//comparison operators compare timestamps

	bool operator<(Event& that){
		return this->timestamp < that.timestamp;
	}

	bool operator==(Event& that){
		return this->timestamp == that.timestamp;
	}
};

struct BST {
private:
	struct Node;
	Node* root;
	uint32_t length;
public:
	Node* smallest_node;				//this will track our smallest node

	struct Node {
	public:
		Event value;					// this is our payload, an event
		Node* left;						// left and right are pointers to our two children. 
		Node* right;					// either or both of left/right can be null (i.e., equal to zero)
		Node* parent;					// the parent pointer points to our parent.
					  
	};

	//returns true if tree is empty

	bool isEmpty(void){
		return !this->root;
	}

	//default constructor, initializes empty tree

	BST(void) {
		root = 0;
		length = 0;
		smallest_node = 0;
	}

	//copy constructor
	 BST(BST& that) {
		 copy(that);
	 }

	 //destructor
	~BST(void) { destroy(); }

	//reassignment operator
	BST& operator=(BST& that) {
		if (this != &that) {
			destroy();
			copy(that);
		}
		return *this;
	}

	//returns size of BST
	uint32_t size(void) { return length; }

	/*---insert----------------------------------------

	Inserts event into priority queue, altering any event
	that would be at the same time as another by 1ms in-
	crements until it doesn't conflict

	Input: event to be inserted
	Output: timestamp for event  

	--------------------------------------------------*/

	uint32_t insert(Event value) {

		//find if our event's timestamp conflicts with already-existing events

		Node* p = base_find(root,value);

		//we'll exit this loop if p has no successor or if its successor's timestamp doesn't equal value's timestamp
		
		while (p) {
			if (p->value == value){
				value.timestamp++;
				p = successor(p);
			} else break;
		}
		//insert event
		root = base_insert(root, value);

		//check to see if smallest node needs to be updated

		if (smallest_node){
			if (value < smallest_node->value){
				smallest_node = smallest(root);
			}
		}
		else smallest_node = root;				//tree was previously empty
			
		//increment length
		length += 1;

		//return (possibly new) timestamp
		return value.timestamp;
	}

	/*---remove_smallest----------------------------------------
		This deletes the smallest event from the queue and updates
		the smallest node pointer.

		Inputs: none
		Outputs: none

	-----------------------------------------------------------*/

	void remove_smallest(void){

		Node* parent = smallest_node->parent;

		//if the node has a parent, it's not the root

		if (parent){

			//find new smallest node
			Node* new_smallest = successor(smallest_node);

			//stitch around
			parent->left = smallest_node->right;
			if (smallest_node->right)
				smallest_node->right->parent = parent;

			delete smallest_node;

			//update smallest node
			smallest_node = new_smallest;
		}

		//otherwise it is the root, and we need a new root
		else {
			//new root, but might be null
			root = smallest_node->right;
			Node* new_smallest = 0;

			//if tree isn't empty, we'll set root's parent to null and find a new smallest node
			if (root){
				root->parent = 0;
				new_smallest = successor(smallest_node);
			}
			delete smallest_node;

			//this will be 0 for an empty tree, and point to the smallest otherwise
			smallest_node = new_smallest;
		}

		//decrement length
		length--;
	}

	void remove(Event value) {
		root = base_remove(root, value);
		length -= 1;
	}

	bool is_member(Event value) {
		return 0 != base_find(root, value);
	}

private:
	void destroy(void) {
		destroyTree(root);
	}

	void copy(BST& that) {
		root = copyTree(that.root);
		length = that.length;
	}


	static Node* base_insert(Node* root, Event key) {
		Node* child = new Node;
		child->value = key;
		child->left = 0;
		child->right = 0;
		child->parent = 0;

		if (root == 0) { 
			return child;
		}

		Node* parent = root;
		while (true) {
			if (key < parent->value) {
				if (parent->left == 0) { break; }
				parent = parent->left;
			} else {
				if (parent->right == 0) { break; }
				parent = parent->right;
			}
		}

		if (key < parent->value) { 
			parent->left = child;
		} else {
			parent->right = child;
		}
		child->parent = parent;

		return root;
	}

	static Node* base_remove(Node* root, Event key) {
		Node* p = base_find(root, key);
		if (p == 0) { // WTF???
			return root;
		}

		if (p->right == 0) { // case A, no right subtree (easy)
			Node* parent = p->parent;
			if (parent == 0) {
				root = p->left;
			} else {
				if (parent->right == p) {
					parent->right = p->left;
				} else {
					parent->left = p->left;
				}
			}
			if (p->left != 0) {
				p->left->parent = parent;
			}

			delete p;
		} else { // case B, has a right subtree (hard)
			Node* s = successor(p);
			p->value = s->value;

			Node* parent = s->parent;
			if (s == parent->right) {
				parent->right = s->right;
			} else {
				parent->left = s->right;
			}

			if (s->right != 0) {
				s->right->parent = parent;
			}
			delete s;
		}
		return root;
	}

	static Node* base_find(Node* root, Event key) {
		Node* p = root;

		while (p != 0) {
			if (p->value == key) { return p; }
			else if (key < p->value) {
				p = p->left;
			} else {
				p = p->right;
			}
		}
		return 0;
	}

	static void destroyTree(Node* root) {
		if (root == 0) { 
			return;
		}

		destroyTree(root->left);
		destroyTree(root->right);

		delete root;
	}

	static Node* copyTree(Node* root) {
		if (root == 0) { return 0; }

		Node* p = new Node;
		p->value = root->value;
		p->parent = 0;

		Node* left_child = copyTree(root->left);
		Node* right_child = copyTree(root->right);

		p->left = left_child;
		if (left_child != 0) { left_child->parent = p; }

		p->right = right_child;
		if (right_child != 0) { right_child->parent = p; }

		return p;
	}

	static Node* smallest(Node* root) {
		if (! root) { return 0; } // WTF? smallest in an empty tree????

		while (root->left != 0) {
			root = root->left;
		}

		return root;
	}

	void print_tree(void){
		if (!root)
			return;
		printf("our queue:\n");
		Node* p = smallest(root);
		while (p){
			printf("car %d timestamp %d\n",p->value.num,p->value.timestamp);
			p=successor(p);
		}
		printf("\n");
	}

	static Node* successor(Node* p) {
		if (p == 0) { // WTF?
			return 0;
		}

		if (p->right != 0) {
			Node* s = p->right;
			while (s->left != 0) {
				s = s->left;
			}
			return s;
		} else {
			Node* s = p->parent;
			while (s != 0 && s->left != p) {
				p = s;
				s = p->parent;
			}
			return s;
		}
	}

};


