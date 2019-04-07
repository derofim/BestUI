#pragma once

#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "ScrollView.h"
struct TextDesc
{
	SkColor color;
	SkUnichar  text;
};

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
	void OnChar(SkUnichar c, uint32_t modifiers) override;
private:
	std::vector<TextDesc> textlist;
	SkColor text_color;
};