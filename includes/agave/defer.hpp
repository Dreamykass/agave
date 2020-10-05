#pragma once

#include <deque>
#include <memory>

namespace agave {

  // detail
  namespace detail {

    class AbstrDefrdObj {
    public:
      virtual ~AbstrDefrdObj(){};
      bool ready_for_collection = false;
    };

    thread_local std::deque<std::unique_ptr<detail::AbstrDefrdObj>>
      deferred_objects;

  }

  //
  template<typename T>
  class UniqueDefrd {
  private:
    T* obj;

  public:
  };

  //
  //   class SharedDefrd {};

  //
  // template<typename T, typename... As>
  // UniqueDefrd<T> Defer(As... args) {
  //   // ?
  // }

  //
  //   SharedDefrd DeferShared() {}

  //
  // returns: number of remaining deferred objects
  size_t Collect() {
    auto& objs = detail::deferred_objects;
    auto cond = [](const auto& up) { return up->ready_for_collection; };
    objs.erase(std::remove_if(objs.begin(), objs.end(), cond), objs.end());
    return objs.size();
  }

}
