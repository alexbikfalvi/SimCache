#ifndef __Indication_h__
#define __Indication_h__

class CIndication
{
private:
	bool		cache;
public:
	CIndication() { this->cache = 0; }
	CIndication(bool cache) { this->cache = cache; }
	~CIndication() {}

	inline bool		Cache() { return this->cache; }
};

#endif