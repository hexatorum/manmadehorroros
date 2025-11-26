#ifndef BITS_H
#define BITS_H

#define BIT(a, n) ((a & (1 << n)) >> n)
#define BIT_ON(a, n) (a | (1 << n)) 
#define BIT_OFF(a, n) (a & ~(1 << n))

#endif