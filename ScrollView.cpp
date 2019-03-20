#include "ScrollView.h"

ScrollView::ScrollView()
{
	SetPosition(0, 0);
	SetSize(0, 0);
//	nDirectionType = Direction::Vertical;
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	background = SkColorSetRGB(255, 255, 255);
	vert_bar = new ScrollBar(Direction::Vertical);
	hori_bar = new ScrollBar(Direction::Horizontal);
	vert_bar->set_controller(this);
	hori_bar->set_controller(this);
}

//void ScrollView::Draw(SkCanvas* canvas)
//{
//	GrContext* context = canvas->getGrContext();
//	SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
//	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
//    auto surfaceCanvas = gpuSurface->getCanvas();
//	SkScalar x = 0, y = 0;
//
//	/*ContentInfo.offs += 0.1;
//	if (ContentInfo.offs >= ContentInfo.height - GetHeight())
//	{
//		ContentInfo.offs = 0;
//	}*/
//	y = ContentInfo.offs;
//	for (auto iter = imagelist.begin(); iter != imagelist.end(); iter++)
//	{
//		surfaceCanvas->drawImage((*iter).get(), x, y, 0);
//		y += (*iter)->height();
//	}
//	SkFont font;
//	font.setSubpixel(true);
//	font.setSize(16);
//	SkPaint paint;
//	surfaceCanvas->drawSimpleText("very goods", 10, kUTF8_SkTextEncoding, x, y,font, paint);
//	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
//	canvas->drawImage(image, GetSkRect().left(), GetSkRect().top());
//}

void ScrollView::Draw(SkCanvas* canvas)
{
	/*ScrollContentInfo offset;
	memset(&offset, 0x00, sizeof(offset));
	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		if (nDirectionType == Direction::Vertical)
		{
	
			pChild->SetRect(pChild->GetSkRect().left(), offset.height+ ContentInfo.offs, pChild->GetWidth() + pChild->GetSkRect().left(), offset.height + pChild->GetHeight()+ ContentInfo.offs);
			offset.height += pChild->GetHeight();
			offset.width = GetWidth();
		}
		else if (nDirectionType == Direction::Horizontal)
		{
			pChild->SetRect(offset.width+ ContentInfo.offs, pChild->GetSkRect().top(), pChild->GetWidth() + offset.width+ ContentInfo.offs, pChild->GetSkRect().top() + pChild->GetHeight());
			offset.width += pChild->GetWidth();
			offset.height = GetHeight();

		}
	}


	ContentInfo.height = offset.height;
	ContentInfo.width = offset.width;*/

	//test 
	/*if (nDirectionType == Direction::Vertical)
	{
		ContentInfo.offs += 0.01;
		if (ContentInfo.offs >= ContentInfo.height - GetHeight())
		{
			ContentInfo.offs = 0;
		}
	}
	else if (nDirectionType == Direction::Horizontal)
	{
		ContentInfo.offs += 0.1;
		if (ContentInfo.offs >= ContentInfo.width - GetWidth())
		{
			ContentInfo.offs = 0;
		}
	}*/



	ContentInfo.offs_y = GetScrolloffsY();
	SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	ContentInfo.offs_x = GetScrolloffsX();
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;
	/*if (diff != 0)
	{
		printf("diff=%f\n", diff);
	}*/
	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->SetRect(pChild->GetSkRect().left()+diff_x, pChild->GetSkRect().top() + diff_y, pChild->GetWidth()+diff_x + pChild->GetSkRect().left(), pChild->GetSkRect().top() + pChild->GetHeight() + diff_y);

	}


	ContentInfo.preoffs_y = ContentInfo.offs_y;
	ContentInfo.preoffs_x = ContentInfo.offs_x;
	GrContext* context = canvas->getGrContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
	SkPaint paint;
	paint.setColor(background);
	surfaceCanvas->drawRect(SkRect{ 0,0,GetWidth(),GetHeight() }, paint);
	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->Draw(surfaceCanvas);
	}

	if (vert_bar != NULL)
	{
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.height;
		barinfo.DisplaySize=GetSkRect().height();
		barinfo.offset=ContentInfo.offs_y;
		vert_bar->SetScrollBarInfo(barinfo);
		vert_bar->Draw(surfaceCanvas);
	}

	if (hori_bar != NULL)
	{
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.width;
		barinfo.DisplaySize=GetSkRect().width();
		barinfo.offset=ContentInfo.offs_x;
		hori_bar->SetScrollBarInfo(barinfo);
		hori_bar->Draw(surfaceCanvas);
	}
	
	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetSkRect().left(), GetSkRect().top());
}

void ScrollView::OnMouseMove(int x, int y)
{
	SkPoint point=ScrollViewToChildPoint(x,y);
	if (vert_bar != NULL && vert_bar->IsVisible())
		vert_bar->OnMouseMove(point.x(), point.y());

	if (hori_bar != NULL && hori_bar->IsVisible())
		hori_bar->OnMouseMove(point.x(), point.y());

	/*if (point.x() >= vert_bar->GetSkRect().left() && point.x() <= vert_bar->GetSkRect().right() && point.y() >= vert_bar->GetSkRect().top() && point.y() <= vert_bar->GetSkRect().bottom())
		return;
	if (point.x() >= hori_bar->GetSkRect().left() && point.x() <= hori_bar->GetSkRect().right() && point.y() >= hori_bar->GetSkRect().top() && point.y() <= hori_bar->GetSkRect().bottom())
		return ;

	if(point.x()>GetWidth() || point.y()>GetHeight())
		return;*/

	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->OnMouseMove(point.x(), point.y());
	}

	
}

void  ScrollView::OnMouseDown(int x, int y)
{
	SkPoint point=ScrollViewToChildPoint(x,y);
	if (vert_bar != NULL)
	{
		if (point.x() >= vert_bar->GetSkRect().left() && point.x() <= vert_bar->GetSkRect().right() && point.y() >= vert_bar->GetSkRect().top() && point.y() <= vert_bar->GetSkRect().bottom())
		  return  vert_bar->OnMouseDown(point.x(), point.y());
	}

	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		if (point.x() >= hori_bar->GetSkRect().left() && point.x() <= hori_bar->GetSkRect().right() && point.y() >= hori_bar->GetSkRect().top() && point.y() <= hori_bar->GetSkRect().bottom())
		   return  hori_bar->OnMouseDown(point.x(), point.y());
	}
	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		if (point.x() >= pChild->GetSkRect().left() && point.x() <= pChild->GetSkRect().right() && point.y() >= pChild->GetSkRect().top() && point.y() <= pChild->GetSkRect().bottom())
		{
			//printf("x=%d,y=%d,child_x=%d,child_y=%d,left=%f,right=%f,top=%f,bottom=%f\n", x, y, point.x(), point.y(), pChild->GetSkRect().left(), pChild->GetSkRect().right(), pChild->GetSkRect().top(), pChild->GetSkRect().bottom());
			return pChild->OnMouseDown(point.x(), point.y());
		}
		
	}

	
}


void ScrollView::OnMouseUp(int x, int y)
{
	SkPoint point=ScrollViewToChildPoint(x,y);
	if (vert_bar != NULL && vert_bar->IsVisible())
	{
		vert_bar->OnMouseUp(point.x(), point.y());
	}
	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		hori_bar->OnMouseUp(point.x(), point.y());
	}
}

void ScrollView::OnMouseWheel(float delta, uint32_t modifier)
{
	//printf("OnMouseWheel delta=%f,modifier=%d\n",delta,modifier);
	if(vert_bar==NULL || !vert_bar->IsVisible())
		return;
	ScrollToPosition(vert_bar,GetScrolloffsY()+delta*10);
}


void ScrollView::ScrollToPosition(ScrollBar* source, int position)
{
	if(source==NULL)
		return;
	if (source == vert_bar)
	{
		SkScalar pos_y=position;
		//printf("scroll before pos=%f\n",pos_y);
		pos_y=std::min((float)0,(float)pos_y);
		pos_y=std::max((float)(-(ContentInfo.height - GetHeight())),(float)pos_y);
		//printf("scroll pos=%f\n",pos_y);
		SetScrolloffsY(pos_y);
	}

	if (source == hori_bar)
	{
		SkScalar pos_x=position;
		pos_x=std::min((float)0,(float)pos_x);
		pos_x=std::max((float)(-(ContentInfo.width - GetWidth())),(float)pos_x);
		SetScrolloffsX(pos_x);
	}
}

//void ScrollView::SetDirection(Direction nType)
//{
//	nDirectionType = nType;
//}
//
//void ScrollView::JumpTop()
//{
//	if (nDirectionType == Direction::Vertical)
//		ContentInfo.offs_y = 0;
//	this->RunAction(new ScrollTo(0, 0, ContentInfo.offs_y));
//}
//void ScrollView::JumpBottom()
//{
//	if (nDirectionType == Direction::Vertical)
//		ContentInfo.offs_y= -(ContentInfo.height - GetHeight());
//	this->RunAction(new ScrollTo(0, 0, ContentInfo.offs_y));
//	/*if (nDirectionType == Direction::Vertical)
//		ContentInfo.offs = -(ContentInfo.height - GetHeight());
//	SetScrollOffs(ContentInfo.offs);*/
//}
//
//void ScrollView::JumpLeft()
//{
//}
//void ScrollView::JumpRight()
//{
//}

void ScrollView::InitOffset()
{
	ContentInfo.offs_y = 0;
	ContentInfo.preoffs_y = 0;
	ContentInfo.offs_x = 0;
	ContentInfo.preoffs_x = 0;
}

void ScrollView::RemoveAllChildWidget()
{

	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		delete pChild;
	}
	childlist.clear();
	InitOffset();

}

void ScrollView::AddChild(UIWidget *pWidget)
{
	childlist.push_back(pWidget);
}


void ScrollView::SetContentSize(SkScalar width, SkScalar height)
{
	ContentInfo.height = height;
	ContentInfo.width = width;
	if (ContentInfo.height > GetSkRect().height())
	{
		vert_bar->SetPosition(GetSkRect().width()-10,0);
		vert_bar->SetSize(10,GetSkRect().height());
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.height;
		barinfo.DisplaySize=GetSkRect().height();
		barinfo.offset=0;
		vert_bar->SetScrollBarInfo(barinfo);
	}

	if (ContentInfo.width > GetSkRect().width())
	{
		hori_bar->SetPosition(0,GetSkRect().height()-10);
		hori_bar->SetSize(GetSkRect().width(),10);
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.width;
		barinfo.DisplaySize=GetSkRect().width();
		barinfo.offset=0;
		hori_bar->SetScrollBarInfo(barinfo);
	}
}

void ScrollView::SetBackGroundColor(SkColor color)
{
	background = color;
}



void ScrollView::SetHorizontalScrollBar(ScrollBar* horiz_sb) 
{
   horiz_sb->SetVisible(vert_bar->IsVisible());
   delete vert_bar;
   horiz_sb->set_controller(this);
   vert_bar = horiz_sb;
}

void ScrollView::SetVerticalScrollBar(ScrollBar* vert_sb) 
{
   vert_sb->SetVisible(hori_bar->IsVisible());
   delete hori_bar;
   vert_sb->set_controller(this);
   hori_bar = vert_sb;
}




//void  ScrollView::AddChild(char *pImagePath)
//{
//
//	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
//	if (blob == NULL) return;
//	sk_sp<SkImage> image = SkImage::MakeFromEncoded(blob);
//	imagelist.push_back(image);
//
//	if (nDirectionType == Direction::Vertical)
//	{
//		ContentInfo.height += image->height();
//		ContentInfo.width = GetWidth();
//	}
//	else if (nDirectionType == Direction::Horizontal)
//	{
//		ContentInfo.width += image->width();
//		ContentInfo.height = GetHeight();
//	}
//
//	//sk_sp<SkSurface> surface =  SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info, 0, &props) : SkSurface::MakeRaster(info, &props);
//
//
//}

//void draw(SkCanvas* canvas) {
//	SkPaint paint;
//	paint.setTextSize(32);
//	GrContext* context = canvas->getGrContext();
//	if (!context) {
//		canvas->drawString("GPU only!", 20, 40, paint);
//		return;
//	}
//	SkImageInfo info = SkImageInfo::MakeN32(256, 64, kOpaque_SkAlphaType);
//	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
//	auto surfaceCanvas = gpuSurface->getCanvas();
//	surfaceCanvas->clear(SK_ColorWHITE);
//	surfaceCanvas->drawString("GPU rocks!", 20, 40, paint);
//	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
//	canvas->drawImage(image, 0, 0);
//}