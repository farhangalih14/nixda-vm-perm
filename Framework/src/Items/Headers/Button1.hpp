#include "main.h"

struct Button1_State {
	ImVec4 ButtonBG;
	ImVec4 ButtonBG2;
	ImVec4 Text;
};

namespace B1 {
	/*************************************************************************************/
	const ImVec4 BG_Active = colors::MainColor; // Background color when its active
	const ImVec4 BG_Hovered = colors::MainColor; // Background color when its hovered
	const ImVec4 BG = colors::SecondColor; // Background color when nothing its happening
	/*************************************************************************************/
	const ImVec4 BG_Active2 = colors::MainColor; // Background color when its active
	const ImVec4 BG_Hovered2 = colors::SecondColor; // Background color when its hovered
	const ImVec4 BG2 = colors::SecondColor; // Background color when nothing its happening
	/*************************************************************************************/
	const ImVec4 Text_Active = colors::White; // Text color when its active
	const ImVec4 Text_Hovered = colors::lwhite; // Text color when its hovered
	const ImVec4 Text = colors::lwhite; // Text color when nothing its happening
	/*************************************************************************************/
	const float Rounding = 4.f; // Button's rounding
	const float Speed = 4.f; // Animation's speed
	/*************************************************************************************/
}