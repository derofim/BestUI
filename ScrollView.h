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

//SkSurface

struct ScrollContentInfo {
	SkScalar width;
	SkScalar height;
	SkScalar offs;
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

	void AddChild(char *pImagePath);

	void JumpTop();
	void JumpBottom();
	void JumpLeft();
	void JumpRight();

	void SetDirection(Direction nType);
	

private:
	std::vector<sk_sp<SkImage>> imagelist;
	/*SkScalar offs;
	SkScalar hei;*/
	Direction nDirectionType;
	ScrollContentInfo ContentInfo;
};