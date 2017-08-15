#pragma once

#include "RulerRichEditCtrl/RulerRichEditCtrl.h"

class CEditor
{
public:
	enum 
	{
		NONE = -1,
		RICHEDIT = 0,
		EDIT = 1,
	};
	CEditor() : m_type(NONE)
	{

	}
	~CEditor()
	{
	}
	BOOL CreateRichEdit(DWORD dwStyle, const RECT &rect, CWnd* pParentWnd, UINT nID, 
		CString font = _T( "Courier New" ), 
		int size = 10, 
		BOOL multiLine = TRUE, 
		BOOL autohscroll = FALSE)
	{
		m_type = RICHEDIT;
		BOOL res = m_richEdit.Create(dwStyle, rect, pParentWnd, nID, font, 
			size, multiLine, autohscroll);
		m_richEdit.GetRichEditCtrl().LimitText(1024 * 1024 * 10);
		return res;
	}
	BOOL CreateEdit(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID,
		CString font = _T("Arial"),
		int size = 10,
		BOOL multiLine = FALSE)
	{
		m_type = EDIT;
		if (multiLine)
			dwStyle |= ES_MULTILINE;
		else
			dwStyle &= ~ES_MULTILINE;

		if (m_edit.Create(dwStyle, rect, pParentWnd, nID) == FALSE)
			return FALSE;

		m_font.CreatePointFont(size * 10, font, NULL); 
		m_edit.SetFont(&m_font);
		return TRUE;
	}

	CEdit &Edit() { return m_edit; }
	CRulerRichEditCtrl &RichEdit() 
	{ 
		return m_richEdit; 
	}
	CWnd *GetWnd()
	{
		if (m_type == RICHEDIT)
			return &m_richEdit.GetRichEditCtrl();
		else if (m_type == EDIT)
			return &m_edit;

		return NULL;
	}
	void SetFocus()
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().SetFocus();
		else if (m_type == EDIT)
			m_edit.SetFocus();
	}
	void SetModify(BOOL modified)
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().SetModify(modified);
		else if (m_type == EDIT)
			m_edit.SetModify(modified);
	}
	BOOL GetModify()
	{
		if (m_type == RICHEDIT)
			return m_richEdit.GetRichEditCtrl().GetModify();
		else if (m_type == EDIT)
			return m_edit.GetModify();

		return FALSE;
	}
	void ReplaceSel(LPCTSTR s)
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().ReplaceSel(s);
		else if (m_type == EDIT)
			m_edit.ReplaceSel(s);
	}
/*
	CString GetRTF()
	{
		if (m_type == RICHEDIT)
			return m_richEdit.GetRTF();
		else 
			return "";
	}
	void SetRTF(const CString & str)
	{
		if (m_type == RICHEDIT)
			m_richEdit.SetRTF(str);
		else 
			m_edit.SetWindowText(str);
	}
*/
	void SetText(LPCTSTR str)
	{
		if (m_type == RICHEDIT)
			m_richEdit.SetRTF(str);
		else 
			m_edit.SetWindowText(str);
	}
	void GetText(CString & str)
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRTF(str);
		else 
			m_edit.GetWindowText(str);
	}

	void SetSel(long nStartChar, long nEndChar)
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().SetSel(nStartChar, nEndChar);
		else if (m_type == EDIT)
			m_edit.SetSel(nStartChar, nEndChar);
	}
	void Clear()
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().Clear();
		else if (m_type == EDIT)
			m_edit.Clear();
	}

    void SetAllFont(CString str)
    {
        SetSel(0, -1);
		if (m_type == RICHEDIT)
			m_richEdit.SetCurrentFontName(str);
		else if (m_type == EDIT)
        {
        }
    }
/*
	void SetWindowText(LPCTSTR str)
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().SetWindowText(str);
		else if (m_type == EDIT)
			m_edit.SetWindowText(str);
	}
	void GetWindowText(CString & str)
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().GetWindowText(str);
		else if (m_type == EDIT)
			m_edit.GetWindowText(str);
	}
*/
	void Copy()
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().Copy();
		else if (m_type == EDIT)
			m_edit.Copy();
	}
	void Paste()
	{
		if (m_type == RICHEDIT)
		{
			CRichEditCtrl &ctl = m_richEdit.GetRichEditCtrl();

			if (ctl.CanPaste(CF_TEXT) && ctl.CanPaste(CF_DIB))
				ctl.PasteSpecial(CF_TEXT);
			else if (ctl.CanPaste(CF_TEXT) && !ctl.CanPaste(CF_DIB))
				ctl.Paste();
			else if (!ctl.CanPaste(CF_TEXT) && ctl.CanPaste(CF_DIB))
				ctl.PasteSpecial(CF_DIB);
		}
		else if (m_type == EDIT)
			m_edit.Paste();
	}
	void Cut()
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().Cut();
		else if (m_type == EDIT)
			m_edit.Cut();
	}
	void Undo()
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().Undo();
		else if (m_type == EDIT)
			m_edit.Undo();
	}
	void SelectAll()
	{
		if (m_type == RICHEDIT)
			m_richEdit.GetRichEditCtrl().SetSel(0, -1);
		else if (m_type == EDIT)
			m_edit.SetSel(0, -1);
	}

	void DestroyWindow()
	{
		if (m_type == RICHEDIT)
			m_richEdit.DestroyWindow();
		else if (m_type == EDIT)
		{
			m_edit.DestroyWindow();
			m_font.DeleteObject();
		}
	}
	void SetReadOnly(BOOL readOnly)
	{
		if (m_type == RICHEDIT)
			m_richEdit.SetReadOnly(readOnly);
		else if (m_type == EDIT)
			m_edit.SetReadOnly(readOnly);
	}
	void ShowWindow(int nCmdShow)
	{
		if (m_type == RICHEDIT)
			m_richEdit.ShowWindow(nCmdShow);
		else if (m_type == EDIT)
			m_edit.ShowWindow(nCmdShow);
	}
	void ShowToolbar(BOOL show)
	{
		if (m_type == RICHEDIT)
			m_richEdit.ShowToolbar(show);
	}
	void ShowRuler(BOOL show)
	{
		if (m_type == RICHEDIT)
			m_richEdit.ShowRuler(show);
	}
	void SetLink(CString &text, CString &link, bool addLF = false)
	{
		if (m_type == RICHEDIT)
			m_richEdit.SetLink(text, link, addLF);
	}
private:
	int					m_type;
	CEdit				m_edit;
	CRulerRichEditCtrl	m_richEdit;
	CFont				m_font;

};
