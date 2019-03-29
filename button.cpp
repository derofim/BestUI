

#include "Button.h"

//#include "GrContext.h"
//#include "SkCanvas.h"
//#include "SkFont.h"
//#include "SkGradientShader.h"
//#include "SkGraphics.h"
//#include "SkImage.h"


ButtonImage::ButtonImage(const char *pNormalPath, const char *pPressedPath, const char *pDisabledPath)
{
	if (pNormalPath == NULL)
		return;
	SetButtonImageNormal(pNormalPath);
	SetButtonImagePressed(pPressedPath == 0 ? pNormalPath : pPressedPath);
	SetButtonImageDisabled(pDisabledPath == 0 ? pNormalPath : pDisabledPath);

	



}

void ButtonImage::SetButtonImageNormal(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	NormalImage = SkImage::MakeFromEncoded(blob);
	SetSize(NormalImage->width(), NormalImage->height());
}

void ButtonImage::SetButtonImagePressed(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	PressedImage = SkImage::MakeFromEncoded(blob);
}

void ButtonImage::SetButtonImageDisabled(const char *pImagePath)
{
	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	DisabledImage = SkImage::MakeFromEncoded(blob);
}





void ButtonImage::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	if (GetBound().right() == 0 || GetBound().bottom() == 0)
	{
		SetSize(NormalImage->width(), NormalImage->height());
		SetSize(PressedImage->width(), PressedImage->height());
		SetSize(DisabledImage->width(), DisabledImage->height());
	}
	if(GetButState()== but_NormalStatu)
	  canvas->drawImageRect(NormalImage.get(), GetBound(),0);
	else if(GetButState() == but_MouseStayStatu  || GetButState()==but_PressedStatu)
		canvas->drawImageRect(PressedImage.get(), GetBound(), 0);
	else if(GetButState() == but_DisabledStatu)
		canvas->drawImageRect(DisabledImage.get(), GetBound(), 0);
}




Button::Button()
{
	nButState = but_NormalStatu;
	SetPosition(0, 0);
	SetSize(0, 0);
}

void Button::SetText(const char *pText)
{
	text = pText;
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
	if (IsVisible() == false)
		return;
	SkFont font;
	font.setSubpixel(true);
	font.setSize(16);
	SkPaint paint;
	
	SkRect bounds;
	font.measureText(text.c_str(), text.size(), kUTF8_SkTextEncoding, &bounds);
	if (nButState == but_NormalStatu )
	{
		canvas->drawSimpleText(text.c_str(), text.size(), kUTF8_SkTextEncoding, GetBound().left(), GetBound().top()-bounds.top(), font, paint);

	}
	if (nButState == but_MouseStayStatu || nButState==but_PressedStatu)
	{
		//paint.setColor(SK_ColorDKGRAY);
		paint.setColor(SK_ColorRED);
		canvas->drawSimpleText(text.c_str(), text.size(), kUTF8_SkTextEncoding, GetBound().left(), GetBound().top() - bounds.top(), font, paint);
	}
}

void Button::OnMouseDown(int x, int y)
{
	if (nButState == but_DisabledStatu)
		return;
	nButState=but_PressedStatu;
	/*if (GetMouseDownCallBack() != NULL)
		GetMouseDownCallBack()(this);*/
}


void Button::OnMouseMove(int x, int y)
{
	if (nButState == but_DisabledStatu)
		return;
	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom())
	{
		if(nButState!=but_PressedStatu)
		  nButState = but_MouseStayStatu;
	}
	else
		nButState = but_NormalStatu;
}

void Button::OnMouseUp(int x, int y)
{
	if (nButState == but_DisabledStatu)
		return;
	if (x >= GetBound().left() && x <= GetBound().right() && y >= GetBound().top() && y <= GetBound().bottom())
	{
		if (nButState == but_PressedStatu)
		{
			if (GetMouseDownCallBack() != NULL)
				GetMouseDownCallBack()(this, MOUSE_LBUTTONPRESS);
			nButState=but_MouseStayStatu;
		}
	}

}

StaticText::StaticText(char *pText)
{
	text = pText;
}
StaticText::~StaticText()
{

}

void StaticText::Draw(SkCanvas* canvas)
{
	if (IsVisible() == false)
		return;
	SkFont font;
	font.setSubpixel(true);
	font.setSize(16);
	SkPaint paint;
	SkRect bounds;
	font.measureText(text.c_str(), text.size(), kUTF8_SkTextEncoding, &bounds);

	SkPaint paint2;
	paint2.setColor(GetBackGroundColor());
	canvas->drawRect(GetBound(),paint2);

	int nDrawTextLength=text.size();
	if (bounds.width() > GetWidth())
	   nDrawTextLength=(GetWidth())/(bounds.width()/text.size())-1;
	nDrawTextLength=std::max(0,nDrawTextLength);
	canvas->drawSimpleText(text.c_str(), nDrawTextLength, kUTF8_SkTextEncoding, GetBound().left(), GetBound().top()-bounds.top(), font, paint);
}