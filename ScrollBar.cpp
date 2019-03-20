
#include "ScrollBar.h"

ScrollBar::ScrollBar(Direction dir)
{
	nDirectionType=dir;
	thumbst=ThumbStatu::NormalStatu;
	SetMouseDragged(false);
	SetPosition(0, 0);
	SetSize(0,0);
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
	if(GetWidth()==0 || GetHeight()==0 || !IsVisible())
		return;
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
		 thumbrect.set(GetSkRect().left()+ThumbPos, GetSkRect().top(), GetSkRect().left()+ThumbPos+GetThumbSize(),GetSkRect().top()+GetSkRect().height());
	canvas->drawRect(thumbrect,paint);
	
}

void ScrollBar::OnMouseMove(int x, int y)
{
	if (IsVisible() == false)
		return;
	if (x >= thumbrect.left() && x <= thumbrect.right() && y >= thumbrect.top() && y <= thumbrect.bottom())
	{
		thumbst=ThumbStatu::MouseStayStatu;
	}
	else
	{
		thumbst = ThumbStatu::NormalStatu;
	}

	if (GetMouseDragged())
	{
		if (nDirectionType == Direction::Vertical)
		{
			if (x >= thumbrect.left() - 80 && x <= thumbrect.right() + 80 && y >= thumbrect.top() && y <= thumbrect.bottom())
				thumbst = ThumbStatu::MousePressedStatu;
			else
				return;
		}
		else if (nDirectionType == Direction::Horizontal)
		{
			if (x >= thumbrect.left()&& x <= thumbrect.right()  && y >= thumbrect.top()-50 && y <= thumbrect.bottom()+50)
				thumbst = ThumbStatu::MousePressedStatu;
			else
				return;
		}
	}

	if (GetMouseDragged())
	{
		int nMousePos=x;
	    if(nDirectionType==Direction::Vertical)
		    nMousePos=y;
		thumbst=ThumbStatu::MousePressedStatu;
		int thumb_move =  mouse_offset-nMousePos;
		//printf("thumb_y2=%d\n",nMousePos);
		if(nDirectionType==Direction::Vertical)
		   GetScrollBarController()->ScrollToPosition(this,thumb_move*barinfo.ContentSize/GetHeight());
		else if (nDirectionType == Direction::Horizontal)
			GetScrollBarController()->ScrollToPosition(this,thumb_move*barinfo.ContentSize/GetWidth());
	}
}

void ScrollBar::OnMouseDown(int x, int y)
{
	if (IsVisible() == false)
		return;
	int nMousePos=x;
	if(nDirectionType==Direction::Vertical)
		nMousePos=y;
	if (GetMouseDragged()==false)
	{
		mouse_offset=nMousePos+(-GetThumbPosition());
		//printf("mouse_offset=%d\n",y);
		SetMouseDragged(true);
	}
	
}

void ScrollBar::OnMouseUp(int x, int y)
{
	if (IsVisible() == false)
		return;
	//printf("mouse up=%d\n",y);
	SetMouseDragged(false);
}