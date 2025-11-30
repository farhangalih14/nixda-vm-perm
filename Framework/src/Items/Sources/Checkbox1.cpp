#include "Checkbox1.hpp"

bool items::Checkbox1(const char* label, ImVec2 pos, bool* active, float space)
{
	auto* window = ImGui::GetCurrentWindow();
	auto& style = ImGui::GetStyle();
	auto& io = ImGui::GetIO();

	style.Alpha = Alpha;
	float time = io.DeltaTime * C1::Speed;

	ImGui::SetCursorPos(pos);
	bool result = ImGui::InvisibleButton(label, { 35, 18 });

	ImGuiID id = window->GetID(label);
	static std::map<ImGuiID, Checkbox1_State> anim;
	auto& c1 = anim[id];

	if (result) *active = !*active;

	const ImVec2 MIN = ImGui::GetItemRectMin();
	const ImVec2 MAX = ImGui::GetItemRectMax();

	ImVec4 BG_Target = *active ? C1::BG_Active : ImGui::IsItemHovered() ? C1::BG_Hovered : C1::BG;
	ImVec4 Text_Target = *active ? C1::Text_Active : ImGui::IsItemHovered() ? C1::Text_Hovered : C1::Text;
	ImVec4 Circle_Target = *active ? C1::Circle_Active : ImGui::IsItemHovered() ? C1::Circle_Hovered : C1::Circle;
	ImVec4 Shadow_Target = *active ? C1::Shadow_Active : ImGui::IsItemHovered() ? C1::Shadow_Hovered : C1::Shadow;
	ImVec4 Border_Target = *active ? C1::Border_Active : ImGui::IsItemHovered() ? C1::Border_Hovered : C1::Border;
	ImVec2 Circle_Pos_Target = *active ? ImVec2(35 - 9, 9) : ImVec2(9, 9);

	c1.Checkbox_BG = ImVec4(ImLerp(c1.Checkbox_BG, BG_Target, time));
	c1.Text_Color = ImVec4(ImLerp(c1.Text_Color, Text_Target, time));
	c1.Shadow_Color = ImVec4(ImLerp(c1.Shadow_Color, Shadow_Target, time));
	c1.Circle_Color = ImVec4(ImLerp(c1.Circle_Color, Circle_Target, time));
	c1.Border_Color = ImVec4(ImLerp(c1.Border_Color, Border_Target, time));
	c1.Circle_Pos = ImVec2(ImLerp(c1.Circle_Pos.x, Circle_Pos_Target.x, time), ImLerp(c1.Circle_Pos.y, Circle_Pos_Target.y, time));

	window->DrawList->AddRectFilled(MIN, MAX, ImGui::GetColorU32(c1.Checkbox_BG), 25.f);
	window->DrawList->AddCircleFilled(MIN + c1.Circle_Pos, 7, ImGui::GetColorU32(c1.Circle_Color));
	window->DrawList->AddShadowCircle(MIN + c1.Circle_Pos, 7, ImGui::GetColorU32(c1.Shadow_Color), 35, { 0,0 }, 0, 1000);
	window->DrawList->AddText({ MIN.x - space, MIN.y + (MAX.y - MIN.y - ImGui::CalcTextSize(label).y) / 2 }, ImGui::GetColorU32(c1.Text_Color), label);
	
	ImGui::PopID();
	return result;
}