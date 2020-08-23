#pragma once

 
/*
	通用链表实现，参照Linux内核中的list.h
	通用链表的结点即只有指针域的结构体，它们被嵌套在需要使用链表的其他结构体中
*/
typedef struct list_head {
	struct list_head *prev;
	struct list_head *next;
} LIST_HEAD;

/*
	初始化链表的宏
*/
#define LIST_HEAD_INIT(name) { &（name）, &(name)}

#define LIST_HEAD(name) \
		struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


/*
	结构体成员到结构体头的地址偏移量
*/
#define offsetof(type, member) ((size_t) &((type *)0) -> member)


/*
	获得含有链表结点的结构体的入口地址
	ptr: 结构体中的链表结点指针（& struct list_head pointer）
	type：结构体类型
	member: 结构体中的链表结点名字
*/

#define container_of(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *_mptr = (ptr); \
	(type *) ((char *) _mptr - offsetof(type, member));})

#define list_entry(ptr,type,member) container_of(ptr,type,member)


/*
	遍历
*/
#define list_for_next_each(pos, head) \
	for(pos = (head) -> next; pos != (head); pos = pos -> next)
#define list_for_prev_each(pos, head) \
	for(pos = (head) -> next; pos != (head); pos = pos -> prev)

#define list_for_each_entry(pos, head, member) \
	for(pos = list_entry((head)->next, typeof(*pos), member); \
		 &pos->member !=(head); \
		pos = list_entry(pos->member.next, typeof(*pos), member)) 


/*
	在两个结点之间插入新结点
*/
static inline void _list_add(LIST_HEAD* _new, LIST_HEAD* prev, LIST_HEAD* next) {
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}


/*
	在某个结点之后插入
*/
static inline void list_add(LIST_HEAD* _new, LIST_HEAD* head) {
	_list_add(_new, head, head->next);
}


/*
	在某个结点之前插入
*/
static inline void list_add_tail(LIST_HEAD* _new, LIST_HEAD* head) {
	_list_add(_new, head->prev, head);
}

/*
	删除结点
*/
static inline void _list_del(LIST_HEAD* prev, LIST_HEAD* next) {
	next->prev = prev;
	prev->next = next;
}

static inline void list_del(LIST_HEAD* entry) {
	_list_del(entry->prev, entry->next);
	entry->next = (void*)0;
	entry->prev = (void*)0;
}

static inline int list_empty(LIST_HEAD* head) {
	return head->next == head;
}

