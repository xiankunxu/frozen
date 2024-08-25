#include "frozen.h"
#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

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

#define MAX_UART_BUF_SZ 200
#define Du_printf(fmt, ...)  dbgPrintf( __FUNCTION__, __LINE__, __FILE__, fmt, ## __VA_ARGS__ )
int dbgPrintf(const char* function, 
              int line,
              const char* file,
              const char* fmt, ...) 
{
    char buf[MAX_UART_BUF_SZ] = "";
    int len = snprintf(buf, sizeof(buf), "func: %s, line: %d, file: %s. ", function, line, file);

    va_list args;
    va_start( args, fmt );
    len = vsnprintf(buf+len, sizeof(buf)-len, fmt, args);
    va_end(args);

    printf("%s\n", buf);
    return len;
}


int main() {
/* json scan */
//    const char* str = "{a:{b:{c:4},d:5},e:\"abcaaaabaaaaaaaaaaaaaaaaaaaaafffffff\"}";
//    const char* str = "{e:\"abcaaaabaaaaaaaaaaaadfhasdgfajdfjadaaghfasdkfakghaaaaaaaaaafffffff\"}";
//    int c=0,d = 0;
//    char* e = NULL;
//    ASSERT(json_scanf(str, strlen(str), "{a:{b:{c:%d},d:%d},e:%Q}", &c, &d, &e) == 3);
//    ASSERT(c == 4);
//    ASSERT(d == 5);
//    ASSERT(strcmp(e, "abc") == 0);
//    free(e);

//    char rst[2560];
//    int a = json_scanf(str, strlen(str), "{e :   %s}", rst);
//    printf("%d  %lu  %s\n", a, strlen(rst), rst);


    char buf[200] = "";   // First define the buffer to hold the json string.
    struct json_out out = JSON_OUT_BUF(buf, sizeof(buf));
    const char *result = "{\"foo\": 123, \"x\": [false, true], \"y\": \"hi\"}";
    json_printf(&out, "{%Q: %d, x: [%B, %B], y: %Q}", "foo", 123, 0, -1, "hi");
//    ASSERT(strcmp(buf, result) == 0);

    size_t foo = 12;
    json_printf(&out, "%zu %d", foo, 42);

    printf("%s\n", buf);

    Du_printf("You received: %s", buf);
}