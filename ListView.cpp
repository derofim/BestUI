#include "ListView.h"

ListView::ListView()
{
	collist.clear();
	rowlist.clear();

	SetPosition(0, 0);
	SetSize(0, 0);
	memset(&ContentInfo, 0x00, sizeof(ContentInfo));
	vert_bar = new ScrollBar(Direction::Vertical);
	hori_bar = new ScrollBar(Direction::Horizontal);
	vert_bar->set_controller(this);
	hori_bar->set_controller(this);

	background = SkColorSetRGB(255, 255, 255);

	SetScrolloffsX(0);
	SetScrolloffsY(0);
}

void ListView::AddCol(ColumnInfo info)
{
	collist.push_back(info);
}
void ListView::AddCol(char * name, int nWidth)
{
	ColumnInfo info;
	info.colname=name;
	info.nWidth=nWidth;
	info.nOrder=collist.size();
	collist.push_back(info);
}

void ListView::SetListViewRow(int nRow)
{
	if(rowlist.size()>nRow)
		return;
	int nAddRow=nRow-rowlist.size()+1;
	CellItem cell;
	memset(&cell,0x00,sizeof(CellItem));
	for (int k = 0; k < nAddRow; k++)
	{
		RowItem *rowit=new RowItem();
		rowit->nRowHeigth=25;
		rowlist.push_back(rowit);
		for (int j = 0; j < collist.size(); j++)
		{
			rowit->celllist.push_back(cell);
		}
	}
}

void ListView::AddCellItem(UIWidget *pWidget,int nRow,int nCol)
{
	SetListViewRow(nRow);
	CellItemUpdate(pWidget,nRow,nCol);
}


void ListView::CellItemUpdate(UIWidget *pWidget,int nRow,int nCol)
{
	rowlist.at(nRow)->celllist.at(nCol).pWidget=pWidget;
	pWidget->SetPosition(0,0);
	pWidget->SetSize(collist.at(nCol).nWidth,rowlist.at(nRow)->nRowHeigth);
}


void ListView::CellItemUpdate(SkString text,int nRow,int nCol)
{
}

void ListView::DelRow(int nRow)
{
}

void ListView::DelAllRow()
{
}

void ListView::Draw(SkCanvas* canvas) 
{

	ContentInfo.offs_y = GetScrolloffsY();
	SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	ContentInfo.offs_x = GetScrolloffsX();
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;
	/*SkScalar diff_y = ContentInfo.offs_y - ContentInfo.preoffs_y;
	SkScalar diff_x = ContentInfo.offs_x - ContentInfo.preoffs_x;*/

	ContentInfo.preoffs_y = ContentInfo.offs_y;
	ContentInfo.preoffs_x = ContentInfo.offs_x;
	GrContext* context = canvas->getGrContext();
	SkImageInfo info = SkImageInfo::MakeN32(GetWidth(), GetHeight(), kOpaque_SkAlphaType);
	auto gpuSurface(SkSurface::MakeRenderTarget(context, SkBudgeted::kNo, info));
    auto surfaceCanvas = gpuSurface->getCanvas();
	SkPaint paint;
	paint.setColor(background);
	surfaceCanvas->drawRect(SkRect{ 0,0,GetWidth(),GetHeight() }, paint);
	int nOffsetY=0;
	int nOffsetX=0;

	displaylist.clear();
	for (auto iter = rowlist.begin(); iter != rowlist.end(); iter++)
	{
		RowItem *currow=(*iter);
	    nOffsetY+=currow->nRowHeigth;
		if (nOffsetY >= (-ContentInfo.offs_y) && nOffsetY <= (-ContentInfo.offs_y) + GetDisplayHeigth()+currow->nRowHeigth)
		{
			nOffsetX=0;
			int nCol=0;
			for (auto iter =currow->celllist.begin(); iter != currow->celllist.end(); iter++,nCol++)
			{
				nOffsetX+=collist.at(nCol).nWidth;
				if (nOffsetX >= (-ContentInfo.offs_x) && nOffsetX <= (-ContentInfo.offs_x) + GetDisplayWidth()+collist.at(nCol).nWidth)
				{
					UIWidget *pChild = (*iter).pWidget;
					SkScalar top=(nOffsetY-(-ContentInfo.offs_y))-currow->nRowHeigth;
					SkScalar bottom=top+currow->nRowHeigth;
					SkScalar left=(nOffsetX-(-ContentInfo.offs_x))-collist.at(nCol).nWidth;
					SkScalar rigth=left+collist.at(nCol).nWidth;
					pChild->SetBound(left,top,rigth,bottom);
					displaylist.push_back(pChild);
				}
			}
		}
	}


	//for (auto iter = childlist.begin(); iter != childlist.end(); iter++)
	//{
	//    UIWidget *pChild = *iter;
	//    pChild->SetBound(pChild->GetBound().left()+diff_x, pChild->GetBound().top() + diff_y, pChild->GetWidth()+diff_x + pChild->GetBound().left(), pChild->GetBound().top() + pChild->GetHeight() + diff_y);
	//	if (pChild->GetBound().right() >= 0 && pChild->GetBound().left() <= GetDisplayWidth() && pChild->GetBound().bottom() >= 0 && pChild->GetBound().top() <= GetDisplayHeigth())
	//		displaylist.push_back(pChild);
	//}


	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->Draw(surfaceCanvas);
	}

	if (vert_bar != NULL)
	{
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.height;
		barinfo.DisplaySize=vert_bar->GetHeight();
		barinfo.offset=ContentInfo.offs_y;
		vert_bar->SetScrollBarInfo(barinfo);
		vert_bar->Draw(surfaceCanvas);
	}

	if (hori_bar != NULL)
	{
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.width;
		barinfo.DisplaySize=hori_bar->GetWidth();
		barinfo.offset=ContentInfo.offs_x;
		hori_bar->SetScrollBarInfo(barinfo);
		hori_bar->Draw(surfaceCanvas);
	}
	
	sk_sp<SkImage> image(gpuSurface->makeImageSnapshot());
	canvas->drawImage(image, GetBound().left(), GetBound().top());
}
void ListView::OnMouseMove(int x, int y)
{
	SkPoint point=ScrollViewToChildPoint(x,y);
	if (vert_bar != NULL && vert_bar->IsVisible())
		vert_bar->OnMouseMove(point.x(), point.y());

	if (hori_bar != NULL && hori_bar->IsVisible())
		hori_bar->OnMouseMove(point.x(), point.y());


	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		pChild->OnMouseMove(point.x(), point.y());
	}
}
void ListView::OnMouseDown(int x, int y)
{
	SkPoint point=ScrollViewToChildPoint(x,y);
	if (vert_bar != NULL)
	{
		if (point.x() >= vert_bar->GetBound().left() && point.x() <= vert_bar->GetBound().right() && point.y() >= vert_bar->GetBound().top() && point.y() <= vert_bar->GetBound().bottom())
		  return  vert_bar->OnMouseDown(point.x(), point.y());
	}

	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		if (point.x() >= hori_bar->GetBound().left() && point.x() <= hori_bar->GetBound().right() && point.y() >= hori_bar->GetBound().top() && point.y() <= hori_bar->GetBound().bottom())
		   return  hori_bar->OnMouseDown(point.x(), point.y());
	}
	for (auto iter = displaylist.begin(); iter != displaylist.end(); iter++)
	{
		UIWidget *pChild = *iter;
		if (point.x() >= pChild->GetBound().left() && point.x() <= pChild->GetBound().right() && point.y() >= pChild->GetBound().top() && point.y() <= pChild->GetBound().bottom())
		{
			//printf("x=%d,y=%d,child_x=%d,child_y=%d,left=%f,right=%f,top=%f,bottom=%f\n", x, y, point.x(), point.y(), pChild->GetBound().left(), pChild->GetBound().right(), pChild->GetBound().top(), pChild->GetBound().bottom());
			return pChild->OnMouseDown(point.x(), point.y());
		}
		
	}
}
void ListView::OnMouseUp(int x, int y)
{
	SkPoint point=ScrollViewToChildPoint(x,y);
	if (vert_bar != NULL && vert_bar->IsVisible())
	{
		vert_bar->OnMouseUp(point.x(), point.y());
	}
	if (hori_bar != NULL && hori_bar->IsVisible())
	{
		hori_bar->OnMouseUp(point.x(), point.y());
	}
}
void ListView::OnMouseWheel(float delta, uint32_t modifier)
{
	if(vert_bar==NULL || !vert_bar->IsVisible())
		return;
	ScrollToPosition(vert_bar,GetScrolloffsY()+delta*10);
}

SkScalar ListView::GetDisplayWidth()
{
	SkScalar width=GetBound().width();
	if (ContentInfo.height > GetBound().height() && vert_bar->IsVisible())
		width-=/*vert_bar->GetWidth()*/10;
	return width;
}
SkScalar ListView::GetDisplayHeigth() 
{
	SkScalar heigth=GetBound().height();
	if (ContentInfo.width > GetBound().width() && hori_bar->IsVisible())
		heigth-=/*hori_bar->GetHeight()*/10;
	return heigth;
}

void ListView::ScrollToPosition(ScrollBar* source, int position)
{
	if(source==NULL)
		return;
	if (source == vert_bar)
	{
		SkScalar pos_y=position;
		//printf("scroll before pos=%f\n",pos_y);
		pos_y=std::min((float)0,(float)pos_y);
		pos_y=std::max((float)(-(ContentInfo.height - GetDisplayHeigth())),(float)pos_y);
	//	printf("scroll pos=%f\n",pos_y);
		SetScrolloffsY(pos_y);

	}

	if (source == hori_bar)
	{
		SkScalar pos_x=position;
		pos_x=std::min((float)0,(float)pos_x);
		pos_x=std::max((float)(-(ContentInfo.width - GetDisplayWidth())),(float)pos_x);
		SetScrolloffsX(pos_x);
	}
}


void ListView::SetContentSize(SkScalar width, SkScalar height)
{
	ContentInfo.height = height;
	ContentInfo.width = width;
	if (ContentInfo.height > GetBound().height())
	{
		vert_bar->SetPosition(GetBound().width()-10,0);
		vert_bar->SetSize(10,GetDisplayHeigth());
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.height;
		barinfo.DisplaySize=vert_bar->GetHeight();
		barinfo.offset=0;
		vert_bar->SetScrollBarInfo(barinfo);
	}

	if (ContentInfo.width > GetBound().width())
	{
		hori_bar->SetPosition(0,GetBound().height()-10);
		hori_bar->SetSize(GetDisplayWidth(),10);
		ScrollBarInfo barinfo;
		barinfo.ContentSize=ContentInfo.width;
		barinfo.DisplaySize=hori_bar->GetWidth();
		barinfo.offset=0;
		hori_bar->SetScrollBarInfo(barinfo);
	}
}