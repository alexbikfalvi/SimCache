#ifndef __ModelNetKing_h__
#define __ModelNetKing_h__

#include "ModelNet.h"

#define NUM_NODES		1740
#define NUM_PEERS		1738
#define INDEX_BOOT		1738
#define INDEX_SERVER	1739

class CModelNetKing : public CModelNet
{
private:
	unsigned int*		nodeMap;
public:
	CModelNetKing(
		bool			rnd,
		char*			kingFile
		);
	virtual ~CModelNetKing();

	virtual __time		Delay(
		__time			time,
		unsigned int	src,
		unsigned int	dst,
		unsigned int	data
		);
	virtual __time		DelayPeerToPeer(
		__time			time,
		unsigned int	src,
		unsigned int	dst,
		unsigned int	data
		);
	virtual __time		DelayPeerToBoot(
		__time			time,
		unsigned int	src,
		unsigned int	data
		);
	virtual __time		DelayBootToPeer(
		__time			time,
		unsigned int	dst,
		unsigned int	data
		);
	virtual __time		DelayPeerToServer(
		__time			time,
		unsigned int	src,
		unsigned int	data
		);
	virtual __time		DelayServerToPeer(
		__time			time,
		unsigned int	dst,
		unsigned int	data
		);
};

#endif