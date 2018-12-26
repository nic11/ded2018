#pragma once

#include <memory>
#include <cmath>

namespace Tree {
    struct Node {
        enum Type {
            ADD, SUB, MUL, DIV, POW,
            SIN, COS, LOG,
            CONST, VAR
        };

        Type type;
        double val;
        int varid;
        std::shared_ptr<Node> l, r;

        Node(Type t) : type(t), val(NAN), varid(-1), l(nullptr), r(nullptr) {}
    };

    typedef std::shared_ptr<Node> NodePtr;

    NodePtr makeOp(Node::Type op, NodePtr l, NodePtr r = nullptr);
    NodePtr makeConst(double val);
    NodePtr makeVar(int varid);
}
