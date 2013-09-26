
struct stack;

struct stack * stack_create (void);
int stack_destroy (struct stack *stack);
int stack_push (struct stack *stack, void *data);
void * stack_pop (struct stack *stack);
unsigned int stack_size (struct stack *stack);
int stack_iterate (struct stack *stack, int (*iterator) (void *context, struct stack *stack, void *item), void *context);
