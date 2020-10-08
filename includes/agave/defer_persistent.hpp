#pragma once
#include <agave/defer.hpp>

// lasts until the next flush
template<typename T> class PersistentDfr {};

template<typename T, typename... As>
PersistentDfr<T> DeferPersistent(As... args) {}
