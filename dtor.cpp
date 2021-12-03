#include "dtor.h"

#define DL dif(node->left)
#define DR dif(node->right)
#define CL copy_node(node->left)
#define CR copy_node(node->right)
#define LND(main_node) main_node->left->data.db
#define RND(main_node) main_node->right->data.db
#define FREE_L_R(node) free(node->left);\
free(node->right);\
node->left = NULL;\
node->right = NULL;

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

    return tmp_node;
}

Node* NEW_SIN (Node* node)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.db   = SIN;
    tmp_node->left      = node;
    tmp_node->right     = NULL;

    return tmp_node;
}

Node* NEW_COS (Node* node)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.db   = COS;
    tmp_node->left      = node;
    tmp_node->right     = NULL;

    return tmp_node;
}

int differentiator (FILE* inputfile)
{
    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    tree = TreeReadFileIN(inputfile);
    FILE* outputfile = fopen("tree_out.txt", "w");
    Tree* dtree = (Tree*) calloc(1, sizeof(tree));
    //dtree->peak = (Node*) calloc(1, sizeof(Node));
    
    dtree->peak = dif(tree->peak);

    optimisate_all(dtree->peak);

    VisitPrintFileIN(dtree->peak, outputfile);
    
    TreeDump(dtree->peak);
    
    FreeTheTree(dtree);

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
            switch(node->data.ch)
            {
                case MUL:
                    PRINT_LINE
                    return NEW_ADD(NEW_MUL(DL, CR), NEW_MUL(CL, DR));

                case SUB:
                    PRINT_LINE
                    return NEW_SUB(DL, DR);

                case ADD:
                    PRINT_LINE
                    return NEW_ADD(DL, DR);

                case DIV:
                    PRINT_LINE
                    return NEW_DIV(NEW_SUB(NEW_MUL(DL, CR), NEW_MUL(DR, CL)), NEW_DEG(CR, NEW_CONST(2.0)));

                case DEG:
                    PRINT_LINE
                    return NEW_MUL(NEW_MUL(CR, NEW_DEG(CL, NEW_SUB(CR, NEW_CONST(1.0)))), DL);
                
                case SIN:
                    PRINT_LINE
                    return NEW_MUL(NEW_MUL(NEW_COS(node->left), NEW_CONST(-1)), DL);

                case COS:
                    PRINT_LINE
                    return NEW_MUL(NEW_SIN(node->left), DL);

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

void optimisation_values (Node* node, int* val)
{
    if (!node->left && !node->right)
        return;
    
    if (node->data_type == OPERATOR && node->left->data_type == CONSTANT && node->right->data_type == CONSTANT)
    {
        switch(node->data.ch)
        {
            case MUL:
            PRINT_LINE
            node->data_type = CONSTANT;
            node->data.db = LND(node) * RND(node);
            FREE_L_R(node)
            break;

            case SUB:
            PRINT_LINE
            node->data_type = CONSTANT;
            node->data.db = LND(node) - RND(node);
            FREE_L_R(node)
            break;

            case ADD:
            PRINT_LINE
            node->data_type = CONSTANT;
            node->data.db = LND(node) + RND(node);
            FREE_L_R(node)
            break;

            case DIV:
            PRINT_LINE
            node->data_type = CONSTANT;
            node->data.db = LND(node) / RND(node);
            FREE_L_R(node)
            break;

            case DEG:
            PRINT_LINE
            node->data_type = CONSTANT;
            node->data.db = pow(LND(node), RND(node));
            FREE_L_R(node)
            break;

        }
        *val += 1;
    }
    if (node->left)
        optimisation_values(node->left, val);
    
    if (node->right)
        optimisation_values(node->right, val);
    
}

void optimisation_operators (Node* node, int* val)
{
    if (!node->left && !node->right)
        return;
    PRINT_LINE
    if (node->data_type == OPERATOR && node->left->data.db == 0 
     || node->data_type == OPERATOR && node->right->data.db == 0)
    {
        if (node->data.ch == MUL)
        {
            PRINT_LINE
            node->data_type = CONSTANT;
            node->data.db = 0;

            TreeNodesFree(node->right);
            TreeNodesFree(node->left);
            
            node->left = NULL;
            node->right = NULL;
            *val += 1;
            //return;
        }

        else if (node->data.ch == DEG)
        {
            PRINT_LINE
            node->data_type = CONSTANT;
            node->data.db = 1;

            TreeNodesFree(node->right);
            TreeNodesFree(node->left);
            
            node->left = NULL;
            node->right = NULL;
            *val += 1;
            //return;
        }

        else if (node->data.ch == SUB || node->data.ch == ADD)
        {
            PRINT_LINE
            if (node->left->data.db == 0)
            {
                Node* tmpnd = node->right;

                TreeNodesFree(node->left);

                node->data_type = tmpnd->data_type;
                node->data = tmpnd->data;
                node->left = tmpnd->left;
                node->right = tmpnd->right;
                *val += 1;
            }

            else if (node->right->data.db == 0)
            {
                Node* tmpnd = node->left;

                TreeNodesFree(node->right);

                node->data_type = tmpnd->data_type;
                node->data.db = tmpnd->data.db;
                node->left = tmpnd->left;
                node->right = tmpnd->right;
                *val += 1;
            }
        }
    }
    
    else if (node->data_type == OPERATOR && node->left->data.db == 1)
    {
        if (node->data.ch == MUL || node->data.ch == DIV || node->data.ch == DEG)
        {
            PRINT_LINE
            Node* tmpnd = node->right;

            TreeNodesFree(node->left);

            node->data_type = tmpnd->data_type;
            node->data = tmpnd->data;
            node->left = tmpnd->left;
            node->right = tmpnd->right;
            *val += 1;
        }
    }
    
    else if (node->data_type == OPERATOR && node->right->data.db == 1)
    {
        
        if (node->data.ch == MUL || node->data.ch == DIV || node->data.ch == DEG)
        {
            PRINT_LINE
            Node* tmpnd = node->left;

            TreeNodesFree(node->right);

            node->data_type = tmpnd->data_type;
            node->data = tmpnd->data;
            node->left = tmpnd->left;
            node->right = tmpnd->right;

            tmpnd->left = NULL;
            tmpnd->right = NULL;
            free(tmpnd);

            *val += 1;
        }
    }

    if (node->left)
        optimisation_operators(node->left, val);
    
    if (node->right)
        optimisation_operators(node->right, val);
    
}

void optimisate_all (Node* node)
{
    int optimisation_cunt = 1;

    while(optimisation_cunt != 0)
    {
        optimisation_cunt = 0;
        optimisation_values(node, &optimisation_cunt);
        optimisation_operators(node, &optimisation_cunt);
    }
}