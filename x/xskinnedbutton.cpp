#include "xskinnedbutton.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XSkinnedButton,XButton)
  EVENT_MOUSELEAVE(XSkinnedButton::OnMouseLeave)
  EVENT_MOUSEMOVE(XSkinnedButton::OnMouseMove)
  EVENT_ERASEBKGND(XSkinnedButton::OnEraseBkgnd)
  EVENT_NCHITTEST(XSkinnedButton::OnNCHitTest)
  EVENT_BN_CLICKED(XTHISCTRLID,XSkinnedButton::OnClick)
  EVENT_BM_SETCHECK(XSkinnedButton::OnSetCheck)
	EVENT_BM_GETCHECK(XSkinnedButton::OnGetCheck)
	//just as a child!
	EVENT_DRAWITEM_EX(XSkinnedButton::OnDrawItem,FALSE)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XSkinnedButton,XButton)

XSkinnedButton::XSkinnedButton(HWND hwnd):XButton(hwnd){
  m_hFont.Create(XFONT_SIZE,0,0,0,FALSE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH|FF_DONTCARE,XFONT_FACE);
  m_dwState=m_dwStyle=0;
  m_bMouseOver=FALSE;
}

XSkinnedButton::~XSkinnedButton(){
  m_hFont.Delete();
}

VOID XSkinnedButton::OnMouseLeave(){
  if(m_bMouseOver){
    m_bMouseOver=FALSE;
    Invalidate();
  }
}

VOID XSkinnedButton::OnMouseMove(DWORD /*dwVKState*/,POINTS /*pt*/){
  if(!(m_dwStyle&BS_GROUPBOX)){
    TrackMouseEvent(TME_LEAVE,10);
    if(!m_bMouseOver){
      m_bMouseOver=TRUE;
      Invalidate();
    }
  }
}

BOOL XSkinnedButton::OnDrawItem(UINT /*uCtrlID*/,PDRAWITEMSTRUCT pDIS){
  XDC WDC(pDIS->hDC);
  XRect wr(pDIS->rcItem);
  UINT state=pDIS->itemState;
  UINT action=pDIS->itemAction;
  DWORD dwXCorrection=0;
  UINT len=GetWindowTextLength();
  if((MASKENUM(m_dwStyle,BS_AUTOCHECKBOX)||MASKENUM(m_dwStyle,BS_CHECKBOX)||
    MASKENUM(m_dwStyle,BS_AUTO3STATE)||MASKENUM(m_dwStyle,BS_3STATE))&&!(m_dwStyle&BS_PUSHLIKE)){
    WDC.FillSolidRect(wr,XCOLOR_BACKGROUND);
    DWORD x=0,y=3;
    if(m_dwStyle&BS_LEFTTEXT)
      x=wr.right-14;
    //Draw Background
    if(state&ODS_SELECTED){
      WDC.DrawSolidRectangle(x,y,x+14,y+14,XCOLOR_SELECTED,XCOLOR_BACKGROUND);
    }else if(state&ODS_DISABLED){
      WDC.DrawSolidRectangle(x,y,x+14,y+14,XCOLOR_BACKGROUND_ACTIVE,XCOLOR_BACKGROUND);
    }else if(m_bMouseOver||state&ODS_FOCUS){
      WDC.DrawSolidRectangle(x,y,x+14,y+14,XCOLOR_HOVER,XCOLOR_BACKGROUND);
    }else{//NORMAL
      WDC.DrawSolidRectangle(x,y,x+14,y+14,XCOLOR_BACKGROUND_ACTIVE,XCOLOR_BACKGROUND);
    }
    //do the mark
    if(m_dwState==BST_CHECKED){
      WDC.DrawSolidLine(x+2,y+5,x+5,y+8,XCOLOR_SELECTED);
      WDC.DrawSolidLine(x+2,y+6,x+5,y+9,XCOLOR_SELECTED);
      WDC.DrawSolidLine(x+2,y+7,x+5,y+10,XCOLOR_SELECTED);
      WDC.DrawSolidLine(x+2,y+8,x+5,y+11,XCOLOR_HOVER);
      WDC.DrawSolidLine(x+5,y+8,x+12,y+1,XCOLOR_SELECTED);
      WDC.DrawSolidLine(x+5,y+9,x+12,y+2,XCOLOR_SELECTED);
      WDC.DrawSolidLine(x+5,y+10,x+12,y+3,XCOLOR_SELECTED);
      WDC.DrawSolidLine(x+5,y+11,x+12,y+4,XCOLOR_HOVER);
    }else if(m_dwState==BST_INDETERMINATE){
      WDC.DrawSolidRectangle(x+2,y+2,x+12,y+12,XCOLOR_BACKGROUND_INACTIVE,XCOLOR_HOVER);
    }
    dwXCorrection=22;
  }else if((MASKENUM(m_dwStyle,BS_AUTORADIOBUTTON)||MASKENUM(m_dwStyle,BS_RADIOBUTTON))&&!(m_dwStyle&BS_PUSHLIKE)){
    WDC.FillSolidRect(wr,XCOLOR_BACKGROUND);
    DWORD x=0,y=3;
    if(m_dwStyle&BS_LEFTTEXT)
      x=wr.right-14;
    //Draw Background
    if(state&ODS_SELECTED){
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_BACKGROUND_ACTIVE,2);
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_SELECTED);
    }else if(state&ODS_DISABLED){
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_BACKGROUND_INACTIVE,2);
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_BACKGROUND_ACTIVE);
    }else if(m_bMouseOver||state&ODS_FOCUS){
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_BACKGROUND_ACTIVE,2);
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_HOVER);
    }else{//NORMAL
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_BACKGROUND_INACTIVE,2);
      WDC.FrameSolidEllipse(x,y,x+14,y+14,XCOLOR_NORMAL);
    }
    //do the mark
    if(m_dwState==BST_CHECKED){
      WDC.DrawSolidEllipse(x+3,y+3,x+11,y+11,XCOLOR_NORMAL,XCOLOR_NORMAL);
      WDC.DrawSolidEllipse(x+4,y+4,x+10,y+10,XCOLOR_SELECTED,XCOLOR_SELECTED);
    }
    dwXCorrection=22;
  }else if(MASKENUM(m_dwStyle,BS_GROUPBOX)){
    if(action==ODA_DRAWENTIRE)//XCOLOR_BACKGROUND_BORDER
      WDC.FrameSolidRect(wr.left,wr.top+(len?7:0),wr.right,wr.bottom,XCOLOR_NORMAL,1);
    dwXCorrection=(len?13:0);
  }else{
    //Draw Background
    if(state&ODS_SELECTED){
      WDC.DrawSolidRectangle(wr.left,wr.top,wr.right,wr.bottom,XCOLOR_SELECTED,XCOLOR_BACKGROUND_INACTIVE);
    }else if(state&ODS_DISABLED){
      WDC.DrawSolidRectangle(wr.left,wr.top,wr.right,wr.bottom,XCOLOR_BACKGROUND_ACTIVE,XCOLOR_BACKGROUND);
    }else if(m_bMouseOver||state&ODS_FOCUS){
      if(m_dwState==BST_CHECKED)
        WDC.DrawSolidRectangle(wr.left,wr.top,wr.right,wr.bottom,XCOLOR_HOVER,XCOLOR_BACKGROUND_INACTIVE);
      else
        WDC.DrawSolidRectangle(wr.left,wr.top,wr.right,wr.bottom,XCOLOR_HOVER,XCOLOR_BACKGROUND_ACTIVE);
    }else{//NORMAL
      //do the mark
      if(m_dwState==BST_CHECKED)
        WDC.DrawSolidRectangle(wr.left,wr.top,wr.right,wr.bottom,XCOLOR_NORMAL,XCOLOR_BACKGROUND_INACTIVE);
      else
        WDC.DrawSolidRectangle(wr.left,wr.top,wr.right,wr.bottom,XCOLOR_BACKGROUND_ACTIVE,XCOLOR_BACKGROUND_INACTIVE);
    }
  }

  //Draw Text
  if(len++){
    PTCHAR lpszText=(PTCHAR)malloc(len*sizeof(TCHAR));

    if(GetWindowText(lpszText,len)){
      HFONT hOldFont=(HFONT)WDC.SelectObject((HFONT)m_hFont);
      DWORD dwTextFlags=DT_WORD_ELLIPSIS|DT_NOCLIP;

      if((MASKENUM(m_dwStyle,BS_AUTOCHECKBOX)||MASKENUM(m_dwStyle,BS_CHECKBOX)||
        MASKENUM(m_dwStyle,BS_AUTO3STATE)||MASKENUM(m_dwStyle,BS_3STATE)||
        MASKENUM(m_dwStyle,BS_AUTORADIOBUTTON)||MASKENUM(m_dwStyle,BS_RADIOBUTTON))&&!(m_dwStyle&BS_PUSHLIKE)){

        dwTextFlags|=DT_VCENTER;
        if(m_dwStyle&BS_LEFTTEXT){
          dwTextFlags|=DT_RIGHT;
          wr.right-=dwXCorrection;
        }else{
          dwTextFlags|=DT_LEFT;
          wr.left+=dwXCorrection;
        }

      }else if(MASKENUM(m_dwStyle,BS_GROUPBOX)){
        XRect tr;

        dwTextFlags|=DT_TOP;
        if(m_dwStyle&BS_LEFTTEXT){
          dwTextFlags|=DT_RIGHT;
          wr.right-=dwXCorrection;
        }else{
          dwTextFlags|=DT_LEFT;
          wr.left+=dwXCorrection;
        }
        // Draw text background
        WDC.DrawColoredText(lpszText,&tr,TRANSPARENT,0,XCOLOR_TEXT_ACTIVE,DT_CALCRECT);
        WDC.FillSolidRect(tr.left+6,tr.top,tr.right+12,tr.bottom,XCOLOR_BACKGROUND);
      }else
        dwTextFlags|=DT_CENTER|DT_VCENTER;

      if(m_dwStyle&BS_LEFT)dwTextFlags|=DT_LEFT;
      if(m_dwStyle&BS_CENTER)dwTextFlags|=DT_CENTER;
      if(m_dwStyle&BS_RIGHT)dwTextFlags|=DT_RIGHT;

      if(m_dwStyle&BS_TOP)dwTextFlags|=DT_TOP;
      if(m_dwStyle&BS_VCENTER)dwTextFlags|=DT_VCENTER;
      if(m_dwStyle&BS_BOTTOM)dwTextFlags|=DT_BOTTOM;

      if(!(m_dwStyle&BS_MULTILINE))dwTextFlags|=DT_SINGLELINE;

      WDC.DrawColoredText(lpszText,&wr,TRANSPARENT,0,XCOLOR_TEXT_ACTIVE,dwTextFlags);
      WDC.SelectObject(hOldFont);
    }
    free(lpszText);
  }
  return TRUE;
}

VOID XSkinnedButton::OnClick(WPARAM /*wParam*/,LPARAM /*lParam*/){
  if(MASKENUM(m_dwStyle,BS_AUTOCHECKBOX)||MASKENUM(m_dwStyle,BS_AUTORADIOBUTTON)){
    m_dwState=((m_dwState==BST_UNCHECKED)?BST_CHECKED:BST_UNCHECKED);
    Invalidate();
  }
  if(MASKENUM(m_dwStyle,BS_AUTO3STATE)){
    m_dwState=((m_dwState==BST_UNCHECKED)?BST_CHECKED:((m_dwState==BST_CHECKED)?BST_INDETERMINATE:BST_UNCHECKED));
    Invalidate();
  }
}

INT XSkinnedButton::OnSetCheck(DWORD dwState){
  if(MASKENUM(m_dwStyle,BS_AUTOCHECKBOX)||MASKENUM(m_dwStyle,BS_CHECKBOX)||
    MASKENUM(m_dwStyle,BS_AUTORADIOBUTTON)||MASKENUM(m_dwStyle,BS_RADIOBUTTON)){
    if((dwState==BST_CHECKED||dwState==BST_UNCHECKED)&&m_dwState!=dwState){
      m_dwState=dwState;
      Invalidate();
    }
  }else if(MASKENUM(m_dwStyle,BS_AUTO3STATE)||MASKENUM(m_dwStyle,BS_3STATE)){
    if((dwState==BST_CHECKED||dwState==BST_INDETERMINATE||dwState==BST_UNCHECKED)&&m_dwState!=dwState){
      m_dwState=dwState;
      Invalidate();
    }
  }
  return 0;
}

INT XSkinnedButton::OnGetCheck(){
  if(MASKENUM(m_dwStyle,BS_AUTOCHECKBOX)||MASKENUM(m_dwStyle,BS_CHECKBOX)||
    MASKENUM(m_dwStyle,BS_AUTORADIOBUTTON)||MASKENUM(m_dwStyle,BS_RADIOBUTTON)||
    MASKENUM(m_dwStyle,BS_AUTO3STATE)||MASKENUM(m_dwStyle,BS_3STATE)){
    return m_dwState;
  }
  return 0;
}

INT XSkinnedButton::OnEraseBkgnd(HDC /*hDC*/){
  if(MASKENUM(m_dwStyle,BS_GROUPBOX))
    return 0;
  return Default();
}

LRESULT XSkinnedButton::OnNCHitTest(POINTS /*pt*/){
  if(MASKENUM(m_dwStyle,BS_GROUPBOX))
    return HTTRANSPARENT;
  return Default();
}
