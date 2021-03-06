#ifndef NSASM_TOKEN_H_
#define NSASM_TOKEN_H_

#include <ostream>
#include <string>

#include "absl/types/variant.h"
#include "nsasm/directive.h"
#include "nsasm/error.h"
#include "nsasm/mnemonic.h"
#include "nsasm/numeric_type.h"

namespace nsasm {

struct EndOfLine {
  bool operator==(EndOfLine rhs) const { return true; }
  bool operator!=(EndOfLine rhs) const { return false; }
};

enum Punctuation {
  P_none = 0,
  P_scope = 257,
};

class Token {
 public:
  explicit Token(const std::string& identifier, Location loc)
      : value_(identifier), location_(loc) {}
  explicit Token(int number, Location loc, NumericType type = T_unknown)
      : value_(number), location_(loc), type_(type) {}
  explicit Token(Mnemonic mnemonic, Location loc)
      : value_(mnemonic), location_(loc) {}
  explicit Token(DirectiveName directive_name, Location loc)
      : value_(directive_name), location_(loc) {}
  explicit Token(char punctuation, Location loc)
      : value_(nsasm::Punctuation(punctuation)), location_(loc) {}
  explicit Token(nsasm::Punctuation punctuation, Location loc)
      : value_(punctuation), location_(loc) {}
  explicit Token(EndOfLine eol, Location loc) : value_(eol), location_(loc) {}

  bool IsIdentifier() const {
    return absl::holds_alternative<std::string>(value_);
  }

  bool IsLiteral() const { return absl::holds_alternative<int>(value_); }

  bool IsMnemonic() const {
    return absl::holds_alternative<nsasm::Mnemonic>(value_);
  }

  bool IsDirectiveName() const {
    return absl::holds_alternative<nsasm::DirectiveName>(value_);
  }

  bool IsPunctuation() const {
    return absl::holds_alternative<nsasm::Punctuation>(value_);
  }

  bool IsEndOfLine() const {
    return absl::holds_alternative<EndOfLine>(value_);
  }

  absl::optional<std::string> Identifier() const {
    if (!IsIdentifier()) {
      return absl::nullopt;
    }
    return absl::get<std::string>(value_);
  }
  absl::optional<int> Literal() const {
    if (!IsLiteral()) {
      return absl::nullopt;
    }
    return absl::get<int>(value_);
  }
  absl::optional<nsasm::Mnemonic> Mnemonic() const {
    if (!IsMnemonic()) {
      return absl::nullopt;
    }
    return absl::get<nsasm::Mnemonic>(value_);
  }
  absl::optional<nsasm::DirectiveName> DirectiveName() const {
    if (!IsDirectiveName()) {
      return absl::nullopt;
    }
    return absl::get<nsasm::DirectiveName>(value_);
  }
  absl::optional<nsasm::Punctuation> Punctuation() const {
    if (!IsPunctuation()) {
      return absl::nullopt;
    }
    return absl::get<nsasm::Punctuation>(value_);
  }

  NumericType Type() const { return type_; }
  const nsasm::Location& Location() const { return location_; }

  // Stringize this token, for use in error messages
  std::string ToString() const;

  // Equality compares raw value, but not location or bit width.
  // Intended for testing and parser writing.
  bool operator==(const Token& rhs) const { return value_ == rhs.value_; }
  bool operator!=(const Token& rhs) const { return value_ != rhs.value_; }

 private:
  absl::variant<std::string, int, nsasm::Mnemonic, nsasm::DirectiveName,
                nsasm::Punctuation, EndOfLine>
      value_;
  nsasm::Location location_;
  NumericType type_ = T_unknown;
};

using TokenSpan = absl::Span<const nsasm::Token>;

ErrorOr<std::vector<Token>> Tokenize(absl::string_view, Location loc);

// Convenience comparisons.  Tokens cannot be created from values implicitly,
// but can be compared for equality with those objects.
inline bool operator==(const std::string& lhs, const Token& rhs) {
  return Token(lhs, Location()) == rhs;
}
inline bool operator!=(const std::string& lhs, const Token& rhs) {
  return Token(lhs, Location()) != rhs;
}
inline bool operator==(const Token& lhs, const std::string& rhs) {
  return lhs == Token(rhs, Location());
}
inline bool operator!=(const Token& lhs, const std::string& rhs) {
  return lhs != Token(rhs, Location());
}

inline bool operator==(int lhs, const Token& rhs) {
  return Token(lhs, Location()) == rhs;
}
inline bool operator!=(int lhs, const Token& rhs) {
  return Token(lhs, Location()) != rhs;
}
inline bool operator==(const Token& lhs, int rhs) {
  return lhs == Token(rhs, Location());
}
inline bool operator!=(const Token& lhs, int rhs) {
  return lhs != Token(rhs, Location());
}

inline bool operator==(Mnemonic lhs, const Token& rhs) {
  return Token(lhs, Location()) == rhs;
}
inline bool operator!=(Mnemonic lhs, const Token& rhs) {
  return Token(lhs, Location()) != rhs;
}
inline bool operator==(const Token& lhs, Mnemonic rhs) {
  return lhs == Token(rhs, Location());
}
inline bool operator!=(const Token& lhs, Mnemonic rhs) {
  return lhs != Token(rhs, Location());
}

inline bool operator==(DirectiveName lhs, const Token& rhs) {
  return Token(lhs, Location()) == rhs;
}
inline bool operator!=(DirectiveName lhs, const Token& rhs) {
  return Token(lhs, Location()) != rhs;
}
inline bool operator==(const Token& lhs, DirectiveName rhs) {
  return lhs == Token(rhs, Location());
}
inline bool operator!=(const Token& lhs, DirectiveName rhs) {
  return lhs != Token(rhs, Location());
}

inline bool operator==(char lhs, const Token& rhs) {
  return Token(lhs, Location()) == rhs;
}
inline bool operator!=(char lhs, const Token& rhs) {
  return Token(lhs, Location()) != rhs;
}
inline bool operator==(const Token& lhs, char rhs) {
  return lhs == Token(rhs, Location());
}
inline bool operator!=(const Token& lhs, char rhs) {
  return lhs != Token(rhs, Location());
}

inline bool operator==(nsasm::Punctuation lhs, const Token& rhs) {
  return Token(lhs, Location()) == rhs;
}
inline bool operator!=(nsasm::Punctuation lhs, const Token& rhs) {
  return Token(lhs, Location()) != rhs;
}
inline bool operator==(const Token& lhs, nsasm::Punctuation rhs) {
  return lhs == Token(rhs, Location());
}
inline bool operator!=(const Token& lhs, nsasm::Punctuation rhs) {
  return lhs != Token(rhs, Location());
}

// googletest pretty printer (streams suck)
inline void PrintTo(const Token& v, std::ostream* out) { *out << v.ToString(); }

}  // namespace nsasm

#endif  // NSASM_TOKEN_H_
