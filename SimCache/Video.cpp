#include "stdafx.h"
#include "Video.h"

CVideo::CVideo(
	unsigned int	id,
	__time			duration,
	unsigned int	segSize,
	__bitrate		rate,
	unsigned int	popularity
	)
{
	assert(duration);
	assert(segSize);
	assert(rate);
	
	this->id = id;
	this->duration = duration;
	this->segSize = segSize;
	this->rate = rate;
	this->size = (unsigned int)ceil(((double)this->duration * this->rate) / ((double)this->segSize * TIME_UNIT));
	this->segDuration = TIME_UNIT * this->segSize / this->rate;
	this->popularity = popularity;
}

CVideo::~CVideo()
{
}
