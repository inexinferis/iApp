#include "xskinnedlistview.h"

#define LB_ITEM_HEIGHT 44

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedListView,XListView)
  EVENT_DRAWITEM(XSkinnedListView::OnDrawItem)
  EVENT_MEASUREITEM(XSkinnedListView::OnMeasureItem)
  EVENT_CODE_NOTIFY(LVN_ITEMCHANGED,XSkinnedListView::OnItemChanged)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedListView,XListView)

XSkinnedListView::XSkinnedListView(HWND hwnd):XListView(hwnd){
  m_hFont.Create(XFONT_BIGSIZE,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,XFONT_FACE);
  bNeedChange=FALSE;
}

XSkinnedListView::~XSkinnedListView(){
  m_hFont.Delete();
}

VOID XSkinnedListView::OnMeasureItem(UINT /*uCtrlID*/,LPMEASUREITEMSTRUCT lpmis){
  lpmis->itemHeight=LB_ITEM_HEIGHT;
}

BOOL XSkinnedListView::OnDrawItem(UINT /*uCtrlID*/,PDRAWITEMSTRUCT pDIS){
  XDC WDC(pDIS->hDC);
  XRect wr(pDIS->rcItem);
  wr.right-=2;

  LV_ITEM lvi;
  lvi.mask=LVIF_STATE|LVIF_IMAGE;
  lvi.iItem=pDIS->itemID;
  lvi.iSubItem=0;
  lvi.stateMask=(UINT)-1;     // get all state flags
  GetItem(&lvi);

  XRect rcBounds;
  GetItemRect(pDIS->itemID,&rcBounds,LVIR_BOUNDS);

  TCHAR sBuffer[256];
  GetItemText(pDIS->itemID,0,sBuffer,sizeof(sBuffer));

  if(lvi.state&LVIS_FOCUSED){
    XRect tr=wr;
    ::InflateRect(&tr,-1,-1);
    tr.bottom-=(tr.bottom-tr.top)/2;
    WDC.DrawGradient(&tr,XCOLOR_BACKGROUND_ACTIVE,XCOLOR_BACKGROUND,1);
    tr.bottom=wr.bottom-1;
    tr.top+=(tr.bottom-tr.top)/2;
    WDC.DrawGradient(&tr,XCOLOR_BACKGROUND,XCOLOR_BACKGROUND_ACTIVE,1);
  }else{
    XRect tr=wr;
    ::InflateRect(&tr,-1,-1);
    WDC.FillSolidRect(&tr,XCOLOR_BACKGROUND);
  }

  XImageList il(GetImageList(LVSIL_NORMAL));
  if(il.IsValid()){
    POINT pt={wr.left+22,wr.top+6};
    if(il.Draw(WDC,lvi.iImage,pt,TRANSPARENT))
      wr.left+=64;
  }

  HFONT hOldFont=(HFONT)WDC.SelectObject(m_hFont);
  DWORD dwTextFlags=DT_WORD_ELLIPSIS|DT_NOCLIP|DT_VCENTER|DT_SINGLELINE;
  ::InflateRect(&wr,-10,-5);
  WDC.DrawColoredText(sBuffer,&wr,TRANSPARENT,0,(lvi.state&LVIS_FOCUSED)?XCOLOR_TEXT_ACTIVE:XCOLOR_TEXT_INACTIVE,dwTextFlags);
  WDC.SelectObject(hOldFont);
  return TRUE;
}

VOID XSkinnedListView::OnItemChanged(LPNMLISTVIEW plv){
  XWnd *pWin=(XWnd *)GetItemData(plv->iItem);
  if(pWin){
    if(plv->uNewState&LVIS_FOCUSED){
      if(pWin->IsValid()&&!pWin->IsVisible()){
        pWin->SetWindowPos(NULL,m_x,m_y,m_cx,m_cy,SWP_SHOWWINDOW|SWP_NOZORDER);
        bNeedChange=TRUE;
      }
    }else if(plv->uOldState&LVIS_FOCUSED&&bNeedChange){
      if(pWin->IsValid()&&pWin->IsVisible())
        pWin->SetWindowPos(NULL,m_x,m_y,m_cx,m_cy,SWP_HIDEWINDOW|SWP_NOZORDER);
      bNeedChange=FALSE;
    }
  }
}

VOID XSkinnedListView::UpdateClient(INT x,INT y,INT cx,INT cy){
  INT iItem=GetSelectedItem(LVNI_FOCUSED);
  if(iItem!=-1){
    XWnd *pWin=(XWnd *)GetItemData(iItem);
    if(pWin&&pWin->IsValid())
      pWin->SetWindowPos(NULL,x,y,cx,cy,SWP_SHOWWINDOW|SWP_NOZORDER);
    //save size of last active window...
    m_x=x;m_y=y;m_cx=cx;m_cy=cy;
  }
}
