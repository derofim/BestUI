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

class ScrollView :public UIWidget {
public:
	ScrollView();

	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;

	void AddChild(char *pImagePath);

private:
	std::vector<sk_sp<SkImage>> imagelist;
	SkScalar offs;
};