//-------------------------------------------------------------------------------------
//
// JGE++ is a hardware accelerated 2D game SDK for PSP/Windows.
//
// Licensed under the BSD license, see LICENSE in JGE root for details.
// 
// Copyright (c) 2007 James Hui (a.k.a. Dr.Watson) <jhkhui@gmail.com>
// 
//-------------------------------------------------------------------------------------

#include <stdio.h>

#include <JGE.h>
#include <JRenderer.h>
#include <JLBFont.h>

#include "GameApp.h"

//-------------------------------------------------------------------------------------
// Globals
//-------------------------------------------------------------------------------------
cpSpace *space;
cpBody *staticBody;

#ifndef WIN32
#ifdef __cplusplus
extern "C" {
#endif
#endif

extern void demo1_init(void);
extern void demo1_update(int);

extern void demo2_init(void);
extern void demo2_update(int);

extern void demo3_init(void);
extern void demo3_update(int);

extern void demo4_init(void);
extern void demo4_update(int);

extern void demo5_init(void);
extern void demo5_update(int);

extern void demo6_init(void);
extern void demo6_update(int);

extern void demo7_init(void);
extern void demo7_update(int);

typedef void (*demo_init_func)(void);
typedef void (*demo_update_func)(int);
typedef void (*demo_destroy_func)(void); 

#ifndef WIN32
#ifdef __cplusplus
}
#endif
#endif

#ifndef WIN32
demo_init_func __attribute__((aligned(16))) init_funcs[] = {
#else
demo_init_func init_funcs[] = {
#endif
	demo1_init,
	demo2_init,
	demo3_init,
	demo4_init,
	demo5_init,
	demo6_init,
	demo7_init,
};

#ifndef WIN32
demo_update_func __attribute__((aligned(16))) update_funcs[] = {
#else
demo_update_func update_funcs[] = {
#endif
	demo1_update,
	demo2_update,
	demo3_update,
	demo4_update,
	demo5_update,
	demo6_update,
	demo7_update,
};

void demo_destroy(void);

#ifndef WIN32
demo_destroy_func __attribute__((aligned(16))) destroy_funcs[] = {
#else
demo_destroy_func destroy_funcs[] = {
#endif
	demo_destroy,
	demo_destroy,
	demo_destroy,
	demo_destroy,
	demo_destroy,
	demo_destroy,
	demo_destroy,
};

void demo_destroy(void)
{
	cpSpaceFreeChildren(space);
	cpSpaceFree(space);

	cpBodyFree(staticBody);
}

//-------------------------------------------------------------------------------------
// Constructor. Variables can be initialized here.
//
//-------------------------------------------------------------------------------------
GameApp::GameApp()
{
	demo_index = 0;
	ticks = 0;
	iAspectRatio = arWS;
	setAspectRatio();
	iAutoPlay = 1;
	bDoScreenCap = false;
	iScreenNum = 0;
}


//-------------------------------------------------------------------------------------
// Destructor.
//
//-------------------------------------------------------------------------------------
GameApp::~GameApp()
{

}

//-------------------------------------------------------------------------------------
// This is the init callback function. You should load and create your in-game 
// resources here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Create()
{
	JRenderer* renderer = JRenderer::GetInstance();

	renderer->SetFOV(60.0f);

	cpInitChipmunk();
	init_funcs[demo_index]();
}


//-------------------------------------------------------------------------------------
// This is the clean up callback function. You should delete all your in-game 
// resources, for example texture and quads, here.
// 
//-------------------------------------------------------------------------------------
void GameApp::Destroy()
{

}


//-------------------------------------------------------------------------------------
// This is the update callback function and is called at each update frame
// before rendering. You should update the game logic here.
//
//-------------------------------------------------------------------------------------
void GameApp::Update()
{

	JGE* engine = JGE::GetInstance();

	// toggle bDoScrenCap when TRIANGLE is pressed
	if (engine->GetButtonClick(PSP_CTRL_TRIANGLE))		
	{
		bDoScreenCap = !bDoScreenCap;
	}
	if(bDoScreenCap)
	{
#ifndef WIN32
		char __attribute__((aligned(16))) s[80];
#else
		char s[80];
#endif

		// save screen shot to root of Memory Stick 
		sprintf(s, "s%05i.png", iScreenNum);				
		JRenderer::GetInstance()->ScreenShot(s);
		iScreenNum++;
	}

	float dt = engine->GetDelta();		// Get time elapsed since last update.

	//
	// Your updating code here...
	//
	ticks++;
	if(iAutoPlay == 1)
	{
		update_funcs[demo_index](ticks);
	}

	if(engine->GetButtonState(PSP_CTRL_RIGHT))
	{
		update_funcs[demo_index](ticks);
	}
	if(engine->GetButtonClick(PSP_CTRL_LEFT))
	{
		update_funcs[demo_index](ticks);
	}
	if(engine->GetButtonClick(PSP_CTRL_DOWN))
	{
		iAutoPlay = (iAutoPlay == 1) ? 0 : 1;
	}
	if(engine->GetButtonClick(PSP_CTRL_SELECT))
	{
		iAspectRatio = (iAspectRatio == arWS) ? arFS : arWS;
		setAspectRatio();
	}

	if(engine->GetButtonClick(PSP_CTRL_LTRIGGER))
	{
		destroy_funcs[demo_index]();
		if(demo_index > 0) { demo_index--; }
		ticks = 0;
		init_funcs[demo_index]();
	}

	if(engine->GetButtonClick(PSP_CTRL_RTRIGGER))
	{
		destroy_funcs[demo_index]();
		if(demo_index < 6) { demo_index++; }
		ticks = 0;
		init_funcs[demo_index]();
	}

	if(engine->GetButtonClick(PSP_CTRL_START))
	{
		destroy_funcs[demo_index]();
		ticks = 0;
		init_funcs[demo_index]();
	}
}

//-------------------------------------------------------------------------------------
// Draw a Circle
// 
//-------------------------------------------------------------------------------------
void GameApp::drawCircle(cpFloat x, cpFloat y, cpFloat r, cpFloat a, JRenderer *renderer)
{
	const int segs = 15;
	const cpFloat coef = 2.0*M_PI/(cpFloat)segs;

#ifndef WIN32
	float __attribute__((aligned(16))) xVerts[16];
	float __attribute__((aligned(16))) yVerts[16];
#else
	float xVerts[16];
	float yVerts[16];
#endif

	for(int n = 0; n <= segs; n++)
	{
		cpFloat rads = n*coef;
		xVerts[segs-n] = (r*cos(rads + a) + x + fXOffset1)*fXRatio + fXOffset2;
		yVerts[segs-n] = r*sin(rads + a) + y;
	}
	for(int n = 0; n <=segs; n++)
	{
		yVerts[n] = (-yVerts[n] + fYOffset1)*fYRatio + fYOffset2;
	}
	renderer->DrawPolygon(xVerts, yVerts, segs, ARGB(255,0,0,0));
}

//-------------------------------------------------------------------------------------
// Draw a Circle
// 
//-------------------------------------------------------------------------------------
void GameApp::drawCircleShape(cpShape *shape, JRenderer *renderer)
{
	cpBody *body = shape->body;
	cpCircleShape *circle = (cpCircleShape *)shape;
	cpVect c = cpvadd(body->p, cpvrotate(circle->c, body->rot));
	drawCircle(c.x, c.y, circle->r, body->a, renderer);
}

//-------------------------------------------------------------------------------------
// Draw a Segment (just a line)
// 
//-------------------------------------------------------------------------------------
void GameApp::drawSegmentShape(cpShape *shape, JRenderer *renderer)
{
	cpBody *body = shape->body;
	cpSegmentShape *seg = (cpSegmentShape *)shape;
	cpVect a = cpvadd(body->p, cpvrotate(seg->a, body->rot));
	cpVect b = cpvadd(body->p, cpvrotate(seg->b, body->rot));

	renderer->DrawLine((a.x+fXOffset1)*fXRatio+fXOffset2, (-a.y+fYOffset1)*fYRatio, (b.x+fXOffset1)*fXRatio+fXOffset2, (-b.y+fYOffset1)*fYRatio, 1, ARGB(255,0,0,0));
}

//-------------------------------------------------------------------------------------
// Draw a Polygon
// 
//-------------------------------------------------------------------------------------
void GameApp::drawPolyShape(cpShape *shape, JRenderer *renderer)
{
#ifndef WIN32
	float __attribute__((aligned(16))) xVerts[255];
	float __attribute__((aligned(16))) yVerts[255];
#else
	float xVerts[255];
	float yVerts[255];
#endif

	cpBody *body = shape->body;
	cpPolyShape *poly = (cpPolyShape *)shape;
	cpVect v;

	int num = poly->numVerts;
	cpVect *verts = poly->verts;

	for(int i=0; i<num; i++){
		v = cpvadd(body->p, cpvrotate(verts[i], body->rot));
		xVerts[i] = v.x+fXOffset1;
		yVerts[i] = -v.y+fYOffset1;

		xVerts[i] = xVerts[i]*fXRatio+fXOffset2;
		yVerts[i] = yVerts[i]*fYRatio;
	}

	renderer->DrawPolygon(xVerts, yVerts, num, ARGB(255,0,0,0));
}

//-------------------------------------------------------------------------------------
// drawObject
// 
//-------------------------------------------------------------------------------------
void GameApp::drawObject(void *ptr, JRenderer *renderer)
{
	cpShape *shape = (cpShape *)ptr;
	switch(shape->type){
		case CP_CIRCLE_SHAPE:
			drawCircleShape(shape, renderer);
			break;
		case CP_SEGMENT_SHAPE:
			drawSegmentShape(shape, renderer);
			break;
		case CP_POLY_SHAPE:
			drawPolyShape(shape, renderer);
			break;
		default:
			printf("Bad enumeration in drawObject().\n");
	}
}

//-------------------------------------------------------------------------------------
// All rendering operations should be done in Render() only.
// 
//-------------------------------------------------------------------------------------
void GameApp::Render()
{

	// get JRenderer instance
	JRenderer* renderer = JRenderer::GetInstance();

	// clear screen to black
	renderer->ClearScreen(ARGB(255,255,255,255));

	//
	// Your rendering code here...
	//
	renderer->Enable2D();
	myHashSetEach(space->activeShapes->handleSet, renderer);
	myHashSetEach(space->staticShapes->handleSet, renderer);

	cpArray *bodies = space->bodies;
	int num = bodies->num;
	for(int i=0; i<num; i++){
		cpBody *body = (cpBody *)bodies->arr[i];
		renderer->DrawRect((body->p.x+fXOffset1)*fXRatio+fXOffset2, (-body->p.y+fYOffset1)*fYRatio, 1, 1, ARGB(255,0,0,255));
	}

	for(int i=0; i < space->arbiters->num; i++)
	{
		cpArbiter *arb = (cpArbiter *)space->arbiters->arr[i];
		for(int j=0; j < arb->numContacts; j++)
		{
			cpVect v = arb->contacts[j].p;
			renderer->DrawRect((v.x+fXOffset1)*fXRatio+fXOffset2, (-v.y+fYOffset1)*fYRatio, 1, 1, ARGB(255,255,0,0));
		}
	}
}

void GameApp::myHashSetEach(cpHashSet *set, JRenderer *renderer)
{
	int i;

	for(i=0; i<set->size; i++){
		cpHashSetBin *bin;
		for(bin = set->table[i]; bin; bin = bin->next)
		{
			cpHandle *hand = (cpHandle *) bin->elt;
			drawObject(hand->obj, renderer);
		}
	}
} 

//-------------------------------------------------------------------------------------
// This function is called when the system wants to pause the game. You can set a flag
// here to stop the update loop and audio playback.
//
//-------------------------------------------------------------------------------------
void GameApp::Pause()
{
	
}


//-------------------------------------------------------------------------------------
// This function is called when the game returns from the pause state.
//
//-------------------------------------------------------------------------------------
void GameApp::Resume()
{
	
}

//-------------------------------------------------------------------------------------
// Set Aspect Ratio
//
//-------------------------------------------------------------------------------------
void GameApp::setAspectRatio()
{
	if(iAspectRatio == arWS)
	{
		fXOffset1 = 320;
		fXOffset2 = 0;
		fXRatio = 0.75f;
		fYOffset1 = 240;
		fYOffset2 = 0;
		fYRatio = 0.566666666f;
	}
	if(iAspectRatio == arFS)
	{
		fXOffset1 = 320;
		fXOffset2 = 56;
		fXRatio = 0.566666666f;
		fYOffset1 = 240;
		fYOffset2 = 0;
		fYRatio = 0.566666666f;
	}
}