#ifndef __ModelNet_h__
#define __ModelNet_h__

class CModelNet
{
public:
	CModelNet() {}
	virtual ~CModelNet() {}

	virtual __time		Delay(
		__time			time,
		unsigned int	src,
		unsigned int	dst,
		unsigned int	data
		) = 0;
	virtual __time		DelayPeerToPeer(
		__time			time,
		unsigned int	src,
		unsigned int	dst,
		unsigned int	data
		) = 0;
	virtual __time		DelayPeerToBoot(
		__time			time,
		unsigned int	src,
		unsigned int	data
		) = 0;
	virtual __time		DelayBootToPeer(
		__time			time,
		unsigned int	dst,
		unsigned int	data
		) = 0;
	virtual __time		DelayPeerToServer(
		__time			time,
		unsigned int	src,
		unsigned int	data
		) = 0;
	virtual __time		DelayServerToPeer(
		__time			time,
		unsigned int	dst,
		unsigned int	data
		) = 0;
};

#endif