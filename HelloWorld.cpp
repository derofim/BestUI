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
#include "button.h"
using namespace sk_app;

Application* Application::Create(int argc, char** argv, void* platformData) {
	return new HelloWorld(argc, argv, platformData);

	//std::max();
}

void HelloWorld::ClickCallback(UIWidget *pWidget)
{
	int a;
	a = 5;
}

HelloWorld::HelloWorld(int argc, char** argv, void* platformData)
	: fBackendType(Window::kNativeGL_BackendType), fRotationAngle(0) {
	SkGraphics::Init();

	fWindow = Window::CreateNativeWindow(platformData);
	fWindow->setRequestedDisplayParams(DisplayParams());

	// register callbacks
	fWindow->pushLayer(this);

	fWindow->attach(fBackendType);

	Button *but = new Button("a.png", "b.png", "c.png");
	but->SetPosition(300, 300);
	this->AddWidget(but);

	but->SetUiEventCallBack(std::bind(&HelloWorld::ClickCallback, this, std::placeholders::_1));
	
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