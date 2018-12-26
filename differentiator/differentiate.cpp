#include "differentiate.h"

#include <cassert>
#include <cmath>

Tree::NodePtr differentiate(Tree::NodePtr n, int varid) {
    if (n == nullptr) {
        return nullptr;
    }

    switch (n->type) {
    case Tree::Node::ADD:
    case Tree::Node::SUB:
        return Tree::makeOp(n->type,
            differentiate(n->l, varid),
            differentiate(n->r, varid)
        );
    case Tree::Node::MUL:
        return Tree::makeOp(Tree::Node::ADD,
            Tree::makeOp(Tree::Node::MUL,
                differentiate(n->l, varid), n->r),
            Tree::makeOp(Tree::Node::MUL,
                n->l, differentiate(n->r, varid))
        );
    case Tree::Node::DIV:
        return Tree::makeOp(Tree::Node::DIV,
            Tree::makeOp(Tree::Node::SUB,
                Tree::makeOp(Tree::Node::MUL,
                    differentiate(n->l, varid), n->r),
                Tree::makeOp(Tree::Node::MUL,
                    n->l, differentiate(n->r, varid))
            ),
            Tree::makeOp(Tree::Node::POW,
                n->r, Tree::makeConst(2))
        );
    case Tree::Node::POW:
        return Tree::makeOp(Tree::Node::MUL,
            differentiate(
                Tree::makeOp(Tree::Node::MUL,
                    n->r,
                    Tree::makeOp(Tree::Node::LOG,
                        n->l
                    )
                ),
                varid
            ),
            n
        );
    case Tree::Node::SIN:
        return Tree::makeOp(Tree::Node::MUL,
            differentiate(n->l, varid),
            Tree::makeOp(Tree::Node::COS, n->l)
        );
    case Tree::Node::COS:
        return Tree::makeOp(Tree::Node::MUL,
            differentiate(n->l, varid),
            Tree::makeOp(Tree::Node::SUB,
                Tree::makeConst(0),
                Tree::makeOp(Tree::Node::COS, n->l))
        );
    case Tree::Node::LOG:
        return Tree::makeOp(Tree::Node::DIV,
            differentiate(n->l, varid),
            n->l
        );
    case Tree::Node::CONST:
        return Tree::makeConst(0);
    case Tree::Node::VAR:
        return Tree::makeConst(n->varid == varid);
    }

    assert(false);
}

bool isConst(Tree::NodePtr n) {
    return n == nullptr || n->type == Tree::Node::Type::CONST;
}

Tree::NodePtr applyOp(Tree::NodePtr n) {
    assert(isConst(n->l) && isConst(n->r));
    switch (n->type) {
    case Tree::Node::ADD:
        return Tree::makeConst(n->l->val + n->r->val);
    case Tree::Node::SUB:
        return Tree::makeConst(n->l->val - n->r->val);
    case Tree::Node::MUL:
        return Tree::makeConst(n->l->val * n->r->val);
    case Tree::Node::DIV:
        return Tree::makeConst(n->l->val / n->r->val);
    case Tree::Node::POW:
        return Tree::makeConst(pow(n->l->val, n->r->val));
    case Tree::Node::SIN:
        return Tree::makeConst(sin(n->l->val));
    case Tree::Node::COS:
        return Tree::makeConst(cos(n->l->val));
    case Tree::Node::LOG:
        return Tree::makeConst(log(n->l->val));
    case Tree::Node::CONST:
        return n;
    case Tree::Node::VAR:
        assert(false);
    }
    assert(false);
}

bool eq(double a, double b) {
    return fabs(a - b) < 1e-9;
}

std::pair<Tree::NodePtr, bool> simplify(Tree::NodePtr n) {
    if (n == nullptr) {
        return {nullptr, false};
    }

    if (n->type != Tree::Node::VAR && n->type != Tree::Node::CONST &&
            isConst(n->l) && isConst(n->r)) {
        return {applyOp(n), true};
    }

    if (n->l != nullptr) {
        Tree::NodePtr n2;
        bool didSomething;
        std::tie(n2, didSomething) = simplify(n->l);
        if (didSomething) {
            return {Tree::makeOp(n->type, n2, n->r), true};
        }
    }

    if (n->r != nullptr) {
        Tree::NodePtr n2;
        bool didSomething;
        std::tie(n2, didSomething) = simplify(n->r);
        if (didSomething) {
            return {Tree::makeOp(n->type, n->l, n2), true};
        }
    }

    switch (n->type) {
    case Tree::Node::ADD:
        if (isConst(n->l) && eq(n->l->val, 0)) return {n->r, true};
        if (isConst(n->r) && eq(n->r->val, 0)) return {n->l, true};
        return {n, false};
    case Tree::Node::SUB:
        if (isConst(n->r) && eq(n->r->val, 0)) return {n->l, true};
        return {n, false};
    case Tree::Node::MUL:
        if (isConst(n->l) && eq(n->l->val, 0)) return {Tree::makeConst(0), true};
        if (isConst(n->r) && eq(n->r->val, 0)) return {Tree::makeConst(0), true};
        if (isConst(n->l) && eq(n->l->val, 1)) return {n->r, true};
        if (isConst(n->r) && eq(n->r->val, 1)) return {n->l, true};
        return {n, false};
    case Tree::Node::DIV:
        if (isConst(n->l) && eq(n->l->val, 0)) return {Tree::makeConst(0), true};
        if (isConst(n->r) && eq(n->r->val, 0)) assert(false);
        if (isConst(n->r) && eq(n->r->val, 1)) return {n->l, true};
        return {n, false};
    case Tree::Node::POW:
        if (isConst(n->l) && eq(n->l->val, 0)) return {Tree::makeConst(0), true};
        if (isConst(n->l) && eq(n->l->val, 1)) return {Tree::makeConst(1), true};
        if (isConst(n->r) && eq(n->r->val, 0)) return {Tree::makeConst(1), true};
        if (isConst(n->r) && eq(n->r->val, 1)) return {n->l, true};
        return {n, false};
    case Tree::Node::SIN:
    case Tree::Node::COS:
    case Tree::Node::LOG:
        return {n, false};
    case Tree::Node::CONST:
        return {n, false};
    case Tree::Node::VAR:
        return {n, false};
    }

    assert(false);
}
