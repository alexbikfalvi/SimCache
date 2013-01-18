#include "stdafx.h"
#include "Rand.h"

double CRand::Generate()
{
	unsigned int rvalue;
	rand_s(&rvalue);
	return ((double)rvalue) / UINT_MAX;
}

unsigned int CRand::Generate(unsigned int max)
{
	unsigned int rvalue;
	rand_s(&rvalue);
	return (unsigned int)(max * (((double)rvalue) / UINT_MAX));
}

//unsigned long long CRand::GenerateLong()
//{
//	unsigned int rvalue1, rvalue2;
//	rand_s(&rvalue1);
//	rand_s(&rvalue2);
//	return (((unsigned long long) rvalue1) << 32) | rvalue2;
//}
//
//unsigned int CRand::GenerateInt()
//{
//	unsigned int rvalue;
//	rand_s(&rvalue);
//	return rvalue;
//}