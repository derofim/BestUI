
#include "ScrollBar.h"

ScrollBar::ScrollBar(Direction dir)
{
}

ScrollBar::~ScrollBar()
{

}
void ScrollBar::SetScrollBarInfo(ScrollBarInfo info)
{
	memcpy(&barinfo,&info,sizeof(ScrollBarInfo));
}

void ScrollBar::Draw(SkCanvas* canvas)
{
	SkPaint paint;
	paint.setColor(SkColorSetRGB(241,241,241));
	canvas->drawRect(GetSkRect(),paint);


	paint.setColor(SkColorSetRGB(193,193,193));

	SkScalar fillhei=GetHeight()*(barinfo.DisplaySize/barinfo.ContentSize);

	SkScalar of=barinfo.offset/barinfo.ContentSize* GetHeight();

	canvas->drawRect(SkRect{ GetSkRect().left(),GetSkRect().top(),GetWidth()+GetSkRect().left(),fillhei+GetSkRect().top() },paint);
}