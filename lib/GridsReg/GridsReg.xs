#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <stdlib.h>
#include <stdio.h>

#define REG(i) (*(handle + i))

typedef int reg_t;
typedef unsigned int  ureg_t;
typedef int * reghandle_t;

reghandle_t alloc_regs(unsigned int count) {
  reghandle_t regs = malloc(sizeof(reg_t) * count);
  return regs;
}

void dealloc_regs(reghandle_t handle) {
  if (! handle) {
    fprintf(stderr, "Attempting to dealloc null handle\n");
    return;
  }
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

MODULE = GridsReg		PACKAGE = GridsReg

reghandle_t
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

ureg_t
and_regs(handle, idx1, idx2)
  reghandle_t handle
  unsigned int idx1
  unsigned int idx2

ureg_t
or_regs(handle, idx1, idx2)
  reghandle_t handle
  unsigned int idx1
  unsigned int idx2

ureg_t
xor_regs(handle, idx1, idx2)
  reghandle_t handle
  unsigned int idx1
  unsigned int idx2


