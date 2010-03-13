#include "EXTERN.h"
#include "perl.h"
#include "XSUB.h"

#include "ppport.h"

#include <stdlib.h>
#include <stdio.h>

/////

#include "GridsReg.h"

#define REG(i) (*(handle + i))

reghandle_t alloc_regs(reg_idx_t count) {
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
 
reg_t get_reg(reghandle_t handle, reg_idx_t idx) {
  return REG(idx);
}

ureg_t get_reg_u(reghandle_t handle, reg_idx_t idx) {
  return (ureg_t)get_reg(handle, idx);
}

SV* get_reg_sv(reghandle_t handle, reg_idx_t idx) {
  reg_t v = REG(idx);
  return newSViv(v);
}

SV* get_reg_u_sv(reghandle_t handle, reg_idx_t idx) {
  ureg_t v = (ureg_t)get_reg(handle, idx);
  return newSVuv(v);
}

void set_reg(reghandle_t handle, reg_idx_t idx, reg_t val) {
  REG(idx) = val;
}

void set_reg_u(reghandle_t handle, reg_idx_t idx, ureg_t val) {
  REG(idx) = val;
}

ureg_t and_regs(reghandle_t handle, reg_idx_t idx1, reg_idx_t idx2) {
  return get_reg_u(handle, idx1) & get_reg_u(handle, idx2);
}

ureg_t or_regs(reghandle_t handle, reg_idx_t idx1, reg_idx_t idx2) {
  return get_reg_u(handle, idx1) | get_reg_u(handle, idx2);
}

ureg_t xor_regs(reghandle_t handle, reg_idx_t idx1, reg_idx_t idx2) {
  return get_reg_u(handle, idx1) ^ get_reg_u(handle, idx2);
}

qword_t mult_regs(reghandle_t handle, reg_idx_t idx1, reg_idx_t idx2, reg_idx_t hi_idx, reg_idx_t lo_idx) {
  qword_t prod = (qword_t)(get_reg(handle, idx1)) * (qword_t)(get_reg(handle, idx2));
  set_reg(handle, hi_idx, hi(prod));
  set_reg(handle, lo_idx, lo(prod));
}

void mult_regs_u(reghandle_t handle, reg_idx_t idx1, reg_idx_t idx2, reg_idx_t hi_idx, reg_idx_t lo_idx) {
  uqword_t prod = (uqword_t)(get_reg_u(handle, idx1)) * (uqword_t)(get_reg_u(handle, idx2));
  set_reg_u(handle, hi_idx, hi_u(prod));
  set_reg_u(handle, lo_idx, lo_u(prod));
}

reg_t hi(qword_t val){ 
  uqword_t himask = 0xFFFFFFFFU;
  himask = himask << 32;
  qword_t nval = val & himask;
  return (reg_t)(nval >> 32);
}
reg_t lo(qword_t val) {
  return (reg_t)(val & 0x00000000FFFFFFFFU);
}
ureg_t hi_u(uqword_t val){ 
  uqword_t himask = 0xFFFFFFFFU;
  himask = himask << 32;
  uqword_t nval = val & himask;
  return (ureg_t)(nval >> 32);
}
ureg_t lo_u(uqword_t val) {
  return (ureg_t)(val & 0x00000000FFFFFFFFU);
}

MODULE = Grids::VM::Register		PACKAGE = Grids::VM::Register

reghandle_t
alloc_regs(count)
  reg_idx_t count

void
dealloc_regs(handle)
  reghandle_t handle

reg_t
get_reg(handle, idx)
  reghandle_t handle
  reg_idx_t idx

ureg_t
get_reg_u(handle, idx)
  reghandle_t handle
  reg_idx_t idx

SV*
get_reg_sv(handle, idx)
  reghandle_t handle
  reg_idx_t idx

SV*
get_reg_u_sv(handle, idx)
  reghandle_t handle
  reg_idx_t idx

void
set_reg(handle, idx, val)
  reghandle_t handle
  reg_idx_t idx
  reg_t val

void
set_reg_u(handle, idx, val)
  reghandle_t handle
  reg_idx_t idx
  ureg_t val

ureg_t
and_regs(handle, idx1, idx2)
  reghandle_t handle
  reg_idx_t idx1
  reg_idx_t idx2

ureg_t
or_regs(handle, idx1, idx2)
  reghandle_t handle
  reg_idx_t idx1
  reg_idx_t idx2

ureg_t
xor_regs(handle, idx1, idx2)
  reghandle_t handle
  reg_idx_t idx1
  reg_idx_t idx2

void
mult_regs(handle, idx1, idx2, hi_idx, lo_idx)
  reghandle_t handle
  reg_idx_t idx1
  reg_idx_t idx2
  reg_idx_t hi_idx
  reg_idx_t lo_idx

void
mult_regs_u(handle, idx1, idx2, hi_idx, lo_idx)
  reghandle_t handle
  reg_idx_t idx1
  reg_idx_t idx2
  reg_idx_t hi_idx
  reg_idx_t lo_idx

reg_t
hi(val)
  qword_t val

reg_t
lo(val)
  qword_t val

ureg_t
hi_u(val)
  qword_t val

ureg_t
lo_u(val)
  qword_t val
