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

struct Noun* slot(struct Noun* noun)
{
    printf("\n / < ");
    print_noun(noun);

    struct Noun* ret = NULL;

    switch(noun->tag)
    {
        case NT_Atom:
        {
            ret = construct_Noun_copy(noun);
            break;
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

    struct Noun* subj = noun->head;
    struct Noun* form = noun->tail;

    assert(form->tag == NT_Cell);

    if(form->head->tag == NT_Atom)
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

            case 5:
            {
                struct Noun* c = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail));
                free_noun(noun);
                return tis(_nock(c));
            }

            case 6:
            {
                struct Noun* c =
                    CN_L(12,
                            construct_Noun_copy(subj),
                            CN_A(2),
                    CN_C(
                            CN_A(0),
                            CN_A(1)
                    ),
                            CN_A(2),
                    CN_L(3,
                            CN_A(1),
                            construct_Noun_copy(form->tail->tail->head),
                            construct_Noun_copy(form->tail->tail->tail)
                    ),
                    CN_C(
                            CN_A(1),
                            CN_A(0)
                    ),
                            CN_A(2),
                    CN_L(3,
                            CN_A(1),
                            CN_A(2),
                            CN_A(3)
                    ),
                    CN_C(
                            CN_A(1),
                            CN_A(0)
                    ),
                            CN_A(4),
                            CN_A(4),
                            construct_Noun_copy(form->tail->head)
                    );
                free_noun(noun);
                return _nock(c);
            }

            case 7:
            {
                struct Noun* c =
                    CN_L(5,
                            construct_Noun_copy(subj),
                            CN_A(2),
                            construct_Noun_copy(form->tail->head),
                            CN_A(1),
                            construct_Noun_copy(form->tail->tail)
                    );
                free_noun(noun);
                return _nock(c);
            }

            case 8:
            {
                struct Noun* c =
                    CN_L(4,
                            construct_Noun_copy(subj),
                            CN_A(7),
                    CN_L(3,
                    CN_L(3,
                            CN_A(7),
                    CN_C(
                            CN_A(0),
                            CN_A(1)
                    ),
                            construct_Noun_copy(form->tail->head)
                    ),
                            CN_A(0),
                            CN_A(1)
                    ),
                            construct_Noun_copy(form->tail->tail)
                    );
                free_noun(noun);
                return _nock(c);
            }

            case 9:
            {
                struct Noun* c =
                    CN_L(4,
                            construct_Noun_copy(subj),
                            CN_A(7),
                            construct_Noun_copy(form->tail->tail),
                    CN_L(3,
                            CN_A(2),
                    CN_C(
                            CN_A(0),
                            CN_A(1)
                    ),
                    CN_C(
                            CN_A(0),
                            construct_Noun_copy(form->tail->head)
                    )));
                free_noun(noun);
                return _nock(c);
            }

            case 10:
            {
                assert(form->tail->tag == NT_Cell);
                if(form->tail->head->tag == NT_Atom)
                {
                    struct Noun* c = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail->tail));
                    free_noun(noun);
                    return _nock(c);
                }
                else
                {
                    struct Noun* c =
                        CN_L(6,
                                construct_Noun_copy(subj),
                                CN_A(8),
                                construct_Noun_copy(form->tail->head->tail),
                                CN_A(7),
                        CN_C(
                                CN_A(0),
                                CN_A(2)
                        ),
                                construct_Noun_copy(form->tail->tail)
                        );
                    free_noun(noun);
                    return _nock(c);
                }
            }

            default:
            {
                assert(false);
                return NULL;
            }
        }
    }
    else
    {
        struct Noun* h = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->head));
        struct Noun* t = construct_Noun_cell(construct_Noun_copy(subj), construct_Noun_copy(form->tail));
        free_noun(noun);
        return construct_Noun_cell(_nock(h), _nock(t));
    }
}

struct Noun* nock(struct Noun* noun)
{
    return _nock(noun);
}
