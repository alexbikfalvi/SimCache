#ifndef __Bitmap_h__
#define __Bitmap_h__

class CBitmap
{
private:
	unsigned int	size;
	unsigned int*	request;
	unsigned int*	response;
	unsigned int	bsize;

public:
	CBitmap(
		unsigned int	size
		);
	CBitmap(
		unsigned int	size,
		unsigned int	first,
		unsigned int	last
		);
	~CBitmap();

	inline unsigned int		Size() { return this->size; }

	bool					HasReq(unsigned int index);
	void					StoreReq(unsigned int index);
	void					EraseReq(unsigned int index);

	bool					HasConf(unsigned int index);
	void					StoreConf(unsigned int index);

	void					SetConf(CBitmap* bitmap);
	void					SetZeroConf();

	void					Debug();
};

#endif