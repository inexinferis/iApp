#include "xgdi.h"

/**************************************************************/
// XCOLOR REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XColorRef,XObject)

INT XColorRef::Round(double val){
	INT ret_val=(int)val;
	INT temp=(int)(val*100);
	if((temp%100)>=50)
		ret_val+=1;
	return ret_val;
}

HSL XColorRef::RGBtoHSL(){
  RGB c;HSL hsl;INT Max,Min,Diff;
  c.R=GetRed();
  c.G=GetGreen();
  c.B=GetBlue();
	if(c.R>c.G){
	  Max=c.R;
	  Min=c.G;
  }else{
    Max=c.G;
    Min=c.R;
  }
	if(c.B>Max)
    Max=c.B;
	else if(c.B<Min)
    Min=c.B;
	Diff=Max-Min;
	hsl.L=(double)Max/255;
	if(Max==0)
    hsl.S=0;
	else
    hsl.S=(double)Diff/Max;
	double q;
	if(Diff==0)
    q=0;
	else
    q=(double)60/Diff;
	if(Max==c.R){
		if(c.G<c.B)
      hsl.H=(double)(360+q*(c.G-c.B));
		else
      hsl.H=(double)(q*(c.G-c.B));
	}else if(Max==c.G)
    hsl.H=(double)(120+q*(c.B-c.R));
	else if(Max==c.B)
    hsl.H=(double)(240+q*(c.R-c.G));
	else
    hsl.H = 0.0;
	return hsl;
}

COLORREF XColorRef::HSLtoRGB(HSL hsl){
  INT Max,Mid,Min;double q;
  hsl.H/=360;//Normalized...
	Max=Round(hsl.L * 255);
	Min=Round((1.0-hsl.S)*(hsl.L/1.0)*255);
	q=(double)(Max-Min)/255;
	if(hsl.H>=0&&hsl.H<=(double)1/6){
		Mid=Round(((hsl.H)*q)*1530+Min);
		return g_color=RGB(Min,Mid,Max);
	}else if(hsl.H<=(double)1/3){
		Mid=Round(-((hsl.H-(double)1/6)*q)*1530+Max);
		return g_color=RGB(Min,Mid,Max);
	}else if(hsl.H<=0.5){
		Mid=Round(((hsl.H-(double)1/3)*q)*1530+Min);
		return g_color=RGB(Min,Mid,Max);
	}else if(hsl.H<=(double)2/3){
		Mid=Round(-((hsl.H-0.5)*q)*1530+Max);
		return g_color=RGB(Min,Mid,Max);
	}else if(hsl.H<=(double)5/6){
		Mid=Round(((hsl.H-(double)2/3)*q)*1530+Min);
		return g_color=RGB(Min,Mid,Max);
	}else if(hsl.H<=1.0){
		Mid=Round(-((hsl.H-(double)5/6)*q)*1530+Max);
		return g_color=RGB(Min,Mid,Max);
	}else
    return RGB(0,0,0);
}

COLORREF XColorRef::MixColors(COLORREF color){
  DWORD red   = MulDiv(86,GetRed(),100) + MulDiv(14,GetRValue(color),100);
  DWORD green = MulDiv(86,GetGreen(),100) + MulDiv(14,GetGValue(color),100);
  DWORD blue  = MulDiv(86,GetBlue(),100) + MulDiv(14,GetBValue(color),100);
  return RGB(red,green,blue);
}

COLORREF XColorRef::MidColor(COLORREF color){
  DWORD red   = MulDiv(7,GetRed(),10) + MulDiv(3,GetRValue(color),10);
  DWORD green = MulDiv(7,GetGreen(),10) + MulDiv(3,GetGValue(color),10);
  DWORD blue  = MulDiv(7,GetBlue(),10) + MulDiv(3,GetBValue(color),10);
  return RGB(red,green,blue);
}

COLORREF XColorRef::LightColor(BYTE lScale){
  DWORD red   = MulDiv(255-GetRed(),lScale,255)+GetRed();
  DWORD green = MulDiv(255-GetGreen(),lScale,255)+GetGreen();
  DWORD blue  = MulDiv(255-GetBlue(),lScale,255)+GetBlue();
  return RGB(red,green,blue);
}

COLORREF XColorRef::DarkColor(BYTE lScale){
  DWORD red   = MulDiv(GetRed(),(255-lScale),255);
  DWORD green = MulDiv(GetGreen(),(255-lScale),255);
  DWORD blue  = MulDiv(GetBlue(),(255-lScale),255);
  return RGB(red,green,blue);
}

COLORREF XColorRef::GrayColor(){
  BYTE Gray=(((int)GetRed())+GetGreen()+GetBlue())/3;
  return RGB(Gray,Gray,Gray);
}

COLORREF XColorRef::GrayScale(){
  BYTE Gray=(INT)((GetRed()*0.2125)+(GetGreen()*0.7154)+(GetBlue()*0.0721));
  return RGB(Gray,Gray,Gray);
}

COLORREF XColorRef::AlphaBlendColor(INT weighting){
  DWORD red   = ((weighting * GetRed()) + ((255-weighting) * GetRValue(g_color))) / 256;
  DWORD green = ((weighting * GetGreen()) + ((255-weighting) * GetGValue(g_color))) / 256;
  DWORD blue  = ((weighting * GetBlue()) + ((255-weighting) * GetBValue(g_color))) / 256;
  return RGB(red,green,blue);
}

COLORREF XColorRef::BleachColor(BYTE cant,BOOL Add){
  DWORD red,green,blue;
  if(Add){
    red   = __min(GetRed()+cant,255);
    green = __min(GetGreen()+cant,255);
    blue  = __min(GetBlue()+cant,255);
  }else{
    red   = __max(GetRed()-cant,0);
    green = __max(GetGreen()-cant,0);
    blue  = __max(GetBlue()-cant,0);
  }
  return RGB(red,green,blue);
}

COLORREF XColorRef::BrigthColor(BYTE porc,BOOL Add){
  DWORD red   = GetRed();
  DWORD green = GetGreen();
  DWORD blue  = GetBlue();
  if(Add){
    red+=red*porc/100;
    if(red<GetRed())red=0xFF;
    green+=green*porc/100;
    if(green<GetGreen())green=0xFF;
    blue+=blue*porc/100;
    if(blue<GetBlue())blue=0xFF;
  }else{
    red-=red*porc/100;
    if(red>GetRed())red=0x00;
    green-=green*porc/100;
    if(green>GetGreen())green=0x00;
    blue-=blue*porc/100;
    if(blue>GetBlue())blue=0x00;
  }
  return RGB(red,green,blue);
}

COLORREF XColorRef::SetBrightness(double brightness){
  HSL hsl=RGBtoHSL();
  hsl.L=brightness;
  if(hsl.L>1)hsl.L=1;
  else if((hsl.L<0))hsl.L=0;
  return HSLtoRGB(hsl);
}

COLORREF XColorRef::ModifyBrightness(double brightness){
  HSL hsl=RGBtoHSL();
  hsl.L*=brightness;
  if(hsl.L>1)hsl.L=1;
  else if((hsl.L<0))hsl.L=0;
  return HSLtoRGB(hsl);
}

COLORREF XColorRef::SetSaturation(double Saturation){
  HSL hsl=RGBtoHSL();
  hsl.S=Saturation;
  if(hsl.S>1)hsl.S=1;
  else if((hsl.S<0))hsl.S=0;
  return HSLtoRGB(hsl);
}

COLORREF XColorRef::ModifySaturation(double Saturation){
  HSL hsl=RGBtoHSL();
  hsl.S*=Saturation;
  if(hsl.S>1)hsl.S=1;
  else if((hsl.S<0))hsl.S=0;
  return HSLtoRGB(hsl);
}

COLORREF XColorRef::SetHue(double Hue){
  HSL hsl=RGBtoHSL();
  hsl.H=Hue;
  if(hsl.H>360)hsl.H=360;
  else if((hsl.H<0))hsl.H=0;
  return HSLtoRGB(hsl);
}

COLORREF XColorRef::ModifyHue(double Hue){
  HSL hsl=RGBtoHSL();
  hsl.H*=Hue;
  if(hsl.H>360)hsl.H=360;
  else if((hsl.H<0))hsl.H=0;
  return HSLtoRGB(hsl);
}

/**************************************************************/
// XDC REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XDC,XObject)

VOID XDC::FillSolidRgn(HRGN hrgn,COLORREF fillcol){
  XASSERT(m_hDC!=NULL)
  HBRUSH hbr=::CreateSolidBrush(fillcol);
  ::FillRgn(m_hDC,hrgn,hbr);
  ::DeleteObject(hbr);
}

VOID XDC::FillSolidRect(LPRECT rc,COLORREF fillcol){
  XASSERT(m_hDC!=NULL)
  INT pclr=::SetBkColor(m_hDC,fillcol);
	::ExtTextOut(m_hDC,0,0,ETO_OPAQUE,rc,NULL,0,NULL);
  ::SetBkColor(m_hDC,pclr);
}

VOID XDC::FillSolidRect(INT nXDest,INT nYDest,INT nWidth,INT nHeight,COLORREF fillcol){
  RECT rc={nXDest,nYDest,nXDest+nWidth,nYDest+nHeight};
  return FillSolidRect(&rc,fillcol);
}

VOID XDC::Draw3dRect(LPCRECT lpRect,COLORREF clrTopLeft,COLORREF clrBottomRight){
	Draw3dRect(lpRect->left,lpRect->top,lpRect->right-lpRect->left,
		lpRect->bottom-lpRect->top,clrTopLeft,clrBottomRight);
}

VOID XDC::Draw3dRect(INT x,INT y,INT cx,INT cy,COLORREF clrTopLeft,COLORREF clrBottomRight){
	FillSolidRect(x, y, cx - 1, 1, clrTopLeft);
	FillSolidRect(x, y, 1, cy - 1, clrTopLeft);
	FillSolidRect(x + cx, y, -1, cy, clrBottomRight);
	FillSolidRect(x, y + cy, cx, -1, clrBottomRight);
}

VOID XDC::DrawShadow(LPRECT rc,COLORREF color){
  XASSERT(m_hDC!=NULL)
  XColorRef clrMenuBar(color);
  DWORD X,Y,rHeight=rc->bottom-rc->top;
  for(X=1;X<=4;X++){
    for(Y=0;Y<4;Y++)
      ::SetPixel(m_hDC,rc->right+4-X,Y+rc->top,clrMenuBar.GetColor());
    for (Y=4;Y<8;Y++)
      ::SetPixel(m_hDC,rc->right+4-X,Y+rc->top,clrMenuBar.DarkColor(2* 3 * X * (Y - 3)));
    for (Y=8;Y<=(rHeight-1);Y++)
      ::SetPixel(m_hDC,rc->right+4-X,Y+rc->top,clrMenuBar.DarkColor(2*15 * X));
  }
}

VOID XDC::DrawGradient(LPRECT rc,COLORREF color1,COLORREF color2,BYTE direction){
  XASSERT(m_hDC!=NULL)
  //Check num of colors...
  INT Count=::GetDeviceCaps(m_hDC,NUMCOLORS);
  if(Count==-1){
    DWORD size=0,wlong=0;
    switch(direction){
      case 2://diagonal
        size=rc->right-rc->left+rc->bottom-rc->top;
        wlong=rc->right-rc->left;
      break;
      case 1://vertical
        size=rc->bottom-rc->top;
        wlong=rc->right-rc->left;
      break;
      default://0 horizontal
        size=rc->right-rc->left;
        wlong=rc->bottom-rc->top;
      break;
    }
  	//initial colors
    float fColorRed=GetRValue(color1);
    float fColorGreen=GetGValue(color1);
    float fColorBlue=GetBValue(color1);
    //Get color incremment
    float fRedFade = ((float)GetRValue(color2)-fColorRed)/float(size);
    float fGreenFade = ((float)GetGValue(color2)-fColorGreen)/float(size);
    float fBlueFade = ((float)GetBValue(color2)-fColorBlue)/float(size);
    //Draw Lines
    for(UINT fade=0;fade<size;fade++){
      COLORREF color=RGB((BYTE)fColorRed,(BYTE)fColorGreen,(BYTE)fColorBlue);
      switch(direction){
        case 2:{ //Diagonal...
          //Creates a new pen
          HPEN hNewPen=::CreatePen(PS_SOLID,0,color);
          HPEN hOldPen=(HPEN)::SelectObject(m_hDC,hNewPen);
          //Draw Diag Lines
          if(fade<=wlong){
            ::MoveToEx(m_hDC,rc->left+fade,rc->top,NULL);
            ::LineTo(m_hDC,rc->left,rc->top+fade);
          }else{
            ::MoveToEx(m_hDC,rc->right,rc->top+(fade-wlong),NULL);
            ::LineTo(m_hDC,rc->left+(fade-wlong),rc->bottom);
          }
          //Reset pen
          ::SelectObject(m_hDC,hOldPen);
          ::DeleteObject(hNewPen);
        }
        break;
        case 1://Vertical...
          FillSolidRect(rc->left,rc->top+fade,wlong,1,color);
        break;
        default://0 Horizontal...
          FillSolidRect(rc->left+fade,rc->top,1,wlong,color);
        break;
      }
      //Inc colors
      fColorRed+=fRedFade;
      fColorGreen+=fGreenFade;
      fColorBlue+=fBlueFade;
    }
  }else
    FillSolidRect(rc,GetNearestColor(color2));
}

VOID XDC::DrawSolidRectangle(LPRECT rc,COLORREF outlincol,COLORREF fillcol,INT nWidth){
  DrawSolidRectangle(rc->left,rc->top,rc->right,rc->bottom,outlincol,fillcol,nWidth);
}

VOID XDC::DrawSolidRectangle(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,COLORREF fillcol,INT nWidth){
  XASSERT(m_hDC!=NULL)
  HPEN hPen=(HPEN)::SelectObject(m_hDC,::CreatePen(PS_SOLID,nWidth,outlincol));
  HBRUSH hBrush=(HBRUSH)::SelectObject(m_hDC,::CreateSolidBrush(fillcol));
  ::Rectangle(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
  ::DeleteObject(::SelectObject(m_hDC,hPen));
  ::DeleteObject(::SelectObject(m_hDC,hBrush));
}

VOID XDC::FrameSolidRect(LPRECT rc,COLORREF outlincol,INT nWidth){
  FrameSolidRect(rc->left,rc->top,rc->right,rc->bottom,outlincol,nWidth);
}

VOID XDC::FrameSolidRect(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,INT nWidth){
  XASSERT(m_hDC!=NULL)
  HPEN hPen=(HPEN)::SelectObject(m_hDC,::CreatePen(PS_SOLID,nWidth,outlincol));
  HBRUSH hBrush=(HBRUSH)::SelectObject(m_hDC,::GetStockObject(NULL_BRUSH));
  ::Rectangle(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
  ::DeleteObject(::SelectObject(m_hDC,hPen));
  ::DeleteObject(::SelectObject(m_hDC,hBrush));
}

VOID XDC::FillSolidEllipse(LPRECT rc,COLORREF fillcol){
  FillSolidEllipse(rc->left,rc->top,rc->right,rc->bottom,fillcol);
}

VOID XDC::FillSolidEllipse(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF fillcol){
  XASSERT(m_hDC!=NULL)
  HPEN hPen=(HPEN)::SelectObject(m_hDC,::CreatePen(PS_SOLID,0,fillcol));
  HBRUSH hBrush=(HBRUSH)::SelectObject(m_hDC,::CreateSolidBrush(fillcol));
  ::Ellipse(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
  ::DeleteObject(::SelectObject(m_hDC,hPen));
  ::DeleteObject(::SelectObject(m_hDC,hBrush));
}

VOID XDC::DrawSolidEllipse(LPRECT rc,COLORREF outlincol,COLORREF fillcol,INT nWidth){
  DrawSolidEllipse(rc->left,rc->top,rc->right,rc->bottom,outlincol,fillcol,nWidth);
}

VOID XDC::DrawSolidEllipse(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,COLORREF fillcol,INT nWidth){
  XASSERT(m_hDC!=NULL)
  HPEN hPen=(HPEN)::SelectObject(m_hDC,::CreatePen(PS_SOLID,nWidth,outlincol));
  HBRUSH hBrush=(HBRUSH)::SelectObject(m_hDC,::CreateSolidBrush(fillcol));
  ::Ellipse(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
  ::DeleteObject(::SelectObject(m_hDC,hPen));
  ::DeleteObject(::SelectObject(m_hDC,hBrush));
}

VOID XDC::FrameSolidEllipse(LPRECT rc,COLORREF outlincol,INT nWidth){
  FrameSolidEllipse(rc->left,rc->top,rc->right,rc->bottom,outlincol,nWidth);
}

VOID XDC::FrameSolidEllipse(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,COLORREF outlincol,INT nWidth){
  XASSERT(m_hDC!=NULL)
  HPEN hPen=(HPEN)::SelectObject(m_hDC,::CreatePen(PS_SOLID,nWidth,outlincol));
  HBRUSH hBrush=(HBRUSH)::SelectObject(m_hDC,::GetStockObject(NULL_BRUSH));
  ::Ellipse(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect);
  ::DeleteObject(::SelectObject(m_hDC,hPen));
  ::DeleteObject(::SelectObject(m_hDC,hBrush));
}

VOID XDC::DrawSolidLine(LPPOINT pt,LPSIZE sz,COLORREF lincol,INT nWidth){
  DrawSolidLine(pt->x,pt->y,pt->x+sz->cx,pt->y+sz->cy,lincol,nWidth);
}

VOID XDC::DrawSolidLine(INT left,INT top,INT right,INT bottom,COLORREF lincol,INT nWidth){
  XASSERT(m_hDC!=NULL)
  HPEN hPen=(HPEN)::SelectObject(m_hDC,::CreatePen(PS_SOLID,nWidth,lincol));
  ::MoveToEx(m_hDC,left,top,NULL);
  ::LineTo(m_hDC,right,bottom);
  ::DeleteObject(::SelectObject(m_hDC,hPen));
}

VOID XDC::DrawSolidRoundRect(LPRECT rc,LPPOINT pt,COLORREF outlincol,COLORREF fillcol,INT nWidth){
  DrawSolidRoundRect(rc->left,rc->top,rc->right,rc->bottom,pt->x,pt->y,outlincol,fillcol,nWidth);
}

VOID XDC::DrawSolidRoundRect(INT nLeftRect,INT nTopRect,INT nRightRect,INT nBottomRect,INT nWidth,INT nHeight,COLORREF outlincol,COLORREF fillcol,INT nBorderWidth){
  XASSERT(m_hDC!=NULL)
  HPEN hPen=(HPEN)::SelectObject(m_hDC,::CreatePen(PS_SOLID,nBorderWidth,outlincol));
  HBRUSH hBrush=(HBRUSH)::SelectObject(m_hDC,::CreateSolidBrush(fillcol));
  ::RoundRect(m_hDC,nLeftRect,nTopRect,nRightRect,nBottomRect,nWidth,nHeight);
  ::DeleteObject(::SelectObject(m_hDC,hPen));
  ::DeleteObject(::SelectObject(m_hDC,hBrush));
}

VOID XDC::DrawColoredText(LPCTSTR lpString,LPRECT rect,INT bkgmode,COLORREF bkgcol,COLORREF txtcol,UINT uFormat,INT nCount){
  INT iBkMode=::SetBkMode(m_hDC,bkgmode);
  INT iBkgColor=::SetBkColor(m_hDC,bkgcol);
  INT iTxtColor=::SetTextColor(m_hDC,txtcol);
  ::DrawText(m_hDC,lpString,nCount,rect,uFormat);
  ::SetBkMode(m_hDC,iBkMode);
  ::SetBkColor(m_hDC,iBkgColor);
  ::SetTextColor(m_hDC,iTxtColor);
}

VOID XDC::TextOutEx(INT nXStart,INT nYStart,LPCTSTR lpString,INT bkgmode,COLORREF bkgcol,COLORREF txtcol){
  INT iBkMode=::SetBkMode(m_hDC,bkgmode);
  INT iBkgColor=::SetBkColor(m_hDC,bkgcol);
  INT iTxtColor=::SetTextColor(m_hDC,txtcol);
  ::TextOut(m_hDC,nXStart,nYStart,lpString,lstrlen(lpString));
  ::SetBkMode(m_hDC,iBkMode);
  ::SetBkColor(m_hDC,iBkgColor);
  ::SetTextColor(m_hDC,iTxtColor);
}

BOOL XDC::DrawSpecialChar(UINT x,UINT y,TCHAR Sign,COLORREF cColor,UINT size){
  HFONT marFont=::CreateFont(size,0,0,0,FF_DONTCARE,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH,_T("Marlett"));
  if(marFont){
    HFONT hOldFont=(HFONT)::SelectObject(m_hDC,marFont);
    COLORREF cOldColor=::SetTextColor(m_hDC,cColor);
    INT iOldMode=::SetBkMode(m_hDC,TRANSPARENT);
    ::TextOut(m_hDC,x,y,&Sign,1);
    ::SetBkMode(m_hDC,iOldMode);
    ::SetTextColor(m_hDC,cOldColor);
    ::SelectObject(m_hDC,hOldFont);
    ::DeleteObject(marFont);
    return TRUE;
  }
  return FALSE;
}

BOOL XDC::PolyPatBlt(PRECT pRects,UINT cRects,HBRUSH hBrush,DWORD dwRop){
  if(!pRects)return FALSE;
	HBRUSH hBrOld=(HBRUSH)::SelectObject(m_hDC,hBrush);
	PRECT r=pRects;
	for(UINT i=0;i<cRects;i++,r++)
		::PatBlt(m_hDC,r->left,r->top,r->right-r->left,r->bottom-r->top,dwRop);
	::SelectObject(m_hDC,hBrOld);
	return TRUE;
}

BOOL XDC::FramePatBlt(PRECT pRect,UINT uWidth,HBRUSH hBrush,DWORD dwRop){
  if(!pRect)return FALSE;
  HBRUSH hBrOld=(HBRUSH)::SelectObject(m_hDC,hBrush);
	::PatBlt(m_hDC,pRect->left,pRect->top,pRect->right-pRect->left,pRect->top+uWidth,dwRop);
	::PatBlt(m_hDC,pRect->left,pRect->top+uWidth,pRect->left+uWidth,pRect->bottom-pRect->top-2*uWidth,dwRop);
	::PatBlt(m_hDC,pRect->right-uWidth,pRect->top+uWidth,pRect->right,pRect->bottom-pRect->top-2*uWidth,dwRop);
	::PatBlt(m_hDC,pRect->left,pRect->bottom-uWidth,pRect->right-pRect->left,pRect->bottom,dwRop);
  ::SelectObject(m_hDC,hBrOld);
	return TRUE;
}

VOID XDC::DPtoHIMETRIC(LPSIZE lpSize)const{
  XASSERT(m_hDC!=NULL)
	XASSERT(lpSize!=NULL)
	INT nMapMode=GetMapMode();
	if(nMapMode<MM_ISOTROPIC&&nMapMode!=MM_TEXT){
		SetMapMode(MM_HIMETRIC);
		DPtoLP(lpSize);
		SetMapMode(nMapMode);
	}else{
    INT cxPerInch=GetDeviceCaps(LOGPIXELSX);
		INT cyPerInch=GetDeviceCaps(LOGPIXELSY);
		XASSERT(cxPerInch!=0&&cyPerInch!=0)
		lpSize->cx=MulDiv(lpSize->cx,HIMETRIC_INCH,cxPerInch);
		lpSize->cy=MulDiv(lpSize->cy,HIMETRIC_INCH,cyPerInch);
	}
}

VOID XDC::HIMETRICtoDP(LPSIZE lpSize)const{
  XASSERT(m_hDC!=NULL)
	XASSERT(lpSize!=NULL)
	INT nMapMode=GetMapMode();
	if(nMapMode<MM_ISOTROPIC&&nMapMode!=MM_TEXT){
		SetMapMode(MM_HIMETRIC);
		LPtoDP(lpSize);
		SetMapMode(nMapMode);
	}else{
		INT cxPerInch = GetDeviceCaps(LOGPIXELSX);
		INT cyPerInch = GetDeviceCaps(LOGPIXELSY);
		XASSERT(cxPerInch!=0&&cyPerInch!=0)
		lpSize->cx = MulDiv(lpSize->cx, cxPerInch, HIMETRIC_INCH);
		lpSize->cy = MulDiv(lpSize->cy, cyPerInch, HIMETRIC_INCH);
	}
}

/**************************************************************/
// XPAINTDC REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XPaintDC,XDC)

/**************************************************************/
// XCLIENTDC REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XClientDC,XDC)

/**************************************************************/
// XWINDOWDC REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XWindowDC,XDC)

/**************************************************************/
// XDCEX REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XDCEx,XDC)

/**************************************************************/
// XGDIOBJECT REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XGDIObject,XObject)

/**************************************************************/
// XPEN REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XPen,XGDIObject)

/**************************************************************/
// XBRUSH REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XBrush,XGDIObject)

/**************************************************************/
// XFONT REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XFont,XGDIObject)

HFONT XFont::CreatePointFont(INT nPointSize,LPCTSTR lpszFaceName,HDC hDC,BOOL bBold,BOOL bItalic,BOOL bUnderline){
  XASSERT(lpszFaceName!=NULL)
  LOGFONT logFont;
  ZeroMemory(&logFont,sizeof(logFont));
  logFont.lfCharSet=DEFAULT_CHARSET;
  logFont.lfHeight=nPointSize;
  lstrcpy(logFont.lfFaceName,lpszFaceName);
  logFont.lfWeight=(bBold)?FW_BOLD:FW_NORMAL;
  logFont.lfItalic=(BYTE)bItalic;
  logFont.lfUnderline=(BYTE)bUnderline;
  return CreatePointFontIndirect(&logFont,hDC);
}

HFONT XFont::CreatePointFontIndirect(LOGFONT* lpLogFont,HDC hDC){
  HDC hDC1=(!hDC)?::GetDC(NULL):hDC;
  POINT pt={0,0},ptOrg={0,0};
  pt.y=::MulDiv(::GetDeviceCaps(hDC1,LOGPIXELSY),lpLogFont->lfHeight,720);
  ::DPtoLP(hDC1,&pt,1);
  ::DPtoLP(hDC1,&ptOrg,1);
  lpLogFont->lfHeight=-abs(pt.y-ptOrg.y);
  if(!hDC)::ReleaseDC(NULL,hDC1);
  return (HFONT)Attach(::CreateFontIndirect(lpLogFont));
}

/**************************************************************/
// XBITMAP REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XBitmap,XGDIObject)

BOOL XBitmap::Draw(HDC hdc,LPPOINT pt,LPSIZE bsz,LPPOINT spt,HBITMAP g_bitmask){
  HDC hdcMem;BOOL bRet;SIZE fbsz;
  if(!bsz)fbsz=GetBitmapDimension();
  else fbsz=*bsz;
  hdcMem=::CreateCompatibleDC(hdc);
  if(g_bitmask){
    ::SelectObject(hdcMem,g_bitmask);
    ::BitBlt(hdc,pt->x,pt->y,fbsz.cx,fbsz.cy,hdcMem,(spt)?spt->x:0,(spt)?spt->y:0,SRCAND);
  }
  ::SelectObject(hdcMem,(HBITMAP)m_hObject);
  bRet=::BitBlt(hdc,pt->x,pt->y,fbsz.cx,fbsz.cy,hdcMem,(spt)?spt->x:0,(spt)?spt->y:0,SRCCOPY);
  ::DeleteDC(hdcMem);
  return bRet;
}

/**************************************************************/
// XPALETTE REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XPalette,XGDIObject)

/**************************************************************/
// XRGN REFERENCE...
/**************************************************************/

IMPLEMENT_DYNAMIC(XRgn,XGDIObject)
