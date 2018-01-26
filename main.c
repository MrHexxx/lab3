//Mateusz Okulus
//PRI3 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "LinkedLists.h"

StringNode* readData(); //wczytuje wszystkie dane, jednak lista StringNode jest zapetlona
int isLineCorrect(const char* line, const char* delimeters);//sprawdza czy linia jest poprawnie sformatowana
int emptyLine();//czy wczytano pusta linie? przerwanie dalszego wczytywania danych
void sortIntNodes(IntNode* first);
void cleanStringNodes(StringNode* node); //czysci liste StringNode
void cleanIntNodes(IntNode* node); //czysci liste IntNode
void clean(StringNode* node); //przerywa za[petlenie i usuwa wszystkie dane
void sort(StringNode* node); //sortuje wszyskie podlisty w zapetlonej liscie
void processCommandLine(StringNode* first); //analiza i zastosowanie komend

void main()
{
  puts("<label> <int>* 0");
  StringNode* node = readData();
  if(node == NULL)
  {
    puts("No data");
    return;
  }
  sort(node);
  puts("Read all data");
  StringNode* first = node;
  while(node->next != first) node = node->next;
  processCommandLine(node);
  clean(node);
}

StringNode* readData()
{
  //"korzenie", by nie dzielić przypadków na wpisanie pierwszego i dodanie
  //tak jest tylko dodawanie
  StringNode* const stringRoot = malloc(sizeof(StringNode));
  stringRoot->first = NULL;
  stringRoot->next = NULL;
  StringNode* lastStringNode = stringRoot;
  IntNode* const intRoot = malloc(sizeof(IntNode));
  intRoot->next = NULL;
  IntNode* lastIntNode = intRoot;
  int intBuffer = 0;
  const int bufferSize = 1024;
  char* const lineBuffer = malloc(sizeof(char) * bufferSize);
  memset(lineBuffer, '\0', bufferSize);
  const char* const delimeters = " \n";

  while(!emptyLine())
  {
      if(!fgets(lineBuffer, bufferSize, stdin))
      {
        break;
      }
      if( !isLineCorrect(lineBuffer, delimeters) )
      {
        puts("Incorrect line");
        continue;
      }

      char* token = strtok(lineBuffer, delimeters);
      StringNode* newStringNode = malloc( sizeof(StringNode) );
      newStringNode->next = NULL;
      newStringNode->first = NULL;
      lastStringNode->next = newStringNode;
      lastStringNode = newStringNode;
      sscanf(token, "%20s", newStringNode->string);

      token = strtok(NULL, delimeters);
      do
      {
        sscanf(token, "%d", &intBuffer);
        if(intBuffer == 0)
        {
          //koniec wczytywania linii, czyscimy
          IntNode* firstIntNode = intRoot->next;
          lastStringNode->first = firstIntNode;
          intRoot->next = NULL;
          lastIntNode = intRoot;
        }
        else
        {
          IntNode* newIntNode = malloc( sizeof(IntNode) );
          newIntNode->value = intBuffer;
          newIntNode->next = NULL;
          lastIntNode->next = newIntNode;
          lastIntNode = newIntNode;
        }

        token = strtok(NULL, delimeters);
      }while(token != NULL);
  }//while
  //koniec wczytywania

  StringNode* beginning = stringRoot->next;
  lastStringNode->next = beginning;
  free(stringRoot);
  free(intRoot);
  free(lineBuffer);
  return beginning;
}

void clean(StringNode* node)
{
  if(node != NULL)
  {
    StringNode* first = node->next;
    node->next = NULL;
    cleanStringNodes(first);
  }
}
void cleanStringNodes(StringNode* node)
{
    if(node->next != NULL)
      cleanStringNodes(node->next);
    if(node->first != NULL)
      cleanIntNodes(node->first);
    free(node);
    node = NULL;
}
void cleanIntNodes(IntNode* node)
{
  if(node->next != NULL)
    cleanIntNodes(node->next);
  free(node);
  node = NULL;
}

int isLineCorrect(const char* line, const char* delimeters)
{
  int length = strlen(line) + 1; //dla nulla na koncu
  char* const buffer = malloc(sizeof(char) * length);
  memset(buffer, '\0', length);
  char stringBuffer[21];
  int intBuffer = -1;
  strcpy(buffer, line);
  int noError = 1;

  //string
  char* token = strtok(buffer, delimeters);
  noError = noError && sscanf(token, "%20s", stringBuffer);
  token = strtok(NULL, delimeters);

  while(token != NULL)
  {
    noError = noError && sscanf(token, "%d", &intBuffer);
    noError = noError && (intBuffer >= 0);
    if(intBuffer == 0)
    {
      token = strtok(NULL, delimeters);
      noError = noError && (token == NULL); //nie ma nic wiecej
      free(buffer);
      return noError;
    }
    token = strtok(NULL, delimeters);
  }

  //token jest null ale nie wyszlismy przez zero, czyli nie ma zera na końcu
  noError = noError && (intBuffer == 0);
  free(buffer);
  return noError;
}

int emptyLine()
{
  char c = getchar();
  if(c == '\n')
    return 1;
  ungetc(c, stdin);
  return 0;
}

void sortIntNodes(IntNode* first)
{
  int count = 0;
  IntNode* lastNode = first;
  while(lastNode != NULL)
  {
    count++;
    lastNode = lastNode->next;
  }
  if(count <= 1)
    return;

  IntNode* currentNode = first;
  int i = 0;
  for(i = 0; i < count - 1; i++)
  {
    int j = 0;
    for(j = 0; j < count - 1 - i; j++)
    {
      if(currentNode->next->value < currentNode->value)
      {
        int temp = currentNode->next->value;
        currentNode->next->value = currentNode->value;
        currentNode->value = temp;
      }
      currentNode = currentNode->next;
    }
    currentNode = first;
  }
}

void sort(StringNode* node)
{
  StringNode* lastStringNode = node;
  StringNode* first = node;
  if(lastStringNode != NULL) //gdy jest tylko 1 element jest zapetlony na sam siebie
  {
    sortIntNodes(lastStringNode->first);
    lastStringNode = lastStringNode->next;
  }
  while(lastStringNode != NULL && first != lastStringNode)
  {
    sortIntNodes(lastStringNode->first);
    lastStringNode = lastStringNode->next;
  }
}

void processCommandLine(StringNode* first)
{
  const int bufferSize = 1024;
  char* const lineBuffer = malloc(sizeof(char) * bufferSize);
  int intBuffer = -1;
  memset(lineBuffer, '\0', bufferSize);
  StringNode* current = first;
  char stringBuffer[128];
  memset(stringBuffer, '\0', 128);

  while(fgets(lineBuffer, bufferSize, stdin))
  {
    if(sscanf(lineBuffer, "%127s", stringBuffer) && !strcmp("next", stringBuffer))
    {
        if(current == NULL)
        {
          puts("\tEmpty list");
          return;
        }
        else
        {
          current = current->next;
        }
        puts("\tMoved to the next element");
    }
    else if(sscanf(lineBuffer, "%127s", stringBuffer) && !strcmp("delete", stringBuffer))
    {
      StringNode* toDelete = current->next;
      if(toDelete == current)
      {
        puts("\tOnly 1 link - can't delete");
        continue;
      }
      StringNode* next = toDelete->next;
      toDelete->next = NULL;
      current->next = next;
      cleanStringNodes(toDelete);
      puts("\tDeleted");
    }
    else if(sscanf(lineBuffer, "add %d", &intBuffer))
    {
      StringNode* toAdd = current->next;
      if(intBuffer <= 0)
      {
        puts("\tIncorrect number");
      }
      else
      {
        if(toAdd->first == NULL)
        {
          toAdd->first = malloc(sizeof(IntNode));
          toAdd->first->value = intBuffer;
          toAdd->first->next = NULL;
          printf("\tAdded %d\n", intBuffer);
        }
        else
        {
          IntNode* last = toAdd->first;
          while(last->next != NULL)
          {
            last = last->next;
          }
          last->next = malloc(sizeof(IntNode));
          last->next->value = intBuffer;
          last->next->next = NULL;
          sortIntNodes(toAdd->first);
          printf("\tAdded %d\n", intBuffer);
        }
      }
    }
    else if(sscanf(lineBuffer, "add %20s", stringBuffer))
    {
      StringNode* toAdd = current->next;
      StringNode* next = toAdd->next;
      StringNode* newNode = malloc(sizeof(StringNode));
      newNode->first = NULL;
      stringBuffer[20] = '\0';
      strcpy(newNode->string, stringBuffer);
      newNode->next = next;
      toAdd->next = newNode;
      current = toAdd;
      memset(stringBuffer, '\0', 128);
      printf("\tAdded %s\n", newNode->string);
    }
    else if(sscanf(lineBuffer, "%127s", stringBuffer) && !strcmp("print", stringBuffer))
    {
      StringNode* nodeToPrint = current->next;
      printf("\t%s", nodeToPrint->string);
      IntNode* last = nodeToPrint->first;
      while(last != NULL)
      {
        if(last->value != 0)
          printf(" %d", last->value);
        last = last->next;
      }
      printf("\n");
    }
    else if(sscanf(lineBuffer, "%127s", stringBuffer) && !strcmp("exit", stringBuffer))
    {
      free(lineBuffer);
      return;
    }
    else if(sscanf(lineBuffer, "%127s", stringBuffer) && !strcmp("help", stringBuffer))
    {
      puts("\t        next - move to the next element");
      puts("\tadd <number> - add number to current list");
      puts("\tadd <string> - add new element with empty list");
      puts("\t      delete - remove current element");
      puts("\t       print - print current list");
      puts("\t        exit - clean and exit program");
    }
    else
    {
      puts("\tIncorrect command - type help");
    }

    memset(lineBuffer, '\0', bufferSize);
    memset(stringBuffer, '\0', 128);
  }
  free(lineBuffer);
}
