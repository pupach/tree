#include "tree.h"
#include "akinator.h"
#include "graphic_dump.h"


bool func_cmp_akin(Node *poison_node, Node *cur_quest, Tree *tree)
{

    const int MAX_SIZE_ANS = 10;

    LOG(1, stderr, "cur_quest = %p cur_quest->prev = %p\n", cur_quest, cur_quest->prev);

    if((cur_quest->left != nullptr) and (cur_quest->left != nullptr)) //починить костыль
    {
        fprintf(stdout, "ОТВЕТЬТЕ НА ВОПРОС \n %s \n [y] - yes, [n] - no \n"
                , cur_quest->data);

        return Analaze_Ans_on_quest("yes", "no");
    }
    else
    {
        fprintf(stdout, "ОТВЕТЬТЕ НА ВОПРОС \n Это %s ? \n [y] - yes, [n] - no \n"
        , cur_quest->data);

        bool res = Analaze_Ans_on_quest("yes", "no");

        LOG(1, stderr, "Analaze_Ans_on_quest in last node res =%d \n", res);

        if(res)
        {
            Node *new_quest = Gen_New_Quest_Akin(tree);
            Node *new_ans   = Gen_New_Ans_Akin(tree, new_quest);
            Elen_s way = 0;
            HANDLER_ERROR(Stack_Pop(tree->stk_of_choise, &way));//сделать более адекватную связь с Set_Node_on_Place
            LOG(1, stderr, "cur_quest = %p cur_quest->prev = %p\n", cur_quest, cur_quest->prev);
            HANDLER_ERROR(Insert_Node_to_Tree(new_quest, cur_quest->prev, (WAY_INS_NODE)(int)way, LEFT_BRANCH));

            return false;
        }
    }
}

Node *Gen_New_Quest_Akin(Tree *tree)
{
    char *ans = (char *)calloc(MAX_SIZE_DATA_STR, sizeof(char *));

    HANDLER_ERROR(clear_buff_stdin());
    fprintf(stdout, "введите новый вопрос\n");
    int res = fscanf(stdin, "%255[^\n]s", ans);

    Node *to_ret = Create_Node(tree, ans);

    free(ans);
    return to_ret;
}


Node *Gen_New_Ans_Akin(Tree *tree, Node *quest)
{
    fprintf(stdout, "что за существо вы загадали при ответе да на вопрос"data_spec"\n" , quest->data);
    char *ans = (char *)calloc(MAX_SIZE_DATA_STR, sizeof(char *));

    HANDLER_ERROR(clear_buff_stdin());
    int res = fscanf(stdin, "%255[^\n]s", ans);
    Node *cur_node = Create_Node(tree, ans);

    quest->right = cur_node;
    cur_node->prev = quest;

    free(ans);
    return cur_node;
}


CODE_ERRORS Play_Akinator(Tree *tree)
{
    fprintf(stdout, "вы хотите загрузить данные из базы?");
    bool ans_in = Analaze_Ans_on_quest("no", "yes");

    if(ans_in or (tree==nullptr))
    {
        FILE *stream_read = open_file("database/new.txt", "r");//сделать динамическую генерацию имени
        tree = init_tree();
        len_arr *buff = read_from_file_to_buff(stream_read);

        int counter = 0;
        tree->head_node.prev = Read_from_file_Node((char *)buff->arr, tree, &counter);//переписать на CODE_ERRORS

        free_mem_buf(buff);
    }

    bool f_game = false;
    do
    {
        fprintf(stdout, "game begin\n");

        Set_Node_on_Place_in_Tree(tree, nullptr, func_cmp_akin, false);

        fprintf(stdout, "вы хотите продолжать играть?\n");

        f_game = Analaze_Ans_on_quest("yes", "no");

        LOG(1, stderr, "while end f_game=%d\n", f_game);
    }while(!f_game);

    fprintf(stdout, "вы хотите сохранить данные в базу?");
    bool ans_out = Analaze_Ans_on_quest("no", "yes");
    if(ans_out)
    {
        FILE *stream_to_write = open_file("database/new.txt", "w");//сделать динамическую генерацию имени(fstat смотреть есть ли файл)

        Write_inf_about_node_to_File(stream_to_write, tree->head_node.prev);

        fclose(stream_to_write);

        Graphiz_Dump_Tree(tree);
    }


    HANDLER_ERROR(Destructor_Tree(tree));

    return ALL_GOOD;
}
