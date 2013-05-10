
#include "colision_service.h"
#include "attributes\attributes.h"
#include "gameobject\gameobject.h"
#include "utils\list.h"
#include <stdlib.h>
#include <string.h>
#include "message_service.h"

typedef struct ColisionObject_t
{
	Component_t* comp;	
	Vector2d_t* position;
	Vector2d_t* size;
	double* rotation;
	struct list_head next;
}ColisionObject_t;

typedef struct ColisionList_t
{
	char* filter;
	struct list_head objs;
	struct list_head next;
}ColisionList_t;

static struct list_head g_colisionables;

void colision_service_init(void)
{
	INIT_LIST_HEAD(&g_colisionables);
}

static void add_to_list(ColisionObject_t* obj, char* filter)
{
	struct list_head* q;
	ColisionList_t* collision_list;

	list_for_each(q, &g_colisionables)
	{
		collision_list = list_entry(q, ColisionList_t, next);
		if(strcmp(filter, collision_list->filter)==0)
		{
			list_add(&obj->next, &collision_list->objs);
			return;
		}
	}

	collision_list = (ColisionList_t*)malloc(sizeof(ColisionList_t));
	collision_list->filter = filter;
	INIT_LIST_HEAD(&collision_list->objs);
	list_add(&collision_list->next, &g_colisionables);
	list_add(&obj->next, &collision_list->objs);
}

static void removeFromList(Component_t *comp, ColisionList_t* c_list)
{
	struct list_head* q;
	struct list_head* n;
	ColisionObject_t* obj;

	list_for_each_safe(q, n,  &c_list->objs)
	{
		obj = list_entry(q, ColisionObject_t, next);
		
		if(obj->comp == comp)
		{
			list_del(q);
			free(obj);
		}
	}

}

static void removeFromAllList(Component_t *comp)
{
	struct list_head* q;
	struct list_head* n;
	ColisionList_t* collision_list;

	list_for_each_safe(q, n,  &g_colisionables)
	{
		collision_list = list_entry(q, ColisionList_t, next);
		removeFromList(comp, collision_list);
	}
}

void colision_service_register(Component_t* comp, char* filter)
{
	ColisionObject_t* c_obj = (ColisionObject_t*)malloc(sizeof(ColisionObject_t));
	Attribute_t* att;

	memset(c_obj, 0, sizeof(ColisionObject_t));
	
	c_obj->comp = comp;
	
	att = GameObject_QueryAttribute(comp->parent, "position");
	c_obj->position = Attribute_ToVector2d(att);

	att = GameObject_QueryAttribute(comp->parent, "size");
	c_obj->size = Attribute_ToVector2d(att);

	att = GameObject_QueryAttribute(comp->parent, "rotation");
	c_obj->rotation = Attribute_ToDouble(att);

	add_to_list(c_obj, filter);
}

void colision_service_unregister(Component_t* comp)
{
	removeFromAllList(comp);
}

void colision_service_send_msg_neighbors(Component_t* comp_, char* msg, void* value)
{
}

static int checkColision(ColisionObject_t* obj1, ColisionObject_t* obj2)
{
	double left1, left2;
	double right1, right2;
	double top1, top2;
	double bottom1, bottom2;

	left1 = obj1->position->x;
	left2 = obj2->position->x;
	right1 = obj1->position->x + obj1->size->x;
	right2 = obj2->position->x + obj2->size->x;
	top1 = obj1->position->y;
	top2 = obj2->position->y;
	bottom1 = obj1->position->y + obj1->size->y;
	bottom2 = obj2->position->y + obj2->size->y;

	if (bottom1 < top2) return(0);
	if (top1 > bottom2) return(0);

	if (right1 < left2) return(0);
	if (left1 > right2) return(0);

	return(1);
}

static void testCollisionObjects(ColisionList_t* list)
{
	struct list_head *p, *q;
	ColisionObject_t* obj, *obj2;

	list_for_each(p, &list->objs)
	{
		obj = list_entry(p, ColisionObject_t, next);

		for(q = p->next; q!= &list->objs; q = q->next)
		{
			obj2 = list_entry(q, ColisionObject_t, next);

			if(checkColision(obj, obj2))
			{
				message_service_send_msg(obj->comp->parent, "collision", obj2);
				message_service_send_msg(obj2->comp->parent, "collision", obj);
			}
		}
	}
}
void colision_service_update(double dt)
{
	struct list_head* p;
	ColisionList_t* lists;
	
	list_for_each(p, &g_colisionables)
	{
		lists = list_entry(p, ColisionList_t, next);
		testCollisionObjects(lists);
	}
}