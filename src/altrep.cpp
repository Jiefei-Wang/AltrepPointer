#include <Rcpp.h>
#include "R_ext/Altrep.h"
#include "tools.h"
#include "altrep.h"
using namespace Rcpp;




R_altrep_class_t get_altrep_class(String dataType) {
	if (dataType=="raw") {
		return altrep_raw_class;
	}

	if (dataType == "logical") {
		return altrep_logical_class;
	}

	if (dataType == "integer") {
		return altrep_integer_class;
	}

	if (dataType == "real") {
		return altrep_real_class;
	}
	errorHandle("The class type is not available\n");
	return altrep_real_class;
}




