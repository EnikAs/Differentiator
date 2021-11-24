#ifndef TREE_H_INCLUDED
#define TREE_H_INCLUDED
#include <iostream>
#include <iostream>
#include <cstring>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <cassert>
#include <sys\stat.h>

typedef char* telem_t;

const int MAX_STR_SIZE = 1000;

#define PRINT_LINE printf("I'm at line %d at function %s\n", __LINE__, __func__);

#define TYPE_UNION

enum input
{
    INCORRECT_INPUT = 228,
    CORRECT_INPUT
};

enum memory_check
{
    CALLOC_ERROR = 322,
    MALLOC_ERROR,
    MEMORY_IS_OK
};

enum node_data_type
{   
    CONSTANT,
    VARIABLE,
    OPERATOR
};

enum operator_type
{
    MUL = '*',
    SUB = '-',
    ADD = '+',
    DIV = '/',
    DEG = '^'
};

enum trash_skip_key
{
    LETTERS,
    ZERO_ONLY
};

enum read_node_in_key
{
    LEFT,
    RIHGT
};

union node_data
{
    double db = 0;
    char ch;
};

struct Node
{
    Node* left = NULL;
    Node* right = NULL;
    node_data data = {};
    int data_calloc = 0;
    int data_type = 0;
};

struct Tree
{
    Node* peak = NULL;

};

int     VisitPrintFilePRE        (const Node* node, FILE* treefile);

int     VisitPrintFileIN         (const Node* node, FILE* treefile);

int     TreeDump                 (Node* node);

int     TreeSetNode              (Node* node, telem_t value, Node* leftptr, Node* rightptr);

Tree*   TreeReadFilePRE          (FILE* treefile);

Tree*   TreeReadFileIN           (FILE* treefile);

int     TreeReadNodePRE          (const char* buffer, Node* main_node, int* buf_pos);

int     scanf_data_diffrent_type (const char* buffer, Node* node, int* buf_pos);

int     TreeReadNodeIN           (const char* buffer, Node* main_node, int* buf_pos);

int     FreeTheTree              (Tree* tree);


#endif