#ifndef __Server_h__
#define __Server_h__

#include "Node.h"
#include "Info.h"
#include "Data.h"
#include "ModelNetKing.h"

class CServer : public CNode
{
private:
	CInfo*		info;
	CData*		data;

public:
	CServer(
		CInfo*		info,
		CData*		data
		);
	virtual ~CServer() {}

	virtual inline unsigned int	Id() { return INDEX_SERVER; }
	virtual inline __bitrate	Bandwidth() { return 0xFFFFFFFFFFFFFFFFL; }	// 10 Gbps

	virtual void				EventStreamRequest(CNode* src, unsigned int video, unsigned int segment);
};

#endif