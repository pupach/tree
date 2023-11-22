#include "graphic_dump.h"


CODE_ERRORS Graphiz_Dump_Tree(Tree *tree)
{
    char *filename = Create_Filename_by_Time();

    FILE *stream_write = fopen(filename, "w");   //обернуть в define
    if(stream_write == nullptr)     return PTR_NULL;

    HANDLER_ERROR(Write_Graphiz_Dump_to_File_Tree(stream_write, tree));

    fclose(stream_write);

    HANDLER_ERROR(Create_Png_And_Show(filename));

    free(filename);
}

CODE_ERRORS Recr_Write_Node_Dump(FILE *stream_write, Node *cur_node, int deep_recr)
{
        LOG(1, stderr, "Write_Graphiz_Dump_to_File_Tree i=%d, node = %p, data="data_spec, deep_recr ,cur_node, cur_node->data);

        const char *color = "#fff3e0";
        fprintf(stream_write,
                "node%p[label = \""data_spec"\", fillcolor = \"%s\", rank = %d];\n",
                cur_node, cur_node->data,
                color, deep_recr);

        if(cur_node->left != nullptr)    Recr_Write_Node_Dump(stream_write, cur_node->left, deep_recr + 1);

        if(cur_node->right != nullptr)    Recr_Write_Node_Dump(stream_write, cur_node->right, deep_recr + 1);

        return ALL_GOOD;
}

CODE_ERRORS Write_Graphiz_Dump_to_File_Tree(FILE *stream_write, Tree *tree)
{

    fprintf(stream_write, "digraph\n{\n");

    fprintf(stream_write, "node [shape = Mrecord, color  = \"navy\", style = \"filled\"];\n");

    HANDLER_ERROR(Recr_Write_Node_Dump(stream_write, tree->beg_node, 0));

    /*
    fprintf(stream_write, "fillcolor = \"#33ff66\"];\n");

    fprintf(stream_write, "\nedge [color = \"orange\"];\n\n");
    Elem_list cur_elem= list->arr[0];

    fprintf(stream_write, "node%d -> node%d;\n", 0, cur_elem.val);

    fprintf(stream_write, "\nedge [color = \"cornFlowerBlue\"];\n\n");*/
    Node *cur_node = tree->list_node;

    for (int i = 0; i <= tree->size; i++)
    {
        if((cur_node + i)->left != nullptr)
        {
            fprintf(stream_write, "node%p -> node%p;\n", (cur_node + i), (cur_node + i)->left);
        }
        if((cur_node + i)->right != nullptr)
        {
            fprintf(stream_write, "node%p -> node%p;\n", (cur_node + i), (cur_node + i)->right);
        }
    }
    fprintf(stream_write, "}\n");

    return ALL_GOOD;
}
