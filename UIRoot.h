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

//SkTime
//class Action {
//public:
//	enum ActionType{
//		Blink,
//		MoveTo
//	};
//	virtual void StartAction(SkCanvas times, ActionType type) = 0;
//	virtual void StopAction(int nId) = 0;
//private:
//	SkCanvas runtimes;
//	int nActionId;
//};