#include <utils/lib.h>
#include <lib/string.h>
#include <lib/assembly.h>
#include <lib/assert.h>

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
    const char *str = "str";
    sprintf(buf, "%s", str);
    assert(!strcmp(buf, str));
    /* mixed */
    const char *str_mixed = "a str a 0xFA0";
    sprintf(buf, "a %s a %X", str, 0xFA0);
    assert(!strcmp(buf, str_mixed));
#endif
}
