//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#ifndef _GAMEAPP_H_
#define _GAMEAPP_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h> 

#include <JApp.h>
#include <JRenderer.h>

#include <chipmunk.h>

//-------------------------------------------------------------------------------------
// Enums
//-------------------------------------------------------------------------------------
typedef enum {arFS, arWS} AspectRatio;

//-------------------------------------------------------------------------------------
// Classes
//-------------------------------------------------------------------------------------
class GameApp:	public JApp
{
	
public:
	GameApp();
	virtual ~GameApp();
	virtual void Create();
	virtual void Destroy();
	virtual void Update();
	virtual void Render();
	virtual void Pause();
	virtual void Resume();

private:
	int demo_index;
	int ticks;
	int iAspectRatio;
	int iAutoPlay;
	int iScreenNum;
	bool bDoScreenCap;

	float fXOffset1, fXOffset2, fXRatio;
	float fYOffset1, fYOffset2, fYRatio;

	void setAspectRatio();
	void drawCircle(cpFloat x, cpFloat y, cpFloat r, cpFloat a, JRenderer *renderer);
	void drawCircleShape(cpShape *shape, JRenderer *renderer);
	void drawSegmentShape(cpShape *shape, JRenderer *renderer);
	void drawPolyShape(cpShape *shape, JRenderer *renderer);
	void drawObject(void *ptr, JRenderer *renderer);
	void myHashSetEach(cpHashSet *set, JRenderer *renderer);
};

#endif
