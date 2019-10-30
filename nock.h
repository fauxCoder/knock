#pragma once

#include "noun.h"

struct Noun* _slot(struct Noun* noun)
{
    assert(noun->head->tag == NT_Atom);
    uint32_t i = noun->head->atom;
    assert(i > 0);

    if(i == 1)                                              //      /[1 a]                  a
    {
        struct Noun* a = construct_Noun_copy(noun->tail);
        free_noun(noun);
        return a;
    }
    else if(i == 2)                                         //      /[2 a b]                a
    {
        assert(noun->tail->tag == NT_Cell);
        struct Noun* a = construct_Noun_copy(noun->tail->head);
        free_noun(noun);
        return a;
    }
    else if(i == 3)                                         //      /[3 a b]                b
    {
        assert(noun->tail->tag == NT_Cell);
        struct Noun* b = construct_Noun_copy(noun->tail->tail);
        free_noun(noun);
        return b;
    }
    else if((i % 2) == 0)                                   //      /[(a + a) b]            /[2 /[a b]]
    {
        struct Noun* b = construct_Noun_copy(noun->tail);
        free_noun(noun);
        return _slot(CELL(
            ATOM(2),
            _slot(CELL(ATOM(i / 2), b))
        ));
    }
    else                                                    //      /[(a + a + 1) b]        /[3 /[a b]]
    {
        struct Noun* b = construct_Noun_copy(noun->tail);
        free_noun(noun);
        return _slot(CELL(
            ATOM(3),
            _slot(CELL(ATOM(i / 2), b))
        ));
    }
};

struct Noun* slot(struct Noun* noun)
{
    printf("\n / ");
    print_noun(noun);

    struct Noun* ret = _slot(noun);

    printf("   ");
    print_noun(ret);
    printf("\n");

    return ret;
}

struct Noun* _hax(struct Noun* noun)
{
    assert(noun->tag == NT_Cell);                           //      #a                      #a

    assert(noun->head->tag == NT_Atom);
    uint32_t i = noun->head->atom;
    assert(noun->tail->tag == NT_Cell);
    assert(i > 0);

    if(i == 1)                                              //      #[1 a b]                a
    {
        struct Noun* a = construct_Noun_copy(noun->tail->head);
        free_noun(noun);
        return a;
    }
    else if((i % 2) == 0)                                   //      #[(a + a) b c]          #[a [b /[(a + a + 1) c]] c]
    {
        struct Noun* b = construct_Noun_copy(noun->tail->head);
        struct Noun* cI = construct_Noun_copy(noun->tail->tail);
        struct Noun* cII = construct_Noun_copy(noun->tail->tail);
        free_noun(noun);
        return _hax(LIST(3,
            ATOM(i / 2),
            CELL(
                b,
                _slot(CELL(ATOM(i + 1), cI))
            ),
            cII
        ));
    }
    else                                                    //      #[(a + a + 1) b c]      #[a [/[(a + a) c] b] c]
    {
        struct Noun* b = construct_Noun_copy(noun->tail->head);
        struct Noun* cI = construct_Noun_copy(noun->tail->tail);
        struct Noun* cII = construct_Noun_copy(noun->tail->tail);
        free_noun(noun);
        return _hax(LIST(3,
            ATOM(i / 2),
            CELL(
                _slot(CELL(ATOM(i - 1), cI)),
                b
            ),
            cII
        ));
    }
}

struct Noun* hax(struct Noun* noun)
{
    printf("\n # ");
    print_noun(noun);

    struct Noun* ret = _hax(noun);

    printf("   ");
    print_noun(ret);
    printf("\n");

    return ret;
}

struct Noun* wut(struct Noun* noun)
{
    printf("\n ? ");
    print_noun(noun);

    struct Noun* ret = construct_Noun_atom((noun->tag == NT_Cell) ? 0 : 1);
    free_noun(noun);

    printf("   ");
    print_noun(ret);
    printf("\n");

    return ret;
}

struct Noun* lus(struct Noun* noun)
{
    printf("\n + ");
    print_noun(noun);

    assert(noun->tag == NT_Atom);
    struct Noun* ret = construct_Noun_atom(noun->atom + 1);
    free_noun(noun);

    printf("   ");
    print_noun(ret);
    printf("\n");

    return ret;
}

bool _tis(struct Noun* lhs, struct Noun* rhs)
{
    if(lhs->tag != rhs->tag)
    {
        return false;
    }
    else if(lhs->tag == NT_Atom)
    {
        return lhs->atom == rhs->atom;
    }
    else if(lhs->tag == NT_Cell)
    {
        return _tis(lhs->head, rhs->head) && _tis(lhs->tail, rhs->tail);
    }
    else
    {
        assert(false);
        return false;
    }
}

struct Noun* tis(struct Noun* noun)
{
    printf("\n = ");
    print_noun(noun);

    assert(noun->tag == NT_Cell);
    struct Noun* ret = construct_Noun_atom(_tis(noun->head, noun->tail) ? 0 : 1);
    free_noun(noun);

    printf("   ");
    print_noun(ret);
    printf("\n");

    return ret;
}

struct Noun* _nock(struct Noun* noun)
{
    assert(noun->tag == NT_Cell);
    assert(noun->tail->tag == NT_Cell);

    struct Noun* subj = noun->head;
    struct Noun* form = noun->tail;

    if(form->head->tag == NT_Cell)                          //      *[a [b c] d]            [*[a b c] *[a d]]
    {
        struct Noun* aI = construct_Noun_copy(subj);
        struct Noun* aII = construct_Noun_copy(subj);
        struct Noun* b = construct_Noun_copy(form->head->head);
        struct Noun* c = construct_Noun_copy(form->head->tail);
        struct Noun* d = construct_Noun_copy(form->tail);
        free_noun(noun);
        return CELL(
            _nock(LIST(3,
                aI,
                b,
                c
            )),
            _nock(CELL(
                aII,
                d
            ))
        );
    }
    else
    {
        switch(form->head->atom)
        {
        case 0:                                             //      *[a 0 b]                /[b a]
        {
            struct Noun* a = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail);
            free_noun(noun);
            return slot(CELL(b, a));
        }

        case 1:                                             //      *[a 1 b]                b
        {
            struct Noun* b = construct_Noun_copy(form->tail);
            free_noun(noun);
            return b;
        }

        case 2:                                             //      *[a 2 b c]              *[*[a b] *[a c]]
        {
            struct Noun* aI = construct_Noun_copy(subj);
            struct Noun* aII = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail->head);
            struct Noun* c = construct_Noun_copy(form->tail->tail);
            free_noun(noun);
            return _nock(CELL(
                _nock(CELL(aI, b)),
                _nock(CELL(aII, c))
            ));
        }

        case 3:                                             //      *[a 3 b]                ?*[a b]
        {
            struct Noun* a = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail);
            free_noun(noun);
            return wut(_nock(CELL(a, b)));
        }

        case 4:                                             //      *[a 4 b]                +*[a b]
        {
            struct Noun* a = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail);
            free_noun(noun);
            return lus(_nock(CELL(a, b)));
        }

        case 5:                                             //      *[a 5 b c]              =[*[a b] *[a c]]
        {
            struct Noun* aI = construct_Noun_copy(subj);
            struct Noun* aII = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail->head);
            struct Noun* c = construct_Noun_copy(form->tail->tail);
            free_noun(noun);
            return tis(CELL(
                _nock(CELL(aI, b)),
                _nock(CELL(aII, c))
            ));
        }

        case 6:                                             //      *[a 6 b c d]            *[a *[[c d] 0 *[[2 3] 0 *[a 4 4 b]]]]
        {
            struct Noun* aI = construct_Noun_copy(subj);
            struct Noun* aII = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail->head);
            struct Noun* c = construct_Noun_copy(form->tail->tail->head);
            struct Noun* d = construct_Noun_copy(form->tail->tail->tail);
            free_noun(noun);
            return _nock(CELL(
                aI,
                _nock(LIST(3,
                    CELL(c, d),
                    ATOM(0),
                    _nock(LIST(3,
                        CELL(ATOM(2), ATOM(3)),
                        ATOM(0),
                        _nock(LIST(4,
                            aII,
                            ATOM(4),
                            ATOM(4),
                            b
                        ))
                    ))
                ))
            ));
        }

        case 7:                                             //      *[a 7 b c]              *[*[a b] c]
        {
            struct Noun* a = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail->head);
            struct Noun* c = construct_Noun_copy(form->tail->tail);
            free_noun(noun);
            return _nock(CELL(
                _nock(CELL(a, b)),
                c
            ));
        }

        case 8:                                             //      *[a 8 b c]              *[[*[a b] a] c]
        {
            struct Noun* aI = construct_Noun_copy(subj);
            struct Noun* aII = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail->head);
            struct Noun* c = construct_Noun_copy(form->tail->tail);
            free_noun(noun);
            return _nock(CELL(
                CELL(
                    _nock(CELL(aI, b)),
                    aII
                ),
                c
            ));
        }

        case 9:                                             //      *[a 9 b c]              *[*[a c] 2 [0 1] 0 b]
        {
            struct Noun* a = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail->head);
            struct Noun* c = construct_Noun_copy(form->tail->tail);
            free_noun(noun);
            return _nock(LIST(5,
                _nock(CELL(a, c)),
                ATOM(2),
                CELL(ATOM(0), ATOM(1)),
                ATOM(0),
                b
            ));
        }

        case 10:                                            //      *[a 10 [b c] d]         #[b *[a c] *[a d]]
        {
            struct Noun* aI = construct_Noun_copy(subj);
            struct Noun* aII = construct_Noun_copy(subj);
            struct Noun* b = construct_Noun_copy(form->tail->head->head);
            struct Noun* c = construct_Noun_copy(form->tail->head->tail);
            struct Noun* d = construct_Noun_copy(form->tail->tail);
            free_noun(noun);
            return hax(LIST(3,
                b,
                _nock(CELL(aI, c)),
                _nock(CELL(aII, d))
            ));
        }

        case 11:
        {
            assert(form->tail->tag == NT_Cell);
            if(form->tail->head->tag == NT_Cell)            //      *[a 11 [b c] d]         *[[*[a c] *[a d]] 0 3]
            {
                struct Noun* aI = construct_Noun_copy(subj);
                struct Noun* aII = construct_Noun_copy(subj);
                struct Noun* c = construct_Noun_copy(form->tail->head->tail);
                struct Noun* d = construct_Noun_copy(form->tail->tail);
                free_noun(noun);
                return _nock(LIST(3,
                    CELL(
                        _nock(CELL(aI, c)),
                        _nock(CELL(aII, d))
                    ),
                    ATOM(0),
                    ATOM(3)
                ));
            }
            else                                            //      *[a 11 b c]             *[a c]
            {
                struct Noun* a = construct_Noun_copy(subj);
                struct Noun* c = construct_Noun_copy(form->tail->tail);
                free_noun(noun);
                return _nock(CELL(a, c));
            }
        }

        default:
        {
            assert(false);
            free_noun(noun);
            return NULL;
        }
        }
    }
}

struct Noun* nock(struct Noun* noun)
{
    printf(" * ");
    print_noun(noun);

    struct Noun* ret = _nock(noun);

    printf("   ");
    print_noun(ret);

    return ret;
}
