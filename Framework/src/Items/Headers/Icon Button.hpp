#include "main.h"
#include "!Items.hpp"

struct Icon_Button_State {
    ImVec4 Text;
};

namespace I1 {
    /*************************************************************************************/
    const ImVec4 Text_Hovered = colors::White; // Icon color when its hovered
    const ImVec4 Text = colors::lwhite; // Icon color when nothing its happening
    /*************************************************************************************/
    const float Speed = 10.f; // Animation's speed
    /*************************************************************************************/
}