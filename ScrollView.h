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
//#include "button.h"

//SkSurface

struct ScrollContentInfo {
	SkScalar width;
	SkScalar height;
	SkScalar offs_y;
	SkScalar preoffs_y;
	SkScalar offs_x;
	SkScalar preoffs_x;
};

class ScrollView :public UIWidget {
public:

	enum Direction {
		Vertical,
		Horizontal
	};
	ScrollView();

	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;

//	void AddChild(char *pImagePath);
	void AddChild(UIWidget *pWidget);

	void JumpTop();
	void JumpBottom();
	void JumpLeft();
	void JumpRight();

	void SetContentSize(SkScalar width, SkScalar height);

	void SetDirection(Direction nType);
	
	void RemoveAllChildWidget();

	void SetBackGroundColor(SkColor color);

	void InitOffset();

private:
	std::vector<sk_sp<SkImage>> imagelist;
	/*SkScalar offs;
	SkScalar hei;*/
	Direction nDirectionType;
	ScrollContentInfo ContentInfo;

	std::vector<UIWidget *> childlist;
	SkColor background;

};