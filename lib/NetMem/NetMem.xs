#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <stdlib.h>
#include <stdio.h>

unsigned int mem_new (unsigned long size) {
  return (unsigned int)malloc(size);
}

void mem_set (unsigned int handle, unsigned long offset, const char *data,
          unsigned long len) {
  memcpy((char *)(handle + offset), data, len);
}

SV *mem_get(unsigned int handle, unsigned long offset, unsigned long len) {
  char *buf = malloc(len);
  memcpy(buf, (char *)(handle + offset), len);
  return newSVpvn(buf, len);
}

void mem_destroy(unsigned int handle) {
  free((char *)handle);
}

MODULE = NetMem		PACKAGE = NetMem

unsigned int
mem_new(size)
    unsigned long size

void
mem_destroy(handle)
     unsigned int handle

SV*
mem_get(handle,offset,len)
     unsigned int handle
     unsigned long offset
     unsigned long len

void
mem_set(handle,offset,data)
     unsigned int handle
     unsigned long offset
     SV *data
CODE:
     unsigned long size;
     char *buf = SvPVx(data, size);
     mem_set(handle, offset, buf, size);

