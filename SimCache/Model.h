#ifndef __Model_h__
#define __Model_h__

#include "Info.h"
#include "SimModel.h"
#include "Callback.h"
#include "Boot.h"
#include "Server.h"
#include "Network.h"
#include "Data.h"

class CModel : public CSimModel, CCallback
{
private:
	CInfo*				info;
	CNode**				peers;
	CBoot*				boot;
	CServer*			server;
	CNetwork*			network;
	CData*				data;

public:
	CModel(
		CModelNet*		modelNet,
		CModelWorkload*	modelWorkload,
		CModelCache*	modelCache,
		CData*			data,
		__time			startTime,
		__time			endTime,
		unsigned int	numSenders,
		unsigned int	cacheSize,
		double			indicationThreshold,
		double			pvcShortSmoothFactor,
		double			pvcLongSmoothFactor,
		double			pvcTrendThreshold
		);
	virtual ~CModel();

	// SimModel
	virtual __time			MaxTime();
	virtual unsigned int	Events();
	virtual CSimEvent*		Event(unsigned int index, __time& time);
	virtual void			Run(__time time, CSimEvent* evt);
	virtual void			SetHandler(CSimHandler* handler);

	// Callback
	virtual void			Workload(unsigned int index);
};

#endif