#include <assert.h>
#include "WinMsgDispatcher.h"

CWinMsgDispatcher::CWinMsgDispatcher(HWND hMsgWnd /*= nullptr*/,UINT uMsgId/*=0*/): m_hMsgWnd(hMsgWnd),m_uMsgId(uMsgId)
{}


CWinMsgDispatcher::~CWinMsgDispatcher(void)
{
}

void CWinMsgDispatcher::AttachMsgTarget(HWND hMsgWnd,UINT uMsgId)
{
    m_hMsgWnd = hMsgWnd;
    m_uMsgId = uMsgId;
}

BOOL CWinMsgDispatcher::PostMsg(WPARAM wParam,LPARAM lParam)
{
	assert(m_hMsgWnd != nullptr);
	if (m_hMsgWnd == nullptr)
		return FALSE;

    return ::PostMessage(m_hMsgWnd,m_uMsgId,wParam,lParam);
}

LRESULT CWinMsgDispatcher::SendMsg(WPARAM wParam,LPARAM lParam)
{
	assert(m_hMsgWnd != nullptr);
	if (m_hMsgWnd == nullptr)
		return S_FALSE;

    return ::SendMessage(m_hMsgWnd,m_uMsgId,wParam,lParam);
}
