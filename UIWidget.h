#pragma once


#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"
#include "Action.h"

class Action;

extern  ActionManage *gActionManage;

class UIWidget;
typedef std::function<void(UIWidget *p)> CallBackFun;

class UIWidget {
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

private:
	//SkPoint point;
	ActionManage *pActionManage;
	SkRect rect;
	CallBackFun callbackf;
	int nTag;
	bool bShowWindow;
	SkScalar fDegress;
};