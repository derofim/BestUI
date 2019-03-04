#include "ScrollView.h"

ScrollView::ScrollView(UIRoot *pUi)
{
	SetPosition(0, 0);
	SetSize(0, 0);
	nDirectionType = Direction::Vertical;
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	pUIRoot = pUi;
}

void ScrollView::Draw(SkCanvas* canvas)
{
	GrContext* context = canvas->getGrContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
	SkScalar x = 0, y = 0;

	/*ContentInfo.offs += 0.1;
	if (ContentInfo.offs >= ContentInfo.height - GetHeight())
	{
		ContentInfo.offs = 0;
	}*/
	y = ContentInfo.offs;
	for (auto iter = imagelist.begin(); iter != imagelist.end(); iter++)
	{
		surfaceCanvas->drawImage((*iter).get(), x, y, 0);
		y += (*iter)->height();
	}
	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetSkRect().left(), GetSkRect().top());
}


void ScrollView::OnMouseMove(int x, int y)
{
}

void  ScrollView::OnMouseDown(int x, int y)
{
}

void ScrollView::SetDirection(Direction nType)
{
	nDirectionType = nType;
}

void ScrollView::JumpTop()
{
	if (nDirectionType == Direction::Vertical)
		ContentInfo.offs = 0;
}
void ScrollView::JumpBottom()
{
	if (nDirectionType == Direction::Vertical)
		ContentInfo.offs = -(ContentInfo.height - GetHeight());
}

void ScrollView::JumpLeft()
{
}
void ScrollView::JumpRight()
{
}


void ScrollView::AddChild(UIWidget *pWidget)
{
	//pWidget->add
//	pUIRoot->AddWidget(pWidget);
}


void  ScrollView::AddChild(char *pImagePath)
{

	sk_sp<SkData> blob = SkData::MakeFromFileName(pImagePath);
	if (blob == NULL) return;
	sk_sp<SkImage> image = SkImage::MakeFromEncoded(blob);
	imagelist.push_back(image);

	if (nDirectionType == Direction::Vertical)
	{
		ContentInfo.height += image->height();
		ContentInfo.width = GetWidth();
	}
	else if (nDirectionType == Direction::Horizontal)
	{
		ContentInfo.width += image->width();
		ContentInfo.height = GetHeight();
	}

	//sk_sp<SkSurface> surface =  SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info, 0, &props) : SkSurface::MakeRaster(info, &props);


}

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