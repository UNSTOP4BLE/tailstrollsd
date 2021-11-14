/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#include "swap.h"

#include "mem.h"
#include "stage.h"

//Swap functions
void Swap_Free(Swap *this)
{
	//Check if NULL
	if (this == NULL)
		return;
	
	//Free Swap
	this->free(this);
	Mem_Free(this);
}

void Swap_Init(Swap *this, fixed_t x, fixed_t y)
{
	//Perform common Swap initialization
	this->x = x;
	this->y = y;
	
	this->set_anim(this, SwapAnim_Idle);

	this->pad_held = 0;
}

void Swap_Draw(Swap *this, Gfx_Tex *tex, const SwapFrame *cframe)
{
	//Draw Swap
	fixed_t x = this->x - stage.camera.x - ((fixed_t)cframe->off[0] << FIXED_SHIFT);
	fixed_t y = this->y - stage.camera.y - ((fixed_t)cframe->off[1] << FIXED_SHIFT);
	
	RECT src = {cframe->src[0], cframe->src[1], cframe->src[2], cframe->src[3]};
	RECT_FIXED dst = {x, y, src.w << FIXED_SHIFT, src.h << FIXED_SHIFT};
	Stage_DrawTex(tex, &src, &dst, stage.camera.bzoom);
}

void Swap_CheckStartSing(Swap *this)
{
	//Update sing end if singing animation
	if (this->animatable.anim == SwapAnim_Left ||
	    this->animatable.anim == SwapAnim_LeftAlt ||
	    this->animatable.anim == SwapAnim_Down ||
	    this->animatable.anim == SwapAnim_DownAlt ||
	    this->animatable.anim == SwapAnim_Up ||
	    this->animatable.anim == SwapAnim_UpAlt ||
	    this->animatable.anim == SwapAnim_Right ||
	    this->animatable.anim == SwapAnim_RightAlt)
		this->sing_end = stage.note_scroll + (FIXED_DEC(12,1) << 2); //1 beat
}

void Swap_CheckStartSing2(Swap *this)
{
	//Update sing end if singing animation
	if (this->animatable.anim == SwapAnim_Left2 ||
	    this->animatable.anim == SwapAnim_LeftAlt2 ||
	    this->animatable.anim == SwapAnim_Down2 ||
	    this->animatable.anim == SwapAnim_DownAlt2 ||
	    this->animatable.anim == SwapAnim_Up2 ||
	    this->animatable.anim == SwapAnim_UpAlt2 ||
	    this->animatable.anim == SwapAnim_Right2 ||
	    this->animatable.anim == SwapAnim_RightAlt2 )
		this->sing_end = stage.note_scroll + (FIXED_DEC(12,1) << 2); //1 beat
}


void Swap_CheckEndSing(Swap *this)
{
	if ((this->animatable.anim == SwapAnim_Left ||
	     this->animatable.anim == SwapAnim_LeftAlt ||
	     this->animatable.anim == SwapAnim_Down ||
	     this->animatable.anim == SwapAnim_DownAlt ||
	     this->animatable.anim == SwapAnim_Up ||
	     this->animatable.anim == SwapAnim_UpAlt ||
	     this->animatable.anim == SwapAnim_Right ||
	     this->animatable.anim == SwapAnim_RightAlt) &&
	    stage.note_scroll >= this->sing_end)
		this->set_anim(this, SwapAnim_Idle);
}

void Swap_CheckEndSing2(Swap *this)
{
	if ((this->animatable.anim == SwapAnim_Left2 ||
	     this->animatable.anim == SwapAnim_LeftAlt2 ||
	     this->animatable.anim == SwapAnim_Down2 ||
	     this->animatable.anim == SwapAnim_DownAlt2 ||
	     this->animatable.anim == SwapAnim_Up2 ||
	     this->animatable.anim == SwapAnim_UpAlt2 ||
	     this->animatable.anim == SwapAnim_Right2 ||
	     this->animatable.anim == SwapAnim_RightAlt2) &&
	    stage.note_scroll >= this->sing_end)
		this->set_anim(this, SwapAnim_Idlealt);
}


void Swap_PerformIdle(Swap *this)
{
	Swap_CheckEndSing(this);
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		if (Animatable_Ended(&this->animatable) &&
		    (this->animatable.anim != SwapAnim_Left &&
		     this->animatable.anim != SwapAnim_LeftAlt &&
		     this->animatable.anim != SwapAnim_Down &&
		     this->animatable.anim != SwapAnim_DownAlt &&
		     this->animatable.anim != SwapAnim_Up &&
		     this->animatable.anim != SwapAnim_UpAlt &&
		     this->animatable.anim != SwapAnim_Right &&
		     this->animatable.anim != SwapAnim_RightAlt) &&
		    (stage.song_step & 0x7) == 0)
			this->set_anim(this, SwapAnim_Idle);
	}
}

void Swap_PerformIdleAlt(Swap *this)
{
	Swap_CheckEndSing2(this);
	if (stage.flag & STAGE_FLAG_JUST_STEP)
	{
		if (Animatable_Ended(&this->animatable) &&
		    (this->animatable.anim != SwapAnim_Left2 &&
		     this->animatable.anim != SwapAnim_LeftAlt2 &&
		     this->animatable.anim != SwapAnim_Down2 &&
		     this->animatable.anim != SwapAnim_DownAlt2 &&
		     this->animatable.anim != SwapAnim_Up2 &&
		     this->animatable.anim != SwapAnim_UpAlt2 &&
		     this->animatable.anim != SwapAnim_Right2 &&
		     this->animatable.anim != SwapAnim_RightAlt2) &&
		    (stage.song_step & 0x7) == 0)
			this->set_anim(this, SwapAnim_Idlealt);
	}
}
