
#include <doctest/doctest.h>
#include "agave.hpp"

TEST_CASE("UniqueDfr") {
  auto sh = std::make_shared<int>(5);
  CHECK(sh.use_count() == 1);

  agave::UniqueDfr<std::shared_ptr<int>> ud;
  CHECK(!ud);

  ud = agave::DeferUnique<std::shared_ptr<int>>();
  CHECK(ud);

  ud.Deref() = sh;
  CHECK(ud);
  CHECK(sh.use_count() == 2);
  CHECK(*sh == 5);
  CHECK(*ud.Deref() == 5);

  decltype(ud) ud2(std::move(ud));
  CHECK(!ud);
  CHECK(ud2);
  CHECK(sh.use_count() == 2);

  decltype(ud) ud3;
  CHECK(!ud3);
  ud3 = std::move(ud2);
  CHECK(!ud2);
  CHECK(ud3);
  CHECK(sh.use_count() == 2);

  ud3.Reset();
  CHECK(!ud3);
  CHECK(sh.use_count() == 2);
  agave::CollectDfrs();
  CHECK(sh.use_count() == 1);
}
