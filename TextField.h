#pragma once

#include <deque>
#include "GrContext.h"
#include "ScrollView.h"
#include "SkCanvas.h"
#include "UIWidget.h"

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
class TextField : public UIWidget {
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

    SkPoint ScrollViewToChildPoint(int x, int y) {
        SkPoint point;
        point.set(x - GetBound().left(), y - GetBound().top());
        return point;
    }

private:
    // std::deque<TextDesc> textlist;

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

// DrawStringRectWithFlags