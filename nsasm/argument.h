#ifndef NSASM_ARGUMENT_H_
#define NSASM_ARGUMENT_H_

#include <string>

#include "absl/strings/string_view.h"

namespace nsasm {

class Argument {
 public:
  explicit Argument(int v = 0) : value_(v), label_() {}
  explicit Argument(int v, std::string label) : value_(v), label_(label) {}

  // Returns the value of this argument.
  //
  // (Later this will have to account for the case when a value is unknown, for
  // labels and expressions not evaluable until link time.)
  int ToValue() const { return value_; };

  // Returns the name of the label representing this argument, or an empty
  // string view if this argument is not a label.
  absl::string_view Label() const { return label_; };

  // Returns a stringized representation of this argument, where `argument_size`
  // is the size of this argument in bytes.
  //
  // TODO: Should argument objects know their own size?
  std::string ToString(int argument_size) const;

  // Returns a stringized representation of this relative branch argument.
  std::string ToBranchOffset() const;

 private:
  int value_;
  std::string label_;
};

}  // namespace nsasm

#endif  // NSASM_ARGUMENT_H_