#include "TextField.h"

TextField::TextField()
{
	SetBackGroundColor(SkColorSetRGB(230,230,230));
	text_color=SkColorSetRGB(0, 0, 0);

	
}
TextField::~TextField()
{
}

void TextField::Draw(SkCanvas* canvas)
{

	GrContext* context = canvas->getGrContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
	SkPaint paint;
	paint.setColor(GetBackGroundColor());
	surfaceCanvas->drawRect(SkRect{ 0,0,GetWidth(),GetHeight() }, paint);
	SkScalar ins_x=0;
	SkScalar inx_y=20;

	SkFont font;
	font.setSubpixel(true);
	font.setSize(16);
	for (int k = 0; k < textlist.size(); k++)
	{
		if (textlist[k].text == 0x0a)
		{
			inx_y+=20;
		}
		paint.setColor(textlist[k].color);
		SkRect bounds;
		if (textlist[k].text < 128)
		{
			font.measureText(&textlist[k].text, 1, kUTF8_SkTextEncoding, &bounds, &paint);
			surfaceCanvas->drawSimpleText(&textlist[k].text, 1, kUTF8_SkTextEncoding, ins_x, inx_y, font, paint);
		}
		else
		{
			/*char pszText[5];
			pszText[0]=0xe5;
			pszText[1]=0xae;
			pszText[2]=0x8c;
			pszText[3]=0x00;
			font.measureText(pszText, strlen(pszText), kUTF8_SkTextEncoding, &bounds, &paint);
			surfaceCanvas->drawSimpleText(pszText ,strlen(pszText), kUTF8_SkTextEncoding, ins_x, inx_y, font, paint);*/
		}
		ins_x+=bounds.width()+0.5;
		//printf("char width=%f\n",bounds.width());
	}

	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetBound().left(), GetBound().top());
}
void TextField::OnMouseMove(int x, int y)
{
}


void TextField::OnMouseDown(int x, int y)
{
}

void TextField::OnMouseUp(int x, int y)
{
}

void TextField::OnMouseWheel(float delta, uint32_t modifier) 
{
}

void TextField::OnKey(sk_app::Window::Key key, uint32_t modifiers)
{
	
	if (key != sk_app::Window::Key::kBack)
	{
	//	printf("del back key\n");
		/*TextDesc desc;
		desc.text=(char)key;
		desc.color=text_color;
		textlist.push_back(desc);*/
	}
}

void TextField::OnChar(SkUnichar c, uint32_t modifiers)
{
	printf("insert key=%d,modifiers=%d\n",c,modifiers);
	TextDesc desc;
	desc.text=c;
	desc.color=text_color;
	textlist.push_back(desc);
}
