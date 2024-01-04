#include "frozen.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int static_num_tests = 0;
#define FAIL(str, line)                                    \
  do {                                                     \
    fprintf(stderr, "Fail on line %d: [%s]\n", line, str); \
    return str;                                            \
  } while (0)

#define ASSERT(expr)                    \
  do {                                  \
    static_num_tests++;                 \
    if (!(expr)) FAIL(#expr, __LINE__); \
  } while (0)

int main() {
//    const char* str = "{a:{b:{c:4},d:5},e:\"abcaaaabaaaaaaaaaaaaaaaaaaaaafffffff\"}";
    const char* str = "{e:\"abcaaaabaaaaaaaaaaaadfhasdgfajdfjadaaghfasdkfakghaaaaaaaaaafffffff\"}";
//    int c=0,d = 0;
//    char* e = NULL;
//    ASSERT(json_scanf(str, strlen(str), "{a:{b:{c:%d},d:%d},e:%Q}", &c, &d, &e) == 3);
//    ASSERT(c == 4);
//    ASSERT(d == 5);
//    ASSERT(strcmp(e, "abc") == 0);
//    free(e);

    char rst[2560];
    int a = json_scanf(str, strlen(str), "{e :   %s}", rst);
    printf("%d  %lu  %s\n", a, strlen(rst), rst);
}