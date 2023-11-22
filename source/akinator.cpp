#include "tree.h"
#include "akinator.h"


bool Analaze_Ans_on_quest(char right_ans[MAX_SIZE_DATA_STR], char wrong_ans[MAX_SIZE_DATA_STR])
{
    char ans[MAX_SIZE_DATA_STR] = {};
    do
    {
        int res = fscanf(stdin, "%s", ans);

        if(strcmp(ans, right_ans) == 0)
        {
            return false;
        }
        else if(strcmp(ans, wrong_ans) == 0)
        {
            return true;
        }
        else
        {
            fprintf(stdout, "undef input");
            continue;
        }
    }while(false);
}


bool func_cmp_akin(Node *poison_node, Node *cur_quest, Tree *tree)
{

    const int MAX_SIZE_ANS = 10;

    if(!cur_quest->ans_or_quest)
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

        if(res)
        {
            Node *new_quest = Gen_New_Quest_Akin(tree);
            Node *new_ans   = Gen_New_Ans_Akin(tree, new_quest);
            Elen_s way = 0;
            HANDLER_ERROR(Stack_Pop(tree->stk_of_choise, &way));//сделать более адекватную связь с Set_Node_on_Place
            HANDLER_ERROR(Insert_Node_to_Tree(new_quest, cur_quest->prev, (WAY_INS_NODE)(int)way, LEFT_BRANCH));

            return false;
        }
    }
}

Node *Gen_New_Quest_Akin(Tree *tree)
{
    char ans[MAX_SIZE_DATA_STR] = {};

    fprintf(stdout, "введите новый вопрос");
    int res = fscanf(stdin, "%s", ans);

    return Create_Node(tree, ans);
}


Node *Gen_New_Ans_Akin(Tree *tree, Node *quest)
{
    fprintf(stdout, "что за существо вы загадали при ответе да на вопрос"data_spec , quest->data);
    char ans[MAX_SIZE_DATA_STR] = {};

    int res = fscanf(stdin, "%s", ans);
    Node *cur_node = Create_Node(tree, ans);

    quest->right = cur_node;
    cur_node->prev = quest;

    return cur_node;
}


CODE_ERRORS Play_Akinator()
{
    FILE *stream_read = open_file("database/new.txt", "r");//сделать динамическую генерацию имени
    Tree *tree1 = init_tree(); //сделать вопрос-ответ
    len_arr *buff = read_from_file_to_buff(stream_read);
    int counter = 0;
    tree1->beg_node = Read_from_file_Node((char *)buff->arr, tree1, &counter);//переписать на CODE_ERRORS

    bool f_game = false;
    do
    {
        fprintf(stdout, "game begin\n");

        Set_Node_on_Place_in_Tree(tree1, nullptr, func_cmp_akin, false);

        fprintf(stdout, "вы хотите продолжать играть?\n");

        f_game = Analaze_Ans_on_quest("yes", "no");

        LOG(1, stderr, "while end f_game=%d\n", f_game);
    }while(!f_game);

    HANDLER_ERROR(Destructor_Tree(tree1));//сделать окончание игры
    free_mem_buf(buff);

    return ALL_GOOD;
}
