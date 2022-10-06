#pragma once

void USART0_init();
void USART0_transmit(unsigned char data);
void USART0_transmit_str(const char* str);
unsigned char USART0_receive();
int USART0_receive_until(unsigned char* out, char delim);

