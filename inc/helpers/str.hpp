

#include <string>

namespace App::Impl {
  
  std::string ConcatToStr()
  {
    return "";
  }

  // TODO: fix
  template <typename Head, typename... Tail>
  std::string ConcatToStr(Head head, Tail... tail)
  {
    std::string res {};

    if constexpr ( 
        std::is_convertible_v<Head, std::string> || 
        std::is_same_v<std::remove_cv_t<Head>, char> ) 
    {
      res += head;
    } else {
      res += std::to_string(head);
    }
    
    res += ConcatToStr(tail...);

    return res;
  }


}


