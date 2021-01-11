#include <stdio.h>

#include "token.h"
#include "util.h"

char* token_to_str(token_t* t)
{
    static char buffer[128];
    switch (t->type) {
        case TK_INV:  //
            return "invalid token";
        case TK_OPR:      // +: -: *: /: ==: !=: +=:
        case TK_NUM:      // 1 ~ 9
        case TK_SEM:      // ;
        case TK_L_PAR:    // { //TODO gather the parenthes {: }: (: ): [: ]
        case TK_R_PAR:    // }
        case TK_L_R_PAR:  // (
        case TK_R_R_PAR:  // )
        case TK_ASSIGN:   // =
        case TK_UNARY:    // !: ++: --
            NOT_YET_IMPLEMETED;
        case TK_ID: {
            token_id_t* id = (token_id_t*)t;
            sprintf(buffer, "id (%s)", id->id);
            return buffer;
        }
        case TK_EOF: {
            return "eof token";
        }
        case TK_TYPE:  // uint64_t: void
        case TK_RET:   // return
        case TK_IF:
        case TK_ELSE:
        case TK_SHARP:   // #
        case TK_DQUOTE:  // "
            NOT_YET_IMPLEMETED;
    }
}
