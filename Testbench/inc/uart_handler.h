#ifndef UART_HANDLER_H
#define UART_HANDLER_H

int uart_handler_send(const char *text);
int uart_handler_recv(char *buf, size_t bufsize);

#endif
