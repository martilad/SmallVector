# SmallVector

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

SmallVector was created as part of the 
MI-MPC course at the Faculty of Information Technology at CTU as a term paper.

* The implemented small_vector template class is a simplified equivalent of the `std::vector` 
class with the application of small buffer/data optimization (SBO).
* The class is in the mpc exchange space (`::mpc::small_vector`)
* The array elements are stored in linear order in memory one after the other.
* The `mpc::small_vector<T,N>` class has an added variable that can be used to determine 
the buffer size for storing N T-type elements that are suitably aligned in memory, 
if possible for the T. 
(its natural `alignof(T)` alignment is less than or equal to the maximum 
fundamental alignof alignment (`std::max_align_t`))

## License
This project is licensed under the MIT.