#include "video_reader.h"
#include "include/bitrate_selector.h"

class FEC_Param_Adjuster{
public:
	FEC_Param_Adjuster();
	~FEC_Param_Adjuster();

	setFEC_param(double plr, Bitrate_Selector &bitrate_selector,
				 Video_Reader &video_reader);
private:

};