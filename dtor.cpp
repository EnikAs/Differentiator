#include "dtor.h"

#define DL dif(node->left)
#define DR dif(node->right)
#define CL copy_node(node->left)
#define CR copy_node(node->right)
//#define MUL(node1, node2) NEW_MUL(node1, node2)
//#define ADD(node1, node2) NEW_ADD(node1, node2)

Node* NEW_MUL (Node* node1, Node* node2)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.ch   = MUL;
    tmp_node->left      = node1;
    tmp_node->right     = node2;

    return tmp_node;
}

Node* NEW_ADD (Node* node1, Node* node2)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.ch   = ADD;
    tmp_node->left      = node1;
    tmp_node->right     = node2;

    return tmp_node;
}

Node* NEW_SUB (Node* node1, Node* node2)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.ch   = SUB;
    tmp_node->left      = node1;
    tmp_node->right     = node2;

    return tmp_node;
}

Node* NEW_DIV (Node* node1, Node* node2)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.ch   = DIV;
    tmp_node->left      = node1;
    tmp_node->right     = node2;

    return tmp_node;
}

Node* NEW_DEG (Node* node1, Node* node2)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.ch   = DEG;
    tmp_node->left      = node1;
    tmp_node->right     = node2;

    return tmp_node;
}

Node* NEW_CONST (double value)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = CONSTANT;
    tmp_node->data.db   = value;
    tmp_node->left      = NULL;
    tmp_node->right     = NULL;
}

int differentiator (FILE* inputfile)
{
    Tree* tree = TreeReadFileIN(inputfile);
    FILE* outputfile = fopen("tree_out.txt", "w");
    Tree* dtree = {};
    PRINT_LINE
    dtree->peak = dif(tree->peak);
    PRINT_LINE
    VisitPrintFileIN(dtree->peak, outputfile);
    PRINT_LINE
    TreeDump(dtree->peak);
    PRINT_LINE
}

Node* dif(Node* node)
{
    assert (node);
    
    switch(node->data_type)
    {
        case CONSTANT:
            PRINT_LINE
            return NEW_CONST(0.0);

        case VARIABLE:
            PRINT_LINE
            return NEW_CONST(1.0);

        case OPERATOR:
            PRINT_LINE
            switch(node->data.ch)
            {
                case MUL:
                    return NEW_ADD(NEW_MUL(DL, CR), NEW_MUL(CL, DR));

                case SUB:
                    return NEW_SUB(DL, DR);

                case ADD:
                    return NEW_ADD(DL, DR);

                case DIV:
                    return NEW_DIV(NEW_SUB(NEW_MUL(DL, CR), NEW_MUL(DR, CL)), NEW_DEG(CR, NEW_CONST(2.0)));

                case DEG:
                    return NEW_MUL(NEW_MUL(CR, NEW_DEG(CL, NEW_SUB(CR, NEW_CONST(1.0)))), DL);

            }

    }
}

Node* copy_node (Node* node)
{
    assert(node);

    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = node->data_type;
    tmp_node->left      = node->left;
    tmp_node->right     = node->right;

    if (tmp_node->data_type == VARIABLE || tmp_node->data_type == OPERATOR)
        tmp_node->data.ch = node->data.ch;

    else 
        tmp_node->data.db = node->data.db;
    

    if (node->left)
    {
        tmp_node->left = copy_node(node->left);
    }

    if (node->right)
    {
        tmp_node->right = copy_node(node->right);
    }

    return tmp_node;
}