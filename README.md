A big to-do!

Components
--------------------
The projects name and its components are themed around The Hitchhiker's Guide to The Galaxy.

 * arthur - generic C++ utilities
 * deepthgt - trading engine framework
 * example - algorithms, implemented in terms of the framework
 * marvin - market data and order entry protocols

Compiler Support
--------------------
The codebase makes heavy use of C++11 meaning little support for older compilers. Minimum clang 3.1+ or gcc 4.7+

Dependencies        
--------------------
 * boost
 * cpp-netlib
 * glog
 * gtest
 * pugixml

Optional Dependencies
-------------------
The followng are only relevant if scripts/static_cpp_code_view.py is used:
 * astyle (Artistic Style)
 * cppcheck
 * cpplint.py (Google-lint)
