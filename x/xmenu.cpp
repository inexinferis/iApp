#include "xapp.h"
#include "xmenu.h"

IMPLEMENT_DYNAMIC(XMenu,XObject)

XMenu::~XMenu(){
  if(m_hMenu!=NULL&&m_bAttached)
    Destroy();
}

HMENU XMenu::Attach(HMENU hMenu){
  XASSERT(m_hMenu==NULL)
  if(!hMenu)return NULL;
  m_bAttached=XWinApp::m_XMenuMap.Add(hMenu,this);
  m_hMenu=hMenu;
  return hMenu;
}

HMENU XMenu::Detach(){
  HMENU hMenu=m_hMenu;
  if(m_hMenu!=NULL){
    if(m_bAttached){
      XWinApp::m_XMenuMap.Remove(m_hMenu);
      m_bAttached=FALSE;
    }
    m_hMenu=NULL;
  }
  return hMenu;
}

XMenu* XMenu::FromHandle(HMENU hMenu){
  XASSERT(hMenu!=NULL)
  XMenu *pMenu=NULL;
  if(hMenu){
    pMenu=FromHandlePermanent(hMenu);
    if(!pMenu){
      pMenu=new XMenu;
      if(pMenu)pMenu->Attach(hMenu);
    }
  }
  return pMenu;
}

XMenu *XMenu::FromHandlePermanent(HMENU hMenu){
  if(hMenu)
    return XWinApp::m_XMenuMap.Find(hMenu);
  return NULL;
}

XMenu *XMenu::FindSubMenuFromID(HMENU hMenu,UINT wID,DWORD dwItemData){
  XASSERT(hMenu!=NULL)
	int nItems=::GetMenuItemCount(hMenu);
	for(int iItem=0;iItem<nItems;iItem++){
    MENUITEMINFO mii;
    mii.cbSize=sizeof(MENUITEMINFO);
    mii.fMask=MIIM_ID|MIIM_DATA|MIIM_SUBMENU;
    if(::GetMenuItemInfo(hMenu,iItem,TRUE,&mii)){
      if(mii.wID==wID&&(!dwItemData||mii.dwItemData==dwItemData))
        return FromHandlePermanent(hMenu);
      if(mii.hSubMenu){
        XMenu *pMenu=FindSubMenuFromID(mii.hSubMenu,wID,dwItemData);
        if(pMenu)return pMenu;
      }
    }
	}
	return NULL;
}

XMenu *XMenu::FindPopupMenuFromID(UINT nID,DWORD dwItemData){
  std::map < HMENU,XMenu* >::const_iterator it;
  for(it=XWinApp::m_XMenuMap.Begin();it!=XWinApp::m_XMenuMap.End();it++){
    HMENU hMenu=(*it).first;
    XASSERT(hMenu!=NULL)
    if(hMenu&&::IsMenu(hMenu)){
      XMenu *pMenu=FindSubMenuFromID(hMenu,nID,dwItemData);
      if(pMenu)return pMenu;
    }
  }
	return NULL;
}

BOOL XMenu::Destroy(){
  if(m_hMenu)
    return ::DestroyMenu(Detach());
  return FALSE;
}
