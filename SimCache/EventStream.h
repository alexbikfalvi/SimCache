#ifndef __EventStream_h__
#define __EventStream_h__

#include "SimEvent.h"
#include "Node.h"

class CEventStream : public CSimEvent
{
private:
	CNode*			node;
	unsigned int	file;
	unsigned int	segment;

public:
	CEventStream(
		CNode*			node,
		unsigned int	file,
		unsigned int	segment
		);
	virtual ~CEventStream() {}

	virtual void	Process();
};

#endif