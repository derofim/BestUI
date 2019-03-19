
#include "ScrollBar.h"

ScrollBar::ScrollBar(Direction dir)
{
	nDirectionType=dir;
	thumbst=ThumbStatu::NormalStatu;
	offset_reset=true;
}

ScrollBar::~ScrollBar()
{

}
void ScrollBar::SetScrollBarInfo(ScrollBarInfo info)
{
	memcpy(&barinfo,&info,sizeof(ScrollBarInfo));
}


SkScalar ScrollBar::GetThumbSize()
{
	return barinfo.DisplaySize*barinfo.DisplaySize/barinfo.ContentSize;
}

SkScalar ScrollBar::GetThumbPosition()
{
	SkScalar ContentMoveMax=barinfo.ContentSize-barinfo.DisplaySize;
	SkScalar ThumbMoveMax=(barinfo.DisplaySize-GetThumbSize());
	SkScalar ContentOffset=(-barinfo.offset);
	return  ContentOffset*ThumbMoveMax/ContentMoveMax;
}

void ScrollBar::Draw(SkCanvas* canvas)
{
	SkPaint paint;
	paint.setColor(SkColorSetRGB(241,241,241));
	canvas->drawRect(GetSkRect(),paint);

	paint.setColor(SkColorSetRGB(193,193,193));

	if(thumbst==ThumbStatu::MouseStayStatu)
		paint.setColor(SkColorSetRGB(153,153,153));
	else if(thumbst==ThumbStatu::MousePressedStatu)
		paint.setColor(SkColorSetRGB(123,123,123));
	SkScalar ThumbPos=GetThumbPosition();
	if(nDirectionType==Direction::Vertical)
	   thumbrect.set(GetSkRect().left(),GetSkRect().top()+ThumbPos,GetWidth()+GetSkRect().left(),GetThumbSize()+GetSkRect().top()+ThumbPos);
	else if(nDirectionType==Direction::Horizontal)
		 thumbrect.set(GetSkRect().left()+ThumbPos, GetSkRect().top(), GetSkRect().left()+ThumbPos+GetThumbSize(),GetSkRect().height());
	canvas->drawRect(thumbrect,paint);
	/*if(nDirectionType==Direction::Vertical)
	   canvas->drawRect(SkRect{  },paint);
	else if(nDirectionType==Direction::Horizontal)
		 canvas->drawRect(SkRect{ GetSkRect().left()+ThumbPos, GetSkRect().top(), GetSkRect().left()+ThumbPos+GetThumbSize(),GetSkRect().height() },paint);*/
}

void ScrollBar::OnMouseMove(int x, int y)
{
	if (x >= thumbrect.left() && x <= thumbrect.right() && y >= thumbrect.top() && y <= thumbrect.bottom())
	{
		thumbst=ThumbStatu::MouseStayStatu;
	}
	else 
		thumbst=ThumbStatu::NormalStatu;

	if (offset_reset == false)
	{
		thumbst=ThumbStatu::MousePressedStatu;
		int thumb_y =  mouse_offset-y;
		printf("thumb_y2=%d\n",y);
		GetScrollBarController()->ScrollToPosition(this,thumb_y*barinfo.ContentSize/GetHeight());
	}
}

void ScrollBar::OnMouseDown(int x, int y)
{
	if (offset_reset == true)
	{
		mouse_offset=y;
		printf("mouse_offset=%d\n",y);
		offset_reset=false;
	}
	/*if ((x >= thumbrect.left() && x <= thumbrect.right() && y >= thumbrect.top() && y <= thumbrect.bottom()) || offset_reset==false)
	{
		thumbst=ThumbStatu::MousePressedStatu;
		int thumb_y =  mouse_offset-y;
		printf("thumb_y=%d\n",y);
		GetScrollBarController()->ScrollToPosition(this,thumb_y);
	}*/
}

void ScrollBar::OnMouseUp(int x, int y)
{
	printf("mouse up=%d\n",y);
	offset_reset=true;
}