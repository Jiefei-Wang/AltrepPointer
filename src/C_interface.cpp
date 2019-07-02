#include <Rcpp.h>
#include "R_ext/Altrep.h"
#include "altrep.h"
#include "tools.h"
#include "altrep_macro.h"
using namespace Rcpp;


//To avoid the wrapper
SEXP C_get_altrepPointer(SEXP x) {
	if (!ALTREP(x)) {
		errorHandle("object is not an ALTREP\n");
	}

	SEXP data2 = R_altrep_data2(x);
	//Rf_PrintValue(data2);
	//Rf_PrintValue(DATA_PACKAGE_NAME(data2));
	//Rprintf("%s==%s\n", charPtr(DATA_PACKAGE_NAME(data2)), PACKAGE_NAME);


	if (TYPEOF(data2) == VECSXP &&
		TYPEOF(DATA_PACKAGE_NAME(data2)) == STRSXP &&
		std::strcmp(charPtr(DATA_PACKAGE_NAME(data2)), PACKAGE_NAME)==0) {
		return x;
	}

	SEXP data1= R_altrep_data1(x);
	if (ALTREP(data1)) {
		return(C_get_altrepPointer(data1));
	}
	else {
		errorHandle("object is not an AltrepPointer\n");
	}
	return NULL;
}

// [[Rcpp::export]]
SEXP C_get_altrep_data1(SEXP x) {
	x = C_get_altrepPointer(x);
	return R_altrep_data1(x);
}

// [[Rcpp::export]]
SEXP C_get_altrep_data2(SEXP x) {
	x = C_get_altrepPointer(x);
	return R_altrep_data2(x);
}



// [[Rcpp::export]]
SEXP C_create_altrep(SEXP x, SEXP state) {
	String dataType= as<String>(DATA_TYPE(state));
	R_altrep_class_t altrep_class= get_altrep_class(dataType);
	SEXP res = R_new_altrep(altrep_class, x, wrap(state));
	return(res);
}


#define typeMatch \
X("raw",char*)\
X("logical",bool *)\
X("integer", int *)\
X("real", double *)
// Finalizer of a pointer to the shared memory
static void ptr_finalizer(SEXP extPtr) {
	DEBUG(Rprintf("Finalizing data pointer\n"));
	void* ptr = R_ExternalPtrAddr(extPtr);
	String operation= as<String>(R_ExternalPtrTag(extPtr));
	String dataType= as<String>(R_ExternalPtrProtected(extPtr));
	if (operation == "none") {
		return;
	}
	try {
		if (operation == "delete") {
			DEBUG(Rprintf("delete data pointer\n"));
#define X(R_TYPE,C_TYPE) if(dataType==R_TYPE) delete (C_TYPE)ptr;
			typeMatch
#undef X

		}
		if (operation == "free") {
			DEBUG(Rprintf("free data pointer\n"));
			free(ptr);
		}
	}
	catch (exception exp) {
		errorHandle("Error in releasing the pointer:%s\n", exp.what());
	}
}
#undef typeMatch

// [[Rcpp::export]]
SEXP C_get_finalizer(SEXP Extptr,SEXP operation,SEXP dataType) {
	SEXP finalizerPtr = PROTECT(
		R_MakeExternalPtr(R_ExternalPtrAddr(Extptr), operation, dataType)
	);
	R_RegisterCFinalizer(finalizerPtr, ptr_finalizer);
	UNPROTECT(1);
	return finalizerPtr;
}

// [[Rcpp::export]]
void C_attachAttr(SEXP R_source, SEXP R_tag, SEXP R_attr) {
	const char* tag = R_CHAR(Rf_asChar(R_tag));
	Rf_setAttrib(R_source, Rf_install(tag), R_attr);
}

// [[Rcpp::export]]
SEXP C_format_lenght(R_xlen_t length) {
	return(wrap(length));
}



// [[Rcpp::export]]
void C_set_duplicate_method(SEXP x, SEXP value) {
	x = C_get_altrepPointer(x);
	SEXP status = R_altrep_data2(x);
	status = PROTECT(Rf_shallow_duplicate(status));
	SET_DUPLICATE_METHOD(status, value);
	R_set_altrep_data2(x, status);
	UNPROTECT(1);
}

// [[Rcpp::export]]
void C_set_coerce_method(SEXP x, SEXP value) {
	x = C_get_altrepPointer(x);
	SEXP status = R_altrep_data2(x);
	status = PROTECT(Rf_shallow_duplicate(status));
	SET_COERCE_METHOD(status, value);
	R_set_altrep_data2(x, status);
	UNPROTECT(1);
}


// [[Rcpp::export]]
void C_set_reference_count(SEXP x, int count) {
	SET_NAMED(x, count);
}


//Associated with testthat
// [[Rcpp::export]]
SEXP test_int(int len) {
	int* res = new int[len];
	return(R_MakeExternalPtr(res, R_NilValue, R_NilValue));
}
// [[Rcpp::export]]
SEXP test_logical(int len) {
	bool* res = new bool[len];
	return(R_MakeExternalPtr(res, R_NilValue, R_NilValue));
}