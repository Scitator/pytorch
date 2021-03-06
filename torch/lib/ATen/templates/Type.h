#pragma once

#include <memory>
#include <limits>

#include "ATen/ATenGeneral.h"
#include "ATen/ArrayRef.h"
#include "ATen/Generator.h"
#include "ATen/Half.h"
#include "ATen/SparseTensorRef.h"
#include "ATen/ScalarType.h"
#include "ATen/Scalar.h"

// To solve the conflict of s_addr in inaddr.h
#ifdef _MSC_VER
#ifdef s_addr
#undef s_addr
#endif
#endif

namespace at {

class Context;
struct Storage;
struct Generator;

// Note [Undefined-dim versus 0-dim]
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Unlike Torch, ATen treats zero-dimension tensors as having ONE
// element (that is to say, a zero-dimensional tensor is a scalar!)
// This is in contrast to Torch, where a zero-dimension tensor has
// zero elements.
//
// Because we are backed by Torch tensors, we need to be able to
// represent this state (of numel==0).  kUndefinedDimensions represents this
// situation.
constexpr int64_t kUndefinedDimensions = std::numeric_limits<int64_t>::min();

enum class TypeID {
  ${type_ids}
  NumOptions
};


struct ATen_CLASS Type {
  explicit Type(Context * context)
  : context(context) {}
  virtual ~Type() {}
  virtual ScalarType scalarType() const = 0;
  virtual Backend backend() const = 0;
  virtual bool isCuda() const = 0;
  virtual bool isSparse() const = 0;
  virtual bool isDistributed() const = 0;
  static void registerAll(Context * context);
  virtual std::unique_ptr<Storage> storage() const = 0;
  virtual std::unique_ptr<Storage> storage(size_t size) const = 0;
  virtual std::unique_ptr<Storage> storageFromBlob(void * data, int64_t size) const = 0;
  virtual std::unique_ptr<Generator> generator() const = 0;
  virtual Tensor unsafeTensorFromTH(void * th_pointer, bool retain) const = 0;
  virtual const char * toString() const = 0;
  virtual std::size_t elementSizeInBytes() const = 0;
  Type & toBackend(Backend b) const;
  Type & toScalarType(ScalarType s) const;

  // contingious IDs for all types in the system
  // for external dispatch
  virtual TypeID ID() const = 0;

  virtual void copy(const Tensor & src, Tensor & dst) const = 0;
  Tensor copy(const Tensor & src) const;

  Tensor tensorFromBlob(void * data, IntList sizes) const;
  Tensor tensorFromBlob(void * data, IntList sizes, IntList strides) const;
  Tensor scalarTensor(Scalar s) const;

  bool operator==(const Type& other) const;

  // example
  // virtual Tensor * add(Tensor & a, Tensor & b) = 0;
  ${type_method_declarations}
protected:
  Context* context;
};


}
