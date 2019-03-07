#include "UIRoot.h"
#include <algorithm>
using namespace std;

void UIRoot::AddWidget(UIWidget *pWidget, int nShowOrder)
{
	pWidget->nShowOrder = nShowOrder;
	widgetlist.push_back(pWidget);
}

void UIRoot::DrawAllWidget(SkCanvas* canvas)
{
	sort(widgetlist.begin(), widgetlist.end(), [](UIWidget *x, UIWidget *y) {return x->nShowOrder < y->nShowOrder; });

	for (auto iter = widgetlist.begin(); iter != widgetlist.end(); iter++)
	{
		DrawWidget(canvas,*iter);
	}

}

void UIRoot::DrawWidget(SkCanvas* canvas,UIWidget *pWidget)
{
	if(pWidget->IsVisible())
	   pWidget->Draw(canvas);
}


void UIRoot::OnMouseDown(int x, int y)
{
	if (widgetlist.size() == 0)
		return;
	for (auto iter = widgetlist.end()-1; ;iter--)
	{
		UIWidget *pWidget = *iter;
		if (x >= pWidget->GetSkRect().left() && x <= pWidget->GetSkRect().right() && y >= pWidget->GetSkRect().top() && y <= pWidget->GetSkRect().bottom() && pWidget->IsVisible())
		{
			pWidget->OnMouseDown(x, y);
			return;
		}
		if (iter == widgetlist.begin())
			break;
	}
}


void  UIRoot::OnMouseMove(int x, int y)
{
	if (widgetlist.size() == 0)
		return;
	for (auto iter = widgetlist.end() - 1; ; iter--)
	{
		UIWidget *pWidget = *iter;
		pWidget->OnMouseMove(x, y);
		if (x >= pWidget->GetSkRect().left() && x <= pWidget->GetSkRect().right() && y >= pWidget->GetSkRect().top() && y <= pWidget->GetSkRect().bottom())
			return;
		if (iter == widgetlist.begin())
			break;
	}
}