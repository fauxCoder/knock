#pragma once

#include "noun.h"

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

            switch(noun->head->atom)
            {
                case 1:
                {
                    return construct_Noun_copy(noun->tail);
                }

                default:
                {
                    return construct_Noun_atom(noun->head->atom);
                }
            }
        }

        default:
        {
            return construct_Noun_atom(97);
        }
    }
}

struct Noun* nock_inner(struct Noun* noun)
{
    switch(noun->tag)
    {
        case NT_Atom:
        {
            return construct_Noun_atom(noun->atom);
        }

        case NT_Cell:
        {
            switch(noun->tail->tag)
            {
                case NT_Atom:
                {
                    return construct_Noun_atom(noun->tail->atom);
                }

                case NT_Cell:
                {
                    struct Noun* op = noun->tail->head;
                    assert(op->tag == NT_Atom);
                    // struct Noun* op = NULL;
                    // do
                    // {
                    //     op = nock_inner(noun->tail->head);
                    // }
                    // while(op->tag == NT_Cell);

                    struct Noun* arg = noun->tail->tail;

                    switch(op->atom)
                    {
                        case 0:
                        {
                            struct Noun* h = construct_Noun_copy(arg);
                            struct Noun* t = construct_Noun_copy(noun->head);
                            struct Noun* c = construct_Noun_cell(h, t);
                            return slot(c);
                        }

                        default:
                        {
                            assert(false);
                            return NULL;
                        }
                    }
                }

                default:
                {
                    assert(false);
                    return NULL;
                }
            }

            break;
        }

        default:
        {
            assert(false);
            return NULL;
        }
    }
}

struct Noun* nock(struct Noun* noun)
{
    return nock_inner(noun);
}
