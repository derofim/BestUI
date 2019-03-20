#pragma once

#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"

#include "SkSurface.h"
#include <vector>
#include "UIRoot.h"
#include "ScrollBar.h"


struct ScrollContentInfo {
	SkScalar width;
	SkScalar height;
	SkScalar offs_y;
	SkScalar preoffs_y;
	SkScalar offs_x;
	SkScalar preoffs_x;
};

class ScrollView :public UIWidget,public ScrollBarController {
public:

	
	ScrollView();

	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override;

	SkPoint ScrollViewToChildPoint(int x, int y) 
	{
		SkPoint point;
		point.set(x - GetSkRect().left(), y - GetSkRect().top());
		return point;
	}

//	void AddChild(char *pImagePath);
	void AddChild(UIWidget *pWidget);

	/*void JumpTop();
	void JumpBottom();
	void JumpLeft();
	void JumpRight();*/

	void SetContentSize(SkScalar width, SkScalar height);

	//void SetDirection(Direction nType);
	
	void RemoveAllChildWidget();

	void SetBackGroundColor(SkColor color);

	void InitOffset();

	void ScrollToPosition(ScrollBar* source, int position);

	// Customize the scrollbar design. ScrollView takes the ownership of the
    // specified ScrollBar. |horiz_sb| and |vert_sb| cannot be NULL.
    void SetHorizontalScrollBar(ScrollBar* horiz_sb);
    void SetVerticalScrollBar(ScrollBar* vert_sb);

   // Returns the horizontal/vertical scrollbar. This may return NULL.
    ScrollBar* GetHorizontalScrollBar() const { return hori_bar; }
    ScrollBar* GetVerticalScrollBar() const { return vert_bar; }

private:
	std::vector<sk_sp<SkImage>> imagelist;
	/*SkScalar offs;
	SkScalar hei;*/
	//Direction nDirectionType;
	ScrollContentInfo ContentInfo;

	std::vector<UIWidget *> childlist;
	SkColor background;

	ScrollBar *vert_bar;
	ScrollBar *hori_bar;

};