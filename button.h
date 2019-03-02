#pragma once

#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"

//SkPoint


class Button :public UIWidget{
public:
	Button(const char *pNormalPath,const char *pPressedPath=0,const char *pDisabledPath=0);

	void SetButtonNormal(const char *pImagePath);
	void SetButtonPressed(const char *pImagePath);
	void SetButtonDisabled(const char *pImagePath);
	void SetEnable(bool bEnable);

	enum {
		but_NormalStatu,
		but_MouseStayStatu,
		but_DisabledStatu,
	};

	void Draw(SkCanvas* canvas);
    void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
private:
	sk_sp<SkImage> NormalImage;
	sk_sp<SkImage> PressedImage;
	sk_sp<SkImage> DisabledImage;
	int nButState;
	

};