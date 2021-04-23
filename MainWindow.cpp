#include "MainWindow.h"

struct StateInfo {
};

inline StateInfo* GetAppState(HWND hwnd)
{
    LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
    StateInfo* pState = reinterpret_cast<StateInfo*>(ptr);
    return pState;
}

BOOL MainWindow::InitializeTimer()
{
    m_hTimer = CreateWaitableTimer(NULL, FALSE, NULL);

    if (m_hTimer == NULL)
    {
        return FALSE;
    }

    LARGE_INTEGER li = { 0 };

    if (!SetWaitableTimer(m_hTimer, &li, (1000 / 60), NULL, NULL, FALSE))
    {
        CloseHandle(m_hTimer);
        m_hTimer = NULL;
        return FALSE;
    }

    return TRUE;
}

void MainWindow::WaitTimer()
{
    if (MsgWaitForMultipleObjects(1, &m_hTimer, FALSE, INFINITE, QS_ALLINPUT) == WAIT_OBJECT_0)
    {
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    HWND hwnd = m_hwnd;

    switch (uMsg)
    {
    case WM_CREATE:
        if (FAILED(m_scene.Initialize()) || !InitializeTimer())
        {
            return -1;
        }
        return 0;

    case WM_DESTROY:
        CloseHandle(m_hTimer);
        m_scene.CleanUp();
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
    case WM_DISPLAYCHANGE:
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hwnd, &ps);
        m_scene.Render(m_hwnd);
        EndPaint(m_hwnd, &ps);
    }
    return 0;

    case WM_SIZE:
    {
        int x = (int)(short)LOWORD(lParam);
        int y = (int)(short)HIWORD(lParam);
        m_scene.Resize(x, y);
        InvalidateRect(m_hwnd, NULL, FALSE);
    }
    return 0;

    case WM_ERASEBKGND:
        return 1;
    }
    return DefWindowProc(m_hwnd, uMsg, wParam, lParam);
}