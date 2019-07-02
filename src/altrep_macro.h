

#define DATA_PACKAGE_NAME(state) VECTOR_ELT(state,0)
#define DATA_LENGTH(state) VECTOR_ELT(state,1)
#define DATA_TYPE(state) VECTOR_ELT(state,2)
#define DUPLICATE_METHOD(state) VECTOR_ELT(state,3)
#define COERCE_METHOD(state) VECTOR_ELT(state,4)
#define FINALIZE_METHOD(state) VECTOR_ELT(state,5)
#define DUPLICATE_MESSAGE(state) VECTOR_ELT(state,6)
#define COERCE_MESSAGE(state) VECTOR_ELT(state,7)
#define FINALIZER(state) VECTOR_ELT(state,8)


#define SET_DUPLICATE_METHOD(state,value) SET_VECTOR_ELT(state,3,value)
#define SET_COERCE_METHOD(state,value) SET_VECTOR_ELT(state,4,value)

#define ALT_DATA_PTR(x) R_ExternalPtrAddr(R_altrep_data1(x))
#define ALT_DATA_LENGTH(x) DATA_LENGTH(R_altrep_data2(x))
#define ALT_DATA_TYPE(x) DATA_TYPE(R_altrep_data2(x))
#define ALT_DUPLICATE_METHOD(x) DUPLICATE_METHOD(R_altrep_data2(x))
#define ALT_COERCE_METHOD(x) COERCE_METHOD(R_altrep_data2(x))
#define ALT_FINALIZE_METHOD(x) FINALIZE_METHOD(R_altrep_data2(x))



