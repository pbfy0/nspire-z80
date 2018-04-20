#ifndef NAVNET_IO_H
#define NAVNET_IO_H

#include <stdarg.h>
#include <stdint.h>
#include <os.h>

typedef nn_ch_t nn_stream;

void navnet_io_early();

nn_stream navnet_io_init();

void navnet_io_end(nn_stream st);

void navnet_io_send(nn_stream st, char *buf, size_t len);

size_t navnet_io_vprintf(nn_stream st, const char *format, va_list args);

size_t navnet_io_printf(nn_stream st, const char *format, ...);

#endif