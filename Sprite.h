#pragma once
#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"
#include "SkAutoPixmapStorage.h"

//SkPoint


class Sprite :public UIWidget {
public:
	Sprite(const char *pImagePath);

	void Draw(SkCanvas* canvas);
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;

	void SetScale(SkScalar scale);
	void SetScaleX(SkScalar sx);
	void SetScaleY(SkScalar sy);
private:
	sk_sp<SkImage> SpriteImage;
};