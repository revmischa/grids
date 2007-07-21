#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <stdlib.h>
#include <stdio.h>

#define REG(i) (*(handle + i * sizeof(reg_t)))

typedef int reg_t;
typedef unsigned int  ureg_t;
typedef int * reghandle_t;

SV* alloc_regs(unsigned int count) {
  reghandle_t regs = malloc(sizeof(reg_t) * count);
  return newSVuv((unsigned int)regs);
}

void dealloc_regs(reghandle_t handle) {
  free(handle);
}
 
reg_t get_reg(reghandle_t handle, unsigned int idx) {
  return REG(idx);
}

ureg_t get_reg_u(reghandle_t handle, unsigned int idx) {
  return (ureg_t)get_reg(handle, idx);
}

void set_reg(reghandle_t handle, unsigned int idx, reg_t val) {
  REG(idx) = val;
}

void set_reg_u(reghandle_t handle, unsigned int idx, ureg_t val) {
  REG(idx) = val;
}

ureg_t and_regs(reghandle_t handle, unsigned int idx1, unsigned int idx2) {
  return get_reg_u(handle, idx1) & get_reg_u(handle, idx2);
}

ureg_t or_regs(reghandle_t handle, unsigned int idx1, unsigned int idx2) {
  return get_reg_u(handle, idx1) | get_reg_u(handle, idx2);
}

ureg_t xor_regs(reghandle_t handle, unsigned int idx1, unsigned int idx2) {
  return get_reg_u(handle, idx1) ^ get_reg_u(handle, idx2);
}

MODULE = NetReg		PACKAGE = NetReg

SV*
alloc_regs(count)
  unsigned int count

void
dealloc_regs(handle)
  reghandle_t handle

reg_t
get_reg(handle, idx)
  reghandle_t handle
  unsigned int idx

ureg_t
get_reg_u(handle, idx)
  reghandle_t handle
  unsigned int idx

void
set_reg(handle, idx, val)
  reghandle_t handle
  unsigned int idx
  reg_t val

void
set_reg_u(handle, idx, val)
  reghandle_t handle
  unsigned int idx
  ureg_t val

reg_t
and_regs(handle, idx1, idx2)
  reghandle_t handle
  unsigned int idx1
  unsigned int idx2

reg_t
or_regs(handle, idx1, idx2)
  reghandle_t handle
  unsigned int idx1
  unsigned int idx2

reg_t
xor_regs(handle, idx1, idx2)
  reghandle_t handle
  unsigned int idx1
  unsigned int idx2


