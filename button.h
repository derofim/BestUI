#pragma once

#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "SkFont.h"
#include "SkGradientShader.h"
#include "SkGraphics.h"
#include "SkImage.h"

//SkPoint



class Button :public UIWidget {
public:
	Button();
	 ~Button() {};
	void SetEnable(bool bEnable);
	void SetText(const char *pText);
	enum {
		but_NormalStatu,
		but_MouseStayStatu,
		but_DisabledStatu,
	};

	int GetButState()
	{
		return nButState;
	}

	SkString GetText()
	{
		return text;
	}
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override {};
private:
	int nButState;
	SkString text;
};

class ButtonImage :public Button{
public:
	ButtonImage(const char *pNormalPath,const char *pPressedPath=0,const char *pDisabledPath=0);
	 ~ButtonImage() {};
	void SetButtonImageNormal(const char *pImagePath);
	void SetButtonImagePressed(const char *pImagePath);
	void SetButtonImageDisabled(const char *pImagePath);

	void Draw(SkCanvas* canvas) override;

private:
	sk_sp<SkImage> NormalImage;
	sk_sp<SkImage> PressedImage;
	sk_sp<SkImage> DisabledImage;

};


