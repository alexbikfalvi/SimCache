#include "stdafx.h"

#pragma warning(disable : 4996)

#include "Sim.h"
#include "Model.h"
#include "ModelWorkloadGen.h"
#include "ModelNetKing.h"
#include "Data.h"

#include "ModelCacheNone.h"
#include "ModelCacheLru.h"
#include "ModelCacheLfu.h"
#include "ModelCacheLfu2.h"
#include "ModelCachePvcR.h"
#include "ModelCachePvcP.h"

void Sim(
	unsigned int	run,
	unsigned char	cacheModel,
	char*			fileOut,
	__time			startTime,
	__time			endTime,
	unsigned int	segmentSize,
	unsigned int	numSenders,
	unsigned int	cacheSize,
	double			cacheIndicationThreshold,
	double			pvcShortSmoothFactor,
	double			pvcLongSmoothFactor,
	double			pvcTrendThreshold,
	CModelNet*		modelNet,
	CModelWorkload*	modelWorkload
	)
{
	// Create the data
	CData* data = new CData(
		cacheSize,
		modelWorkload->NumFiles(),
		modelWorkload->NumPeers(),
		run,
		fileOut
		);
	assert(data);

	// Create cache model
	CModelCache* modelCache;
	
	switch(cacheModel)
	{
	case 0:
		printf("\nCache model: None\n");
		modelCache = new CModelCacheNone();
		break;
	case 1:
		printf("\nCache model: Least Recently Used\n");
		modelCache = new CModelCacheLru(
			modelWorkload,
			cacheSize
			);
		break;
	case 2:
		printf("\nCache model: Least Frequently Used (v1)\n");
		modelCache = new CModelCacheLfu(
			modelWorkload,
			cacheSize
			);
		break;
	case 3:
		printf("\nCache model: P2P Video Caching Reactive\n");
		modelCache = new CModelCachePvcR(
			modelWorkload,
			cacheSize,
			pvcShortSmoothFactor,
			pvcLongSmoothFactor,
			pvcTrendThreshold
			);
		break;
	case 4:
		printf("\nCache model: P2P Video Caching Proactive\n");
		modelCache = new CModelCachePvcP(
			modelWorkload,
			cacheSize,
			pvcShortSmoothFactor,
			pvcLongSmoothFactor,
			pvcTrendThreshold
			);
		break;
	case 5:
		printf("\nCache model: Least Frequently Used (v2)\n");
		modelCache = new CModelCacheLfu2(
			modelWorkload,
			cacheSize
			);
		break;
	}
	assert(modelCache);

	// Create simulator model
	CModel* model = new CModel(
		modelNet,
		modelWorkload,
		modelCache,
		data,
		startTime,
		endTime,
		numSenders,
		cacheSize,
		cacheIndicationThreshold,
		pvcShortSmoothFactor,
		pvcLongSmoothFactor,
		pvcTrendThreshold
		);
	assert(model);

	// Create simulator
	CSim* sim = new CSim(model);
	assert(sim);

	// Run simulator
	time_t duration = 0;
	
	unsigned int evt = sim->Run(duration);

	printf("\n\nSimulation finished in %I64u seconds (%u unprocessed events)", duration, evt);

	// Delete
	delete model;
	delete modelCache;
	delete sim;

	delete data;
}

int main(int argc, char* argv[])
{
	printf("\nSimulator: PEER-TO-PEER STREAMING CACHE (version 3.0 / %s)\n", __TIMESTAMP__);

	if(argc != 2)
	{
		printf("\nThe parameters are:\
			   \n\t 1 - Config file\
			   \n\n\
			   \nCache models:\
			   \n\t 1 - Least recently used (LRU)\
			   \n\t 2 - Least frequently used v1 (LFU 1)\
			   \n\t 3 - P2P video caching reactive (PVC R)\
			   \n\t 4 - P2P video caching proactive (PVC P)\
			   \n\t 5 - Least frequently used v2 (LFU 2)\
			   \n");
		return 0;
	}

	char* fileConfig = argv[1];

	printf("\nConfiguration file: %s\n", fileConfig);

	FILE* in = fopen(fileConfig, "r");
	assert(in);

	char fileEvents[1024];
	char filePeers[1024];
	char fileVideos[1024];
	char fileKing[1024];
	char fileOutput[1024];
	unsigned int cacheModel;
	unsigned int startTime;
	unsigned int endTime;
	unsigned int segmentSize;
	unsigned int numSenders;
	unsigned int shuffleDelay;
	unsigned int cacheSizeNum;
	unsigned int* cacheSize;
	double cacheIndicationThreshold;
	double pvcShortSmoothFactor;
	double pvcLongSmoothFactor;
	double pvcTrendThreshold;
	unsigned int runs;

	if(fscanf(in, "fileEvents=%s\n", fileEvents) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "filePeers=%s\n", filePeers) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "fileVideos=%s\n", fileVideos) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "fileKing=%s\n", fileKing) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "fileOutput=%s\n", fileOutput) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "cacheModel=%u\n", &cacheModel) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "startTime=%u\n", &startTime) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "endTime=%u\n", &endTime) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "segmentSize=%u\n", &segmentSize) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "numSenders=%u\n", &numSenders) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "shuffleDelay=%u\n", &shuffleDelay) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "cacheSizeNum=%u\n", &cacheSizeNum) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	
	cacheSize = new unsigned int[cacheSizeNum];
	assert(cacheSize);
	
	fscanf(in, "cacheSize={");
	for(unsigned int index = 0; index < cacheSizeNum; index++)
	{
		if(index) fscanf(in, ",");
		if(fscanf(in, "%u", &cacheSize[index]) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	}
	fscanf(in, "}\n");

	if(fscanf(in, "cacheIndicationThreshold=%lf\n", &cacheIndicationThreshold) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "pvcShortSmoothFactor=%lf\n", &pvcShortSmoothFactor) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "pvcLongSmoothFactor=%lf\n", &pvcLongSmoothFactor) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "pvcTrendThreshold=%lf\n", &pvcTrendThreshold) != 1) { printf("\nError reading configuration file.\n"); return -1; }
	if(fscanf(in, "runs=%u\n", &runs) != 1) { printf("\nError reading configuration file.\n"); return -1; }

	fclose(in);

	if(cacheModel > 5) return -2;

	printf("\nConfiguring the simulator with the parameters:\
		   \n\t 1 - File events                                                 : %s\
		   \n\t 2 - File peers                                                  : %s\
		   \n\t 3 - File videos                                                 : %s\
		   \n\t 4 - File king                                                   : %s\
		   \n\t 5 - File output                                                 : %s\
		   \n\t 6 - Cache model                                                 : %u\
		   \n\t 7 - Start time                                                  : %u s\
		   \n\t 8 - End time                                                    : %u s\
		   \n\t 9 - Video segment size                                          : %u kbits\
		   \n\t10 - Number of senders                                           : %u\
		   \n\t11 - Shuffle peer-delay mapping ( 0 - disable, 1 - enable )      : %u\
		   \n\t12 - Cache sizes                                                 : ",
		   fileEvents,
		   filePeers,
		   fileVideos,
		   fileKing,
		   fileOutput,
		   cacheModel,
		   startTime,
		   endTime,
		   segmentSize,
		   numSenders,
		   shuffleDelay);

	segmentSize *= 1000; // Transform from kbits to bits

	for(unsigned int index = 0; index < cacheSizeNum; index++)
	{
		if(index) printf(", ");
		printf("%u", cacheSize[index]);
	}

	printf(" segments\
		   \n\t13 - Cache indication threshold ([0..1] = [cache .. not cache])  : %lf\
		   \n\t14 - [PVC] Short EWMA smooth factor                              : %lf\
		   \n\t15 - [PVC] Long EWMA smooth factor                               : %lf\
		   \n\t16 - [PVC] Trend threshold                                       : %lf\
		   \n\t17 - Runs:                                                       : %u\
		   \n\n",
		   cacheIndicationThreshold,
		   pvcShortSmoothFactor,
		   pvcLongSmoothFactor,
		   pvcTrendThreshold,
		   runs);


	printf("1. Create network...");
	// Create model net
	CModelNet* modelNet = new CModelNetKing(shuffleDelay != 0, fileKing);
	assert(modelNet);
	printf("done.\n");

	printf("2. Create workload...");
	// Create model workload
	CModelWorkload* modelWorkload = new CModelWorkloadGen(
		fileEvents,
		filePeers,
		fileVideos,
		segmentSize
		);
	assert(modelWorkload);
	printf("done.\n");

	printf("\n");
	for(unsigned int run = 0; run < runs; run++)
	{
		printf("\nRun %u", run);

		for(unsigned int index = 0; index < cacheSizeNum; index++)
		{
			printf("\n\tCache size: %u ", cacheSize[index]);

			Sim(
				run,
				cacheModel,
				fileOutput,
				startTime*TIME_UNIT,
				endTime*TIME_UNIT,
				segmentSize,
				numSenders,
				cacheSize[index],
				cacheIndicationThreshold,
				pvcShortSmoothFactor,
				pvcLongSmoothFactor,
				pvcTrendThreshold,
				modelNet,
				modelWorkload
				);
		}
	}
	delete modelWorkload;
	delete modelNet;

	delete[] cacheSize;

	_CrtDumpMemoryLeaks();

	return 0;
}
