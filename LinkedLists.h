#ifndef LINKED_LISTS
#define LINKED_LISTS

#define MAX_CHARS 20

struct sIntNode
{
  unsigned int value;
  struct sIntNode* next;
};

struct sStringNode
{
  char string[MAX_CHARS + 1];
  struct sStringNode* next;
  struct sIntNode* first;
};

typedef struct sStringNode StringNode;
typedef struct sIntNode IntNode;

#endif
