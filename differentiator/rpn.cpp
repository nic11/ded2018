#include <sstream>
#include <vector>
#include <cassert>

#include "rpn.h"

Tree::NodePtr treeFromRpn(std::string str) {
    std::stringstream ss(str);
    std::string token;

    std::vector<Tree::NodePtr> stk;

    while (ss >> token) {
        if (token == "+") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            Tree::NodePtr bot = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::ADD, bot, top));
        } else if (token == "-") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            Tree::NodePtr bot = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::SUB, bot, top));
        } else if (token == "*") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            Tree::NodePtr bot = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::MUL, bot, top));
        } else if (token == "/") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            Tree::NodePtr bot = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::DIV, bot, top));
        } else if (token == "^") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            Tree::NodePtr bot = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::POW, bot, top));
        } else if (token == "sin") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::SIN, top));
        } else if (token == "cos") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::COS, top));
        } else if (token == "log") {
            Tree::NodePtr top = stk.back(); stk.pop_back();
            stk.push_back(Tree::makeOp(Tree::Node::Type::LOG, top));
        } else if (token[0] == 'x') {
            int varid = std::stoi(token.substr(1));
            stk.push_back(Tree::makeVar(varid));
        } else {
            int val = std::stoi(token);
            stk.push_back(Tree::makeConst(val));
        }
    }

    assert(stk.size() == 1);

    return stk.back();
}

void treeToRpnDfs(Tree::NodePtr n, std::stringstream& ss) {
    if (n == nullptr) {
        return;
    }

    treeToRpnDfs(n->l, ss);
    treeToRpnDfs(n->r, ss);

    switch (n->type) {
    case Tree::Node::Type::ADD:
        ss << " +";
        break;
    case Tree::Node::Type::SUB:
        ss << " -";
        break;
    case Tree::Node::Type::MUL:
        ss << " *";
        break;
    case Tree::Node::Type::DIV:
        ss << " /";
        break;
    case Tree::Node::Type::POW:
        ss << " ^";
        break;
    case Tree::Node::Type::SIN:
        ss << " sin";
        break;
    case Tree::Node::Type::COS:
        ss << " cos";
        break;
    case Tree::Node::Type::LOG:
        ss << " log";
        break;
    case Tree::Node::Type::CONST:
        ss << " " << n->val;
        break;
    case Tree::Node::Type::VAR:
        ss << " x" << n->varid;
        break;
    }
}

std::string treeToRpn(Tree::NodePtr n) {
    std::stringstream ss;
    treeToRpnDfs(n, ss);
    return ss.str().substr(1);
}
