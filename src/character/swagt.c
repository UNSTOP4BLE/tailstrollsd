/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "swagt.h"

#include "../mem.h"
#include "../archive.h"
#include "../stage.h"
#include "../main.h"

//Christmas Parents structure
enum
{
	swagt_ArcMain_Idle0,
	swagt_ArcMain_Idle1,
	swagt_ArcMain_Idle2,
	swagt_ArcMain_Idle3,
	swagt_ArcMain_LeftA0,
	swagt_ArcMain_LeftA1,
	swagt_ArcMain_LeftB0,
	swagt_ArcMain_LeftB1,
	swagt_ArcMain_DownA0,
	swagt_ArcMain_DownA1,
	swagt_ArcMain_DownB0,
	swagt_ArcMain_DownB1,
	swagt_ArcMain_UpA0,
	swagt_ArcMain_UpA1,
	swagt_ArcMain_UpB0,
	swagt_ArcMain_UpB1,
	swagt_ArcMain_RightA0,
	swagt_ArcMain_RightA1,
	swagt_ArcMain_RightB0,
	swagt_ArcMain_RightB1,
	
	swagt_Arc_Max,
};

typedef struct
{
	//Character base structure
	Character character;
	
	//Render data and state
	IO_Data arc_main;
	IO_Data arc_ptr[swagt_Arc_Max];
	
	Gfx_Tex tex;
	u8 frame, tex_id;
} Char_swagt;

//Christmas Parents definitions
static const CharFrame char_swagt_frame[] = {
	{swagt_ArcMain_Idle0, {0,   0, 100,140}, {130, 182}}, //0 idle 1
	{swagt_ArcMain_Idle1, {0,   0, 100,140}, {130, 182}}, //1 idle 2
	{swagt_ArcMain_Idle2, {0,   0, 100,140}, {130, 182}}, //2 idle 3
	{swagt_ArcMain_Idle3, {0,   0, 100,140}, {130, 182}}, //3 idle 4
	
	{swagt_ArcMain_LeftA0, {0,   0, 100,140}, {130, 182}}, //4 left a 1
	{swagt_ArcMain_LeftA1, {0,   0, 100,140}, {130, 182}}, //5 left a 2
	{swagt_ArcMain_LeftB0, {0,   0, 197, 200}, {122, 188}}, //6 left b 1
	{swagt_ArcMain_LeftB1, {0,   0, 201, 200}, {125, 188}}, //7 left b 2
	
	{swagt_ArcMain_DownA0, {0,   0, 100,140}, {130, 182}}, //8 down a 1
	{swagt_ArcMain_DownA1, {0,   0, 100,140}, {130, 182}}, //9 down a 2
	{swagt_ArcMain_DownB0, {0,   0, 213, 189}, {121, 177}}, //10 down b 1
	{swagt_ArcMain_DownB1, {0,   0, 213, 190}, {123, 178}}, //11 down b 2
	
	{swagt_ArcMain_UpA0, {0,   0, 100,140}, {130, 182}}, //12 up a 1
	{swagt_ArcMain_UpA1, {0,   0, 100,140}, {130, 182}}, //13 up a 2
	{swagt_ArcMain_UpB0, {0,   0, 197, 204}, {117, 191}}, //14 up b 1
	{swagt_ArcMain_UpB1, {0,   0, 201, 202}, {119, 190}}, //15 up b 2
	
	{swagt_ArcMain_RightA0, {0,   0, 100,140}, {130, 182}}, //16 right a 1
	{swagt_ArcMain_RightA1, {0,   0, 100,140}, {130, 182}}, //17 right a 2
	{swagt_ArcMain_RightB0, {0,   0, 239, 189}, {128, 176}}, //18 right b 1
	{swagt_ArcMain_RightB1, {0,   0, 239, 191}, {130, 178}}, //19 right b 2
};

static const Animation char_swagt_anim[CharAnim_Max] = {
	{2, (const u8[]){ 0,  1,  2,  3, ASCR_BACK, 1}}, //CharAnim_Idle
	{2, (const u8[]){ 4,  5, ASCR_BACK, 1}},         //CharAnim_Left
	{2, (const u8[]){ 6,  7, ASCR_BACK, 1}},         //CharAnim_LeftAlt
	{2, (const u8[]){ 8,  9, ASCR_BACK, 1}},         //CharAnim_Down
	{2, (const u8[]){10, 11, ASCR_BACK, 1}},         //CharAnim_DownAlt
	{2, (const u8[]){12, 13, ASCR_BACK, 1}},         //CharAnim_Up
	{2, (const u8[]){14, 15, ASCR_BACK, 1}},         //CharAnim_UpAlt
	{2, (const u8[]){16, 17, ASCR_BACK, 1}},         //CharAnim_Right
	{2, (const u8[]){18, 19, ASCR_BACK, 1}},         //CharAnim_RightAlt
};

//Christmas Parents functions
void Char_swagt_SetFrame(void *user, u8 frame)
{
	Char_swagt *this = (Char_swagt*)user;
	
	//Check if this is a new frame
	if (frame != this->frame)
	{
		//Check if new art shall be loaded
		const CharFrame *cframe = &char_swagt_frame[this->frame = frame];
		if (cframe->tex != this->tex_id)
			Gfx_LoadTex(&this->tex, this->arc_ptr[this->tex_id = cframe->tex], 0);
	}
}

void Char_swagt_Tick(Character *character)
{
	Char_swagt *this = (Char_swagt*)character;
	
	//Perform idle dance
	if ((character->pad_held & (INPUT_LEFT | INPUT_DOWN | INPUT_UP | INPUT_RIGHT)) == 0)
		Character_PerformIdle(character);
	
	//Animate and draw
	Animatable_Animate(&character->animatable, (void*)this, Char_swagt_SetFrame);
	Character_Draw(character, &this->tex, &char_swagt_frame[this->frame]);
}

void Char_swagt_SetAnim(Character *character, u8 anim)
{
	//Set animation
	Animatable_SetAnim(&character->animatable, anim);
	Character_CheckStartSing(character);
}

void Char_swagt_Free(Character *character)
{
	Char_swagt *this = (Char_swagt*)character;
	
	//Free art
	Mem_Free(this->arc_main);
}

Character *Char_swagt_New(fixed_t x, fixed_t y)
{
	//Allocate Christmas Parents object
	Char_swagt *this = Mem_Alloc(sizeof(Char_swagt));
	if (this == NULL)
	{
		sprintf(error_msg, "[Char_swagt_New] Failed to allocate Christmas Parents object");
		ErrorLock();
		return NULL;
	}
	
	//Initialize character
	this->character.tick = Char_swagt_Tick;
	this->character.set_anim = Char_swagt_SetAnim;
	this->character.free = Char_swagt_Free;
	
	Animatable_Init(&this->character.animatable, char_swagt_anim);
	Character_Init((Character*)this, x, y);
	
	//Set character information
	this->character.spec = 0;
	
	this->character.health_i = 1;
	
	this->character.focus_x = FIXED_DEC(-10,1);
	this->character.focus_y = FIXED_DEC(-140,1);
	this->character.focus_zoom = FIXED_DEC(1,1);
	
	//Load art
	this->arc_main = IO_Read("\\CHAR\\SWAGT.ARC;1");
	
	const char **pathp = (const char *[]){
		"idle0.tim",   //swagt_ArcMain_Idle0
		"idle1.tim",   //swagt_ArcMain_Idle1
		"idle2.tim",   //swagt_ArcMain_Idle2
		"idle3.tim",   //swagt_ArcMain_Idle3
		"lefta0.tim",  //swagt_ArcMain_LeftA0
		"lefta1.tim",  //swagt_ArcMain_LeftA1
		"leftb0.tim",  //swagt_ArcMain_LeftB0
		"leftb1.tim",  //swagt_ArcMain_LeftB1
		"downa0.tim",  //swagt_ArcMain_DownA0
		"downa1.tim",  //swagt_ArcMain_DownA1
		"downb0.tim",  //swagt_ArcMain_DownB0
		"downb1.tim",  //swagt_ArcMain_DownB1
		"upa0.tim",    //swagt_ArcMain_UpA0
		"upa1.tim",    //swagt_ArcMain_UpA1
		"upb0.tim",    //swagt_ArcMain_UpB0
		"upb1.tim",    //swagt_ArcMain_UpB1
		"righta0.tim", //swagt_ArcMain_RightA0
		"righta1.tim", //swagt_ArcMain_RightA1
		"rightb0.tim", //swagt_ArcMain_RightB0
		"rightb1.tim", //swagt_ArcMain_RightB1
		NULL
	};
	IO_Data *arc_ptr = this->arc_ptr;
	for (; *pathp != NULL; pathp++)
		*arc_ptr++ = Archive_Find(this->arc_main, *pathp);
	
	//Initialize render state
	this->tex_id = this->frame = 0xFF;
	
	return (Character*)this;
}
