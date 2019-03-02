#pragma once
#include "UIWidget.h"
#include <vector>
class UIRoot {
public:
	void AddWidget(UIWidget *pWidget,int nShowOrder=0);
	void DrawAllWidget(SkCanvas* canvas);
	void DrawWidget(SkCanvas* canvas,UIWidget *pWidget);

	void OnMouseDown(int x, int y);
	void OnMouseMove(int x, int y);
private:
	std::vector<UIWidget *> widgetlist;
};