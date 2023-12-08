#include "tree.h"

Tree *init_tree(int beg_copacity)
{
    LOG(1, stderr, "tree init begin \n");

    Tree *tree = (Tree *)calloc(sizeof(Tree), 1);
    tree->factor_copacity = 2;
    if(tree == nullptr)     return nullptr;

    tree->copacity = beg_copacity;
    tree->size = 0;
    tree->last_stor_node = New_stor_Node(tree->copacity);

    tree->head_node.right = tree->last_stor_node->list_node;
    tree->head_node.left = tree->last_stor_node->list_node + beg_copacity - 1;

    Stack *stk2 = (Stack *)calloc(sizeof(Stack), 1);
    Stack_init(stk2, 20, 2);
    tree->stk_of_choise = stk2;

    LOG(1, stderr, "\n init_tree end tree->copacity=%d, tree->last_stor_node->list_nodelast_stor_node->list_node=%p \n", tree->copacity, tree->last_stor_node->list_node);
    return tree;
}

Storage_Node *New_stor_Node(int copacity)
{
    if(copacity > 0)
    {
        Storage_Node *for_ret = (Storage_Node *)calloc(1, sizeof(Storage_Node));
        if(for_ret != nullptr)
        {
            for_ret->copacity = copacity;

            for_ret->list_node = (Node *)calloc(for_ret->copacity, sizeof(Node));

            LOG(2, stderr, "New_stor_Node last_stor_node->list_node = %p \n", for_ret->list_node);

            if(for_ret->list_node != nullptr)
            {
                for_ret->list_node[0].right = for_ret->list_node + 1;
                for(int i = 1; i < (for_ret->copacity - 1); i++)
                {
                    for_ret->list_node[i].right = for_ret->list_node + i + 1;
                    for_ret->list_node[i].left = for_ret->list_node + i - 1;
                }
                for_ret->list_node[for_ret->copacity - 1].left = for_ret->list_node + for_ret->copacity - 2;
                return for_ret;
            }
        }
    }
    return nullptr;
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
    LOG(1, stderr, "hkjjkhkjh/n");

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
    LOG(1, stderr, "may/n");
        free(top_node->data);
    #endif
    if(last_free_node == nullptr) return top_node;

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

    HANDLER_ERROR(Free_all_Stor_node(tree->last_stor_node));
    tree->head_node.prev = nullptr;
    tree->size = -1;
    tree->copacity = -1;

    free(tree);

    return ALL_GOOD;
}

CODE_ERRORS Free_all_Stor_node(Storage_Node *last_stor_node)
{
    Storage_Node *next_stor_node = last_stor_node;

    while(next_stor_node != nullptr)
    {
        LOG(2, stderr, "Free_all_Stor_node last_stor_node->list_node = %p \n", last_stor_node->list_node);

        next_stor_node = last_stor_node->prev;
        free(last_stor_node->list_node);
        free(last_stor_node);
        last_stor_node = next_stor_node;
    }
}

Node *Create_Node(Tree *tree, data_type data, Node *left, Node *right, Node *prev)
{
    #ifdef TREE_STR
        char *data_to_tree = (char *)calloc(sizeof(char), MAX_SIZE_DATA_STR);
        strcpy(data_to_tree, data);
    #else
        data_type data_to_tree = data;
    #endif

 //   LOG(1, stderr, "Create_Node, cop_factor = %d \n", tree->factor_copacity);

    if(tree->size >= (tree->copacity - 2))
    {
        HANDLER_ERROR(Increase_copacity_Tree(tree));
    }
    Node *cur_Node   = tree->head_node.right;
    assert(cur_Node != nullptr);

    tree->head_node.right = cur_Node->right;

    HANDLER_ERROR(Tree_data_init(cur_Node, data));
    cur_Node->left  = left;
    cur_Node->right = right;
    cur_Node->prev  = prev;

    if(left != nullptr)
    {
        left->prev  = cur_Node;
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
    int new_copacity = tree->copacity * (tree->factor_copacity - 1);
    LOG(1, stderr, "Increase_copacity_Tree new_copacity = %d,copacity = %d tree->factor_copacity = %d \n", new_copacity, tree->copacity, tree->factor_copacity);

    Storage_Node *new_stor = New_stor_Node(new_copacity);
    if(new_stor == nullptr)            return PTR_ARR_NULL;

    tree->copacity += new_copacity;
    tree->last_stor_node->next = new_stor;
    new_stor->prev = tree->last_stor_node;
    tree->last_stor_node = new_stor;

    tree->head_node.right = tree->last_stor_node->list_node;
    tree->head_node.left = tree->last_stor_node->list_node + new_copacity - 1;

    return ALL_GOOD;
}

/*
CODE_ERRORS Shift_on_delta_mem_list_node(Tree *tree, long int delta_mem)
{
    for(int i = 0; i < tree->size; i++)
    {
        tree->list_node[i].left  = (Node *)((long int)tree->list_node[i].left + delta_mem);
        tree->list_node[i].right = (Node *)((long int)tree->list_node[i].right + delta_mem);
        tree->list_node[i].prev  = (Node *)((long int)tree->list_node[i].prev + delta_mem);
    }
    tree->head_node.prev += delta_mem;

    return ALL_GOOD;
}
*/

CODE_ERRORS Insert_Node_to_Tree(Node *node_to_ins, Node *node_after_ins, WAY_INS_NODE ins_mode, WAY_CPY_BRANCH cpy_mode)
{
    if((node_to_ins == nullptr) | (node_after_ins == nullptr))
    {
        LOG(1, stderr, "Insert_Node_to_Tree node_to_ins = %p  node_after_ins = %p\n", node_to_ins, node_after_ins);
        return INS_NULL;
    }

    Node *branch_to_cpy = nullptr;
    node_to_ins->prev   = node_after_ins;

//    LOG(1, stderr, "node_to_ins = %p node_to_ins->prev = %p\n", node_to_ins, node_to_ins->prev);

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

    if(cpy_mode == LEFT_BRANCH)         node_to_ins->left   = branch_to_cpy;

    else if(cpy_mode == RIGHT_BRANCH)   node_to_ins->right  = branch_to_cpy;

    else if(cpy_mode == NO_BRANCH)      Free_all_data_in_Node(branch_to_cpy);

    else                                return UNDEF_COM;

    return ALL_GOOD;
}


int Set_Node_on_Place_in_Tree(Tree *tree, Node *cur_root, Node *cur_node, bool func_cmp(Node *, Node *, Tree *), bool flag_add)
{
    int code_ret = 0;
    bool result = func_cmp(cur_node, cur_root, tree);
    if(result)
    {
        if(cur_root->right != nullptr)
        {
            HANDLER_ERROR(Stack_Push(tree->stk_of_choise, (Elen_s) RIGHT_CHOISE)); // TODO do smth with stack
            code_ret = Set_Node_on_Place_in_Tree(tree, cur_root->right, cur_node , func_cmp, flag_add);
        }
        else if(flag_add)
        {
            HANDLER_ERROR(Insert_Node_to_Tree(cur_node, cur_root, RIGHT_INS));
        }
    }
    else
    {
        if(cur_root->left != nullptr)
        {
            HANDLER_ERROR(Stack_Push(tree->stk_of_choise, (Elen_s) LEFT_CHOISE));
            code_ret = Set_Node_on_Place_in_Tree(tree, cur_root->left , cur_node, func_cmp, flag_add);
        }
        else if(flag_add)
        {
            HANDLER_ERROR(Insert_Node_to_Tree(cur_node, cur_root, LEFT_INS));
        }
    }
    return code_ret + 1;
}

Node *Place_Node_from_Stack(Tree *tree, int am_choise)
{
    Node *beg_node = tree->head_node.prev;
    Elen_s elem_s;
    for(int i = 0; i < am_choise; i++)
    {
        HANDLER_ERROR(Stack_Pop(tree->stk_of_choise, &elem_s));
        switch(elem_s)
        {
            case LEFT_CHOISE:
                beg_node = beg_node->left;
                break;

            case RIGHT_CHOISE:
                beg_node = beg_node->right;
                break;
        }
    }
    return beg_node;
}

#ifdef TREE_DOUBLE

CODE_ERRORS Tree_data_init(Node *cur_node, data_type data)
{
    cur_node->data.data = data.data;
    cur_node->data.type_data = data.type_data;
    return ALL_GOOD;
}

#endif
