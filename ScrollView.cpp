#include "ScrollView.h"

ScrollView::ScrollView()
{
	SetPosition(0, 0);
	SetSize(0, 0);
	nDirectionType = Direction::Vertical;
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));

	background = SkColorSetRGB(255, 255, 255);
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

	/*	if (nDirectionType == Direction::Vertical)
		{
			pChild->SetRect(pChild->GetSkRect().left(), pChild->GetSkRect().top() + diff_y, pChild->GetWidth() + pChild->GetSkRect().left(), pChild->GetSkRect().top() + pChild->GetHeight()+ diff_y);
			
		}
		else if (nDirectionType == Direction::Horizontal)
		{
			pChild->SetRect(pChild->GetSkRect().left()+ diff_x, pChild->GetSkRect().top(), pChild->GetWidth() + diff_x + pChild->GetSkRect().left(), pChild->GetSkRect().top() + pChild->GetHeight());
		}*/
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
		if (nDirectionType == Direction::Vertical)
		{

		}
		pChild->Draw(surfaceCanvas);
	}
	
	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetSkRect().left(), GetSkRect().top());
}

void ScrollView::OnMouseMove(int x, int y)
{
	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		int child_x = x - GetSkRect().left();
		int child_y = y - GetSkRect().top();
		pChild->OnMouseMove(child_x, child_y);
	}
}

void  ScrollView::OnMouseDown(int x, int y)
{
	for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	{
		UIWidget *pChild = *iter;

		int child_x = x - GetSkRect().left();
		int child_y = y - GetSkRect().top();

		if (child_x >= pChild->GetSkRect().left() && child_x <= pChild->GetSkRect().right() && child_y >= pChild->GetSkRect().top() && child_y <= pChild->GetSkRect().bottom())
		{

			printf("x=%d,y=%d,child_x=%d,child_y=%d,left=%f,right=%f,top=%f,bottom=%f\n", x, y, child_x, child_y, pChild->GetSkRect().left(), pChild->GetSkRect().right(), pChild->GetSkRect().top(), pChild->GetSkRect().bottom());
			pChild->OnMouseDown(child_x, child_y);
			return;
		}
		
	}
}

void ScrollView::SetDirection(Direction nType)
{
	nDirectionType = nType;
}

void ScrollView::JumpTop()
{
	/*if (nDirectionType == Direction::Vertical)
		ContentInfo.offs = 0;
	SetScrollOffs(ContentInfo.offs);*/
}
void ScrollView::JumpBottom()
{
	if (nDirectionType == Direction::Vertical)
		ContentInfo.offs_y= -(ContentInfo.height - GetHeight());
	this->RunAction(new ScrollTo(0, 0, ContentInfo.offs_y));
	/*if (nDirectionType == Direction::Vertical)
		ContentInfo.offs = -(ContentInfo.height - GetHeight());
	SetScrollOffs(ContentInfo.offs);*/
}

void ScrollView::JumpLeft()
{
}
void ScrollView::JumpRight()
{
}

void ScrollView::RemoveAllChildWidget()
{
	childlist.clear();
}

void ScrollView::AddChild(UIWidget *pWidget)
{
	childlist.push_back(pWidget);
}


void ScrollView::SetContentSize(SkScalar width, SkScalar height)
{
	ContentInfo.height = height;
	ContentInfo.width = width;
}

void ScrollView::SetBackGroundColor(SkColor color)
{
	background = color;
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