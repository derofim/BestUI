#pragma once
#include "UIWidget.h"
#include "GrContext.h"
#include "SkCanvas.h"
#include "ScrollView.h"


enum CellTypes {
  TEXT_ONLY = 0,
  ICON_AND_TEXT,
};

////µ¥Ôª¸ñ

struct CellItem {
	SkString text;
	/*UIWidget *pWidget;
	CellTypes type;*/
};


//struct RowInfo {
//
//};
//struct CellInfo {
//	UIWidget *pWidget;
//	CellTypes types;
//};
//struct ColumnInfo
//{
//	SkString name;
//	int      nOrder;
//	bool     bSort;
//	
//};

class ListView :public ScrollView
{
public:
	void Draw(SkCanvas* canvas) override;
	void OnMouseMove(int x, int y) override;
	void OnMouseDown(int x, int y) override;
	void OnMouseUp(int x,int y) override;
	void OnMouseWheel(float delta, uint32_t modifier) override;
	//void AddColumn(ColumnInfo col);
private:
};