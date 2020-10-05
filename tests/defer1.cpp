
#include <doctest/doctest.h>
#include "agave.hpp"

TEST_CASE("UniqueDfr") {
  auto sh = std::make_shared<int>(5);
  auto de = agave::DeferUnique<std::shared_ptr<int>>();

  CHECK(!de.Deref());
}
