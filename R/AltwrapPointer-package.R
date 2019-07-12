#' AltrepPointer
#'
#' The package uses ALTREP API to wrap a vector pointer. 
#' The wrapper behaves the same as a regular R vector.
#' Users can directly manipulate the pointer without the need of calling C++ function.
#'
#' @details 
#' The main usage of the package is in the case where there is a vector pointer in C++
#' and users want to access it in R without allocating a new R vector. 
#' 
#' The pointer may come from shared memory, on-disk file or the other library where the 
#' return value is not an R vector. The package can wrap the pointer and users are able to get and set 
#' the values from the pointer via `[` and `[<`. Any changes made on the wrapper will
#' directly change the values in the vector pointer, which can be potentially useful if 
#' a synchronization between R and C++ is desirable.
#'
#' @docType package
#' @name AltrepPointer
#' @import Rcpp
#' @useDynLib AltrepPointer, .registration = TRUE
NULL