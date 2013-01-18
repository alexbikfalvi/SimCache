#ifndef __ModelWorkload_h__
#define __ModelWorkload_h__

enum EEvent
{
	ARRIVE = 0,
	LEAVE = 1,
	SESSION = 2
};

enum EClass
{
	CLASS_DEFAULT = 0,
	CLASS_REGULAR  = 1,
	CLASS_NEWS = 2
};

struct SEvent
{
	double			time;
	unsigned int	peer;
	EEvent			op;
	unsigned int	file;
	double			duration;
	double			start;
	double			prefix;
};

struct SFile
{
	unsigned int	file;
	double			duration;
	unsigned int	freq;
	double			arrival;
	EClass			lifespan;
	unsigned int	bitrate;
};

struct SPeer
{
	unsigned int	bandwidth;
};

class CModelWorkload
{
public:
	CModelWorkload() {}
	virtual ~CModelWorkload() {}

	virtual unsigned int	NumPeers() = 0;
	virtual unsigned int	NumEvents() = 0;
	virtual unsigned int	NumSessions() = 0;
	virtual unsigned int	NumFiles() = 0;

	virtual SFile			File(unsigned int index) = 0;
	virtual SEvent			Event(unsigned int index) = 0;
	virtual SPeer			Peer(unsigned int index) = 0;

	virtual unsigned int	SegmentSize() = 0;
	virtual __bits			FileSizeBits(unsigned int index) = 0;
	virtual unsigned int	FileSizeSegments(unsigned int index) = 0;
	virtual __time			SegmentDuration(unsigned int index) = 0;
	virtual unsigned int	Segment(unsigned int index, double time) = 0;
};

#endif