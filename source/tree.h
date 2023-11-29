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
#include "../../my_lib/len_arr/len_array.h"


#define TREE_STR

#ifdef TREE_STR

typedef char * data_type;
#define data_spec "%s"

#endif

#ifdef TREE_INT

typedef int data_type;
#define data_spec "%d"

#endif

#ifdef TREE_DOUBLE

typedef double data_type;
#define data_spec "%lf"

#endif




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
    data_type data;//TODO: structиз enum and uint
    Node* left;
    Node* right;
    Node *prev;
    bool ans_or_quest = false; //
    int type_data = 0;
};

struct Tree
{
    Node *beg_node;
    int size = -1;
    Node *list_node;
    int copacity = -1;
    int factor_copacity = 2;
    Stack *stk_of_choise;
    Node head_node;
};

Tree *init_tree(int beg_copacity=10);

CODE_ERRORS Destructor_Tree(Tree *tree);

#ifdef TREE_INT
Node *Read_from_file_Node(char *buff, Tree *tree, int *counter);

CODE_ERRORS Write_inf_about_node_to_File(FILE *stream_write, Node *node);

bool func_cmp_int(Node *node1, Node *node2, Tree *tree);
#endif


#ifdef TREE_STR
CODE_ERRORS Write_inf_about_node_to_File(FILE *stream_write, Node *node);

Node *Read_from_file_Node(char *buff, Tree *tree, int *counter);

bool func_cmp_str(Node *node1, Node *node2, Tree *tree);
#endif
CODE_ERRORS Set_new_High(Tree *tree, Node *node);

int Set_Node_on_Place_in_Tree(Tree *tree, Node *cur_node, bool func_cmp(Node *, Node *, Tree *), bool flag_add=true);

CODE_ERRORS Increase_copacity_Tree(Tree *tree);

Node *Free_all_data_in_Node(Node *top_node);

CODE_ERRORS Shift_on_delta_mem_list_node(Tree *tree, long int delta_mem);

Node *Create_Node(Tree *tree, data_type data, Node *left=nullptr, Node *right=nullptr, Node *prev=nullptr);

CODE_ERRORS Insert_Node_to_Tree(Node *node_to_ins, Node *node_after_ins, WAY_INS_NODE ins_mode, WAY_CPY_BRANCH cpy_mode=NO_BRANCH);
#endif
