#ifndef __Boot_h__
#define __Boot_h__

#include "Node.h"
#include "Info.h"
#include "ModelNetKing.h"

class CBoot : public CNode
{
private:
	CInfo*		info;
	CNode**		peers;

public:
	CBoot(
		CInfo*		info,
		CNode**		peers
		);
	virtual ~CBoot();

	virtual inline unsigned int	Id() { return INDEX_BOOT; }				// Bootstrap server ID
	virtual inline __bitrate	Bandwidth() { return 0xFFFFFFFFFFFFFFFFL; }

	inline CNode*				Peer(unsigned int id) { return this->peers[id]; }

	virtual void				EventBootRequest(CNode* src, unsigned int video, unsigned int* numSenders, CNode** senders, double* numCopies);
};

#endif