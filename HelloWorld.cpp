/*
 * Copyright 2017 Google Inc.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 */

#include "HelloWorld.h"

#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"

#include "SkDashPathEffect.h"
#include "SkTime.h"
#include "Button.h"
#include "ScrollView.h"
#include "Sprite.h"
#include "Action.h"
#include "RollImage.h"
#include "ListView.h"

using namespace sk_app;


 ActionManage *gActionManage;
 GameTimerManage *gTimerManage;
 std::vector<UIWidget *> gWidgetList;
Application* Application::Create(int argc, char** argv, void* platformData) {

	gActionManage = new ActionManage();
	gTimerManage = new GameTimerManage();
	gWidgetList.clear();
	return new HelloWorld(argc, argv, platformData);

	//std::max();

}

void HelloWorld::ClickCallback(UIWidget *pWidget,MouseEvent ev)
{
	Button *but =(Button *) pWidget;
	const char *pBuf = but->GetText().c_str();
	printf("%s\n", pBuf);
	int a;
	a = 5;
}
#include "windows.h"

void HelloWorld::TestTimer()
{
	printf("TestTimer %f\n", SkTime::GetMSecs());
	
}

void HelloWorld::TestTimer2()
{
	printf("TestTimer2 %f\n", SkTime::GetMSecs());
	//this->KillTimer(timer_sel(HelloWorld::TestTimer));
}



void HelloWorld::TestScrollView()
{
	char pszTest[32][32] = { "very goods","hello world","miss","SogouWBIpunt","skscalar","button","client","press","oleacc","winine.dll"};
	ScrollView *sview = new ScrollView();
	char pszPath[256];
	
	int nLine=100000;
	for (int j = 0; j <nLine; j++)
	{
		for (int k = 0; k < 3; k++)
		{
			StaticText *text;
			if (k == 0)
			{
				sprintf_s(pszPath,256,"%d",j+1);
				text = new StaticText(pszPath);
			}
			else
			   text = new StaticText(pszTest[k]);
			//but->SetText(pszTest[k]);
			text->SetSize(100, 25);
			text->SetPosition(100*k,j*25);
			sview->AddChild(text);
		}
		//but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this, std::placeholders::_1));
		
	}

	//sview->SetDirection(ScrollView::Direction::Horizontal);
	sview->SetPosition(100, 200);
	sview->SetSize(580,580);
	sview->SetContentSize(1024,nLine*25);
	//sview->JumpBottom();
	this->AddWidget(sview);
}
void HelloWorld::TestListView()
{
	char pszTest[32][32] = { "very goods","hello world","miss","SogouWBIpunt","skscalar","button","client","press","oleacc","winine.dll"};

	char pszTitel[32][32] = { "num","probetcd_helloworld","memory","pid","cpu","username","statues","spec","odbc","myserver"};
	ListView *view = new ListView();
	for(int k=0;k<11;k++)
		view->AddCol(pszTitel[k],100);
	char pszPath[256];
	
	int nLine=1000;
	for (int j = 0; j <nLine; j++)
	{
		for (int k = 0; k < 11; k++)
		{
			StaticText *text;
			if (k == 0)
			{
				sprintf_s(pszPath,256,"%d",j+1);
				text = new StaticText(pszPath);
			}
			else
			   text = new StaticText(pszTest[k]);
			
			view->AddCellItem(text,j,k);
		}
		//but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this, std::placeholders::_1));
		
	}

	//sview->SetDirection(ScrollView::Direction::Horizontal);
	view->SetPosition(100, 100);
	view->SetSize(500,500);
	view->SetViewStyle(LIST_STYLE_HEAD);
	//sview->JumpBottom();
	this->AddWidget(view);
}

HelloWorld::HelloWorld(int argc, char** argv, void* platformData)
	: fBackendType(Window::kNativeGL_BackendType), fRotationAngle(0) {
	SkGraphics::Init();

	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);

	

	fWindow = Window::CreateNativeWindow(platformData);
	fWindow->setRequestedDisplayParams(DisplayParams());

	// register callbacks
	fWindow->pushLayer(this);

	fWindow->attach(fBackendType);

	/*InitUi();
	return;*/
	
	//this->SetTimer(std::bind(&HelloWorld::TestTimer, this), 10);
	//TimerCallBackFun p2 = static_cast<TimerCallBackFun>(&HelloWorld::TestTimer);
	/*this->SetTimer(timer_sel(HelloWorld::TestTimer), 2);
	this->SetTimer(timer_sel(HelloWorld::TestTimer2), 5);*/
	
	TestListView();
	//TestScrollView();
	return;

	/*char pszPath[256];
	sprintf_s(pszPath, 256, "G:\\0.png");
	Sprite *p = new Sprite(pszPath);
	this->AddWidget(p);
	p->SetPosition(300, 300);*/

	//p->SetAnchorPoint(SkPoint::Make(0.8, 0.8));
	//RotateTo *ro = new RotateTo(2, 360);
	//p->RunAction(ro);


	//Repeat re = Repeat(p, ro, 3, 0);
	/*MoveTo *to = new MoveTo(2, 800, 500);
	p->RunAction(to);*/
	//p->RunAction(new DelayTime(10.2, [&](void){
	//	  printf("kkkkk\n");
	//	//  this->KillTimer(timer_sel(HelloWorld::TestTimer));
	//   }
	//));

	//this->RunAction(new DelayTime(18, [&](void) {
	//	printf("jjjjj\n");
	//	this->KillTimer(timer_sel(HelloWorld::TestTimer2));
	// }
	//));


	//new Sequence(p, de, pBlink, 0);

	//Blink *pBlink = new Blink(2, 2);
	//DelayTime *de = new DelayTime(2);

	//Sequence se =  Sequence(p, [&](void) {
	//	printf("kkkkk222222\n");
	//	this->KillTimer(timer_sel(HelloWorld::TestTimer));
	//},pBlink,de,0);

	//Sequence se=Sequence(p,0,ro, de, pBlink,0);
   // Repeat re = Repeat(p, &se, 3, 0);
//	se.RunSequence();

	
	//p->RunAction(pBlink);

	//
	//p->RunAction(de);
	//p->SetOpacity(0.2);
	//p->SetScaleY(3);



	
	
}

HelloWorld::~HelloWorld() {
	fWindow->detach();
	delete fWindow;
}

void HelloWorld::updateTitle() {
	if (!fWindow || fWindow->sampleCount() <= 1) {
		return;
	}

	SkString title("Hello World ");
	title.append(Window::kRaster_BackendType == fBackendType ? "Raster" : "OpenGL");
	fWindow->setTitle(title.c_str());
}

void HelloWorld::onBackendCreated() {
	this->updateTitle();
	fWindow->show();
	fWindow->inval();
}

void HelloWorld::onPaint(SkCanvas* canvas) {
	canvas->clear(SK_ColorWHITE);

	DrawAllWidget(canvas);
	return;

}

void HelloWorld::onIdle() {
	// Just re-paint continously
	fWindow->inval();
}

bool HelloWorld::onChar(SkUnichar c, uint32_t modifiers) {
	if (' ' == c) {
		fBackendType = Window::kRaster_BackendType == fBackendType ? Window::kNativeGL_BackendType
			: Window::kRaster_BackendType;
		fWindow->detach();
		fWindow->attach(fBackendType);
	}
	return true;
}


bool HelloWorld::onMouse(int x, int y, Window::InputState state, uint32_t modifiers)
{
	if (Window::kDown_InputState == state)
	{
		OnMouseDown(x,y);
	}
	else if(Window::kMove_InputState==state)
	{
		OnMouseMove(x, y);
	}
	else if (Window::kUp_InputState == state)
	{
		OnMouseUp(x,y);
	}
	return true;
}

bool HelloWorld::onMouseWheel(float delta, uint32_t modifiers)
{
	OnMouseWheel(delta,modifiers);
	return true;
}