#include "BitOutputStream.hpp"

void BitOutputStream::flush() {
	//put the entire buffer (1 byte) into the ostream and flush it
	out.put(buf);
	out.flush();
	//clear buffer and bit counter below
	buf = nbits = 0;
}

/** Write the least significant bit of the argument to the bit buffer,
  * and increment the bit buffer index. But flush the buffer first, if
  * it is full */
void BitOutputStream::writeBit(int i) {
	// If buffer is full, clear and reset nbits and place char in stream
	// sizeof(double) == 8 which is the number of bits
	// Write the LSB of i into the buffer at the current index
	buf = buf | (unsigned char)i;
	// shift buf by 1 for next time it is called
	nbits += 1;
	if(nbits == BYTE_SIZE) {
		flush();
	}
	buf = buf << 1;
	// Increment the index
}
/** Write the entire byte from an ascii char to the buffer, 
 *  flushing it as the buffer gets full and continuing until 
 *  the character is completely written */
void BitOutputStream::writeByte(char c) {
	unsigned char toBuf = 0;
	for(int i = 0; i < BYTE_SIZE; i++) {
		//read MSB of c and place into toBuf
		toBuf = c & 0x80;
		//shift toBuf by 7 to the right
		toBuf = toBuf >> 7;
		// shift c left by 1
		c = c << 1;
		// place MSB of c (in toBuf) into LSB of buf
		buf = buf | toBuf;
		// Increment nbits index
		nbits += 1;
		if(nbits == BYTE_SIZE) {
			flush();
		}
		// shift buf left by 1
		buf = buf << 1;
	}
}
/** Write the input int for header
 *  can only read an int up to 256
 */
void BitOutputStream::writeInt(int i) {
	//check if input is greater than 255, 0s to indicate
	if(i >= 256) {
		buf = 0;
		flush();
	}
	unsigned char toBuf = 0;
	unsigned char c = (unsigned char)i;
	for(int j = 0; j < BYTE_SIZE; j++) {
		//read MSB of c and place into buf
		toBuf = c & 0x80;
		//shift toBuf right by 7
		toBuf = toBuf >> 7;
		// shift c left by 1
		c = c << 1;
		// place MSB of char into LSB of buf
		buf = buf | toBuf;
		// Increment nbits index
		nbits += 1;
		if(nbits == BYTE_SIZE) {
			flush();
		}
		// shift buf left by 1
		buf = buf << 1;
	}

}

// Fill buffer with ending zeros to finish writing stream
void BitOutputStream::fill() {
	//only run if there are any lingering bits in the buffer
	if(nbits != 0) {
		// shift buf to have ending zeros equal to nbits + 1
		// this ends the byte with zeros in order to be filled
		 buf = buf << (BYTE_SIZE - (nbits + 1));
		// flush to stream
		flush();
	}
}
