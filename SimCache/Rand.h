#ifndef __Rand_h__
#define __Rand_h__

class CRand
{
public:
	static double				Generate();
	static unsigned int			Generate(unsigned int max);
	//static unsigned int			GenerateInt();
	//static unsigned long long	GenerateLong();
};

#endif