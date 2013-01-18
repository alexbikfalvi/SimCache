#include "stdafx.h"
#include "ModelWorkloadGen.h"

#pragma warning(disable : 4996)

CModelWorkloadGen::CModelWorkloadGen(
	char*			fileEvents,
	char*			filePeers,
	char*			fileFiles,
	unsigned int	segmentSize
	)
{
	assert(fileEvents);
	assert(filePeers);
	assert(fileFiles);
	assert(segmentSize);

	FILE* in;

	// Read events file
	printf("\n\t- read events: %s", fileEvents);

	if(NULL == (in = fopen(fileEvents, "rb")))
	{
		printf("Error workload : cannot read file %s", fileEvents);
		throw 1;
	}

	fread(&this->numEvents, sizeof(this->numEvents), 1, in);
	fread(&this->numSessions, sizeof(this->numSessions), 1, in);
	fread(&this->numPeers, sizeof(this->numPeers), 1, in);

	this->events = new SEvent[this->numEvents];
	assert(this->events);

	fread(this->events, sizeof(SEvent), this->numEvents, in);

	fclose(in);

	// Read peers file
	printf("\n\t- read peers: %s", filePeers);

	if(NULL == (in = fopen(filePeers, "rb")))
	{
		printf("Error workload : cannot read file %s", filePeers);
		throw 1;
	}

	unsigned int numPeers;
	fread(&numPeers, sizeof(numPeers), 1, in);
	assert(this->numPeers == numPeers);

	this->peers = new SPeer[this->numPeers];
	assert(this->peers);

	fread(this->peers, sizeof(SPeer), this->numPeers, in);

	fclose(in);

	// Read files file
	printf("\n\t- read files: %s", filePeers);

	if(NULL == (in = fopen(fileFiles, "rb")))
	{
		printf("Error workload : cannot read file %s", fileFiles);
		throw 1;
	}

	fread(&this->numFiles, sizeof(this->numFiles), 1, in);

	this->files = new SFile[this->numFiles];
	assert(this->files);

	if(this->numFiles != fread(this->files, sizeof(SFile), this->numFiles, in))
	{
		printf("Error workload : cannot read file %s", fileFiles);
		throw 1;
	}

	fclose(in);

	// Pre-calculate file sizes in bits and segments
	this->segmentSize = segmentSize;

	this->fileSizeBits = new __bits[this->numFiles];
	assert(this->fileSizeBits);

	this->fileSizeSegments = new unsigned int[this->numFiles];
	assert(this->fileSizeSegments);

	FILE* f = fopen("files.txt", "w");

	for(unsigned int index = 0; index < this->numFiles; index++)
	{
		this->fileSizeBits[index] = (__bits)(this->files[index].duration * this->files[index].bitrate);
		this->fileSizeSegments[index] = (unsigned int)ceil((double)this->fileSizeBits[index]/this->segmentSize);
		fprintf(f, "%u :\t%lf s @\t%u bps\t:\t%I64u b\t%u seg\n",
			index,
			this->files[index].duration,
			this->files[index].bitrate,
			this->fileSizeBits[index],
			this->fileSizeSegments[index]
			);
	}

	printf("\n...");

	fclose(f);
}

CModelWorkloadGen::~CModelWorkloadGen()
{
	delete[] this->files;
	delete[] this->events;
	delete[] this->peers;

	delete[] this->fileSizeBits;
	delete[] this->fileSizeSegments;
}
