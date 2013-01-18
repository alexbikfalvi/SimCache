#ifndef __Videomap_h__
#define __Videomap_h__

#include "Bitmap.h"
#include "Node.h"

class CVideomap
{
private:
	unsigned int	begin;
	unsigned int	end;
	unsigned int	length;
	unsigned int	numSenders;

	CNode**			senders;

public:
	CVideomap(
		unsigned int	begin,		// The begin segment is included
		unsigned int	end,		// The end segment is included
		unsigned int	numSenders
		);
	~CVideomap();

	void		AddSender(
		CBitmap*		bitmap,
		CNode*			sender,
		unsigned int	senderIndex
		);
	//void		AddServer(
	//	CNode*			server
	//	);
	CNode*		GetSender(
		unsigned int	pointer,
		unsigned int&	lastIndex
		);
};

#endif