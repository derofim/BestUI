#include "UIWidget.h"
#include "GrContext.h"
//#include "ScrollView.h"

enum Direction {
	Vertical,
	Horizontal
};


struct ScrollBarInfo{
	SkScalar ContentSize;
	SkScalar DisplaySize;
	SkScalar offset;
};
class ScrollBar:public UIWidget
{
public:
	enum ScrollAmount {
    SCROLL_NONE = 0,
    SCROLL_START,
    SCROLL_END,
    SCROLL_PREV_LINE,
    SCROLL_NEXT_LINE,
    SCROLL_PREV_PAGE,
    SCROLL_NEXT_PAGE,
   };

	ScrollBar(Direction dir);
	~ScrollBar();
	void SetScrollBarInfo(ScrollBarInfo info);

	//void SetOffset(SkScalar off)
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override {};
	void OnMouseDown(int x, int y) override {};

	void OnMouseWheel(float delta, uint32_t modifier) override {};
private:
	ScrollBarInfo barinfo;
};