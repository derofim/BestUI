#pragma once
#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "ScrollView.h"


enum CellTypes {
  TEXT_ONLY = 0,
  ICON_AND_TEXT,
};



struct CellItem {
	UIWidget *pWidget;
	/*UIWidget *pWidget;
	CellTypes type;*/
};


struct ColumnInfo {
	int nOrder;
	int nWidth;
	SkString colname;
};


class RowItem {
public:
//private:
	std::vector<CellItem> celllist;
	int nRowHeigth;
};

class ListView :public UIWidget ,public ScrollBarController 
{
public:
	ListView();
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override;

	void AddCol(ColumnInfo info);
	void AddCol(char *name,int nWidth);

	//void AddCellItem(SkString text);
	//void AddCellItem(UIWidget *pWidget);

	void AddCellItem(UIWidget *pWidget,int nRow,int nCol);

	void CellItemUpdate(UIWidget *pWidget,int nRow,int nCol);
	void CellItemUpdate(SkString text,int nRow,int nCol);

	void DelRow(int nRow);
	void DelAllRow();

	void SetListViewRow(int nRow);

	void ScrollToPosition(ScrollBar* source, int position);

	void SetContentSize(SkScalar width, SkScalar height);

	SkScalar GetDisplayWidth();
	SkScalar GetDisplayHeigth();

	SkPoint ScrollViewToChildPoint(int x, int y) 
	{
		SkPoint point;
		point.set(x - GetBound().left(), y - GetBound().top());
		return point;
	}
	//void AddColumn(ColumnInfo col);
private:
	ScrollContentInfo ContentInfo;
	ScrollBar *vert_bar;
	ScrollBar *hori_bar;
	SkColor background;
	std::vector<UIWidget *> displaylist;

	std::vector<RowItem *> rowlist;
	std::vector<ColumnInfo> collist;
};