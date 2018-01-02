#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "LinkedLists.h"

int endOfStream(); //czy wczytano pustą linię? (tylko znak nowej linii)
StringNode* readAllData();
void printList(StringNode* list);

void main()
{
    StringNode* firstList = readAllData();
    if(firstList != NULL)
    {
      IntNode* ptr = firstList->first;
      while(ptr != NULL)
      {
        printf("\n%d", ptr->value);
        ptr = ptr->next;
      }
    }
}

int endOfStream()
{
  char c = getchar();
  if(c == '\n')
    return 1;
  ungetc(c, stdin);
  return 0;
}

StringNode* readAllData()
{
  enum State {STRING, INT, ZERO, EOS} state = STRING;
  //"korzenie", by nie dzielić przypadków na wpisanie pierwszego i dodanie
  //tak jest tylko dodawanie
  StringNode* stringRoot = malloc(sizeof(StringNode));
  StringNode* lastStringNode = stringRoot;
  IntNode* intRoot = malloc(sizeof(IntNode));
  IntNode* lastIntNode = intRoot;
  char stringBuffer[21];
  int intBuffer = 0;

  while(state != EOS)
  {
    switch (state)
    {
      case STRING:
        if(endOfStream())
        {
          state = EOS;
        }
        else if(scanf("%20s", stringBuffer))
        {
          StringNode* newStringNode = malloc(sizeof(StringNode));
          strcpy(newStringNode->string, stringBuffer);
          lastStringNode->next = newStringNode;
          lastStringNode = lastStringNode->next;
          state = INT;
        }
      break;

      case INT:
        if(scanf("%d", &intBuffer))
        {
          if(intBuffer == 0)
          {
            state = ZERO;
          }
          else
          {
            IntNode* newIntNode = malloc(sizeof(IntNode));
            newIntNode->value = intBuffer;
            lastIntNode->next = newIntNode;
            lastIntNode = lastIntNode->next;
          }
        }
        else
        {
          //tutaj program nie może się zatrzymać
          //jest to koniec danych bez zera na końcu
          printf("\nBLAD\n");
          exit(1);
        }
      break;

      case ZERO:
        lastStringNode->first = intRoot->next;
        intRoot->next = NULL;
        lastIntNode = intRoot;
        state = STRING;
        scanf("%*c");
      break;
    }
  }//while

  StringNode* beginning = stringRoot->next;
  free(stringRoot);
  free(intRoot);
  return beginning;
}

void printList(StringNode* list)
{

}
