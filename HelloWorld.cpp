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
using namespace sk_app;


 ActionManage *gActionManage;

Application* Application::Create(int argc, char** argv, void* platformData) {


	return new HelloWorld(argc, argv, platformData);

	//std::max();

}

void HelloWorld::ClickCallback(UIWidget *pWidget)
{
	Button *but =(Button *) pWidget;
	const char *pBuf = but->GetText().c_str();
	printf("%s\n", pBuf);
	int a;
	a = 5;
}
#include "windows.h"

HelloWorld::HelloWorld(int argc, char** argv, void* platformData)
	: fBackendType(Window::kNativeGL_BackendType), fRotationAngle(0) {
	SkGraphics::Init();

	/*AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);*/

	gActionManage = new ActionManage();

	fWindow = Window::CreateNativeWindow(platformData);
	fWindow->setRequestedDisplayParams(DisplayParams());

	// register callbacks
	fWindow->pushLayer(this);

	fWindow->attach(fBackendType);

	

	

	char pszTest[32][32] = { "very goods","hello world","miss","SogouWBIpunt","skscalar","button","client","press","oleacc","winine.dll"};

	
	/*Button *but = new Button();
	but->SetText(pszTest[0]);
	but->SetSize(80, 17);
	but->SetPosition(100, 100);
	this->AddWidget(but);
	but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this, std::placeholders::_1));*/
	
	//ScrollView *sview = new ScrollView();
	//char pszPath[256];
	//

	//for (int k = 0; k <10; k++)
	//{
	//	Button *but = new Button();
	//	but->SetText(pszTest[k]);
	//	but->SetSize(100, 25);
	//	sview->AddChild(but);
	//	but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this, std::placeholders::_1));
	//	
	//}

	//sview->SetDirection(ScrollView::Direction::Horizontal);
	//sview->SetPosition(100, 200);
	//sview->SetSize(380,30);
	////sview->JumpBottom();
	//this->AddWidget(sview);

	char pszPath[256];
	sprintf_s(pszPath, 256, "H:\\0.png");
	Sprite *p = new Sprite(pszPath);
	this->AddWidget(p);
	p->SetPosition(100, 100);
	Blink *pBlink = new Blink(5, 5);
	p->RunAction(pBlink);
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
	return true;
}