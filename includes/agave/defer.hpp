#pragma once

#include <deque>
#include <memory>

namespace agave {

  // detail
  namespace detail {

    class AbstrDefrd {
    public:
      virtual ~AbstrDefrd(){};
      bool ready_for_collection = false;
    };

    thread_local std::deque<std::unique_ptr<detail::AbstrDefrd>> deferred_objs;

  }

  //
  template<typename T>
  class UniqueDefrd : public AbstrDefrd {
  private:
    T* obj;

  public:
  };

  //
  //   class SharedDefrd {};

  //
  template<typename T, typename... As>
  UniqueDefrd<T> Defer(As... args) {
    // ?
  }

  //
  //   SharedDefrd DeferShared() {}

  //
  // returns: number of remaining deferred objects
  size_t Collect() {
    auto& objs = detail::deferred_objs;
    auto cond = [](const std::unique_ptr<detail::AbstrDefrd>& up) {
      return up->ready_for_collection;
    };
    objs.erase(std::remove_if(objs.begin(), objs.end(), cond), objs.end());
    return objs.size();
  }

}
