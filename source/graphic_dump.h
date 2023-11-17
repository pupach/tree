#ifndef GRAPHIC_DUMP
#define GRAPHIC_DUMP

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <string.h>
#include "../../Stack/source/stack.h"
#include "../../my_lib/work_with_file.h"
#include "tree.h"

CODE_ERRORS Write_Graphiz_Dump_to_File_Tree(FILE *stream_write, Tree *tree);

CODE_ERRORS Graphiz_Dump_Tree(Tree *tree);

CODE_ERRORS Recr_Write_Node_Dump(FILE *stream_write, Node *cur_node, int deep_recr);

#endif
