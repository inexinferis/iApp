#ifndef _XCOORD_CPP_
#define _XCOORD_CPP_

#ifndef _XCOORD_H_
	//#error xcoords.cpp requires xcoords.h to be included first
#else


// XSize

XINLINE XSize::XSize(){cx=0;cy=0;}
XINLINE XSize::XSize(LONG x,LONG y){cx=x;cy=y;}

XINLINE XSize::XSize(const SIZE &size){
  cx=size.cx;cy=size.cy;
}

XINLINE XSize::XSize(DWORD dwSize){
  cx=(short)LOWORD(dwSize);
  cy=(short)HIWORD(dwSize);
}

XINLINE XSize &XSize::SetSize(LONG nx,LONG ny){
  cx=nx;cy=ny;
  return *this;
}

XINLINE XSize &XSize::Offset(SIZE size){
  cx += size.cx; cy += size.cy;
  return *this;
}

XINLINE XSize &XSize::Offset(INT nx,INT ny){
  cx+=nx;cy+=ny;
  return *this;
}

XINLINE BOOL XSize::operator==(const SIZE &sZ) {
  return ((cx==sZ.cx)&&(cy==sZ.cy));
}

XINLINE BOOL XSize::operator!=(const SIZE &sZ) {
  return ((cx!=sZ.cx)||(cy!=sZ.cy));
}

XINLINE XSize &XSize::operator-=(const SIZE &sZ){
  cx -= sZ.cx; cy -= sZ.cy;
  return *this;
}

XINLINE XSize &XSize::operator+=(const SIZE &sZ){
  cx += sZ.cx; cy += sZ.cy;
  return *this;
}

XINLINE XSize XSize::operator-(const POINT &pP){
  return XSize(cx - pP.x, cy - pP.y);
}

XINLINE XSize XSize::operator+(const POINT &pP){
  return XSize(cx + pP.x, cy + pP.y);
}

XINLINE XSize XSize::operator-(const SIZE &sZ){
  return XSize(cx - sZ.cx, cy - sZ.cy);
}

XINLINE XSize XSize::operator+(const SIZE &sZ){
  return XSize(cx + sZ.cx, cy + sZ.cy);
}

XINLINE XSize XSize::operator-(LONG offset){
  return XSize(cx-offset, cy-offset);
}

XINLINE XSize XSize::operator+(LONG offset){
  return XSize(cx+offset, cy+offset);
}

XINLINE XSize XSize::operator-(){
  return XSize(-cx, -cy);
}

XINLINE XSize &XSize::operator=(const SIZE &sZ){
  cx = sZ.cx; cy = sZ.cy;
  return *this;
}

XINLINE XSize &XSize::operator=(const RECT &rc) {
  cx = rc.right-rc.left;cy = rc.bottom-rc.top;
  return *this;
}

XINLINE XSize::operator LPSIZE(){
  return (LPSIZE)this;
}

XINLINE XSize::operator const SIZE*() const {
  return (SIZE*)this;
}

#ifdef _DEBUG
XINLINE VOID XSize::Dump(){
  DbgPrint(_T("Size: %d %d\n"),(int)cx,(int)cy);
}
#endif


// XPoint

XINLINE XPoint::XPoint(){x=0,y=0;}
XINLINE XPoint::XPoint(LONG nx,LONG ny){x=nx;y=ny;}
XINLINE XPoint::XPoint(const POINT &point) {
  x=point.x;y=point.y;
}

XINLINE XPoint::XPoint(DWORD dwPoint){
  x = (short)LOWORD(dwPoint);
  y = (short)HIWORD(dwPoint);
}

XINLINE XPoint &XPoint::SetPoint(LONG nx,LONG ny){
  x=nx;y=ny;
  return *this;
}

XINLINE XPoint &XPoint::Offset(POINT point){
  x += point.x; y += point.y;
  return *this;
}

XINLINE XPoint &XPoint::Offset(INT nx,INT ny){
  x += nx; y += ny;
  return *this;
}

XINLINE BOOL XPoint::operator==(const POINT &pP) {
  return ((x==pP.x)&&(y==pP.y));
}

XINLINE BOOL XPoint::operator!=(const POINT &pP) {
  return ((x!=pP.x)||(y!=pP.y));
}

XINLINE XPoint &XPoint::operator-=(const POINT &pP){
  x -= pP.x; y -= pP.y;
  return *this;
}

XINLINE XPoint &XPoint::operator+=(const POINT &pP){
  x += pP.x; y += pP.y;
  return *this;
}

XINLINE XPoint XPoint::operator-(const POINT &pP){
  return XPoint(x - pP.x, y - pP.y);
}

XINLINE XPoint XPoint::operator+(const POINT &pP){
  return XPoint(x + pP.x, y + pP.y);
}

XINLINE XPoint XPoint::operator-(const SIZE &sZ){
  return XPoint(x - sZ.cx, y - sZ.cy);
}

XINLINE XPoint XPoint::operator+(const SIZE &sZ){
  return XPoint(x + sZ.cx, y + sZ.cy);
}

XINLINE XPoint XPoint::operator-(LONG offset){
  return XPoint(x-offset, y-offset);
}

XINLINE XPoint XPoint::operator+(LONG offset){
  return XPoint(x+offset, y+offset);
}

XINLINE XPoint XPoint::operator-(){
  return XPoint(-x, -y);
}

XINLINE XPoint &XPoint::operator=(const POINT &initPt) {
  *(POINT*)this = initPt;
  return *this;
}

XINLINE XPoint &XPoint::operator=(const RECT &rc) {
  x = rc.left;y = rc.top;
  return *this;
}

XINLINE XPoint::operator LPPOINT(){
  return (LPPOINT)this;
}

XINLINE XPoint::operator const POINT*() const {
  return (POINT*)this;
}

#ifdef _DEBUG
XINLINE VOID XPoint::Dump(){
  DbgPrint(_T("Point: %d %d\n"),(int)x,(int)y);
}
#endif


// XRect

XINLINE XRect::XRect(){left=top=right=bottom=0;}
XINLINE XRect::XRect(LONG nx,LONG ny){left=top=0;right=nx;bottom=ny;}
XINLINE XRect::XRect(LONG x,LONG y,LONG cx,LONG cy){left=x;top=y;right=cx;bottom=cy;}

XINLINE XRect::XRect(const RECT &pP){*(RECT*)this=pP;}
XINLINE XRect::XRect(const RECT *pP){*(RECT*)this=*pP;}

XINLINE XRect::XRect(const POINT &pPt,const SIZE &pSz){
  right = (left = pPt.x) + pSz.cx;
  bottom = (top = pPt.y) + pSz.cy;
}

XINLINE XRect::XRect(const POINT &pPt,const POINT &pSz){
  left = pPt.x; top = pPt.y;
  right = pSz.x; bottom = pSz.y;
}

XINLINE XRect &XRect::SetTopLeft(LONG x,LONG y){
  left=x;top=y;
  return *this;
}

XINLINE XRect &XRect::SetTopLeft(const PPOINT pP){
  left=pP->x;top=pP->y;
  return *this;
}

XINLINE XRect &XRect::SetBottomRight(LONG cx,LONG cy){
  right=cx;bottom=cy;
  return *this;
}

XINLINE XRect &XRect::SetBottomRight(const PPOINT pP){
  right=pP->x;bottom=pP->y;
  return *this;
}

XINLINE XRect &XRect::SetSize(const PSIZE nSz){
  right=left+nSz->cx;bottom=top+nSz->cy;
  return *this;
}

XINLINE BOOL XRect::SetRect(LONG x,LONG cx,LONG y,LONG cy){
  return ::SetRect(this,x,y,cx,cy);
}

XINLINE XRect &XRect::SetRect(const PPOINT pPt,const PSIZE pSz){
  right = (left = pPt->x) + pSz->cx;
  bottom = (top = pPt->y) + pSz->cy;
  return *this;
}

XINLINE XRect &XRect::SetRect(const PPOINT pPt,const PPOINT pSz){
  left = pPt->x; top = pPt->y;
  right = pSz->x; bottom = pSz->y;
  return *this;
}

XINLINE BOOL XRect::CopyRect(const RECT &pP) {
  return ::CopyRect(this, &pP);
}

XINLINE VOID XRect::SetRectEmpty(){
  ::SetRectEmpty(this);
}

XINLINE LONG XRect::Width(){
  return (right - left);
}

XINLINE LONG XRect::Height(){
  return (bottom - top);
}

XINLINE LONG XRect::GetXProm(){
  return ((right + left)/2);
}

XINLINE LONG XRect::GetYProm(){
  return ((bottom + top)/2);
}

XINLINE SIZE XRect::Size(){
  SIZE tmp={right - left, bottom - top};
  return tmp;
}

XINLINE POINT XRect::TopLeft(){
  POINT tmp={left,top};
  return tmp;
}

XINLINE POINT XRect::BottomRight(){
  POINT tmp={right,bottom};
  return tmp;
}

XINLINE POINT XRect::CenterPoint(){
  POINT tmp={(right - left)/2,(bottom - top)/2};
  return tmp;
}

XINLINE VOID XRect::SwapLeftRight(){
  SwapLeftRight(LPRECT(this));
}

XINLINE VOID XRect::SwapLeftRight(LPRECT lpRect){
  LONG temp=lpRect->left;
  lpRect->left=lpRect->right;
  lpRect->right=temp;
}

XINLINE BOOL XRect::IsRectEmpty(){
  return ::IsRectEmpty(this);
}

XINLINE BOOL XRect::IsRectNull(){
  return (!left&&!top&&!right&&!bottom);
}

XINLINE BOOL XRect::EqualRect(LPCRECT rect){
  return ::EqualRect(this, rect);
}

XINLINE BOOL XRect::InflateRect(INT cx,INT cy){
  return ::InflateRect(this, cx, cy);
}

XINLINE BOOL XRect::InflateRect(const SIZE &pSz){
  return ::InflateRect(this, pSz.cx, pSz.cy);
}

XINLINE XRect &XRect::InflateRect(LPCRECT lpRect){
  left-=lpRect->left;top-=lpRect->top;
  right+=lpRect->right;bottom+=lpRect->bottom;
  return *this;
}

XINLINE XRect &XRect::InflateRect(INT l, INT t, INT r, INT b){
  left-=l;top-=t;
  right+=r;bottom+=b;
  return *this;
}

XINLINE BOOL XRect::DeflateRect(INT cx,INT cy){
  return ::InflateRect(this, -cx, -cy);
}

XINLINE BOOL XRect::DeflateRect(const SIZE &pSz){
  return ::InflateRect(this, -pSz.cx, -pSz.cy);
}

XINLINE XRect &XRect::DeflateRect(LPCRECT lpRect){
  left+=lpRect->left;top+=lpRect->top;
  right-=lpRect->right;bottom-=lpRect->bottom;
  return *this;
}

XINLINE XRect &XRect::DeflateRect(INT l, INT t, INT r, INT b){
  left+=l;top+= t;
  right-=r;bottom-=b;
  return *this;
}

XINLINE XRect &XRect::ResizeRect(INT r, INT b){
  right+=r;bottom+=b;
  return *this;
}

XINLINE XRect &XRect::ResizeRect(INT l, INT t, INT r, INT b){
  left+=l;top+= t;
  right+=r;bottom+=b;
  return *this;
}

XINLINE BOOL XRect::OffsetRect(INT cx,INT cy){
  return ::OffsetRect(this,cx,cy);
}

XINLINE BOOL XRect::OffsetRect(POINT &pPT){
  return ::OffsetRect(this,pPT.x,pPT.y);
}

XINLINE BOOL XRect::OffsetRect(SIZE &pSz){
  return ::OffsetRect(this,pSz.cx,pSz.cy);
}

XINLINE VOID XRect::MoveToX(INT x){
  right=Width()+x;
  left=x;
}

XINLINE VOID XRect::MoveToY(INT y){
  bottom=Height()+y;
  top=y;
}

XINLINE VOID XRect::MoveToXY(INT x,INT y){
  MoveToX(x);
  MoveToY(y);
}

XINLINE VOID XRect::MoveToXY(POINT pt){
  MoveToX(pt.x);
  MoveToY(pt.y);
}

XINLINE BOOL XRect::UnionRect(LPRECT rect1,LPRECT rect2){
  return ::UnionRect(this,rect1,rect2);
}

XINLINE BOOL XRect::IntersectRect(LPRECT rect1,LPRECT rect2){
  return ::IntersectRect(this,rect1,rect2);
}

XINLINE BOOL XRect::SubtractRect(LPRECT rect1,LPRECT rect2){
  return ::SubtractRect(this,rect1,rect2);
}

XINLINE BOOL XRect::PtInRect(POINT opt){
  return ::PtInRect(this,opt);
}

XINLINE BOOL XRect::RectInRect(LPRECT orc){
  return (left >= orc->left && right <= orc->right &&
    top >= orc->top && bottom <= orc->bottom);
}

XINLINE BOOL XRect::NormalizeRect(){
  BOOL ret=FALSE;INT tmp;
  if(left>right){
    tmp = left;
    left = right;
    right = tmp;
    ret=TRUE;
  }
  if(top>bottom){
    tmp = top;
    top = bottom;
    bottom = tmp;
    ret=TRUE;
  }
  return ret;
}

XINLINE XRect XRect::MulDiv(INT nMultiplier,INT nDivisor) const {
  return XRect(
    ::MulDiv(left, nMultiplier, nDivisor),
    ::MulDiv(top, nMultiplier, nDivisor),
    ::MulDiv(right, nMultiplier, nDivisor),
    ::MulDiv(bottom, nMultiplier, nDivisor)
  );
}

XINLINE XRect &XRect::operator=(const RECT &rc){
  ::CopyRect(this,&rc);
  return *this;
}

XINLINE BOOL XRect::operator==(const RECT &pP) {
  return ::EqualRect(this,&pP);
}

XINLINE BOOL XRect::operator!=(const RECT &pP) {
  return !::EqualRect(this,&pP);
}

XINLINE XRect XRect::operator &(RECT &pP){
  XRect rect;
  ::IntersectRect(&rect,this,&pP);
  return rect;
}

XINLINE XRect &XRect::operator &=(RECT &pP){
  IntersectRect(this,&pP);
  return *this;
}

XINLINE XRect XRect::operator |(RECT &pP){
  XRect rect;
  ::UnionRect(&rect,this,&pP);
  return rect;
}

XINLINE XRect &XRect::operator |=(RECT &pP){
  UnionRect(this,&pP);
  return *this;
}

XINLINE XRect XRect::operator+(POINT &pt){
  XRect rect(*this);
  rect.OffsetRect(pt);
  return rect;
}

XINLINE XRect XRect::operator-(POINT pt){
  XRect rect(*this);
  rect.OffsetRect(-pt.x, -pt.y);
  return rect;
}

XINLINE XRect XRect::operator+(SIZE &size){
  XRect rect(*this);
  rect.OffsetRect(size);
  return rect;
}

XINLINE XRect XRect::operator-(SIZE size){
  XRect rect(*this);
  rect.OffsetRect(-size.cx, -size.cy);
  return rect;
}

XINLINE XRect XRect::operator+(LPCRECT lpRect){
  XRect rect(*this);
  rect.InflateRect(lpRect);
  return rect;
}

XINLINE XRect XRect::operator-(LPCRECT lpRect){
  XRect rect(*this);
  rect.DeflateRect(lpRect);
  return rect;
}

XINLINE XRect &XRect::operator+=(POINT &point){
  OffsetRect(point);
  return *this;
}

XINLINE XRect &XRect::operator+=(SIZE &size){
  OffsetRect(size);
  return *this;
}

XINLINE XRect &XRect::operator+=(LPCRECT lpRect){
  InflateRect(lpRect);
  return *this;
}

XINLINE XRect &XRect::operator-=(POINT point){
  ::OffsetRect(this,-point.x,-point.y);
  return *this;
}

XINLINE XRect &XRect::operator-=(SIZE size){
  ::OffsetRect(this,-size.cx,-size.cy);
  return *this;
}

XINLINE XRect &XRect::operator-=(LPCRECT lpRect){
  DeflateRect(lpRect);
  return *this;
}

XINLINE XRect::operator LPCRECT()const{
  return this;
}

XINLINE XRect::operator LPRECT(){
  return this;
}

#ifdef _DEBUG
XINLINE VOID XRect::Dump(){
  DbgPrint(_T("Rect: %d %d %d %d\n"),(int)left,(int)top,(int)right,(int)bottom);
}
#endif

#endif	// _XCOORD_H_

#endif // _XCOORD_CPP_
