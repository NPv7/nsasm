#include "nsasm/assemble.h"

#include "gtest/gtest.h"
#include "nsasm/expression.h"
#include "nsasm/opcode_map.h"

namespace nsasm {
namespace {

TEST(Assemble, round_trip) {
  // round trip every possible instruction through the assembler
  for (int i = 0; i < 256; ++i) {
    nsasm::Instruction ins = nsasm::DecodeOpcode(i);
    if (ins.addressing_mode == A_rel8 || ins.addressing_mode == A_rel16) {
      ins.arg1 = absl::make_unique<Identifier>("label");
    } else if (ins.addressing_mode != A_imp && ins.addressing_mode != A_acc) {
      ins.arg1 = absl::make_unique<Literal>(0);
    }
    if (ins.addressing_mode == A_mov) {
      ins.arg2 = absl::make_unique<Literal>(0);
    }

    std::string line = absl::StrCat(
        nsasm::ToString(ins.mnemonic), " ",
        nsasm::ArgsToString(ins.addressing_mode, ins.arg1, ins.arg2));
    SCOPED_TRACE(i);
    SCOPED_TRACE(line);
    auto tokens = Tokenize(line, Location());
    ASSERT_TRUE(tokens.ok());
    auto assembled = Assemble(*tokens);
    ASSERT_TRUE(assembled.ok());
    ASSERT_EQ(assembled->size(), 1);
    ASSERT_TRUE(absl::holds_alternative<Instruction>(assembled->front()));
    const nsasm::Instruction& round_tripped =
        absl::get<Instruction>(assembled->front());

    EXPECT_EQ(ins.mnemonic, round_tripped.mnemonic);
    EXPECT_EQ(ins.addressing_mode, round_tripped.addressing_mode);
    EXPECT_EQ(ins.arg1.Evaluate(Location()),
              round_tripped.arg1.Evaluate(Location()));
    EXPECT_EQ(ins.arg2.Evaluate(Location()),
              round_tripped.arg2.Evaluate(Location()));
  }
}

}  // namespace
}  // namespace nsasm