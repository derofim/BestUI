#include "TextField.h"
#include "windows.h"
#pragma comment(lib, "Imm32.lib")
//extern HWND hwnd;
HIMC imm_context;
TextField::TextField() {
    SetBackGroundColor(SkColorSetRGB(230, 230, 230));
    text_color = SkColorSetRGB(0, 0, 0);

  //  imm_context = ::ImmGetContext(hwnd);
   // CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500}, {200, 100, 500, 200}};
   // ImmSetCandidateWindow(imm_context, &exclude_rectangle);
    unindex = 0;
    TextLine info;
    info.nHeight = 20;
    line.push_back(info);
    fCurPosBlinkTime = SkTime::GetMSecs();
}
TextField::~TextField() {}

// HIMC imm_context = ::ImmGetContext(window_handle);
//::ImmSetCandidateWindow(imm_context, &exclude_rectangle);
#include "SkTextBlob.h"
void TextField::Draw(SkCanvas* canvas) {
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

    for (int k = 0; k < line.size(); k++) {
        paint.setColor(SkColorSetRGB(0, 0, 0));

        std::wstring* ptext = GetLineText(k);
        surfaceCanvas->drawSimpleText(ptext->c_str(), ptext->size() * 2, kUTF16_SkTextEncoding,
                                      ins_x, ins_y, font, paint);
        ins_y += line[k].nHeight;
        // font.measureText(textlist.c_str(),textlist.size(), kUTF8_SkTextEncoding, &bounds,
        // &paint); ins_x+=/*bounds.width()*/30+0.5;
    }

    DrawCurPosBlink(surfaceCanvas);
    /*Sequence sq = Sequence(0, [&]() {
        DrawCurPosBlink(surfaceCanvas);
    }, new DelayTime(1.0), 0);
    sq.RunSequence();*/

    //
    //	SkTextBlobBuilder builder;
    //  const auto& run_buffer = builder.allocRunPos(font, textlist.size());
    //
    //  //static_assert(sizeof(*glyphs) == sizeof(*run_buffer.glyphs), "");
    //  memcpy(run_buffer.glyphs, textlist.c_str(), textlist.size());
    //
    ////  static_assert(sizeof(*pos) == 2 * sizeof(*run_buffer.pos), "");
    //  memcpy(run_buffer.pos, 0, textlist.size() );
    //  surfaceCanvas->drawTextBlob(builder.make(), 0, 0, paint);

    // for (int k = 0; k < textlist.size(); k++)
    //{
    //	if (textlist[k].text == 0x0a)
    //	{
    //		inx_y+=20;
    //	}
    //	paint.setColor(textlist[k].color);
    //	SkRect bounds;
    //	if (textlist[k].text < 128)
    //	{
    //		//font.measureText(&textlist[k].text, 1, kUTF8_SkTextEncoding, &bounds, &paint);
    //		// paint.textToGlyphs(bunny, len, glyphs);
    //		 uint16_t glyphs[2];
    //		font.textToGlyphs(&textlist[k].text, 1, kUTF8_SkTextEncoding,&glyphs[0],1);

    //		surfaceCanvas->drawSimpleText(&textlist[k].text, 1, kUTF8_SkTextEncoding, ins_x, inx_y,
    //font, paint);
    //	}
    //	else
    //	{
    //		char pszText[30]="hello world\r\ntest enter\r\n";
    //		/*pszText[0]=0xe5;
    //		pszText[1]=0xae;
    //		pszText[2]=0x8c;
    //		pszText[3]=0x00;*/
    //		font.measureText(pszText, strlen(pszText), kUTF8_SkTextEncoding, &bounds, &paint);
    //		surfaceCanvas->drawSimpleText(pszText ,strlen(pszText), kUTF8_SkTextEncoding, ins_x,
    //inx_y, font, paint);
    //	}
    //	ins_x+=bounds.width()+0.5;
    //	//printf("char width=%f\n",bounds.width());
    //}
    paint.setColor(GetBackGroundColor());
    sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
    canvas->drawImage(image, GetBound().left(), GetBound().top());
}

void TextField::DrawCurPosBlink(SkCanvas* canvas) {
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

void TextField::OnMouseMove(int x, int y) {}

void TextField::OnMouseDown(int x, int y) {
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
    for (k = 0; k < ptext->length(); k++) {
        font.measureText(ptext->c_str(), (k + 1) * 2, kUTF16_SkTextEncoding, &bounds, &paint);
        if (point.x() < bounds.width()) {
            break;
        }
    }
    curpos.x = k;
    curpos.y = nIndex;
}

void TextField::OnMouseUp(int x, int y) {}

void TextField::OnMouseWheel(float delta, uint32_t modifier) {}

void TextField::OnKey(sk_app::Window::Key key, uint32_t modifiers) {
    if (key != sk_app::Window::Key::kBack) {
        //	printf("del back key\n");
        /*TextDesc desc;
        desc.text=(char)key;
        desc.color=text_color;
        textlist.push_back(desc);*/
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

void TextField::OnChar(SkUnichar c, uint32_t modifiers) {
    /*CANDIDATEFORM exclude_rectangle = {0, CFS_EXCLUDE, {500, 500},
      {200, 100,500,200}};
        ImmSetCandidateWindow(imm_context, &exclude_rectangle);*/

    /*printf("insert key=%d,modifiers=%d\n",c,modifiers);
    TextDesc desc;
    desc.text=c;
    desc.color=text_color;
    textlist.push_back(desc);*/

    // printf(" key=%d,modifier=%d,len=%d\n",c,modifiers,textlist.length());

    std::wstring* ptext = GetLineText(curpos.y);

    if (c == 0x08) {
        if (ptext->length() == 0) {
            if (curpos.y >= 1) {
                line.erase(line.begin() + curpos.y);
                curpos.y--;
                curpos.x = GetLineText(curpos.y)->size();
            }
            return;
        }
        if (ptext->length() >= 1) ptext->pop_back();

        curpos.x--;
        return;
    }

    if (c == 0x0d) {
        curpos.y += 1;
        curpos.x = 0;

        TextLine info;
        info.nHeight = 20;
        line.push_back(info);
        return;
    }
    if (c > 0x80) {
        unchar[unindex] = (char)c;
        unindex++;

        if (unindex == 2) {
            std::wstring wtxt = string2wstring(unchar, "Chinese");
            ptext->push_back(wtxt[0]);
            curpos.x++;
        }
        unindex = unindex % 2;
    } else {
        ptext->push_back(c);
        curpos.x++;
    }
}