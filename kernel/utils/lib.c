#include <utils/lib.h>
#include <utils/kassert.h>
#include <lib/string.h>
#include <lib/assembly.h>

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
    kassert(__FILE__, __LINE__, !strcmp(buf, "0x20"));
    /* strings */
    const char *str = "str";
    sprintf(buf, "%s", str);
    kassert(__FILE__, __LINE__, !strcmp(buf, str));
    /* mixed */
    const char *str_mixed = "a str a 0xFF0";
    sprintf(buf, "a %s a %X", str, 0xF00);
    kassert(__FILE__, __LINE__, !strcmp(buf, str_mixed));
#endif
}
