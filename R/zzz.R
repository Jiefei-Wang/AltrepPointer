#' @useDynLib AltrepPointer, .registration = TRUE
#' @import Rcpp
#' 


.onUnload <- function(libpath) {
  library.dynam.unload("AltrepPointer", libpath)
}