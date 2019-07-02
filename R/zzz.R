#' @useDynLib AltrepPointer, .registration = TRUE


.onUnload <- function(libpath) {
  library.dynam.unload("AltrepPointer", libpath)
}