#include "!Items.hpp"
#include "Button1.hpp"

bool items::Button1(const char* label, ImVec2 pos, ImVec2 size, ImVec4 bg)
{
	auto* window = ImGui::GetCurrentWindow();
	auto& style = ImGui::GetStyle();
	auto& io = ImGui::GetIO();

	style.Alpha = Alpha;
	float time = io.DeltaTime * B1::Speed;

	ImGui::SetCursorPos(pos);
	bool result = ImGui::InvisibleButton(label, size);

	ImGuiID id = window->GetID(label);
	static std::map<ImGuiID, Button1_State> anim;
	auto& b1 = anim[id];

	ImVec2 MIN = ImGui::GetItemRectMin();
	ImVec2 MAX = ImGui::GetItemRectMax();

	ImVec4 BG_Target = ImGui::IsItemActive() ? B1::BG_Active : ImGui::IsItemHovered() ? B1::BG_Hovered : B1::BG;
	ImVec4 BG_Target2 = ImGui::IsItemActive() ? B1::BG_Active2 : ImGui::IsItemHovered() ? B1::BG_Hovered2 : B1::BG2;
	ImVec4 Text_Target = ImGui::IsItemActive() ? B1::Text_Active : ImGui::IsItemHovered() ? B1::Text_Hovered : B1::Text;

	b1.ButtonBG = ImVec4(ImLerp(b1.ButtonBG, BG_Target, time));
	b1.ButtonBG2 = ImVec4(ImLerp(b1.ButtonBG2, BG_Target2, time));
	b1.Text = ImVec4(ImLerp(b1.Text, Text_Target, time));

	Shadow_On_Point(MIN, MAX, colors::MainColor, 500);

	window->DrawList->AddRect(MIN, MAX, ImGui::GetColorU32(colors::Gray), B1::Rounding);
	window->DrawList->AddRectFilledMultiColorRounded(MIN, MAX, ImGui::GetColorU32(bg), ImGui::GetColorU32(b1.ButtonBG), ImGui::GetColorU32(b1.ButtonBG2), ImGui::GetColorU32(b1.ButtonBG2), ImGui::GetColorU32(b1.ButtonBG), B1::Rounding, ImDrawFlags_RoundCornersAll);
	window->DrawList->AddText(MIN + (MAX - MIN) * 0.5 - ImGui::CalcTextSize(label) * 0.5, ImGui::GetColorU32(b1.Text), label);

	return result;
}