#include "xmenuex.h"
#include "xutils.h"
#include "xapp.h"

IMPLEMENT_DYNAMIC(XMenuEx,XMenu)

XMenuEx::~XMenuEx(){
  if(!m_mParent){
    if(m_bAttached)
      Detach();
    else
      DoDestroy();
  }
}

XMenuEx* XMenuEx::FromHandle(HMENU hMenu){
  MENUINFO mi;
  mi.cbSize=sizeof(MENUINFO);
  mi.fMask=MIM_MENUDATA;
  if(::GetMenuInfo(hMenu,&mi))
    return (XMenuEx*)mi.dwMenuData;
  return NULL;
}

XMenuEx* XMenuEx::FromItemHandle(HMENU hMenu,UINT iItem,BOOL bByPos){
  MENUITEMINFO mii;
  mii.cbSize=sizeof(MENUITEMINFO);
  mii.fMask=MIIM_DATA;
  if(::GetMenuItemInfo(hMenu,iItem,bByPos,&mii))
    return (XMenuEx*)(DWORD_PTR)mii.dwItemData;
  return NULL;
}

HMENU XMenuEx::Attach(HMENU hMenu){
  if(!m_bAttached&&XMenu::Attach(hMenu))
    DoConvert();
  return m_hMenu;
}

HMENU XMenuEx::Detach(){
  if(m_bAttached&&DoRevert())
    return XMenu::Detach();
  return NULL;
}

BOOL XMenuEx::DoConvert(){
  if(IsValid()){
    // walk through all items
    UINT nItems=GetMenuItemCount();
    for(UINT iItem=0;iItem<nItems;iItem++){
      //Get Item Data
      MENUITEMINFO mii;
      mii.cbSize=sizeof(mii);
      mii.fMask=MIIM_DATA|MIIM_SUBMENU;
      if(XMenu::GetMenuItemInfo(iItem,&mii,TRUE)&&!mii.dwItemData){
        //Creates a New Item...
        XMenuEx* newItem=DoCreate(this);
        XASSERT(newItem!=NULL)
        //Update
        mii.dwItemData=(DWORD_PTR)newItem;
        if(XMenu::SetMenuItemInfo(iItem,&mii,TRUE)){
          m_mSubMenus.push_back(newItem);
          //Has Sub Menus?
          if(mii.hSubMenu)
            newItem->Attach(mii.hSubMenu);
        }
      }
    }
    return TRUE;
  }
  return FALSE;
}

BOOL XMenuEx::DoRevert(){
  if(IsValid()){
    // walk through all items
    UINT nItems=GetMenuItemCount();
    for(UINT iItem=0;iItem<nItems;iItem++){
      //Get Item Data
      MENUITEMINFO mii;
      mii.cbSize=sizeof(mii);
      mii.fMask=MIIM_DATA|MIIM_SUBMENU;
      if(XMenu::GetMenuItemInfo(iItem,&mii,TRUE)&&mii.dwItemData){
        //Remove Item...
        XMenuEx* newItem=(XMenuEx*)(DWORD_PTR)mii.dwItemData;
        //Update
        mii.dwItemData=0;
        if(XMenu::SetMenuItemInfo(iItem,&mii,TRUE)){
          m_mSubMenus.remove(newItem);
          //Has Sub Menus?
          if(mii.hSubMenu)
            newItem->Detach();
          //finaly remove...
          delete newItem;
        }
      }
    }
    return TRUE;
  }
  return FALSE;
}

XMenuEx* XMenuEx::DoInsert(UINT uPos,UINT wID,LPCTSTR mtitle,UINT fState,UINT fType,HBITMAP hbmpItem,HBITMAP hbmpChecked,HBITMAP hbmpUnchecked){
  //create the menu (if not created)
  if(!IsValid()){
    if(!CreateMenu())
      return NULL;
    //Menu Info (extra data)
    MENUINFO mi;
    mi.cbSize=sizeof(MENUINFO);
    mi.fMask=MIM_MENUDATA;
    mi.dwMenuData=(DWORD_PTR)this;
    SetMenuInfo(&mi);
    //Update Parent
    if(m_mParent){
      MENUITEMINFO mii;
      mii.cbSize=sizeof(MENUITEMINFO);
      mii.fMask=MIIM_SUBMENU;
      mii.hSubMenu=m_hMenu;
      if(!SetMenuItemInfo(&mii))
        return NULL;
    }
  }
  //Creates a New Item...
  XMenuEx* newItem=DoCreate(this);
  XASSERT(newItem!=NULL)
  //get text from rsrc?
  LPCTSTR lpText=mtitle;
  LPTSTR szBuffer=NULL;
  if(IS_INTRESOURCE(mtitle)){
    UINT uLen=::GetStringLength(XGetInstanceHandle(),(UINT_PTR)mtitle);
    if(uLen&&(szBuffer=(LPTSTR)LocalAlloc(LMEM_ZEROINIT,++uLen*sizeof(TCHAR)))){
      if(::LoadString(XGetInstanceHandle(),(UINT_PTR)mtitle,szBuffer,uLen))
        lpText=szBuffer;
      else//Can't find Resource!
        lpText=_T("");
    }
  }
  //fill data...
  MENUITEMINFO mii;
  mii.cbSize=sizeof(MENUITEMINFO);
  mii.fMask=MIIM_DATA|MIIM_ID|MIIM_FTYPE|MIIM_STATE|MIIM_SUBMENU|MIIM_CHECKMARKS|MIIM_STRING|MIIM_BITMAP;
  mii.fType=fType;
  mii.fState=fState;
  mii.hbmpChecked=hbmpChecked;
  mii.hbmpUnchecked=hbmpUnchecked;
  mii.dwTypeData=(LPTSTR)lpText;
  mii.cch=lstrlen(lpText);
  mii.hSubMenu=NULL;
  mii.dwItemData=(DWORD_PTR)newItem;
  mii.hbmpItem=hbmpItem;
  mii.wID=wID;
  //append new element...
  if(!InsertMenuItem(uPos,&mii,TRUE)){
    delete newItem;
    newItem=NULL;
  }else
    m_mSubMenus.push_back(newItem);
  //free buffer...
  if(szBuffer)
    LocalFree(szBuffer);
  return newItem;
}

BOOL XMenuEx::FindMenuItemPosition(XMenuEx *Item,PUINT pPos){
  MENUITEMINFO mii;
  if(XMenu::IsValid()){
    UINT nItems=XMenu::GetMenuItemCount();
    mii.cbSize=sizeof(mii);
    mii.fMask=MIIM_DATA;
    for(UINT i=0;i<nItems;i++)
    if(XMenu::GetMenuItemInfo(i,&mii,TRUE)&&mii.dwItemData==(DWORD_PTR)Item){
      if(pPos)*pPos=i;
      return TRUE;
    }
  }
  return FALSE;
}

XMenuEx* XMenuEx::DoCreate(XMenuEx* parent){
  return new XMenuEx(parent);
}

BOOL XMenuEx::DoDestroy(){
  // clear sub items
  if(m_mSubMenus.size()){
    for(std::list< XMenuEx* >::iterator it=m_mSubMenus.begin();it!=m_mSubMenus.end();it++){
      (*it)->DoDestroy();
      delete (*it);
    }
    m_mSubMenus.clear();
  }
  //free handle
  if(IsValid())
    return XMenu::Destroy();
  //we can be just a MENU item so just return true
  return TRUE;
}
