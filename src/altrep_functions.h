#include "Rcpp.h"
#include "tools.h"
#include "altrep_macro.h"

using namespace Rcpp;
/*
altrep_inspect

altrep_length
altrep_duplicate

altrep_dataptr
altrep_dataptr_or_null
altrep_subset
altrep_Elt
altrep_region
*/

#define ULLong unsigned long long

Rboolean altrep_inspect(SEXP x, int pre, int deep, int pvec,
	void (*inspect_subtree)(SEXP, int, int, int)) {
	Rprintf(" ALTREP %s data pointer(Name=%d, duplicate=%s, coerce=%s)\n", 
		charPtr(ALT_DATA_TYPE(x)),
		NAMED(x),
		charPtr(ALT_DUPLICATE_METHOD(x)),
		charPtr(ALT_COERCE_METHOD(x)));
	return Rboolean::TRUE;
}

R_xlen_t altrep_length(SEXP x) {
	R_xlen_t len = as<R_xlen_t>(ALT_DATA_LENGTH(x));
	DEBUG(Rprintf("accessing length:%llu\n", len));
	return len;
}

SEXP altrep_duplicate(SEXP x, Rboolean deep) {
	String duplicate_method = ALT_DUPLICATE_METHOD(x);
	DEBUG(Rprintf("Duplicating data, method:%s\n", duplicate_method.get_cstring()));
	SEXP state = R_altrep_data2(x);
	if (duplicate_method == "duplicate") {
		return NULL;
	}
	if (duplicate_method == "sameObject") {
		Rprintf("check\n");
		return x;
	}
	if (duplicate_method == "error") {
		SEXP message = DUPLICATE_MESSAGE(state);
		if (message == R_NilValue)
			errorHandle("This ALTREP pointer is not allowed to be duplicated.\n");
		else
			errorHandle(charPtr(message));
	}
	errorHandle("Unexpected error\n");
	return NULL;
}
SEXP altrep_coerce(SEXP x, int type) {
	DEBUG(Rprintf("coercing data to type of %d\n", type));
	String coerce_method = ALT_COERCE_METHOD(x);
	SEXP state = R_altrep_data2(x);
	if (coerce_method == "error") {
		SEXP message = COERCE_MESSAGE(state);
		if (message == R_NilValue)
			errorHandle("Type conversion of this ALTREP pointer is not allowed.\n");
		else
			errorHandle(charPtr(message));
	}
	return(NULL);
}


void* altrep_dataptr(SEXP x, Rboolean writeable) {
	DEBUG(Rprintf("accessing data pointer\n"));
	String data_type = as<String>(ALT_DATA_TYPE(x));
	if (data_type == "logical") {
		errorHandle("Logical pointer is not allowed to be accessed by R since R does not support it.\nIf you are trying to print the variable, please use `var[1:n]` instead.");
	}
	return ALT_DATA_PTR(x);
}

const void* altrep_dataptr_or_null(SEXP x) {
	DEBUG(Rprintf("accessing data pointer or null\n"));
	return altrep_dataptr(x, Rboolean::TRUE);
}



template<class T>
T altrep_Elt(SEXP x, R_xlen_t i) {
	DEBUG(Rprintf("accessing element %d\n", i));
	return ((T*)ALT_DATA_PTR(x))[i];
}

template<class target_C_type,class source_C_type>
R_xlen_t altrep_region(SEXP x, R_xlen_t start, R_xlen_t size, target_C_type* out) {
	DEBUG(Rprintf("accessing region\n"));
	R_xlen_t rest_len = as<R_xlen_t>(ALT_DATA_LENGTH(x)) - start;
	R_xlen_t ncopy = rest_len > size ? size : rest_len;
	source_C_type* dataPtr = (source_C_type*)ALT_DATA_PTR(x);
	//memcpy(out, (T*)ALT_DATA_PTR(x) + start, ncopy * sizeof(T));
	for (R_xlen_t k = 0; k < ncopy; k++)
		out[k] = dataPtr[k+start];
	return ncopy;
}


#include <type_traits>
template<class target_C_type,class source_C_type, class index_type>
void template_subset_assignment(target_C_type* target, source_C_type* source, index_type* indx, R_xlen_t src_len, R_xlen_t ind_len) {
	source = source - 1L;
	DEBUG(Rprintf("Index:"));
	for (R_xlen_t i = 0; i < ind_len; i++) {
		DEBUG(Rprintf("%d,", (int)indx[i]));
		if (indx[i] <= src_len && indx[i] > 0) {
			if (std::is_same<index_type, double>::value) {
				target[i] = source[(R_xlen_t)indx[i]];
			}
			else {
				target[i] = source[(int)indx[i]];
			}
		}
		else {
			errorHandle("Index out of bound:\n index: %llu length:%llu\n", (ULLong)indx[i], (ULLong)src_len);
		}
	}
	DEBUG(Rprintf("\n"));
}
template<int SXP_TYPE, class target_C_TYPE, class source_C_TYPE>
SEXP altrep_subset(SEXP x, SEXP indx, SEXP call) {
	using namespace Rcpp;
	R_xlen_t len = Rf_xlength(indx);
	target_C_TYPE* result = Calloc(len, target_C_TYPE);
	switch (TYPEOF(indx)) {
	case INTSXP:
		template_subset_assignment(result, (source_C_TYPE*)ALT_DATA_PTR(x),
			INTEGER(indx), Rf_xlength(x), Rf_xlength(indx));
		break;
	case REALSXP:
		template_subset_assignment(result, (source_C_TYPE*)ALT_DATA_PTR(x),
			REAL(indx), Rf_xlength(x), Rf_xlength(indx));
		break;
	}
	SEXP res = wrap(Vector<SXP_TYPE>(result, result + len));
	return res;
}