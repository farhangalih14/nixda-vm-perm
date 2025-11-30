#include "!Items.hpp"
#include "main.h"
#include "font_awesome.h"

DWORD window_flags = ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollWithMouse | ImGuiWindowFlags_NoBackground;

void style()
{
    ImGuiStyle& style = ImGui::GetStyle();

    style.WindowBorderSize = 0.f;
    style.WindowRounding = window::rounding;
    style.ChildRounding = 0.f;
    style.FrameBorderSize = 0.f;
    style.WindowPadding = ImVec2(0, 0);
    style.Colors[ImGuiCol_ChildBg] = colors::lbg;
    style.ScrollbarSize = 0.f;
    style.ScrollbarRounding = 0.f;
    style.ItemSpacing = { 10, 15 };
    style.Colors[ImGuiCol_ScrollbarGrab] = colors::Transparent;
    style.Colors[ImGuiCol_TextSelectedBg] = colors::SecondColor;
    ImGui::PushFont(fonts::Inter_Medium);
}

void Move_Window()
{
    GetWindowRect(g_hwnd, &rc);
    MoveWindow(g_hwnd, rc.left + ImGui::GetWindowPos().x, rc.top + ImGui::GetWindowPos().y, window::Begin_Size.x, window::Begin_Size.y, TRUE);
    ImGui::SetWindowPos(ImVec2(0.f, 0.f));
}

void Title(const char title[], ImVec2 pos)
{
    auto* window = ImGui::GetCurrentWindow();
    ImGui::SetCursorPos(pos);
    ImVec2 currentPos = pos;

    ImGui::PushFont(fonts::Lexend);

    for (int i = 0; title[i] != '\0'; i++)
    {
        float alpha = ImClamp(1.0f - (i * 0.070f), 0.0f, 1.0f);

        ImVec4 color = ImVec4(colors::lwhite.x, colors::lwhite.y, colors::lwhite.z, alpha);

        ImGui::SetCursorPos(currentPos);
        ImGui::TextColored(color, "%c", title[i]);

        currentPos.x += ImGui::CalcTextSize(&title[i], &title[i] + 1).x;
    }
}

void custom::Begin(const char* name, ImVec2 size)
{
    ImGuiStyle& s = ImGui::GetStyle();

    s.WindowRounding = window::rounding;
    s.Alpha = 1;

    ImGui::SetNextWindowSize(size);
    ImGui::Begin(name, NULL, window_flags);
    ImGuiWindow* window = ImGui::GetCurrentWindow();
    window->DrawList->AddRectFilled(window->Pos, ImVec2(window->Pos + size - ImVec2(1, 1)), ImGui::GetColorU32(colors::bg), window::rounding);

    custom::AddImage(images::logo, { 35, 18 }, { 50, 50 });
    ImGui::PushFont(fonts::Lexend);

    Title(name, { 85, 30 });

    items::Icon_Button(ICON_FA_XMARK, {size.x - 50, 33}, close);

    window->DrawList->AddShadowCircle(window->Pos + ImVec2(50, 450), 15.f, ImGui::GetColorU32(colors::SecondColor), 400, { 0,0 }, 0, 1000);
    window->DrawList->AddShadowCircle(window->Size - ImVec2(100, 450), 15.f, ImGui::GetColorU32(colors::SecondColor), 400, { 0,0 }, 0, 1000);

    window->DrawList->AddRect(window->Pos, ImVec2(window->Pos + size - ImVec2(1, 1)), ImGui::GetColorU32(colors::Gray), window::rounding);
    style();
    Move_Window();
}

void custom::End() { 
    ImGui::End(); 
}

void custom::Info(const char* Title, const char* Description, ImVec2 pos, ImVec4 col)
{
    auto* w = ImGui::GetCurrentWindow();

    w->DrawList->AddText(fonts::Inter_Medium, 17, pos + w->Pos, ImGui::GetColorU32(colors::lwhite), Title);
    w->DrawList->AddText(fonts::Inter_Medium, 17, { w->Pos.x + w->Size.x - ImGui::CalcTextSize(Description).x - pos.x, w->Pos.y + pos.y }, ImGui::GetColorU32(col), Description);
    w->DrawList->AddLine({ w->Pos.x + 15, pos.y + w->Pos.y + 30 }, { w->Pos.x + w->Size.x - 15, pos.y + w->Pos.y + 30 }, ImGui::GetColorU32(colors::SecondColor));
}

void custom::Parameter(const char* label, bool& c, float PosY, float space)
{
    auto* w = ImGui::GetCurrentWindow();

    items::Checkbox1(label, { w->Size.x - 60, PosY }, &c, space);
    w->DrawList->AddLine({ w->Pos.x + 15, PosY + w->Pos.y + 30 }, { w->Pos.x + w->Size.x - 15, PosY + w->Pos.y + 30 }, ImGui::GetColorU32(colors::SecondColor));
}

void custom::Child_Begin(const char* name, ImVec2 pos, ImVec2 size)
{
    auto& s = ImGui::GetStyle();

    ImGui::SetNextWindowPos(pos);
    ImGui::BeginChild(name, size);
    auto* window = ImGui::GetCurrentWindow();

    window->DrawList->AddRectFilled(window->Pos, window->Pos + size, ImGui::GetColorU32(colors::lbg), window::rounding);
    window->DrawList->AddRect(window->Pos, window->Pos + size, ImGui::GetColorU32(colors::Gray), window::rounding);
}

void custom::Child_End() { ImGui::EndChild(); }

void custom::Pf(ImFont* font) { ImGui::PushFont(font); }

void custom::AddImage(ID3D11ShaderResourceView* pic, ImVec2 pos, ImVec2 size)
{
    ImGui::SetCursorPos(pos);
    ImGui::Image(pic, size);
}

void custom::Text(const char* text, ImVec2 pos, ImFont* font, int font_Size, ImVec4 color)
{
    auto* window = ImGui::GetCurrentWindow();
    auto& style = ImGui::GetStyle();
    auto& io = ImGui::GetIO();
    
    window->DrawList->AddText(font, font_Size, pos, ImGui::GetColorU32(color), text);
    ImGui::PushFont(fonts::Inter_Medium);
}

void custom::Blur(HWND hwnd)
{
    struct ACCENTPOLICY
    {
        int na;
        int nf;
        int nc;
        int nA;
    };
    struct WINCOMPATTRDATA
    {
        int na;
        PVOID pd;
        ULONG ul;
    };

    const HINSTANCE hm = LoadLibrary(L"user32.dll");
    if (hm)
    {
        typedef BOOL(WINAPI* pSetWindowCompositionAttribute)(HWND, WINCOMPATTRDATA*);

        const pSetWindowCompositionAttribute SetWindowCompositionAttribute = (pSetWindowCompositionAttribute)GetProcAddress(hm, "SetWindowCompositionAttribute");
        if (SetWindowCompositionAttribute)
        {
            ACCENTPOLICY policy = { 3, 0, 0, 0 }; // 4,0,155,0 (Acrylic blur) 3,0,0,0 
            WINCOMPATTRDATA data = { 19, &policy,sizeof(ACCENTPOLICY) };
            SetWindowCompositionAttribute(hwnd, &data);
        }
        FreeLibrary(hm);
    }
}
