This is a C++ math library with all the goodies

We have constants like pi, e, phi, and more ([constants.hpp](includes/constants.hpp))

We have interpolation functions like lerp, eas in, ease out, and map ([easing.hpp](includes/easing.hpp))

We have multiple hashing functions for all our hashing needs ([hashing.hpp](includes/hashing.hpp))

We have a random number generator, which is fast because it uses a permutation table ([random.hpp](includes/random.hpp))

We have the basic statistics so that you can get the mean, median, and more out of an array ([statistics.hpp](includes/statistics.hpp))

We even have some trigonometric for you ([trigonometric.hpp](includes/trigonometric.hpp))

And for those who don't like the impression of long double, we provide the `Dekker Double` (does not work with other functions) ([dekkerdouble.hpp](includes/dekkerdouble.hpp))


NOTE: Vectors are temporary and will be replaced with Geometric Algebra k-vectors