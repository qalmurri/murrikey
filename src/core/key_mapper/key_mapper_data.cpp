#include "key_mapper.h"
#include <X11/keysym.h>
#include <X11/Xlib.h>

QMap<unsigned long, KeyMapper::KeyEntry> KeyMapper::keyCache;
QMap<QString, QString> KeyMapper::specialCharMap;
QSet<unsigned long> KeyMapper::modifiers;

void KeyMapper::loadCache() {
    if (!keyCache.isEmpty()) return;

    modifiers = {XK_Control_L, XK_Control_R,
                 XK_Shift_L, XK_Shift_R,
                 XK_Alt_L, XK_Alt_R,
                 XK_Super_L};

    keyCache[XK_space]     = {" ", " "};
    keyCache[XK_Return]    = {"⏎", "⏎"};
    keyCache[XK_BackSpace] = {"⌫", "⌫"};
    keyCache[XK_Tab]       = {"⇥", "⇥"};
    keyCache[XK_Escape]    = {"⎋", "⎋"};
    keyCache[XK_Delete]    = {"⌦", "."};
    keyCache[XK_Home]      = {"⤒", "7"};
    keyCache[XK_End]       = {"⤓", "1"};
    keyCache[XK_Page_Up]   = {"⇞", "9"};
    keyCache[XK_Page_Down] = {"⇟", "3"};

    keyCache[XK_Left]      = {"←", "4"};
    keyCache[XK_Right]     = {"→", "6"};
    keyCache[XK_Up]        = {"↑", "8"};
    keyCache[XK_Down]      = {"↓", "2"};

    keyCache[XK_KP_Insert]   = {"Ins", "0"};
    keyCache[XK_KP_End]      = {"End", "1"};
    keyCache[XK_KP_Down]     = {"↓", "2"};
    keyCache[XK_KP_Page_Down]= {"PgDn", "3"};
    keyCache[XK_KP_Left]     = {"←", "4"};
    keyCache[XK_KP_Begin]    = {"5", "5"};
    keyCache[XK_KP_Right]    = {"→", "6"};
    keyCache[XK_KP_Home]     = {"Home", "7"};
    keyCache[XK_KP_Up]       = {"↑", "8"};
    keyCache[XK_KP_Page_Up]  = {"PgUp", "9"};
    keyCache[XK_KP_Delete]   = {"Del", "."};
    keyCache[XK_KP_Enter]    = {"⏎", "⏎"};
    keyCache[XK_KP_Add]      = {"+", "+"};
    keyCache[XK_KP_Subtract] = {"-", "-"};
    keyCache[XK_KP_Multiply] = {"×", "×"};
    keyCache[XK_KP_Divide]   = {"÷", "÷"};

    keyCache[XK_F1]  = {"F1", "F1"};
    keyCache[XK_Print] = {"⎙", "⎙"};
    keyCache[XK_Pause] = {"Ⅱ", "Ⅱ"};

    keyCache[XK_exclam]      = {"!", "!"};
    keyCache[XK_at]          = {"@", "@"};
    keyCache[XK_numbersign]  = {"#", "#"};
    keyCache[XK_dollar]      = {"$", "$"};
    keyCache[XK_percent]     = {"%", "%"};
    keyCache[XK_asciicircum] = {"^", "^"};
    keyCache[XK_ampersand]   = {"&", "&"};
    keyCache[XK_asterisk]    = {"*", "*"};
    keyCache[XK_parenleft]   = {"(", "("};
    keyCache[XK_parenright]  = {")", ")"};
    keyCache[XK_underscore]  = {"_", "_"};
    keyCache[XK_plus]        = {"+", "+"};

    specialCharMap["bracketleft"]  = "[";
    specialCharMap["bracketright"] = "]";
    specialCharMap["semicolon"]    = ";";
    specialCharMap["apostrophe"]   = "'";
    specialCharMap["comma"]        = ",";
    specialCharMap["period"]       = ".";
    specialCharMap["slash"]        = "/";
    specialCharMap["backslash"]    = "\\";
    specialCharMap["minus"]        = "-";
    specialCharMap["equal"]        = "=";
    specialCharMap["grave"]        = "`";
}
