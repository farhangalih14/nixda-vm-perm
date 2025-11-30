#include "Combo.hpp"
#include "font_awesome.h"

bool items::Combo(const char* label, ImVec2 pos, const char* modes[], int numModes, int* currentItem)
{
    ImGui::PushID(label);

    auto* window = ImGui::GetCurrentWindow();
    auto& style = ImGui::GetStyle();
    auto& io = ImGui::GetIO();

    style.Alpha = Alpha;
    float time = io.DeltaTime * C1::Speed;

    ImVec2 originalSize = { 235, 44 };
    ImVec2 adjustedSize = { originalSize.x - 5, originalSize.y - 5 };

    ImGui::SetCursorPos(pos);
    bool result = ImGui::InvisibleButton(label, originalSize);

    static bool opened = false;
    static float py = 1.f;

    if (result) { opened = !opened; }

    ImVec2 MIN = ImGui::GetItemRectMin();
    ImVec2 MAX = ImGui::GetItemRectMax();

    ImVec4 Border_Target = opened ? C1::Border_Active : ImGui::IsItemHovered() ? C1::Border_Hovered : C1::Border;
    bool itemSelected = false;

    ImGuiID id = window->GetID(label);
    static std::map<ImGuiID, Combo_State[20]> animStates;
    auto& states = animStates[id];

    states->Border_Color = Border_Target;
    states->Combo_BG = C1::BG;

    window->DrawList->AddRect(MIN, MAX, ImGui::GetColorU32(states[*currentItem != -1 ? *currentItem : 0].Border_Color), C1::Rounding);
    Shadow_On_Point(MIN, MAX, colors::MainColor, 500);
    window->DrawList->AddRectFilled(MIN + ImVec2(2, 2), MAX - ImVec2(2, 2), ImGui::GetColorU32(states[*currentItem != -1 ? *currentItem : 0].Combo_BG), C1::Rounding);

    const char* selectedItemText = (*currentItem != -1) ? modes[*currentItem] : "Motherboard";

    ImGui::SetCursorPos({ pos.x + 10, pos.y + (originalSize.y - ImGui::CalcTextSize(selectedItemText).y) / 2.0f });
    ImGui::TextColored(colors::lwhite, "%s", selectedItemText);

    ImFont* c = ImGui::GetFont();
    ImGui::PushFont(fonts::FontAwesome);

    ImGui::SetCursorPos({ pos.x + 225 - ImGui::CalcTextSize(ICON_FA_ANGLE_DOWN).x, pos.y + (originalSize.y - ImGui::CalcTextSize(ICON_FA_ANGLE_DOWN).y) / 2.0f });
    ImGui::TextColored(colors::lwhite, "%s", ICON_FA_ANGLE_DOWN);

    ImGui::PushFont(c);

    py = opened ? ImLerp(py, 130.f, time) : ImLerp(py, -5.f, time);

    style.Colors[ImGuiCol_Border] = colors::Gray;
    style.ChildRounding = C1::Rounding;

    if (py > 1.5)
    {
        ImGui::SetCursorPos({ pos.x, pos.y + 40 });
        ImGui::BeginChild("ComboChild", ImVec2(adjustedSize.x + 5, py), false, ImGuiWindowFlags_AlwaysVerticalScrollbar);
        {
            auto* c = ImGui::GetCurrentWindow();
            c->DrawList->AddRectFilled(c->Pos - ImVec2(0, -2), c->Pos + c->Size, ImGui::GetColorU32(colors::bg));

            for (int i = 0; i < numModes; i++) {
                float centeredX = (adjustedSize.x + 5 - adjustedSize.x) / 2.0f;
                ImVec2 buttonPos = { centeredX, (adjustedSize.y + 2) * i };
                ImGui::SetCursorPos(buttonPos);
                bool selected = ImGui::InvisibleButton(modes[i], adjustedSize);

                ImVec4 BG_Target = ImGui::IsItemActive() ? C1::BG_Active : ImGui::IsItemHovered() ? C1::BG_Hovered : C1::BG;
                ImVec4 Text_Target = (selected || i == *currentItem) || ImGui::IsItemActive() ? C1::Text_Active : ImGui::IsItemHovered() ? C1::Text_Hovered : C1::Text;

                states[i].Combo_BG = ImLerp(states[i].Combo_BG, BG_Target, time / 2);
                states[i].Text_Color = ImLerp(states[i].Text_Color, Text_Target, time / 2);
                states[i].Border_Color = ImLerp(states[i].Border_Color, Border_Target, time / 2);

                if (selected) { *currentItem = i; itemSelected = true; }

                ImVec2 itemMin = ImGui::GetItemRectMin();
                ImVec2 itemMax = ImGui::GetItemRectMax();

                c->DrawList->AddRectFilled(itemMin, itemMax + ImVec2(0, 2), ImGui::GetColorU32(states[i].Combo_BG));

                ImVec2 textSize = ImGui::CalcTextSize(modes[i]);
                ImVec2 textPos = { itemMin.x + 10, itemMin.y + (adjustedSize.y - textSize.y) / 2.0f };

                c->DrawList->AddText(textPos, ImGui::GetColorU32(states[i].Text_Color), modes[i]);
            }
            c->DrawList->AddRect(MIN, c->Pos + c->Size, ImGui::GetColorU32(states->Border_Color), C1::Rounding, ImDrawFlags_RoundCornersBottom);
        }
        ImGui::EndChild();
    }
    ImGui::PopID();
    return itemSelected;
}
