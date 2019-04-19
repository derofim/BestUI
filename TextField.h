#pragma once

#include <deque>
#include "GrContext.h"
#include "ScrollView.h"
#include "SkCanvas.h"
#include "UIWidget.h"
#include "ScrollView.h"
struct TextDesc {
    SkColor color;
    SkUnichar text;
};

// struct

struct TextPoint {
    int x = 0;
    int y = 0;
};

struct TextLine {
    std::wstring line_text;
    int nHeight;
};



class TextField : public UIWidget , public ScrollBarController {
public:
    TextField();
    ~TextField();

    void Draw(SkCanvas* canvas) override;
    void OnMouseMove(int x, int y) override;
    void OnMouseDown(int x, int y) override;
    void OnMouseUp(int x, int y) override;
    void OnMouseWheel(float delta, uint32_t modifier) override;
    void OnKey(sk_app::Window::Key key, uint32_t modifiers) override;
    void OnChar(SkUnichar c, uint32_t modifiers) override;

    std::wstring* GetLineText(int nLine);

    void DrawCurPosBlink(SkCanvas* canvas);

    int FindSuitX(std::wstring text, int nCmpX);
    void SetLastX();
	SkScalar GetDisplayWidth();
	SkScalar GetDisplayHeigth();
	void SetContentSize(SkScalar width, SkScalar height);
	void ScrollToPosition(ScrollBar* source, int position);
	void UpdateScrollBarInfo();
    SkPoint ScrollViewToChildPoint(int x, int y) {
        SkPoint point;
        point.set(x - GetBound().left(), y - GetBound().top());
        return point;
    }

private:
    // std::deque<TextDesc> textlist;
    ScrollBar* vert_bar;
    ScrollBar* hori_bar;
	ScrollContentInfo ContentInfo;
    SkColor text_color;

    std::deque<TextLine> line;
    // std::wstring textlist;
    TextPoint curpos;

    int nLastX;
    char unchar[2];
    int unindex;

    double fCurPosBlinkTime;
    // SkString
};

// DrawStringRectWithFlag

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