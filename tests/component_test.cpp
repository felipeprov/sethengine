#include "gtest/gtest.h"
extern "C"
{
#include "components\components.h"
}

static int g_running_instances = 0;
static int g_updated = 0;
static int g_msgs = 0;

static void dummyCompInit(Component_t* comp_)
{
	g_running_instances++;
}

static void dummyCompDeinit( Component_t* comp_)
{
	g_running_instances--;
}

static void dummyCompOnUpdate( Component_t* comp_, double dt)
{
	g_updated++;
}

static void dummyCompOnMsg( Component_t* comp_)
{
}

static Component_t* dummyComponentCreate(void)
{
	Component_t *ptr = new Component_t();

	ptr->init = dummyCompInit;
	ptr->deinit = dummyCompDeinit;
	ptr->onMsg = dummyCompOnMsg;
	ptr->onUpdate = dummyCompOnUpdate;
	ptr->data = 0;
	ptr->parent = 0;

	return ptr;
}

TEST(Components, CreateComponent)
{
	Component_SystemInit();

	Component_RegisterNew("dummy", dummyComponentCreate);
	Component_t* ptr = 0;
	GameObject_t parent;

	ptr = Component_New(&parent, "dummy");
	ASSERT_NE((int)ptr, 0);
	ASSERT_EQ((int)&parent, (int)ptr->parent);


	Component_Init(ptr);
	ASSERT_EQ(g_running_instances, 1);

	ASSERT_EQ(0, g_updated);
	Component_Update(ptr, 0);
	ASSERT_EQ(1, g_updated);

	Component_Free(ptr);
	ASSERT_EQ(g_running_instances, 0);
}
