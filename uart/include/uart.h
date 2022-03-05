#ifndef UART_H
#define UART_H

void uart0_init(void);
int getchar(void);
int putchar(int ch);
int puts(const char *s);

#endif // UART_H