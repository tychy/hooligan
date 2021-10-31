static char *reg32[9] = {
    "edi",
    "esi",
    "edx",
    "ecx",
    "r8d",
    "r9d",
    "eax",
    "ebp",
    "esp",
};

static char *reg64[9] = {
    "rdi",
    "rsi",
    "rdx",
    "rcx",
    "r8",
    "r9",
    "rax",
    "rbp",
    "rsp",
};

static char *reg8[9] = {
    "dil",
    "sil",
    "dl",
    "cl",
    "r8b",
    "r9b",
    "al",
    "epl",
    "spl",
};

static char *xmm[9] = {
    "xmm0",
    "xmm1",
    "xmm2",
    "xmm3",
    "xmm4",
    "xmm5",
    "xmm6",
    "xmm7",
    "xmm8",
};

static char *get_register_from_type(int index, Type *ty)
{
    if (is_float(ty))
    {
        return xmm[index];
    }
    else
    {
        switch (calc_bytes(ty))
        {
        case 1:
            return reg8[index];
        case 4:
            return reg32[index];
        case 8:
        default:
            return reg64[index];
        }
    }
}
