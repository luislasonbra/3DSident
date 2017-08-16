#include "misc.h"

u32 titleCount(FS_MediaType mediaType)
{
	u32 count = 0;
	
	if (R_SUCCEEDED(AM_GetTitleCount(mediaType, &count)))
		return count;

    return 0;
}