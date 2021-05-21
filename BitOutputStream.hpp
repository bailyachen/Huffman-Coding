#include <iostream>

class BitOutputStream {

private:
	char buf;
	int nbits;
	std::ostream & out;
	
	//size of byte in bits
	const int BYTE_SIZE = 8;

public:
	/** Initialize a BitOutputStream that will use the given ostream for output. */
	BitOutputStream(std::ostream & os) : out(os), buf(0), nbits(0) {
		// clear buffer and bit counter
	}

	/** Send the buffer to the output, and clear */
	void flush();

	/** Write the least significant bit of the argument to the bit buffer,
 	  * and increment the bit buffer index. But flush the buffer first, if
 	  * it is full */
	void writeBit(int i);
	
	/** Write the entire byte from an ascii char to the buffer, 
	 *  flushing it as the buffer gets full and continuing until 
	 *  the character is completely written */
	void writeByte(char c);

	/** Write entire byte for an int less than 256
 	  */
	void writeInt(int i);

	// Fill buffer with ending zeros to finish writing stream
	void fill();

}; // End of Class
