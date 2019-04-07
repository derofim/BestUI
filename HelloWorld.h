/*
* Copyright 2017 Google Inc.
*
* Use of this source code is governed by a BSD-style license that can be
* found in the LICENSE file.
*/

#ifndef HelloWorld_DEFINED
#define HelloWorld_DEFINED

#include "sk_app/Application.h"
#include "sk_app/Window.h"
#include "UIRoot.h"

class SkCanvas;

class HelloWorld : public sk_app::Application, sk_app::Window::Layer,public UIRoot {
public:
    HelloWorld(int argc, char** argv, void* platformData);
    ~HelloWorld() override;

    void onIdle() override;

    void onBackendCreated() override;
    void onPaint(SkCanvas* canvas) override;
    bool onChar(SkUnichar c, uint32_t modifiers) override;
	bool onMouse(int x, int y, sk_app::Window::InputState state, uint32_t modifiers) override;
	bool onKey(sk_app::Window::Key key, sk_app::Window::InputState state, uint32_t modifiers)  override;
	bool onMouseWheel(float delta, uint32_t modifiers) override;

	void ClickCallback(UIWidget *pWidget,MouseEvent ev);
	void TestTimer();
	void TestTimer2();
	void InitUi();
	void TestScrollView();
	void TestListView();
	void TestTextField();
private:
    void updateTitle();

    sk_app::Window* fWindow;
    sk_app::Window::BackendType fBackendType;

    SkScalar fRotationAngle;
	sk_sp<SkImage> image;
};

#endif
