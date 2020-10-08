#pragma once
#include <agave/defer.hpp>

namespace agave {

  //
  template<typename T> class UniqueDfr : public AbstractDfr<T> {
  private:
    detail::UniqueDfrObj<T>* dfr_obj;

  public:
    // default constructor
    UniqueDfr()
      : dfr_obj(nullptr) {}

    // constructor from a deferred object created by DeferUnique()
    UniqueDfr(detail::UniqueDfrObj<T>& _dfr_obj)
      : dfr_obj(&_dfr_obj) {}

    // move constructor
    UniqueDfr(UniqueDfr&& _other) noexcept
      : dfr_obj(std::exchange(_other.dfr_obj, nullptr)) {}
    // move assignment
    UniqueDfr& operator=(UniqueDfr&& _other) noexcept {
      dfr_obj = std::exchange(_other.dfr_obj, nullptr);
      return *this;
    }

    // copy constructor (deleted)
    UniqueDfr(const UniqueDfr&) = delete;
    // copy assignment (deleted)
    UniqueDfr& operator=(const UniqueDfr&) = delete;

    // destructor
    ~UniqueDfr() { Reset(); }

  public:
    // resets, owns nothing afterwards
    void Reset() {
      if (dfr_obj)
        dfr_obj->ready_for_collection = true;
      dfr_obj = nullptr;
    }

    // true if owns, false otherwise
    bool Owns() const { return dfr_obj; }
    // explicit conversion to bool, true if owns, false otherwise
    explicit operator bool() const { return Owns(); }

    // returns a reference to owned T
    // `assert(Owns());` inside
    T& Deref() {
      assert(Owns());
      return dfr_obj->obj;
    }
    // returns a reference to owned T
    // `assert(Owns());` inside
    const T& Deref() const {
      assert(Owns());
      return dfr_obj->obj;
    }
  };

  //
  template<typename T, typename... As> UniqueDfr<T> DeferUnique(As... args) {
    auto uptr =
      std::make_unique<detail::UniqueDfrObj<T>>(std::move(T(args...)));
    auto& ref = *uptr;
    detail::deferred_objects.emplace_back(std::move(uptr));
    return UniqueDfr(ref);
  }
}