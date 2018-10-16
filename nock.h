#pragma once

#include "noun.h"

struct Noun* _slot(struct Noun* noun, uint32_t i)
{
    assert(i > 0);

    switch(i)
    {
        case 1:
        {
            return noun;
        }

        case 2:
        {
            assert(noun->tag == NT_Cell);
            return noun->head;
        }

        case 3:
        {
            assert(noun->tag == NT_Cell);
            return noun->tail;
        }

        default:
        {
            if((i % 2) == 0)
            {
                return _slot(_slot(noun, i / 2), 2);
            }
            else
            {
                return _slot(_slot(noun, i / 2), 3);
            }
        }
    }
}

struct Noun* hax(struct Noun* noun);

struct Noun* _hax(struct Noun* noun, uint32_t i)
{
    assert(noun->tag == NT_Cell);
    assert(i > 0);

    switch(i)
    {
        case 1:
        {
            return noun->head;
        }

        default:
        {
            struct Noun* c;
            if((i % 2) == 0)
            {
                c = CELL(
                    ATOM(i / 2),
                    CELL(
                        CELL(construct_Noun_copy(noun->head), construct_Noun_copy(_slot(noun->tail, i + 1))),
                        construct_Noun_copy(noun->tail)
                    )
                );
            }
            else
            {
                c = CELL(
                    ATOM(i / 2),
                    CELL(
                        CELL(construct_Noun_copy(_slot(noun->tail, i - 1)), construct_Noun_copy(noun->head)),
                        construct_Noun_copy(noun->tail)
                    )
                );
            }

            return hax(c);
        }
    }
}

struct Noun* slot(struct Noun* noun)
{
    printf("\n / < ");
    print_noun(noun);

    struct Noun* ret = NULL;

    switch(noun->tag)
    {
        case NT_Atom:
        {
            assert(false);
            return NULL;
        }

        case NT_Cell:
        {
            assert(noun->head->tag == NT_Atom);

            ret = construct_Noun_copy(_slot(noun->tail, noun->head->atom));
            break;
        }

        default:
        {
            assert(false);
            return NULL;
        }
    }

    printf(" / > ");
    print_noun(ret);
    printf("\n");

    free_noun(noun);

    return ret;
}

struct Noun* hax(struct Noun* noun)
{
    printf("\n # < ");
    print_noun(noun);

    struct Noun* ret = NULL;

    switch(noun->tag)
    {
        case NT_Atom:
        {
            assert(false);
            return NULL;
        }

        case NT_Cell:
        {
            assert(noun->head->tag == NT_Atom);

            ret = construct_Noun_copy(_hax(noun->tail, noun->head->atom));
            break;
        }

        default:
        {
            assert(false);
            return NULL;
        }
    }

    printf(" # > ");
    print_noun(ret);
    printf("\n");

    free_noun(noun);

    return ret;
}

struct Noun* wut(struct Noun* noun)
{
    printf("\n ? < ");
    print_noun(noun);

    struct Noun* ret = construct_Noun_atom((noun->tag == NT_Cell) ? 0 : 1);

    printf(" ? > ");
    print_noun(ret);
    printf("\n");

    free_noun(noun);

    return ret;
}

struct Noun* lus(struct Noun* noun)
{
    printf("\n + < ");
    print_noun(noun);

    assert(noun->tag == NT_Atom);
    struct Noun* ret = construct_Noun_atom(noun->atom + 1);

    printf(" + > ");
    print_noun(ret);
    printf("\n");

    free_noun(noun);

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
    printf("\n = < ");
    print_noun(noun);

    assert(noun->tag == NT_Cell);
    struct Noun* ret = construct_Noun_atom(_tis(noun->head, noun->tail) ? 0 : 1);

    printf(" = > ");
    print_noun(ret);
    printf("\n");

    free_noun(noun);

    return ret;
}

struct Noun* _nock(struct Noun* noun)
{
    assert(noun->tag == NT_Cell);
    assert(noun->tail->tag == NT_Cell);

    struct Noun* subj = noun->head;
    struct Noun* form = noun->tail;

    if(form->head->tag == NT_Cell)
    {
        struct Noun* h = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->head));
        struct Noun* t = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail));
        free_noun(noun);
        return construct_Noun_cell(_nock(h), _nock(t));
    }
    else
    {
        switch(form->head->atom)
        {
            case 0:
            {
                struct Noun* c = construct_Noun_cell(construct_Noun_copy(form->tail), construct_Noun_copy(subj));
                free_noun(noun);
                return slot(c);
            }

            case 1:
            {
                struct Noun* ret = construct_Noun_copy(form->tail);
                free_noun(noun);
                return ret;
            }

            case 2:
            {
                assert(form->tail->tag == NT_Cell);
                struct Noun* h = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail->head));
                struct Noun* t = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail->tail));
                struct Noun* c = construct_Noun_cell(_nock(h), _nock(t));
                free_noun(noun);
                return _nock(c);
            }

            case 3:
            {
                struct Noun* c = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail));
                free_noun(noun);
                return wut(_nock(c));
            }

            case 4:
            {
                struct Noun* c = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail));
                free_noun(noun);
                return lus(_nock(c));
            }

            case 5:     // *[a 5 b c]           =[*[a b] *[a c]]
            {
                assert(form->tail->tag == NT_Cell);
                struct Noun* h = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail->head));
                struct Noun* t = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail->tail));
                struct Noun* c = construct_Noun_cell(_nock(h), _nock(t));
                free_noun(noun);
                return tis(c);
            }

            case 6:     // *[a 6 b c d]         *[a *[[c d] 0 *[[2 3] 0 *[a 4 4 b]]]]
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

            case 7:     // *[a 7 b c]           *[*[a b] c]
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

            case 8:     // *[a 8 b c]           *[[*[a b] a] c]
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

            case 9:     // *[a 9 b c]           *[*[a c] 2 [0 1] 0 b]
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

            case 10:    // *[a 10 [b c] d]      #[b *[a c] *[a d]]
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
                if(form->tail->head->tag == NT_Cell)
                {
                    struct Noun* c =
                        LIST(6,
                            construct_Noun_copy(subj),
                            ATOM(8),
                            construct_Noun_copy(form->tail->head->tail),
                            ATOM(7),
                            CELL(ATOM(0), ATOM(3)),
                            construct_Noun_copy(form->tail->tail)
                        );
                    free_noun(noun);
                    return _nock(c);
                }
                else
                {
                    struct Noun* c = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail->tail));
                    free_noun(noun);
                    return _nock(c);
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
    return _nock(noun);
}
