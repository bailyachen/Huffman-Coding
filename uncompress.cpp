#include "HCTree.hpp"
#include "HCNode.hpp"
#include <fstream>
#include <sstream>
using namespace std;

/**
 * Authors: Baily Chen. Will Knox
 * File Name: uncompress.cpp
 * Description: 
 */

int main (int argc, char** argv){
	//check if number of args is valid
	if(argc != 3) {
		cout << "Invalid number of arguments." << endl;
		return EXIT_FAILURE;
	}

	//create stream to decode
	ifstream toDecode;
	toDecode.open(argv[1], ios::in | ios::binary);

	//check if file valid
	if(!toDecode.good()) {
		cout << "Invalid input file. Please try again." << endl;
		return EXIT_FAILURE;
	}

	//check for empty tree
	if(toDecode.peek() == ifstream::traits_type::eof()) {
		ofstream empty(argv[2], ios::out);
		empty.close();
		toDecode.close();
		return EXIT_SUCCESS;
	}
	//read first byte to get a count for number of leaves
	BitInputStream in(toDecode);
	in.fill();
	unsigned int leafCount = in.readInt();
	in.fill();
	unsigned int trailingZeros = in.readInt();
	if (trailingZeros == 256) trailingZeros = 0;

	//reconstruct tree from header information
	HCTree* tree = new HCTree();
	in.fill();
	tree->build(leafCount, in);
	in.fill();
	
	//create stream for outfile
	ofstream decoded;
	decoded.open(argv[2], ios::out | ios::binary);
	
	int decodedInt = 0;
	//pick up where left off, going through decoded portion until eof
	bool nextCall = true;
	while(true) {
		if(toDecode.peek() == ifstream::traits_type::eof()) {
			if ((in.accessNBits()) == trailingZeros) {
				if(nextCall && !trailingZeros) {
					nextCall = false;
				}
				else break;
			}
		}
		
		//call tree's decode
		decodedInt = tree->decode(in);
		if (decodedInt == -1) {
			break;
		}
		//cast to char and write to outfile
		decoded << (unsigned char)decodedInt;
	}
	//close files and delete tree
	toDecode.close();
	decoded.close();
	delete tree;
	return EXIT_SUCCESS;
}
