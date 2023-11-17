#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "graphic_dump.h"

int main()
{
    Tree *tree = init_tree();
    Node *node1 = Create_Node(tree, "1");
    HANDLER_ERROR(Set_new_High(tree, node1));
    Node *node2 = Create_Node(tree, "2");
    Node *node3 = Create_Node(tree, "3");
    Node *node4 = Create_Node(tree, "4");

    Insert_Node_to_Tree(node2, node1, LEFT_INS);
    Insert_Node_to_Tree(node3, node1, RIGHT_INS);
    Insert_Node_to_Tree(node4, node3, RIGHT_INS);

    Graphiz_Dump_Tree(tree);

    FILE *stream_to_write = open_file("database/new.txt", "w");//сделать динамическую генерацию имени

    Write_inf_about_node_to_File(stream_to_write, tree->beg_node);
    fclose(stream_to_write);

    FILE *stream_read = open_file("database/new.txt", "r");//сделать динамическую генерацию имени
    Tree *tree1 = init_tree();
    int counter = 0;
    len_arr *buff = read_from_file_to_buff(stream_read);
    tree1->beg_node = Read_from_file_Node((char *)buff->arr, tree1, &counter);//переписать на CODE_ERRORS
    Graphiz_Dump_Tree(tree1);

    HANDLER_ERROR(Destructor_Tree(tree));
    HANDLER_ERROR(Destructor_Tree(tree1));
    free_mem_buf(buff);
}
