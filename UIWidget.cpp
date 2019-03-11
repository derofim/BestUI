#include "UIWidget.h"

UIWidget::UIWidget()
{
	callbackf = NULL;
	SetVisible(true);
	pActionManage = gActionManage;
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
	pActionManage->AddAction(type, this);
}

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