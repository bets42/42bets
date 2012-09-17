#include <memory>
#include <utility>

namespace bets42 { namespace arthur {

    /*
      From Herb Sutter:
      "Although Standard C++ does not yet have make_unique, this is mostly an 
      oversight and it will almost certainly eventually be added. In the meantime, "
      use the version shown above, and your code will be forward-compatible with 
      the likely direction of the C++ standard.

      http://herbsutter.com/gotw/_102/
    */
    template<typename T, typename ...Args>
    std::unique_ptr<T> make_unique( Args&& ...args )
    {
        return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
    }

}}
