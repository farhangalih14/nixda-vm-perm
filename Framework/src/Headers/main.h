#pragma once

#define IMGUI_DEFINE_MATH_OPERATORS
#pragma comment(lib, "D3DX11.lib")

#include <map>
#include <d3d11.h>
#include <windows.h>
#include <dwmapi.h>
#include <string>
#include <vector>
#include <D3DX11tex.h>
#include <algorithm>

#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"

inline RECT rc;
extern HWND g_hwnd;

inline float Alpha = 0;
inline int Tab;
inline int Sub_Tab = 0;

inline char user[50] = "";
inline char pass[20] = "";
inline char key[20] = "";

inline bool remember;
inline bool GPU;
inline bool DISK;
inline bool ARP;

inline bool ring0;
inline bool permanent;
inline bool legit;

inline bool remember1;

inline ID3D11Device* g_pd3dDevice = nullptr;
inline void AlphaAnim() {
	auto& style = ImGui::GetStyle();
	style.Alpha = Alpha;
	Alpha = std::clamp(Alpha + (0.88f * ImGui::GetIO().DeltaTime * 1.f), 0.0f, 1.f);
}

enum Icon_Button_Actions {
	close,
	minimize
};

namespace images {
	inline ID3D11ShaderResourceView* logo = nullptr;
}

namespace window 
{ 
	inline ImVec2 size_max = { 675, 575 }; 
	inline ImVec2 Begin_Size = { 500, 575 }; 
	const float rounding = 4.f; }
	namespace ui { void Before_Loop(); void Render(); 
}

namespace fonts {
	inline ImFont* Inter_Medium_T = nullptr;
	inline ImFont* Inter_Medium = nullptr;
	inline ImFont* Lexend = nullptr;
	inline ImFont* FontAwesome = nullptr;
}

namespace colors {
	const ImVec4 bg = { 0.03137255f, 0.03137255f, 0.03137255f, 0.95 };
	const ImVec4 lbg = { 0.04313726f, 0.04313726f, 0.04313726f, 1 };

	const ImVec4 SecondColor = ImColor(161, 18, 166, 255);
	const ImVec4 MainColor = ImColor(238, 0, 255, 255);

	const ImVec4 White = { 1, 1, 1, 1 };
	const ImVec4 lwhite = { 0.75960787f, 0.7556863f, 0.7556863f, 1.0f };
	const ImVec4 Gray = { 0.14811765f, 0.14811765f, 0.14811765f, 1.0f };
	const ImVec4 Gray2 = { 0.4f, 0.4f, 0.4f, 1.0f };
	const ImVec4 Green = { 0.247, 0.6824, 0.247 ,1 };
	const ImVec4 Red = { 0.6824, 0.1608, 0.1608, 1 };

	const ImVec4 Transparent = { 0., 0., 0., .0f };
}