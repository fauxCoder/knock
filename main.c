#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Noun
{
    uint32_t        atom;

    struct Noun*    head;
    struct Noun*    tail;
} Noun;

bool is_atom(Noun* a_Noun)
{
    return a_Noun->head == NULL;
}

void print_noun_inner(Noun* a_Noun)
{
    if(is_atom(a_Noun))
    {
        printf("%u", a_Noun->atom);
    }
    else
    {
        printf("[");
        print_noun_inner(a_Noun->head);
        printf(" ");
        print_noun_inner(a_Noun->tail);
        printf("]");
    }
}

bool is_digit(char c)
{
    return(
        (c >= '0')
        &&
        (c <= '9')
    );
}

Noun* read_noun(char** p)
{
    while(**p == ' ')
    {
        ++(*p);
    }

    if(is_digit(**p))
    {
        uint32_t number = strtoul(*p, p, 10);

        Noun* a = malloc(sizeof(Noun));
        a->atom = number;
        a->head = NULL;
        a->tail = NULL;

        return a;
    }
    else if(**p == '[')
    {
        ++(*p);

        Noun* h = malloc(sizeof(Noun));
        h->atom = 0;
        h->head = read_noun(p);
        h->tail = read_noun(p);
        return h;
    }
    else
    {
        Noun* a = malloc(sizeof(Noun));
        a->atom = 99;
        a->head = NULL;
        a->tail = NULL;

        return a;
    }
}

void free_noun(Noun* a_Noun)
{
    a_Noun = NULL;
}

void print_noun(Noun* a_Noun)
{
    print_noun_inner(a_Noun);
    printf("\n");
}

int32_t main(int32_t argc, char* argv[])
{
    if(argc != 2)
    {
        printf("Provide noun.\n");
        return 0;
    }

    char* s = argv[1];

    Noun* n = read_noun(&s);

    print_noun(n);

    free_noun(n);

    return 0;
}
