
#include "renderService.h"
#include "utils\list.h"
#include <stdlib.h>
#include "gameobject\gameobject.h"

typedef struct RenderServiceNode_t
{
	struct list_head next;
	GameObject_t *obj;
	int*		layer;
	renderCallback cb;

	void* arg;
}RenderServiceNode_t;

static struct list_head g_render_service_root;
static struct list_head g_render_service_gui_root;

static inline void list_sort(struct list_head *head, int (*cmp)(struct list_head *a, struct list_head *b)) 
{ 
	struct list_head *p, *q, *e, *list, *tail, *oldhead; 
	int insize, nmerges, psize, qsize, i; 
	list = head->next; 
	list_del(head); 
	insize = 1; 
	for (;;) { 
		p = oldhead = list; 
		list = tail = 0; 
		nmerges = 0; 
		while (p) { 
			nmerges++; 
			q = p; 
			psize = 0; 
			for (i = 0; i < insize; i++) { 
				psize++; 
				q = q->next == oldhead ? 0 : q->next; 
				if (!q) 
					break; 
			} 
			qsize = insize; 
			while (psize > 0 || (qsize > 0 && q)) { 
				if (!psize) { 
					e = q; 
					q = q->next; 
					qsize--; 
					if (q == oldhead) 
						q = 0; 
				} else if (!qsize || !q) { 
					e = p; 
					p = p->next; 
					psize--; 
					if (p == oldhead) 
						p = 0; 
				} else if (cmp(p, q) <= 0) { 
					e = p; 
					p = p->next; 
					psize--; 
					if (p == oldhead) 
						p = 0; 
				} else { 
					e = q; 
					q = q->next; 
					qsize--; 
					if (q == oldhead) 
						q = 0; 
				} 
				if (tail) 
					tail->next = e; 
				else 
					list = e; 
				e->prev = tail; 
				tail = e; 
			} 
			p = q; 
		} 
		tail->next = list; 
		list->prev = tail; 
		if (nmerges <= 1) 
			break; 
		insize *= 2; 
	} 
	head->next = list; 
	head->prev = list->prev; 
	list->prev->next = head; 
	list->prev = head; 
}


void render_service_init(void)
{
	INIT_LIST_HEAD(&g_render_service_root);
	INIT_LIST_HEAD(&g_render_service_gui_root);
}

void render_service_add_listenner(GameObject_t *obj, renderCallback cb, void* arg)
{
	Attribute_t *att;
	RenderServiceNode_t* node = (RenderServiceNode_t*)malloc(sizeof(RenderServiceNode_t));
	node->arg = arg;
	node->cb = cb;
	node->obj = obj;

	att = GameObject_QueryAttribute(obj, "layer");
	node->layer = Attribute_ToInteger(att);

	list_add_tail(&node->next, &g_render_service_root);

	render_service_sort();
}

void render_service_add_listenner_gui(GameObject_t *obj, renderCallback cb, void* arg)
{
	Attribute_t *att;
	RenderServiceNode_t* node = (RenderServiceNode_t*)malloc(sizeof(RenderServiceNode_t));
	node->arg = arg;
	node->cb = cb;
	node->obj = obj;

	att = GameObject_QueryAttribute(obj, "layer");
	node->layer = Attribute_ToInteger(att);

	list_add_tail(&node->next, &g_render_service_gui_root);

	render_service_sort();
}

static int comparation(struct list_head* a, struct list_head *b)
{
	RenderServiceNode_t* node1 = list_entry(a, RenderServiceNode_t, next);
	RenderServiceNode_t* node2 = list_entry(b, RenderServiceNode_t, next);

	return *node1->layer <= *node2->layer;
}

void render_service_sort(void)
{
	list_sort(&g_render_service_root,comparation);
	list_sort(&g_render_service_gui_root, comparation);
}

void render_all(void)
{
	RenderServiceNode_t* node;
	struct list_head* p;

	list_for_each(p, &g_render_service_root)
	{
		node = list_entry(p,RenderServiceNode_t, next);
		node->cb(node->arg);
	}

	
}

void render_gui(void)
{
	RenderServiceNode_t* node;
	struct list_head* p;

	list_for_each(p, &g_render_service_gui_root)
	{
		node = list_entry(p,RenderServiceNode_t, next);
		node->cb(node->arg);
	}
}