#ifndef Stack_H_
#define Stack_H_

typedef struct _Dicks{
	int size;
	int *indices; 
	struct _Dicks *previous;
}Dick;

typedef struct _Stack{
	int size;
	Dick *head;
	/*push method*/
	void(*push)(struct _Stack *stack, struct _Dicks *d);
	/*retrieve the last element from stack(*d will point to this item).this element shouldn't appear in the stack after*/
	Dick*(*pop)(struct _Stack *stack);
	/*return 1 if the stack is empty and 0 otherwise*/
	int(*is_empty)(struct _Stack *stack);
	

}Stack;

/* Allocates a new Stack */
Stack* stack_allocate();




#endif
