#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
int main()
{
uint32_t num=0X0A0B0C0D;
uint8_t b0=(num>>24)&0xFF;
uint8_t b1=(num>>16)&0xFF;
uint8_t b2=(num>>8)&0xFF;
uint8_t b3=(num>>0)&0xFF;
printf("the original number is 0x%08X\n",num);
printf("Bytes: 0x%02X, 0x%02X, 0x%02X, 0x%02X\n", b0, b1, b2, b3);
}
