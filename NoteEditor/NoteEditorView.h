
// NoteEditorView.h : interface of the CNoteEditorView class
//

#pragma once

#include "ComponentBase.h"
#include <dcomp.h>
#include <functional>
#include <memory>
#include <ole2.h>
#include <vector>

#ifdef USE_WEBVIEW2_WIN10
#include <winrt/Windows.UI.Composition.h>
#include <winrt/Windows.UI.ViewManagement.h>
namespace winrtComp = winrt::Windows::UI::Composition;
#endif

class CNoteEditorDoc;

class CNoteEditorView : public CFormView
{
protected: // create from serialization only
	CNoteEditorView() noexcept;
	DECLARE_DYNCREATE(CNoteEditorView)

public:
#ifdef AFX_DESIGN_TIME
	enum{ IDD = IDD_NOTEEDITOR_FORM };
#endif

// Attributes
public:
	CNoteEditorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct

// Implementation
public:
	virtual ~CNoteEditorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	void InitializeWebView();
	void CloseWebView(bool cleanupUserDataFolder = false);
	HRESULT OnCreateEnvironmentCompleted(HRESULT result, ICoreWebView2Environment* environment);
	HRESULT OnCreateCoreWebView2ControllerCompleted(HRESULT result, ICoreWebView2Controller* controller);
	HRESULT DCompositionCreateDevice2(IUnknown* renderingDevice, REFIID riid, void** ppv);

	ICoreWebView2Controller* GetWebViewController()
	{
		return m_controller.get();
	}
	ICoreWebView2* GetWebView()
	{
		return m_webView.get();
	}
	ICoreWebView2Environment* GetWebViewEnvironment()
	{
		return m_webViewEnvironment.get();
	}
	HWND GetMainWindow()
	{
		return this->GetSafeHwnd();
	}
	void ResizeEverything();

protected:
	template <class ComponentType, class... Args> void NewComponent(Args&&... args);
	template <class ComponentType> ComponentType* GetComponent();

protected:
	wil::com_ptr<ICoreWebView2Environment> m_webViewEnvironment;
	wil::com_ptr<ICoreWebView2Controller> m_controller;
	wil::com_ptr<ICoreWebView2> m_webView;
	wil::com_ptr<IDCompositionDevice> m_dcompDevice;
	std::vector<std::unique_ptr<ComponentBase>> m_components;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSettingManageSection();
	afx_msg void OnSettingManageProperty();
	afx_msg void OnSettingManageTag();
};

#ifndef _DEBUG  // debug version in NoteEditorView.cpp
inline CNoteEditorDoc* CNoteEditorView::GetDocument() const
   { return reinterpret_cast<CNoteEditorDoc*>(m_pDocument); }
#endif

template <class ComponentType, class... Args> void CNoteEditorView::NewComponent(Args&&... args)
{
	m_components.emplace_back(new ComponentType(std::forward<Args>(args)...));
}

template <class ComponentType> ComponentType* CNoteEditorView::GetComponent()
{
	for (auto& component : m_components)
	{
		if (auto wanted = dynamic_cast<ComponentType*>(component.get()))
		{
			return wanted;
		}
	}
	return nullptr;
}
