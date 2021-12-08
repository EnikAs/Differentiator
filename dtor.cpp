#include "dtor.h"

#define DL dif(node->left)
#define DR dif(node->right)
#define CL copy_node(node->left)
#define CR copy_node(node->right)
#define LND(main_node) main_node->left->data.db
#define RND(main_node) main_node->right->data.db
#define NL node->left
#define NR node->right
#define FREE_L_R(node) free(node->left);\
free(node->right);\
node->left = NULL;\
node->right = NULL;

//#define MUL(node1, node2) NEW_MUL(node1, node2)
//#define ADD(node1, node2) NEW_ADD(node1, node2)

Node* NEW_MUL (Node* node1, Node* node2)
{
    assert(node1);
    assert(node2);
    
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
    tmp_node->data.ch   = SIN;
    tmp_node->left      = node;
    tmp_node->right     = NULL;

    return tmp_node;
}

Node* NEW_COS (Node* node)
{
    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = OPERATOR;
    tmp_node->data.ch   = COS;
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
    FILE* texfile = fopen("tex.txt", "w");

    PrintTexStart(texfile);

    PrintTexTreeBefore(tree->peak, texfile);

    dtree->peak = dif(tree->peak);

    PrintTexTreeAfter(dtree->peak, texfile);

    optimisate_all(dtree->peak, texfile);

    PrintTexEnd(texfile);

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
            return NEW_CONST(0.0);

        case VARIABLE:
            return NEW_CONST(1.0);

        case OPERATOR:
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
                
                case SIN:
                    return NEW_MUL(NEW_COS(NL), DL);

                case COS:
                    return NEW_MUL(NEW_MUL(NEW_SIN(NL), NEW_CONST(-1)), DL);

            }

    }
}

Node* copy_node (Node* node)
{
    assert(node);

    Node* tmp_node      = (Node*) calloc(1, sizeof(Node));
    tmp_node->data_type = node->data_type;
    tmp_node->left      = NL;
    tmp_node->right     = NR;

    if (tmp_node->data_type == VARIABLE || tmp_node->data_type == OPERATOR)
        tmp_node->data.ch = node->data.ch;

    else 
        tmp_node->data.db = node->data.db;
    
    if (NL)
    {
        tmp_node->left = copy_node(NL);
    }

    if (NR)
    {
        tmp_node->right = copy_node(NR);
    }

    return tmp_node;
}

void optimisation_values (Node* node, int* opt_cunt)
{
    if (!NL && !NR)
        return;
    
    if (node->data_type == OPERATOR && NL->data_type == CONSTANT && NR->data_type == CONSTANT)
    {
        switch(node->data.ch)
        {
            case MUL:
            node->data_type = CONSTANT;
            node->data.db = LND(node) * RND(node);
            FREE_L_R(node)
            break;

            case SUB:
            node->data_type = CONSTANT;
            node->data.db = LND(node) - RND(node);
            FREE_L_R(node)
            break;

            case ADD:
            node->data_type = CONSTANT;
            node->data.db = LND(node) + RND(node);
            FREE_L_R(node)
            break;

            case DIV:
            node->data_type = CONSTANT;
            node->data.db = LND(node) / RND(node);
            FREE_L_R(node)
            break;

            case DEG:
            node->data_type = CONSTANT;
            node->data.db = pow(LND(node), RND(node));
            FREE_L_R(node)
            break;

        }

        *opt_cunt += 1;

    }
    if (NL)
        optimisation_values(NL, opt_cunt);
    
    if (NR)
        optimisation_values(NR, opt_cunt);
    
}

void optimisation_operators (Node* node, int* opt_cunt)
{
    
    if (NL == NULL || NR == NULL)
    {
        if (NL)
        optimisation_operators(NL, opt_cunt);
    
        if (NR)
        optimisation_operators(NR, opt_cunt);
    }
    else
    {
        if (node->data_type == OPERATOR && NL->data.db == 0 
        || node->data_type == OPERATOR && NR->data.db == 0)
        {
            if (node->data.ch == MUL)
            {
                node->data_type = CONSTANT;
                node->data.db = 0;

                TreeNodesFree(NR);
                TreeNodesFree(NL);
                
                NL = NULL;
                NR = NULL;

                *opt_cunt += 1;
            }

            else if (node->data.ch == DEG)
            {
                node->data_type = CONSTANT;
                node->data.db = 1;

                TreeNodesFree(NR);
                TreeNodesFree(NL);
                
                NL = NULL;
                NR = NULL;

                *opt_cunt += 1;
            }

            else if (node->data.ch == SUB || node->data.ch == ADD)
            {
                if (NL->data.db == 0)
                {
                    Node* tmpnd = NR;

                    TreeNodesFree(NL);

                    node->data_type = tmpnd->data_type;
                    node->data = tmpnd->data;
                    NL = tmpnd->left;
                    NR = tmpnd->right;

                    *opt_cunt += 1;
                }

                else if (NR->data.db == 0)
                {
                    Node* tmpnd = NL;

                    TreeNodesFree(NR);

                    node->data_type = tmpnd->data_type;
                    node->data.db = tmpnd->data.db;
                    NL = tmpnd->left;
                    NR = tmpnd->right;

                    *opt_cunt += 1;
                }
            }
        }
        
        else if (node->data_type == OPERATOR && NL->data.db == 1)
        {
            if (node->data.ch == MUL || node->data.ch == DIV || node->data.ch == DEG)
            {
                Node* tmpnd = NR;

                TreeNodesFree(NL);

                node->data_type = tmpnd->data_type;
                node->data = tmpnd->data;
                NL = tmpnd->left;
                NR = tmpnd->right;

                *opt_cunt += 1;
            }
        }
        
        else if (node->data_type == OPERATOR && NR->data.db == 1)
        {
            
            if (node->data.ch == MUL || node->data.ch == DIV || node->data.ch == DEG)
            {
                Node* tmpnd = NL;

                TreeNodesFree(NR);

                node->data_type = tmpnd->data_type;
                node->data = tmpnd->data;
                NL = tmpnd->left;
                NR = tmpnd->right;

                tmpnd->left = NULL;
                tmpnd->right = NULL;
                free(tmpnd);

                *opt_cunt += 1;
            }
        
        }

        if (NL)
            optimisation_operators(NL, opt_cunt);
        
        if (NR)
            optimisation_operators(NR, opt_cunt);
    }
}       

void optimisate_all (Node* node, FILE* texfile)
{
    int optimisation_cunt = 1;

    while(optimisation_cunt != 0)
    {
        optimisation_cunt = 0;
        optimisation_values(node, &optimisation_cunt);
        optimisation_operators(node, &optimisation_cunt);
    }

    PrintTexTreeBetween(node, texfile);
}

int VisitPrintTex (Node* node, FILE* texfile)
{   
    if (node->data_type == OPERATOR)
    {
        if (node->data.ch == SIN || node->data.ch == COS)
        {
            if (node->data.ch == SIN)
            fprintf(texfile, "sin");

            if (node->data.ch == COS)
            fprintf(texfile, "cos");

            if(NL)
                VisitPrintTex(NL, texfile);

            if(NR)
                VisitPrintTex(NR, texfile);
        }

        else if (node->data.ch == ADD || node->data.ch == SUB)
        {
            fprintf(texfile, "(");

            if(NL)
                VisitPrintTex(NL, texfile);

            PrintData(node, texfile);

            if(NR)
                VisitPrintTex(NR, texfile);
            
            fprintf(texfile, ")");
        }

        else if (node->data.ch == DIV)
        {
            fprintf(texfile, "\\frac{");

            if(NL)
                VisitPrintTex(NL, texfile);

            fprintf(texfile, "}{");

            if(NR)
                VisitPrintTex(NR, texfile);
            
            fprintf(texfile, "}");
        }

        else if (node->data.ch == DEG)
        {
            if(NL)
                VisitPrintTex(NL, texfile);

            fprintf(texfile, "^{");

            if(NR)
                VisitPrintTex(NR, texfile);
            
            fprintf(texfile, "}");
        }

        else
        {
            if(NL)
                VisitPrintTex(NL, texfile);

            PrintData(node, texfile);

            if(NR)
                VisitPrintTex(NR, texfile);
        }
    }
    else 
    {
        if(NL)
            VisitPrintTex(NL, texfile);

        PrintData(node, texfile);

        if(NR)
            VisitPrintTex(NR, texfile);
    }
}

void PrintData(Node* node, FILE* texfile)
{
    if (node->data_type == CONSTANT)
    {  
        if(node->data.db >= 0)
            fprintf(texfile, "%.1lf", node->data.db);
        else
            fprintf(texfile, "(%.1lf)", node->data.db);
    }

    else if (node->data_type == OPERATOR)
    {
        if (node->data.ch == MUL)
            fprintf(texfile, "\\cdot ");
        else
            fprintf(texfile, "%c", node->data.ch);
    }

    else if (node->data_type == VARIABLE)
        fprintf(texfile, "%c", node->data.ch);

}

void PrintTexStart (FILE* texfile)
{
    fprintf(texfile, "\n\\documentclass{article}\n\n"
    "\\usepackage[utf8]{inputenc}\n\n"
    "\\title{The most complete guide to differentiation}\n\n"
    "\\begin{document}\n\n"
    "\\maketitle"
    "\\newpage");
}

void PrintTexTreeBefore (Node* node, FILE* texfile)
{
    fprintf(texfile, "\n\\section{Before dif}\n\n");

    fprintf(texfile, "$");
    VisitPrintTex(node, texfile);
    fprintf(texfile, "$");
}

void PrintTexTreeAfter (Node* node, FILE* texfile)
{
    fprintf(texfile, "\n\\section{After dif}\n\n");

    fprintf(texfile, "$");
    VisitPrintTex(node, texfile);
    fprintf(texfile, "$");
}

void PrintTexEnd (FILE* texfile)
{
    fprintf(texfile, "\n\\end{document}");
}

void PrintTexTreeBetween (Node* node, FILE* texfile)
{
    fprintf(texfile, "\n\\section{Using simple, well-known transformations, it is easy to notice that}\n\n");
    fprintf(texfile, "$");
    VisitPrintTex(node, texfile);
    fprintf(texfile, "$");
}