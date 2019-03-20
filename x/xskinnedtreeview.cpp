#include "xskinnedtreeview.h"
#include "time.h"

#define TV_ITEM_HEIGHT 40

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedTreeView,XTreeView)
  EVENT_CREATE(XSkinnedTreeView::OnCreate)
  EVENT_CODE_NOTIFY(TVN_SELCHANGING,XSkinnedTreeView::OnSelChanging)
  EVENT_CODE_NOTIFY(TVN_SELCHANGED,XSkinnedTreeView::OnSelChanged)
  EVENT_CODE_NOTIFY(NM_CUSTOMDRAW,XSkinnedTreeView::OnCustomDraw)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedTreeView,XTreeView)

XSkinnedTreeView::XSkinnedTreeView(HWND hwnd):XTreeView(hwnd){
  m_hFont.Create(XFONT_BIGSIZE,0,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,XFONT_FACE);
}

XSkinnedTreeView::~XSkinnedTreeView(){
  m_hFont.Delete();
}

INT XSkinnedTreeView::OnCreate(LPCREATESTRUCT /*pcs*/){
  INT ret=Default();
  SetItemHeight(TV_ITEM_HEIGHT);
  return ret;
}

INT XSkinnedTreeView::OnCustomDraw(LPNMTVCUSTOMDRAW pNMTVCD){
  switch(pNMTVCD->nmcd.dwDrawStage){
    case CDDS_PREPAINT:{
      XDC WDC(pNMTVCD->nmcd.hdc);
      XRect wr(pNMTVCD->nmcd.rc);
      if(wr.IsRectEmpty())
        return 0;
      WDC.FillSolidRect(&wr,XCOLOR_DARKBACKGROUND);
      return CDRF_NOTIFYITEMDRAW;//|CDRF_NOTIFYSUBITEMDRAW);
    }break;
    case CDDS_ITEMPREPAINT:{
      XDC WDC(pNMTVCD->nmcd.hdc);
      XRect wr(pNMTVCD->nmcd.rc);
      if(wr.IsRectEmpty())
        return CDRF_DODEFAULT;

      TV_ITEM tvi;TCHAR sBuffer[256];
      tvi.mask=TVIF_HANDLE|TVIF_TEXT|TVIF_STATE|TVIF_IMAGE;
      tvi.hItem=(HTREEITEM)(DWORD_PTR)pNMTVCD->nmcd.dwItemSpec;
      tvi.pszText=sBuffer;
      tvi.cchTextMax=sizeof(sBuffer);
      tvi.stateMask=(UINT)-1;
      GetItem(&tvi);

      if(tvi.state&TVIS_SELECTED||tvi.state&TVIS_EXPANDED){
        XRect tr=wr;
        ::InflateRect(&tr,-1,-1);
        tr.bottom-=(tr.bottom-tr.top)/2;
        WDC.DrawGradient(&tr,(tvi.state&TVIS_EXPANDED)?XCOLOR_BACKGROUND_INACTIVE:XCOLOR_BACKGROUND_ACTIVE,XCOLOR_BACKGROUND,1);
        tr.bottom=wr.bottom-1;
        tr.top+=(tr.bottom-tr.top)/2;
        WDC.DrawGradient(&tr,XCOLOR_BACKGROUND,(tvi.state&TVIS_EXPANDED)?XCOLOR_BACKGROUND_INACTIVE:XCOLOR_BACKGROUND_ACTIVE,1);
      }else{
        XRect tr=wr;
        ::InflateRect(&tr,-1,-1);
        WDC.FillSolidRect(&tr,XCOLOR_BACKGROUND);
      }

      wr.left+=pNMTVCD->iLevel*15;
      XImageList il(GetImageList(TVSIL_NORMAL));
      if(il.IsValid()){
        POINT pt={wr.left+15,wr.top+4};
        if(il.Draw(WDC,tvi.iImage,pt,TRANSPARENT))
          wr.left+=55;
      }

      HFONT hOldFont=(HFONT)WDC.SelectObject(m_hFont);
      DWORD dwTextFlags=DT_WORD_ELLIPSIS|DT_NOCLIP|DT_VCENTER|DT_SINGLELINE;
      ::InflateRect(&wr,-10,-5);
      WDC.DrawColoredText(sBuffer,&wr,TRANSPARENT,0,(tvi.state&TVIS_SELECTED||tvi.state&TVIS_EXPANDED)?XCOLOR_TEXT_ACTIVE:XCOLOR_TEXT_INACTIVE,dwTextFlags);
      WDC.SelectObject(hOldFont);

      return CDRF_SKIPDEFAULT;
    }break;
    default:break;
  }
  return 0;
}

BOOL XSkinnedTreeView::OnSelChanging(LPNMTREEVIEW ptv){
  if(!ptv->itemNew.lParam){
    if(ptv->itemNew.state&TVIS_EXPANDED)
      return TRUE;
    else{
      HTREEITEM hChild=GetChildItem(ptv->itemNew.hItem);
      if(hChild){
        Expand(ptv->itemNew.hItem);
        SelectItem(hChild);
        return TRUE;
      }
    }
  }
  /*if(!ptv->itemOld.lParam){
    if(ptv->itemOld.state&TVIS_EXPANDED){
      Expand(ptv->itemOld.hItem,TVE_COLLAPSE);
      return TRUE;
    }
  }*/
  return FALSE;
}

VOID XSkinnedTreeView::OnSelChanged(LPNMTREEVIEW ptv){
  if(ptv->itemNew.lParam){
    XWnd *pWin=(XWnd *)ptv->itemOld.lParam;
    if(pWin&&pWin->IsValid()&&pWin->IsVisible())
      pWin->SetWindowPos(NULL,m_x,m_y,m_cx,m_cy,SWP_HIDEWINDOW|SWP_NOZORDER);
    pWin=(XWnd *)ptv->itemNew.lParam;
    if(pWin&&pWin->IsValid()&&!pWin->IsVisible()){
      pWin->SetWindowPos(NULL,m_x,m_y,m_cx,m_cy,SWP_SHOWWINDOW|SWP_NOZORDER);
      m_Client=pWin->GetSafeHandle();
      ::SendMessage(GetParent(),WM_XTVSELCHANGED,0,(LPARAM)pWin);
    }
  }else{
    XWnd *pWin=(XWnd *)ptv->itemOld.lParam;
    if(pWin&&pWin->IsValid()&&pWin->IsVisible())
      pWin->SetWindowPos(NULL,m_x,m_y,m_cx,m_cy,SWP_HIDEWINDOW|SWP_NOZORDER);
    ::SendMessage(GetParent(),WM_XTVSELCHANGED,0,0);
  }
}

VOID XSkinnedTreeView::UpdateClient(INT x,INT y,INT cx,INT cy){
  if(m_Client.IsValid())
    m_Client.SetWindowPos(NULL,x,y,cx,cy,SWP_SHOWWINDOW|SWP_NOZORDER);
  //save size of last active window...
  m_x=x;m_y=y;m_cx=cx;m_cy=cy;
}
