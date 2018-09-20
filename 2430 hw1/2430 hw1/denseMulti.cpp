#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "ArgumentManager.h"

using namespace std;

int isNumber(string line);
bool errors(string filename, string outputName);

int main(int argc, char* argv[]) {	
	//start of code to run in linux server
	if (argc != 2 && argc != 4) {
		cout << "Usage: denseMulti \"A = <file>; B = <file>; C = <file>\"" << endl;
		return -1;
	}

	ArgumentManager am(argc, argv);
	string inFilename1 = am.get("A");
	string inFilename2 = am.get("B");
	string outFilename = am.get("C");

	cout << "File name for Matrix A: " << inFilename1 << endl <<
		"File name for Matrix B: " << inFilename2 << endl <<
		"File name for Matrix C: " << outFilename << endl;
	//end of code to run in linux server
	ifstream inFile1(inFilename1.c_str());	
	errors(inFilename1, outFilename); //function that checks for various errors

	double value = 0; //value holds the number read from .txt
	string aLine;
	ofstream outFile;
	double m1[20][20] = { { 0 } }, m2[20][20] = { { 0 } }, outM[20][20] = { { 0 } };
	int aCol = 0, aRow = 0, bCol = 0, bRow = 0;

	while (getline(inFile1, aLine)) { //read line by line
		stringstream ss(aLine);
		aCol = 0;

		while (ss >> value) {
			if (isNumber(aLine) > 0) { //if alphabets are found or >0. output file is created								 
				ofstream outFile(outFilename.c_str());
				outFile << "error" << endl;
				outFile.close();
				return -1; //force exit
			}
		
			if (isNumber(aLine) == 0) {
				m1[aRow][aCol] = value; //assign value to current matrix dimensions
				aCol++;
			}
		}
		aRow++;
	}

	ifstream inFile2(inFilename2.c_str());
	errors(inFilename2, outFilename);

	value = 0;
	string bLine;

	while (getline(inFile2, bLine)) {
		stringstream ss(bLine);
		if (isNumber(bLine) > 0) {
			ofstream outFile(outFilename.c_str());
			outFile << "error" << endl;
			outFile.close();
			return -1; //force exit
		}
		bCol = 0;
		while (ss >> value) {
			m2[bRow][bCol] = value;
			bCol++;
		}
		bRow++;
	}

	outFile.open(outFilename.c_str()); //creating output file
	
	if (!outFile.is_open()) { //creates error if ouput file can't be created
		cerr << "-----ERROR CAN'T CREATE OUTPUT FILE-----" << endl;
		return -1; //force exit
	}
	else if (aCol != bRow) { //creates error if m1 col != m2 row 
		outFile << "error" << endl; //print error in output file
		outFile.close();
		return -1; //force exit
	}
	else { //matrix multiplication
		for (int i = 0; i < aCol; i++) {
			for (int j = 0; j < bCol; j++) {
				outM[i][j] = 0;
				for (int k = 0; k < bRow; k++) {
					outM[i][j] += m1[i][k] * m2[k][j];
				}
			}
		}
	}
	outFile.precision(2);	//prints numbers up to only hundredth space after decimal
	outFile.setf(ios::fixed);
	//for loops that'll write the output Matrix onto the output file
	for (int i = 0; i< aRow; i++) {
		for (int j = 0; j < bCol; j++) {
			outFile << outM[i][j] << " ";
		}
		outFile << endl;
	}
	outFile.close(); //closing output file

	return 0;
}

int isNumber(string line) { //checks each char in string for illegal char. method of for loop found on geeksforgeeks
	int illegal = 0; //keeps track of how many letters found
	for (int i = 0; line[i] != '\0'; i++) {
		if (isalpha(line[i]) != 0) { //checks for letters
			illegal++;
			cerr << "-----ERROR A NON-NUMBER IS FOUND-----" << endl;
			return illegal; //returns current value of alphafound
		}
		if (((line[i] == '+') && (line[i + 1] == '+'))
			|| ((line[i] == '-') && (line[i + 1] == '-'))) { //checks for ++ or -- after or before numbers
			illegal++;
			cerr << "-----ERROR '++' or '--' WAS FOUND-----" << endl;
			return illegal;
		}
		if ((line[i] == '.') || (line[i] == '-') || (line[i] == '+')) { //checks for empty spaces after '.', '+', and '-'
			if ((line[i + 1] == ' ') || (isalpha(line[i + 1]) != 0)) {
				illegal++;
				cerr << "-----ERROR ILLEGAL CHAR AFTER '.', '-', or '+' WAS FOUND-----" << endl;
				return illegal;
			}
		}
		if (((line[i] == '\n') || (line[i] == ' ')) 
			&& ((line[i + 1] == '\n') || (line[i + 1] == ' '))) {
			illegal--;
			cerr << "-----ERROR MULTIPLE EMPTY LINES OR EMPTY SPACES FOUND-----" << endl;
			return illegal;
		}
	}
	return illegal; //returns 0 if no discrepancies are found
}

bool errors(string filename, string outputName) {	//function that checks for errors
	ifstream inputFile(filename.c_str());
	string line;
	getline(inputFile, line);

	if (!inputFile.is_open()) { //error if file is not found or can't be opened
		cerr << "-----ERROR FILE DOES NOT EXIST OR CAN'T OPEN-----" << endl;
		return -1;
	}
	if (line.empty()) {	//error if file is empty
		cerr << "-----ERROR FILE IS EMPTY-----" << endl;
		ofstream outputFile(outputName.c_str());
		outputFile << "error" << endl;	//write error on output file if one of the files are empty
		outputFile.close();
		return -1;
	}
}