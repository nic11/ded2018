#include "expr.h"

namespace Tree {
    NodePtr makeOp(Node::Type op, NodePtr l, NodePtr r) {
        NodePtr n = std::make_shared<Node>(op);
        n->l = l;
        n->r = r;
        return n;
    }

    NodePtr makeConst(double val) {
        NodePtr n = std::make_shared<Node>(Node::Type::CONST);
        n->val = val;
        return n;
    }

    NodePtr makeVar(int varid) {
        NodePtr n = std::make_shared<Node>(Node::Type::VAR);
        n->varid = varid;
        return n;
    }
}
