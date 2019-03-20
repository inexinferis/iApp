#include "xdialog.h"

//Definición de Eventos a Interceptar...
BEGIN_EVENT_TABLE(XDialog,XWnd)
  EVENT_CTLCOLORDLG(XDialog::OnCtlColorDlg)
END_EVENT_TABLE()

IMPLEMENT_DYNAMIC(XDialog,XWnd)

HBRUSH XDialog::OnCtlColorDlg(HDC /*hDC*/,HWND /*hDlg*/){
  if(m_wcx.hbrBackground)
    return m_wcx.hbrBackground;
  return (HBRUSH)Default();
}

BOOL XDialog::EndDialog(INT_PTR nResult){
  if(m_hWnd==NULL||!::IsWindow(m_hWnd))
    return FALSE;
  if(m_nFlags&XWF_MODALLOOP){
    //save result
    m_nModRes=nResult;
    //remove modal flag...
		m_nFlags&=~XWF_MODALLOOP;
  }
  //Make sure still valid!!!
  if(m_hWnd!=NULL&&::IsWindow(m_hWnd))
    return ::EndDialog(m_hWnd,nResult);
  return FALSE;
}
