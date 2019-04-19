#include "TextField.h"
#include "windows.h"
#pragma comment(lib, "Imm32.lib")

HIMC imm_context;
TextField::TextField() 
{
    SetBackGroundColor(SkColorSetRGB(230, 230, 230));
    text_color = SkColorSetRGB(0, 0, 0);
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	vert_bar = new ScrollBar(Direction::Vertical);
    hori_bar = new ScrollBar(Direction::Horizontal);
    vert_bar->set_controller(this);
    hori_bar->set_controller(this);

    //imm_context = ::ImmGetContext(hwnd);
    //CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500}, {200, 100, 500, 200}};
    //ImmSetCandidateWindow(imm_context, &exclude_rectangle);
    unindex = 0;
    TextLine info;
    info.nHeight = 20;
    line.push_back(info);
	SetContentSize(GetWidth(), line.size() * 20);
    fCurPosBlinkTime = SkTime::GetMSecs();
}
TextField::~TextField() 
{
}

// HIMC imm_context = ::ImmGetContext(window_handle);
//::ImmSetCandidateWindow(imm_context, &exclude_rectangle);
#include "SkTextBlob.h"
void TextField::Draw(SkCanvas* canvas) 
{
	ContentInfo.offs_y = GetScrolloffsY();
	SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	ContentInfo.offs_x = GetScrolloffsX();
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;

    GrContext* context = canvas->getGrContext();
    SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
    auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
    SkPaint paint;
    paint.setColor(GetBackGroundColor());
    surfaceCanvas->drawRect(SkRect{0, 0, GetWidth(), GetHeight()}, paint);
  /*  SkScalar ins_x = 0;
    SkScalar ins_y = 20;*/
    SkFont font;
    font.setSubpixel(true);
    font.setSize(16);


	int nIndex = (-ContentInfo.offs_y) / 20;

	int ins_y = 20;
	int ins_x = 0;
	for (int k = nIndex; k < line.size(); k++)
	{
		paint.setColor(SkColorSetRGB(0, 0, 0));
		std::wstring* ptext = GetLineText(k);
		surfaceCanvas->drawSimpleText(ptext->c_str(), ptext->size() * 2, kUTF16_SkTextEncoding, ins_x, ins_y, font, paint);
		ins_y += line[k].nHeight;
		if (ins_y > GetDisplayHeigth())
			break;
	}

	/*int ins_y = 20;
	int ins_x = 0;
    for (int k = 0; k < line.size(); k++) 
	{
        paint.setColor(SkColorSetRGB(0, 0, 0));

        std::wstring* ptext = GetLineText(k);
        surfaceCanvas->drawSimpleText(ptext->c_str(), ptext->size() * 2, kUTF16_SkTextEncoding,ins_x, ins_y, font, paint);
        ins_y += line[k].nHeight;
    }*/

    DrawCurPosBlink(surfaceCanvas);
    /*Sequence sq = Sequence(0, [&]() {
        DrawCurPosBlink(surfaceCanvas);
    }, new DelayTime(1.0), 0);
    sq.RunSequence();*/

    paint.setColor(GetBackGroundColor());
    sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
    canvas->drawImage(image, GetBound().left(), GetBound().top());

	UpdateScrollBarInfo();
	if (vert_bar != NULL)
		vert_bar->Draw(canvas);
	if (hori_bar != NULL)
		hori_bar->Draw(canvas);
}

void TextField::DrawCurPosBlink(SkCanvas* canvas) 
{
    // double diff = SkTime::GetMSecs() - fCurPosBlinkTime;
    // printf("diff=%f\n", diff);
    // if (SkTime::GetMSecs() - fCurPosBlinkTime >= 100)
    {
        fCurPosBlinkTime = SkTime::GetMSecs();
        SkScalar x, y;
		y = curpos.y * 20 + GetScrolloffsY();
        std::wstring* ptext = GetLineText(curpos.y);
        SkPaint paint;
        SkFont font;
        font.setSubpixel(true);
        font.setSize(16);
        SkRect bounds;
        font.measureText(ptext->c_str(), curpos.x * 2, kUTF16_SkTextEncoding, &bounds, &paint);
        x = bounds.width() + 3;

        paint.setColor(SkColorSetRGB(0, 0, 0));
        SkPoint p1, p2;
        p1.set(x, y + 5);
        p2.set(x, y + 25);
        canvas->drawLine(p1, p2, paint);
    }
}



int TextField::FindSuitX(std::wstring text, int nCmpX) 
{
    int k;
    SkPaint paint;
    SkFont font;
    font.setSubpixel(true);
    font.setSize(16);
    SkRect bounds;
    for (k = 0; k < text.length(); k++) 
	{
        font.measureText(text.c_str(), (k + 1) * 2, kUTF16_SkTextEncoding, &bounds, &paint);
        if (nCmpX < bounds.width()) 
		{
            break;
        }
    }
    return k;
}
void TextField::SetLastX() 
{
    SkPaint paint;
    SkFont font;
    font.setSubpixel(true);
    font.setSize(16);
    SkRect bounds;
    std::wstring* ptext = GetLineText(curpos.y);
    font.measureText(ptext->c_str(), curpos.x * 2, kUTF16_SkTextEncoding, &bounds, &paint);
    nLastX = bounds.width();
}

void TextField::OnMouseMove(int x, int y) 
{
    if (vert_bar != NULL && vert_bar->IsVisible())
		vert_bar->OnMouseMove(x, y);
    if (hori_bar != NULL && hori_bar->IsVisible()) 
		hori_bar->OnMouseMove(x, y);
}

void TextField::OnMouseDown(int x, int y) 
{

	if (vert_bar != NULL && vert_bar->IsVisible())
	{
        if (x >= vert_bar->GetBound().left() && x <= vert_bar->GetBound().right() && y >= vert_bar->GetBound().top() && y <= vert_bar->GetBound().bottom())
            return vert_bar->OnMouseDown(x, y);
    }

    if (hori_bar != NULL && hori_bar->IsVisible())
	{
        if (x >= hori_bar->GetBound().left() && x <= hori_bar->GetBound().right() && y >= hori_bar->GetBound().top() && y <= hori_bar->GetBound().bottom())
            return hori_bar->OnMouseDown(x, y);
    }

    SkPoint point = ScrollViewToChildPoint(x, y);

    if (point.y() > line.size() * 20) return;

	int nYIndex = (-GetScrolloffsY()) / 20;
    int nIndex = point.y() / 20+ nYIndex;

    std::wstring* ptext = GetLineText(nIndex);
    SkPaint paint;
    SkFont font;
    font.setSubpixel(true);
    font.setSize(16);
    SkRect bounds;

    int k;
    for (k = 0; k < ptext->length(); k++) 
	{
        font.measureText(ptext->c_str(), (k + 1) * 2, kUTF16_SkTextEncoding, &bounds, &paint);
        if (point.x() < bounds.width()) 
		{
            break;
        }
    }
    curpos.x = k;
	curpos.y = nIndex;

    nLastX = point.x();
}

void TextField::OnMouseUp(int x, int y) 
{
    if (vert_bar != NULL && vert_bar->IsVisible())
	{
        vert_bar->OnMouseUp(x, y);
    }
    if (hori_bar != NULL && hori_bar->IsVisible()) 
	{
        hori_bar->OnMouseUp(x, y);
    }
}

void TextField::OnMouseWheel(float delta, uint32_t modifier) 
{
    if (vert_bar == NULL || !vert_bar->IsVisible()) 
		return;
    ScrollToPosition(vert_bar, GetScrolloffsY() + delta * 20);
}

void TextField::OnKey(sk_app::Window::Key key, uint32_t modifiers) 
{
    if (key == sk_app::Window::Key::kDown && modifiers == 0) 
	{
        // printf("on key modifiers=%d\n", modifiers);
        curpos.y++;
        curpos.y = std::min((int)line.size() - 1, curpos.y);
        std::wstring* ptext = GetLineText(curpos.y);
        curpos.x = FindSuitX(*ptext, nLastX);
    }
    if (key == sk_app::Window::Key::kUp && modifiers == 0) 
	{
        curpos.y--;
        curpos.y = std::max(0, curpos.y);
        std::wstring* ptext = GetLineText(curpos.y);
        curpos.x = FindSuitX(*ptext, nLastX);
    }
    if (key == sk_app::Window::Key::kLeft && modifiers == 0) 
	{
        //	std::wstring *ptext = GetLineText(curpos.y);
        if (curpos.x == 0 && curpos.y == 0) return;
        if (curpos.x == 0 /*&& curpos.y>0*/) 
		{
            curpos.y--;
            std::wstring* ptext = GetLineText(curpos.y);
            curpos.x = ptext->size();
        }
		else 
		{
            curpos.x--;
        }
        SetLastX();
    }

    if (key == sk_app::Window::Key::kRight && modifiers == 0) 
	{
        std::wstring* ptext = GetLineText(curpos.y);
        if (curpos.x >= ptext->size() && curpos.y < line.size() - 1) 
		{
            curpos.y++;
            curpos.x = 0;
            // curpos.y = std::min((int)line.size() - 1, curpos.y);
        } 
		else 
		{
            curpos.x++;
            curpos.x = std::min(curpos.x, (int)ptext->size());
        }
        SetLastX();
    }
}


void TextField::OnChar(SkUnichar c, uint32_t modifiers)
{
    /*CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500},
      {200, 100,500,200}};
        ImmSetCandidateWindow(imm_context, &exclude_rectangle);*/

   

    std::wstring* ptext = GetLineText(curpos.y);
    printf("cur line=%d,text=%s\n", curpos.y, wstring2string(*ptext, "chinese").c_str());
    if (c == 0x08) //delete
	{
        if (ptext->length() == 0 || curpos.x == 0) 
		{
            if (curpos.y >= 1) 
			{
                curpos.y--;
                curpos.x = GetLineText(curpos.y)->size();
                GetLineText(curpos.y)->insert(curpos.x, ptext->c_str(), ptext->size());
                line.erase(line.begin() + curpos.y + 1);
            }
			SetContentSize(GetWidth()-10, line.size() * 20);
            return;
        }
        if (ptext->length() >= 1) 
		{
            // ptext->pop_back();
            ptext->erase(curpos.x - 1, 1);
			SetContentSize(GetWidth()-10, line.size() * 20);
        }

        curpos.x--;
		
        return;
    }

    if (c == 0x0d) 
	{
        if (curpos.x < ptext->size()) 
		{
            TextLine info;
            info.nHeight = 20;
            info.line_text = ptext->substr(curpos.x, ptext->size() - curpos.x);
            // line.push_back(info);

            ptext->resize(curpos.x);
            curpos.y += 1;
            curpos.x = 0;

            line.insert(line.begin() + curpos.y, info);
        } 
		else 
		{
            TextLine info;
            info.nHeight = 20;
            line.push_back(info);
            curpos.y += 1;
            curpos.x = 0;
        }
		SetContentSize(GetWidth()-10, line.size() * 20);
        return;
    }
    if (c > 0x80) 
	{
        unchar[unindex] = (char)c;
        unindex++;

        if (unindex == 2) 
		{
            std::wstring wtxt = string2wstring(unchar, "Chinese");
            // ptext->push_back(wtxt[0]);
            ptext->insert(curpos.x, &wtxt[0], 1);
            curpos.x++;
        }
        unindex = unindex % 2;
    } 
	else 
	{
        wchar_t inc = c;
        ptext->insert(curpos.x, &inc, 1);
        curpos.x++;
    }
}


SkScalar TextField::GetDisplayWidth()
{
	SkScalar width = GetBound().width();
	if (ContentInfo.height > GetBound().height() && vert_bar->IsVisible())
		width -= vert_bar->GetWidth();
	return width;
}
SkScalar TextField::GetDisplayHeigth()
{
	SkScalar heigth = GetBound().height();
	if (ContentInfo.width > GetBound().width() && hori_bar->IsVisible())
		heigth -= hori_bar->GetHeight();
	return heigth;
}

void TextField::SetContentSize(SkScalar width, SkScalar height)
{
	ContentInfo.height = height;
	ContentInfo.width = width;
	UpdateScrollBarInfo();
}


void TextField::UpdateScrollBarInfo()
{
	if (ContentInfo.height > GetDisplayHeigth())
	{
		if (vert_bar->GetWidth() == 0)
		{
			vert_bar->SetPosition(GetBound().width() - BAR_VER_WIDTH + GetBound().left(), GetBound().top());
			vert_bar->SetSize(BAR_VER_WIDTH, GetDisplayHeigth());
		}
		else
		{
			vert_bar->SetPosition(GetBound().width() - vert_bar->GetBound().width() + GetBound().left(), GetBound().top());
			vert_bar->SetSize(vert_bar->GetBound().width(), GetDisplayHeigth());
		}

		ScrollBarInfo barinfo;
		barinfo.ContentSize = ContentInfo.height;
		barinfo.DisplaySize = vert_bar->GetHeight();
		barinfo.offset = ContentInfo.offs_y;
		SkScalar ContentMoveMax = barinfo.ContentSize - barinfo.DisplaySize;
		barinfo.offset = std::max(barinfo.offset, -ContentMoveMax);
		SetScrolloffsY(barinfo.offset);
		vert_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		vert_bar->SetBound(0, 0, 0, 0);
		SetScrolloffsY(0);
	}

	if (ContentInfo.width > GetDisplayWidth())
	{
		if (hori_bar->GetWidth() == 0)
		{
			hori_bar->SetPosition(GetBound().left(), GetBound().bottom() - BAR_HORI_HEIGHT);
			hori_bar->SetSize(GetDisplayWidth(), BAR_HORI_HEIGHT);
		}
		else
		{
			hori_bar->SetPosition(GetBound().left(), GetBound().bottom() - hori_bar->GetBound().height());
			hori_bar->SetSize(GetDisplayWidth(), hori_bar->GetBound().height());
		}
		ScrollBarInfo barinfo;
		barinfo.ContentSize = ContentInfo.width;
		barinfo.DisplaySize = hori_bar->GetWidth();
		barinfo.offset = ContentInfo.offs_x;

		SkScalar ContentMoveMax = barinfo.ContentSize - barinfo.DisplaySize;
		barinfo.offset = std::max(barinfo.offset, -ContentMoveMax);
		SetScrolloffsX(barinfo.offset);
		hori_bar->SetScrollBarInfo(barinfo);
	}
	else
	{
		hori_bar->SetBound(0, 0, 0, 0);
		SetScrolloffsX(0);
	}
}

void TextField::ScrollToPosition(ScrollBar* source, int position)
{
	if (source == NULL)
		return;
	if (source == vert_bar)
	{
		SkScalar pos_y = position;
		//printf("scroll before pos=%f\n",pos_y);
		pos_y = std::min((float)0, (float)pos_y);
		pos_y = std::max((float)(-(ContentInfo.height - GetDisplayHeigth())), (float)pos_y);
		//	printf("scroll pos=%f\n",pos_y);
		SetScrolloffsY(pos_y);

	}

	if (source == hori_bar)
	{
		SkScalar pos_x = position;
		pos_x = std::min((float)0, (float)pos_x);
		pos_x = std::max((float)(-(ContentInfo.width - GetDisplayWidth())), (float)pos_x);
		SetScrolloffsX(pos_x);
	}
}
