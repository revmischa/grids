#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <stdlib.h>
#include <stdio.h>

typedef char * memhandle_t;

memhandle_t mem_new (unsigned long size) {
  return (memhandle_t)malloc(size);
}

void mem_set (memhandle_t handle, unsigned long offset, const char *data,
          unsigned long len) {
  memcpy((char *)(handle + offset), data, len);
}

SV *mem_get(memhandle_t handle, unsigned long offset, unsigned long len) {
  char *base = (char *)handle;

  // copy buffer into a SV
  char *buf = malloc(len);
  memcpy(buf, (base + offset), len);
  SV* ret = newSVpvn(buf, len);
  free(buf);

  return ret;
}

void mem_copy(memhandle_t oldhandle, memhandle_t newhandle, unsigned long len) {
  if (! oldhandle || ! newhandle) return;
  memcpy((char *)newhandle, (char *)(oldhandle), len);
}

void mem_destroy(memhandle_t handle) {
  free((char *)handle);
}

MODULE = Grids::VM::Memory		PACKAGE = Grids::VM::Memory

memhandle_t
mem_new(size)
    unsigned long size

void
mem_destroy(handle)
     memhandle_t handle

void
mem_copy(oldhandle, newhandle, len)
     memhandle_t oldhandle
     memhandle_t newhandle
     unsigned long len

SV*
mem_get(handle,offset,len)
     memhandle_t handle
     unsigned long offset
     unsigned long len

void
mem_set(handle,offset,data)
     memhandle_t handle
     unsigned long offset
     SV *data
CODE:
     unsigned long size;
     char *buf = SvPVbytex(data, size);
     mem_set(handle, offset, buf, size);

