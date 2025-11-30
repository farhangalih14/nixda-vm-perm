#include "main.h"

struct Tab1_State {
	ImVec4 TabBG;
	ImVec4 TabText;
	ImVec4 TabIcon;
	ImVec4 ShadowBG;
	float Shadow_Thickness;
};

namespace T1 {
	/****************************************************************************************/
	const ImVec4 BG_Active = colors::MainColor; // Background color when its active
	const ImVec4 BG_Hovered = colors::Transparent; // Background color when its hovered
	const ImVec4 BG = colors::Transparent; // Background color when nothing its happening
	/****************************************************************************************/
	const ImVec4 Text_Active = colors::White; // Text color when its active
	const ImVec4 Text_Hovered = colors::White; // Text color when its hovered
	const ImVec4 Text = colors::Gray; // Text color when nothing its happening
	/****************************************************************************************/
	const float Speed = 5.f; // Tab's animation speed
	/****************************************************************************************/
}