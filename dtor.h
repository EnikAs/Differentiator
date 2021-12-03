#define DTOR_H_INCLUDED
#ifdef DTOR_H_INCLUDED

#include "tree.h"
#include <math.h>


int differentiator (FILE* inputfile);

Node* dif(Node* node);

Node* copy_node (Node* node);

void optimisation_values (Node* node, int* val);

void optimisation_operators (Node* node, int* val);

void optimisate_all (Node* node);

#endif