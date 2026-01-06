#include <linux/module.h>

int base_func(void)
{
    return 42;
}
EXPORT_SYMBOL(base_func);

MODULE_LICENSE("GPL");
