/*
 * Virtual Keys, Standard Set
 */
#define VIRTUALKEY_LBUTTON        0x01
#define VIRTUALKEY_RBUTTON        0x02
#define VIRTUALKEY_CANCEL         0x03
#define VIRTUALKEY_MBUTTON        0x04    /* NOT contiguous with L & RBUTTON */

#if(_WIN32_WINNT >= 0x0500)
#define VIRTUALKEY_XBUTTON1       0x05    /* NOT contiguous with L & RBUTTON */
#define VIRTUALKEY_XBUTTON2       0x06    /* NOT contiguous with L & RBUTTON */
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0x07 : unassigned
 */

#define VIRTUALKEY_BACK           0x08
#define VIRTUALKEY_TAB            0x09

/*
 * 0x0A - 0x0B : reserved
 */

#define VIRTUALKEY_CLEAR          0x0C
#define VIRTUALKEY_RETURN         0x0D

#define VIRTUALKEY_SHIFT          0x10
#define VIRTUALKEY_CONTROL        0x11
#define VIRTUALKEY_MENU           0x12
#define VIRTUALKEY_PAUSE          0x13
#define VIRTUALKEY_CAPITAL        0x14

#define VIRTUALKEY_KANA           0x15
#define VIRTUALKEY_HANGEUL        0x15  /* old name - should be here for compatibility */
#define VIRTUALKEY_HANGUL         0x15
#define VIRTUALKEY_JUNJA          0x17
#define VIRTUALKEY_FINAL          0x18
#define VIRTUALKEY_HANJA          0x19
#define VIRTUALKEY_KANJI          0x19

#define VIRTUALKEY_ESCAPE         0x1B

#define VIRTUALKEY_CONVERT        0x1C
#define VIRTUALKEY_NONCONVERT     0x1D
#define VIRTUALKEY_ACCEPT         0x1E
#define VIRTUALKEY_MODECHANGE     0x1F

#define VIRTUALKEY_SPACE          0x20
#define VIRTUALKEY_PRIOR          0x21
#define VIRTUALKEY_NEXT           0x22
#define VIRTUALKEY_END            0x23
#define VIRTUALKEY_HOME           0x24
#define VIRTUALKEY_LEFT           0x25
#define VIRTUALKEY_UP             0x26
#define VIRTUALKEY_RIGHT          0x27
#define VIRTUALKEY_DOWN           0x28
#define VIRTUALKEY_SELECT         0x29
#define VIRTUALKEY_PRINT          0x2A
#define VIRTUALKEY_EXECUTE        0x2B
#define VIRTUALKEY_SNAPSHOT       0x2C
#define VIRTUALKEY_INSERT         0x2D
#define VIRTUALKEY_DELETE         0x2E
#define VIRTUALKEY_HELP           0x2F

/*
 * VIRTUALKEY_0 - VIRTUALKEY_9 are the same as ASCII '0' - '9' (0x30 - 0x39)
 * 0x40 : unassigned
 * VIRTUALKEY_A - VIRTUALKEY_Z are the same as ASCII 'A' - 'Z' (0x41 - 0x5A)
 */

#define VIRTUALKEY_LWIN           0x5B
#define VIRTUALKEY_RWIN           0x5C
#define VIRTUALKEY_APPS           0x5D

/*
 * 0x5E : reserved
 */

#define VIRTUALKEY_SLEEP          0x5F

#define VIRTUALKEY_NUMPAD0        0x60
#define VIRTUALKEY_NUMPAD1        0x61
#define VIRTUALKEY_NUMPAD2        0x62
#define VIRTUALKEY_NUMPAD3        0x63
#define VIRTUALKEY_NUMPAD4        0x64
#define VIRTUALKEY_NUMPAD5        0x65
#define VIRTUALKEY_NUMPAD6        0x66
#define VIRTUALKEY_NUMPAD7        0x67
#define VIRTUALKEY_NUMPAD8        0x68
#define VIRTUALKEY_NUMPAD9        0x69
#define VIRTUALKEY_MULTIPLY       0x6A
#define VIRTUALKEY_ADD            0x6B
#define VIRTUALKEY_SEPARATOR      0x6C
#define VIRTUALKEY_SUBTRACT       0x6D
#define VIRTUALKEY_DECIMAL        0x6E
#define VIRTUALKEY_DIVIDE         0x6F
#define VIRTUALKEY_F1             0x70
#define VIRTUALKEY_F2             0x71
#define VIRTUALKEY_F3             0x72
#define VIRTUALKEY_F4             0x73
#define VIRTUALKEY_F5             0x74
#define VIRTUALKEY_F6             0x75
#define VIRTUALKEY_F7             0x76
#define VIRTUALKEY_F8             0x77
#define VIRTUALKEY_F9             0x78
#define VIRTUALKEY_F10            0x79
#define VIRTUALKEY_F11            0x7A
#define VIRTUALKEY_F12            0x7B
#define VIRTUALKEY_F13            0x7C
#define VIRTUALKEY_F14            0x7D
#define VIRTUALKEY_F15            0x7E
#define VIRTUALKEY_F16            0x7F
#define VIRTUALKEY_F17            0x80
#define VIRTUALKEY_F18            0x81
#define VIRTUALKEY_F19            0x82
#define VIRTUALKEY_F20            0x83
#define VIRTUALKEY_F21            0x84
#define VIRTUALKEY_F22            0x85
#define VIRTUALKEY_F23            0x86
#define VIRTUALKEY_F24            0x87

/*
 * 0x88 - 0x8F : unassigned
 */

#define VIRTUALKEY_NUMLOCK        0x90
#define VIRTUALKEY_SCROLL         0x91

/*
 * NEC PC-9800 kbd definitions
 */
#define VIRTUALKEY_OEM_NEC_EQUAL  0x92   // '=' key on numpad

/*
 * Fujitsu/OASYS kbd definitions
 */
#define VIRTUALKEY_OEM_FJ_JISHO   0x92   // 'Dictionary' key
#define VIRTUALKEY_OEM_FJ_MASSHOU 0x93   // 'Unregister word' key
#define VIRTUALKEY_OEM_FJ_TOUROKU 0x94   // 'Register word' key
#define VIRTUALKEY_OEM_FJ_LOYA    0x95   // 'Left OYAYUBI' key
#define VIRTUALKEY_OEM_FJ_ROYA    0x96   // 'Right OYAYUBI' key

/*
 * 0x97 - 0x9F : unassigned
 */

/*
 * VIRTUALKEY_L* & VIRTUALKEY_R* - left and right Alt, Ctrl and Shift virtual keys.
 * Used only as parameters to GetAsyncKeyState() and GetKeyState().
 * No other API or message will distinguish left and right keys in this way.
 */
#define VIRTUALKEY_LSHIFT         0xA0
#define VIRTUALKEY_RSHIFT         0xA1
#define VIRTUALKEY_LCONTROL       0xA2
#define VIRTUALKEY_RCONTROL       0xA3
#define VIRTUALKEY_LMENU          0xA4
#define VIRTUALKEY_RMENU          0xA5

#if(_WIN32_WINNT >= 0x0500)
#define VIRTUALKEY_BROWSER_BACK        0xA6
#define VIRTUALKEY_BROWSER_FORWARD     0xA7
#define VIRTUALKEY_BROWSER_REFRESH     0xA8
#define VIRTUALKEY_BROWSER_STOP        0xA9
#define VIRTUALKEY_BROWSER_SEARCH      0xAA
#define VIRTUALKEY_BROWSER_FAVORITES   0xAB
#define VIRTUALKEY_BROWSER_HOME        0xAC

#define VIRTUALKEY_VOLUME_MUTE         0xAD
#define VIRTUALKEY_VOLUME_DOWN         0xAE
#define VIRTUALKEY_VOLUME_UP           0xAF
#define VIRTUALKEY_MEDIA_NEXT_TRACK    0xB0
#define VIRTUALKEY_MEDIA_PREV_TRACK    0xB1
#define VIRTUALKEY_MEDIA_STOP          0xB2
#define VIRTUALKEY_MEDIA_PLAY_PAUSE    0xB3
#define VIRTUALKEY_LAUNCH_MAIL         0xB4
#define VIRTUALKEY_LAUNCH_MEDIA_SELECT 0xB5
#define VIRTUALKEY_LAUNCH_APP1         0xB6
#define VIRTUALKEY_LAUNCH_APP2         0xB7

#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xB8 - 0xB9 : reserved
 */

#define VIRTUALKEY_OEM_1          0xBA   // ';:' for US
#define VIRTUALKEY_OEM_PLUS       0xBB   // '+' any country
#define VIRTUALKEY_OEM_COMMA      0xBC   // ',' any country
#define VIRTUALKEY_OEM_MINUS      0xBD   // '-' any country
#define VIRTUALKEY_OEM_PERIOD     0xBE   // '.' any country
#define VIRTUALKEY_OEM_2          0xBF   // '/?' for US
#define VIRTUALKEY_OEM_3          0xC0   // '`~' for US

/*
 * 0xC1 - 0xD7 : reserved
 */

/*
 * 0xD8 - 0xDA : unassigned
 */

#define VIRTUALKEY_OEM_4          0xDB  //  '[{' for US
#define VIRTUALKEY_OEM_5          0xDC  //  '\|' for US
#define VIRTUALKEY_OEM_6          0xDD  //  ']}' for US
#define VIRTUALKEY_OEM_7          0xDE  //  ''"' for US
#define VIRTUALKEY_OEM_8          0xDF

/*
 * 0xE0 : reserved
 */

/*
 * Various extended or enhanced keyboards
 */
#define VIRTUALKEY_OEM_AX         0xE1  //  'AX' key on Japanese AX kbd
#define VIRTUALKEY_OEM_102        0xE2  //  "<>" or "\|" on RT 102-key kbd.
#define VIRTUALKEY_ICO_HELP       0xE3  //  Help key on ICO
#define VIRTUALKEY_ICO_00         0xE4  //  00 key on ICO

#if(WINVER >= 0x0400)
#define VIRTUALKEY_PROCESSKEY     0xE5
#endif /* WINVER >= 0x0400 */

#define VIRTUALKEY_ICO_CLEAR      0xE6


#if(_WIN32_WINNT >= 0x0500)
#define VIRTUALKEY_PACKET         0xE7
#endif /* _WIN32_WINNT >= 0x0500 */

/*
 * 0xE8 : unassigned
 */

/*
 * Nokia/Ericsson definitions
 */
#define VIRTUALKEY_OEM_RESET      0xE9
#define VIRTUALKEY_OEM_JUMP       0xEA
#define VIRTUALKEY_OEM_PA1        0xEB
#define VIRTUALKEY_OEM_PA2        0xEC
#define VIRTUALKEY_OEM_PA3        0xED
#define VIRTUALKEY_OEM_WSCTRL     0xEE
#define VIRTUALKEY_OEM_CUSEL      0xEF
#define VIRTUALKEY_OEM_ATTN       0xF0
#define VIRTUALKEY_OEM_FINISH     0xF1
#define VIRTUALKEY_OEM_COPY       0xF2
#define VIRTUALKEY_OEM_AUTO       0xF3
#define VIRTUALKEY_OEM_ENLW       0xF4
#define VIRTUALKEY_OEM_BACKTAB    0xF5

#define VIRTUALKEY_ATTN           0xF6
#define VIRTUALKEY_CRSEL          0xF7
#define VIRTUALKEY_EXSEL          0xF8
#define VIRTUALKEY_EREOF          0xF9
#define VIRTUALKEY_PLAY           0xFA
#define VIRTUALKEY_ZOOM           0xFB
#define VIRTUALKEY_NONAME         0xFC
#define VIRTUALKEY_PA1            0xFD
#define VIRTUALKEY_OEM_CLEAR      0xFE