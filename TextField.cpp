#include "TextField.h"
#include "windows.h"
#pragma comment(lib, "Imm32.lib")

HIMC imm_context;
TextField::TextField() 
{
    SetBackGroundColor(SkColorSetRGB(230, 230, 230));
    text_color = SkColorSetRGB(0, 0, 0);

    //imm_context = ::ImmGetContext(hwnd);
    //CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500}, {200, 100, 500, 200}};
    //ImmSetCandidateWindow(imm_context, &exclude_rectangle);
    unindex = 0;
    TextLine info;
    info.nHeight = 20;
    line.push_back(info);
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
    GrContext* context = canvas->getGrContext();
    SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
    auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
    SkPaint paint;
    paint.setColor(GetBackGroundColor());
    surfaceCanvas->drawRect(SkRect{0, 0, GetWidth(), GetHeight()}, paint);
    SkScalar ins_x = 0;
    SkScalar ins_y = 20;
    SkFont font;
    font.setSubpixel(true);
    font.setSize(16);

    for (int k = 0; k < line.size(); k++) 
	{
        paint.setColor(SkColorSetRGB(0, 0, 0));

        std::wstring* ptext = GetLineText(k);
        surfaceCanvas->drawSimpleText(ptext->c_str(), ptext->size() * 2, kUTF16_SkTextEncoding,ins_x, ins_y, font, paint);
        ins_y += line[k].nHeight;
    }

    DrawCurPosBlink(surfaceCanvas);
    /*Sequence sq = Sequence(0, [&]() {
        DrawCurPosBlink(surfaceCanvas);
    }, new DelayTime(1.0), 0);
    sq.RunSequence();*/

    paint.setColor(GetBackGroundColor());
    sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
    canvas->drawImage(image, GetBound().left(), GetBound().top());
}

void TextField::DrawCurPosBlink(SkCanvas* canvas) 
{
    // double diff = SkTime::GetMSecs() - fCurPosBlinkTime;
    // printf("diff=%f\n", diff);
    // if (SkTime::GetMSecs() - fCurPosBlinkTime >= 100)
    {
        fCurPosBlinkTime = SkTime::GetMSecs();
        SkScalar x, y;
        y = curpos.y * 20;
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

void TextField::OnMouseMove(int x, int y) 
{
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

void TextField::OnMouseDown(int x, int y) 
{
    SkPoint point = ScrollViewToChildPoint(x, y);

    if (point.y() > line.size() * 20) return;
    int nIndex = point.y() / 20;

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
}

void TextField::OnMouseWheel(float delta, uint32_t modifier) 
{
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
#include <codecvt>  // std::codecvt_utf8
#include <cstdint>  // std::uint_least32_t
#include <locale>   // std::wstring_convert
#include <string>   // std::string, std::u32string

std::string wstring2utf8string(const std::wstring& str) {
    static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}

std::string wstring2utf16string(const std::wstring& str) {
    static std::wstring_convert<std::codecvt_utf16<wchar_t> > strCnv;
    return strCnv.to_bytes(str);
}

std::wstring utf8string2wstring(const std::string& str) {
    static std::wstring_convert<std::codecvt_utf8<wchar_t> > strCnv;
    return strCnv.from_bytes(str);
}

std::string wstring2string(const std::wstring& str, const std::string& locale) {
    typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
    static std::wstring_convert<F> strCnv(new F(locale));
    return strCnv.to_bytes(str);
}

std::wstring string2wstring(const std::string& str, const std::string& locale) {
    typedef std::codecvt_byname<wchar_t, char, std::mbstate_t> F;
    static std::wstring_convert<F> strCnv(new F(locale));
    return strCnv.from_bytes(str);
}

std::wstring* TextField::GetLineText(int nLine) {
    if (nLine > line.size()) return 0;
    return &line[nLine].line_text;
}

void TextField::OnChar(SkUnichar c, uint32_t modifiers)
{
    /*CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500},
      {200, 100,500,200}};
        ImmSetCandidateWindow(imm_context, &exclude_rectangle);*/

   

    std::wstring* ptext = GetLineText(curpos.y);
    printf("cur line=%d,text=%s\n", curpos.y, wstring2string(*ptext, "chinese").c_str());
    if (c == 0x08) 
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
            return;
        }
        if (ptext->length() >= 1) 
		{
            // ptext->pop_back();
            ptext->erase(curpos.x - 1, 1);
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
        /*curpos.y += 1;
        curpos.x = 0;

        TextLine info;
        info.nHeight = 20;
        line.push_back(info);*/
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

        // ptext->push_back(c);
        curpos.x++;
    }
}