#include "Icon Button.hpp"


bool items::Icon_Button(const char* icon, ImVec2 pos, Icon_Button_Actions action)
{
	auto* window = ImGui::GetCurrentWindow();
	auto& style = ImGui::GetStyle();
	auto& io = ImGui::GetIO();

	float time = io.DeltaTime * I1::Speed;

	ImGui::SetCursorPos(pos);
	bool result = ImGui::InvisibleButton(icon, { 12, 15 });

	ImGuiID id = window->GetID(icon);
	static std::map<ImGuiID, Icon_Button_State> anim;
	auto& i1 = anim[id];

	ImVec2 MIN = ImGui::GetItemRectMin();
	ImVec2 MAX = ImGui::GetItemRectMax();

	ImVec4 Text_Target = ImGui::IsItemHovered() ? I1::Text_Hovered : I1::Text;

	i1.Text = ImVec4(ImLerp(i1.Text, Text_Target, time));

	ImFont* c = ImGui::GetFont();
	ImGui::PushFont(fonts::FontAwesome);
	window->DrawList->AddText(MIN, ImGui::GetColorU32(i1.Text), icon);
	ImGui::PushFont(c);

	switch (action)
	{
	case close: if (result) exit(0); break;
	case minimize: if (result) ShowWindow(g_hwnd, SW_MINIMIZE); break;
	}
}