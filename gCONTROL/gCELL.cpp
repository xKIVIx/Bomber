#include "gCELL.h"

gCELL::gCELL(unsigned int x, unsigned int y)
{

}

gCELL::~gCELL()
{
	DeleteObject();
}

void gCELL::SwapObject(gCELL * source)
{
	if (object_ != NULL)
		if (!object_->Colis(source->object_))
			return;
	object_->SetNewCoord(source->x_pos_, source->y_pos_, source->z_pos_);
	source->object_->SetNewCoord(x_pos_, y_pos_, z_pos_);
	gOBJECT * tmp = source->object_;
	source->object_ = object_;
	object_ = tmp;
}

void gCELL::SetObject(gOBJECT * new_object)
{
	DeleteObject();
	object_ = new_object;
}

void gCELL::DeleteObject()
{
	if (object_ != NULL)
		delete object_;
	object_ = NULL;
}
