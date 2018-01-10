#ifndef LINKED_LISTS
#define LINKED_LISTS

struct sIntNode
{
  unsigned int value;
  struct sIntNode* next;
};

struct sStringNode
{
  char string[21];
  struct sStringNode* next;
  struct sIntNode* first;
};

typedef struct sStringNode StringNode;
typedef struct sIntNode IntNode;

#endif
