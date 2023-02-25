#include <cassert>
#include <deque>
#include <iostream>

#include "../../utils/test_runner.h"

using namespace std;


struct Node {
  Node(int v, Node* p)
    : value(v)
    , parent(p)
  {}

  int value;
  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent;
};


class NodeBuilder {
public:
  Node* CreateRoot(int value) {
    nodes.emplace_back(value, nullptr);
    return &nodes.back();
  }

  Node* CreateLeftSon(Node* me, int value) {
    assert( me->left == nullptr );
    nodes.emplace_back(value, me);
    me->left = &nodes.back();
    return me->left;
  }

  Node* CreateRightSon(Node* me, int value) {
    assert( me->right == nullptr );
    nodes.emplace_back(value, me);
    me->right = &nodes.back();
    return me->right;
  }

private:
  deque<Node> nodes;
};


Node* Next(Node* me) {
  if (me->parent == nullptr && me->right == nullptr && me->left == nullptr)
  {
    return nullptr;
  }
  else if (me->right != nullptr)
  {
    Node* move = me->right;
    while (move->left != nullptr)
    {
      move = move->left;
    }

    return move;
  }
  else if (me->parent != nullptr && me->parent->value > me->value)
  {
    return me->parent;
  }
  else
  {
    Node* move = me->parent;
    while (move->parent != nullptr && move->parent->value < move->value)
    {
      move = move->parent;
    }

    return move->parent == nullptr ? me->right : move->parent;
  }
}
