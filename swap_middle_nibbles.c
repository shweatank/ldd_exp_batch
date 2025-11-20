#include <stdio.h>
#include <stdint.h>

int main()
{
    uint32_t num = 0x1234; // 0x1324

    num = (num & 0xF00F)| (num & 0x0F00) >> 4 | (num & 0x00F0) << 4;

    printf("%X\n",num);

    return 0;
}
