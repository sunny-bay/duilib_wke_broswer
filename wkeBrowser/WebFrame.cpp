#include "StdAfx.h"
#include "WebFrame.h"

CWebFrame::CWebFrame() :
m_pWkeWebkit(NULL)
{
}

DuiLib::CDuiString CWebFrame::GetSkinFolder()
{
	return _T ("../../../../duilib_wke/skin");
};
DuiLib::CDuiString CWebFrame::GetSkinFile()
{
	return _T ("skin.xml");
};

LPCTSTR CWebFrame::GetWindowClassName(void) const
{
	return _T ("WebFrameWnd");
};

void CWebFrame::InitWindow()
{
	//任务栏标题
	//SetWindowText(m_hWnd, m_PaintManager.FindControl(_T("labelTitle"))->GetText());

 	m_pWkeWebkit = static_cast<CWkeWebkitUI*>(m_PaintManager.FindControl(_T("wkeBrowser")));
	if (m_pWkeWebkit != NULL)
	{
		//m_pWkeWebkit->Navigate(m_PaintManager.FindControl(_T("editUrl"))->GetText
		m_pWkeWebkit->Navigate(L"www.baidu.com");
	}
	
}


DuiLib::CControlUI* CWebFrame::CreateControl( LPCTSTR pstrClassName )
{
	if (_tcsicmp(pstrClassName, _T("WkeWebkit")) == 0) return  new CWkeWebkitUI;
	return WindowImplBase::CreateControl(pstrClassName);
}

void CWebFrame::OnClose()
{
	WindowImplBase::OnClose();
	PostQuitMessage(0);//关闭时结束
}

void CWebFrame::OnUrlChanged(DuiLib::TNotifyUI& msg)
{
	m_PaintManager.FindControl(_T("labelUrl"))->SetText(LPCTSTR(msg.wParam));
	m_PaintManager.FindControl(_T("editUrl"))->SetText(LPCTSTR(msg.wParam));
}
void CWebFrame::OnTitleChanged(DuiLib::TNotifyUI& msg)
{
	m_PaintManager.FindControl(_T("labelTitle"))->SetText(LPCTSTR(msg.wParam));
}
void CWebFrame::OnReturn(DuiLib::TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("editUrl"))
	{
		m_pWkeWebkit->Navigate(msg.pSender->GetText());
	}	
}

void CWebFrame::OnClick(DuiLib::TNotifyUI& msg)
{
	if (msg.pSender->GetName() == _T("btnGetReferer"))
	{
		const char * refer=m_pWkeWebkit->getReferer();
		::MessageBoxA(0, refer, "Referer", MB_OK);
	}	
	else 	if (msg.pSender->GetName() == _T("btnSetReferer"))
	{
		m_pWkeWebkit->setReferer(L"it's my test referer");

	}	
	else 	if (msg.pSender->GetName() == _T("btnSetUserAgent"))
	{
		m_pWkeWebkit->setUserAgent(L"it's my test userAgent");
	}	
	else 	if (msg.pSender->GetName() == _T("btnsimuHit"))
	{
		m_pWkeWebkit->simuHit(L"test2");
	}	
	WindowImplBase::OnClick(msg);
}