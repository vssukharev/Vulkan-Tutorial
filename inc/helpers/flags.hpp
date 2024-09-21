  
#pragma once

#include <type_traits>

namespace App {
  template <typename T>
  concept enumeration = std::is_enum<T>::value;

  template <enumeration E>
  class flags 
  {
    public:
      using type = std::underlying_type<E>::type;
 
      flags() = default;
      flags(E e) : value{static_cast<type>(e)} {}
      flags(type m) : value(m) {}

      operator bool() { return value; };

      type value;
  };


  template <enumeration E>
  flags<E> operator|=(flags<E>& fl, E e) 
  {
    using tp = flags<E>::type;
    return fl.value |= static_cast<tp>(e);
  }

  template <enumeration E>
  flags<E> operator&=(flags<E>& fl, E e)
  {
    using tp = flags<E>::type;
    return fl.value &= static_cast<tp>(e);
  }

  template <enumeration E>
  flags<E> operator|(flags<E> fl, E e) 
  {
    return fl |= e;
  }

  template <enumeration E>
  flags<E> operator&(flags<E> fl, E e) 
  {
    return fl &= e;
  }
}

