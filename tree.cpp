#include "tree.h"

static      size_t       scanf_file_size         (FILE* input_file);
static      void         buffer_clean            (char* buffer);
static      int          VisitDumpPrint          (const Node* node);
static      void         trash_skip              (const char* buffer, int* buf_pos, int key);

FILE* log_file = fopen("logfile.txt", "w");

int VisitPrintFileIN(const Node* node, FILE* treefile)
{
    if (!node)
        return 0; // TODO Errors
    fprintf(treefile, "\n{\n");

    if (node->left)
        VisitPrintFileIN(node->left, treefile);

    if (node->data_type == CONSTANT)
        fprintf(treefile, "%lf", node->data.db);

    else if (node->data_type == OPERATOR)
        fprintf(treefile, "%c", node->data.ch);

    else if (node->data_type == VARIABLE)
        fprintf(treefile, "%c", node->data.ch);

    if (node->right)    
        VisitPrintFileIN(node->right, treefile);

    fprintf(treefile, "\n}\n");
    return 0;
}

int VisitDumpPrint(const Node* node)
{
    if (!node)
        return 0; // TODO Errors
    if (node->data_type == CONSTANT)
    {
        fprintf(log_file, "%d [label =\"%lf\"];\n", node, node->data.db);
    }
    else if (node->data_type == OPERATOR)
    {
        fprintf(log_file, "%d [label =\"%c\"];\n", node, node->data.ch);
    }
    else if (node->data_type == VARIABLE)
    {
        fprintf(log_file, "%d [label =\"%c\"];\n", node, node->data.ch);
    }
    if (node->left)
    {
        fprintf(log_file, "%d -> %d ;\n", node, node->left);
        VisitDumpPrint(node->left);    
    }

    if (node->right)
    {
        fprintf(log_file, "%d -> %d ;\n", node, node->right);
        VisitDumpPrint(node->right);
    }

    return 0;
}

int TreeDump (Node* node)
{
    fprintf(log_file, "digraph tree {\n");
    VisitDumpPrint(node);
    fprintf(log_file, "\n}");
    
    fclose(log_file);

    system ("\"C:/Program Files/Graphviz/bin/dot.exe\" -Tpng logfile.txt -o graph.png");
}

int TreeSetNode (Node* node, double data, Node* leftptr, Node* rightptr)
{
    assert(node);

    node->data_type = CONSTANT;
    node->data.db   = data;
    node->left      = leftptr;
    node->right     = rightptr;  
    
    return 0;
}

int TreeSetNode (Node* node, char data, Node* leftptr, Node* rightptr)
{
    assert(node);

    if (data == 'x')
    {
        node->data_type = VARIABLE;
    
    }
    else 
    {
        node->data_type = OPERATOR;
    }

    node->data.ch = data;
    node->left    = leftptr;
    node->right   = rightptr;

    return 0;
}

void trash_skip(const char* buffer, int* buf_pos, int key)
{
    if (key == ZERO_ONLY)
    {
        while (buffer[*buf_pos] == '\0')
        {
            *buf_pos += 1;
        }

    }

    else    
    {
        while (buffer[*buf_pos] != '\0')
        {
            if (buffer[*buf_pos] == 's')
                break;
            *buf_pos += 1;
        }

        trash_skip(buffer, buf_pos, ZERO_ONLY);
    }

}

Tree* TreeReadFileIN (FILE* treefile)
{
    int buf_pos = 0;
    int buf_size = scanf_file_size(treefile);
    char* buffer = (char*) calloc(buf_size, sizeof(char));
    if (buffer == NULL)
    {
        printf("Buffer callocation error!");
        return NULL;
    }
    fread(buffer, sizeof(char), buf_size, treefile);

    buffer_clean(buffer);

    Tree* tree = (Tree*) calloc(1, sizeof(Tree));
    if (tree == NULL)
    {
        printf("Tree callocation error!");
        return NULL;
    }
    tree->peak = (Node*) calloc(1, sizeof(Node));
    if (tree->peak == NULL)
    {
        printf("Main node callocation error!");
        return NULL;
    }

    trash_skip(buffer, &buf_pos, ZERO_ONLY);

    if (buffer[buf_pos] == '{')
        TreeReadNodeIN(buffer, tree->peak, &buf_pos);

    return tree;
}

int scanf_data_diffrent_type (const char* buffer, Node* node, int* buf_pos)
{
    //printf ("'%c' - start of this shitty string, buf pos = %d\n", buffer[*buf_pos], *buf_pos);

    if (sscanf(&buffer[*buf_pos], "%lf", &node->data.db) == 1)
    {
        node->data_type = CONSTANT;
    }

    else if (sscanf(&buffer[*buf_pos], "%c", &node->data.ch) == 1)
    {

        if (node->data.ch == 'x')
            node->data_type = VARIABLE;

        else
            node->data_type = OPERATOR;
    }
    else 
        assert(0 && "Incorrect input!!!");
        
    return 0;
}

int TreeReadNodeIN (const char* buffer, Node* main_node, int* buf_pos)
{
    if (buffer[*buf_pos] == '{')
        {
            trash_skip(buffer, buf_pos, LETTERS);
            if (buffer[*buf_pos] == 's')
            {
                kostyl_trig(buffer, main_node, buf_pos, LEFT, SIN);
            }

            else if (buffer[*buf_pos] == 'c')
            {
                kostyl_trig(buffer, main_node, buf_pos, LEFT, COS);
            }

            else if (buffer[*buf_pos] == '{')
            {
                Node* tmp_left_node = (Node*) calloc(1, sizeof(Node));
                main_node->left = tmp_left_node;
                TreeReadNodeIN(buffer, tmp_left_node, buf_pos);
            }
            else
            {
                    scanf_data_diffrent_type(buffer, main_node, buf_pos);
                    trash_skip(buffer, buf_pos, LETTERS);

                    if (buffer[*buf_pos] != '}')
                        printf("\nNE POVEZLO\n");

                    trash_skip(buffer, buf_pos, LETTERS);
                    return 0;
            }
            scanf_data_diffrent_type(buffer, main_node, buf_pos);
            if (buffer[*buf_pos + 1] == 's')
                *buf_pos += 1;
            else
                trash_skip(buffer, buf_pos, LETTERS);
            
            if (buffer[*buf_pos] == 's')
            {
                kostyl_trig(buffer, main_node, buf_pos, RIGHT, SIN);
            }

            else if (buffer[*buf_pos] == 'c')
            {
                kostyl_trig(buffer, main_node, buf_pos, RIGHT, COS);
            }
            
            else if (buffer[*buf_pos] == '{')
            {
                Node* tmp_right_node = (Node*) calloc(1, sizeof(Node));
                main_node->right = tmp_right_node;
                TreeReadNodeIN(buffer, tmp_right_node, buf_pos);
                
                if (buffer[*buf_pos] != '}')
                {
                    printf("It is not '}' it is %c", buffer[*buf_pos]);
                    assert(0 && "Incorrect input");
                }

                trash_skip(buffer, buf_pos, LETTERS);
                return 0;
            }

        } 
    
    return 0;
}

int kostyl_trig (const char* buffer, Node* main_node, int* buf_pos, int key, int key_tr)
{
    if (key == RIGHT)
    {
        if (key_tr == SIN)
        {
            Node* tmp_right_node = (Node*) calloc(1, sizeof(Node));
            main_node->right = tmp_right_node;
            tmp_right_node->data_type = OPERATOR;
            tmp_right_node->data.ch = 's';
            *buf_pos += 2;
            Node* tmp_sin_node = (Node*) calloc(1, sizeof(Node));
            tmp_right_node->left = tmp_sin_node;
            TreeReadNodeIN(buffer, tmp_sin_node, buf_pos);
        }

        else if (key_tr == COS)
        {
            Node* tmp_right_node = (Node*) calloc(1, sizeof(Node));
            main_node->right = tmp_right_node;
            tmp_right_node->data_type = OPERATOR;
            tmp_right_node->data.ch = 'c';
            *buf_pos += 2;
            Node* tmp_cos_node = (Node*) calloc(1, sizeof(Node));
            tmp_right_node->left = tmp_cos_node;
            TreeReadNodeIN(buffer, tmp_cos_node, buf_pos);
        }
    }

    else
    {
        if (key_tr == SIN)
        {
            Node* tmp_left_node = (Node*) calloc(1, sizeof(Node));
            main_node->left = tmp_left_node;
            tmp_left_node->data_type = OPERATOR;
            tmp_left_node->data.ch = 's';
            *buf_pos += 2;
            Node* tmp_sin_node = (Node*) calloc(1, sizeof(Node));
            tmp_left_node->left = tmp_sin_node;
            TreeReadNodeIN(buffer, tmp_sin_node, buf_pos);
        }

        else if (key_tr == COS)
        {
            PRINT_LINE
            Node* tmp_left_node = (Node*) calloc(1, sizeof(Node));
            main_node->left = tmp_left_node;
            tmp_left_node->data_type = OPERATOR;
            tmp_left_node->data.ch = 'c';
            *buf_pos += 2;
            Node* tmp_cos_node = (Node*) calloc(1, sizeof(Node));
            tmp_left_node->left = tmp_cos_node;
            TreeReadNodeIN(buffer, tmp_cos_node, buf_pos);
        }
    }
}

void buffer_clean (char* buffer)
{
    int size = strlen(buffer);
    for(int i = 0 ; i < size ; i++)
    {
        if (buffer[i] == '\n')
        {
            buffer[i] = '\0';
        }
    }

}

size_t scanf_file_size (FILE* input_file)
{
    struct stat file;
    if (fstat(fileno(input_file), &file ) == -1)
    {
        printf("FILE READING ERROR");
        return EOF;
    }

    return file.st_size;
}

int FreeTheTree (Tree* tree)
{
    TreeNodesFree(tree->peak);

    free(tree);
}

int TreeNodesFree (Node* node)
{
    if (node->left)
    {
        TreeNodesFree(node->left);
        node->left = NULL;
    }
    
    if (node->right)
    {
        TreeNodesFree(node->right);
        node->right = NULL;
    }

#ifdef TYPE_CHAR

    if (node->data_calloc > 0)
        free(node->data);
    
#endif

    free(node);
}

void CreateTreeFile (FILE* inputfile, FILE* outputfile)
{
    char tmp_ch = 0;
    tmp_ch = fgetc(inputfile);
    while (tmp_ch != EOF)
    {
        if (tmp_ch == '{' || tmp_ch == '}')
        {
            fprintf(outputfile, "\n%c\n", tmp_ch);
        }

        else
        {
            fprintf(outputfile, "%c", tmp_ch);
        }
        tmp_ch = fgetc(inputfile);
    }
}