#pragma once


#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"
#include "Action.h"
#include "GameTimer.h"

class Action;

extern  ActionManage *gActionManage;

class UIWidget;
typedef std::function<void(UIWidget *p)> CallBackFun;



extern  ActionManage *gActionManage;
extern  GameTimerManage *gTimerManage;
extern std::vector<UIWidget *> gWidgetList;
class UIShard {
public:
	UIShard();
	void RunAction(Action *type);
	void AddWidget(UIWidget *pWidget, int nShowOrder=0);
	void SetTimer(TimerCallBackFun fun, double fElapse);
	void KillTimer(TimerCallBackFun fun);
	ActionManage *GetActionManage()
	{
		return pActionManage;
	}
	GameTimerManage *GetTimerManage()
	{
		return pTimerManage;
	}
	std::vector<UIWidget *> * GetWidgetList()
	{
		return pWidgetList;
	}
private:
	ActionManage *pActionManage;
	GameTimerManage *pTimerManage;
	std::vector<UIWidget *> *pWidgetList;
};

class UIWidget :public UIShard{
public:
	UIWidget();
	int nShowOrder;
	virtual void Draw(SkCanvas* canvas)=0;
	void SetPosition(SkScalar x, SkScalar y);
	void SetSize(SkScalar width, SkScalar height);
	void SetRect(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom);

	void RunAction(Action *type);

//	void RunSequence(ActCallBackFun callback, ...);

	virtual void OnMouseDown(int x, int y)=0;
	virtual void OnMouseMove(int x, int y)=0;

	void SetUiEventCallBack(CallBackFun fu);
	SkScalar GetWidth()
	{
		return rect.width();
	}
	SkScalar GetHeight()
	{
		return rect.height();
	}

	SkRect GetSkRect()
	{
		return rect;
	}

	CallBackFun GetMouseDownCallBack()
	{
		return callbackf;
	}

	void SetTag(int nSet)
	{
		nTag = nSet;
	}
	int GetTag()
	{
		return nTag;
	}
	bool IsVisible()
	{
		return bShowWindow;
	}
	void SetVisible(bool bVisible)
	{
		bShowWindow = bVisible;
	}

	SkScalar GetDegress()
	{
		return fDegress;
	}
	void SetDegress(SkScalar degress)
	{
		fDegress = degress;
	}

	SkScalar GetScrolloffsY()
	{
		return fScrolloffsY;
	}

	void SetScrolloffsY(SkScalar offs)
	{
		fScrolloffsY = offs;
	}

	SkScalar GetScrolloffsX()
	{
		return fScrolloffsX;
	}

	void SetScrolloffsX(SkScalar offs)
	{
		fScrolloffsX = offs;
	}

private:
	//SkPoint point;
	SkRect rect;
	CallBackFun callbackf;
	int nTag;
	bool bShowWindow;
	SkScalar fDegress;

	SkScalar fScrolloffsY;
	SkScalar fScrolloffsX;
};