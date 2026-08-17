//============ Copyright ImMagic, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#define APP_NAME        "ImMagic"
#define APP_VERSION     "1.2.3"
#define APP_BUILD       __DATE__

#include "Application.h"
#include "Console.h"
#include "Window.h"
#include "Direct3D11.h"
#include "Menu.h"

class CGuiApp : public CApplication
{
public:
    CGuiApp() : m_pWindow(nullptr), m_pDirect3D11(nullptr)
    {
    }

public:
    bool Initialize() override;
    void Shutdown() override;
    void Frame() override;

private:
    static LRESULT MessageHandler(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

private:
    CWindow* m_pWindow;
    CDirect3D11* m_pDirect3D11;

private:
    inline static CGuiApp* s_pAppInstance = nullptr;
};

LRESULT CGuiApp::MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    Menu::HandleMessage(hWnd, uMsg, wParam, lParam);

    switch (uMsg)
    {
    case WM_SIZE:
    {
        if (wParam == SIZE_MINIMIZED)
            return 0;

        int iNewWidth = (UINT)LOWORD(lParam); // Queue resize
        int iNewHeight = (UINT)HIWORD(lParam);

        s_pAppInstance->m_pDirect3D11->SetBuffersSize(iNewWidth, iNewHeight);

        return 0;
    }

    case WM_DESTROY:
    {
        PostQuitMessage(0);
        return 0;
    }

    default:
    {
        return DefWindowProc(hWnd, uMsg, wParam, lParam);
    }
    }
}

bool CGuiApp::Initialize()
{
    if (!CApplication::Initialize())
        return false;

	Console::SetTitle(APP_NAME " v" APP_VERSION " - " APP_BUILD);

    s_pAppInstance = this;

    m_pWindow = new CWindow();
    if (!m_pWindow->Initialize("Class001", APP_NAME, 100, 100, 240, 70, true))
    {
        Shutdown();
        return false;
    }

    m_pWindow->HandleMessage = MessageHandler;

    m_pDirect3D11 = new CDirect3D11();
    if (!m_pDirect3D11->Initialize(true, m_pWindow->GetWindowHandle()))
    {
        Shutdown();
        return false;
    }

    m_pWindow->Show();

    if (!Menu::Initialize(m_pWindow->GetWindowHandle(), m_pDirect3D11->GetDevice(), m_pDirect3D11->GetDeviceContext()))
    {
        Shutdown();
        return false;
    }

    return true;
}

void CGuiApp::Shutdown()
{
    Menu::Shutdown();

    if (m_pDirect3D11)
    {
        m_pDirect3D11->Shutdown();
        delete m_pDirect3D11;
        m_pDirect3D11 = nullptr;
    }

    if (m_pWindow)
    {
        m_pWindow->Release();
        delete m_pWindow;
        m_pWindow = nullptr;
    }

    CApplication::Shutdown();
}

void CGuiApp::Frame()
{
    m_pDirect3D11->BeginScene(0.0f, 0.0f, 0.0f, 0.0f);

    Menu::Render();

    m_pDirect3D11->EndScene();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
    CGuiApp app;

    if (!app.Initialize())
        return EXIT_FAILURE;

    app.Run();

    return EXIT_SUCCESS;
}
