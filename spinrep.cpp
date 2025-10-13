#include <iostream>
#include <string>
#include <vector>
using namespace std;

//individual words in Clifford algebra
class CliffWord {
public:
	//position 1 is the coefficient (0 or 1)
	//positions 2-(n+1) are 0 if e_{i-1} are absent, 1 if present
	vector<int> letters;
	string display() {
		string output = to_string(letters[0]);
		for (int i = 1; i < letters.size(); i++) {
			if (letters[i] == 1) {
				output += "e" + to_string(i);
			}
		}
		return output;
	}
};

CliffWord multiply (CliffWord a, CliffWord b) {
	CliffWord output;
	output.letters.resize(a.letters.size());
	//multiply coefficients
	output.letters[0] = a.letters[0] * b.letters[0];
	//multiply basis elements
	for (int i = 1; i < a.letters.size(); i++) {
		int sign = 0;
		//cost of sorting the product
		for (int j = i + 1; j < a.letters.size(); j++) {
			if (a.letters[j] == 1) {
					sign++;
			}
		}
		output.letters[i] = (a.letters[i] + b.letters[i]) % 2;
		//basis elements in a are already sorted, this sorts basis elements in b
		if (b.letters[i] == 1 && sign % 2 == 1) {
			output.letters[0] *= -1;
		}
	}
	return output;
}

class CliffSentence {
public:
	vector<CliffWord> coeffs;
	void simplify() {
		//combine like terms
		for (int i = 0; i < coeffs.size(); i++) {
			for (int j = i + 1; j < coeffs.size(); j++) {
				bool likeTerms = true;
				for (int k = 1; k < coeffs[i].letters.size(); k++) {
					if (coeffs[i].letters[k] != coeffs[j].letters[k]) {
						likeTerms = false;
						break;
					}
				}
				if (likeTerms) {
					coeffs[i].letters[0] += coeffs[j].letters[0];
					coeffs.erase(coeffs.begin() + j);
					j--;
				}
			}
		}
		//remove zero terms
		for (int i = 0; i < coeffs.size(); i++) {
			if (coeffs[i].letters[0] == 0) {
				coeffs.erase(coeffs.begin() + i);
				i--;
			}
		}
	}
	string display() {
		string output;
		for (int i = 0; i < coeffs.size(); i++) {
			output += coeffs[i].display();
			if (i < coeffs.size() - 1) {
				output += " + ";
			}
		}
		return output;
	}
};

CliffSentence multiply(CliffSentence a, CliffSentence b) {
	CliffSentence output;
	output.coeffs.resize(a.coeffs.size() * b.coeffs.size());
	//term-by-term multiplication
	for (int i = 0; i < a.coeffs.size(); i++) {
		for (int j = 0; j < b.coeffs.size(); j++) {
			output.coeffs[i * b.coeffs.size() + j] = multiply(a.coeffs[i], b.coeffs[j]);
		}
	}
	output.simplify();
	return output;
}

int main() {
	//dim = 3
	CliffSentence rot[10][10];
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			if (j == i) {
				rot[i][j].coeffs.resize(1);
				rot[i][j].coeffs[0].letters = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
			}
			else {
				rot[i][j].coeffs.resize(2);
				rot[i][j].coeffs[0].letters = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				if (j < i) {
					rot[i][j].coeffs[1].letters = { 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				}
				else {
					rot[i][j].coeffs[1].letters = { -1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
				}
				rot[i][j].coeffs[1].letters[i + 1] = 1;
				rot[i][j].coeffs[1].letters[j + 1] = 1;
			}
		}
	}
	
	//always multiply backwards!
	CliffSentence path32c = multiply(multiply(multiply(multiply(multiply(rot[1][2], rot[1][0]), rot[0][2]), rot[2][1]), rot[0][2]), rot[1][2]);
	//example: Type 3.1b easiest case
	CliffSentence path31b = multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(rot[1][0], rot[0][2]), rot[2][1]), rot[2][3]), rot[3][1]), rot[1][2]), rot[2][3]), rot[2][0]), rot[0][1]), rot[1][2]), rot[2][3]), rot[3][1]), rot[2][1]), rot[1][0]), rot[0][2]), rot[2][1]), rot[0][1]), rot[1][3]), rot[3][2]), rot[2][1]);
	CliffSentence path31a = multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(rot[1][0], rot[0][2]), rot[0][2]), rot[2][1]), rot[2][3]), rot[3][1]), rot[0][3]), rot[0][1]), rot[1][3]), rot[1][3]), rot[3][2]), rot[2][1]);
	CliffSentence path30c = multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(multiply(rot[1][0], rot[1][2]), rot[2][0]), rot[0][1]), rot[1][2]), rot[1][3]), rot[3][0]), rot[3][4]), rot[4][0]), rot[0][3]), rot[3][4]), rot[3][1]), rot[1][2]), rot[1][4]), rot[4][2]), rot[2][1]), rot[1][4]), rot[1][3]), rot[2][3]), rot[3][0]), rot[0][2]), rot[2][3]), rot[0][3]), rot[3][1]), rot[4][1]), rot[1][0]), rot[0][4]), rot[4][1]), rot[0][1]), rot[1][3]), rot[4][3]), rot[3][2]), rot[2][4]), rot[4][3]), rot[2][3]), rot[3][1]);

	cout << path32c.display() << endl;
	cout << path31a.display() << endl;
	cout << path31b.display() << endl;
	cout << path30c.display() << endl;

	return 0;

}

