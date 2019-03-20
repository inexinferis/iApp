#ifndef _XMENU_H_
#define _XMENU_H_

#include "xobject.h"

class XMenu:public XObject{
	DECLARE_DYNCREATE(XMenu)
  public:
    XMenu(HMENU hMenu=NULL):m_hMenu(hMenu),m_bAttached(FALSE){}
    virtual ~XMenu();

    BOOL IsValid(){
      return (this==NULL?FALSE:(m_hMenu!=NULL&&IsMenu(m_hMenu)));
    }

    static XMenu *FromHandle(HMENU hMenu);
    static XMenu *FromHandlePermanent(HMENU hMenu);
    static XMenu *FindSubMenuFromID(HMENU hMenu,UINT nID,DWORD dwItemData=0);
    static XMenu *FindPopupMenuFromID(UINT nID,DWORD dwItemData=0);

    HMENU Attach(HMENU hMenu);
    HMENU Detach();

    HMENU GetSafeHandle()const{
      return (this==NULL?NULL:m_hMenu);
    }

    HMENU CreateMenu(){
      return Attach(::CreateMenu());
    }

    HMENU CreatePopupMenu(){
      return Attach(::CreatePopupMenu());
    }

    HMENU LoadMenu(HINSTANCE hInstance,LPCTSTR lpszResourceName){
      return Attach(::LoadMenu(hInstance,lpszResourceName));
    }

    HMENU LoadMenu(HINSTANCE hInstance,UINT nIDResource){
      return Attach(::LoadMenu(hInstance,MAKEINTRESOURCE(nIDResource)));
    }

    HMENU LoadMenuIndirect(const void* lpMenuTemplate){
      return Attach(::LoadMenuIndirect(lpMenuTemplate));
    }

    BOOL AppendMenu(UINT nFlags,UINT_PTR nIDNewItem=0,LPCTSTR lpszNewItem=NULL){
      XASSERT(::IsMenu(m_hMenu))
      return ::AppendMenu(m_hMenu,nFlags,nIDNewItem,lpszNewItem);
    }

    BOOL AppendMenu(UINT nFlags,UINT_PTR nIDNewItem,const HBITMAP hBmp){
      XASSERT(::IsMenu(m_hMenu))
      return ::AppendMenu(m_hMenu,nFlags|MF_BITMAP,nIDNewItem,(LPCTSTR)hBmp);
    }

    UINT CheckMenuItem(UINT nIDCheckItem,UINT nCheck){
      XASSERT(::IsMenu(m_hMenu))
      return (UINT)::CheckMenuItem(m_hMenu,nIDCheckItem,nCheck);
    }

    UINT EnableMenuItem(UINT nIDEnableItem,UINT nEnable){
      XASSERT(::IsMenu(m_hMenu))
      return ::EnableMenuItem(m_hMenu,nIDEnableItem,nEnable);
    }

    BOOL SetDefaultItem(UINT uItem,BOOL fByPos=FALSE){
      XASSERT(::IsMenu(m_hMenu))
      return ::SetMenuDefaultItem(m_hMenu,uItem,fByPos);
    }

    UINT GetDefaultItem(UINT gmdiFlags,BOOL fByPos=FALSE){
      XASSERT(::IsMenu(m_hMenu))
      return ::GetMenuDefaultItem(m_hMenu,fByPos,gmdiFlags);
    }

    INT GetMenuItemCount()const{
      XASSERT(::IsMenu(m_hMenu))
      return ::GetMenuItemCount(m_hMenu);
    }

    UINT GetMenuItemID(INT nPos)const{
      XASSERT(::IsMenu(m_hMenu))
      return ::GetMenuItemID(m_hMenu,nPos);
    }

    UINT GetMenuState(UINT nID,UINT nFlags)const{
      XASSERT(::IsMenu(m_hMenu))
      return ::GetMenuState(m_hMenu,nID,nFlags);
    }

    INT GetMenuString(UINT nIDItem,LPTSTR lpString,INT nMaxCount,UINT nFlags)const{
      XASSERT(::IsMenu(m_hMenu))
      return ::GetMenuString(m_hMenu,nIDItem,lpString,nMaxCount,nFlags);
    }

    BOOL GetMenuItemInfo(UINT uItem,LPMENUITEMINFO lpMenuItemInfo,BOOL fByPos=FALSE){
      XASSERT(::IsMenu(m_hMenu)&&lpMenuItemInfo!=NULL)
      return ::GetMenuItemInfo(m_hMenu,uItem,fByPos,lpMenuItemInfo);
    }

    BOOL SetMenuItemInfo(UINT uItem,LPMENUITEMINFO lpMenuItemInfo,BOOL fByPos=FALSE){
      XASSERT(::IsMenu(m_hMenu)&&lpMenuItemInfo!=NULL)
      return ::SetMenuItemInfo(m_hMenu,uItem,fByPos,lpMenuItemInfo);
    }

    HMENU GetSubMenu(INT nPos)const{
      XASSERT(::IsMenu(m_hMenu))
      return ::GetSubMenu(m_hMenu,nPos);
    }

    BOOL InsertMenu(UINT nPosition,UINT nFlags,UINT_PTR nIDNewItem=0,LPCTSTR lpszNewItem=NULL){
      XASSERT(::IsMenu(m_hMenu))
      return ::InsertMenu(m_hMenu,nPosition,nFlags,nIDNewItem,lpszNewItem);
    }

    BOOL InsertMenu(UINT nPosition,UINT nFlags,UINT_PTR nIDNewItem,const HBITMAP hBmp){
      XASSERT(::IsMenu(m_hMenu))
      return ::InsertMenu(m_hMenu,nPosition,nFlags|MF_BITMAP,nIDNewItem,(LPCTSTR)hBmp);
    }

    BOOL InsertMenuItem(UINT uItem,LPMENUITEMINFO lpMenuItemInfo,BOOL fByPos=FALSE){
      XASSERT(::IsMenu(m_hMenu)&&lpMenuItemInfo!=NULL)
      return ::InsertMenuItem(m_hMenu,uItem,fByPos,lpMenuItemInfo);
    }

    BOOL ModifyMenu(UINT nPosition,UINT nFlags,UINT_PTR nIDNewItem=0,LPCTSTR lpszNewItem=NULL){
      XASSERT(::IsMenu(m_hMenu))
      return ::ModifyMenu(m_hMenu,nPosition,nFlags,nIDNewItem,lpszNewItem);
    }

    BOOL ModifyMenu(UINT nPosition,UINT nFlags,UINT_PTR nIDNewItem,const HBITMAP hBmp){
      XASSERT(::IsMenu(m_hMenu))
      return ::ModifyMenu(m_hMenu,nPosition,nFlags|MF_BITMAP,nIDNewItem,(LPCTSTR)hBmp);
    }

    BOOL RemoveMenu(UINT nPosition,UINT nFlags){
      XASSERT(::IsMenu(m_hMenu))
      return ::RemoveMenu(m_hMenu,nPosition,nFlags);
    }

    BOOL SetMenuItemBitmaps(UINT nPosition,UINT nFlags,const HBITMAP hBmpUnchecked,const HBITMAP hBmpChecked){
      XASSERT(::IsMenu(m_hMenu))
      return ::SetMenuItemBitmaps(m_hMenu,nPosition,nFlags,(HBITMAP)hBmpUnchecked,(HBITMAP)hBmpChecked);
    }

    BOOL SetMenuContextHelpId(DWORD dwContextHelpId){
      XASSERT(::IsMenu(m_hMenu))
      return ::SetMenuContextHelpId(m_hMenu,dwContextHelpId);
    }

    DWORD GetMenuContextHelpId() const{
      XASSERT(::IsMenu(m_hMenu))
      return ::GetMenuContextHelpId(m_hMenu);
    }

    BOOL CheckMenuRadioItem(UINT nIDFirst,UINT nIDLast,UINT nIDItem,UINT nFlags){
      XASSERT(::IsMenu(m_hMenu))
      return ::CheckMenuRadioItem(m_hMenu,nIDFirst,nIDLast,nIDItem,nFlags);
    }

    BOOL TrackPopupMenu(UINT nFlags,INT x,INT y,HWND hWnd,LPCRECT lpRect=NULL){
      XASSERT(m_hMenu!=NULL)
      return ::TrackPopupMenu(m_hMenu,nFlags,x,y,0,hWnd,lpRect);
    }

    BOOL TrackPopupMenuEx(UINT fuFlags,INT x,INT y,HWND hWnd,LPTPMPARAMS lptpm){
      XASSERT(m_hMenu!=NULL)
      return ::TrackPopupMenuEx(m_hMenu,fuFlags,x,y,hWnd,lptpm);
    }

  #if(WINVER >= 0x0500)

    BOOL SetMenuInfo(LPCMENUINFO lpcmi){
      XASSERT(::IsMenu(m_hMenu))
      return ::SetMenuInfo(m_hMenu,lpcmi);
    }

    BOOL GetMenuInfo(LPMENUINFO lpcmi)const{
      XASSERT(::IsMenu(m_hMenu))
      return ::GetMenuInfo(m_hMenu,lpcmi);
    }

  #endif

    BOOL DeleteMenu(UINT nPosition,UINT nFlags){
      XASSERT(::IsMenu(m_hMenu))
      return ::DeleteMenu(m_hMenu,nPosition,nFlags);
    }

    BOOL Destroy();

    XMenu &operator=(HMENU hMenu){
      XASSERT(m_hMenu==NULL)
      m_hMenu=hMenu;
      return *this;
    }

    BOOL operator==(const HMENU hMenu) const{
      return (hMenu==m_hMenu);
    }

    BOOL operator!=(const HMENU hMenu) const{
      return (hMenu!=m_hMenu);
    }

    operator HMENU() const{
      return (this==NULL?NULL:m_hMenu);
    }

    //Events
    virtual BOOL OnDrawItem(HWND /*hWnd*/,LPDRAWITEMSTRUCT /*lpDrawItemStruct*/){return FALSE;}
    virtual BOOL OnMeasureItem(HWND /*hWnd*/,LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/){return FALSE;}
    virtual LRESULT OnMenuChar(HWND /*hWnd*/,UINT /*nChar*/,UINT /*nFlags*/){return MAKELONG(0, MNC_IGNORE);}

  protected:
    HMENU m_hMenu;
    BOOL m_bAttached;
};

#endif // _XMENU_H_
