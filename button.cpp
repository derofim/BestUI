

#include "button.h"

//#include "GrContext.h"
//#include "SkCanvas.h"
//#include "SkFont.h"
//#include "SkGradientShader.h"
//#include "SkGraphics.h"
//#include "SkImage.h"


Button::Button(const char *pNormalPath, const char *pPressedPath, const char *pDisabledPath)
{
	if (pNormalPath == NULL)
		return;
	nButState = but_NormalStatu;
	SetButtonNormal(pNormalPath);
	/*if (pPressedPath == NULL)
		SetButtonPressed(pNormalPath);
	else
		SetButtonPressed(pPressedPath);*/
	SetButtonPressed(pPressedPath == 0 ? pNormalPath : pPressedPath);
	SetButtonDisabled(pDisabledPath == 0 ? pNormalPath : pDisabledPath);



}

void Button::SetButtonNormal(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	NormalImage = SkImage::MakeFromEncoded(blob);

	//NormalImage->width();
}

void Button::SetButtonPressed(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	PressedImage = SkImage::MakeFromEncoded(blob);
}

void Button::SetButtonDisabled(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	DisabledImage = SkImage::MakeFromEncoded(blob);
}

void Button::SetEnable(bool bEnable)
{
	if (bEnable == false)
		nButState = but_DisabledStatu;
	else
		nButState = but_NormalStatu;
}




void Button::Draw(SkCanvas* canvas)
{
	//canvas->drawImage(pNormalImage, 50, 50, 0);
	if (GetSkRect().right() == 0 || GetSkRect().bottom() == 0)
		SetSize(NormalImage->width(), NormalImage->height());
	if(nButState== but_NormalStatu)
	  canvas->drawImageRect(NormalImage.get(), GetSkRect(),0);
	else if(nButState == but_MouseStayStatu)
		canvas->drawImageRect(PressedImage.get(), GetSkRect(), 0);
	else if(nButState== but_DisabledStatu)
		canvas->drawImageRect(DisabledImage.get(), GetSkRect(), 0);
}


void Button::OnMouseDown(int x, int y)
{
	if (nButState == but_DisabledStatu)
		return;
	if (GetMouseDownCallBack() != NULL)
		GetMouseDownCallBack()(this);
}


void Button::OnMouseMove(int x,int y)
{
	if (nButState == but_DisabledStatu)
		return;
	if (x >= GetSkRect().left() && x <= GetSkRect().right() && y >= GetSkRect().top() && y <= GetSkRect().bottom())
		nButState = but_MouseStayStatu;
	else
		nButState = but_NormalStatu;
}