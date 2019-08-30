#include <lib/assembly.h>
#include <lib/assert.h>
#include <lib/stdio.h>
#include <lib/string.h>
#include <utils/lib.h>

static void test_lib();
static void test_sprintf();

/*
 * Api
 */
extern void lib_init()
{
    test_lib();
}

/*
 * Test c library 
 */
static void test_lib()
{
#ifdef TEST
    test_sprintf();
#endif
}

/*
 * Test sprintf
 */
static void test_sprintf()
{
#ifdef TEST
    char buf[256];
    /* numbers */
    sprintf(buf, "%X", 0x20);
    assert(!strcmp(buf, "0x20"));
    /* strings */
    const char* str = "str";
    sprintf(buf, "%s", str);
    assert(!strcmp(buf, str));
    /* mixed */
    const char* str_mixed = "a str a 0xFA0";
    sprintf(buf, "a %s a %X", str, 0xFA0);
    assert(!strcmp(buf, str_mixed));
    /* string scan */
    char* cmd = "fork 1";
    assert(strspn(cmd, " ") == 0);
    assert(strspn(cmd + 4, " ") == 1);
    assert(strcspn(cmd, " ") == 4);
    /* string parse */
    char* save_ptr = null;
    char* token_1 = strtok_r(cmd, " ", &save_ptr);
    char* token_2 = strtok_r(null, " ", &save_ptr);
    assert(!strcmp(token_1, "fork"));
    assert(!strcmp(token_2, "1"));
    /* atou */
    assert(atou(token_2) == 1);
#endif
}
