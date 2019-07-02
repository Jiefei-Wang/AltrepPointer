#include "Rcpp.h"
#include "altrep_functions.h"
#include "R_ext/Altrep.h"
#include "tools.h"
using namespace Rcpp;


#define ALT_COMMOM_REG(ALT_CLASS,ALT_TYPE,C_TYPE,R_TYPE)                   \
ALT_CLASS =R_make_##ALT_TYPE##_class(class_name, PACKAGE_NAME, dll);           \
/* override ALTREP methods */                                                  \
R_set_altrep_Inspect_method(ALT_CLASS, altrep_inspect);                  \
R_set_altrep_Length_method(ALT_CLASS, altrep_length);                    \
R_set_altrep_Duplicate_method(ALT_CLASS, altrep_duplicate);              \
R_set_altrep_Coerce_method(ALT_CLASS, altrep_coerce);                        \
/*R_set_altrep_Unserialize_method(ALT_CLASS, altrep_unserialize);*/          \
/*R_set_altrep_Serialized_state_method(ALT_CLASS, altrep_serialized_state);*/\
/* override ALTVEC methods */                                                  \
R_set_altvec_Dataptr_method(ALT_CLASS, altrep_dataptr);                  \
R_set_altvec_Dataptr_or_null_method(ALT_CLASS, altrep_dataptr_or_null);  \
R_set_altvec_Extract_subset_method(ALT_CLASS, altrep_subset<R_TYPE, C_TYPE>); \
/* override ALTREAL methods */                                                 \
R_set_##ALT_TYPE##_Elt_method(ALT_CLASS, altrep_Elt<C_TYPE>);                 \
R_set_##ALT_TYPE##_Get_region_method(ALT_CLASS, altrep_region<C_TYPE>);

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




R_altrep_class_t altrep_raw_class;
#define C_TYPE Rbyte
#define R_TYPE RAWSXP
//[[Rcpp::init]]
void init_raw_class(DllInfo* dll)
{
	char class_name[] = "altrep_pointer_raw";
	ALT_COMMOM_REG(altrep_raw_class, altraw, C_TYPE, R_TYPE)
}
#undef C_TYPE
#undef R_TYPE


R_altrep_class_t altrep_logical_class;
#define C_TYPE int
#define R_TYPE LGLSXP
//[[Rcpp::init]]
void init_logical_class(DllInfo* dll) {
	char class_name[] = "altrep_pointer_logical";
	ALT_COMMOM_REG(altrep_logical_class, altlogical, C_TYPE, R_TYPE)
}
#undef C_TYPE
#undef R_TYPE

R_altrep_class_t altrep_integer_class;
#define C_TYPE int
#define R_TYPE INTSXP
//[[Rcpp::init]]
void init_integer_class(DllInfo* dll) {
	char class_name[] = "altrep_pointer_integer";
	ALT_COMMOM_REG(altrep_integer_class, altinteger, C_TYPE, R_TYPE)
}
#undef C_TYPE
#undef R_TYPE


R_altrep_class_t altrep_real_class;
#define C_TYPE double
#define R_TYPE REALSXP
//[[Rcpp::init]]
void init_real_class(DllInfo* dll)
{
	char class_name[] = "altrep_pointer_real";
	ALT_COMMOM_REG(altrep_real_class, altreal, C_TYPE, R_TYPE)
}
#undef C_TYPE
#undef R_TYPE
