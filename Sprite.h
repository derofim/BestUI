#pragma once
#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"

//SkPoint


class Sprite :public UIWidget {
public:
	Sprite(const char *pImagePath);

	void Draw(SkCanvas* canvas);
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
private:
	sk_sp<SkImage> SpriteImage;
};