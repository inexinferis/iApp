#ifndef _XMENUEX_H_
#define _XMENUEX_H_

#include "xmenu.h"

class XMenuEx:public XMenu{
  DECLARE_DYNCREATE(XMenuEx)
  public:
    XMenuEx():XMenu(NULL),m_mParent(NULL){}
    virtual ~XMenuEx();

    static XMenuEx* FromHandle(HMENU hMenu);
    static XMenuEx* FromItemHandle(HMENU hMenu,UINT iItem,BOOL bByPos=FALSE);
    HMENU  Attach(HMENU hMenu);
    HMENU  Detach();

    //For bitmap,string and bitmap-string
    XMenuEx* Append(INT itemId,LPCTSTR mtitle,UINT mstate=MFS_ENABLED,UINT mtype=MFT_STRING,HBITMAP hBmp=NULL){
      return DoInsert(-1,itemId,mtitle,mstate,mtype,hBmp);
    }

    XMenuEx* AppendRadioCheck(INT itemId,LPCTSTR mtitle,UINT mstate=MFS_ENABLED,HBITMAP hbmpChecked=NULL,HBITMAP hbmpUnchecked=NULL){
      return DoInsert(-1,itemId,mtitle,mstate,MFT_RADIOCHECK,NULL,hbmpChecked,hbmpUnchecked);
    }

    XMenuEx* AppendMenuBreak(){
      return DoInsert(-1,(UINT)-1,NULL,0,MFT_MENUBREAK);
    }

    XMenuEx* AppendMenuBarBreak(){
      return DoInsert(-1,(UINT)-1,NULL,0,MFT_MENUBARBREAK);
    }

    XMenuEx* AppendSeparator(){
      return DoInsert(-1,(UINT)-1,NULL,0,MFT_SEPARATOR);
    }

    //For bitmap,string and bitmap-string
    XMenuEx* Insert(INT itemPos,INT itemId,LPCTSTR mtitle,UINT mstate=MFS_ENABLED,UINT mtype=MFT_STRING,HBITMAP hBmp=NULL){
      return DoInsert(itemPos,itemId,mtitle,mstate,mtype,hBmp);
    }

    XMenuEx* InsertRadioCheck(INT itemPos,INT itemId,LPCTSTR mtitle,UINT mstate=MFS_ENABLED,HBITMAP hbmpChecked=NULL,HBITMAP hbmpUnchecked=NULL){
      return DoInsert(itemPos,itemId,mtitle,mstate,MFT_RADIOCHECK,NULL,hbmpChecked,hbmpUnchecked);
    }

    XMenuEx* InsertMenuBreak(INT itemPos){
      return DoInsert(itemPos,(UINT)-1,NULL,0,MFT_MENUBREAK);
    }

    XMenuEx* InsertMenuBarBreak(INT itemPos){
      return DoInsert(itemPos,(UINT)-1,NULL,0,MFT_MENUBARBREAK);
    }

    XMenuEx* InsertSeparator(INT itemPos){
      return DoInsert(itemPos,(UINT)-1,NULL,0,MFT_SEPARATOR);
    }

    //For bitmap,string and bitmap-string
    XMenuEx* Prepend(INT itemId,LPTSTR mtitle,UINT mstate=MFS_ENABLED,UINT mtype=MFT_STRING,HBITMAP hBmp=NULL){
      return DoInsert(0,itemId,mtitle,mstate,mtype,hBmp);
    }

    XMenuEx* PrependRadioCheck(INT itemId,LPTSTR mtitle,UINT mstate=MFS_ENABLED,HBITMAP hbmpChecked=NULL,HBITMAP hbmpUnchecked=NULL){
      return DoInsert(0,itemId,mtitle,mstate,MFT_RADIOCHECK,NULL,hbmpChecked,hbmpUnchecked);
    }

    XMenuEx* PrependMenuBreak(){
      return DoInsert(0,(UINT)-1,NULL,0,MFT_MENUBREAK);
    }

    XMenuEx* PrependMenuBarBreak(){
      return DoInsert(0,(UINT)-1,NULL,0,MFT_MENUBARBREAK);
    }

    XMenuEx* PrependSeparator(){
      return DoInsert(0,(UINT)-1,NULL,0,MFT_SEPARATOR);
    }

    BOOL Enable(BOOL bEnable=TRUE){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::EnableMenuItem(uPos,(bEnable)?MF_ENABLED:MF_GRAYED|MF_BYPOSITION);
      return FALSE;
    }

    DWORD CheckMenuItem(BOOL bCheck=TRUE){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::CheckMenuItem(uPos,(bCheck)?MF_CHECKED:MF_UNCHECKED|MF_BYPOSITION);
      return (DWORD)-1;
    }

    BOOL CheckMenuRadioItem(UINT idFirst,UINT idLast){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::CheckMenuRadioItem(idFirst,idLast,uPos,MF_BYPOSITION);
      return FALSE;
    }

    BOOL GetMenuItemInfo(LPMENUITEMINFO mii){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::GetMenuItemInfo(uPos,mii,MF_BYPOSITION);
      return FALSE;
    }

    BOOL SetMenuItemInfo(LPMENUITEMINFO mii){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::SetMenuItemInfo(uPos,mii,MF_BYPOSITION);
      return FALSE;
    }

    INT GetMenuString(LPTSTR lpString,INT nMaxCount){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::GetMenuString(uPos,lpString,nMaxCount,MF_BYPOSITION);
      return 0;
    }

    UINT GetMenuState(){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::GetMenuState(uPos,MF_BYPOSITION);
      return (UINT)-1;
    }

    BOOL SetMenuItemBitmaps(HBITMAP hBitmapUnchecked,HBITMAP hBitmapChecked){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return m_mParent->XMenu::SetMenuItemBitmaps(uPos,MF_BYPOSITION,hBitmapUnchecked,hBitmapChecked);
      return FALSE;
    }

    BOOL IsDefaultItem(UINT gmdiFlags=GMDI_USEDISABLED){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        return (uPos==m_mParent->XMenu::GetDefaultItem(TRUE,gmdiFlags));
      return FALSE;
    }

    BOOL DeleteMenu(){
      UINT uPos=0;
      if(m_mParent&&m_mParent->FindMenuItemPosition(this,&uPos))
        if(m_mParent->XMenu::DeleteMenu(uPos,MF_BYPOSITION))
          return DoDestroy();
      return FALSE;
    }

    BOOL Destroy(){
      if(!m_mParent)
        return DoDestroy();
      return FALSE;
    }

    // Extra menu functions
    const XMenuEx *GetParent()const{
      return m_mParent;
    }

    HMENU GetParentMenu(){
      if(m_mParent)
        return m_mParent->m_hMenu;
      return NULL;
    }

    //events
    virtual BOOL OnDrawItem(HWND /*hWnd*/,LPDRAWITEMSTRUCT /*lpDrawItemStruct*/){return FALSE;}
    virtual BOOL OnMeasureItem(HWND /*hWnd*/,LPMEASUREITEMSTRUCT /*lpMeasureItemStruct*/){return FALSE;}
    virtual LRESULT OnMenuChar(HWND /*hWnd*/,UINT /*nChar*/,UINT /*nFlags*/){return MAKELONG(0, MNC_IGNORE);}

  protected:
    //for Subclassing!
    virtual XMenuEx* DoCreate(XMenuEx* parent);

    virtual XMenuEx* DoInsert(UINT uPos,UINT wID,LPCTSTR mtitle,UINT fState=MFS_ENABLED,UINT fType=MFT_STRING,
      HBITMAP hbmpItem=NULL,HBITMAP hbmpChecked=NULL,HBITMAP hbmpUnchecked=NULL);

    virtual BOOL DoConvert();
    virtual BOOL DoRevert();

    virtual BOOL DoDestroy();

    BOOL FindMenuItemPosition(XMenuEx *Item,PUINT pPos);

  protected:
    XMenuEx *m_mParent;
    std::list < XMenuEx* > m_mSubMenus;

  protected://internal initialization
    XMenuEx(XMenuEx *mParent):m_mParent(mParent){}
};

#endif // _XMENUEX_H_
