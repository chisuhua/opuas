    .text
    .global _Z9vectorAddPfS_S_i
    .type _Z9vectorAddPfS_S_i,@function
_Z9vectorAddPfS_S_i:
bb_00:
    ld.param.u64    %vd0, [%s0 + _Z9vectorAddPfS_S_i_param_0]
    ld.param.u64    %vd2, [%s0 + _Z9vectorAddPfS_S_i_param_1]
    ld.param.u64    %vd4, [%s0 + _Z9vectorAddPfS_S_i_param_2]
    ld.param.u32    %v6, [%s0 + _Z9vectorAddPfS_S_i_param_3]
    mov.u32 %v7, %tid.x
    mov.u32 %v8, %ntid.x
    mov.u32 %v9, %ctaid.x
    mul.lo.u32  %v10, %v8, %v9
    add.u32     %v11, %v10, %v7
    set_tcc.ge.u32     %p0, %v11, %v6
    s_branch_tccnz p0    BB0_3
    mul.wide.u32    %vd6, %v11, 4
    ld.global.f32   %v12, [%vd0 + %vd6]
    ld.global.f32   %v13, [%vd2 + %vd6]
    add.f32         %v14, %v12, %v13
    st.global.f32   [%vd4 + %vd6], %v14
    BB0_3:
    t_exit
-
opu.kernels:
 - .name: _Z9vectorAddPfS_S_i
   .args:
     - .address_space: global .name: _Z9vectorAddPfS_S_i_param_0 .offset: 0 .size: 8 .value_kind: global_buffer
     - .address_space: global .name: _Z9vectorAddPfS_S_i_param_1 .offset: 8 .size: 8 .value_kind: global_buffer
     - .address_space: global .name: _Z9vectorAddPfS_S_i_param_2 .offset: 16 .size: 8 .value_kind: global_buffer
     - .address_space: global .name: _Z9vectorAddPfS_S_i_param_3 .offset: 24 .size: 4 .value_kind: global_buffer
   .shared_memsize: 0
   .private_memsize: 0
   .cmem_size: 0
   .bar_used: 0
   .local_framesize: 0
   .kernel_ctrl: 7
   .kernel_mode: 0
opu.version:
 - 2
 - 0
...
