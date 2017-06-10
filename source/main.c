#include "main.h"
#include "utils.h"

void initServices()
{
	hidInit();
	romfsInit();
	sf2d_init();
	sftd_init();
	
	sf2d_set_clear_color(RGBA8(0, 0, 0, 255));
	sf2d_set_vblank_wait(0);
	
	btn_A = sfil_load_PNG_file("romfs:/buttons/A.png", SF2D_PLACE_RAM); setBilinearFilter(btn_A);
	btn_B = sfil_load_PNG_file("romfs:/buttons/B.png", SF2D_PLACE_RAM); setBilinearFilter(btn_B);
	btn_X = sfil_load_PNG_file("romfs:/buttons/X.png", SF2D_PLACE_RAM); setBilinearFilter(btn_X);
	btn_Y = sfil_load_PNG_file("romfs:/buttons/Y.png", SF2D_PLACE_RAM); setBilinearFilter(btn_Y);
	btn_Start_Select = sfil_load_PNG_file("romfs:/buttons/Start_select.png", SF2D_PLACE_RAM); setBilinearFilter(btn_Start_Select);
	btn_L = sfil_load_PNG_file("romfs:/buttons/L.png", SF2D_PLACE_RAM); setBilinearFilter(btn_L);
	btn_R = sfil_load_PNG_file("romfs:/buttons/R.png", SF2D_PLACE_RAM); setBilinearFilter(btn_R);
	btn_ZL = sfil_load_PNG_file("romfs:/buttons/ZL.png", SF2D_PLACE_RAM); setBilinearFilter(btn_ZL);
	btn_ZR = sfil_load_PNG_file("romfs:/buttons/ZR.png", SF2D_PLACE_RAM); setBilinearFilter(btn_ZR);
	btn_Dpad = sfil_load_PNG_file("romfs:/buttons/D_pad.png", SF2D_PLACE_RAM); setBilinearFilter(btn_Dpad);
	btn_Cpad = sfil_load_PNG_file("romfs:/buttons/Circle_pad.png", SF2D_PLACE_RAM); setBilinearFilter(btn_Cpad);
	btn_Cstick = sfil_load_PNG_file("romfs:/buttons/C_stick.png", SF2D_PLACE_RAM); setBilinearFilter(btn_Cstick);
	btn_home = sfil_load_PNG_file("romfs:/buttons/Home.png", SF2D_PLACE_RAM); setBilinearFilter(btn_home);
	
	cursor = sfil_load_PNG_file("romfs:/buttons/cursor.png", SF2D_PLACE_RAM); setBilinearFilter(cursor);
	volumeIcon = sfil_load_PNG_file("romfs:/buttons/volume.png", SF2D_PLACE_RAM); setBilinearFilter(volumeIcon);
	
	font = sftd_load_font_mem(Ubuntu_ttf, Ubuntu_ttf_size);
	
	if (isN3DS())
		osSetSpeedupEnable(true);
}

void termServices()
{
	romfsExit();
	sftd_fini();
	sf2d_fini();
	hidExit();
}

void mainMenu()
{
	circlePosition cPad;
	circlePosition cStick;
	touchPosition touch;
	
	u16 touch_x = 0, touch_y = 0;
	
	u8 volume;
	
	while (aptMainLoop())
    {
		hidScanInput();
		//irrstScanInput(); // Already defined in hidScanInput();
		hidCircleRead(&cPad);
		hidCstickRead(&cStick);
		
		u32 kHeld = hidKeysHeld();
		
		HIDUSER_GetSoundVolume(&volume);
		
		sf2d_start_frame(GFX_BOTTOM, GFX_LEFT);
		
		sf2d_draw_rectangle(0, 0, 320, 240, RGBA8(48, 10, 36, 255));
		
		if (kHeld & KEY_TOUCH) 
		{
			hidTouchRead(&touch);
			touch_x = touch.px;
			touch_y = touch.py;
		}
		sf2d_draw_texture(cursor, touch_x, touch_y);
		
		sf2d_end_frame();
		
		sf2d_start_frame(GFX_TOP, GFX_LEFT);
		
		sf2d_draw_rectangle(0, 0, 400, 240, RGBA8(60, 61, 63, 255));
	
		sf2d_draw_rectangle(75, 30, 250, 210, RGBA8(97, 101, 104, 255));
		sf2d_draw_rectangle(85, 40, 230, 175, RGBA8(242, 241, 239, 255));
		sf2d_draw_rectangle(85, 40, 230, 15, RGBA8(66, 65, 61, 255));
	
		sftd_draw_textf(font, 90, 40, RGBA8(230, 232, 214, 255), 11, "3DSident Button Test");
	
		sftd_draw_textf(font, 90, 56, RGBA8(77, 76, 74, 255), 11, "Circle pad: %04d, %04d", cPad.dx, cPad.dy);
		sftd_draw_textf(font, 90, 70, RGBA8(77, 76, 74, 255), 11, "C stick: %04d, %04d", cStick.dx, cStick.dy);
		sftd_draw_textf(font, 90, 84, RGBA8(77, 76, 74, 255), 11, "Touch position: %03d, %03d", touch.px, touch.py);
		sftd_draw_textf(font, 90, 84, RGBA8(77, 76, 74, 255), 11, "Touch position: %03d, %03d", touch.px, touch.py);
		
		sf2d_draw_texture(volumeIcon, 90, 98);
		double volPercent = (volume * 1.5873015873);
		sf2d_draw_rectangle(115, 104, 190, 5, RGBA8(219, 219, 219, 255));
		sf2d_draw_rectangle(115, 104, ((volPercent / 100) * 190), 5, RGBA8(241, 122, 74, 255));
		
		sftd_draw_textf(font, 90, 118, RGBA8(77, 76, 74, 255), 11, "3D");
		double _3dSliderPercent = (osGet3DSliderState() * 100.0);
		sf2d_draw_rectangle(115, 122, 190, 5, RGBA8(219, 219, 219, 255));
		sf2d_draw_rectangle(115, 122, ((_3dSliderPercent / 100) * 190), 5, RGBA8(241, 122, 74, 255));
		
		sftd_draw_textf(font, 90, 138, RGBA8(77, 76, 74, 255), 11, "Use home button to exit.");
	
		sf2d_draw_texture(btn_home, 180, 215);
		
		if (kHeld & KEY_L)
			sf2d_draw_texture_blend(btn_L, 0, 0, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_L, 0, 0);
	
		if (kHeld & KEY_R)
			sf2d_draw_texture_blend(btn_R, 345, 0, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_R, 345, 0);
	
		if (kHeld & KEY_ZL)
			sf2d_draw_texture_blend(btn_ZL, 60, 0, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_ZL, 60, 0);
	
		if (kHeld & KEY_ZR)
			sf2d_draw_texture_blend(btn_ZR, 300, 0, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_ZR, 300, 0);
	
		if (kHeld & KEY_A)
			sf2d_draw_texture_blend(btn_A, 370, 80, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_A, 370, 80);
	
		if (kHeld & KEY_B)
			sf2d_draw_texture_blend(btn_B, 350, 100, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_B, 350, 100);
	
		if (kHeld & KEY_X)
			sf2d_draw_texture_blend(btn_X, 350, 60, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_X, 350, 60);
	
		if (kHeld & KEY_Y)
			sf2d_draw_texture_blend(btn_Y, 330, 80, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_Y, 330, 80);
	
		if (kHeld & KEY_START)
			sf2d_draw_texture_blend(btn_Start_Select, 330, 140, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_Start_Select, 330, 140);
	
		if (kHeld & KEY_SELECT)
			sf2d_draw_texture_blend(btn_Start_Select, 330, 165, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_Start_Select, 330, 165);
	
		if (kHeld & KEY_CPAD_LEFT)
			sf2d_draw_texture_blend(btn_Cpad, 3, 55, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_CPAD_RIGHT)
			sf2d_draw_texture_blend(btn_Cpad, 13, 55, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_CPAD_UP)
			sf2d_draw_texture_blend(btn_Cpad, 8, 50, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_CPAD_DOWN)
			sf2d_draw_texture_blend(btn_Cpad, 8, 60, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_Cpad, 8, 55);
	
		if (kHeld & KEY_DLEFT)
			sf2d_draw_texture_blend(btn_Dpad, 0, 110, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_DRIGHT)
			sf2d_draw_texture_blend(btn_Dpad, 10, 110, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_DUP)
			sf2d_draw_texture_blend(btn_Dpad, 5, 105, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_DDOWN)
			sf2d_draw_texture_blend(btn_Dpad, 5, 115, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_Dpad, 5, 110);
	
		if (kHeld & KEY_CSTICK_LEFT)
			sf2d_draw_texture_blend(btn_Cstick, 325, 35, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_CSTICK_RIGHT)
			sf2d_draw_texture_blend(btn_Cstick, 335, 35, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_CSTICK_UP)
			sf2d_draw_texture_blend(btn_Cstick, 330, 30, RGBA8(242, 119, 62, 100));
		else if (kHeld & KEY_CSTICK_DOWN)
			sf2d_draw_texture_blend(btn_Cstick, 330, 40, RGBA8(242, 119, 62, 100));
		else
			sf2d_draw_texture(btn_Cstick, 330, 35);
		
		endDrawing();
	}
}

int main(int argc, char *argv[])
{      
	initServices();
	
	mainMenu();
	
	termServices();
	
	return 0;	
}