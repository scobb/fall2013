// Project10.hpp -- definition of the MyString class
/* Steve Cobb
*/

#ifndef _Project10_hpp
#define _Project10_hpp once
#include <cstdint>
#include <iostream>
#include <vector>
int ignoreCase(char);

struct stringHolder{
	const char* data;
	int len;
	bool deleteMe;

	//default constructor
	stringHolder(){
		data = 0;
		len = 0;
	}

	//constructor that takes an existing char* and integer length
	stringHolder(const char* x, int y){
		data = x;
		len = y;
	}

	//copy constructor
	stringHolder(const stringHolder&  that){
		this->data = that.data;
		this->len = that.len;
	}
	
	//boring destructor
	~stringHolder(void){

	}

	//comparison function used for case-insensitive sorting

	bool operator < (const stringHolder& that) const{
		int it1 = 0;
		int it2 = 0;
		while (it1 < this->len && it2 < that.len){
			if (ignoreCase(this->data[it1]) < ignoreCase(that.data[it2])){
				return true;
			}
			else if (ignoreCase(this->data[it1]) > ignoreCase(that.data[it2])){
				return false;
			} 
			else {
				it1++;
				it2++;
			}
		}
		if (it2 == that.len)
			return false;
		else if (it1 == this->len)
			return true;
		else return false;
	}
};

//MyString class
//basically a fancy version of the character vector

struct MyString {
	
public:

	std::vector<char> string;

	
	//standard constructor
	MyString (void){

	}

	MyString (stringHolder that){
		string = std::vector<char>(that.len);

		for (int i = 0; i < that.len; i++){
			string[i] = ignoreCase(that.data[i]) + 'A';
		}

	}

	//constructor given a character vector
	MyString(const std::vector<char> that){
		string = that;

	}

	//constructor given a C string
	MyString (const char* that){
		int i = 0;
		while (that[i] != 0){
			string.push_back(that[i]);
			i++;
		}
	}
	 
	//copy constructor
	MyString (const MyString& that){
		this->string = that.string;
	}


	//destructor
	~MyString (void){
		
	}

	
	//reassignment operator

	MyString& operator= (const MyString& that){
		this->string = that.string;
		return *this;
	}

	//size function
	int size(void) const {
		return this->string.size();
	}

	//push_back with amortized doubling
	void push_back(char a){
		string.push_back(a);
	}

	//element-access brackets
	char& operator[] (int n) {
		return string[n];
	}

	//resets MyString to empty
	void reset(void){
		string.clear();
	}

	//concatenation with +=, modifies this object
	void operator+= (const MyString& x){
		for (char i : x.string){
			this->string.push_back(i);
		}
	}

	//returns stringHolder with ptr to MyString's data

	stringHolder toUpper(void) const {
		
		return stringHolder(this->string.data(),string.size());
		
	}


	

	//input function
	void readInput(std::istream& input) {
		reset();

		char ch = input.get();
		while (input
			&& (ch == ' ' || ch == '\n' || ch == '\t')) {
			ch = input.get();
		}

		do {
			push_back(ch);
			ch = input.get();
		} while (input && ch != ' ' && ch != '\n' && ch != '\t');
	}

	//output function
	void writeOutput(std::ostream& out) const {

		for (char i : string){
			out << i;
		}
	}

	//equality comparison
	bool operator==(const MyString& that) const {
		return this->string == that.string;
	}

	//inequality comparison
	bool operator!=(const MyString& that) const{
		return this->string != that.string;
	}

	//less-than comparison
	bool operator<(const MyString& that) const{
		
		std::vector<char>::const_iterator p1 = this->string.begin();
		std::vector<char>::const_iterator p2 = that.string.begin();

		while (p1 != this->string.end() && p2 != that.string.end()){
			if (*p1 < *p2){
				return true;
			}
			else if (*p2 < *p1){
				return false;
			}
			p1++;
			p2++;
		}

		if (p2 == that.string.end())
			return false;
		else if (p1 == this->string.end())
			return true;
		else return false;


	}

};

inline std::istream& operator>>(std::istream& in, MyString& str) {
	str.readInput(in);
	return in;
}

inline std::ostream& operator<<(std::ostream& out, MyString str) {
	str.writeOutput(out);
	return out;
}

//swap overload; doesn't save much
void mySwap(MyString& x, MyString& y) {
	std::vector<char> tmp = x.string;
	x.string = y.string;
	y.string = tmp;
	}


//if a letter, returns the offset from either case of A
//if not a letter, returns the offset from A
//used for case-insensitive compare

int ignoreCase(char a){
	
	if (a >= 'a' && a <= 'z'){
		return a-'a';
		}
	return a-'A';
}

#endif /* _Project10_hpp */
