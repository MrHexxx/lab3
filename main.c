#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <malloc.h>
#include "LinkedLists.h"

StringNode* readData();
int isLineCorrect(const char* line, const char* delimeters);
int emptyLine();
int testLineCorrect();
void printLists(StringNode* first);
void sortIntNodes(IntNode* first);
void cleanStringNodes(StringNode* node);
void cleanIntNodes(IntNode* node);

void main()
{
  StringNode* node = readData();
  StringNode* lastStringNode = node;
  while(lastStringNode != NULL)
  {
    sortIntNodes(lastStringNode->first);
    lastStringNode = lastStringNode->next;
  }
  printLists(node);
  cleanStringNodes(node);
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
      if( ! (fgets(lineBuffer, bufferSize, stdin) && isLineCorrect(lineBuffer, delimeters)) )
      {
        puts("Niepoprawna linia");
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
  free(stringRoot);
  free(intRoot);
  free(lineBuffer);
  return beginning;
}

void cleanStringNodes(StringNode* node)
{
  if(node->next != NULL)
    cleanStringNodes(node->next);
  if(node->first != NULL)
    cleanIntNodes(node->first);
  free(node);
}
void cleanIntNodes(IntNode* node)
{
  if(node->next != NULL)
    cleanIntNodes(node->next);
  free(node);
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

int testLineCorrect()
{
  const int bufferSize = 1024;
  char* const lineBuffer = malloc(sizeof(char) * bufferSize);
  memset(lineBuffer, '\0', bufferSize);
  const char* const delimeters = " \n";
  while(!emptyLine())
  {
    fgets(lineBuffer, bufferSize, stdin);
    if(!isLineCorrect(lineBuffer, delimeters)) puts("Niepoprawna linia");
  }
  free(lineBuffer);
}

void printLists(StringNode* first)
{
  StringNode* lastStringNode = first;
  while(lastStringNode != NULL)
  {
    printf("%s ", lastStringNode->string);
    IntNode* lastIntNode = lastStringNode->first;
    while(lastIntNode != NULL)
    {
      printf("%d ", lastIntNode->value);
      lastIntNode = lastIntNode->next;
    }
    puts("0");
    lastStringNode = lastStringNode->next;
  }
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
