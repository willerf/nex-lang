
#pragma once

#include <memory>

struct Code;
struct Block;
struct Word;
struct BeqLabel;
struct BneLabel;
struct DefineLabel;
struct UseLabel;
struct VarAccess;
struct Scope;
struct IfStmt;
struct Call;

template<typename T>
class Visitor {
  public:
    virtual T visit(std::shared_ptr<Code>) = 0;
    virtual T visit(std::shared_ptr<Block>);
    virtual T visit(std::shared_ptr<Word>);
    virtual T visit(std::shared_ptr<BeqLabel>);
    virtual T visit(std::shared_ptr<BneLabel>);
    virtual T visit(std::shared_ptr<DefineLabel>);
    virtual T visit(std::shared_ptr<UseLabel>);
    virtual T visit(std::shared_ptr<VarAccess>);
    virtual T visit(std::shared_ptr<Scope>);
    virtual T visit(std::shared_ptr<IfStmt>);
    virtual T visit(std::shared_ptr<Call>);

    virtual ~Visitor() {}
};
