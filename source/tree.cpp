#include "tree.h"

Tree *init_tree(int beg_copacity)
{
    LOG(1, stderr, "tree init begin \n");

    Tree *tree = (Tree *)calloc(sizeof(Tree), 1);
    if(tree == nullptr)     return nullptr;

    tree->copacity = beg_copacity;
    tree->list_node = (Node *)calloc(sizeof(Node), beg_copacity);
    tree->size = 0;

    Stack *stk2 = (Stack *)calloc(sizeof(Stack), 1);
    Stack_init(stk2, 20, 2);
    tree->stk_of_choise = stk2;

    LOG(1, stderr, "\n init_tree end tree->copacity=%d, tree->list_node=%p \n", tree->copacity, tree->list_node);
    return tree;
}

CODE_ERRORS Set_new_High(Tree *tree, Node *node)
{
    tree->beg_node = node;
    return ALL_GOOD;
}

#ifdef TREE_STR

Node *Read_from_file_Node(char *buff, Tree *tree, int *counter)
{
    char *data = (char *)calloc(sizeof(char), MAX_SIZE_DATA_STR);

    if(buff[*counter] != '(')
    {
        free(data);
        if(strcmp(buff + *counter, "nill") == 0)
        {
            *counter += strlen(buff + *counter) + 1;
            return nullptr;
        }
        else    exit(1);
    }
    (*counter)++;

    LOG(1, stderr, "R_F_N counter=%d, data=%s\n", *counter, buff + *counter);

    strcpy(data, buff + *counter);
    *counter += strlen(buff + *counter) + 1;

    Node *left  = Read_from_file_Node(buff, tree, counter);
    Node *right = Read_from_file_Node(buff, tree, counter);

    Node *cur_node = Create_Node(tree, data, left, right);

    if(buff[*counter] != ')')   LOG(1, stderr, "ERRRRROOOOOORRRRRRR!!!!!!!!!!!!!!");
    (*counter)++;

    free(data);
    return cur_node;
}


CODE_ERRORS Write_inf_about_node_to_File(FILE *stream_write, Node *node)
{
    fprintf(stream_write, "(%s%c", node->data, '\0');

    if(node->left != nullptr)
    {
        HANDLER_ERROR(Write_inf_about_node_to_File(stream_write, node->left));
    }
    else
    {
        fprintf(stream_write, "nill%c", '\0');
    }

    if(node->right != nullptr)
    {
        HANDLER_ERROR(Write_inf_about_node_to_File(stream_write, node->right));
    }
    else
    {
        fprintf(stream_write, "nill%c", '\0');
    }
    fprintf(stream_write, ")");
}


bool func_cmp_str(Node *node1, Node *node2, Tree *tree)
{
    if(node1->data[0] > node2->data[0])   return true;
    return false;
}

#endif

#ifdef TREE_INT

CODE_ERRORS Write_inf_about_node_to_File(FILE *stream_write, Node *node)
{
    fprintf(stream_write, "(%d%c", node->data, '\0');

    if(node->left != nullptr)
    {
        HANDLER_ERROR(Write_inf_about_node_to_File(stream_write, node->left));
    }
    else
    {
        fprintf(stream_write, "nill%c", '\0');
    }

    if(node->right != nullptr)
    {
        HANDLER_ERROR(Write_inf_about_node_to_File(stream_write, node->right));
    }
    else
    {
        fprintf(stream_write, "nill%c", '\0');
    }
    fprintf(stream_write, ")");
}

Node *Read_from_file_Node(char *buff, Tree *tree, int *counter)
{
    char *data = (char *)calloc(sizeof(char), MAX_SIZE_DATA_STR);

    if(buff[*counter] != '(')
    {
        if(strcmp(buff + *counter, "nill") == 0)
        {
            *counter += strlen(buff + *counter) + 1;
            return nullptr;
        }
        else    exit(1);
    }
    (*counter)++;

    LOG(1, stderr, "R_F_N counter=%d, data=%s\n", *counter, buff + *counter);

    strcpy(data, buff + *counter);
    *counter += strlen(buff + *counter) + 1;

    Node *left  = Read_from_file_Node(buff, tree, counter);
    Node *right = Read_from_file_Node(buff, tree, counter);

    Node *cur_node = Create_Node(tree, atoi(data), left, right);

    if(buff[*counter] != ')')   LOG(1, stderr, "ERRRRROOOOOORRRRRRR!!!!!!!!!!!!!!");
    (*counter)++;

    free(data);
    return cur_node;
}

bool func_cmp_int(Node *node1, Node *node2, Tree *tree)
{
    if(node1->data > node2->data)   return true;
    return false;
}

#endif

CODE_ERRORS Destructor_Tree(Tree *tree)
{
    Stack_Destructor(tree->stk_of_choise);
    free(tree->stk_of_choise);

    LOG(1, stderr, "Destructor tree->size = %d\n", tree->size);

    #ifdef TREE_STR
    for(int i = 0; i < tree->size; i++)
    {
        free((tree->list_node + i)->data);
        LOG(1, stderr, " tree->size = %d - free\n", i);
    }
    #endif

    free(tree->list_node);
    tree->beg_node = nullptr;
    tree->list_node = nullptr;
    tree->size = -1;
    tree->copacity = -1;

    free(tree);

    return ALL_GOOD;
}

Node *Create_Node(Tree *tree, data_type data, Node *left, Node *right, Node *prev)
{
    #ifdef TREE_STR
        char *data_to_tree = (char *)calloc(sizeof(char), MAX_SIZE_DATA_STR);
        strcpy(data_to_tree, data);
    #else
        data_type data_to_tree = data;
    #endif

    if(tree->size >= tree->copacity)
    {
        HANDLER_ERROR(Increase_copacity_Tree(tree));
    }
    Node *cur_Node = tree->list_node + tree->size;
    cur_Node->data = data_to_tree;
    cur_Node->left = left;
    cur_Node->right = right;
    cur_Node->prev = prev;

    if(left != nullptr)
    {
        left->prev = cur_Node;
    }
    if(right != nullptr)
    {
        right->prev = cur_Node;
    }

    tree->size += 1;
    return cur_Node;//убрать возможность делать узлы с помощь.create_node or Insert_Node_to_Tree
}

CODE_ERRORS Increase_copacity_Tree(Tree *tree)
{
    tree->copacity = tree->copacity * tree->factor_copacity;
    int numb_beg_node = (tree->beg_node - tree->list_node) / sizeof(Node);
    if (numb_beg_node < 0)              return NO_NUMBER;

    tree->list_node = (Node *)realloc(tree->list_node, tree->copacity);
    if(tree->list_node == nullptr)      return PTR_NULL;

    tree->beg_node = tree->list_node + numb_beg_node;
    return ALL_GOOD;
}

CODE_ERRORS Insert_Node_to_Tree(Node *node_to_ins, Node *node_after_ins, WAY_INS_NODE ins_mode, WAY_CPY_BRANCH cpy_mode)
{
    assert(node_to_ins != nullptr);
    assert(node_after_ins != nullptr);

    Node *branch_to_cpy = nullptr;
    node_to_ins->prev = node_after_ins;

    LOG(1, stderr, "node_to_ins = %p node_to_ins->prev = %p\n", node_to_ins, node_to_ins->prev);

    if (ins_mode == LEFT_INS)
    {
        branch_to_cpy = node_after_ins->left;
        node_after_ins->left = node_to_ins;
    }
    else if(ins_mode == RIGHT_INS)
    {
        branch_to_cpy = node_after_ins->right;
        node_after_ins->right = node_to_ins;
    }
    else    return UNDEF_COM;

    if(cpy_mode == LEFT_BRANCH)         node_to_ins->left = branch_to_cpy;

    else if(cpy_mode == RIGHT_BRANCH)   node_to_ins->right = branch_to_cpy;

    else if(cpy_mode != NO_BRANCH)      return UNDEF_COM;

    return ALL_GOOD;
}

int Set_Node_on_Place_in_Tree(Tree *tree, Node *cur_node, bool func_cmp(Node *, Node *, Tree *), bool flag_add)
{
    Node *save_top_node = tree->beg_node;
    int code_ret = 0;
    bool result = func_cmp(cur_node, tree->beg_node, tree);
    if(result)
    {
        if(tree->beg_node->right != nullptr)
        {
            HANDLER_ERROR(Stack_Push(tree->stk_of_choise, (Elen_s) RIGHT_CHOISE));
            tree->beg_node = tree->beg_node->right;
            code_ret = Set_Node_on_Place_in_Tree(tree, cur_node, func_cmp, flag_add);
        }
        else if(flag_add)
        {
            HANDLER_ERROR(Insert_Node_to_Tree(cur_node, tree->beg_node, RIGHT_INS));
        }
    }
    else
    {
        if(tree->beg_node->left != nullptr)
        {
            HANDLER_ERROR(Stack_Push(tree->stk_of_choise, (Elen_s) LEFT_CHOISE));
            tree->beg_node = tree->beg_node->left;
            code_ret = Set_Node_on_Place_in_Tree(tree, cur_node, func_cmp, flag_add);
        }
        else if(flag_add)
        {
            HANDLER_ERROR(Insert_Node_to_Tree(cur_node, tree->beg_node, LEFT_INS));
        }
    }

    tree->beg_node = save_top_node;
    return code_ret + 1;
}

