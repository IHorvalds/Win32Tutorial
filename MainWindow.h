#pragma once

#include <d2d1.h>
#pragma comment(lib, "d2d1")
#include "BaseWindow.h"
#include "Scene.h"

class MainWindow :
    public BaseWindow<MainWindow>
{

    HANDLE m_hTimer;
    Scene m_scene;

    BOOL InitializeTimer();

public:

    MainWindow() : m_hTimer(NULL) { }

    PCWSTR ClassName() const { return L"Sample Window class"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);

    void WaitTimer();
};

