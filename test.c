#include <stdio.h>

struct LayoutGroup
{
    struct Layout *child;
    int len;
};

struct WidgetGroup
{
    struct Widget *child;
    int len;
};

struct Widget
{
    int w, h;
    const char *color;
};

struct Layout
{
    int w, h;
    const char *color;
    struct LayoutGroup layout;
    struct WidgetGroup widget;
};

#define WIDGET(group...) \
    widget: { \
        len: sizeof((struct Widget[])group) / sizeof((struct Widget[])group[0]), \
        child : (struct Widget[])group \
    }
#define LAYOUT(group...) \
    layout: { \
        len:sizeof((struct Layout[])group) / sizeof((struct Layout[])group[0]), \
        child : (struct Layout[])group \
    }

int main()
{
    struct Layout a = {
        w : 1920,
        h : 1080,
        color : "#FFFFFF",
        WIDGET({
            {
                w : 1920,
                h : 1080,
                color : "#FFFFFF",
            },
            {
                w : 1920,
                h : 1080,
                color : "#FFFFFF",
            },
        }),
        LAYOUT({{
            w : 1920,
            h : 1080,
            color : "#FFFFFF",
            LAYOUT({{
                w : 1920,
                h : 1080,
                color : "#FFFFFF",
                LAYOUT({{
                    w : 1920,
                    h : 1080,
                    color : "#FFFFFF",
                }}),
            }}),
        }}),
    };
    return 0;
}
