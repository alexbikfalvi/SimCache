#ifndef __Shuffle_h__
#define __Shuffle_h__

class CShuffle
{
private:
	unsigned int	num;

	unsigned int*	mapping;

public:
	CShuffle(
		unsigned int	num
		);
	~CShuffle();

	inline unsigned int	operator[] (unsigned int index) { assert(index < this->num); return this->mapping[index];}
	void				Shuffle();
};

#endif