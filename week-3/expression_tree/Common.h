#pragma once

#include <memory>
#include <string>

class Expression {
public:
  virtual ~Expression() = default;

  virtual int Evaluate() const = 0;

  virtual std::string ToString() const = 0;
};

using ExpressionPtr = std::unique_ptr<Expression>;

class NumberNode : public Expression {
public:
  explicit NumberNode(int value) : 
  value_(value) {}

  int Evaluate() const override {
    return value_;
  }

  std::string ToString() const override {
    return std::to_string(value_);
  }

private:
  int value_;
};

class SumNode : public Expression {
public:
  explicit SumNode(ExpressionPtr left, ExpressionPtr right) :
  left_(std::move(left)),
  right_(std::move(right))
  {}

  int Evaluate() const override {
    return left_->Evaluate() + right_->Evaluate();
  }

  std::string ToString() const override {
    return "(" + left_->ToString() + ")" + "+" + "(" + right_->ToString() + ")";
  }

private:
  ExpressionPtr left_;
  ExpressionPtr right_;
};

class ProductNode : public Expression {
public:
  explicit ProductNode(ExpressionPtr left, ExpressionPtr right) :
  left_(std::move(left)),
  right_(std::move(right))
  {}

  int Evaluate() const override {
    return left_->Evaluate() * right_->Evaluate();
  }

  std::string ToString() const override {
    return  "(" + left_->ToString() + ")" + "*" + "(" + right_->ToString() + ")";
  }
private:
  ExpressionPtr left_;
  ExpressionPtr right_;
};

// functions to form an expression
ExpressionPtr Value(int value);
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right);
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right);
