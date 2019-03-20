#include "xskinnedmenu.h"

IMPLEMENT_DYNAMIC(XSkinnedMenu,XMenuEx)

XSkinnedMenu::XSkinnedMenu(){
  m_borderBrush.CreateSolid(XCOLOR_BACKGROUND_BORDER);
  m_hbmpItem=m_hbmpChecked=m_hbmpUnchecked=NULL;
}

XSkinnedMenu::~XSkinnedMenu(){
}

XMenuEx* XSkinnedMenu::DoCreate(XMenuEx* parent){
  return new XSkinnedMenu(parent);
}

XMenuEx* XSkinnedMenu::DoInsert(UINT uPos,UINT wID,LPCTSTR mtitle,UINT fState,UINT fType,
  HBITMAP hbmpItem,HBITMAP hbmpChecked,HBITMAP hbmpUnchecked){
  m_hbmpItem=hbmpItem;
  m_hbmpChecked=hbmpChecked;
  m_hbmpUnchecked=hbmpUnchecked;
  if(hbmpItem!=HBMMENU_SYSTEM)//create the menu
    return XMenuEx::DoInsert(uPos,wID,mtitle,fState,fType|MFT_OWNERDRAW);
  return XMenuEx::DoInsert(uPos,wID,mtitle,fState,fType,hbmpItem,hbmpChecked,hbmpUnchecked);
}

BOOL XSkinnedMenu::DoConvert(){
  if(IsValid()){
    //Walk through all items
    UINT nItems=GetMenuItemCount();
    for(UINT iItem=0;iItem<nItems;iItem++){
      //Get Item Data
      MENUITEMINFO mii;
      mii.cbSize=sizeof(mii);
      mii.fMask=MIIM_DATA|MIIM_FTYPE|MIIM_BITMAP|MIIM_CHECKMARKS|MIIM_SUBMENU;
      if(XMenu::GetMenuItemInfo(iItem,&mii,TRUE)&&!mii.dwItemData){
        //Creates a New Item...
        XSkinnedMenu* newItem=(XSkinnedMenu*)DoCreate(this);
        XASSERT(newItem!=NULL)
        //Update...
        mii.dwItemData=(DWORD_PTR)newItem;
        mii.fType|=MFT_OWNERDRAW;
        //Bitmaps menus are not sized!
        if(mii.hbmpItem){
          newItem->m_hbmpItem=mii.hbmpItem;
          newItem->m_hbmpChecked=mii.hbmpChecked;
          newItem->m_hbmpUnchecked=mii.hbmpUnchecked;
          mii.hbmpChecked=mii.hbmpUnchecked=NULL;
          mii.hbmpItem=HBMMENU_CALLBACK;
        }
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

BOOL XSkinnedMenu::DoRevert(){
  if(IsValid()){
    // walk through all items
    UINT nItems=GetMenuItemCount();
    for(UINT iItem=0;iItem<nItems;iItem++){
      //Get Item Data
      MENUITEMINFO mii;
      mii.cbSize=sizeof(mii);
      mii.fMask=MIIM_DATA|MIIM_FTYPE|MIIM_BITMAP|MIIM_CHECKMARKS|MIIM_SUBMENU;
      if(XMenu::GetMenuItemInfo(iItem,&mii,TRUE)&&mii.dwItemData){
        //Remove Item...
        XSkinnedMenu* newItem=(XSkinnedMenu*)(DWORD_PTR)mii.dwItemData;
        //Update
        mii.dwItemData=0;
        mii.fType&=~MFT_OWNERDRAW;
        //Restore Bitmaps
        if(newItem->m_hbmpItem){
          mii.hbmpItem=newItem->m_hbmpItem;
          mii.hbmpChecked=newItem->m_hbmpChecked;
          mii.hbmpUnchecked=newItem->m_hbmpUnchecked;
          newItem->m_hbmpItem=newItem->m_hbmpChecked=newItem->m_hbmpUnchecked=NULL;
        }
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

/*HMENU FindMenuFromIDData(HMENU hMenu,UINT nID,ULONG_PTR pData){
  if(!hMenu||!::IsMenu(hMenu))
    return NULL;
  XASSERT(::IsMenu(hMenu))
  // walk through all items, looking for ID match
  UINT nItems=::GetMenuItemCount(hMenu);
  for(UINT iItem=0;iItem<nItems;iItem++){
    HMENU hPopup=::GetSubMenu(hMenu,iItem);
    if(hPopup!=NULL){
      // recurse to child popup
      hPopup=FindMenuFromIDData(hPopup,nID,pData);
      // check popups on this popup
      if(hPopup!=NULL)
        return hPopup;
    }else if(::GetMenuItemID(hMenu,iItem)==nID){
      MENUITEMINFO MenuItemInfo;
      MenuItemInfo.cbSize=sizeof(MenuItemInfo);
      MenuItemInfo.fMask=MIIM_DATA;
      if(::GetMenuItemInfo(hMenu,iItem,TRUE,&MenuItemInfo)){
        if(MenuItemInfo.dwItemData==pData)
          return hMenu;
      }
    }
  }
  // not found
  return NULL;
}*/

BOOL XSkinnedMenu::OnDrawItem(HWND hWnd,LPDRAWITEMSTRUCT lpdis){
  //By Microsoft...
  if((lpdis->CtlType!=ODT_MENU)||(lpdis->itemAction==ODA_FOCUS)||(!lpdis->hDC))
    return FALSE;
  if(!::IsMenu((HMENU)lpdis->hwndItem))
    return FALSE;
  XSkinnedMenu *menuitem=(XSkinnedMenu *)(DWORD_PTR)lpdis->itemData;
  if(!menuitem)
		return FALSE;

  BOOL bIsMenuBar=(GetMenu(hWnd)==(HMENU)lpdis->hwndItem);
  BOOL bChecked=lpdis->itemState&ODS_CHECKED;
  BOOL bDisabled=lpdis->itemState&ODS_GRAYED;
	BOOL bSelected=lpdis->itemState&ODS_SELECTED||lpdis->itemState&ODS_HOTLIGHT;

  MENUITEMINFO mii;
  TCHAR itemText[512];
  //GetMenuItemData...
	mii.cbSize=sizeof(mii);
	mii.dwTypeData=itemText;
	mii.cch=sizeof(itemText);
	mii.fMask=MIIM_ID|MIIM_FTYPE|MIIM_STATE|MIIM_SUBMENU|MIIM_CHECKMARKS|MIIM_STRING|MIIM_BITMAP;
	menuitem->GetMenuItemInfo(&mii);

  XDC WDC(lpdis->hDC);
  XRECT mr=lpdis->rcItem;
  XRECT br=lpdis->rcItem;
  XRECT tr=lpdis->rcItem;
  //Calc Border...
  br.right=br.left+GetSystemMetrics(SM_CXMENUSIZE)+2*GetSystemMetrics(SM_CXEDGE);
  tr.left=br.right;

  //Draw Backgraund...
  if(bSelected&&!bDisabled)
    WDC.DrawSolidRectangle(&mr,XCOLOR_BACKGROUND_BORDER,XCOLOR_BACKGROUND_ACTIVE);
  else if(bIsMenuBar)
    WDC.DrawGradient(&mr,XCOLOR_BACKGROUND_ACTIVE,XCOLOR_BACKGROUND,1);
  else{
    WDC.DrawGradient(&br,XCOLOR_BACKGROUND,XCOLOR_BACKGROUND_INACTIVE,0);
    WDC.FillSolidRect(&tr,XCOLOR_BACKGROUND);
  }

  // Text color
  COLORREF txc=XCOLOR_TEXT_INACTIVE;
  if(bDisabled)
    txc=XCOLOR_TEXT_DISABLED;
  else if(bSelected)
    txc=XCOLOR_TEXT_ACTIVE;
  LONG clrOldText=WDC.SetTextColor(txc);
  INT iOldBk=WDC.SetBkMode(TRANSPARENT);

  if(bIsMenuBar){
    // Image...
    if(mii.hbmpItem!=NULL){
      XDC hdcMem;HBITMAP hbmSave;
      hdcMem.CreateCompatible(WDC);
      if((hbmSave=(HBITMAP)hdcMem.SelectObject(mii.hbmpItem))){
        WDC.BitBlt(br.left,br.top,GetSystemMetrics(SM_CXMENUSIZE),GetSystemMetrics(SM_CYMENUSIZE),hdcMem,0,0,SRCCOPY);
        hdcMem.SelectObject(hbmSave);
        hdcMem.Delete();
      }
    }
    // Text...
    if(mii.dwTypeData!=NULL){
      UINT dt_Hide=(lpdis->itemState&ODS_NOACCEL)?DT_HIDEPREFIX:0;
      WDC.DrawTextEx(mii.dwTypeData,mii.cch,&mr,dt_Hide|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_CENTER,NULL);
    }
  }else{
    // Text Margin
    tr.left+=2*GetSystemMetrics(SM_CXEDGE);
    tr.right-=2*GetSystemMetrics(SM_CXEDGE);
    //Separator...
    if((mii.fType&MFT_SEPARATOR)){
      tr.top+=(tr.bottom-tr.top)>>1;
      WDC.DrawSolidLine(tr.left,tr.top,tr.right,tr.top,XCOLOR_BACKGROUND_ACTIVE);
    }else///MenuBreak hasn't size...
    if(!(mii.fType&MFT_MENUBREAK)){
      /*if(mii.hbmpItem==HBMMENU_SYSTEM){//not implemented for now...
        HWND hWnd=(HWND)mii.dwItemData;
        if(hWnd){
          HICON hi=(HICON)::GetClassLong(hWnd,GCL_HICONSM);
          if(!hi)
            hi=(HICON)::GetClassLong(hWnd,GCL_HICON);
          if(!hi)// Use Default Icon...
            hi=::LoadIcon(NULL,IDI_APPLICATION);
          if(hi){
            WDC.DrawIconEx(GetSystemMetrics(SM_CXEDGE)*2,GetSystemMetrics(SM_CYBORDER),hi,
              mr.right-GetSystemMetrics(SM_CXEDGE)*2,mr.bottom-GetSystemMetrics(SM_CYBORDER),0,0,DI_NORMAL);
          }
        }
      */
      // Icon
      HBITMAP checkbmp=NULL;
      if(menuitem->m_hbmpItem!=NULL){
        switch((ULONG_PTR)menuitem->m_hbmpItem){
          case 8://HBMMENU_POPUP_CLOSE
            WDC.DrawSpecialChar(br.left+4,br.top+3,'r');
          break;
          case 9://HBMMENU_POPUP_RESTORE
            WDC.DrawSpecialChar(br.left+4,br.top+3,'2');
          break;
          case 10://HBMMENU_POPUP_MAXIMIZE
            WDC.DrawSpecialChar(br.left+4,br.top+3,'1');
          break;
          case 11://HBMMENU_POPUP_MINIMIZE
            WDC.DrawSpecialChar(br.left+4,br.top+3,'0');
          break;
          case 5://HBMMENU_MBAR_CLOSE
            checkbmp=(HBITMAP)LoadImage(NULL,MAKEINTRESOURCE(OBM_CLOSE),IMAGE_BITMAP,0,0,LR_LOADMAP3DCOLORS|LR_SHARED);
          break;
          /*case 1://HBMMENU_SYSTEM
          case 2://HBMMENU_MBAR_RESTORE
          case 3://HBMMENU_MBAR_MINIMIZE
          case 4://(??)
          case 5://HBMMENU_MBAR_CLOSE
          case 6://HBMMENU_MBAR_CLOSE_D
          case 7://HBMMENU_MBAR_MINIMIZE_D*/
          default:
            checkbmp=menuitem->m_hbmpItem;
          break;
        }
      }else if(bChecked){
        if(menuitem->m_hbmpChecked&&menuitem->m_hbmpUnchecked)
          checkbmp=bChecked?menuitem->m_hbmpChecked:menuitem->m_hbmpUnchecked;
        if(!checkbmp){
          if((mii.fType&MFT_RADIOCHECK))
            WDC.DrawSpecialChar(br.left+4,br.top+3,'h');
          else
            WDC.DrawSpecialChar(br.left+4,br.top+3,'a');
        }
      }
      if(checkbmp){
        XDC hdcMem;HBITMAP hbmSave;
        hdcMem.CreateCompatible(WDC);
        if((hbmSave=(HBITMAP)hdcMem.SelectObject(checkbmp))){
          BITMAP bm;
          GetObject(checkbmp,sizeof(bm),&bm);
          INT iOldBkClr=WDC.SetBkColor(bSelected?XCOLOR_BACKGROUND_ACTIVE:XCOLOR_BACKGROUND_INACTIVE);
          //if(checkbmp==mii.hbmpItem){
            //WDC.DrawRectangle(&ibgRect,0,clrCheckDark);
          //}
          WDC.BitBlt(br.left+((br.right-br.left)-bm.bmWidth)/2,br.top+((br.bottom-br.top)-bm.bmHeight)/2,
            bm.bmWidth,bm.bmHeight,hdcMem,0,0,SRCCOPY);
          WDC.SetBkColor(iOldBkClr);
          hdcMem.SelectObject(hbmSave);
          hdcMem.Delete();
        }
      }
      // Text...
      if(mii.dwTypeData!=NULL&&mii.cch!=0){
        LPTSTR name=_tcstok(mii.dwTypeData,_T("\t"));
        LPTSTR tab=_tcstok(NULL,_T("\0"));
        UINT dt_Hide=(lpdis->itemState&ODS_NOACCEL)?DT_HIDEPREFIX:0;
        WDC.DrawTextEx(name,-1,&tr,dt_Hide|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_LEFT,NULL);
        if(tab)WDC.DrawTextEx(tab,-1,&tr,DT_HIDEPREFIX|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER|DT_RIGHT,NULL);
      }
    }
  }
  //WDC.FrameRect(&mr,m_borderBrush);
  WDC.SetTextColor(clrOldText);
  WDC.SetBkMode(iOldBk);

  return TRUE;
}

BOOL XSkinnedMenu::OnMeasureItem(HWND hWnd,LPMEASUREITEMSTRUCT lpmis){
  if(lpmis->CtlType!=ODT_MENU)
		return FALSE;
  XSkinnedMenu *menuitem=(XSkinnedMenu *)(DWORD_PTR)lpmis->itemData;
  if(!menuitem)
		return FALSE;
  BOOL bIsMenuBar=(menuitem->GetParentMenu()==GetMenu(hWnd));
  MENUITEMINFO mii;
  TCHAR itemText[512];
	//GetMenuItemData...
	mii.cbSize=sizeof(mii);
	mii.dwTypeData=itemText;
	mii.cch=sizeof(itemText);
	mii.fMask=MIIM_FTYPE|MIIM_SUBMENU|MIIM_STRING|MIIM_BITMAP;
	menuitem->GetMenuItemInfo(&mii);
	//initialize...
	lpmis->itemWidth=lpmis->itemHeight=0;
  //Separator...
  if((mii.fType&MFT_SEPARATOR)){
	  //lpmis->itemWidth=0;//Already is 0
    lpmis->itemHeight+=GetSystemMetrics(SM_CYMENUSIZE)>>1;//x/2
  }else///MenuBreak hasn't size...
  if(!(mii.fType&MFT_MENUBREAK)){
    //Get Text Size
    XClientDC CDC;TEXTMETRIC tm;
    NONCLIENTMETRICS nm;
    nm.cbSize=sizeof(nm);
    SystemParametersInfo(SPI_GETNONCLIENTMETRICS,nm.cbSize,&nm,0);
    // Make the font bold
    if(menuitem->IsDefaultItem(0))
    nm.lfMenuFont.lfWeight=FW_BOLD;
    HFONT hMenuFont=CreateFontIndirect(&nm.lfMenuFont);
    HFONT hOldFont=(HFONT)CDC.SelectObject(hMenuFont);
    // Text...
    if(mii.dwTypeData!=NULL){
      SIZE size=CDC.GetTextExtent(mii.dwTypeData,mii.cch);
      lpmis->itemWidth+=size.cx+2*GetSystemMetrics(SM_CXEDGE);
      lpmis->itemHeight=__max(GetSystemMetrics(SM_CYMENU),size.cy+2*GetSystemMetrics(SM_CYEDGE));
    }
    // Add Vertical Margin
    if(!bIsMenuBar){
      CDC.GetTextMetrics(&tm);
      // Fixed border size (for images)
      lpmis->itemWidth+=GetSystemMetrics(SM_CXMENUSIZE)+2*GetSystemMetrics(SM_CXEDGE)+2*tm.tmAveCharWidth;
      lpmis->itemHeight=__max(lpmis->itemHeight,(UINT)GetSystemMetrics(SM_CYMENUSIZE));
      // Has SubMenu?... add space for "mark"
      if(mii.hSubMenu!=NULL){
        lpmis->itemWidth+=2*GetSystemMetrics(SM_CXEDGE)+2;
        lpmis->itemHeight+=2;
      }
    }else if(mii.hbmpItem!=NULL){
      // Menu bar image
      lpmis->itemWidth+=GetSystemMetrics(SM_CXMENUSIZE)+2*GetSystemMetrics(SM_CXEDGE);
      lpmis->itemHeight=__max(lpmis->itemHeight,(UINT)GetSystemMetrics(SM_CYMENUSIZE));
    }
    CDC.SelectObject(hOldFont);
    DeleteObject(hMenuFont);
  }
  //lpmis->itemWidth+=50;
  return TRUE;
}

LRESULT XSkinnedMenu::OnMenuChar(HWND /*hWnd*/,UINT nChar,UINT /*nFlags*/){
  INT nIndex=-1,c;TCHAR itemText[512];
  BOOL fMoreThanOne=FALSE;
  c=GetMenuItemCount();
  for(int i=0;i<c;i++){
    MENUITEMINFO mii;
    mii.cbSize=sizeof(MENUITEMINFO);
    mii.fMask=MIIM_DATA|MIIM_STATE|MIIM_STRING;
    mii.dwTypeData=itemText;
    mii.cch=sizeof(itemText);
    if(XMenu::GetMenuItemInfo(i,&mii,TRUE)){
      if(mii.dwTypeData!=NULL){
        LPCTSTR pchAS=_tcschr(mii.dwTypeData,'&');
        if(pchAS&&AnsiUpperChar(*pchAS)==AnsiUpperChar(nChar)){
          if(nIndex!=-1){
            fMoreThanOne=TRUE;
            break;
          }else
            nIndex = i;
        }
      }
    }
  }
  if(nIndex != -1)
    return MAKELONG(nIndex,fMoreThanOne?MNC_SELECT:MNC_EXECUTE);
  return MAKELONG(0,MNC_IGNORE);
}
