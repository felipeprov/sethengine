#include "gameobject.h"
#include "components\components.h"
#include <stdlib.h>
#include "utils\list.h"

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

void GameObject_AddComponent(GameObject_t* obj_, Component_t* comp_, char* name_)
{
	Component_Node_t* c_node;

	c_node = (Component_Node_t*)(malloc(sizeof(Component_Node_t)));
	c_node->name = name_;
	c_node->ptr_comp = comp_;

	list_add(&obj_->component_list, &c_node->next);
}

void GameObject_AddAttribute(GameObject_t* obj_, Attribute_t* attr_, char* name_)
{
	Attribute_node_t* a_node;

	a_node = (Attribute_node_t*)(malloc(sizeof(Attribute_node_t)));
	a_node->name = name_;
	a_node->ptr_attr = attr_;

	list_add(&obj_->component_list, &a_node->next);
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