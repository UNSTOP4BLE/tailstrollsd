/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "sonic.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Christmas Parents structure
enum
{
	sonic_ArcMain_Idle0,
	sonic_ArcMain_Idle1,
	sonic_ArcMain_Idle2,
	sonic_ArcMain_Idle3,
	sonic_ArcMain_LeftA0,
	sonic_ArcMain_LeftA1,
	sonic_ArcMain_LeftB0,
	sonic_ArcMain_LeftB1,
	sonic_ArcMain_DownA0,
	sonic_ArcMain_DownA1,
	sonic_ArcMain_DownB0,
	sonic_ArcMain_DownB1,
	sonic_ArcMain_UpA0,
	sonic_ArcMain_UpA1,
	sonic_ArcMain_UpB0,
	sonic_ArcMain_UpB1,
	sonic_ArcMain_RightA0,
	sonic_ArcMain_RightA1,
	sonic_ArcMain_RightB0,
	sonic_ArcMain_RightB1,

	sonic_ArcMain_LeftC0,
	sonic_ArcMain_LeftC1,
	sonic_ArcMain_DownC0,
	sonic_ArcMain_DownC1,
	sonic_ArcMain_UpC0,
	sonic_ArcMain_UpC1,
	sonic_ArcMain_RightC0,
	sonic_ArcMain_RightC1,
	
	sonic_ArcMain_Idlealt0,
	sonic_ArcMain_Idlealt1,
	sonic_ArcMain_Idlealt2,
	sonic_ArcMain_Idlealt3,

	sonic_ArcMain_Idleb0,
	sonic_ArcMain_Idleb1,
	sonic_ArcMain_Idleb2,
	sonic_ArcMain_Idleb3,

	
	sonic_Arc_Max,
};

typedef struct
{
	//Swap base structure
	Swap swap;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[sonic_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Swap_sonic;

//Christmas Parents definitions
static const SwapFrame swap_sonic_frame[] = {
	{sonic_ArcMain_Idle0, {0,   0, 110, 142}, {130, 182}}, //0 idle 1
	{sonic_ArcMain_Idle1, {0,   0, 110, 142}, {130, 182}}, //1 idle 2
	{sonic_ArcMain_Idle2, {0,   0, 110, 142}, {130, 182}}, //2 idle 3
	{sonic_ArcMain_Idle3, {0,   0, 110, 142}, {130, 182}}, //3 idle 4
	
	{sonic_ArcMain_LeftA0, {0,   0, 110, 142}, {130, 182}}, //4 left a 1
	{sonic_ArcMain_LeftA1, {0,   0, 110, 142}, {130, 182}}, //5 left a 2
	{sonic_ArcMain_LeftB0, {0,   0, 110, 142}, {130, 182}}, //6 left b 1
	{sonic_ArcMain_LeftB1, {0,   0, 110, 142}, {130, 182}}, //7 left b 2
	
	{sonic_ArcMain_DownA0, {0,   0, 110, 142}, {130, 182}}, //8 down a 1
	{sonic_ArcMain_DownA1, {0,   0, 110, 142}, {130, 182}}, //9 down a 2
	{sonic_ArcMain_DownB0, {0,   0, 110, 142}, {130, 182}}, //10 down b 1
	{sonic_ArcMain_DownB1, {0,   0, 110, 142}, {130, 182}}, //11 down b 2
	
	{sonic_ArcMain_UpA0, {0,   0, 110, 142}, {130, 182}}, //12 up a 1
	{sonic_ArcMain_UpA1, {0,   0, 110, 142}, {130, 182}}, //13 up a 2
	{sonic_ArcMain_UpB0, {0,   0, 110, 150}, {130, 182}}, //14 up b 1
	{sonic_ArcMain_UpB1, {0,   0, 110, 150}, {130, 182}}, //15 up b 2
	
	{sonic_ArcMain_RightA0, {0,   0, 110, 142}, {130, 182}}, //16 right a 1
	{sonic_ArcMain_RightA1, {0,   0, 110, 142}, {130, 182}}, //17 right a 2
	{sonic_ArcMain_RightB0, {0,   0, 110, 142}, {130, 182}}, //18 right b 1
	{sonic_ArcMain_RightB1, {0,   0, 110, 142}, {130, 182}}, //19 right b 2

	{sonic_ArcMain_LeftC0, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_LeftC1, {0,   0, 110, 142}, {130, 182}},

	{sonic_ArcMain_DownC0, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_DownC1, {0,   0, 110, 142}, {130, 182}},

	{sonic_ArcMain_UpC0, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_UpC1, {0,   0, 110, 142}, {130, 182}},

	{sonic_ArcMain_RightC0, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_RightC1, {0,   0, 110, 142}, {130, 182}},

	{sonic_ArcMain_Idlealt0, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_Idlealt1, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_Idlealt2, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_Idlealt3, {0,   0, 110, 142}, {130, 182}},

	{sonic_ArcMain_Idleb0, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_Idleb1, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_Idleb2, {0,   0, 110, 142}, {130, 182}},
	{sonic_ArcMain_Idleb3, {0,   0, 110, 142}, {130, 182}},

};

static const Animation swap_sonic_anim[SwapAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 4,  5, ASCR_BACK, 1}},         //CharAnim_Left
	{2, (const u8[]){ 6,  7, ASCR_CHGANI,  SwapAnim_Idleb}},         //CharAnim_LeftAlt
	{2, (const u8[]){ 8,  9, ASCR_BACK, 1}},         //CharAnim_Down
	{2, (const u8[]){10, 11, ASCR_CHGANI,  SwapAnim_Idleb}},         //CharAnim_DownAlt
	{2, (const u8[]){12, 13, ASCR_BACK, 1}},         //CharAnim_Up
	{2, (const u8[]){14, 15, ASCR_CHGANI,  SwapAnim_Idleb}},         //CharAnim_UpAlt
	{2, (const u8[]){16, 17, ASCR_BACK, 1}},         //CharAnim_Right
	{2, (const u8[]){18, 19, ASCR_CHGANI,  SwapAnim_Idleb}},         //CharAnim_RightAlt
	{2, (const u8[]){ 20,  21, ASCR_BACK,1 }},	//CharAnim_LeftAltc
    {2, (const u8[]){ 20,  21, ASCR_BACK}},	//CharAnim_LeftAltc
	{2, (const u8[]){ 22,  23, ASCR_BACK,1 }},	//CharAnim_DownAltc
    {2, (const u8[]){ 20,  21, ASCR_BACK}},	//CharAnim_LeftAltc
	{2, (const u8[]){ 24,  25, ASCR_BACK, 1 }},	//CharAnim_UpAltc
    {2, (const u8[]){ 20,  21, ASCR_BACK}},	//CharAnim_LeftAltc
	{2, (const u8[]){ 26,  27, ASCR_BACK, 1 }},	//CharAnimCharAnim_RightAltc
    {2, (const u8[]){ 20,  21, ASCR_BACK}},	//CharAnim_LeftAltc
	{2, (const u8[]){ 28,  29,  30,  31, ASCR_BACK, 1 }}, //CharAnim_altIdle
	{2, (const u8[]){ 32,  33,  34,  35, ASCR_CHGANI,  SwapAnim_Idleb}}, //CharAnim_altIdle2
};

//Christmas Parents functions
void Swap_sonic_SetFrame(void *user, u8 frame)
{
	Swap_sonic *this = (Swap_sonic*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const SwapFrame *cframe = &swap_sonic_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Swap_sonic_Tick(Swap *swap)
{
	Swap_sonic *this = (Swap_sonic*)swap;
	
	//Perform idle dance
	if ((swap->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
	  {
	   if(stage.stage_id == StageId_1_3 && stage.song_step >= 1408 && stage.song_step <= 1664)
		Swap_PerformIdleAlt(swap);

		else
		Swap_PerformIdle(swap);
	  }
	
	//Animate and draw
	Animatable_Animate(&swap->animatable, (void*)this, Swap_sonic_SetFrame);
	Swap_Draw(swap, &this->tex, &swap_sonic_frame[this->frame]);
}

void Swap_sonic_SetAnim(Swap *swap, u8 anim)
{

	//Set animation
	Animatable_SetAnim(&swap->animatable, anim);
	
    if(stage.stage_id == StageId_1_3 && stage.song_step >= 1408 && stage.song_step <= 1664)
	Swap_CheckStartSing2(swap);

	else
	Swap_CheckStartSing(swap);

}

void Swap_sonic_Free(Swap *Swap)
{
	Swap_sonic *this = (Swap_sonic*)Swap;
	
	//Free art
	Mem_Free(this->arc_main);
}

Swap *Swap_sonic_New(fixed_t x, fixed_t y)
{
	//Allocate Christmas Parents object
	Swap_sonic *this = Mem_Alloc(sizeof(Swap_sonic));
	if (this == NULL)
	{
		sprintf(error_msg, "[Swap_sonic_New] Failed to allocate Christmas Parents object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize Swap
	this->swap.tick = Swap_sonic_Tick;
	this->swap.set_anim = Swap_sonic_SetAnim;
	this->swap.free = Swap_sonic_Free;
	
	Animatable_Init(&this->swap.animatable, swap_sonic_anim);
	Swap_Init((Swap*)this, x, y);
	
	//Set Swap information

    this->swap.spec = CHAR_SPEC_MISSANIM2;
	
	this->swap.health_i = 5;
	
	this->swap.focus_x = FIXED_DEC(25,1);
	this->swap.focus_y = FIXED_DEC(-50,1);
	this->swap.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SONIC.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",   //sonic_ArcMain_Idle0
		"idle1.tim",   //sonic_ArcMain_Idle1
		"idle2.tim",   //sonic_ArcMain_Idle2
		"idle3.tim",   //sonic_ArcMain_Idle3
		"lefta0.tim",  //sonic_ArcMain_LeftA0
		"lefta1.tim",  //sonic_ArcMain_LeftA1
		"leftb0.tim",  //sonic_ArcMain_LeftB0
		"leftb1.tim",  //sonic_ArcMain_LeftB1
		"downa0.tim",  //sonic_ArcMain_DownA0
		"downa1.tim",  //sonic_ArcMain_DownA1
		"downb0.tim",  //sonic_ArcMain_DownB0
		"downb1.tim",  //sonic_ArcMain_DownB1
		"upa0.tim",    //sonic_ArcMain_UpA0
		"upa1.tim",    //sonic_ArcMain_UpA1
		"upb0.tim",    //sonic_ArcMain_UpB0
		"upb1.tim",    //sonic_ArcMain_UpB1
		"righta0.tim", //sonic_ArcMain_RightA0
		"righta1.tim", //sonic_ArcMain_RightA1
		"rightb0.tim", //sonic_ArcMain_RightB0
		"rightb1.tim", //sonic_ArcMain_RightB1
		"leftc0.tim",  //sonic_ArcMain_Leftc0
		"leftc1.tim",  //sonic_ArcMain_Leftc0
		"downc0.tim",  //sonic_ArcMain_Downc0
		"downc1.tim",  //sonic_ArcMain_Downc1
		"upc0.tim",    //sonic_ArcMain_UpA0
		"upc1.tim",    //sonic_ArcMain_UpA1
		"rightc0.tim", //sonic_ArcMain_Rightc0
		"rightc1.tim", //sonic_ArcMain_Rightc1
		"idlealt0.tim",   //sonic_ArcMain_Idle0
		"idlealt1.tim",   //sonic_ArcMain_Idle1
		"idlealt2.tim",   //sonic_ArcMain_Idle2
		"idlealt3.tim",   //sonic_ArcMain_Idle3
		"idleb0.tim",   //sonic_ArcMain_Idle0
		"idleb1.tim",   //sonic_ArcMain_Idle1
		"idleb2.tim",   //sonic_ArcMain_Idle2
		"idleb3.tim",   //sonic_ArcMain_Idle3

		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Swap*)this;
}
