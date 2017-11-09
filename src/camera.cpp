struct Camera {
	mat4 view_matrix;
	mat4 projection_matrix;

	hm::vec3 cam_position;
	hm::vec3 cam_orientation;
	hm::vec3 cam_up;

	bool locked;

	float current_speed;
	float current_turn_speed;

	float fov;
	float far_plane;
	float near_plane;

	void set_cam_position(hm::vec3 newpos) {
		cam_position = newpos;
		view_matrix = mat4::look_at(newpos, cam_orientation, cam_up);
	}
	void set_cam_orientation(hm::vec3 newori) {
		cam_orientation = newori;
		view_matrix = mat4::look_at(cam_position, cam_position + newori, cam_up);
	}

	void move_forward_and_back(float amt) {
		set_cam_position(cam_position + (cam_orientation * amt));
		set_cam_orientation(cam_orientation);
	}
	void move_sideways(float amt) {
		hm::vec3 left = hm::vec3::normalize(hm::vec3::cross(cam_up, cam_orientation));
		set_cam_position(cam_position + (left * amt));
		set_cam_orientation(cam_orientation);
	}

	void rotate_x(float amt) {
		mat4 rot = mat4::rotate(cam_up, amt);
		hm::vec3 normalized_ori = hm::vec3::normalize(cam_orientation);
		normalized_ori = rot * normalized_ori;
		set_cam_orientation(normalized_ori);
	}

	void rotate_y(float amt) {
		mat4 rot = mat4::rotate(hm::vec3::cross(cam_orientation, cam_up), amt);
		hm::vec3 normalized_ori = hm::vec3::normalize(cam_orientation);
		normalized_ori = rot * normalized_ori;
		set_cam_orientation(normalized_ori);
	}
};

void init_camera(Camera* camera, float aspect, float fov, float znear, float zfar) {
	hm::vec3 campos(0, 0, 0);
	hm::vec3 camori(0, 0, -1);
	hm::vec3 up(0, 1, 0);

	camera->view_matrix = hm::mat4::look_at(campos, camori, up);
	camera->projection_matrix = hm::mat4::perspective(fov, aspect, znear, zfar);
	camera->fov = fov;
	camera->far_plane = zfar;
	camera->near_plane = znear;

	camera->cam_position = campos;
	camera->set_cam_orientation(camori);
	camera->cam_up = up;
	camera->locked = false;
}

extern Keyboard_State keyboard_state;
extern Mouse_State mouse_state;

void mouse_move_event(Camera* camera, int valuex, int valuey)
{
	if (!win_state.move_camera) return;
#define TURNSPEED -0.1f

	float x = (2.0f * valuex) / win_state.win_width - 1;
	float y = (2.0f * valuey) / win_state.win_height - 1;

	float amtx = valuex * TURNSPEED;
	float amty = valuey * TURNSPEED;

	camera->rotate_x(amtx);
	camera->rotate_y(amty);
}

void input_camera(Camera* camera)
{
	if (!win_state.do_input) return;
	float CAMSPEED = 65.0f;
	float CAMTURNSPEED = 30.0f;

	if (keyboard_state.key[VK_SHIFT]) {
		CAMSPEED = 10.0f;
		CAMTURNSPEED = 6.0f;
	}

	if (keyboard_state.key['W']) {
		camera->current_speed = CAMSPEED;
	} else if (keyboard_state.key['S']) {
		camera->current_speed = -CAMSPEED;
	} else {
		camera->current_speed = 0;
	}

	if (keyboard_state.key['A']) {
		camera->current_turn_speed = CAMTURNSPEED;
	} else if (keyboard_state.key['D']) {
		camera->current_turn_speed = -CAMTURNSPEED;
	} else {
		camera->current_turn_speed = 0;
	}

	if (mouse_state.is_captured) {
		mouse_move_event(camera, mouse_state.x - mouse_state.x_left, mouse_state.y - mouse_state.y_left);
		mouse_state.x_left = mouse_state.x;
		mouse_state.y_left = mouse_state.y;
	}

	float aspect = (float)win_state.win_width / win_state.win_height;
	camera->projection_matrix = mat4::perspective(camera->fov, aspect, camera->near_plane, camera->far_plane);

	camera->move_forward_and_back(camera->current_speed * (1.0f / 60.0f));
	camera->move_sideways(camera->current_turn_speed * (1.0f / 60.0f));
}