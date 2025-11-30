#include "Tab1.hpp"
#include "!Items.hpp"

bool items::Tab1(const char* label, ImVec2 pos, bool active)
{
	auto* window = ImGui::GetCurrentWindow();
	auto& style = ImGui::GetStyle();
	auto& io = ImGui::GetIO();

	style.Alpha = 1;
	float time = io.DeltaTime * T1::Speed;

	ImVec2 Size = ImGui::CalcTextSize(label);

	ImGui::SetCursorPos(pos);
	bool result = ImGui::InvisibleButton(label, Size);

	ImGuiID id = window->GetID(label);
	static std::map<ImGuiID, Tab1_State> anim;
	auto& t1 = anim[id];

	ImVec2 MIN = ImGui::GetItemRectMin();
	ImVec2 MAX = ImGui::GetItemRectMax();

	ImVec4 BG_Target = active ? T1::BG_Active : ImGui::IsItemHovered() ? T1::BG_Hovered : T1::BG;
	ImVec4 Text_Target = active ? T1::Text_Active : ImGui::IsItemHovered() ? T1::Text_Hovered : T1::Text;

	t1.TabBG = ImVec4(ImLerp(t1.TabBG, BG_Target, time));
	t1.TabText = ImVec4(ImLerp(t1.TabText, Text_Target, time));

	window->DrawList->AddCircleFilled(ImVec2((MIN.x + MAX.x) * 0.5f, MAX.y + 7), 2.f, ImGui::GetColorU32(t1.TabBG), 1000);	
	window->DrawList->AddText(MIN + (MAX - MIN) * 0.5 - ImGui::CalcTextSize(label) * 0.5, ImGui::GetColorU32(t1.TabText), label);

	if (result) Alpha = 0;
	return result;
}