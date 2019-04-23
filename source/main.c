#include <stdlib.h>
#include <string.h>
#include <nds.h>

#define WIDTH 256
#define HEIGHT 192

#include "Touch_The_Screen_Pcx.h"

typedef struct {
	int x,y;			// screen co-ordinates	
	SpriteEntry* oam;	// pointer to the sprite attributes in OAM
	int gfxID; 			// graphics lovation	
}Text;

void ScreenSetUp(void)
{
	//set main display to render 256 color bitmap
	videoSetMode(MODE_5_2D | DISPLAY_BG3_ACTIVE);

	//set up the sub display
	videoSetModeSub(MODE_0_2D 		 	|
					DISPLAY_SPR_1D_LAYOUT |
					DISPLAY_SPR_ACTIVE |
					DISPLAY_BG0_ACTIVE |
					DISPLAY_BG1_ACTIVE );
}



int main(void) 
{

//Initial Functions

	ScreenSetUp();	
	consoleDemoInit(); 
	
//Initialise Variables
	
	//Background
	int i = 0;
	int delta = 0;
	
	int ix = 0;
	int iy = 0;	
	
	uint16* map0 = (uint16*)SCREEN_BASE_BLOCK_SUB(1);
	uint16* map1 = (uint16*)SCREEN_BASE_BLOCK_SUB(2);
	
	//Touchscreen	
	touchPosition touch;	
	touch.px = 0;
	touch.py = 0;
	
//Background
	
	REG_BG0CNT_SUB = BG_COLOR_256 | (1 << MAP_BASE_SHIFT);
	REG_BG1CNT_SUB = BG_COLOR_256 | (2 << MAP_BASE_SHIFT);
	
	BG_PALETTE_SUB[0] = RGB15(31,0,0);
	BG_PALETTE_SUB[1] = RGB15(0,0,31);
	BG_PALETTE_SUB[2] = RGB15(0,31,0);
	
	for(iy = 1; iy < 32; iy+= 4) 
	{
		for(ix = 0; ix < 32; ix += 4) 
		{
			map0[iy     * 32 + ix    ] = (ix ^ iy) & 1;
			map0[iy     * 32 + ix + 1] = (ix ^ iy) & 1;
			map0[(iy - 1) * 32 + ix    ] = (ix ^ iy) & 1;
			map0[(iy - 1) * 32 + ix + 1] = (ix ^ iy) & 1;
			
			map0[(iy + 1)  * 32 + ix + 2] = (ix ^ iy) & 1;
			map0[(iy + 1)  * 32 + ix + 3] = (ix ^ iy) & 1;
			map0[(iy + 2) * 32 + ix + 2] = (ix ^ iy) & 1;
			map0[(iy + 2) * 32 + ix + 3] = (ix ^ iy) & 1;
		}		
	}
	
	sImage ball;

	//load our ball pcx file into an image
	loadPCX((u8*)ball_pcx, &ball);

	//tile it so it is usefull as sprite data
	imageTileData(&ball);
	
	

	//fill 2 tiles with different colors
	for(i = 0; i < 32; i++) 
	{	
		BG_GFX_SUB[i+32] = 0x0202;
		//BG_GFX_SUB[i+32] = 0x0000;
	
		//BG_GFX_SUB[i+32+32] = 0x0303;  
		//+32 = Offset (Makes it so it fits properly)
		// 0x0n0n = BG_PALLETTE_SUB[n]
		//i < 32 = if 0 then only first pixel appears
		//i < 16 means Top Half of Square Appears
		//i < 48 means Bottom Half of Square Appears		
	}

	while(1) 
	{	
		swiWaitForVBlank();
		
		scanKeys();
		
		int held = keysHeld();

		if(held & KEY_START) break;

		if(held & KEY_TOUCH) 
		{

			touchRead(&touch);
		/*
			if (touch.z1 != 0 ) 
			{
				area = touch.px * touch.z2 / touch.z1 - touch.px;
			}
		*/

			//iprintf("\x1b[10;0HTouch x = %04i, %04i\n", touch.rawx, touch.px);

			iprintf("\x1b[11;5HTouch (X,Y) = (%d,%d)       ", touch.px, touch.py);

			//iprintf("Touch Area (pressure) %04i\n", area);

		}
	
		//scroll the background
		REG_BG0HOFS_SUB = delta ++; // Horizontal
		//REG_BG0VOFS_SUB = delta++; // Vertical
		
	
	}

	

















	return 0;
}

