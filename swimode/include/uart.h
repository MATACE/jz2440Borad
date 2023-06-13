#ifndef UART_H
#define UART_H

void uart0_init(void);
int getchar(void);
int putchar(int ch);
int puts(const char *s);
void print_hex(unsigned int val);

#endif // UART_H