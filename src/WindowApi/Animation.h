#pragma once
#include <hmath.h>
#include "../ResourceLoad/Texture.h"

class Animation {
public:
	Animation(Texture* texture, int num_rows, int num_cols, int num_indices, int frame_speed);
	Animation(char* texture_filepath, int num_rows, int num_cols, int num_indices, int frame_speed);
	bool animate();	// returns true when finished animating
	hm::vec2* state_offsets;

	hm::vec2 m_rows_cols;
	hm::vec2 m_offset;

	int m_frame_speed;
	int m_num_indices;
	int m_num_rows;
	int m_num_cols;
	bool valid;
	bool animate_reverse;
private:
	bool forward;

	int m_frame_pos;
	int m_remaining_frames;

public:
	Texture* m_texture;
};