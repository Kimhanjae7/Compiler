/* parser.c-단순 산술식에 대한 구문 분석기 시스템 */
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "Lexical_analyzer.c"

extern int nextToken;
extern FILE* in_fp;

/* 토큰 코드들 */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/* 함수선언 */
int term();
int factor();
int lex();
void error();
void getChar();

/* 함수 expr
다음 규칙으로 생성되는 언어의 문자열을 파싱한다:
<expr> → <term> {( + | - ) <term>}
*/
int expr() {
    //printf("Enter <expr>\n");
    int result = term();

    /* 첫 번째 term 을 파싱한다 */
    /* 다음 번째 토큰이 + 또는 -이면, 다음 번째 토큰을 가져오고, 다음 번째 term 을 파싱한다 */
    while (nextToken == ADD_OP || nextToken == SUB_OP) {
        int op = nextToken;
        lex();
        int termValue = term();
        if (op == ADD_OP) {
            result += termValue;
        } else {
            result -= termValue;
        }
    }

    //printf("Exit <expr>\n");
    return result;
}

/* 함수 term
다음 규칙으로 생성되는 언어의 문자열을 파싱한다:
<term> → <factor> {( * | / ) <factor>)
*/
int term() {
    //printf("Enter <term>\n");
    int result = factor();

    /* 첫 번째 factor 를 파싱한다 */
    /* 다음 번째 토큰이 * 또는 / 이면, lex 를 호출하여 다음 번째 토큰을 가져오고, 다음 번째 factor 를 파싱한다 */
    while (nextToken == MULT_OP || nextToken == DIV_OP) {
        int op = nextToken;
        lex();
        int factorValue = factor();
        if (op == MULT_OP) {
            result *= factorValue;
        } else {
            result /= factorValue;
        }
    }

    //printf("Exit <term>\n");
    return result;
}

/* 함수 factor
다음 규칙으로 생성되는 언어의 문자열을 파싱한다:
<factor> → id | int_constant | (<expr>)
*/
int factor() {
    //printf("Enter <factor>\n");
    /* 어느 RHS 를 파싱할 것인지를 결정한다 */
    int result = 0;

    if (nextToken == IDENT || nextToken == INT_LIT) {
        result = atoi(lexeme);
        lex();
    }
    /* RHS 가 (<expr>)이면, lex 를 호출하여 왼쪽 괄호를 전달하고, expr 을 호출하고, 오른쪽 괄호를 검사한다 */
    else if (nextToken == LEFT_PAREN) {
        lex();
        result = expr();
        if (nextToken == RIGHT_PAREN)
            lex();
        else
            error();
    }
    /* id, 정수리터럴, 또는 왼쪽 괄호가 아니다 */
    else {
        error();
    }

    //printf("Exit <factor>\n");
    return result;
}

void error() {
    printf("Syntax error\n");
}

/* main 구동기 */
int main() {
    char c[100];
    if ((in_fp = fopen("front.in", "w")) == NULL)
        printf("ERROR-cannot open front.in \n");
    else {
        printf("입력 : ");
        scanf("%s", c);
        fprintf(in_fp, "%s", c);
        fclose(in_fp);
    }
    printf("\n");

    /* 입력 데이터 파일을 열고 그 내용을 처리 */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR-cannot open front.in \n");
    else {
        getChar();
        do {
            lex();
            int result = expr();
			printf("----------------------\n\n");
            printf("계산 결과: %d\n\n", result);
        } while (nextToken != EOF);
    }

    return 0;
}
