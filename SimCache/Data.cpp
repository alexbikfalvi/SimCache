#include "stdafx.h"
#include "Data.h"

CData::CData(
	unsigned int	cacheSize,
	unsigned int	numVideos,
	unsigned int	numPeers,
	unsigned int	run,
#ifdef _TRACE
	char*			filenameTrace,
#endif
	char*			filenameOut
	)
{
#ifdef _TRACE
	assert(filenameTrace);
#endif
	assert(filenameOut);

	this->cacheSize = cacheSize;
	this->numVideos = numVideos;
	this->numPeers = numPeers;
	this->run = run;

#ifdef _TRACE
	if(fopen_s(&this->fileTrace, filenameTrace, "w"))
	{
		printf("\nCannot write to file %s. Execution aborted.", filenameTrace);
		throw "I/O error";
	}
#endif

	if(fopen_s(&this->fileOut, filenameOut, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenameOut);
		throw "I/O error";
	}

#ifdef _VCOP
	char filenameVcop[256];
	sprintf_s(filenameVcop, 256, "vcop_%s", filenameOut);
	if(fopen_s(&this->fileVcop, filenameVcop, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenameVcop);
		throw "I/O error";
	}
#endif
#ifdef _META
	char filenamePeer[256];
	
	sprintf_s(filenamePeer, 256, "meta_segt_%s", filenameOut);
	if(fopen_s(&this->fileMetaSegTotal, filenamePeer, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenamePeer);
		throw "I/O error";
	}
	
	sprintf_s(filenamePeer, 256, "meta_sega_%s", filenameOut);
	if(fopen_s(&this->fileMetaSegAdd, filenamePeer, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenamePeer);
		throw "I/O error";
	}
	
	sprintf_s(filenamePeer, 256, "meta_segr_%s", filenameOut);
	if(fopen_s(&this->fileMetaSegReplace, filenamePeer, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenamePeer);
		throw "I/O error";
	}
	
	sprintf_s(filenamePeer, 256, "meta_req_%s", filenameOut);
	if(fopen_s(&this->fileMetaServedRequests, filenamePeer, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenamePeer);
		throw "I/O error";
	}
	
	sprintf_s(filenamePeer, 256, "meta_str_%s", filenameOut);
	if(fopen_s(&this->fileMetaRecvStreams, filenamePeer, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenamePeer);
		throw "I/O error";
	}
	
	sprintf_s(filenamePeer, 256, "meta_mup_%s", filenameOut);
	if(fopen_s(&this->fileMetaMaxUplink, filenamePeer, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenamePeer);
		throw "I/O error";
	}

	sprintf_s(filenamePeer, 256, "meta_aup_%s", filenameOut);
	if(fopen_s(&this->fileMetaAvgUplink, filenamePeer, "a"))
	{
		printf("\nCannot append to file %s. Execution aborted.", filenamePeer);
		throw "I/O error";
	}
#endif

	this->numSamplesReceiver = 0;
	this->numSamplesSender = 0;

	// Quality
	this->sumQuality = 0;
	this->ssqQuality = 0;

	// Cache hit ratio
	this->sumSegTotal = 0;
	this->sumSegCache = 0;

	// Peer rejection ratio
	this->sumPeerRej = 0;
	this->ssqPeerRej = 0;

	// Cache rejection ratio
	this->sumCacheRej = 0;
	this->ssqCacheRej = 0;

	// Cache admission ratio
	this->sumCacheAdm = 0;
	this->ssqCacheAdm = 0;

	// Copies
	this->sumCopies = 0;
	this->ssqCopies = 0;

	// Number of senders
	this->sumSenders = 0;
	this->ssqSenders = 0;

	// Received requests
	this->sumRequests = 0;
	this->ssqRequests = 0;

	// Received requests success
	this->sumRequestsSuccess = 0;
	this->ssqRequestsSuccess = 0;

	// Received requests fail peer
	this->sumRequestsFailPeer = 0;
	this->ssqRequestsFailPeer = 0;

	// Received requests fail cache
	this->sumRequestsFailCache = 0;
	this->ssqRequestsFailCache = 0;

	// Peer uplink
	this->sumUplink = 0;
	this->ssqUplink = 0;

#ifdef _VCOP
	// Video copies
	this->numVcopies = new unsigned int[this->numVideos];
	assert(this->numVcopies);
	this->sumVcopies = new double[this->numVideos];
	assert(this->sumVcopies);
	this->ssqVcopies = new double[this->numVideos];
	assert(this->ssqVcopies);
	this->avgVcopies = new double[this->numVideos];
	assert(this->avgVcopies);
	this->stdVcopies = new double[this->numVideos];
	assert(this->stdVcopies);

	for(unsigned int video = 0; video < this->numVideos; video++)
	{
		this->numVcopies[video] = 0;
		this->sumVcopies[video] = 0;
		this->ssqVcopies[video] = 0;
	}
#endif

#ifdef _META
	this->metaSegTotal = new unsigned __int64[this->numPeers];
	this->metaSegAdd = new unsigned __int64[this->numPeers];
	this->metaSegReplace = new unsigned __int64[this->numPeers];
	this->metaServedRequests = new unsigned __int64[this->numPeers];
	this->metaRecvStreams = new unsigned int[this->numPeers];
	this->metaMaxUplink = new __bitrate[this->numPeers];
	this->metaAvgUplink = new double[this->numPeers];

	for(unsigned int index = 0; index < this->numPeers; index++)
	{
		this->metaSegTotal[index] = 0;
		this->metaSegAdd[index] = 0;
		this->metaSegReplace[index] = 0;
		this->metaServedRequests[index] = 0;
		this->metaRecvStreams[index] = 0;
		this->metaMaxUplink[index] = 0;
		this->metaAvgUplink[index] = 0;
	}
#endif
}

CData::~CData()
{
	// Global processing
		// Quality
	this->avgQuality = this->sumQuality / this->numSamplesReceiver;
	this->stdQuality = sqrt((this->ssqQuality / this->numSamplesReceiver) - this->avgQuality * this->avgQuality);
		// Cache hit ratio
	this->cacheHitRatio = this->sumSegCache / this->sumSegTotal;
		// Peer rejection ratio
	this->avgPeerRej = this->sumPeerRej / this->numSamplesReceiver;
	this->stdPeerRej = sqrt((this->ssqPeerRej / this->numSamplesReceiver) - this->avgPeerRej * this->avgPeerRej);
		// Cache rejection ratio
	this->avgCacheRej = this->sumCacheRej / this->numSamplesReceiver;
	this->stdCacheRej = sqrt((this->ssqCacheRej / this->numSamplesReceiver) - this->avgCacheRej * this->avgCacheRej);
		// Cache admission ratio
	this->avgCacheAdm = this->sumCacheAdm / this->numSamplesReceiver;
	this->stdCacheAdm = sqrt((this->ssqCacheAdm / this->numSamplesReceiver) - this->avgCacheAdm * this->avgCacheAdm);
		// Copies
	this->avgCopies = this->sumCopies / this->numSamplesReceiver;
	this->stdCopies = sqrt((this->ssqCopies / this->numSamplesReceiver) - this->avgCopies * this->avgCopies);
		// Number of senders
	this->avgSenders = this->sumSenders / this->numSamplesReceiver;
	this->stdSenders = sqrt((this->ssqSenders / this->numSamplesReceiver) - this->avgSenders * this->avgSenders);
		// Received requests
	this->avgRequests = this->sumRequests / this->numSamplesSender;
	this->stdRequests = sqrt((this->ssqRequests / this->numSamplesSender) - this->avgRequests * this->avgRequests);
		// Received requests success
	this->avgRequestsSuccess = this->sumRequestsSuccess / this->numSamplesSender;
	this->stdRequestsSuccess = sqrt((this->ssqRequestsSuccess / this->numSamplesSender) - this->avgRequestsSuccess * this->avgRequestsSuccess);
		// Received requests fail peer
	this->avgRequestsFailPeer = this->sumRequestsFailPeer / this->numSamplesSender;
	this->stdRequestsFailPeer = sqrt((this->ssqRequestsFailPeer / this->numSamplesSender) - this->avgRequestsFailPeer * this->avgRequestsFailPeer);
		// Received requests fail cache
	this->avgRequestsFailCache = this->sumRequestsFailCache / this->numSamplesSender;
	this->stdRequestsFailCache = sqrt((this->ssqRequestsFailCache / this->numSamplesSender) - this->avgRequestsFailCache * this->avgRequestsFailCache);
		// Peer uplink
	this->avgUplink = this->sumUplink / this->numSamplesSender;
	this->stdUplink = sqrt((this->ssqUplink / this->numSamplesSender) - this->avgUplink * this->avgUplink);

	printf("\
		   \n\tNumber of videos                         : %u\
		   \n\tCache size                               : %u\
		   \n\tRun                                      : %u\
		   \n\tNumber of samples                        : %u\
		   \n\tQuality (average)                        : %lf\
		   \n\tQuality (standard deviation)             : %lf\
		   \n\tCache hit ratio                          : %lf\
		   \n\tPeer rejection (average)                 : %lf\
		   \n\tPeer rejection (standard deviation)      : %lf\
		   \n\tCache rejection (average)                : %lf\
		   \n\tCache rejection (standard deviation)     : %lf\
		   \n\tCache admission (average)                : %lf\
		   \n\tCache admission (standard deviation)     : %lf\
		   \n\tNumber of copies (average)               : %lf\
		   \n\tNumber of copies (standard deviation)    : %lf\
		   \n\tNumber of senders (average)              : %lf\
		   \n\tNumber of senders (standard deviation)   : %lf\
		   \n\tRequests (average)                       : %lf\
		   \n\tRequests (standard deviation)            : %lf\
		   \n\tRequests success (average)               : %lf\
		   \n\tRequests success (standard deviation)    : %lf\
		   \n\tRequests fail peer (average)             : %lf\
		   \n\tRequests fail peer (standard deviation)  : %lf\
		   \n\tRequests fail cache (average)            : %lf\
		   \n\tRequests fail cache (standard deviation) : %lf\
		   \n\tUplink (average)                         : %lf\
		   \n\tUplink (standard deviation)              : %lf\
		   \n",
		this->numVideos,
		this->cacheSize,
		this->run,
		this->numSamplesReceiver,
		this->avgQuality,
		this->stdQuality,
		this->cacheHitRatio,
		this->avgPeerRej,
		this->stdPeerRej,
		this->avgCacheRej,
		this->stdCacheRej,
		this->avgCacheAdm,
		this->stdCacheAdm,
		this->avgCopies,
		this->stdCopies,
		this->avgSenders,
		this->stdSenders,
		this->avgRequests,
		this->stdRequests,
		this->avgRequestsSuccess,
		this->stdRequestsSuccess,
		this->avgRequestsFailPeer,
		this->stdRequestsFailPeer,
		this->avgRequestsFailCache,
		this->stdRequestsFailCache,
		this->avgUplink,
		this->stdUplink
		);

	fprintf(this->fileOut, "%u,%u,%u,%u,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf,%.010lf\n",
		this->numVideos,
		this->cacheSize,
		this->run,
		this->numSamplesReceiver,
		this->avgQuality,
		this->stdQuality,
		this->cacheHitRatio,
		this->avgPeerRej,
		this->stdPeerRej,
		this->avgCacheRej,
		this->stdCacheRej,
		this->avgCacheAdm,
		this->stdCacheAdm,
		this->avgCopies,
		this->stdCopies,
		this->avgSenders,
		this->stdSenders,
		this->avgRequests,
		this->stdRequests,
		this->avgRequestsSuccess,
		this->stdRequestsSuccess,
		this->avgRequestsFailPeer,
		this->stdRequestsFailPeer,
		this->avgRequestsFailCache,
		this->stdRequestsFailCache,
		this->avgUplink,
		this->stdUplink
		);

#ifdef _VCOP
	// Video copies
	fprintf(this->fileVcop, "%u", this->numVideos);
	for(unsigned int video = 0; video < this->numVideos; video++)
	{
		if(this->numVcopies[video])
		{
			this->avgVcopies[video] = this->sumVcopies[video] / this->numVcopies[video];
			this->stdVcopies[video] = sqrt((this->ssqVcopies[video]/this->numVcopies[video]) - this->avgVcopies[video]*this->avgVcopies[video]);
		}
		else
		{
			this->avgVcopies[video] = 0;
			this->stdVcopies[video] = 0;
		}
		fprintf(this->fileVcop, ",%.010lf", this->avgVcopies[video]);
	}
	fprintf(this->fileVcop, "\n");
#endif

#ifdef _TRACE
	fclose(this->fileTrace);
#endif

	fclose(this->fileOut);

#ifdef _VCOP
	fclose(this->fileVcop);

	delete[] this->numVcopies;
	delete[] this->sumVcopies;
	delete[] this->ssqVcopies;
	delete[] this->avgVcopies;
	delete[] this->stdVcopies;
#endif

#ifdef _META

	// Save peer metadata

	for(unsigned int index = 0; index < this->numPeers; index++)
	{
		fprintf(this->fileMetaSegTotal, "%I64u,", this->metaSegTotal[index]);
		fprintf(this->fileMetaSegAdd, "%I64u,", this->metaSegAdd[index]);
		fprintf(this->fileMetaSegReplace, "%I64u,", this->metaSegReplace[index]);
		fprintf(this->fileMetaServedRequests, "%I64u,", this->metaServedRequests[index]);
		fprintf(this->fileMetaRecvStreams, "%u,", this->metaRecvStreams[index]);
		fprintf(this->fileMetaMaxUplink, "%I64u,", this->metaMaxUplink[index]);
		fprintf(this->fileMetaAvgUplink, "%.010lf,", this->metaAvgUplink[index]);
	}

	fprintf(this->fileMetaSegTotal, "\n");
	fprintf(this->fileMetaSegAdd, "\n");
	fprintf(this->fileMetaSegReplace, "\n");
	fprintf(this->fileMetaServedRequests, "\n");
	fprintf(this->fileMetaRecvStreams, "\n");
	fprintf(this->fileMetaMaxUplink, "\n");
	fprintf(this->fileMetaAvgUplink, "\n");

	fclose(this->fileMetaSegTotal);
	fclose(this->fileMetaSegAdd);
	fclose(this->fileMetaSegReplace);
	fclose(this->fileMetaServedRequests);
	fclose(this->fileMetaRecvStreams);
	fclose(this->fileMetaMaxUplink);
	fclose(this->fileMetaAvgUplink);

	delete[] this->metaSegTotal;
	delete[] this->metaSegAdd;
	delete[] this->metaSegReplace;
	delete[] this->metaServedRequests;
	delete[] this->metaRecvStreams;
	delete[] this->metaMaxUplink;
	delete[] this->metaAvgUplink;
#endif
}


void CData::RecordReceiver(
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
	)
{
	this->numSamplesReceiver++;

		// Quality
	this->sumQuality += quality;
	this->ssqQuality += quality * quality;
		// Cache hit ratio
	this->sumSegTotal += segTotal;
	this->sumSegCache += segLocalCache + segPeerCache;
		// Peer rejection ratio
	this->sumPeerRej += numRejectPeer / duration;
	this->ssqPeerRej += (numRejectPeer / duration) * (numRejectPeer / duration);
		// Cache rejection ratio
	this->sumCacheRej += numRejectCache / duration;
	this->ssqCacheRej += (numRejectCache / duration) * (numRejectCache / duration);
		// Cache admission ratio
	this->sumCacheAdm += statIndication;
	this->ssqCacheAdm += statIndication;
		// Copies
	this->sumCopies += copies;
	this->ssqCopies += copies * copies;
		// Number of senders
	this->sumSenders += numSenders;
	this->ssqSenders += numSenders * numSenders;

#ifdef _VCOP
	// Number of video copies;
	this->numVcopies[video]++;
	this->sumVcopies[video] += copies;
	this->sumVcopies[video] += copies * copies;
#endif

#ifdef _TRACE
	// Trace file
	fprintf(this->fileTrace, "%I64u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%u,%.010lf,%u,%u,%u,%u,%u,%.010lf,%u,%u",
		time,
		peer,
		video,
		size,
		popularity,
		start,
		duration,
		segTotal,
		segServer,
		segLocalCache,
		segPeerCache,
		quality,
		numRejectCache,
		numRejectPeer,
		statIndication,
		currentCacheSize,
		maxCacheSize,
		copies,
		numSenders,
		maxSenders
		);
	for(unsigned int index = 0; index < maxSenders; index++)
		fprintf(this->fileTrace, ",%d", senders[index]);
	fprintf(this->fileTrace, "\n");
#endif

#ifdef _META
	// Record metadata
		// Cached segments per peer
#endif
}

void CData::RecordSender(
	unsigned int	peer,
	unsigned int	recvRequests,
	unsigned int	recvRequestsSuccess,
	unsigned int	recvRequestsFailPeer,
	unsigned int	recvRequestsFailCache,
	double			uplink
	)
{
	this->numSamplesSender++;

	// Received requests
	this->sumRequests += recvRequests;
	this->ssqRequests += recvRequests * recvRequests;

	// Received requests success
	this->sumRequestsSuccess += recvRequestsSuccess;
	this->ssqRequestsSuccess += recvRequestsSuccess * recvRequestsSuccess;

	// Received requests fail peer
	this->sumRequestsFailPeer += recvRequestsFailPeer;
	this->ssqRequestsFailPeer += recvRequestsFailPeer * recvRequestsFailPeer;

	// Received requests fail cache
	this->sumRequestsFailCache += recvRequestsFailCache;
	this->ssqRequestsFailCache += recvRequestsFailCache * recvRequestsFailCache;

	// Uplink
	this->sumUplink += uplink;
	this->ssqUplink += uplink * uplink;
}

#ifdef _META
void CData::RecordMetadata(
	unsigned int		peer,
	unsigned __int64	metaSegTotal,
	unsigned __int64	metaSegAdd,
	unsigned __int64	metaSegReplace,
	unsigned __int64	metaServedRequests,
	unsigned int		metaRecvStreams,
	__bitrate			metaMaxUplink,
	double				metaAvgUplink
	)
{
	assert(peer < this->numPeers);
	
	this->metaSegTotal[peer] = metaSegTotal;
	this->metaSegAdd[peer] = metaSegAdd;
	this->metaSegReplace[peer] = metaSegReplace;
	this->metaServedRequests[peer] = metaServedRequests;
	this->metaRecvStreams[peer] = metaRecvStreams;
	this->metaMaxUplink[peer] = metaMaxUplink;
	this->metaAvgUplink[peer] = metaAvgUplink;
}
#endif