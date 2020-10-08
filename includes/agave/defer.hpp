#pragma once

#include <deque>
#include <memory>
#include <cassert>
#include <utility>

namespace agave {

  // ------------------------------------------------------------------------

  namespace detail {

    class AbstrDfrObj {
    public:
      virtual ~AbstrDfrObj(){};
      bool ready_for_collection = false;
    };

    template<typename T> class UniqueDfrObj : public AbstrDfrObj {
    public:
      T obj;

      UniqueDfrObj(T&& _obj)
        : obj(std::move(_obj)) {}
    };

    template<typename T> class CountedDfrObj : public AbstrDfrObj {
    public:
      T obj;
      size_t counter;

      CountedDfrObj(T&& _obj)
        : obj(std::move(_obj))
        , counter(1) {}
    };

    thread_local std::deque<std::unique_ptr<detail::AbstrDfrObj>>
      deferred_objects;

  }

  // ------------------------------------------------------------------------

  class F {
  private:
    std::deque<std::unique_ptr<detail::AbstrDfrObj>> deferred_objects;

  public:
  };

  // ------------------------------------------------------------------------

  // abstract interface for the Dfrs
  template<typename T> class AbstractDfr {
  private:
  public:
    // destructor
    virtual ~AbstractDfr() {}

  public:
    // resets, owns nothing afterwards
    virtual void Reset() = 0;

    // true if owns, false otherwise
    virtual bool Owns() const = 0;
    // explicit conversion to bool, true if owns, false otherwise
    virtual explicit operator bool() const = 0;

    // returns a reference to owned T
    // `assert(Owns());` inside
    virtual T& Deref() = 0;
    // returns a reference to owned T
    // `assert(Owns());` inside
    virtual const T& Deref() const = 0;
  };

  // ------------------------------------------------------------------------

  // collects objects on the thread local thing that are ready for collection
  void CollectDfrs() {
    auto& objs = detail::deferred_objects;
    auto cond = [](const auto& up) { return up->ready_for_collection; };
    objs.erase(std::remove_if(objs.begin(), objs.end(), cond), objs.end());
  }

  // flushes all objects on the thread local thing
  void FlushDfrs() { detail::deferred_objects.clear(); }

  // ------------------------------------------------------------------------
}
