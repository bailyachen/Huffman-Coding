#include <iostream>

class BitInputStream {

private:
	char buf;
	int nbits;
	std::istream & in;
	// size of byte in bits
	const int BYTE_SIZE = 8;

public:
/** Each of the function here should be a "Mirror" of BitInputStream class */

//For example, What should be the "Mirror" of BitInputSteam::flush?

	/** Initialize a BitInputSteam that will use the given istream for input */
	BitInputStream(std::istream & is) : in(is), buf(0), nbits(0) {}

	/** Fill the buffer from the input */
	void fill();

	/* return nbits for counting in uncompress*/
	int accessNBits();

	/** Read the next bit from the bit buffer.
          * Fill the buffer from the input stream first if needed.
          * Return 1 if the bit read is 1
          * Return 0 if the bit read is 0
          */
	int readBit();

	/** Read the next byte from the bit buffer.
 	  * Fill from stream if needed
 	  * returns character byte */
	unsigned char readByte();

	/** Read an integer from the first byte in the stream 
 	  * We are reading the first byte which will only hold an int up to 255	
          * if the int is 0, we know that it is 256 unique symbol leaf nodes
          * this is because empty files hold no data */
	unsigned int readInt();

}; //End of Class

	//void fill() { 
	//Read 1 Byte into Buffer, and reset the counter
	//}
