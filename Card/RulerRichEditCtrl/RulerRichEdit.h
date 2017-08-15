#if !defined(AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_)
#define AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_

// RulerRichEdit.h : header file
//

#include "OleRichEditCtrl.h"

/////////////////////////////////////////////////////////////////////////////
// CRulerRichEdit window

class CRulerRichEdit : public COleRichEditCtrl
{
public:
// Construction/creation/destruction
	CRulerRichEdit();
	virtual ~CRulerRichEdit();

	BOOL Create( DWORD style, CRect rect, CWnd* parent );

protected:
// Message handlers
	//{{AFX_MSG(CRulerRichEdit)
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnChange();
	afx_msg UINT OnGetDlgCode();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

private:
// Private helpers
	void UpdateRuler();

};

#endif // !defined(AFX_RULERRICHEDIT_H__E10A8ED3_2E1D_402E_A599_003214085F1A__INCLUDED_)
