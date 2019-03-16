#include "UIWidget.h"

UIWidget::UIWidget()
{
	callbackf = NULL;
	SetVisible(true);
	fDegress = 0;
	SetScrolloffsX(0);
	SetScrolloffsY(0);
}
void UIWidget::SetPosition(SkScalar x, SkScalar y)
{
	rect.set(x,y,x+GetWidth(),y+GetHeight());
	
}
void UIWidget::SetSize(SkScalar width, SkScalar height)
{
	rect.set(rect.left(), rect.top(), width + rect.left(), height + rect.top());
}

void UIWidget::SetRect(SkScalar left, SkScalar top, SkScalar right, SkScalar bottom)
{
	rect.set(left, top, right, bottom);
}


void UIWidget::RunAction(Action *type)
{
	GetActionManage()->AddAction(type, this);
}

//void UIWidget::RunSequence(ActCallBackFun callback, ...)
//{
//	Sequence(this, callback);
//}

void UIWidget::SetUiEventCallBack(CallBackFun fu)
{
	callbackf = fu;
}

//void UIWidget::RunAction(Action *type)
//{
//
//}

//void UIWidget::OnMouseDown(int x, int y)
//{
//	if(callbackf!=NULL)
//	 callbackf(this);
//}

//void UIWidget::OnMouseMove(int x, int y)
//{
//	OnMouseMove(x, y);
//}


UIShard::UIShard()
{
	pActionManage = gActionManage;
	pTimerManage = gTimerManage;
	pWidgetList = &gWidgetList;
}

void UIShard::RunAction(Action *type)
{
	pActionManage->AddAction(type, 0);
}

void UIShard::AddWidget(UIWidget *pWidget, int nShowOrder)
{
	pWidget->nShowOrder = nShowOrder;
	pWidgetList->push_back(pWidget);
}

void UIShard::SetTimer(TimerCallBackFun fun, double fElapse)
{
	pTimerManage->SetTimer(this, fun, fElapse);
}

void UIShard::KillTimer(TimerCallBackFun fun)
{
	pTimerManage->KillTimer(fun);
}