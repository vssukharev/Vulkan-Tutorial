  
#pragma once

#include <type_traits>
#include <concepts>


namespace App {
  template <typename T>
  concept enumeration = std::is_enum<T>::value;


  template <enumeration E>
  class FLAGS 
  {
    public:
      using type = std::underlying_type<E>::type;
 
      FLAGS() = default;
      FLAGS(E e) : value{static_cast<type>(e)} {}
      FLAGS(type m) : value(m) {}

      operator bool() { return value; };

      type value;
  };

  template <enumeration E>
  FLAGS<E> operator|(E e1, E e2) 
  {
    using tp = FLAGS<E>::type;
    return static_cast<tp>(e1) | static_cast<tp>(e2);
  }

  template <enumeration E>
  FLAGS<E> operator|(FLAGS<E> fl, E e) 
  {
    using tp = FLAGS<E>::type;
    return static_cast<tp>(fl) | static_cast<tp>(e);
  }

  template <enumeration E>
  FLAGS<E> operator|=(FLAGS<E>& fl, E e) 
  {
    using tp = FLAGS<E>::type;
    return fl.value |= static_cast<tp>(e);
  }

  template <enumeration E>
  FLAGS<E> operator&(FLAGS<E> fl, E e) 
  {
    using tp = FLAGS<E>::type;
    return fl.value & static_cast<tp>(e);
  }
}

