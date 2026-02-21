#include "key_mapper.h"
#include <X11/keysym.h>

void KeyMapper::loadCache() {
    // --- Basic & Navigation ---
    symbolCache[XK_space]     = " ";
    symbolCache[XK_Return]    = "⏎";
    symbolCache[XK_BackSpace] = "⌫";
    symbolCache[XK_Tab]       = "⇥";
    symbolCache[XK_Escape]    = "⎋";
    symbolCache[XK_Delete]    = "⌦";
    symbolCache[XK_Home]      = "⤒";
    symbolCache[XK_End]       = "⤓";
    symbolCache[XK_Page_Up]   = "⇞";
    symbolCache[XK_Page_Down] = "⇟";

    symbolCache[XK_Left]      = "←";
    symbolCache[XK_Right]     = "→";
    symbolCache[XK_Up]        = "↑";
    symbolCache[XK_Down]      = "↓";

    symbolCache[XK_KP_Insert]   = "Ins";
    symbolCache[XK_KP_End]      = "End";
    symbolCache[XK_KP_Down]     = "↓";
    symbolCache[XK_KP_Page_Down]= "PgDn";
    symbolCache[XK_KP_Left]     = "←";
    symbolCache[XK_KP_Begin]    = "5";
    symbolCache[XK_KP_Right]    = "→";
    symbolCache[XK_KP_Home]     = "Home";
    symbolCache[XK_KP_Up]       = "↑";
    symbolCache[XK_KP_Page_Up]  = "PgUp";
    symbolCache[XK_KP_Delete]   = "Del";
    symbolCache[XK_KP_Enter]    = "⏎";
    symbolCache[XK_KP_Add]      = "+";
    symbolCache[XK_KP_Subtract] = "-";
    symbolCache[XK_KP_Multiply] = "×";
    symbolCache[XK_KP_Divide]   = "÷";

    numlockCache[XK_KP_Insert]   = "0";
    numlockCache[XK_KP_End]      = "1";
    numlockCache[XK_KP_Down]     = "2";
    numlockCache[XK_KP_Page_Down]= "3";
    numlockCache[XK_KP_Left]     = "4";
    numlockCache[XK_KP_Begin]    = "5";
    numlockCache[XK_KP_Right]    = "6";
    numlockCache[XK_KP_Home]     = "7";
    numlockCache[XK_KP_Up]       = "8";
    numlockCache[XK_KP_Page_Up]  = "9";
    numlockCache[XK_KP_Delete]   = ".";

    symbolCache[XK_F1]  = "F1"; // Tetap teks agar jelas, atau pakai icon
    symbolCache[XK_Print] = "⎙";
    symbolCache[XK_Pause] = "Ⅱ";
}
