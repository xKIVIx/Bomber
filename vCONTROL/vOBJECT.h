#pragma once
#ifndef _vOBJECT_HEADER
#define _vOBJECT_HEADER

struct vOBJECT
{
	unsigned int texture_id_ =0,
		sprite_now_=0,
		vertex_id_=0,
		id_sprite_mass_=0, 
		dir_angl_=0;
	float x_pos_=0, y_pos_=0, z_pos_=0;
	bool game_object = 1;
	float scale_cof_x, scale_cof_y;
};
#endif