#ifndef MY_MACROS_HPP
#define MY_MACROS_HPP

#define CONCAT(line1, line2) line1 line2


#define CONCAT_LN(line1, line2) line1 \
__NL__ line2

#define INCLUDE_ALL

#define ADD_INCLUDE(classname) INCLUDE_ALL \
__NL__ #include "classname ## .hpp" \
__NL__

#endif // MY_MACROS_HPP
