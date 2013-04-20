#include "gtest\gtest.h"

extern "C"
{
	#include "utils\list.h"
}
typedef struct Dummy_List
{
	int data1;
	int data2;
	list_head list;
	static int counter;
	Dummy_List()
	{
		counter++;
	}

	~Dummy_List()
	{
		counter--;
	}
}Dummy_List;

int Dummy_List::counter = 0;

TEST(List, CreateList)
{
	Dummy_List my_list;
	Dummy_List* tmp;
	list_head* pos, *q;
	int counter = 0;

	INIT_LIST_HEAD(&my_list.list);
	tmp = new Dummy_List;
	list_add(&tmp->list, &my_list.list);
	
	tmp = new Dummy_List;
	list_add(&tmp->list, &my_list.list);

	tmp = new Dummy_List;
	list_add(&tmp->list, &my_list.list);


	ASSERT_EQ(my_list.list.next, &tmp->list);

	list_for_each(pos, &my_list.list){
		counter++;
	}

	ASSERT_EQ(counter, 3);

	list_for_each_safe(pos, q, &my_list.list){
		tmp= list_entry(pos, Dummy_List, list);
		list_del(pos);
		delete tmp;
	}

	ASSERT_EQ(1, Dummy_List::counter);
}