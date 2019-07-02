#include "R_ext/Altrep.h"
#include <Rcpp.h>


extern R_altrep_class_t altrep_raw_class;
extern R_altrep_class_t altrep_logical_class;
extern R_altrep_class_t altrep_integer_class;
extern R_altrep_class_t altrep_real_class;

R_altrep_class_t get_altrep_class(Rcpp::String dataType);

