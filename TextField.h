#pragma once

#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"

class TextField :public UIWidget
{
public:
	TextField();
	~TextField();

	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override;
	void OnKey(sk_app::Window::Key key, uint32_t modifiers) override;
private:
};