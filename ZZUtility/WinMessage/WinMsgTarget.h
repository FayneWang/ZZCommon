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
	 * ��ȡ������Ϣ����Ӧ��Ա����
	 * 
	 * @param[in] uMsg		��Ϣ��ʶ������ʹ�ã���鿴Windowsϵͳ�ṩ����Ϣ�б�
	 * @param[in] wParam	�������Ϣ��Ϣ������ȡ����ϵͳ��uMsg�������Ա��uMsg��
	 * @param[in] lParam	�������Ϣ��Ϣ������ȡ����ϵͳ��uMsg�������Ա��uMsg��
	 *
	 * @return �����յ���Ϣ�����������ʱ������TRUE�����򷵻�FALSE��
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
