#ifndef NSASM_ERROR_H_
#define NSASM_ERROR_H_

#include <string>

#include "absl/strings/str_format.h"
#include "absl/types/variant.h"

namespace nsasm {

// Representation of a position in a file
struct Location {
  std::string path;
  int offset;
};

class Error {
 public:
  template <typename... Args>
  explicit Error(const absl::FormatSpec<Args...>& format, const Args&... args)
      : message_(absl::StrFormat(format, args...)), location_() {}

  Error& SetLocation(Location location) {
    location_ = location;
    return *this;
  }

  Error& SetLocation(const std::string& path) {
    location_.path = path;
    return *this;
  }

  Error& SetLocation(int offset) {
    location_.offset = offset;
    return *this;
  }

  Error& SetLocation(const std::string& path, int offset) {
    location_.path = path;
    location_.offset = offset;
    return *this;
  }

  std::string ToString() const;

 private:
  std::string message_;
  Location location_;
};

template <typename T>
class ErrorOr {
 public:
  ErrorOr(T&& t) : value(t) {}
  ErrorOr(const T& t) : value(t) {}
  ErrorOr(Error&& e) : value(e) {}
  ErrorOr(const Error& e) : value(e) {}

  const T& operator*() const& { return absl::get<T>(value); }
  T& operator*() & { return absl::get<T>(value); }
  const T&& operator*() const&& { return absl::get<T>(value); }
  T&& operator*() && { return absl::get<T>(value); }

  const T* operator->() const { return &absl::get<T>(value); }
  T* operator->() { return &absl::get<T>(value); }

  Error error() const { return absl::get<Error>(value); }

  bool ok() const { return absl::holds_alternative<T>(value); }

 private:
  absl::variant<Error, T> value;
};

}  // namespace nsasm

#define NSASM_RETURN_IF_ERROR(v) \
  do {                           \
    if (!v.ok()) {               \
      return v.error();          \
    }                            \
  } while (0)

#define NSASM_RETURN_IF_ERROR_WITH_LOCATION(v, ...) \
  do {                                              \
    if (!v.ok()) {                                  \
      return v.error().SetLocation(__VA_ARGS__);    \
    }                                               \
  } while (0)

#endif  // NSASM_ERROR_H_
