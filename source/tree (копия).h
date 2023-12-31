#ifndef TREE
#define TREE

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "../../Stack/source/stack.h"
#include "../../my_lib/work_with_file.h"

typedef int data_type;

enum WAY_CPY_BRANCH
{
    LEFT_BRANCH = -1,
    RIGHT_BRANCH = 1,
    NO_BRANCH = 0
};

enum WAY_OF_CHOISE
{
    LEFT_CHOISE = -1,
    RIGHT_CHOISE = 1
};

enum WAY_INS_NODE
{
    LEFT_INS = -1,
    RIGHT_INS = 1
};

struct Node
{
    data_type data;
    Node* left;
    Node* right;
    Node *prev;
};

struct Tree
{
    Node *beg_node;
    int size = -1;
    Node *list_node;
    int copacity = -1;
    size_t factor_copacity = 2;
    Stack *stk_of_choise;
};

Tree *init_tree(int beg_copacity=10);

CODE_ERRORS Destructor_Tree(Tree *tree);

CODE_ERRORS Write_inf_about_tree_to_File(FILE *stream_write, Tree *tree);

CODE_ERRORS Write_inf_about_node_to_File_int(FILE *stream_write, Node *node);

CODE_ERRORS Write_inf_about_node_to_File_str(FILE *stream_write, Node *node);

CODE_ERRORS Set_new_High(Tree *tree, Node *node);

int Set_Node_on_Place_in_Tree(Tree *tree, Node *cur_node, bool func_cmp(Node *, Node *), bool flag_add=true);

CODE_ERRORS Increase_copacity_Tree(Tree *tree);

Node *Create_Node(Tree *tree, data_type data, Node *left=nullptr, Node *right=nullptr, Node *prev=nullptr);

CODE_ERRORS Insert_Node_to_Tree(Node *node_to_ins, Node *node_after_ins, WAY_INS_NODE ins_mode, WAY_CPY_BRANCH cpy_mode=NO_BRANCH);
#endif
