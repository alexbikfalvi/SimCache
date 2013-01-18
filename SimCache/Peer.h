#ifndef __Peer_h__
#define __Peer_h__

#include "Node.h"
#include "Info.h"
#include "Boot.h"
#include "Server.h"
#include "Data.h"
#include "Callback.h"
#include "Videomap.h"

struct SSenderInfo
{
	CBitmap*		bitmap;
	bool			relay;
	CIndication		indication;
};

class CPeer : public CNode
{
public:
	enum EState
	{
		STATE_IDLE		= 0,
		STATE_BOOT		= 1,
		STATE_BITMAP	= 2,
		STATE_SEGMENT	= 3,
		STATE_STREAM	= 4
	};

private:
	// Global
	CInfo*				info;
	CData*				data;
	CCallback*			callback;
	CBoot*				boot;
	CServer*			server;

	// State
	EState				state;

	// Properties
	unsigned int		id;

	// Session variables
	unsigned int		video;
	double				start;
	double				prefix;
	unsigned int		firstSegment;
	unsigned int		lastSegment;
//	unsigned int		numSegments;

	// Request variables
	unsigned int		numRequests;
	unsigned int		numResponses;

	// Streaming variables
	unsigned int		pointer;

	// Segment variables
	CNode**				senders;
	SSenderInfo*		sendersInfo;
	int*				sendersId;

	unsigned int		numSenders;
	double				numCopies;

	// Stream variables
	CVideomap*			videomap;
	unsigned int		lastSenderIndex;

	// Receiver variables

		// Streaming sources
	unsigned int		segTotal;
	unsigned int		segServer;
	unsigned int		segPeerCache;
	unsigned int		segLocalCache;
		// Hits/misses
	unsigned int		numRejectCache;
	unsigned int		numRejectPeer;

	// Sender variables
	unsigned int		recvRequests;
	unsigned int		recvRequestsSuccess;
	unsigned int		recvRequestsFailPeer;
	unsigned int		recvRequestsFailCache;

	// Bandwidth variables
	__bitrate			uplink;
	__time				lastUplink;
	double				avgUplink;

public:
	CPeer(
		unsigned int	id,
		CInfo*			info,
		CCallback*		callback,
		CBoot*			boot,
		CServer*		server,
		CData*			data
		);
	virtual ~CPeer();

	virtual inline unsigned int	Id() { return this->id; }
	virtual inline __bitrate	Bandwidth() { throw 0; return 0; }

	virtual void				Arrive();
	virtual void				Leave();
	virtual void				Session(unsigned int file, double duration, double start, double prefix);

	virtual void				EventStream(unsigned int pointer);
	virtual void				EventFinish(unsigned int file);

	virtual	void				EventBootRequest(CNode* src, unsigned int video, unsigned int* numSenders, CNode** senders, double* numCopies) { throw "Not supported"; }
	virtual void				EventBootResponse(CNode* src, unsigned int video, unsigned int* numSenders, CNode** senders, double* numCopies);

	virtual void				EventSegmentRequest(CNode* src, unsigned int video, CBitmap* bitmap, unsigned int sender);
	virtual void				EventSegmentResponse(CNode* src, unsigned int video, CBitmap* bitmap, unsigned int sender, CIndication indication, bool relaying);

	virtual void				EventStreamRequest(CNode* src, unsigned int video, unsigned int segment);
	virtual void				EventStreamResponse(CNode* src, unsigned int video, unsigned int segment, EStreamResponseResult result);

protected:
	void						Stream();
	void						StreamRequest();

	void						Finish();
};

#endif