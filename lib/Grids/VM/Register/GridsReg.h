
typedef U16   reg_idx_t;
typedef I32   reg_t;
typedef U32   ureg_t;
typedef uint64_t uqword_t;
typedef int64_t qword_t;
typedef int*  reghandle_t;

reg_t hi(qword_t val);
reg_t lo(qword_t val);
ureg_t hi_u(uqword_t val);
ureg_t lo_u(uqword_t val);

