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
//    json_printf(&out, "%f, %.2f ", 1.36, 0.23);
    const char *result = "{\"foo\": 123, \"x\": [false, true], \"y\": \"hi\"}";
    int len = json_printf(&out, "{%Q: %d, x: [%B, %B], y: %Q, \"%d-%d-%d\"}", "foo", 123, 0, -1, "hi", 1, 2, 3);
    printf("len = %d\n", len);
    printf("buf = %s\n", buf);
    len = json_printf(&out, "%zu %d", 12, 42);
    printf("len = %d\n", len);
    printf("buf = %s\n", buf);

//    ASSERT(strcmp(buf, result) == 0);

//    size_t foo = 12;
//    len = json_printf(&out, "%zu %d %f %f", foo, 42, 1.2, 2.3);
//    printf("len = %d\n", len);

    char* str = "ad;gha;fdklgdjagekl2!#$!%!$@%&*()_+|}{:<>?~`==+";
    memcpy(buf + len, str, strlen(str));
    len += strlen(str);
    buf[len] = '\0';

    printf("%s\n", buf);

    Du_printf("You received: %s", buf);


    int foo_int;
    char rst[100] = "";
    json_scanf(buf, strlen(buf), "{foo: %d, y: %s}", &foo_int, rst);

    printf("foo = %d, rst = %s\n", foo_int, rst);

    str = "{a:{b:4}, cmd: 0x12, c:\"abc\"}";
    char* a = NULL;
    int b = 0;
    char* c = NULL;
    int cmd = 0;
    len = json_scanf(str, strlen(str), "{a:%Q,{b:%d},c:%Q,cmd:%d}", &a, &b, &c, &cmd);
    printf("a = %s, b = %d, c = %s, cmd = %d\n", a, b, c, cmd);
    free(a);
    free(c);

    /* Test base64 encode/decode via the %V format specifier.
     * Requires JSON_ENABLE_BASE64=1 (set in Makefile CFLAGS). */
    {
        char b64buf[256] = "";
        struct json_out b64out = JSON_OUT_BUF(b64buf, sizeof(b64buf));
        /* Arbitrary bytes including non-printable ones to ensure binary-safe round trip. */
        const unsigned char original[] = {
            0x00, 0x01, 0x02, 'H', 'e', 'l', 'l', 'o', ' ',
            'B', '6', '4', '!', 0xff, 0xfe, 0x7f
        };
        int orig_len = (int) sizeof(original);
        int n, i;
        char *decoded = NULL;
        int decoded_len = 0;

        n = json_printf(&b64out, "{data: %V, note: %Q}", original, orig_len, "b64-test");
        printf("b64 encoded (n=%d): %s\n", n, b64buf);

        if (json_scanf(b64buf, strlen(b64buf), "{data: %V}", &decoded, &decoded_len) == 1) {
            printf("b64 decoded_len = %d (orig_len = %d)\n", decoded_len, orig_len);
            printf("b64 decoded bytes: ");
            for (i = 0; i < decoded_len; i++) {
                printf("%02x ", (unsigned char) decoded[i]);
            }
            printf("\n");

            if (decoded_len == orig_len &&
                memcmp(decoded, original, orig_len) == 0) {
                printf("b64 roundtrip: PASSED\n");
            } else {
                printf("b64 roundtrip: FAILED\n");
            }
            free(decoded); // json_scanf_cb's 'V' case mallocs the decoded buffer, so we need to free it here.
        } else {
            printf("b64 roundtrip: FAILED (json_scanf did not convert %%V)\n");
        }
    }
}