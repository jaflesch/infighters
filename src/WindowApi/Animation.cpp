#include "Animation.h"
#include <assert.h>

Animation::Animation(Texture* texture, int num_rows, int num_cols, int num_indices, int frame_speed) 
	: m_frame_speed(frame_speed / 8), m_num_indices(num_indices), m_num_rows(num_rows), m_num_cols(num_cols), m_texture(texture)
{
	assert(num_rows > 0);
	assert(num_cols > 0);
	assert(num_indices > 0);

	m_offset = hm::vec2(0, (float)(num_rows - 1));
	m_frame_pos = 0;
	m_remaining_frames = m_frame_speed;
	m_rows_cols = hm::vec2((float)num_cols, (float)num_rows);
	state_offsets = (hm::vec2*)calloc(num_indices, sizeof(hm::vec2));
	valid = true;
	forward = true;
	animate_reverse = false;
}

Animation::Animation(char* texture_filepath, int num_rows, int num_cols, int num_indices, int frame_speed)
	: Animation(new Texture(texture_filepath), num_rows, num_cols, num_indices, frame_speed)
{
}

bool Animation::animate() {
	bool ended = false;
	if (animate_reverse) {
		if (m_remaining_frames > 0)
			m_remaining_frames--;
		else {
			m_remaining_frames = m_frame_speed;
			if (forward)
				m_frame_pos += 1;
			else
				m_frame_pos -= 1;
			if (m_frame_pos >= m_num_indices) {
				m_frame_pos--;
				forward = !forward;
			}
			else if (m_frame_pos < 0) {
				m_frame_pos++;
				forward = !forward;
				ended = true;
			}
			m_offset = state_offsets[m_frame_pos];
		}
	} else {
		if (m_remaining_frames > 0)
			m_remaining_frames--;
		else {
			m_remaining_frames = m_frame_speed;
			m_frame_pos += 1;
			if (m_frame_pos >= m_num_indices) {
				m_frame_pos = 0;
				ended = true;
			}
			m_offset = state_offsets[m_frame_pos];
		}
	}
	return ended;
}