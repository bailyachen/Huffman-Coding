/* HCTree.cpp
 * By Will Knox and Baily Chen
 * wknox@ucsd.edu // bac028@ucsd.edu
 * For cs100sp19
 * Cpp for HCTree datastructure
 * "Description"
 */

#include "HCTree.hpp"

/*Give access to Root*/
HCNode* HCTree::accessRoot() {
	return root;
}

/*gives access to leaves*/
vector<HCNode*> HCTree::accessLeaves() {
	return leaves;
}

/*Build the HCTree*/
void HCTree::build(const vector<int>& freqs)
{
	//create a priority queue that holds each node
	priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> queue;
	//set the character byte and count of each HCNode in leaves to match that of freqs
	for(unsigned int i = 0; i < leaves.size(); i++) {
		if(freqs[i] > 0) {
			leaves[i] = new HCNode(freqs[i], (char)i);
			queue.push(leaves[i]);
		}
	}
	while(queue.size() > 1) {
		// pop first two elementsp freqs
		HCNode* firstNode = queue.top();
		queue.pop();
		HCNode* secondNode = queue.top();
		queue.pop();
		// create parent
		int newCount = firstNode->count + secondNode->count;
		HCNode* parent = new HCNode(newCount, firstNode->symbol);
		parent->c0 = firstNode;
		parent->c1 = secondNode;
		parent->c0->p = parent;
		parent->c1->p = parent;
		// reinsert
		queue.push(parent);
	}
	root = queue.top();
}

/* Build the tree when only info is leafcount*/
void HCTree::build(unsigned int& leafCount, BitInputStream& in) {
	//get first bit
	//if 1, root is also a leaf
	int readBit = in.readBit();
	if(readBit == 1) {
		unsigned char symbol = in.readByte();
		root = new HCNode(0, symbol);
	}
	else {
		//create a new node to be the root
		root = new HCNode(0, '0');
		//delegate rest of build to preOrderRead
		preOrderRead(root, in, leafCount); 
	}
}	

/* preOrder helper function
 * traverses the tree and writes each node to header file
 */
void HCTree::preOrderWrite(HCNode* node, BitOutputStream& out) {
	//first write the info of the node to the output stream
	//if node has children: inside node, write 0
	if(node->c0 || node->c1) {
		out.writeBit(0);
		preOrderWrite(node->c0, out);
		preOrderWrite(node->c1, out);
	}
	//if node has no children, is leaf: write 1 and char
	else {
		out.writeBit(1);
		out.writeByte(node->symbol);
	}
}

void HCTree::preOrderRead(HCNode* node, BitInputStream& in, unsigned int& leafCount) {
	//check if leafcount is 0, return
	if(!leafCount) {
		return;
	}
	//first, read bit to decide what to do to left node
	//if 0, create a new left child left of current node, and call recursivley on it
	int readBit = in.readBit();
	if(readBit == 0) {
		node->c0 = new HCNode(0, '0');
		preOrderRead(node->c0, in, leafCount);
		if(!leafCount) return;
	}
	//if 1, create a new left child with a symbol, and end recursion
	else {
		node->c0 = new HCNode(0, in.readByte());
		leafCount--;
	}
	//next, read bit again for right side and repeat logic
	readBit = in.readBit();
	if(readBit == 0) {
		node->c1 = new HCNode(0, '0');
		preOrderRead(node->c1, in, leafCount);
		if(!leafCount) return;
	}	
	//otherwise, node to add is a leaf
	else {
		//if currNode has no left, insert newNode there
		node->c1 = new HCNode(0, in.readByte());
		leafCount--;
	}
}


void HCTree::encode(unsigned char symbol, ofstream& out) const
{
	//get node of symbol in leaves
	HCNode* node = leaves[symbol];
	//move to parent
	string buffer = "";
	if(root == node) {
		out << 0;
		return;
	}
	while (node) {
		
		//check children if node was left or right child
		if (node->p) { 
			if (node->symbol == node->p->symbol) {
			//node is left append 0 to string
			buffer.insert(0, 1, '0');
			}
			//else node is right append 1 to string
			else buffer.insert(0, 1, '1');
		} else break;
		node = node->p;
	}
	out << buffer;
}

int HCTree::decode(ifstream& in) const
{
	//starting at root, traverse tree according to input bits
	HCNode* currNode = root;
	char ch;
	//run until a leaf is reached	
	while(currNode->c0 || currNode->c1) {
		in.get(ch);
		//if character is 0, move left
		if(ch == '0') {
			currNode = currNode->c0;
		}
		//else character is 1, move right
		else currNode = currNode->c1;
	}
	//now currNode should be pointing to a leaf
	//if currNode is still poitning to the root, that's fine:
	//just eat a character and return currnode's symbol
	if(currNode == root) in.get(ch);
	return currNode->symbol;
}

void HCTree::encode(unsigned char symbol, BitOutputStream& out) const
{
	//get node of symbol in leaves
	HCNode* node = leaves[symbol];
	//move to parent
	string buffer = "";
	if(root == node) {
		out.writeBit(1);
		return;
	}
	while (node) {
		
		//check children if node was left or right child
		if (node->p) { 
			if (node->symbol == node->p->symbol) {
			//node is left append 0 to string
			buffer.insert(0, 1, '0');
			}
			//else node is right append 1 to string
			else buffer.insert(0, 1, '1');
		} else break;
		node = node->p;
	}
	//read through "buffer" and write each bit
	int toWrite = 0;
	for(unsigned int i=0; i<buffer.length(); i++) {
		//compare char value and set appropriately
		if(buffer[i] == '1') {
			toWrite = 1;
		}
		else toWrite = 0;
		out.writeBit(toWrite);
	}	
}

int HCTree::decode(BitInputStream& in) const
{
	//starting at root, traverse tree according to input bits
	HCNode* currNode = root;
	//run until a leaf is reached
	while(currNode->c0 || currNode->c1) {
		//if character is 0, move left
		if(in.readBit() == 0) {	
			currNode = currNode->c0;
		}
		//else character is 1, move right
		else currNode = currNode->c1;
	}
	//now currNode should be pointing to a leaf
	//if currNode is still poitning to the root, that's fine:
	//just eat a character and return currnode's symbol
	if(currNode == root) {
		int readBit = in.readBit();
		if(readBit == 0) {
			return -1;
		}
	}

	//returns the symbol from the leaf node
	return currNode->symbol;
}

HCTree::~HCTree() {
	deleteAll(root);
}
