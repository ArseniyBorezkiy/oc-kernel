#include <utils/kassert.h>
#include <utils/kpanic.h>
#include <messages.h>

/*
 * Kernel assertion
 */
extern void kassert(u_int line, u_int column, bool expr) {
    if (!expr) {
        kprint(MSG_KERNEL_LINE_COLUMN, line, column);
        kpanic("assertion falied");
    }
}
