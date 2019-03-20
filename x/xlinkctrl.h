#ifndef _XLINKCTRL_H_
#define _XLINKCTRL_H_

#include "xwnd.h"

#if (_WIN32_WINNT >= 0x0501)

#define XDEFLINKCTRLSTYLE WS_CHILD|WS_CLIPCHILDREN|WS_VISIBLE

class XLinkCtrl:public XWnd{
  DECLARE_DYNAMIC(XLinkCtrl)
  public:
    XLinkCtrl(HWND hwnd=NULL):XWnd(hwnd){}
    virtual ~XLinkCtrl(){}

    virtual HWND Create(HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLINKCTRLSTYLE,LPVOID lpParam=NULL){
      return XWnd::Create(WC_LINK,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    virtual HWND CreateEx(DWORD dwStyleEx,HWND pParentWnd,LPCTSTR lpszWindowName,UINT CtrlId,const RECT& rect=XDEFAULTRECT,DWORD dwStyle=XDEFLINKCTRLSTYLE,LPVOID lpParam=NULL){
      return XWnd::CreateEx(dwStyleEx,WC_LINK,lpszWindowName,dwStyle,rect.left,rect.top,rect.right,rect.bottom,pParentWnd,(HMENU)(UINT_PTR)CtrlId,lpParam);
    }

    INT GetIdealHeight()const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LM_GETIDEALHEIGHT,0,0);
    }

    BOOL SetItem(PLITEM pItem){
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(pItem!=NULL)
      return (BOOL)::SendMessage(m_hWnd,LM_SETITEM,0,(LPARAM)pItem);
    }

    BOOL SetItemID(INT iLink,LPCWSTR szID){
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(wcslen(szID)<=MAX_LINKID_TEXT)
      if(wcslen(szID)>MAX_LINKID_TEXT)
        return FALSE;
      LITEM item;
      item.mask=LIF_ITEMINDEX|LIF_ITEMID;
      item.iLink=iLink;
      wcscpy(item.szID,szID);
      return SetItem(&item);
    }

    BOOL SetItemUrl(INT iLink,LPCWSTR szUrl){
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(wcslen(szUrl)<=L_MAX_URL_LENGTH)
      if(wcslen(szUrl)>=L_MAX_URL_LENGTH)
        return FALSE;
      LITEM item;
      item.mask=LIF_ITEMINDEX|LIF_URL;
      item.iLink=iLink;
      wcscpy(item.szUrl,szUrl);
      return SetItem(&item);
    }

    BOOL SetItemState(INT iLink,UINT state,UINT stateMask=(UINT)-1){
      XASSERT(::IsWindow(m_hWnd))
      LITEM item;
      item.mask=LIF_ITEMINDEX|LIF_STATE;
      item.iLink=iLink;
      item.state=state;
      item.stateMask=stateMask;
      return SetItem(&item);
    }

    BOOL GetItem(PLITEM pItem)const{
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(pItem != NULL)
      return (BOOL)::SendMessage(m_hWnd,LM_GETITEM,0,(LPARAM)pItem);
    }

    BOOL GetItemID(INT iLink,LPWSTR szID,UINT cchID)const{
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(szID!=NULL)
      XASSERT(cchID>=MAX_LINKID_TEXT)
      if(szID==NULL||cchID<MAX_LINKID_TEXT)
        return FALSE;
      if(szID!=NULL)
        szID[0]=0;
      LITEM item;
      item.mask=LIF_ITEMINDEX|LIF_ITEMID;
      item.iLink=iLink;
      if(!GetItem(&item))
        return FALSE;
      wcsncpy(szID,item.szID,cchID);
      return TRUE;
    }


    BOOL GetItemUrl(INT iLink,LPWSTR szUrl,UINT cchUrl)const{
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(szUrl!=NULL)
      XASSERT(cchUrl>=L_MAX_URL_LENGTH)
      if(szUrl==NULL||cchUrl<L_MAX_URL_LENGTH)
        return FALSE;
      if(szUrl!=NULL)
        szUrl[0]=0;
      LITEM item;
      item.mask=LIF_ITEMINDEX|LIF_URL;
      item.iLink=iLink;
      if (!GetItem(&item))
        return FALSE;
      wcsncpy(szUrl,item.szUrl,cchUrl);
      return TRUE;
    }

    BOOL GetItemState(INT iLink,UINT *pnState,UINT stateMask=(UINT)-1)const{
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(pnState!=NULL)
      if(pnState==NULL)
        return FALSE;
      LITEM item;
      item.mask=LIF_ITEMINDEX|LIF_URL;
      item.iLink=iLink;
      item.stateMask=stateMask;
      if(!GetItem(&item))
        return FALSE;
      *pnState=item.state;
      return TRUE;
    }

    BOOL HitTest(PLHITTESTINFO phti)const{
      XASSERT(::IsWindow(m_hWnd))
      XASSERT(phti!=NULL)
      return (BOOL)::SendMessage(m_hWnd,LM_HITTEST,0,(LPARAM)phti);
    }

  #if (_WIN32_WINNT >= 0x0600) && defined(UNICODE)

    INT GetIdealSize(INT cxMaxWidth,SIZE* pSize)const{
      XASSERT(::IsWindow(m_hWnd))
      return (INT)::SendMessage(m_hWnd,LM_GETIDEALSIZE,(WPARAM)cxMaxWidth,(LPARAM)pSize);
    }

  #endif

  private:

    DECLARE_EVENT_TABLE()
};

#endif

#endif // _XLINKCTRL_H_
