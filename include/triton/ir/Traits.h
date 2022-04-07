#ifndef TRITON_IR_TRAITS_H_
#define TRITON_IR_TRAITS_H_

#include "mlir/IR/OpDefinition.h"

#include "mlir/IR/BuiltinTypes.h"
#include "mlir/Support/LogicalResult.h"

#include <iostream>

namespace mlir {
namespace OpTrait {
// TODO: should have `namespace triton {}` here

template <class ConcreteType>
class TensorSizeTrait : public TraitBase<ConcreteType, TensorSizeTrait> {
public:
  // TODO: move impl to .cc files
  static LogicalResult verifyTrait(Operation *op) {
    for (auto opType : op->getOperandTypes()) {
      if (auto tensorType = opType.dyn_cast<RankedTensorType>()) {
        int64_t numElements = 1;
        for (int64_t s : tensorType.getShape())
          numElements *= s;
        if (numElements > 1048576)
          return op->emitError("Maximum allowed number of elements is 1048576, but ")
                 << *op << " has more than that";
      }
    }

    for (auto opType : op->getResultTypes()) {
      if (auto tensorType = opType.dyn_cast<RankedTensorType>()) {
        int64_t numElements = 1;
        for (int64_t s : tensorType.getShape())
          numElements *= s;
        if (numElements > 1048576)
          return op->emitError("Maximum allowed number of elements is 1048576, but ")
                 << *op << " has more than that";
      }
    }

    return success();
  }
};

}
}

#endif