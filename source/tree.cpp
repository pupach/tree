#include "tree.h"

Tree *init_tree(int beg_copacity)
{
    LOG(1, stderr, "tree init begin \n");

    Tree *tree = (Tree *)calloc(sizeof(Tree), 1);
    tree->factor_copacity = 2;
    if(tree == nullptr)     return nullptr;

    tree->copacity = beg_copacity;
    tree->list_node = (Node *)calloc(sizeof(Node), beg_copacity);
    tree->size = 0;

    tree->head_node.right = tree->list_node;
    tree->list_node[0].right = tree->list_node + 1;
    tree->list_node[0].left = &(tree->head_node);
    for(int i = 1; i < (beg_copacity - 1); i++)
    {
        tree->list_node[i].right = tree->list_node + i + 1;
        tree->list_node[i].left  = tree->list_node + i - 1;
    }
    tree->list_node[beg_copacity - 1].left = tree->list_node + beg_copacity - 2;
    tree->head_node.left = tree->list_node + beg_copacity - 1;

    Stack *stk2 = (Stack *)calloc(sizeof(Stack), 1);
    Stack_init(stk2, 20, 2);
    tree->stk_of_choise = stk2;

    LOG(1, stderr, "\n init_tree end tree->copacity=%d, tree->list_node=%p \n", tree->copacity, tree->list_node);
    return tree;
}

CODE_ERRORS Set_new_High(Tree *tree, Node *node)
{
    tree->head_node.prev = node;
    return ALL_GOOD;
}

#ifdef TREE_STR

Node *Read_from_file_Node(char *buff, Tree *tree, int *counter)
{
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

    char *data = (char *)calloc(sizeof(char), MAX_SIZE_DATA_STR);

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


Node *Free_all_data_in_Node(Node *top_node)
{
    if((top_node == nullptr) || (top_node->prev == nullptr))
    {
        return top_node;
    }
    Node *last_free_node = nullptr;

    if(top_node->left != nullptr)
    {
        last_free_node = Free_all_data_in_Node(top_node->left);
        if(last_free_node == nullptr)       return nullptr;

        top_node->prev = nullptr;
        top_node->left = last_free_node;
        last_free_node->right = top_node->left;
    }

    if(top_node->right != nullptr)
    {
        last_free_node = Free_all_data_in_Node(top_node->right);
        if(last_free_node == nullptr)       return nullptr;

        top_node->right = last_free_node;
        last_free_node->left = top_node;
    }
    #ifdef TREE_STR
        free(top_node->data);
    #endif

    return last_free_node->right;
}


CODE_ERRORS Destructor_Tree(Tree *tree)
{
    Stack_Destructor(tree->stk_of_choise);
    free(tree->stk_of_choise);

    LOG(1, stderr, "Destructor tree->size = %d\n", tree->size);

    #ifdef TREE_STR
        Free_all_data_in_Node(tree->head_node.prev);
    #endif

    free(tree->list_node);
    tree->head_node.prev = nullptr;
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

    LOG(1, stderr, "Create_Node, cop_factor = %d \n", tree->factor_copacity);

    if(tree->size >= tree->copacity)
    {
        HANDLER_ERROR(Increase_copacity_Tree(tree));
    }
    Node *cur_Node   = tree->head_node.right;
    tree->head_node.right = cur_Node->right;
    cur_Node->left = &tree->head_node;

    cur_Node->data   = data_to_tree;
    cur_Node->left   = left;
    cur_Node->right  = right;
    cur_Node->prev   = prev;

    if(left != nullptr)
    {
        left->prev = cur_Node;
    }
    if(right != nullptr)
    {
        right->prev = cur_Node;
    }

    tree->size += 1;
    return cur_Node;
}

CODE_ERRORS Increase_copacity_Tree(Tree *tree)
{
    int new_copacity = tree->copacity * tree->factor_copacity * 2;
    LOG(1, stderr, "Increase_copacity_Tree new_copacity = %d,copacity = %d tree->factor_copacity = %d \n", new_copacity, tree->copacity, tree->factor_copacity);

    Node *new_list_node = (Node *)realloc(tree->list_node, new_copacity);
    if(new_list_node == nullptr)      return PTR_NULL;

    long int delta_mem = (long int)(new_list_node) - (long int)(tree->list_node);
    tree->list_node = new_list_node;
    HANDLER_ERROR(Shift_on_delta_mem_list_node(tree, delta_mem));

    tree->head_node.left = tree->list_node + tree->copacity;
    for(int i = tree->copacity; i < new_copacity - 1; i++)
    {
        tree->list_node[i].right = tree->list_node + i + 1;
        tree->list_node[i].left  = tree->list_node + i - 1;
    }
    tree->list_node[new_copacity - 1].left = tree->list_node + new_copacity - 2;
    tree->head_node.left = tree->list_node + new_copacity - 1;

    tree->copacity = new_copacity;
    return ALL_GOOD;
}

CODE_ERRORS Shift_on_delta_mem_list_node(Tree *tree, long int delta_mem)
{
    for(int i = 0; i < tree->size; i++)
    {
        tree->list_node[i].left = (Node *)((long int)tree->list_node[i].left + delta_mem);
        tree->list_node[i].right = (Node *)((long int)tree->list_node[i].right + delta_mem);
        tree->list_node[i].prev = (Node *)((long int)tree->list_node[i].prev + delta_mem);
    }
    tree->head_node.prev += delta_mem;

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

    else if(cpy_mode == NO_BRANCH)      Free_all_data_in_Node(branch_to_cpy);

    else                                return UNDEF_COM;

    return ALL_GOOD;
}

// TODO add documentation
int Set_Node_on_Place_in_Tree(Tree *tree, Node *cur_node, bool func_cmp(Node *, Node *, Tree *), bool flag_add)
{
    Node *save_top_node = tree->head_node.prev;
    int code_ret = 0;
    bool result = func_cmp(cur_node, tree->head_node.prev, tree);
    if(result)
    {
        if(tree->head_node.prev->right != nullptr)
        {
            HANDLER_ERROR(Stack_Push(tree->stk_of_choise, (Elen_s) RIGHT_CHOISE)); // TODO do smth with stack
            tree->head_node.prev = tree->head_node.prev->right; // TODO not modify beg_node by functions
            code_ret = Set_Node_on_Place_in_Tree(tree, cur_node, func_cmp, flag_add);
        }
        else if(flag_add)
        {
            HANDLER_ERROR(Insert_Node_to_Tree(cur_node, tree->head_node.prev, RIGHT_INS));
        }
    }
    else
    {
        if(tree->head_node.prev->left != nullptr)
        {
            HANDLER_ERROR(Stack_Push(tree->stk_of_choise, (Elen_s) LEFT_CHOISE));
            tree->head_node.prev = tree->head_node.prev->left;
            code_ret = Set_Node_on_Place_in_Tree(tree, cur_node, func_cmp, flag_add);
        }
        else if(flag_add)
        {
            HANDLER_ERROR(Insert_Node_to_Tree(cur_node, tree->head_node.prev, LEFT_INS));
        }
    }

    tree->head_node.prev = save_top_node;
    return code_ret + 1;
}

