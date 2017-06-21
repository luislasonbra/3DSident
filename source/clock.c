#include "clock.h"
#include "main.h"

void digitalTime()
{
	time_t unix_time = time(0);
	struct tm* time_struct = gmtime((const time_t*)&unix_time);
	int hours = time_struct->tm_hour;
	int minutes = time_struct->tm_min;
	int amOrPm = 0;
	
	if(hours < 12)
		amOrPm = 1;
	if(hours == 0)
		hours = 12;
	else if(hours > 12)
		hours = hours - 12;

	if (hours >= 1 && hours < 10)  
		sftd_draw_textf(font_m, 395 - sftd_get_text_width(font_m, 12, "0:00 XM"), 1, RGBA8(250, 237, 227, 255), 12, "%2i:%02i %s", hours, minutes, amOrPm ? "AM" : "PM");
	else
		sftd_draw_textf(font_m, 395 - sftd_get_text_width(font_m, 12, "00:00 XM"), 1, RGBA8(250, 237, 227, 255), 12, "%2i:%02i %s", hours, minutes, amOrPm ? "AM" : "PM");
}