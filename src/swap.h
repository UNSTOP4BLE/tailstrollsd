/*
  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.
*/

#ifndef _SWAP_H
#define _SWAP_H

#include "io.h"
#include "gfx.h"

#include "fixed.h"
#include "animation.h"

//Swap specs
typedef u8 CharSpec;
#define CHAR_SPEC_MISSANIM2 (2 << 0) //Has miss animations

//Swap enums
typedef enum
{
	SwapAnim_Idle,
	SwapAnim_Left,  SwapAnim_LeftAlt,
	SwapAnim_Down,  SwapAnim_DownAlt,
	SwapAnim_Up,    SwapAnim_UpAlt,
	SwapAnim_Right, SwapAnim_RightAlt,
	SwapAnim_Left2,  SwapAnim_LeftAlt2,
	SwapAnim_Down2,  SwapAnim_DownAlt2,
	SwapAnim_Up2,    SwapAnim_UpAlt2,
	SwapAnim_Right2, SwapAnim_RightAlt2,
	SwapAnim_Idlealt,  SwapAnim_Idleb,
SwapAnim_Idlec,
	
	SwapAnim_Max //Max standard/shared animation
} SwapAnim;

//Swap structures
typedef struct
{
	u8 tex;
	u16 src[4];
	s16 off[2];
} SwapFrame;

typedef struct Swap
{
	//Swap functions
	void (*tick)(struct Swap*);
	void (*set_anim)(struct Swap*, u8);
	void (*free)(struct Swap*);
	
	//Position
	fixed_t x, y;
	
	//Swap information
	CharSpec spec;
	u8 health_i; //hud1.tim
	fixed_t focus_x, focus_y, focus_zoom;
	
	//Animation state
	Animatable animatable;
	fixed_t sing_end;
	u16 pad_held;
} Swap;

//Swap functions
void Swap_Free(Swap *this);
void Swap_Init(Swap *this, fixed_t x, fixed_t y);
void Swap_Draw(Swap *this, Gfx_Tex *tex, const SwapFrame *cframe);

void Swap_CheckStartSing(Swap *this);
void Swap_CheckStartSing2(Swap *this);
void Swap_CheckEndSing(Swap *this);
void Swap_CheckEndSing2(Swap *this);
void Swap_PerformIdle(Swap *this);
void Swap_PerformIdleAlt(Swap *this);

#endif
