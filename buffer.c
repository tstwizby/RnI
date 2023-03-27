//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//Defines the 'Buffer' struct and associated functions
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------

typedef struct Buffer{
	int len;
	int pos;
	uint8_t* data;
} Buffer;

//Returns a pointer to the buffer's data at its current position
uint8_t* readBuffer(Buffer* buffer){
	return (buffer->data)+(buffer->pos);
}

//Increments the buffer's position and returns the data at the new position
uint8_t tickBuffer(Buffer* buffer){
	return buffer->data[++(buffer->pos)];
}

//Returns the number of bytes of data left in the buffer
int countBuffer(Buffer* buffer){
	return (buffer->len)-(buffer->pos)-1;
}

//Resets the length/position of the buffer
int clearBuffer(Buffer* buffer){
	buffer->len=0;
	buffer->pos=-1;
}

//Initializes the length/position and data storage for the buffer
int buildBuffer(Buffer* buffer, int len, uint8_t* source){
	buffer->len=len;
	buffer->pos=-1;
	buffer->data=source;
}