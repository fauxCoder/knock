#pragma once

#include "noun.h"

struct Noun* slot(struct Noun* noun)
{
    return NULL;
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

                    struct Noun* arg = noun->tail->head;
                    assert(arg->tag == NT_Atom);
                    // struct Noun* arg = NULL;
                    // do
                    // {
                    //     arg = nock_inner(noun->tail->tail);
                    // }
                    // while(arg->tag == NT_Cell);

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

// fn nock(n: Nn) -> Nn
// {
//     match n {
//         Nn::C(ns) => {
//             let head: Nn = ns[0];
//             let tail = ns[1];

//             // Gotta have a cell tail
//             match tail {
//                 Nn::C(ns) => {
//                     let op = nock(ns[0]);
//                     let arg = ns[0];

//                     match op {
//                         Nn::A(0) => {
//                             let c: Nn = Nn::C(vec![arg, head]);
//                             slot(c)
//                         },
//                         Nn::A(_) => op,
//                         Nn::C(ns) => op,
//                     }
//                 }
//             }
//         },
//     }
// }

// fn slot(n: Nn) -> Nn
// {
//     match n {
//         Nn::A(a) => n,
//         Nn::C(ns) => {
//             // Ensure valid cell
//             assert_eq!(ns.len(), 2);
//             let head = ns[0];
//             let tail = ns[1];

//             let i = nock(head);

//             match i {
//                 Nn::A(1) => n,
//                 Nn::A(_) => i,
//                 Nn::C(ns) => i,
//             }
//         }
//     }
// }
