#include "stdafx.h"
#include "Bitmap.h"

CBitmap::CBitmap(
	unsigned int	size
	)
{
	assert(size);

	this->size = size;
	this->bsize = (this->size >> 5) + ((this->size & 0x1F)?1:0);

	// Allocate request
	this->request = new unsigned int[this->bsize];
	assert(this->request);

	// Allocate response
	this->response = new unsigned int[this->bsize];
	assert(this->response);
	this->response = NULL;

	for(unsigned int idx = 0; idx < this->bsize; idx++)
		this->request[idx] = 0;
	for(unsigned int idx = 0; idx < this->bsize; idx++)
		this->response[idx] = 0;
}

CBitmap::CBitmap(
	unsigned int	size,
	unsigned int	first,
	unsigned int	last
	)
{
	assert(size);
	assert(first < size);
	assert(last <= size);

	this->size = size;
	this->bsize = (this->size >> 5) + ((this->size & 0x1F)?1:0);

	// Allocate request
	this->request = new unsigned int[this->bsize];
	assert(this->request);

	// Allocate response
	this->response = new unsigned int[this->bsize];
	assert(this->response);

	for(unsigned int idx = 0; idx < this->bsize; idx++)
		this->request[idx] = 0;
	for(unsigned int idx = 0; idx < this->bsize; idx++)
		this->response[idx] = 0;

	for(unsigned int idx = first; idx < last; idx++)
		this->request[idx >> 5] = this->request[idx >> 5] | (((unsigned int)1) << (idx & 0x1F));
}

CBitmap::~CBitmap()
{
	delete[] this->request;
	delete[] this->response;
}

bool CBitmap::HasReq(unsigned int index)
{
	assert(index < this->size);
	return (this->request[index >> 5] >> (index & 0x1F)) & 1;
}

void CBitmap::StoreReq(unsigned int index)
{
	assert(index < this->size);
	assert(((this->request[index >> 5] >> (index & 0x1F)) & 1) == 0);	// Check the bit is not already set

	// Store the bit	
	this->request[index >> 5] = this->request[index >> 5] | (((unsigned int)1) << (index & 0x1F));
}

void CBitmap::EraseReq(unsigned int index)
{
	assert(index < this->size);
	assert((this->request[index >> 5] >> (index & 0x1F)) & 1);	// Check the bit is already set

	// Erase the bit
	this->request[index >> 5] = this->request[index >> 5] & (~(((unsigned int)1) << (index & 0x1F)));
}

bool CBitmap::HasConf(unsigned int index)
{
	assert(index < this->size);
	return ((this->request[index >> 5] >> (index & 0x1F)) & 1) & ((this->response[index >> 5] >> (index & 0x1F)) & 1);
}

void CBitmap::StoreConf(unsigned int index)
{
	assert(index < this->size);

	// Store the bit	
	this->response[index >> 5] = this->response[index >> 5] | (((unsigned int) (this->request[index >> 5] >> (index & 0x1F)) & 1 ) << (index & 0x1F));
}

void CBitmap::SetConf(CBitmap* bitmap)
{
	assert(bitmap);
	assert(this->size == bitmap->size);

	// Set the confirmation map with AND
	for(unsigned int index = 0; index < this->bsize; index++)
		this->response[index] = this->request[index] & bitmap->request[index];
}

void CBitmap::SetZeroConf()
{
	// Set the confirmation map with AND
	for(unsigned int index = 0; index < this->bsize; index++)
		this->response[index] = 0;
}

void CBitmap::Debug()
{
	printf("\nCache buffer %p (%u / %u):\n", this, this->bsize, this->size);

	for(unsigned int idx = 0; idx < this->bsize; idx++)
	{
		if(idx % 4 == 0) printf("\n");
		for(unsigned char bit = 0; bit < 32; bit++) printf("%u", (this->request[idx] >> bit) & 1);
		printf(" ");
	}
}
