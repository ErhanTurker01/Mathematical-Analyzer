//
//  Node.hpp
//  MathematicalAnalyzer
//
//  Created by Erhan Türker on 6/25/24.
//

#include <vector>
#include <optional>
#include <memory>
#include "Tokenizer.hpp"
#include "Number.hpp"

#ifndef Node_hpp
#define Node_hpp

namespace functionNode {

enum class NodeType{
    expr,
    num,
    opr,
    var,
    fun,
    der,
    userNum,
    userFun
};

struct Node{
    NodeType nodeType;
    std::optional<OprType> oprType;
    std::optional<FunType> funType;
    std::optional<Number> value;
    std::optional<std::string> name;
    std::vector<std::shared_ptr<Node>> childs;
};
}

#endif /* Node_hpp */
