
// NoteEditorView.cpp : implementation of the CNoteEditorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "NoteEditor.h"
#endif
#include "MainFrm.h"

#include "ViewComponent.h"
#include "NoteEditorDoc.h"
#include "NoteEditorView.h"
#include "Property.h"
#include "Section.h"
#include "Tag.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CNoteEditorView

IMPLEMENT_DYNCREATE(CNoteEditorView, CFormView)

BEGIN_MESSAGE_MAP(CNoteEditorView, CFormView)
    ON_WM_SIZE()
    ON_COMMAND(ID_SETTING_MANAGESECTION, &CNoteEditorView::OnSettingManageSection)
    ON_COMMAND(ID_SETTING_MANAGE_PROPERTY, &CNoteEditorView::OnSettingManageProperty)
    ON_COMMAND(ID_SETTING_MANAGE_TAG, &CNoteEditorView::OnSettingManageTag)
END_MESSAGE_MAP()

// CNoteEditorView construction/destruction

CNoteEditorView::CNoteEditorView() noexcept
    : CFormView(IDD_NOTEEDITOR_FORM)
{
    // TODO: add construction code here

}

CNoteEditorView::~CNoteEditorView()
{
}

void CNoteEditorView::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
}

BOOL CNoteEditorView::PreCreateWindow(CREATESTRUCT& cs)
{
    // TODO: Modify the Window class or styles here by modifying
    //  the CREATESTRUCT cs

    return CFormView::PreCreateWindow(cs);
}

void CNoteEditorView::OnInitialUpdate()
{
    CFormView::OnInitialUpdate();
    GetParentFrame()->RecalcLayout();
    ResizeParentToFit();

    InitializeWebView();

    CMainFrame* frame = (CMainFrame*)AfxGetMainWnd();
    frame->GetDialogBar().Init();
}


// CNoteEditorView diagnostics

#ifdef _DEBUG
void CNoteEditorView::AssertValid() const
{
    CFormView::AssertValid();
}

void CNoteEditorView::Dump(CDumpContext& dc) const
{
    CFormView::Dump(dc);
}

CNoteEditorDoc* CNoteEditorView::GetDocument() const // non-debug version is inline
{
    ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CNoteEditorDoc)));
    return (CNoteEditorDoc*)m_pDocument;
}
#endif //_DEBUG


// CNoteEditorView message handlers


void CNoteEditorView::OnSize(UINT nType, int cx, int cy)
{
    CFormView::OnSize(nType, cx, cy);

    ResizeEverything();
}

void CNoteEditorView::ResizeEverything()
{
    RECT availableBounds = { 0 };
    GetClientRect(&availableBounds);
    // ClientToScreen(&availableBounds);

    if (auto view = GetComponent<ViewComponent>())
    {
        view->SetBounds(availableBounds);
    }
}

void CNoteEditorView::InitializeWebView()
{

    CloseWebView();
    m_dcompDevice = nullptr;


    HRESULT hr2 = DCompositionCreateDevice2(nullptr, IID_PPV_ARGS(&m_dcompDevice));
    if (!SUCCEEDED(hr2))
    {
        AfxMessageBox("Attempting to create WebView using DComp Visual is not supported.\r\n"
            "DComp device creation failed.\r\n"
            "Current OS may not support DComp.\r\n"
            "Create with Windowless DComp Visual Failed", MB_OK);
        return;
    }


#ifdef USE_WEBVIEW2_WIN10
    m_wincompCompositor = nullptr;
#endif
    LPCWSTR subFolder = nullptr;
    auto options = Microsoft::WRL::Make<CoreWebView2EnvironmentOptions>();
    options->put_AllowSingleSignOnUsingOSPrimaryAccount(FALSE);


    HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, nullptr, options.Get(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CNoteEditorView::OnCreateEnvironmentCompleted).Get());
    //HRESULT hr = CreateCoreWebView2EnvironmentWithOptions(subFolder, nullptr, options.Get(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2EnvironmentCompletedHandler>(this, &CEdgeBrowserAppDlg::OnCreateEnvironmentCompleted).Get());

    if (!SUCCEEDED(hr))
    {
        if (hr == HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND))
        {
            TRACE("Couldn't find Edge installation. Do you have a version installed that is compatible with this ");
        }
        else
        {
            AfxMessageBox("Failed to create webview environment");
        }
    }
}

HRESULT CNoteEditorView::DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv)
{
    HRESULT hr = E_FAIL;
    static decltype(::DCompositionCreateDevice2)* fnCreateDCompDevice2 = nullptr;
    if (fnCreateDCompDevice2 == nullptr)
    {
        HMODULE hmod = ::LoadLibraryEx("dcomp.dll", nullptr, 0);
        if (hmod != nullptr)
        {
            fnCreateDCompDevice2 = reinterpret_cast<decltype(::DCompositionCreateDevice2)*>(
                ::GetProcAddress(hmod, "DCompositionCreateDevice2"));
        }
    }
    if (fnCreateDCompDevice2 != nullptr)
    {
        hr = fnCreateDCompDevice2(renderingDevice, riid, ppv);
    }
    return hr;
}

void CNoteEditorView::CloseWebView(bool cleanupUserDataFolder)
{

    if (m_controller)
    {
        m_controller->Close();
        m_controller = nullptr;
        m_webView = nullptr;
    }
    m_webViewEnvironment = nullptr;
    if (cleanupUserDataFolder)
    {
        //Clean user data        
    }
}

HRESULT CNoteEditorView::OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment)
{
    m_webViewEnvironment = environment;
    m_webViewEnvironment->CreateCoreWebView2Controller(this->GetSafeHwnd(), Microsoft::WRL::Callback<ICoreWebView2CreateCoreWebView2ControllerCompletedHandler>(this, &CNoteEditorView::OnCreateCoreWebView2ControllerCompleted).Get());

    return S_OK;
}

HRESULT CNoteEditorView::OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller)
{
    if (result == S_OK)
    {
        m_controller = controller;
        wil::com_ptr<ICoreWebView2> coreWebView2;
        m_controller->get_CoreWebView2(&coreWebView2);
        coreWebView2.query_to(&m_webView);

        NewComponent<ViewComponent>(
            this, m_dcompDevice.get(),
#ifdef USE_WEBVIEW2_WIN10
            m_wincompCompositor,
#endif
            FALSE);

        HRESULT hresult = m_webView->Navigate(L"http://localhost:5000/editor/SmartEditor2.html");

        if (hresult == S_OK)
        {
            TRACE("Web Page Opened Successfully");
            ResizeEverything();
        }

    }
    else
    {
        TRACE("Failed to create webview");
    }

    return S_OK;
}


void CNoteEditorView::OnSettingManageSection()
{
    Section section;

    section.DoModal();
}


void CNoteEditorView::OnSettingManageProperty()
{
    Property property;

    property.DoModal();
}


void CNoteEditorView::OnSettingManageTag()
{
    Tag tag;

    tag.DoModal();
}
