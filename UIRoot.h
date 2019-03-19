#pragma once
#include "UIWidget.h"
#include <vector>

#include "Action.h"
#include "GameTimer.h"
class Action;



class UIRoot:public UIShard{
public:
	//void AddWidget(UIWidget *pWidget,int nShowOrder=0);
	void DrawAllWidget(SkCanvas* canvas);
	void DrawWidget(SkCanvas* canvas,UIWidget *pWidget);

	void OnMouseDown(int x, int y);
	void OnMouseMove(int x, int y);
	void OnMouseUp(int x,int y);

	void OnMouseWheel(float delta, uint32_t modifiers);

private:
	//std::vector<UIWidget *> widgetlist;
};

