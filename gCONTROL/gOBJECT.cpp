#include "gOBJECT.h"
bool gOBJECT::Destroy(unsigned int * power_fire)
{
	if (*power_fire >= def)
	{
		*power_fire -= def;
		return true;
	}
	*power_fire = 0;
	return false;
}
vOBJECT gOBJECT::GetRendInfo()
{
	return render_info_;
}
void gOBJECT::SetNewCoord(float x, float y, float z)
{
	render_info_.x_pos_ = x;
	render_info_.y_pos_ = y;
	render_info_.y_pos_ = z;
}
gOBJECT_WALL::gOBJECT_WALL(TYPE_WALL type_wall, float x_pos, float y_pos, float z_pos)
{
	render_info_.dir_angl_ = 0;
	render_info_.x_pos_ = x_pos;
	render_info_.y_pos_ = y_pos;
	render_info_.z_pos_ = z_pos;

	if (type_wall == TYPE_WALL::brick)
	{
		def = 3;
		render_info_.recurce_id_ = 1;
	}
	else
	{
		def = 4000;
		render_info_.recurce_id_ = 3;
	}
}
bool gOBJECT_WALL::Colis(gOBJECT * contact_object)
{

}

bool gOBJECT_PERSON::Colis(gOBJECT * contact_object)
{
}
