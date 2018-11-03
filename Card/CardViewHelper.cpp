// CardView.cpp : implementation of the CCardView class
//

#include "stdafx.h"
#include "Card.h"

#include "CardDoc.h"
#include "CardView.h"
#include "MainFrm.h"
#include "Setting.h"
#include "RtfParser/FcListener.h"
#include "RtfParser/HtmlListener.h"
#include "RtfParser/RtfParser.h"
#include <vector>
#include "http/WebServer.h"
#include "Util.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

int ConvertHtml2FlashCard(CString &src, CString &dst);

struct ConvertStrings
{
	string		text;
	vector<string> images;
};

typedef vector<ConvertStrings>		ConvertStringsList;

int CCardView::OpenDB()
{
	TCHAR			szTemp[512];
	CString			path, s;
	int				i;

	GetModuleFileName(NULL, szTemp, 512);

	path = szTemp;
	for(i = path.GetLength() - 1; i >= 0; i--)
	{
		if (path[i] == _T('\\'))
			break;
	}
	path = path.Left(i + 1);
	s = path + m_dbFolder;

	m_useWorkingFolder = GetEnv(PROFILE_SECTION, USE_WORKING_FOLDER, 0);
	if (m_useWorkingFolder)
	{
		if (m_db.Init(s, GetEnv(PROFILE_SECTION, WORKING_FOLDER, "c:\\temp\\")) == -1)
			return -1;
	}
	else
	{
		if (m_db.Init(s) == -1)
			return -1;
	}

	m_dirHandler.SetView(this);
	if (m_useWorkingFolder == TRUE)
	{
		DWORD dwWatch = 0;
		dwWatch = m_dirWatcher.WatchDirectory(
						s,
						FILE_NOTIFY_CHANGE_LAST_WRITE |
						FILE_NOTIFY_CHANGE_FILE_NAME |
						FILE_NOTIFY_CHANGE_CREATION,
						&m_dirHandler,
						FALSE,
						"*.db",
						"");
		if (dwWatch != ERROR_SUCCESS)
		{
			MessageBox("Fail to launch directory watcher", "Error");
			return -1;
		}
	}

    m_db.SetDbListener(this);

	return 0;
}

void CCardView::UpdateSection()
{
	CStringArray	sections;

	m_section->ResetContent();
	m_db.GetDbList(sections);
	for(int i = 0; i < (int) sections.GetSize(); i++)
	{
		m_section->AddString(sections[i]);
	}
	m_section->SetCurSel(m_section->FindStringExact(0, m_db.GetCurDbName()));

	GetDocument()->SetTitle( m_db.GetCurDbName());
}

void CCardView::UpdateCategory()
{
	CNames	category;
	int		n;

	if (m_db.GetCurDb() == NULL)
		return;

	m_itemCategory.ResetContent();
	m_category->ResetContent();
	m_db.GetCategoryList(category);
	n = m_category->AddString("ALL");
	m_category->SetItemData(n, -1);

	for(int i = 0; i < (int) category.GetSize(); i++)
	{
		n = m_category->AddString(category[i].name);
		m_category->SetItemData(n, category[i].id);
		n = m_itemCategory.AddString(category[i].name);
		m_itemCategory.SetItemData(n, category[i].id);
	}
	m_itemCategory.SetCurSel(0);

	if (m_db.GetCurCategory() == -1)
		m_category->SetCurSel(0);
	else
		m_category->SetCurSel(FindItemData(*m_category, m_db.GetCurCategory()));
		
}

void CCardView::UpdateTag()
{
	CNames	names;
	int		n;

	if (m_db.GetCurDb() == NULL)
		return;

	m_itemTagCombo.ResetContent();
	m_tagCombo->ResetContent();

	m_tagComboIDList.RemoveAll();
	m_db.GetTagList(names);

	for(int i = 0; i < (int) names.GetSize(); i++)
	{
		n = m_tagCombo->AddString(names[i].name);
		m_tagCombo->SetCheck(n, FALSE);

		n = m_itemTagCombo.AddString(names[i].name);
		m_itemTagCombo.SetCheck(n, FALSE);

		m_tagComboIDList.InsertAt(n, names[i].id);
	}

	SetTagList(*m_tagCombo, m_db.GetCurTag());
	SetTagList(m_itemTagCombo, m_db.GetCurItemTag());
}

void CCardView::CheckBookmarkChange()
{
	UpdateData();
	if (m_itemBookmark != m_db.GetCurItemBookmark())
		m_itemBookmarkChange = TRUE;
	else
		m_itemBookmarkChange = FALSE;
}

void CCardView::SetTagList(CCheckComboBox &combo, CUIntArray &list)
{
	CMap<int, int, int, int>	hash;
	int							v;

	if (combo.GetCount() == 0)
		return;

	for(int i = 0; i < list.GetCount(); i++)
		hash[list[i]] = i;

	for(int i = 0; i < combo.GetCount(); i++)
		if (hash.Lookup(m_tagComboIDList[i], v))
			combo.SetCheck(i, TRUE);
		else
			combo.SetCheck(i, FALSE);

}

static int compare( const void *arg1, const void *arg2 )
{
   /* Compare all of both strings: */
   return *((int *) arg1) - *((int *) arg2);
}

void CCardView::GetTagList(CCheckComboBox &combo, CUIntArray &res)
{
	res.RemoveAll();
	for(int i = 0; i < combo.GetCount(); i++)
	{
		if (combo.GetCheck(i))
			res.Add(m_tagComboIDList[i]);
	}

	qsort(res.GetData(), res.GetCount(), sizeof(int), compare);
}

BOOL CCardView::CompareTagList(CCheckComboBox &wnd, CUIntArray &tags)
{
	CUIntArray		itemTag;

	GetTagList(wnd, itemTag);
	if (itemTag.GetCount() != tags.GetCount())
	{
		return FALSE;
	}

	for(int i = 0; i < itemTag.GetCount(); i++)
	{
		if (itemTag[i] != tags[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}

BOOL CCardView::CompareTagList(CUIntArray &tag1, CUIntArray &tag2)
{
	if (tag1.GetCount() != tag2.GetCount())
	{
		return FALSE;
	}

	for(int i = 0; i < tag1.GetCount(); i++)
	{
		if (tag1[i] != tag2[i])
		{
			return FALSE;
		}
	}

	return TRUE;
}

int CCardView::FindItemData(CComboBox &wnd, int value)
{
	for(int i = 0; i < wnd.GetCount(); i++)
	{
		if (value == wnd.GetItemData(i))
			return i;
	}

	return -1;
}


static DWORD CALLBACK StreamIn( DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb )
{
	CString* str = ( ( CString* ) dwCookie );

	int max = min( cb, (*str).GetLength() );
	strncpy( ( LPSTR ) pbBuff, (*str) , max  );
	(*str) = (*str).Right( (*str).GetLength() - max );
	*pcb = max;
	
	return 0;
}

void CCardView::GetInfo(CString &s)
{
	int		i;
	CString	section, cate;

	i = m_section->GetCurSel();
	if (i >= 0)
		m_section->GetLBText(i, section);

	i = m_category->GetCurSel();
	if (i >= 0)
		m_category->GetLBText(i, cate);

	if (m_db.GetItemCount() == 0)
	{
		s.Format("0 / 0     %s : %s", section, cate);
	}
	else
		s.Format("%d / %d    %s : %s", 
			m_db.GetItemIndex() + 1, 
			m_db.GetItemCount(), 
			section, 
			cate);

	return;
}

void CCardView::CreateEditors()
{
	/* 
		Too many fonts are not needed.  
		To use all font, modify m_font.FillCombo2 to m_font.FillCombo in
		RRECToolbar.cpp. Line 186.

		To add more font to font dropbox, Add AddFont(..) to FontComboBox.cpp
		of FillCombo2 function. Line 159
	*/
	CRect	rect, rect2;
	CString	font;
	
	m_keyPlacement.GetWindowRect(rect);
	m_keyPlacement.ShowWindow(SW_HIDE);
	ScreenToClient(rect);
	m_valuePlacement.GetWindowRect(rect2);
	m_valuePlacement.ShowWindow(SW_HIDE);
	ScreenToClient(rect2);

	rect.top++;
	//m_dictMode = TRUE;
	if (m_dictMode)
	{
		rect.bottom = rect.top + DICT_KEY_HEIGHT;
		m_key.CreateEdit(ES_AUTOHSCROLL | WS_BORDER | WS_TABSTOP | 
			WS_CHILD | WS_VISIBLE, rect, this, 10100, 
			 "Arial", 10, FALSE);
	}
	else
	{
		m_key.CreateRichEdit(WS_TABSTOP | WS_CHILD | WS_VISIBLE, 
			rect, this, 10100, 
			"Courier New", 10, TRUE, TRUE );
		m_key.ShowToolbar(!m_readOnlyMode);
	}
	m_key.ShowRuler(false);

	rect2.top = rect.bottom + 10;
	m_content.CreateRichEdit(WS_TABSTOP | WS_CHILD | WS_VISIBLE, rect2, this, 10101, 
		font, 10, TRUE, TRUE );
	m_content.ShowRuler(false);
	m_content.ShowToolbar(!m_readOnlyMode);

	BOOL bOk;
	m_resizer.Unhook();
	m_resizer.Hook(this);

	if (m_dictMode)
	{
		bOk = m_resizer.SetAnchor(10101, ANCHOR_ALL);
		ASSERT( bOk == TRUE);
		bOk = m_resizer.SetAnchor(10100, ANCHOR_HORIZONTALLY);
	}
	else
	{
		bOk = m_resizer.CreateSplitContainer(_T("OutterSplit"), 10100, 10101 );
		ASSERT( bOk == TRUE);
		CSize size(100, 60); // cy member will be ignored
		bOk = m_resizer.SetMinimumSize(10100, size);

		bOk = m_resizer.SetMinimumSize(10101, size);
		ASSERT( bOk == TRUE);

		bOk = m_resizer.SetShowSplitterGrip(_T("OutterSplit"), TRUE );
		ASSERT( bOk == TRUE);
		bOk = m_resizer.SetAnchor(_T("OutterSplit"), ANCHOR_ALL);
		bOk = m_resizer.InvokeOnResized();
	}
	bOk = m_resizer.SetAnchor(IDC_STATIC_PLACEMENT3, ANCHOR_ALL);
	bOk = m_resizer.SetAnchor(IDC_STATIC_PLACEMENT2, ANCHOR_ALL);
	bOk = m_resizer.InvokeOnResized();
	
	CRect	r;
	AfxGetMainWnd()->GetWindowRect(&r);
	AfxGetMainWnd()->SetWindowPos(&CWnd::wndNoTopMost, 0, 0, r.Width() - 1, r.Height() - 1, SWP_NOMOVE | SWP_FRAMECHANGED);
	m_setSplitterPos = TRUE;
}

// if text consists of only "|", set it to ""
static bool IsEmptyText(CString &s)
{
	for(int i = 0; i < s.GetLength(); i++)
	{
		if (s[i] != '|' && s[i] != ' ')
			return false;
	}

	return true;
}

int CCardView::SaveAsFc()
{
	CFile				file;
	CString				s;
	CFcListener			listener;
	CRtfParser			parser;
	ConvertStringsList	sl;
	ConvertStrings		cs;
	int					imageCount;
	int					maxImageCount = 0;
	int					i, j, k;
	CString				key, content;
	CString				t;
	CppSQLite3Query		q;
	int					total;
	RtfImageList	    imageList;
	int					id = 0;
	CString				listenerId;
	BOOL				keySizeTag;
	BOOL				contentSizeTag;
	BOOL				addImage2Fc;

	CFcListener::ClearFolder();
	if (m_db.GetItemCount() == 0)
		return 0;

	m_db.GetDbEnv(m_db.GetCurDb(), KEY_SIZE_TAG, keySizeTag);
	m_db.GetDbEnv(m_db.GetCurDb(), CONTENT_SIZE_TAG, contentSizeTag);
	m_db.GetDbEnv(m_db.GetCurDb(), ADD_IMAGE_TO_FC_TAG, addImage2Fc);

	total = m_db.LoadDataFromCurrentSetting(q, -1, 0);

    CString str;

	while(!q.eof())
	{
		imageCount = 0;
		cs.images.clear();

		if (m_dictMode)
		{
			cs.text = q.getStringField("key");
			cs.text +=  "\t";
		}
		else
		{
			id = q.getIntField("id");

			m_db.LoadImage(id, 0, imageList);

            if (q.getIntField("keyCompressed") == 0)
            {
                str = q.getStringField("key");
            }
            else
            {
                int     len = 0;
                auto    p = q.getBlobField("key", len);

                m_db.Decompress(p, len, str);
            }

			if (parser.OpenFromString(str, &imageList) == -1)
			{
				MessageBox("Fail to parse", "Error");
				return -1;
			}

			listener.EnableSizeTag(keySizeTag);
			listenerId.Format("%d_0", id);
			if (parser.Parse(&listener, listenerId) == -1)
			{
				MessageBox("Fail to parse", "Error");
				return -1;
			}

			if (addImage2Fc)
			{
				if (listener.m_images.GetCount() > 0)
				{
					cs.images.push_back((LPCTSTR) listener.m_images[0]);
					imageCount++;
				}
				else
					cs.images.push_back("");
			}
			if (IsEmptyText(listener.m_text))
				cs.text = "\t";
			else
				cs.text = listener.m_text + "\t";
			ResetImageList(imageList);
		}

		m_db.LoadImage(id, 1, imageList);


        if (q.getIntField("contentCompressed") == 0)
        {
            str = q.getStringField("content");
        }
        else
        {
            int     len = 0;
            auto    p = q.getBlobField("content", len);

            m_db.Decompress(p, len, str);
        }

		if (parser.OpenFromString(str, &imageList) == -1)
		{
			MessageBox("Fail to parse", "Error");
			return -1;
		}

		listener.EnableSizeTag(contentSizeTag);
		listenerId.Format("%d_1", id);
		if (parser.Parse(&listener, listenerId) == -1)
		{
			MessageBox("Fail to parse", "Error");
			return -1;
		}

		if (!IsEmptyText(listener.m_text))
			cs.text += listener.m_text;
		else
			cs.text += " ";

		if (addImage2Fc)
		{
			int c;

			if (listener.m_images.GetCount() + cs.images.size() > 5)
				c = 5 - cs.images.size();
			else
				c = listener.m_images.GetCount();

			// even though key doesn't have image, if content has image, it regards count = 1
			if (imageCount == 0)
				imageCount = 1;

			for(int k = 0; k < c; k++)
			{
				imageCount++;
				cs.images.push_back((LPCTSTR) listener.m_images[k]);
			}

			if (imageCount > maxImageCount)
				maxImageCount = imageCount;
		}
		ResetImageList(imageList);

		sl.push_back(cs);
		q.nextRow();
	}

	CString name = m_db.GetCurDbName();
	CString cate;

	i = m_category->GetCurSel();
	if (i > 1)
	{
		m_category->GetLBText(i, cate);
		name += "_";
		name += cate;
	}
	else
		name = m_db.GetCurDbName();

	if (file.Open(name + ".fc", CFile::modeCreate | CFile::modeReadWrite) == false)
	{
		return -1;
	}

	s = "Text 1\tText 2";
	for(int i = 0; i < maxImageCount; i++)
	{
		t.Format("\tPicture %d", i + 1);
		s += t;
	}
	s += "\n";

	ConvUtf8(s, s);
	file.Write(s, s.GetLength());

	for(i = 0; i < (int) sl.size(); i++)
	{
		s = sl[i].text.c_str();
		for(j = 0; j < (int) sl[i].images.size(); j++)
		{
			if (sl[i].images[j] != "")
			{
				CString temp = sl[i].images[j].c_str();
				temp.Replace("\\", "/");
				t.Format("\t%s%s", webserver::GetInternalAddr(), temp);
			}
			else
				t = "\t";
			s += t;
		}
		for(k = j; k < maxImageCount; k++)
		{
			s += "\t";
		}
		s += "\n";

		ConvUtf8WithReplace(s, BULLET_TEMP, BULLET, s); 

		file.Write(s, s.GetLength());
	}

	file.Close();

	s.Format("%s%s.fc\r\n%s%s.fc", webserver::GetExternalAddr(), name,
		webserver::GetInternalAddr(), name);

	MessageBox("Type " + s, "Info");
	return 0;
}


LRESULT  CCardView::OnFileNotification(WPARAM wParam, LPARAM lParam)
{
	CString *	name = (CString *) lParam;

	if (name == NULL)
		return 0;

	TRACE("Notify = %s\n", *name);

    NotifySet::iterator     it;

    it = m_notifySet.find((LPCTSTR) *name);
    if (it != m_notifySet.end())
    {
	    delete name;
        return 0;
    }
    m_notifyMap[m_currentTimer] = (LPCTSTR) *name;
    m_notifySet[(LPCTSTR) *name] = 1;
    SetTimer(m_currentTimer, 1000, 0);

    ++m_currentTimer;
    if (m_currentTimer > 5000)
        m_currentTimer = 1000;

	delete name;
	return 0;
}


void CCardView::SetHistory()
{
	if (!m_prevView.empty() && 
		m_prevView.front().db == m_db.GetCurDbName() &&
			m_prevView.front().id == m_db.GetCurId())
		return;

	ViewInfo	vi;
	vi.db = m_db.GetCurDbName();
	vi.id = m_db.GetCurId();
	m_prevView.push_front(vi);
    if (m_prevView.size() > MAX_HISTORY)
        m_prevView.pop_back();

    m_nextView.clear();
}

void CCardView::DbNotify(int msg)
{
    switch(msg)
    {
    case DB_TAG_CHANGE:
	    PostMessage(WM_COMMAND, IDD_DB_CHANGE);
        break;
    }
}
