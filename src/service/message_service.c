#include "message_service.h"
#include "utils\list.h"
#include <string.h>
#include <stdlib.h>

typedef struct MessageService_t
{
	struct list_head listenners;
}MessageService_t;

typedef struct MsgListenners_t
{
	GameObject_t* dest;
	char* msg_name;
	struct list_head next;
	struct list_head mail_box;
}MsgListenners_t;

typedef struct Msg_t
{
	void* msg;
	struct list_head next;
}Msg_t;

static MessageService_t g_msg_service;
void message_service_init(void)
{
	INIT_LIST_HEAD(&g_msg_service.listenners);
}

void message_service_register_listenner(Component_t* comp, char* msg_name)
{
	MsgListenners_t* listenners;
	struct list_head* p;

	list_for_each(p, &g_msg_service.listenners)
	{
		listenners = list_entry(p,MsgListenners_t, next);
		if(listenners->dest == comp->parent)
		{
			if(strcmp(msg_name, listenners->msg_name) == 0)
			{
				return;
			}
		}
	}

	listenners = (MsgListenners_t*)malloc(sizeof(MsgListenners_t));
	listenners->dest = comp->parent;
	listenners->msg_name = msg_name;
	INIT_LIST_HEAD(&listenners->mail_box);

	list_add(&listenners->next, &g_msg_service.listenners);
}

void message_service_send_msg(GameObject_t* dest, char* msg_name, void* msg)
{
	MsgListenners_t* listenners;
	struct list_head* p;
	Msg_t* m;

	list_for_each(p, &g_msg_service.listenners)
	{
		listenners = list_entry(p,MsgListenners_t, next);
		if(listenners->dest == dest)
		{
			if(strcmp(msg_name, listenners->msg_name) == 0)
			{
				m = (Msg_t*)malloc(sizeof(Msg_t));
				m->msg = msg;
				list_add(&m->next, &listenners->mail_box);
			}
		}
	}
}

void message_dispatch(void)
{
	MsgListenners_t* listenners;
	struct list_head* p, *q, *t;
	Msg_t* msg, *tmp;

	list_for_each(p, &g_msg_service.listenners)
	{
		listenners = list_entry(p,MsgListenners_t, next);
		
		list_for_each_safe(q, t, &listenners->mail_box)
		{
			tmp = list_entry(q, Msg_t, next);

			GameObject_OnMsg(listenners->dest, listenners->msg_name, tmp->msg);

			list_del(q);
			free(tmp);
		}
	}
}