#pragma once
#include "main.h"


namespace custom
{
	/*******************************************************************/
	void Begin(const char* name, ImVec2 size);
	void End();
	void Child_Begin(const char* name, ImVec2 pos, ImVec2 size);
	void Child_End();
    /*******************************************************************/
	void Pf(ImFont* font);
	void AddImage(ID3D11ShaderResourceView* pic, ImVec2 pos, ImVec2 size);
	void Text(const char* text, ImVec2 pos, ImFont* font, int font_Size, ImVec4 color);
	void Info(const char* Title, const char* Description, ImVec2 pos, ImVec4 col);
	void Parameter(const char* label, bool& c, float PosY, float space);
	void Blur(HWND hwnd);
	/*******************************************************************/
}

namespace items
{
	bool Button1(const char* label, ImVec2 pos, ImVec2 size, ImVec4 bg);
	bool Tab1(const char* label, ImVec2 pos, bool active);
	bool Checkbox1(const char* label, ImVec2 pos, bool* active, float space);
	bool InputText(const char* label, const char* hint, const char* icon, ImVec2 pos, static char buf[], size_t buf_size, ImGuiInputTextFlags flag, ImDrawFlags DrawFlag);
	bool Keybind(const char* label, ImVec2 pos, int* keyCode, int* mode, bool isLabelActive);
	bool Combo(const char* label, ImVec2 pos, const char* modes[], int numModes, int* currentItem);
	bool Slider(const char* label, const char* text, ImVec2 pos, float* value, float minValue, float maxValue);
	bool MultiCombo(const char* label, ImVec2 pos, const char* modes[], int numModes, std::vector<bool>& selectedItems);
	bool Icon_Button(const char* icon, ImVec2 pos, Icon_Button_Actions action);
}

inline void Shadow_On_Point(const ImVec2 pos, const ImVec2 size, ImVec4 color, float thickness)
{
	auto* window = ImGui::GetCurrentWindow();
	auto& io = ImGui::GetIO();
	auto& style = ImGui::GetStyle();
	style.Alpha = Alpha;

	window->DrawList->PushClipRect(pos, size, true);
	window->DrawList->AddShadowCircle(io.MousePos, 1.f, ImGui::GetColorU32(color), thickness, ImVec2(0, 0), 1000.f);
	window->DrawList->PopClipRect();
}