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

#define _FOR_EACH(macro, x, ...) CAT(_FOR_EACH_, IS_EMPTY(__VA_ARGS__)) \
(macro, x, __VA_ARGS__)
#define _FOR_EACH_0(macro, x, ...) macro(x) DEFER(_FOR_EACH_I)()(macro, __VA_ARGS__)
#define _FOR_EACH_1(macro, x, ...) macro(x)
#define _FOR_EACH_I() _FOR_EACH
#define FOR_EACH(macro, ...) EVAL(_FOR_EACH(macro, __VA_ARGS__))

#define FUN1(x) #x,
#define FUN2(x) {x, #x},
#define _FUN3(x, y) {x, #y},
#define FUN3(...) _FUN3 __VA_ARGS__

//case 1
FOR_EACH(FUN1, 1, 2, 3, 4, 5)
FOR_EACH(FUN2, 1, 2, 3, 4, 5)
FOR_EACH(FUN3, (a, 1), (b, 2), (c, 3), (d, 4), (e, 5))

//case 2
#define _FUN4(x, ...)      \
    case x:                \
    {                      \
        __VA_ARGS__ break; \
    }
#define FUN4(...) _FUN4 __VA_ARGS__

#define SWITCH_END(...)         \
    FOR_EACH(FUN4, __VA_ARGS__) \
    }

#define SWITCH(x) \
    switch (x)    \
    {             \
        SWITCH_END

//case 3
#define ENUM_STRING(x) [x] = #x,
#define ENUM_RAW(x) x,
#define ENUM_TO_STRING(x,y) enum_map_##x[y]
#define _ENUM(...) 
#define ENUM(x,...) enum x { FOR_EACH(ENUM_RAW, __VA_ARGS__)  };  char *enum_map_##x[] = {FOR_EACH(ENUM_STRING, __VA_ARGS__)}

ENUM(CLASS_ENUM,
    CLASS_1,
    CLASS_2,
    CLASS_3,
    CLASS_4,
);

int main()
{
    int i = 3;
    printf("%s\n", ENUM_TO_STRING(CLASS_ENUM,CLASS_1));

    SWITCH(i)
    (
        (1, printf("1");),
        (2, printf("2");),
        (3, printf("3");),
        (4, printf("4");),
        (5, printf("5");)
    )
    return 0;
}
