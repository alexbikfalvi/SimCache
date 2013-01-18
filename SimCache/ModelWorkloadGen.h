#ifndef __ModelWorkloadGen_h__
#define __ModelWorkloadGen_h__

#include "ModelWorkload.h"

class CModelWorkloadGen : public CModelWorkload
{
private:
	unsigned int	numEvents;
	unsigned int	numSessions;
	unsigned int	numFiles;
	unsigned int	numPeers;
	
	SEvent*			events;
	SPeer*			peers;
	SFile*			files;

	unsigned int	segmentSize;

	__bits*			fileSizeBits;
	unsigned int*	fileSizeSegments;

public:
	CModelWorkloadGen(
		char*			fileEvents,
		char*			filePeers,
		char*			fileFiles,
		unsigned int	segmentSize
		);
	virtual ~CModelWorkloadGen();

	virtual inline unsigned int		NumPeers() { return this->numPeers; }
	virtual inline unsigned int		NumEvents() { return this->numEvents; }
	virtual inline unsigned int		NumSessions() { return this->numSessions; }
	virtual inline unsigned int		NumFiles() { return this->numFiles; }

	virtual inline SEvent			Event(unsigned int index) { assert(index < this->numEvents); return this->events[index]; }
	virtual inline SPeer			Peer(unsigned int index) { assert(index < this->numPeers); return this->peers[index]; }
	virtual inline SFile			File(unsigned int index) { assert(index < this->numFiles);  return this->files[index]; }

	virtual inline unsigned int		SegmentSize() { return this->segmentSize; }	
	virtual inline __bits			FileSizeBits(unsigned int index) { assert(index < this->numFiles); return this->fileSizeBits[index]; }
	virtual inline unsigned int		FileSizeSegments(unsigned int index) { assert(index < this->numFiles); return this->fileSizeSegments[index]; }
	virtual inline __time			SegmentDuration(unsigned int index) { assert(index < this->numFiles); return (__time)ceil((double) TIME_UNIT*this->segmentSize / this->files[index].bitrate); }
	virtual inline unsigned int		Segment(unsigned int index, double time) { 	return (unsigned int)((__bits)(time * this->files[index].bitrate)/this->segmentSize); }
};

#endif