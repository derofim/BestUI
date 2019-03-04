

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

//void ButtonImage::SetEnable(bool bEnable)
//{
//	if (bEnable == false)
//		nButState = but_DisabledStatu;
//	else
//		nButState = but_NormalStatu;
//}




void ButtonImage::Draw(SkCanvas* canvas)
{
	if (GetSkRect().right() == 0 || GetSkRect().bottom() == 0)
	{
		SetSize(NormalImage->width(), NormalImage->height());
		SetSize(PressedImage->width(), PressedImage->height());
		SetSize(DisabledImage->width(), DisabledImage->height());
	}
	if(GetButState()== but_NormalStatu)
	  canvas->drawImageRect(NormalImage.get(), GetSkRect(),0);
	else if(GetButState() == but_MouseStayStatu)
		canvas->drawImageRect(PressedImage.get(), GetSkRect(), 0);
	else if(GetButState() == but_DisabledStatu)
		canvas->drawImageRect(DisabledImage.get(), GetSkRect(), 0);
}


//void ButtonImage::OnMouseDown(int x, int y)
//{
//	if (nButState == but_DisabledStatu)
//		return;
//	if (GetMouseDownCallBack() != NULL)
//		GetMouseDownCallBack()(this);
//}
//
//
//void ButtonImage::OnMouseMove(int x,int y)
//{
//	if (nButState == but_DisabledStatu)
//		return;
//	if (x >= GetSkRect().left() && x <= GetSkRect().right() && y >= GetSkRect().top() && y <= GetSkRect().bottom())
//		nButState = but_MouseStayStatu;
//	else
//		nButState = but_NormalStatu;
//}


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

#include "SkTextBlob.h"

static void add_to_text_blob(SkTextBlobBuilder* builder, const char* text, const SkFont& font,
	SkScalar x, SkScalar y) {
	SkTDArray<uint16_t> glyphs;

	size_t len = strlen(text);
	glyphs.append(font.countText(text, len, kUTF8_SkTextEncoding));
	font.textToGlyphs(text, len, kUTF8_SkTextEncoding, glyphs.begin(), glyphs.count());

	const SkScalar advanceX = font.getSize() * 0.85f;
	const SkScalar advanceY = font.getSize() * 1.5f;

	SkTDArray<SkScalar> pos;
	for (unsigned i = 0; i < len; ++i) {
		*pos.append() = x + i * advanceX;
		*pos.append() = y + i * (advanceY / len);
	}
	const SkTextBlobBuilder::RunBuffer& run = builder->allocRunPos(font, glyphs.count());
	memcpy(run.glyphs, glyphs.begin(), glyphs.count() * sizeof(uint16_t));
	memcpy(run.pos, pos.begin(), len * sizeof(SkScalar) * 2);
}
#include "SkMSAN.h"
void Button::Draw(SkCanvas* canvas)
{
	SkFont font;
	font.setSubpixel(true);
	font.setSize(16);
	SkPaint paint;
	
	
	if (nButState == but_NormalStatu)
	{
		canvas->drawSimpleText(text.c_str(), text.size(), kUTF8_SkTextEncoding, GetSkRect().left(), GetSkRect().top(), font, paint);
	}
	if (nButState == but_MouseStayStatu)
	{
		paint.setColor(SK_ColorDKGRAY);
		canvas->drawSimpleText(text.c_str(), text.size(), kUTF8_SkTextEncoding, GetSkRect().left(), GetSkRect().top(), font, paint);
	}
}

void Button::OnMouseDown(int x, int y)
{
	if (nButState == but_DisabledStatu)
		return;
	if (GetMouseDownCallBack() != NULL)
		GetMouseDownCallBack()(this);
}


void Button::OnMouseMove(int x, int y)
{
	if (nButState == but_DisabledStatu)
		return;
	if (x >= GetSkRect().left() && x <= GetSkRect().right() && y >= GetSkRect().top() && y <= GetSkRect().bottom())
		nButState = but_MouseStayStatu;
	else
		nButState = but_NormalStatu;
}