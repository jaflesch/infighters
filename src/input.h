#include "common.h"

#define MAX_KEYS 1024

#define SHIFT_KEY 16
#define CTRL_KEY 17
#define BACKSPACE_KEY 8
#define LINE_FEED_KEY 10
#define CARRIAGE_RETURN_KEY 13

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
