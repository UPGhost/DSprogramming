#include <stdlib.h>
#include <string.h>
#include <nds.h>

#define WIDTH 256
#define HEIGHT 192

#include "Touch_The_Screen_pcx.h"
#include "Test32by32_pcx.h"
#include "ball_pcx.h"


typedef struct {
	int x;
	int y;			// screen co-ordinates
	int dx; 
	int dy;			// velocity
	SpriteEntry* oam;	// pointer to the sprite attributes in OAM
	int gfxID; 			// graphics lovation	
}Sprite;

typedef struct  
{
   u16* gfx;
   SpriteSize size;
   SpriteColorFormat format;
   int rotationIndex;
   int paletteAlpha;
   int x;
   int y;
}MySprite;

	SpriteEntry OAMCopySub[128];

//Initialise Variables
	
	//Background
	int i = 0;
	int delta = 0;
	
	int ix = 0;
	int iy = 0;	
	
	uint16* map0 = (uint16*)SCREEN_BASE_BLOCK_SUB(1);
	uint16* map1 = (uint16*)SCREEN_BASE_BLOCK_SUB(2);
	
		

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

//OAM

//---------------------------------------------------------------------------------
void initOAM(void) {
//---------------------------------------------------------------------------------	
	for(i = 0; i < 128; i++) {
		OAMCopySub[i].attribute[0] = ATTR0_DISABLED;
	}
}

//---------------------------------------------------------------------------------
void updateOAM(void) {
//---------------------------------------------------------------------------------
	memcpy(OAM_SUB, OAMCopySub, 128 * sizeof(SpriteEntry));
}


//Sprites

//---------------------------------------------------------------------------------
void MoveSprite(Sprite* sp) {
//---------------------------------------------------------------------------------
	/*
	int x = sp->x >> 8;
	int y = sp->y >> 8;

	sp->oam->x = x;
	sp->oam->y = y;
	*/
	
	sp->oam->x = sp->x;
	sp->oam->y = sp->y;

}

void TouchTheScreenText()
{
	sImage TouchTheScreenSprite;

	//load pcx file into an image
	loadPCX((u8*)Test32by32_pcx, &TouchTheScreenSprite);

	//tile it so it is usefull as sprite data
	imageTileData(&TouchTheScreenSprite);
	
	// Sprite initialisation
	for(i = 0; i < 256; i++)
		SPRITE_PALETTE_SUB[i] = TouchTheScreenSprite.palette[i];

	for(i = 0; i< 32*16; i++)
		SPRITE_GFX_SUB[i] = TouchTheScreenSprite.image.data16[i];	
}

void SpriteAdd()
{
	TouchTheScreenText();
	
	//turn off sprites
	initOAM();

}


//Background

void BackgroundGeneration(int ix, int iy)
{	
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
}










int main(void) 
{

//Initial Functions

	ScreenSetUp();
	//vram banks are somewhat complex
	
	//consoleDemoInit();
 
	//Touchscreen
/*	
	touchPosition touch;	
	touch.px = 0;
	touch.py = 0;
*/
	


	

	
	//Sprites
	
	//vram banks are somewhat complex
	vramSetPrimaryBanks(VRAM_A_MAIN_BG_0x06000000, VRAM_B_MAIN_SPRITE, VRAM_C_SUB_BG, VRAM_D_SUB_SPRITE);

	SpriteAdd();
		
	/*
	Sprite SpriteArray[1] =
	{
	  //{  x,  y, dx, dy, &OAMCopySub[i], 0}
		{30, 30, 0, 0, &OAMCopySub[0], 0}
	};
	
	for (i = 0; i < 1; i++)
	{
		SpriteArray[i].oam->attribute[0] = ATTR0_COLOR_256 | ATTR0_SQUARE;
		SpriteArray[i].oam->attribute[1] = ATTR1_SIZE_32;
		SpriteArray[i].oam->attribute[2] = SpriteArray[i].gfxID;
	}*/
	
	Sprite SpriteArray[] =
	{
		// x,  y, dx,dy, &OAMCopySub[i], 0
		{100, 80, 0, 0, &OAMCopySub[0], 0} 
	};
	
	for (i = 0; i < 1; i++)
	{
		SpriteArray[i].oam->attribute[0] = ATTR0_COLOR_256 | ATTR0_SQUARE;
		SpriteArray[i].oam->attribute[1] = ATTR1_SIZE_32;
		SpriteArray[i].oam->attribute[2] = SpriteArray[i].gfxID;
	}
	
	/*

	for(i = 0; i < 1; i++) {
		//random place and speed
		sprites[i].x = 100;
		sprites[i].y = 80;
		sprites[i].dx = 0;
		sprites[i].dy = 0;

		sprites[i].oam = &OAMCopySub[i];
		sprites[i].gfxID = 0;

		//set up our sprites OAM entry attributes
		sprites[i].oam->attribute[0] = ATTR0_COLOR_256 | ATTR0_SQUARE;
		sprites[i].oam->attribute[1] = ATTR1_SIZE_32;
		sprites[i].oam->attribute[2] = sprites[i].gfxID;
	}
	*/
	
	//Background
	
	BackgroundGeneration(ix, iy);
	
	

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
		
		//Sprites
		for (i = 0; i < 1; i++)
		{
			MoveSprite(&SpriteArray[i]);
		}
		
		//scroll the background
		REG_BG0HOFS_SUB = delta ++; // Horizontal
		//REG_BG0VOFS_SUB = delta++; // Vertical
		
		swiWaitForVBlank();
		
		scanKeys();
		
		int held = keysHeld();

		if(held & KEY_START) break;

		if(held & KEY_TOUCH) 
		{
			//touchRead(&touch);

			//iprintf("\x1b[11;5HTouch (X,Y) = (%d,%d)       ", touch.px, touch.py);
		}
		
		updateOAM();
		
	
	}

	

















	return 0;
}

