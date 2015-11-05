#include <iostream>
#include <vector>
#include <stdlib.h>
using namespace std;

/* The big number is stored in the form of a vector, vector<int> digits.
 * digits[0] is the least significant bit.
 */

class BigInt { 
    private:
	vector<int> digits;	

    public:
	BigInt ();
	BigInt (vector<int> a);
	BigInt (const BigInt&);
	BigInt& operator= (const BigInt&);
	bool operator== (const BigInt&) const;
	BigInt operator+ (const BigInt&) const;
	BigInt operator- (const BigInt&) const;
	BigInt operator* (const BigInt&) const;
	BigInt operator/ (const BigInt&) const;
	BigInt simpleMultiply (const BigInt&, int) const;
	void showBigInt();
	bool compareTo (const BigInt&) const;
	void format();
};

BigInt::BigInt() {}

BigInt::BigInt(vector<int> a){
    digits = a;
    this -> format();
}

BigInt::BigInt(const BigInt& a){
    digits = a.digits;
}

void BigInt::showBigInt(){
    for (int i = digits.size() - 1; i >= 0; i--)
	cout << digits[i];
    cout << endl;
}

bool BigInt::compareTo (const BigInt& a) const{
    if (digits.size() > a.digits.size()) return 1;
    else if (digits.size() < a.digits.size()) return 0;
    else{
	for (int i = digits.size() -1; i >= 0; i--) {
	    if (digits[i] > a.digits[i]) return 1;
	    if (digits[i] < a.digits[i]) return 0;
	}
	return 0;
    }
}

bool BigInt::operator== (const BigInt& a) const{
    if (digits.size() != a.digits.size()) return 0;
    else{
	for (int i = 0; i < digits.size(); i++){
	    if (digits[i] != a.digits[i]) return 0;
	}
    }
    return 1;
}

void BigInt::format() {
    int n = digits.size() - 1;
    while (digits[n] == 0 && n > 0){
	digits.erase(digits.end() - 1);	
	--n;
    }
}

BigInt& BigInt::operator=(const BigInt& operand){
    
    this -> digits = operand.digits;
    return *this;
}

BigInt BigInt::operator+ (const BigInt& operand) const{
    BigInt c;
    BigInt a(operand);
    BigInt b(*this);
    int temp = 0;
    int carryAdder = 0;

    //check the sizes of the objects.    
    int sizeDiff = b.digits.size() - a.digits.size();
    if (sizeDiff >= 0){
	for (int i = 0; i < sizeDiff; i++) a.digits.push_back(0);
    }
    else{
	for (int i = 0; i < (-sizeDiff); i++) b.digits.push_back(0);
    } 
    
    for (vector<int>::size_type i = 0; i != a.digits.size(); ++i) {
	temp = b.digits[i] + a.digits[i] + carryAdder;
	if (temp < 10){
	    c.digits.push_back(temp);
	    carryAdder = 0;
	} 
	else{
	    c.digits.push_back(temp - 10);
	    carryAdder = 1;
	}
	if (i == a.digits.size() - 1 && carryAdder == 1) c.digits.push_back(1);
    }
    return c;
}



BigInt BigInt::operator- (const BigInt& operand) const{
    BigInt c;
    BigInt a(operand);
    BigInt b(*this);
    int temp = 0;
    int carryAdder = 0;
    int sign;
    if (this -> compareTo(operand)) {sign = 1;}
    else {sign = -1;}
    
    //check the sizes of the objects.    
    int sizeDiff = b.digits.size() - a.digits.size();
    if (sizeDiff >= 0){
	for (int i = 0; i < sizeDiff; i++) a.digits.push_back(0);
    }
    else{ 
	for (int i = 0; i < -sizeDiff; i++) b.digits.push_back(0);
    }

    for (vector<int>::size_type i = 0; i != a.digits.size(); ++i) {
	temp = sign *  (b.digits[i] - a.digits[i]) - carryAdder;
	if (temp < 0){
	    c.digits.push_back(temp + 10);
	    carryAdder = 1;
	} 
	else{
	    c.digits.push_back(temp);
	    carryAdder = 0;
	}
    }
    c.format();
    int signInd = c.digits.size() - 1;  
    c.digits[signInd] *= sign;
    return c;
}

BigInt BigInt::simpleMultiply (const BigInt& a , int b) const{
//b is an integer [0, 9]
    BigInt c;
    int carry = 0;
    int temp = 0;
    for (int i = 0; i < a.digits.size(); i++){
	temp = a.digits[i] * b + carry;
	c.digits.push_back(temp % 10);
        carry = temp / 10;
	if (i == a.digits.size() - 1 && carry > 0)
    	    c.digits.push_back(carry);
    }
    c.format();
    return c;
}

BigInt BigInt::operator* (const BigInt& operand) const{
    BigInt a, b, c;
    BigInt temp;
    c.digits.push_back(0);
    if (this -> compareTo(operand)){
	b = *this;
	a = operand;
    }

    else{
	b = operand;
	a = *this;
    }
    for ( int i = 0; i < a.digits.size(); i++){
	BigInt d (a.simpleMultiply (b, a.digits[i]));
	BigInt e;
	for (int j = 0; j < i; j++) e.digits.push_back(0);
	for (int k = 0; k < d.digits.size(); k++) e.digits.push_back(d.digits[k]); 
	c = c + e;
	//temp = c + e;
	//c = temp;     
    }
    return c;
}

BigInt BigInt::operator/ (const BigInt& operand) const{
    if (*this == operand){
	vector<int> ans;
	ans.push_back(1);
	BigInt result(ans);
	return result;
    }
    else if (!(this -> compareTo(operand))){
	vector<int> ans;
	ans.push_back(0);
	BigInt result(ans);
	return result;	
    }
    else {
	int sizeDiff = this -> digits.size() - operand.digits.size();
	BigInt result;
	for (int i = 0; i <= sizeDiff; i++) result.digits.push_back(0);
	BigInt d(*this);
	for (int i = sizeDiff; i >= 0; i--){
	    BigInt e;
	    for (int j = 0; j < i; j++) e.digits.push_back(0);
	    for (int k = 0; k < operand.digits.size(); k++)
		 e.digits.push_back(operand.digits[k]);
	    int division = 0;
	    while (true){
		if(!d.compareTo(e)){
		    if (d == e){
			 division++;
			 d = d - d.simpleMultiply(e, division);
		    }
		    result.digits[i] = division;
		    break;
		}
		else{
		division++;
		d = d - e;
		} 
	    }
	}
	result.format();
	return result;
    }

}

int main() {
    string a, b, oper;
    getline(cin, a);
    getline(cin, oper);
    getline(cin, b);
    
    vector<int> av;
    vector<int> bv;

    for (int i = a.length() - 1; i >= 0; i--){
	char c[1] = {a[i]};
	int k = atoi(c); 
	av.push_back(k);     
    }
    BigInt aa(av);

    for (int i = b.length() - 1; i >= 0; i--){
	char c[1] = {b[i]};
	int k = atoi(c); 
	bv.push_back(k);     
    }
    BigInt bb(bv);

    BigInt cc;

    char operc = oper[0];
    switch(operc){
	case '+':
	    cc = aa + bb;
	    break;
	case '-':
	    cc = aa - bb;
	    break;
	case '*':
	    cc = aa * bb;
	    break;
	case '/':
	    cc = aa / bb;
	    break;
	default:
	    cout <<"Not valid operator." <<endl;
    }
    cc.showBigInt();
    return 0;

}
