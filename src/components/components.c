
#include "utils/list.h"
#include "components.h"
#include <stdlib.h>
#include <string.h>

typedef struct ComponentInitializer_t
{
	char name[32];
	createComponentFunction function;
	struct list_head list;
}ComponentInitializer_t;

static Component_t* Component_AllocInit(void);

static ComponentInitializer_t g_components_head;

void Component_SystemInit(void)
{
	INIT_LIST_HEAD(&g_components_head.list);
}

static Component_t* Component_AllocInit(void)
{
	Component_t* tmp = (Component_t*)malloc(sizeof(Component_t));
	memset(tmp, 0, sizeof(Component_t));

	return tmp;
}

void Component_Free(Component_t* comp_)
{
	if(comp_->deinit)
	{
		comp_->deinit(comp_);
	}

	free(comp_);
}

void Component_Copy(Component_t* dest_, Component_t *orig_)
{
	memcpy(dest_, orig_, sizeof(Component_t));
}

void Component_Update(Component_t* comp_, double dt)
{
	if(comp_->onUpdate)
	{
		comp_->onUpdate(comp_, dt);
	}
}

void Component_Init(Component_t* comp_)
{
	if(comp_->init)
	{
		comp_->init(comp_);
	}
}

void Component_RegisterNew(char* name_, createComponentFunction func_)
{
	ComponentInitializer_t* comp =(ComponentInitializer_t*) malloc(sizeof(ComponentInitializer_t));
	strcpy(comp->name, name_);
	comp->function = func_;

	list_add(&comp->list, &g_components_head.list);
}

Component_t* Component_New(GameObject_t* parent, char* name_)
{
	struct list_head* pos;
	ComponentInitializer_t* comp_initializer;
	Component_t* comp = 0;
	list_for_each(pos, &g_components_head.list)
	{
		comp_initializer= list_entry(pos, ComponentInitializer_t, list);

		if(strcmp(comp_initializer->name, name_) == 0)
		{
			comp = comp_initializer->function();
			comp->parent = parent;
			break;
		}
	}

	return comp;
}
