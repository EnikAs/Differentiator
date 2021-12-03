#include "dtor.h"

int main()
{
    FILE* inputfile_before = fopen("tree_in.txt", "r");
    FILE* inputfile_after = fopen("tree_in1.txt", "w");
    CreateTreeFile(inputfile_before, inputfile_after);
    fclose(inputfile_before);
    fclose(inputfile_after);
    inputfile_after = fopen("tree_in1.txt", "r");

    differentiator(inputfile_after);

    return 0;
}
/*

{

{
x
}
^
{
3
}

}
*/