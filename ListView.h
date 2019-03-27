#pragma once
#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "ScrollView.h"
#include "button.h"
//#include "ListHead.h"

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

#define LIST_STYLE_HEAD        1
#define LIST_STYLE_VERTGLINE   2   
#define LIST_STYLE_HORZGLINE   4  

class RowItem {
public:
//private:
	std::vector<CellItem> celllist;
	int nRowHeigth;
};

class ListHead;
class ListView :public UIWidget ,public ScrollBarController 
{
public:

	enum ListAligment
	{
		LEFT,
		CENTER,
		RIGHT
	};
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

	//void SetContentSize(SkScalar width, SkScalar height);
	void UpdateScrollBarInfo();

	void SetViewStyle(int nStyle);

	int GetViewStyle()
	{
		return nViewStyle;
	}
	ListAligment GetListAligment()
	{
		return nAligment;
	}

	SkScalar GetDisplayWidth();
	SkScalar GetDisplayHeigth();

	void Sort(int nCol);

	SkPoint ScrollViewToChildPoint(int x, int y);

	std::vector<ColumnInfo> GetColList()
	{
		return collist;
	}
	//void AddColumn(ColumnInfo col);
private:
	ScrollContentInfo ContentInfo;
	ScrollBar *vert_bar;
	ScrollBar *hori_bar;

	ListHead *header;
	SkColor background;
	std::vector<UIWidget *> displaylist;

	std::vector<RowItem *> rowlist;
	std::vector<ColumnInfo> collist;

	double fDrawTime;

	std::vector<Button *> titbutlist;
	int nViewStyle;
	ListAligment nAligment;
};