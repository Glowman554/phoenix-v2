#include <math_expr.h>
#include <parser.h>

int interpret_expression() {
    // current token + interpret_expression(next_token)
    return get_value() + interpret_expression();
}

int get_value() {
    return 0;
}