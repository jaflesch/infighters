#include "../common.h"

#define MAX_KEYS 1024

#if 0
#define SHIFT_KEY 16
#define CTRL_KEY 17
#define BACKSPACE_KEY 8
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13
#endif
#define KEY_SHIFT 16
#define KEY_CTRL 17
#define KEY_BACKSPACE 8
#define KEY_LINE_FEED 10
#define KEY_CARR_RETURN 13
#define KEY_ESCAPE 0x1B
#define KEY_SPACE 0x20
#define KEY_F1 0x70
#define KEY_F2 0x71
#define KEY_F3 0x72
#define KEY_F4 0x73
#define KEY_F5 0x74
#define KEY_F6 0x75
#define KEY_F7 0x76
#define KEY_F8 0x77
#define KEY_F9 0x78
#define KEY_F10 0x79
#define KEY_F11 0x7A
#define KEY_F12 0x7B


typedef struct {
	s32 x, y;
	s32 x_left, y_left;
	s32 wheel_value;

	bool left_down;
	bool right_down;
	bool middle_down;

	bool is_captured;
} Mouse_State;

typedef struct {
	bool key[MAX_KEYS];
	bool key_event[MAX_KEYS];
} Keyboard_State;
