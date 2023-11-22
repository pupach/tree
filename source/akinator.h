#ifndef AKINATOR
#define AKINATOR

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "../../Stack/source/stack.h"
#include "../../my_lib/work_with_file.h"
#include "../../my_lib/len_arr/len_array.h"
#include "tree.h"

static const int MAX_SIZE_DATA_STR = 100;

CODE_ERRORS Play_Akinator();

Node *Gen_New_Ans_Akin(Tree *tree, Node *quest);

Node *Gen_New_Quest_Akin(Tree *tree);

bool func_cmp_akin(Node *poison_node, Node *cur_quest, Tree *tree);

bool Analaze_Ans_on_quest(char right_ans[MAX_SIZE_DATA_STR], char wrong_ans[MAX_SIZE_DATA_STR]);

#endif
