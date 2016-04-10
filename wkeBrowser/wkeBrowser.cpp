// wkeBrowser.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "wkeBrowser.h"
#include "WebFrame.h"
int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	CWkeWebkitUI::InitializeWebkit();
	DuiLib::CPaintManagerUI::SetInstance(hInstance);
	CWebFrame WebFrame;

	if (WebFrame.Create(NULL, NULL, UI_WNDSTYLE_FRAME, WS_EX_ACCEPTFILES) != NULL)
	{
		WebFrame.SetIcon(IDI_WKEBROWSER);
		WebFrame.CenterWindow();
		WebFrame.ShowWindow();
		DuiLib::CPaintManagerUI::MessageLoop();
	}
	CWkeWebkitUI::UninitializeWebkit();
	return 0;
}