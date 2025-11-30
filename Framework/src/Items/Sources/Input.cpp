#include "Input.hpp"



bool items::InputText(const char* label, const char* hint, const char* icon, ImVec2 pos, static char buf[], size_t buf_size, ImGuiInputTextFlags flag, ImDrawFlags DrawFlag)
{
    auto* window = ImGui::GetCurrentWindow();
    auto& style = ImGui::GetStyle();
    auto& io = ImGui::GetIO();

    float time = io.DeltaTime * I1::Speed;

    style.Alpha = Alpha;
    style.Colors[ImGuiCol_FrameBg] = colors::Transparent;
    style.Colors[ImGuiCol_TextDisabled] = colors::Transparent;
    style.FramePadding = ImVec2(8.f, I1::Size.y);

    ImGui::SetCursorPos(pos);
    ImGui::SetNextItemWidth(I1::Size.x);

    ImGuiID id = window->GetID(label);
    static std::map<ImGuiID, Input_State> anim;
    auto& i1 = anim[id];

    ImVec2 MIN = ImGui::GetCursorScreenPos();
    ImVec2 MAX = ImVec2(MIN.x + I1::Size.x, MIN.y + ImGui::GetFrameHeight());

    window->DrawList->AddRect(MIN, MAX, ImGui::GetColorU32(i1.Border_Color), I1::Rounding, DrawFlag);

    Shadow_On_Point(MIN, MAX, colors::MainColor, 1000);

    window->DrawList->AddRectFilled(MIN + ImVec2(2,2), MAX - ImVec2(2 ,2), ImGui::GetColorU32(i1.BG_Color), I1::Rounding, DrawFlag);

    bool result = ImGui::InputTextWithHint(label, hint, buf, (int)buf_size, flag);

    ImVec4 Background_Target = ImGui::IsItemActive() ? I1::BG_Active : ImGui::IsItemHovered() ? I1::BG_Hovered : I1::BG;
    ImVec4 Border_Target = ImGui::IsItemActive() ? I1::Border_Active : ImGui::IsItemHovered() || strlen(buf) != 0 ? I1::Border_Hovered : I1::Border;
    ImVec4 Text_Color_Target = ImGui::IsItemActive() ? I1::Text_Active : ImGui::IsItemHovered() || strlen(buf) != 0 ? I1::Text_Hovered : I1::Text;
    ImVec4 Icon_Color_Target = ImGui::IsItemActive() ? I1::Icon_Active : ImGui::IsItemHovered() ? I1::Icon_Hovered : I1::Icon;
    ImVec2 Text_Pos_Target = ImGui::IsItemActive() || strlen(buf) != 0 ? ImVec2(10, -19) : ImVec2(10, (MAX.y - MIN.y - ImGui::CalcTextSize(hint).y) / 2);

    i1.BG_Color = ImVec4(ImLerp(i1.BG_Color, Background_Target, time));
    i1.Border_Color = ImVec4(ImLerp(i1.Border_Color, Border_Target, time));
    i1.Text_Color = ImVec4(ImLerp(i1.Text_Color, Text_Color_Target, time));
    i1.Icon_Color = ImVec4(ImLerp(i1.Icon_Color, Icon_Color_Target, time));
    i1.Text_Pos = ImVec2(ImLerp(i1.Text_Pos, Text_Pos_Target, time));

    window->DrawList->AddText(MIN + i1.Text_Pos, ImGui::GetColorU32(i1.Text_Color), hint);

    if (icon)
    {
        ImFont* current = ImGui::GetFont();
        ImGui::PushFont(fonts::FontAwesome);
        window->DrawList->AddText({ MAX.x - 30, MIN.y + (MAX.y - MIN.y) / 2 - ImGui::CalcTextSize(icon).y / 2 }, ImGui::GetColorU32(i1.Icon_Color), icon);
        ImGui::PopFont();
    }

    ImGui::PopStyleVar(1);
    ImGui::PopStyleColor(2);
    ImGui::TreePop();

    return result;
}
