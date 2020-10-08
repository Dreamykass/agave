#pragma once
#include <agave/defer.hpp>

// lasts until the next collection or flush
template<typename T> class FleetingDfr {};

//
template<typename T, typename... As> FleetingDfr<T> DeferFleeting(As... args) {}
