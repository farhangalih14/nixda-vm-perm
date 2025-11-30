#include "main.h"
#include "!Items.hpp"

struct Checkbox1_State {
	ImVec4 Checkbox_BG;
	ImVec4 Circle_Color;
	ImVec4 Shadow_Color;
	ImVec4 Border_Color;
	ImVec4 Text_Color;
	ImVec2 Circle_Pos;
};

namespace C1 {
	/*************************************************************************************/
	const ImVec4 BG_Active = colors::Gray; // Background color when its active
	const ImVec4 BG_Hovered = colors::Gray; // Background color when its hovered
	const ImVec4 BG = colors::Gray; // Background color when nothing its happening
	/*************************************************************************************/
	const ImVec4 Text_Active = colors::White; // Text color when its active
	const ImVec4 Text_Hovered = colors::lwhite; // Text color when its hovered
	const ImVec4 Text = colors::lwhite; // Text color when nothing its happening
	/*************************************************************************************/
	const ImVec4 Border_Active = colors::MainColor; // Border color when its active
	const ImVec4 Border_Hovered = colors::Gray; // Border color when its hovered
	const ImVec4 Border = colors::Gray; // Border color when nothing is happening
	/*************************************************************************************/	
	const ImVec4 Circle_Active = colors::MainColor; // Circle color when its active
	const ImVec4 Circle_Hovered = { 0.29411766f, 0.29411766f, 0.29411766f, 1.0f }; // Circle color when its hovered
	const ImVec4 Circle = { 0.29411766f, 0.29411766f, 0.29411766f, 1.0f }; // Circle color when nothing is happening
	/*************************************************************************************/
	const ImVec4 Shadow_Active = colors::MainColor; // Shadow color when its active
	const ImVec4 Shadow_Hovered = colors::Transparent; // Shadow color when its hovered
	const ImVec4 Shadow = colors::Transparent; // Shadow color when nothing is happening
	/*************************************************************************************/
	const float Speed = 8.f; // Animation's speed
	/*************************************************************************************/
}