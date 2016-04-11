#include "stdafx.h"
#include "WkeWebkit.h"
//#pragma comment(lib, "wke.lib")



std::string Unicode2ANSI(LPCWSTR lpszSrc)
{
	std::string sResult;
	if (lpszSrc != NULL)
	{
		int  nANSILen = WideCharToMultiByte(CP_ACP, 0, lpszSrc, -1, NULL, 0, NULL, NULL);
		char* pANSI = new char[nANSILen + 1];
		if (pANSI != NULL)
		{
			ZeroMemory(pANSI, nANSILen + 1);
			WideCharToMultiByte(CP_ACP, 0, lpszSrc, -1, pANSI, nANSILen, NULL, NULL);
			sResult = pANSI;
			delete[] pANSI;
		}
	}	
	return sResult;
} 

std::wstring ANSI2Unicode(LPCSTR lpszSrc)
{
	std::wstring sResult;
	if (lpszSrc != NULL)
	{
		int nUnicodeLen = MultiByteToWideChar(CP_ACP, 0, lpszSrc, -1, NULL, 0);
		LPWSTR pUnicode = new WCHAR[nUnicodeLen + 1];
		if (pUnicode != NULL)
		{
			ZeroMemory((void*)pUnicode, (nUnicodeLen + 1) * sizeof(WCHAR));
			MultiByteToWideChar(CP_ACP, 0, lpszSrc,-1, pUnicode, nUnicodeLen);
			sResult = pUnicode;
			delete[] pUnicode;
		}
	}
	return sResult;
}


_tstring ANSI2T(LPCSTR lpSrc)
{
#ifdef _UNICODE
	return ANSI2Unicode(lpSrc);
#else
	return lpSrc;
#endif
}
std::string T2ANSI(LPCTSTR lpSrc)
{
#ifdef _UNICODE
	return Unicode2ANSI(lpSrc);
#else
	return lpSrc;
#endif
}

void CWkeWebkitUI::InitializeWebkit()
{
	//wkeInit();
	wkeInitialize();
}
void CWkeWebkitUI::UninitializeWebkit()
{
	//wkeShutdown();
	wkeFinalize();
}

void CWkeWebkitUI::MapNativeFunction(LPCTSTR lpName, jsNativeFunction fn, UINT args)
{
	jsBindFunction(T2ANSI(lpName).c_str(), fn, args);
}

void _onURLChanged(wkeWebView webView, void* param, const wkeString  URL)
{
	CWkeWebkitUI* pWWK = (CWkeWebkitUI*)param;
#ifdef _UNICODE
	//pWWK->m_pManager->SendNotify(pWWK, DUI_MSGTYPE_URLCHANGED, WPARAM(wkeToStringW(URL)));
	pWWK->GetManager()->SendNotify(pWWK, DUI_MSGTYPE_URLCHANGED, WPARAM(wkeGetStringW(URL)));
#else
	pWWK->m_pManager->SendNotify(pWWK, DUI_MSGTYPE_URLCHANGED, WPARAM(wkeToString(URL)));
#endif
}

void _onTitleChanged(wkeWebView webView, void* param, const wkeString title)
{
 	CWkeWebkitUI* pWWK = (CWkeWebkitUI*)param;

#ifdef _UNICODE
	pWWK->GetManager()->SendNotify(pWWK, DUI_MSGTYPE_TITLECHANGED, WPARAM(wkeToStringW(title)));
	//pWWK->m_pManager->SendNotify(pWWK, DUI_MSGTYPE_TITLECHANGED, WPARAM(wkeGetStringW(title)));
#else
	param->SendNotify(pWWK, DUI_MSGTYPE_TITLECHANGED, WPARAM(wkeToString(title)));
#endif
}

CWkeWebkitUI::CWkeWebkitUI(void):
m_pWebView(wkeCreateWebView())
{
	initConfig(m_pWebView);
}
void CWkeWebkitUI::initConfig(wkeWebView  _wkeWebView)
{
	m_pWebView->onURLChanged(_onURLChanged, this);
	m_pWebView->onTitleChanged(_onTitleChanged, this);

	m_pWebView->setUserAgent("Mozilla/5.0 (Windows NT 6.1; WOW64) AppleWebKit/537.36 (KHTML, like Gecko) MY_TEST_USER_AGENT");
	const char *testRefer=m_pWebView->getReferer();
	//m_pWebView->setReferer(L"it's my test referer");

}
CWkeWebkitUI::~CWkeWebkitUI(void)
{	
	m_pManager->KillTimer(this, EVENT_TICK_TIEMER_ID);
	m_pWebView->destroy();
}




LPCTSTR CWkeWebkitUI::GetClass() const
{
	return _T("WkeWebkitUI");
}

LPVOID CWkeWebkitUI::GetInterface(LPCTSTR pstrName)
{
	if( _tcscmp(pstrName, _T("WkeWebkit")) == 0 ) return static_cast<CWkeWebkitUI*>(this);
	return CControlUI::GetInterface(pstrName);
}

void CWkeWebkitUI::SetPos(RECT rc, bool bNeedInvalidate)
{
	CControlUI::SetPos(rc, bNeedInvalidate);
	m_pWebView->resize(rc.right - rc.left, rc.bottom - rc.top);
}

void CWkeWebkitUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
{
	CControlUI::DoPaint(hDC, rcPaint, pStopControl);
	


	m_pWebView->paint(hDC, rcPaint.left, rcPaint.top, rcPaint.right - rcPaint.left, rcPaint.bottom - rcPaint.top, 0, 0, true);
}
void CWkeWebkitUI::DoInit()
{
	CControlUI::DoInit();
	m_pWebView->setName(T2ANSI(GetName()).c_str());
	m_pManager->SetTimer(this, EVENT_TICK_TIEMER_ID, 30);

	
}

void CWkeWebkitUI::DoEvent( DuiLib::TEventUI& event )
{
	RECT rc = GetPos();
	POINT pt = {event.ptMouse.x - rc.left, event.ptMouse.y - rc.top};

	switch (event.Type)
	{
	case DuiLib::UIEVENT_MOUSEENTER:
	case DuiLib::UIEVENT_MOUSELEAVE:
	case DuiLib::UIEVENT_MOUSEMOVE:
		m_pWebView->mouseEvent(WKE_MSG_MOUSEMOVE, pt.x, pt.y, event.wKeyState);
		break;
	case DuiLib::UIEVENT_BUTTONDOWN:
		m_pWebView->mouseEvent(WKE_MSG_LBUTTONDOWN, pt.x, pt.y, event.wKeyState);
		break;
	case DuiLib::UIEVENT_BUTTONUP:
		m_pWebView->mouseEvent(WKE_MSG_LBUTTONUP, pt.x, pt.y, event.wKeyState);
		break;
	case DuiLib::UIEVENT_RBUTTONDOWN:
		m_pWebView->mouseEvent(WKE_MSG_RBUTTONDOWN, pt.x, pt.y, event.wKeyState);
		break;
	case DuiLib::UIEVENT_DBLCLICK:
		m_pWebView->mouseEvent(WKE_MSG_LBUTTONDBLCLK, pt.x, pt.y, event.wKeyState);
		break;
	case DuiLib::UIEVENT_KILLFOCUS:
		m_pWebView->killFocus();
		break;
	case DuiLib::UIEVENT_SETFOCUS:
		m_pWebView->setFocus();
		break;
	case DuiLib::UIEVENT_CONTEXTMENU:
		m_pWebView->contextMenuEvent(pt.x, pt.y, event.wKeyState);
		break;
	case DuiLib::UIEVENT_KEYUP:
		m_pWebView->keyUp(event.chKey, event.lParam, false);
		break;
	case DuiLib::UIEVENT_KEYDOWN:
		m_pWebView->keyDown(event.chKey, event.lParam, false);
		break;
	case DuiLib::UIEVENT_CHAR:
		m_pWebView->keyPress(event.chKey, event.lParam, false);
		break;
	case DuiLib::UIEVENT_SCROLLWHEEL:
		m_pWebView->mouseWheel(pt.x, pt.y, event.wParam == SB_LINEUP ? 120 : -120, event.wKeyState);
		break;
	case DuiLib::UIEVENT_TIMER:
		if(event.wParam == EVENT_TICK_TIEMER_ID)
		{
			m_pWebView->tick();
			Invalidate();
		}
		break;
	default:
		break;
	}
	CControlUI::DoEvent(event);
}

void CWkeWebkitUI::Navigate(LPCTSTR lpUrl)
{
	m_pWebView->loadURL(lpUrl);
}

void CWkeWebkitUI::Close()
{
	m_pWebView->destroy();
}

void CWkeWebkitUI::ShowFile(LPCTSTR lpFile)
{
	m_pWebView->loadFile(lpFile);
}
void CWkeWebkitUI::ShowHtml(LPCTSTR lpHtml)
{
	m_pWebView->loadHTML(lpHtml);
}

_tstring CWkeWebkitUI::GetTitle()
{
#ifdef _UNICODE
	return m_pWebView->titleW();
#else
	return m_pWebView->title();
#endif
}
_tstring CWkeWebkitUI::GetWebkitName() const
{
	return ANSI2T(m_pWebView->name());
}
void CWkeWebkitUI::SetWebKitName(LPCTSTR lpName)
{
	m_pWebView->setName(T2ANSI(lpName).c_str());
}

void CWkeWebkitUI::Stop()
{
	m_pWebView->stopLoading();
}
void CWkeWebkitUI::Reload()
{
	m_pWebView->reload();
}
BOOL CWkeWebkitUI::IsLoadSucessed() const
{
	return m_pWebView->isLoadingSucceeded();
}
BOOL CWkeWebkitUI::IsDocumentReady() const
{
	return m_pWebView->isDocumentReady();
}
BOOL CWkeWebkitUI::IsDocumentLoadCompleted() const
{
	return m_pWebView->isLoadingCompleted();
}

int CWkeWebkitUI::GetContentsWidth() const
{
	//return m_pWebView->contentsWidth();
	return 0;
}
int CWkeWebkitUI::GetContentsHeight() const
{
	//return m_pWebView->contentsHeight();
	return 0;
}
const char* CWkeWebkitUI::getReferer()
{

	return m_pWebView->getReferer();
}
void CWkeWebkitUI::setReferer(LPCTSTR lpUrl)
{
	m_pWebView->setReferer(lpUrl);

}
void CWkeWebkitUI::setUserAgent(LPCTSTR useragent)
{
	m_pWebView->setUserAgent(useragent);


}
void CWkeWebkitUI::simuHit(LPCTSTR ID)
{

	m_pWebView->simulatedHitTestElement(ID);

}

BOOL CWkeWebkitUI::Back()
{
	return m_pWebView->canGoBack() && m_pWebView->goBack();
}
BOOL CWkeWebkitUI::Forward()
{
	return m_pWebView->canGoForward() && m_pWebView->goForward();
}

void CWkeWebkitUI::Cut()
{
	//m_pWebView->cut();
}
void CWkeWebkitUI::Copy()
{
	//m_pWebView->copy();
}
void CWkeWebkitUI::Paste()
{
	//m_pWebView->paste();
}
void CWkeWebkitUI::Delete()
{
	//m_pWebView->delete_();
}
void CWkeWebkitUI::SelectAll()
{
	//m_pWebView->selectAll();
}

float CWkeWebkitUI::GetScale() const
{
	return m_pWebView->zoomFactor();
}
void CWkeWebkitUI::Zoom(float fScale)
{
	m_pWebView->setZoomFactor(fScale);
}

BOOL CWkeWebkitUI::IsCookieEnabled() const
{
	return m_pWebView->cookieEnabled();
}
void CWkeWebkitUI::EnableCookie(BOOL bEnable)
{
	m_pWebView->setCookieEnabled(bEnable ? true : false);
}

float CWkeWebkitUI::GetMediaVolume() const
{
	return m_pWebView->mediaVolume();
}
void CWkeWebkitUI::SetMediaVolume(float fVol)
{
	m_pWebView->setMediaVolume(fVol);
}

// __int64 CWkeWebkitUI::ExecJavaScript(LPCTSTR lpScript)
// {
// 	return m_pWebView->runJS(lpScript);
// }
_tstring CWkeWebkitUI::ExecJavaScript(LPCTSTR lpScript)
{
#ifdef _UNICODE
	return jsToTempStringW(m_pWebView->globalExec(), m_pWebView->runJS(lpScript));
#else
	return jsToString(m_pWebView->globalExec(), m_pWebView->runJS(lpScript));
#endif	
}