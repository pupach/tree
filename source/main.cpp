#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"
#include "akinator.h"
#include "graphic_dump.h"

int main()
{
    Tree *tree = init_tree();
    Node *node1 = Create_Node(tree, "он живой?");
    Node *node2 = Create_Node(tree, "он плаваеvт?");
    Node *node3 = Create_Node(tree, "не кот");
    node3->ans_or_quest = true;
    Node *node4 = Create_Node(tree, "кошкомальчик");
    node4->ans_or_quest = true;
    Node *node5 = Create_Node(tree, "кот");
    node5->ans_or_quest = true;

    HANDLER_ERROR(Set_new_High(tree, node1));
    Insert_Node_to_Tree(node2, node1, LEFT_INS);
    Insert_Node_to_Tree(node3, node1, RIGHT_INS);
    Insert_Node_to_Tree(node4, node2, RIGHT_INS);
    Insert_Node_to_Tree(node5, node2, LEFT_INS);

    Play_Akinator();

    Graphiz_Dump_Tree(tree);

    HANDLER_ERROR(Destructor_Tree(tree));
}
