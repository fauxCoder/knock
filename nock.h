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
    switch(noun->tag)
    {
        case NT_Atom:
        {
            return construct_Noun_copy(noun);
        }

        case NT_Cell:
        {
            assert(noun->head->tag == NT_Atom);

            return _slot(noun->tail, noun->head->atom);
        }

        default:
        {
            return construct_Noun_atom(97);
        }
    }
}

struct Noun* wut(struct Noun* noun)
{
    return construct_Noun_atom((noun->tag == NT_Cell) ? 0 : 1);
}

struct Noun* lus(struct Noun* noun)
{
    assert(noun->tag == NT_Atom);
    return construct_Noun_atom(noun->atom + 1);
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
    assert(noun->tag == NT_Cell);
    return construct_Noun_atom(_tis(noun->head, noun->tail) ? 0 : 1);
}

struct Noun* _nock(struct Noun* noun)
{
    assert(noun->tag == NT_Cell);

    struct Noun* subj = noun->head;
    struct Noun* form = noun->tail;

    assert(form->tag == NT_Cell);

    if(form->head->tag == NT_Atom)
    {
        struct Noun* b = form->tail;

        switch(form->head->atom)
        {
            case 0:
            {
                struct Noun* c = construct_Noun_cell(b, subj);
                return slot(c);
            }

            case 1:
            {
                return construct_Noun_copy(b);
            }

            case 2:
            {
                assert(b->tag == NT_Cell);
                struct Noun* h = construct_Noun_cell(subj, b->head);
                struct Noun* t = construct_Noun_cell(subj, b->tail);
                struct Noun* c = construct_Noun_cell(_nock(h), _nock(t));
                return _nock(c);
            }

            case 3:
            {
                struct Noun* c = construct_Noun_cell(subj, b);
                return wut(_nock(c));
            }

            case 4:
            {
                struct Noun* c = construct_Noun_cell(subj, b);
                return lus(_nock(c));
            }

            case 5:
            {
                struct Noun* c = construct_Noun_cell(subj, b);
                return tis(_nock(c));
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
        struct Noun* h = construct_Noun_cell(subj, form->head);
        struct Noun* t = construct_Noun_cell(subj, form->tail);
        return construct_Noun_cell(_nock(h), _nock(t));
    }
}

struct Noun* nock(struct Noun* noun)
{
    return _nock(noun);
}
