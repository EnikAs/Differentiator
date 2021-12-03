#include "tree.h"

static      size_t       scanf_file_size         (FILE* input_file);
static      void         buffer_clean            (char* buffer);
static      int          VisitDumpPrint          (const Node* node);
static      void         trash_skip              (const char* buffer, int* buf_pos, int key);

FILE* log_file = fopen("logfile.txt", "w");

int VisitPrintFilePRE(const Node* node, FILE* treefile)
{
    if (!node)
        return 0; // TODO Errors
    fprintf(treefile, "\n{\n");

    fprintf(treefile, "%s", node->data);
    if (node->left)
        VisitPrintFilePRE(node->left, treefile);

    if (node->right)    
        VisitPrintFilePRE(node->right, treefile);

    fprintf(treefile, "\n}\n");
    return 0;
}

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
    PRINT_LINE
    fprintf(log_file, "digraph tree {\n");
    PRINT_LINE
    VisitDumpPrint(node);
    PRINT_LINE
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

#ifdef TYPE_CHAR

int TreeSetNode (Node* node, char* value, Node* leftptr, Node* rightptr)
{
    //node->data = (telem_t) calloc(strlen(value), sizeof(char));
    if (node->data == NULL)
    {
        assert(0 && "Calloc in SetNode ERROR!");
    }

    node->data = value;
    node->left = leftptr;
    node->right = rightptr;
    return 0;
}

Tree* TreeReadFilePRE (FILE* treefile)
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

    if (buffer[0] != '\0' || buffer[1] != '{' || buffer[2] != '\0')
        return NULL;

    buf_pos = 3;
    tree->peak->data = &buffer[buf_pos];
    while(1)
    {
        if (buffer[buf_pos] == '{' || buffer[buf_pos] == '}')
            break;
        buf_pos++;  
        
    }

    if (buffer[buf_pos] == '{')
        TreeReadNode(buffer, tree->peak, &buf_pos);

    return tree;
}

#endif

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
            
            if (buffer[*buf_pos] == '{')
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
            trash_skip(buffer, buf_pos, LETTERS);
            if (buffer[*buf_pos] == '{')
            {
                Node* tmp_right_node = (Node*) calloc(1, sizeof(Node));
                main_node->right = tmp_right_node;
                TreeReadNodeIN(buffer, tmp_right_node, buf_pos);
                
                if (buffer[*buf_pos] != '}')
                    printf("It is not '}' it is %c", buffer[*buf_pos]);
                
                trash_skip(buffer, buf_pos, LETTERS);
                return 0;
            }

        }  
    return 0;// END
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
#ifdef TYPE_CHAR
int TreeReadNodePRE (const char* buffer, Node* main_node, int* buf_pos)
{
    if (buffer[*buf_pos] == '{')
    {
        Node* tmp_left_node = (Node*) calloc(1, sizeof(Node));
        if (tmp_left_node == NULL)
        {
            printf("Left node callocation error!");
            return CALLOC_ERROR;
        }
        main_node->left = tmp_left_node;
        *buf_pos += 2;// переставляем указатель на начало названия
        tmp_left_node->data = (char*) &buffer[*buf_pos];

        while(isalpha(buffer[*buf_pos]) > 0 || buffer[*buf_pos] == ' ')//пропуск слова
            *buf_pos += 1;

        *buf_pos += 1; // переход через '\0'
        if (buffer[*buf_pos] == '{') TreeReadNode(buffer, tmp_left_node, buf_pos);
    }
    if (buffer[*buf_pos] == '}')// skip spases
    {
        if (buffer[*buf_pos + 3] == '}')
            return 0;
        else
            *buf_pos += 3;
    }
    if (buffer[*buf_pos] == '{')
    {
        Node* tmp_right_node = (Node*) calloc(1, sizeof(Node));
        if (tmp_right_node == NULL)
        {
            printf("Right node callocation error!");
            return CALLOC_ERROR;
        }

        main_node->right = tmp_right_node;
        *buf_pos += 2;// переставляем указатель на начало названия
        tmp_right_node->data = (char*) &buffer[*buf_pos];

        while(isalpha(buffer[*buf_pos]) > 0 || buffer[*buf_pos] == ' ')// пропуск слова
            *buf_pos += 1;

        *buf_pos += 1; // переход через '\0'
        if (buffer[*buf_pos] == '{') TreeReadNode(buffer, tmp_right_node, buf_pos);
    }
    if (buffer[*buf_pos] == '}')
    {
        if (buffer[*buf_pos + 3] == '}')
        {
            *buf_pos += 3;
            return 0;
        }
        else
            assert(0 && "Check your <C> drive)))))");
    }

    return 0;// END
}
#endif

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
        //printf("%c", tmp_ch);
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