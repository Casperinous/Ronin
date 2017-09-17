#ifndef MELF_H_
#define MELF_H_


/*
 * Here we will have the architecture based definitions, such as Elf headers and other
 * macros which are related to various architecture design decisions.
 */


#if defined(__LP64__)
#define ElfW(what) Elf64_ ## what
#else
#define ElfW(what) Elf32_ ## what
#endif

#if defined(__LP64__)
#define ELFW(what) ELF64_ ## what
#else
#define ELFW(what) ELF32_ ## what
#endif


#define R_GENERIC_NONE 0 // R_*_NONE is always 0

#if defined (__aarch64__)

#define R_GENERIC_JUMP_SLOT R_AARCH64_JUMP_SLOT
#define R_GENERIC_GLOB_DAT  R_AARCH64_GLOB_DAT
#define R_GENERIC_RELATIVE  R_AARCH64_RELATIVE
#define R_GENERIC_IRELATIVE R_AARCH64_IRELATIVE

#elif defined (__arm__)

#define R_GENERIC_JUMP_SLOT R_ARM_JUMP_SLOT
#define R_GENERIC_GLOB_DAT  R_ARM_GLOB_DAT
#define R_GENERIC_RELATIVE  R_ARM_RELATIVE
#define R_GENERIC_IRELATIVE R_ARM_IRELATIVE

#elif defined (__i386__)

#define R_GENERIC_JUMP_SLOT R_386_JMP_SLOT
#define R_GENERIC_GLOB_DAT  R_386_GLOB_DAT
#define R_GENERIC_RELATIVE  R_386_RELATIVE
#define R_GENERIC_IRELATIVE R_386_IRELATIVE

#elif defined (__x86_64__)

#define R_GENERIC_JUMP_SLOT R_X86_64_JUMP_SLOT
#define R_GENERIC_GLOB_DAT  R_X86_64_GLOB_DAT
#define R_GENERIC_RELATIVE  R_X86_64_RELATIVE
#define R_GENERIC_IRELATIVE R_X86_64_IRELATIVE

#endif


#if defined(__aarch64__) || defined(__x86_64__)
#define USE_RELA 1
#endif

#endif
