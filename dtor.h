#define DTOR_H_INCLUDED
#ifdef DTOR_H_INCLUDED

#include "tree.h"
#include <math.h>


int         differentiator              (FILE* inputfile);

Node*       dif                         (Node* node);

Node*       copy_node                   (Node* node);

void        optimisation_values         (Node* node, int* opt_cunt);

void        optimisation_operators      (Node* node, int* opt_cunt);

void        optimisate_all              (Node* node, FILE* texfile);

int         VisitPrintTex               (Node* node, FILE* texfile);

void        PrintData                   (Node* node, FILE* texfile);

void        PrintTexStart               (FILE* texfile);

void        PrintTexTreeBefore          (Node* node, FILE* texfile);

void        PrintTexTreeAfter           (Node* node, FILE* texfile);

void        PrintTexEnd                 (FILE* texfile);

void        PrintTexTreeBetween         (Node* node, FILE* texfile);

#endif