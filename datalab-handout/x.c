#include <stdio.h>

void showbits( unsigned int x )
{
    int i=0;
    for (i = (sizeof(int) * 8) - 1; i >= 0; i--)
    {
       putchar(x & (1u << i) ? '1' : '0');
    }
    printf("\n");
}

int main(int argc, char const *argv[])
{
  int n =__INT_MAX__;
  int x = -1;
  showbits( 0x55555555  ^ 0x55 ^ (0x55<<8)  ^ (0x55<<16) ^ (0x55<<24)   ) ;
  printf("%d\n", 0X5555);

  // & ~(0x55>>7) & ~(0x55<<15) & ~(0x55<<23)
  

  return 0;
}
