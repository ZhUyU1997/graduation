#include <stdio.h>
#include <stdbool.h>

#define PRIMITIVE_CAT(x, y) x##y
#define CAT(x, y) PRIMITIVE_CAT(x, y)

#define __IS_EMPTY(a, b, size, ...) size
#define _IS_EMPTY(...) __IS_EMPTY(, ##__VA_ARGS__, 0, 1)
#define IS_EMPTY(x, ...) _IS_EMPTY(x)

#define EMPTY()
#define DEFER(id) id EMPTY()

#define EVAL(...) EVAL1(EVAL1(EVAL1(__VA_ARGS__)))
#define EVAL1(...) EVAL2(EVAL2(EVAL2(__VA_ARGS__)))
#define EVAL2(...) EVAL3(EVAL3(EVAL3(__VA_ARGS__)))
#define EVAL3(...) EVAL4(EVAL4(EVAL4(__VA_ARGS__)))
#define EVAL4(...) EVAL5(EVAL5(EVAL5(__VA_ARGS__)))
#define EVAL5(...) __VA_ARGS__

#define _FOR_EACH(macro, x, ...)           \
    CAT(_FOR_EACH_, IS_EMPTY(__VA_ARGS__)) \
    (macro, x, __VA_ARGS__)
#define _FOR_EACH_0(macro, x, ...) macro(x) DEFER(_FOR_EACH_I)()(macro, __VA_ARGS__)
#define _FOR_EACH_1(macro, x, ...) macro(x)
#define _FOR_EACH_I() _FOR_EACH
#define FOR_EACH(macro, ...) EVAL(_FOR_EACH(macro, __VA_ARGS__))

// char:
// signed char:
// unsigned char:
// short int:
// unsigned short int:
// int:
// unsigned int:
// long int:
// unsigned long int:
// long long int:
// unsigned long long int:
// float:
// double:
// long double:
// char *:
// void *:
// int *:

#define PRINT_FUNC(fmt, type) \
    type:                     \
    print_##fmt

#if 1
#define PRINT_TEMPLATE(fmt, type)                   \
    void print_##fmt(type value)                    \
    {                                               \
        printf("%s: %" #fmt "\n", __func__, value); \
    }
#else
#define PRINT_TEMPLATE(fmt, type) \
    void print_##fmt(type value)  \
    {                             \
        printf("%" #fmt, value);  \
    }
#endif

#define PRINT_LIST(FUNC) FOR_EACH(FUNC, (s, char *), (d, int), (u, unsigned int), (ld, long int), (lu, unsigned long int), (lld, long long int), (llu, unsigned long long int))
#define PRINT(x) _Generic((x),                                     \
                          PRINT_FUNC(s, char *),                   \
                          PRINT_FUNC(d, int),                      \
                          PRINT_FUNC(u, unsigned int),             \
                          PRINT_FUNC(ld, long int),                \
                          PRINT_FUNC(lu, unsigned long int),       \
                          PRINT_FUNC(lld, long long int),          \
                          PRINT_FUNC(llu, unsigned long long int), \
                          PRINT_FUNC(f, float),                    \
                          PRINT_FUNC(lf, double),                  \
                          void *                                   \
                          : print_void,                            \
                            default                                \
                          : print_default)(x)
#define __PRINTV(x) PRINT(x);
#define _PRINTV_0(...) ({ FOR_EACH(__PRINTV, ##__VA_ARGS__); })
#define _PRINTV_1(...)

#define PRINTV(...) CAT(_PRINTV_,IS_EMPTY(__VA_ARGS__))(__VA_ARGS__)
#define V(x) , x,

void print_void(void *i)
{
    printf("%s: %p", __func__, (void *)i);
}

void print_default(unsigned long i)
{
    printf("%s: %p", __func__, (void *)i);
}

PRINT_TEMPLATE(s, char *);
PRINT_TEMPLATE(d, int);
PRINT_TEMPLATE(u, unsigned int);
PRINT_TEMPLATE(ld, long int);
PRINT_TEMPLATE(lu, unsigned long int);
PRINT_TEMPLATE(lld, long long int);
PRINT_TEMPLATE(llu, unsigned long long int);
PRINT_TEMPLATE(f, float);
PRINT_TEMPLATE(lf, double);

int main()
{
    PRINT(123);
    PRINT(123U);
    PRINT(1234444444444444L);
    PRINT(1244444444444443UL);
    PRINT(123LL);
    PRINT(123ULL);
    char *p = "fdsf";
    char pp[] = "*****";
    PRINT("34324");
    PRINT(p);
    PRINT(pp);
    PRINT(true);
    void *pointer = 0x1232143211431UL;
    // long long double lld;
    PRINTV("hello:" V(100) " world:" V(1.0d) "dfsadas:" V(pointer));
    PRINTV();
    return 0;
}
