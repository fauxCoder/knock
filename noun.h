#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdarg.h>

enum NounTag
{
    NT_Atom,
    NT_Cell,
    NT_Squib,
};

struct Noun
{
    enum NounTag    tag;

    uint32_t        atom;

    struct Noun*    head;
    struct Noun*    tail;
};

struct Noun* construct_Noun_atom(uint32_t atom)
{
    struct Noun* noun = malloc(sizeof(struct Noun));
    noun->tag = NT_Atom;
    noun->atom = atom;
    noun->head = NULL;
    noun->tail = NULL;
    return noun;
}

struct Noun* construct_Noun_cell(struct Noun* head, struct Noun* tail)
{
    struct Noun* noun = malloc(sizeof(struct Noun));
    noun->tag = NT_Cell;
    noun->atom = 0;
    noun->head = head;
    noun->tail = tail;
    return noun;
}

struct Noun* construct_Noun_squib()
{
    struct Noun* noun = malloc(sizeof(struct Noun));
    noun->tag = NT_Squib;
    noun->atom = 0;
    noun->head = NULL;
    noun->tail = NULL;
    return noun;
}

struct Noun* construct_Noun_copy(struct Noun* noun)
{
    switch(noun->tag)
    {
        case NT_Atom:
        {
            return construct_Noun_atom(noun->atom);
            break;
        }

        case NT_Cell:
        {
            struct Noun* h = construct_Noun_copy(noun->head);
            struct Noun* t = construct_Noun_copy(noun->tail);
            return construct_Noun_cell(h, t);
        }

        case NT_Squib:
        {
            return construct_Noun_squib();
        }

        default:
        {
            assert(false);
            return NULL;
        }
    }
}

struct Noun* construct_Noun_list(uint32_t n, ...)
{
    assert(n >= 2);

    va_list arguments;
    va_start(arguments, n);

    struct Noun* ret = NULL;
    struct Noun* p = NULL;

    for(uint32_t i = 0; i < n; ++i)
    {
        if(i < (n-1))
        {
            struct Noun* h = va_arg(arguments, struct Noun*);
            struct Noun* c = construct_Noun_cell(h, NULL);

            if(p)
            {
                p->tail = c;
            }
            else
            {
                ret = c;
            }

            p = c;
        }
        else
        {
            assert(p != NULL);
            p->tail = va_arg(arguments, struct Noun*);
        }
    }

    va_end(arguments);

    assert(ret != NULL);
    return ret;
}

#define ATOM(a) construct_Noun_atom(a)
#define CELL(a, b) construct_Noun_cell(a, b)
#define LIST(n, ...) construct_Noun_list(n, __VA_ARGS__)

void free_noun(struct Noun* a_Noun)
{
    if(a_Noun)
    {
        free_noun(a_Noun->head);
        free_noun(a_Noun->tail);

        free(a_Noun);
    }
}

void _print_noun(struct Noun* a_Noun)
{
    switch(a_Noun->tag)
    {
        case NT_Atom:
        {
            printf("%u", a_Noun->atom);
            break;
        }

        case NT_Cell:
        {
            printf("[");
            _print_noun(a_Noun->head);
            printf(" ");
            _print_noun(a_Noun->tail);
            printf("]");
            break;
        }

        case NT_Squib:
        {
            printf("Q");
            break;
        }
    }
}

void print_noun(struct Noun* a_Noun)
{
    assert(a_Noun != NULL);
    _print_noun(a_Noun);
    printf("\n");
}

bool is_digit(char c)
{
    return(
        (c >= '0')
        &&
        (c <= '9')
    );
}

bool is_open(char c)
{
    return (
        (c == '[') || (c == '(')
    );
}

bool is_close(char c)
{
    return (
        (c == ']') || (c == ')')
    );
}

bool is_valid(char c)
{
    return (
        is_digit(c)
        ||
        is_open(c)
        ||
        is_close(c)
    );
}

bool is_white(char c)
{
    return (
        (c == ' ')
        ||
        (c == '\n')
    );
}

struct ReadStats
{
    uint32_t opened;
    uint32_t closed;
    uint32_t confusions;
};

void init_ReadStats(struct ReadStats* rs)
{
    rs->opened = 0;
    rs->closed = 0;
    rs->confusions = 0;
}

struct Noun* _read_noun(char** p, struct ReadStats* rs)
{
    while(is_white(**p))
    {
        ++(*p);
    }

    if(is_digit(**p))
    {
        uint32_t number = strtoul(*p, p, 10);

        return construct_Noun_atom(number);
    }
    else if((**p == '[') || (**p == '('))
    {
        rs->opened += 1;
        ++(*p);

        struct Noun* h = _read_noun(p, rs);
        struct Noun* ret = construct_Noun_cell(h, NULL);
        struct Noun* c = ret;
        while(
            (**p)
            &&
            ( ! is_close(**p))
        )
        {
            if(**p == ' ')
            {
                ++(*p);
                continue;
            }

            if(c->tail)
            {
                c->tail = construct_Noun_cell(c->tail, NULL);
                c = c->tail;
            }

            c->tail = _read_noun(p, rs);
        }

        if(is_close(**p))
        {
            rs->closed += 1;
            ++(*p);
        }

        if( ! c->tail)
        {
            c->tail = construct_Noun_squib();
        }

        return ret;
    }
    else
    {
        while(
            (**p)
            &&
            ( ! is_valid(**p))
        )
        {
            ++(*p);
            rs->confusions += 1;
        }

        struct Noun* a = construct_Noun_squib();
        return a;
    }
}

struct Noun* read_noun(char** p)
{
    struct ReadStats rs;
    init_ReadStats(&rs);

    struct Noun* n = _read_noun(p, &rs);

    if(rs.opened != rs.closed)
    {
        printf("Reading failed.\n");
        printf("cells opened: %u\ncells closed: %u\n", rs.opened, rs.closed);
    }
    else if(rs.confusions > 0)
    {
        printf("Reading failed.\n");
        printf("unknown characters: %u\n", rs.confusions);
    }
    else
    {
        return n;
    }

    free_noun(n);
    return NULL;
}
