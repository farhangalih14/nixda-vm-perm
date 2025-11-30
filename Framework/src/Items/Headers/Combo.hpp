#include "main.h"
#include "!Items.hpp"

struct Combo_State {
	ImVec4 Combo_BG;
    ImVec4 Border_Color;
	ImVec4 Text_Color;
};

namespace C1 {
    const ImVec4 c = ImVec4(colors::lbg.x, colors::lbg.y, colors::lbg.z, colors::lbg.w * 2);
    /*************************************************************************************/
    const ImVec4 BG_Active = c; // Background color when its active
    const ImVec4 BG_Hovered = c; // Background color when its hovered
    const ImVec4 BG = c; // Background color when nothing its happening
    /*************************************************************************************/
    const ImVec4 Text_Active = colors::MainColor; // Text color when its active
    const ImVec4 Text_Hovered = colors::lwhite; // Text color when its hovered
    const ImVec4 Text = colors::Gray2; // Text color when nothing its happening
    /*************************************************************************************/
    const ImVec4 Border_Active = colors::Gray; // Border color when its active
    const ImVec4 Border_Hovered = colors::Gray; // Border color when its hovered
    const ImVec4 Border = colors::Gray; // Border color when nothing is happening
    /*************************************************************************************/
    const float Speed = 10.f; // Animation's speed
    const float Rounding = 3.f; // Animation's speed
    /*************************************************************************************/
}