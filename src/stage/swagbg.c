/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "swagbg.h"

#include "../archive.h"
#include "../mem.h"
#include "../stage.h"
#include "../random.h"
#include "../timer.h"
#include "../animation.h"

//Week 4 background structure
typedef struct
{
	//Stage background base structure
	StageBack back;
	
	//Textures
	Gfx_Tex tex_back2; //Sunset

} Back_swagbg;

//Week 4 background functions

void Back_swagbg_DrawFG(StageBack *back)
{

}

void Back_swagbg_DrawMD(StageBack *back)
{

}

void Back_swagbg_DrawBG(StageBack *back)
{
	Back_swagbg *this = (Back_swagbg*)back;
	
	fixed_t fx, fy;
	
	
	//Draw sunset
	fx = stage.camera.x;
	fy = stage.camera.y;
	
	RECT sunset_src = {0, 0, 256, 256};
	RECT_FIXED sunset_dst = {
		FIXED_DEC(-165 - SCREEN_WIDEOADD2,1) - fx,
		FIXED_DEC(-140,1) - fy,
		FIXED_DEC(600 + SCREEN_WIDEOADD,1),
		FIXED_DEC(300,1)
	};
	
	Stage_DrawTex(&this->tex_back2, &sunset_src, &sunset_dst, stage.camera.bzoom);
}

void Back_swagbg_Free(StageBack *back)
{
	Back_swagbg *this = (Back_swagbg*)back;
	
	//Free structure
	Mem_Free(this);
}

StageBack *Back_swagbg_New(void)
{
	//Allocate background structure
	Back_swagbg *this = (Back_swagbg*)Mem_Alloc(sizeof(Back_swagbg));
	if (this == NULL)
		return NULL;
	
	//Set background functions
	this->back.draw_fg = Back_swagbg_DrawFG;
	this->back.draw_md = Back_swagbg_DrawMD;
	this->back.draw_bg = Back_swagbg_DrawBG;
	this->back.free = Back_swagbg_Free;
	
	//Load background textures
	IO_Data arc_back = IO_Read("\\SWAGBG\\BACK.ARC;1");
	Gfx_LoadTex(&this->tex_back2, Archive_Find(arc_back, "back2.tim"), 0);
	Mem_Free(arc_back);
	
	return (StageBack*)this;
}
