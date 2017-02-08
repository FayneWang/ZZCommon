#pragma once


#include <wtypes.h>
#include <ZZUtility/DLLDefines.h>

struct CMsgTargetPrivate;
class _ZZUTILITY_EXTERN_ CWinMsgTarget
{
    friend LRESULT CALLBACK _WndMsgProc(HWND, UINT, WPARAM, LPARAM);
    friend class CWinMsgServer;

public:
    virtual ~CWinMsgTarget(void) = 0;

    BOOL PostThreadMessage(UINT uMsg,void *pParam);
    BOOL PostWndMsg(UINT uMsg,WPARAM wPamram = NULL,LPARAM lParam = NULL);

    void WaitMessage();
    void ExitWait();

protected:
    CWinMsgTarget(LPCWSTR lpWindowName = NULL);

	/**
	 * 获取窗口消息的响应成员函数
	 * 
	 * @param[in] uMsg		消息标识，具体使用，请查看Windows系统提供的消息列表。
	 * @param[in] wParam	额外的消息信息；内容取决于系统的uMsg或设计人员的uMsg。
	 * @param[in] lParam	额外的消息信息；内容取决于系统的uMsg或设计人员的uMsg。
	 *
	 * @return 当接收的消息不想继续传递时，返回TRUE；否则返回FALSE。
	 */
    virtual BOOL MsgProc(UINT uMsg,WPARAM wParam,LPARAM lParam)
    {
        UNREFERENCED_PARAMETER(uMsg);
        UNREFERENCED_PARAMETER(wParam);
        UNREFERENCED_PARAMETER(lParam);
        return FALSE;
    }

    HWND    m_hTargetWnd;

    CMsgTargetPrivate *m;

};
