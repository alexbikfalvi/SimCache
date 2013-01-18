#include "stdafx.h"
#include "Shuffle.h"
#include "Rand.h"

CShuffle::CShuffle(
	unsigned int	num
	)
{
	assert(num);

	this->num = num;

	this->mapping = new unsigned int[this->num];
	assert(this->mapping);

	for(unsigned int index = 0; index < this->num; index++)
		this->mapping[index] = index;

	this->Shuffle();
}

CShuffle::~CShuffle()
{
	delete[] this->mapping;
}

void CShuffle::Shuffle()
{
	/*
	 * Portions of this code are based on: http://benpfaff.org/writings/clc/shuffle.html (last updated 04 Apr 2004 16:19)
	 * Copyright © 2004 Ben Pfaff.
	 */

	unsigned int i2;
	unsigned int tmp;
	
	for(unsigned int i1 = 0; i1 < this->num - 1; i1++)
	{
		i2 = i1 + CRand::Generate(this->num - i1);
		assert(i2 < this->num);

		tmp = this->mapping[i2];
		this->mapping[i2] = this->mapping[i1];
		this->mapping[i1] = tmp;
	}
}
