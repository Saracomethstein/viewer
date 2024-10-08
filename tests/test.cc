#include "test.h"

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  ::testing::GTEST_FLAG(break_on_failure) = true;
  return RUN_ALL_TESTS();
}