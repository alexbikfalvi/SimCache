#ifndef __Video_h__
#define __Video_h__

class CVideo
{
private:
	unsigned int	id;				// Identifier
	unsigned int	popularity;		// Popularity
	unsigned int	size;			// Size in number of segments
	unsigned int	segSize;		// Size of a segment (needed to calculate delays)
	__bitrate		rate;			// Playback bit rate
	__time			duration;		// Duration of the video
	__time			segDuration;	// Duration of a segment

public:
	CVideo(
		unsigned int	id,
		__time			duration,
		unsigned int	segSize,
		__bitrate		rate,
		unsigned int	popularity
		);
	~CVideo();

	inline unsigned int		Id() { return this->id; }
	inline unsigned int		Size() { return this->size; }
	inline unsigned int		SegSize() { return this->segSize; }
	inline __bitrate		Rate() { return this->rate; }
	inline __time			Duration() { return this->duration; }
	inline __time			SegDuration() { return this->segDuration; }
	inline unsigned int		Popularity() { return this->popularity; }
};

#endif