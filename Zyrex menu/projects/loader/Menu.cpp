//============ Copyright ImMagic, All rights reserved ============//
//
// Purpose: 
//
//================================================================//

#include "Menu.h"

// Dear ImGui
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

// Dear ImGui - Backends
#include "imgui/backends/imgui_impl_dx11.h"
#include "imgui/backends/imgui_impl_win32.h"

// Dear ImGui - Misc
#include "imgui/misc/imgui_freetype.h"

bool Menu::Initialize(HWND hWnd, ID3D11Device* pDevice, ID3D11DeviceContext* pDeviceContext)
{
    bool result = true;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();

    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    io.IniFilename = nullptr;   // Disable INI File

    // Setup Dear ImGui default style
    ImGui::StyleColorsDark();

    style.WindowRounding    = 3;

    style.WindowBorderSize  = 1;
    style.FrameBorderSize   = 1;
    style.PopupBorderSize   = 1;

    style.WindowPadding     = ImVec2(8, 8);
    style.FramePadding      = ImVec2(6, 6);
    style.ItemSpacing       = ImVec2(6, 6);

    style.Colors[ImGuiCol_WindowBg]         = ImVec4(0, 0, 0, 0.75f);

    // Setup Font
    ImFontConfig font_cfg_main;
    font_cfg_main.FontLoaderFlags = ImGuiFreeTypeLoaderFlags_ForceAutoHint;
    font_cfg_main.GlyphOffset = ImVec2(0, 1);
    font_cfg_main.SizePixels = 12.0f;
	io.Fonts->AddFontDefault(&font_cfg_main);

    // Initialize Dear ImGui - WIN32
    result = ImGui_ImplWin32_Init(hWnd);
    if (!result) return false;

    // Initialize Dear ImGui - DX11
    result = ImGui_ImplDX11_Init(pDevice, pDeviceContext);
    if (!result) return false;

    m_bInitialized = true;

    return true;
}

void Menu::Render()
{
    if (!m_bInitialized) return;

    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    {
        DrawMenu();
    }
    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Menu::DrawMenu()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImGuiIO& io = ImGui::GetIO(); (void)io;

    ImGui::SetNextWindowSize(io.DisplaySize, ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    if (ImGui::Begin("ImMagic - Loader", 0, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImGui::Text("This offer does not include a loader");

        ImGui::End();
    }
}

void Menu::Shutdown()
{
    if (!m_bInitialized) return;

    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();
}

void Menu::InvalidateDeviceObjects()
{
    if (!m_bInitialized) return;

    ImGui_ImplDX11_InvalidateDeviceObjects();
}

void Menu::CreateDeviceObjects()
{
    if (!m_bInitialized) return;

    ImGui_ImplDX11_CreateDeviceObjects();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
bool Menu::HandleMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (!m_bInitialized) return false;

    ImGui_ImplWin32_WndProcHandler(hWnd, uMsg, wParam, lParam);

    static POINTS s_WindowPosition;

    switch (uMsg)
    {
    case WM_LBUTTONDOWN:
    {
        s_WindowPosition = MAKEPOINTS(lParam);
        return 0L;
    }

    case WM_MOUSEMOVE:
    {
        if (wParam == MK_LBUTTON)
        {
            const auto points = MAKEPOINTS(lParam);
            auto rect = ::RECT{ };

            GetWindowRect(hWnd, &rect);

            rect.left += points.x - s_WindowPosition.x;
            rect.top += points.y - s_WindowPosition.y;

            if (GImGui->MovingWindow)
            {
                SetWindowPos(
                    hWnd,
                    HWND_TOPMOST,
                    rect.left, rect.top,
                    0, 0,
                    SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOZORDER
                );
            }
        }

        return 0;
    }
    }

    return false;
}
