#include "main.h"
#include "!Items.hpp"

struct Input_State {
	ImVec4 BG_Color;
	ImVec4 Border_Color;
	ImVec4 Text_Color;
	ImVec4 Icon_Color;
	ImVec2 Text_Pos;
};

namespace I1 {
	/*************************************************************************************/
	const ImVec4 BG_Active = colors::bg; // Background color when its active
	const ImVec4 BG_Hovered = colors::bg; // Background color when its hovered
	const ImVec4 BG = colors::bg; // Background color when nothing its happening
	/*************************************************************************************/
	const ImVec4 Text_Active = colors::White; // Text color when its active
	const ImVec4 Text_Hovered = colors::lwhite; // Text color when its hovered
	const ImVec4 Text = colors::lwhite; // Text color when nothing its happening
	/*************************************************************************************/
	const ImVec4 Border_Active = colors::MainColor; // Border color when its active
	const ImVec4 Border_Hovered = colors::Gray; // Border color when its hovered
	const ImVec4 Border = colors::Gray; // Border color when nothing is happening
	/*************************************************************************************/
	const ImVec4 Icon_Active = colors::MainColor; // Icon color when its active
	const ImVec4 Icon_Hovered = colors::SecondColor; // Icon color when its hovered
	const ImVec4 Icon = colors::SecondColor; // Icon color when nothing is happening
	/*************************************************************************************/
	const float Speed = 5.f; // Animation's speed
	const float Rounding = 3.f; // Item's Rounding
	const ImVec2 Size = ImVec2(340, 14.f); 
	/*************************************************************************************/
}