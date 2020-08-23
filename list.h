#pragma once

 
/*
	ͨ������ʵ�֣�����Linux�ں��е�list.h
	ͨ������Ľ�㼴ֻ��ָ����Ľṹ�壬���Ǳ�Ƕ������Ҫʹ������������ṹ����
*/
typedef struct list_head {
	struct list_head *prev;
	struct list_head *next;
} LIST_HEAD;

/*
	��ʼ������ĺ�
*/
#define LIST_HEAD_INIT(name) { &��name��, &(name)}

#define LIST_HEAD(name) \
		struct list_head name = LIST_HEAD_INIT(name)

#define INIT_LIST_HEAD(ptr) do { \
	(ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)


/*
	�ṹ���Ա���ṹ��ͷ�ĵ�ַƫ����
*/
#define offsetof(type, member) ((size_t) &((type *)0) -> member)


/*
	��ú���������Ľṹ�����ڵ�ַ
	ptr: �ṹ���е�������ָ�루& struct list_head pointer��
	type���ṹ������
	member: �ṹ���е�����������
*/

#define container_of(ptr, type, member) ({ \
	const typeof( ((type *)0)->member ) *_mptr = (ptr); \
	(type *) ((char *) _mptr - offsetof(type, member));})

#define list_entry(ptr,type,member) container_of(ptr,type,member)


/*
	����
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
	���������֮������½��
*/
static inline void _list_add(LIST_HEAD* _new, LIST_HEAD* prev, LIST_HEAD* next) {
	next->prev = _new;
	_new->next = next;
	_new->prev = prev;
	prev->next = _new;
}


/*
	��ĳ�����֮�����
*/
static inline void list_add(LIST_HEAD* _new, LIST_HEAD* head) {
	_list_add(_new, head, head->next);
}


/*
	��ĳ�����֮ǰ����
*/
static inline void list_add_tail(LIST_HEAD* _new, LIST_HEAD* head) {
	_list_add(_new, head->prev, head);
}

/*
	ɾ�����
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

