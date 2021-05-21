#include "BitInputStream.hpp"

/** Fill the buffer from the input */
void BitInputStream::fill() {
	buf = in.get();
	nbits = 0;
}

/** Return nbits*/
int BitInputStream::accessNBits() {
	return nbits;
}

/** Read the next bit from the bit buffer.
  * Fill the buffer from the input stream first if needed.
  * Return 1 if the bit read is 1
  * Return 0 if the bit read is 0
  */
int BitInputStream::readBit() {
	// Get the bit at the appropriate location in the bit buffer
	unsigned char bit = buf & 0x80;
	bit = bit >> 7;
	// Increment the index
	nbits += 1;
	// If all bits in the buffer are read, fill the buffer first
	if(nbits == BYTE_SIZE) {
		fill();
	}
	if(nbits != 0) {
		buf = buf << 1;
	}
	// Then return the appropriate int
	if (bit == 1) return 1;
	else return 0;	
}
unsigned char BitInputStream::readByte() {
	unsigned char fromBuf;
	unsigned char c = 0;
	for(int i=0; i < BYTE_SIZE; i++) {
		//read MSB of buf and place into fromBuf
		fromBuf = buf & 0x80;
		//shift fromBuf by 7 to the right
		fromBuf = fromBuf >> 7;
		//shift buf left by 1
		buf = buf << 1;
		//place MSB of buf into LSB of c
		c = c | fromBuf;
		//increment nbits after taking away a bit
		nbits += 1;
		if(nbits == BYTE_SIZE) {
			fill();
		}
		//shift c left by 1
		if(i < BYTE_SIZE-1) {
			c = c << 1;
		}
	}
	return c;
}

unsigned int BitInputStream::readInt() {
	unsigned char fromBuf;
	unsigned char c;
	for(int i=0; i < BYTE_SIZE; i++) {
		//read MSB of buf and place into fromBuf
		fromBuf = buf & 0x80;
		//shift fromBuf by 7 to the right
		fromBuf = fromBuf >> 7;
		//shift buf left by 1
		buf = buf << 1;
		//place MSB of buf into LSB of c
		c = c | fromBuf;
		//increment nbits after taking away a bit
		nbits += 1;
		if(nbits == BYTE_SIZE) {
			break;
		}
		//shift c left by 1
		c = c << 1;
	}
	// if c is all zeros, we know the int is 256
	if (c == 0) return 256;
	//else it will be the int based on the binary pattern of c
	else return (unsigned int)c;
}
