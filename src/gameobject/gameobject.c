#include "gameobject.h"
#include "components\components.h"
#include <stdlib.h>
#include "utils\list.h"
#include <string.h>

typedef struct Component_Node_t
{
	char* name;
	Component_t* ptr_comp;
	struct list_head next;
}Component_Node_t;

typedef struct Attribute_node_t
{
	char* name;
	Attribute_t* ptr_attr;
	struct list_head next;
}Attribute_node_t;


GameObject_t* GameObjectNew(void)
{
	GameObject_t* obj;
	obj = (GameObject_t*)(malloc(sizeof(GameObject_t)));
	INIT_LIST_HEAD(&obj->attribute_list);
	INIT_LIST_HEAD(&obj->component_list);

	return obj;
}

void GameObject_AddComponent(GameObject_t* obj_, Component_t* comp_, char* name_)
{
	Component_Node_t* c_node;

	c_node = (Component_Node_t*)(malloc(sizeof(Component_Node_t)));
	c_node->name = name_;
	c_node->ptr_comp = comp_;
	comp_->parent = obj_;

	list_add(&c_node->next, &obj_->component_list);

	comp_->init(comp_);
}

void GameObject_AddAttribute(GameObject_t* obj_, Attribute_t* attr_, char* name_)
{
	Attribute_node_t* a_node;

	a_node = (Attribute_node_t*)(malloc(sizeof(Attribute_node_t)));
	a_node->name = name_;
	a_node->ptr_attr = attr_;

	list_add(&a_node->next, &obj_->attribute_list);
}

void GameObject_Update(GameObject_t* obj_, double dt_)
{
	struct list_head* pos, *q;
	Component_Node_t* comp_node;

	list_for_each(pos, &obj_->component_list, next)
	{
		comp_node = list_entry(pos, Component_Node_t, next);
		Component_Update(comp_node->ptr_comp, dt_);
	}
}

struct Attribute_t* GameObject_QueryAttribute(struct GameObject_t* obj_, char* name_)
{
	struct list_head* pos, *q;
	Attribute_node_t* att_node;
	Attribute_t* att = 0;

	list_for_each(pos, &obj_->attribute_list, next)
	{
		att_node = list_entry(pos, Attribute_node_t, next);
		if(strcmp(name_, att_node->name) == 0)
		{
			att = att_node->ptr_attr;
			break;
		}
	}

	return att;
}

void GameObject_OnMsg(struct GameObject_t* obj, char* msg_name, void* msg)
{
	Component_Node_t* node;
	struct list_head* q;

	list_for_each(q,&obj->component_list)
	{
		node = list_entry(q, Component_Node_t, next);
		node->ptr_comp->onMsg(node->ptr_comp, msg_name, msg);
	}
}

void GameObject_SetAttributeInt(struct GameObject_t* obj_, char* att_name, int x)
{
	Attribute_t* t = GameObject_QueryAttribute(obj_, att_name);

	if(t)
	{
		int* v = Attribute_ToInteger(t);
		if(v)
		{
			*v = x;
		}
	}
}

void GameObject_SetAttributeDouble(struct GameObject_t* obj_, char* att_name, double x)
{
	Attribute_t* t = GameObject_QueryAttribute(obj_, att_name);

	if(t)
	{
		double* v = Attribute_ToDouble(t);
		if(v)
		{
			*v = x;
		}
	}
}

void GameObject_SetAttributeVector2d(struct GameObject_t* obj_, char* att_name, float x, float y)
{
	Attribute_t* t = GameObject_QueryAttribute(obj_, att_name);

	if(t)
	{
		Vector2d_t* v = Attribute_ToVector2d(t);
		if(v)
		{
			v->x = x;
			v->y = y;
		}
	}
}

void GameObject_SetAttributeString(struct GameObject_t* obj_, char* att_name, char* str)
{
	Attribute_t* t = GameObject_QueryAttribute(obj_, att_name);

	if(t)
	{
		Attribute_UpdateString(t, str);
	}
}

void GameObject_SetAttributePointer(struct GameObject_t* obj_, char* att_name, void* ptr)
{
	Attribute_t* t = GameObject_QueryAttribute(obj_, att_name);

	if(t)
	{
		Attribute_SetPointer(t, ptr);
	}
	else
	{
		t = Attribute_NewPointer(ptr);
		GameObject_AddAttribute(obj_, t, att_name);
	}
}

void GameObject_Refresh(struct GameObject_t* obj_)
{
	struct list_head* pos, *q;
	Component_Node_t* comp_node;

	list_for_each(pos, &obj_->component_list, next)
	{
		comp_node = list_entry(pos, Component_Node_t, next);
		Component_Refresh(comp_node->ptr_comp);
	}
}
