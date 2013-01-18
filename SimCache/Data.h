#ifndef __Data_h__
#define __Data_h__

//#define _META

class CData
{
private:
#ifdef _TRACE
	FILE*			fileTrace;
#endif
#ifdef _VCOP
	FILE*			fileVcop;
#endif
#ifdef _META
	FILE*			fileMetaSegTotal;
	FILE*			fileMetaSegAdd;
	FILE*			fileMetaSegReplace;
	FILE*			fileMetaServedRequests;
	FILE*			fileMetaRecvStreams;
	FILE*			fileMetaMaxUplink;
	FILE*			fileMetaAvgUplink;
#endif
	FILE*			fileOut;

	unsigned int	cacheSize;
	unsigned int	numVideos;
	unsigned int	numPeers;
	unsigned int	run;

	unsigned int	numSamplesReceiver;
	unsigned int	numSamplesSender;
	
	// Quality
	double			sumQuality;
	double			ssqQuality;
	double			avgQuality;
	double			stdQuality;

	// Cache hit ratio
	double			sumSegTotal;
	double			sumSegCache;
	double			cacheHitRatio;

	// Peer rejection ratio
	double			sumPeerRej;
	double			ssqPeerRej;
	double			avgPeerRej;
	double			stdPeerRej;

	// Cache rejection ratio
	double			sumCacheRej;
	double			ssqCacheRej;
	double			avgCacheRej;
	double			stdCacheRej;

	// Cache admission ratio
	double			sumCacheAdm;
	double			ssqCacheAdm;
	double			avgCacheAdm;
	double			stdCacheAdm;

	// Copies
	double			sumCopies;
	double			ssqCopies;
	double			avgCopies;
	double			stdCopies;

	// Number of senders
	double			sumSenders;
	double			ssqSenders;
	double			avgSenders;
	double			stdSenders;

	// Recieved requests
	double			sumRequests;
	double			ssqRequests;
	double			avgRequests;
	double			stdRequests;

	// Received requests success
	double			sumRequestsSuccess;
	double			ssqRequestsSuccess;
	double			avgRequestsSuccess;
	double			stdRequestsSuccess;

	// Received requests fail peer
	double			sumRequestsFailPeer;
	double			ssqRequestsFailPeer;
	double			avgRequestsFailPeer;
	double			stdRequestsFailPeer;

	// Received requests fail cache
	double			sumRequestsFailCache;
	double			ssqRequestsFailCache;
	double			avgRequestsFailCache;
	double			stdRequestsFailCache;

	// Peer uplink
	double			sumUplink;
	double			ssqUplink;
	double			avgUplink;
	double			stdUplink;

#ifdef _VCOP
	// Video number of sopies
	unsigned int*	numVcopies;
	double*			sumVcopies;
	double*			ssqVcopies;
	double*			avgVcopies;
	double*			stdVcopies;
#endif

#ifdef _META
	unsigned __int64*	metaSegTotal;
	unsigned __int64*	metaSegAdd;
	unsigned __int64*	metaSegReplace;
	unsigned __int64*	metaServedRequests;
	unsigned int*		metaRecvStreams;
	__bitrate*			metaMaxUplink;
	double*				metaAvgUplink;
#endif

public:
	CData(
		unsigned int	cacheSize,
		unsigned int	numVideos,
		unsigned int	numPeers,
		unsigned int	run,
#ifdef _TRACE
		char*			filenameTrace,
#endif
		char*			filenameOut
		);
	~CData();

	void		RecordReceiver(
		__time			time,
		unsigned int	peer,
		unsigned int	video,
		unsigned int	size,
		unsigned int	popularity,
		unsigned int	start,
		unsigned int	duration,
		unsigned int	segTotal,
		unsigned int	segServer,
		unsigned int	segLocalCache,
		unsigned int	segPeerCache,
		double			quality,
		unsigned int	numRejectCache,
		unsigned int	numRejectPeer,
		bool			statIndication,
		unsigned int	currentCacheSize,
		unsigned int	maxCacheSize,
		double			copies,
		unsigned int	numSenders,
		unsigned int	maxSenders,
		int*			senders
		);
	void		RecordSender(
		unsigned int	peer,
		unsigned int	recvRequests,
		unsigned int	recvRequestsSuccess,
		unsigned int	recvRequestsFailPeer,
		unsigned int	recvRequestsFailCache,
		double			uplink
		);
#ifdef _META
	void		RecordMetadata(
		unsigned int		peer,
		unsigned __int64	metaSegTotal,
		unsigned __int64	metaSegAdd,
		unsigned __int64	metaSegReplace,
		unsigned __int64	metaServedRequests,
		unsigned int		metaRecvStreams,
		__bitrate			metaMaxUplink,
		double				metaAvgUplink
		);
#endif
};

#endif