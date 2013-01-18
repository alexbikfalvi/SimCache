#include "stdafx.h"
#include "EventStream.h"


CEventStream::CEventStream(
	CNode*			node,
	unsigned int	file,
	unsigned int	segment
	)
{
	assert(node);

	this->node = node;
	this->file = file;
	this->segment = segment;
}

void CEventStream::Process()
{
	this->node->EventStream(this->file, this->segment);
}
