#include "HCTree.hpp"
#include "HCNode.hpp"
#include <fstream>
#include <sstream>
using namespace std;

/**
 * Authors: Baily Chen. Will Knox
 * File Name: compress.cpp
 * Description: 
 */

int main (int argc, char** argv){
	//check if number of args is valid
	if(argc != 3) {
		cout << "Usage: ./compress <infile outfile>." << endl;
		return EXIT_FAILURE;
	}

	//create stream for infile
	ifstream infile1;
	infile1.open(argv[1], ios::in |ios::binary);
	//check valid input
	if(!infile1.good()) {
		cout << "Invalid input file. Please try again." << endl;
		return EXIT_FAILURE;
	}
	if(infile1.peek() == ifstream::traits_type::eof()) {
		ofstream empty(argv[2], ios::out);
		empty.close();
		infile1.close();
		return EXIT_SUCCESS;
	}

	//create the vector that contains the frequenciesof each char
	char ch;
	int i;
	vector<int> freqs = vector<int>(256, 0);
	while(true) {
		i = infile1.get();
		unsigned char next = (unsigned char)i;
		if(infile1.eof()) break;
		//check for valid character
		if(i >= 0 && i <256) {
			freqs[(int)next] += 1;
		}
	}
	//build tree for encoding	
	HCTree* tree = new HCTree();
	tree->build(freqs);
	vector<HCNode*> leaves = tree->accessLeaves();

	//count number of unique characters in file for header
	int uniqueCount = 0;
	unsigned long long int maxLength = 0;
	for(unsigned int i=0; i<freqs.size(); i++) {
		if(freqs[i] > 0) {
			uniqueCount++;
			HCNode* currNode = leaves[i];
			int pathLength = 0;
			while (currNode->p) {
				pathLength++;
				currNode = currNode->p;
			}
			maxLength += (pathLength*freqs[i]);		
		}
	}
	int paddedZeros = maxLength%8;
				
	//close file
	infile1.close();
	
	//create stream for outfile
	ofstream outfile;
	outfile.open(argv[2], ios::out | ios::binary);
	
	//reopen infile
	ifstream infile2;
	infile2.open(argv[1], ios::in | ios::binary);


	//write file header of outfile
	//create outputstream
	BitOutputStream out(outfile);
	//write header count to first bit
	out.writeInt(uniqueCount);
	//write header to count trailing zeroes
	out.writeInt(paddedZeros);
	//write tree to bits
	tree->preOrderWrite(tree->accessRoot(), out);
	//fill and flush buffer if necessary
	out.fill();		
	

	//encode infile and append to outfile
	while(true) {
		//read char
		infile2.get(ch);
		if(infile2.eof()) break;
		//pass into HCTree::encode which will write to outfile
		//check for valid character before encode
		tree->encode((unsigned char)ch, out);
	}
	out.fill();
	infile2.close();
	outfile.close();
	delete tree;
	return EXIT_SUCCESS;
}

