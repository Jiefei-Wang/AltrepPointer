#' Wrap a pointer
#' 
#' This function simply creates a wrapper for a vector pointer. There is no memory duplication 
#' during the function call. the behavior of the return value of this function is exactly 
#' the same as a regular R vector. Users can access and modify the vector's value via `[` and `[<-`. 
#' Any modification on the R vector will change the corresponding value in the 
#' vector pointer.
#' 
#' @param x An ExternalPtr object, the pointer is the address of the data.
#' @param length numeric, the length of the vector
#' @param dataType character, the type of the data
#' @param duplicateMethod character, the behavior of the duplication function.
#' Default is none, see details.
#' @param coerceMethod character, the behavior of the coercion function.
#' Default is none, see details.
#' @param finalizer character, the behavior of the finalizer.
#' Default is none, see details.
#' @param attributes list, attributes that will be attached to the return value. The most
#' common usage is to create a matrix.
#' @param duplicateErrorMessage character, the error message that will be shown 
#' when duplicateMethod="error" and a duplication occurs
#' @param coerceErrorMessage character, the error message that will be shown 
#' when coerceMethod="error" and a type conversion occurs
#' 
#' @details 
#' For creating a wrapper for a pointer, the required arguments
#' are `x`, `length` and `dataType`. The other arguments
#' controls the behavior of the wrapper object and by default they
#' would not do anything.
#' 
#' The current supported types of C++ vector pointer are `char*`, `bool*`, `int*` 
#' and `double*`, which corresponds to `raw`, `logical`, `integer` and `real` in 
#' the argument `dataType` respectively. For logical vector pointer, since R uses
#' `int*` to represent it internally, this mismatch makes the ALTREP API `getDataPtr`
#' invalid. Therefore, when the function `getDataPtr` is called for logical data
#' type, an error message will be shown. This makes many R functions not being able
#' to process the logical vector pointer(Which they should not be, e.g. `print`).
#' However, users can still access(without modification) individual elements of the 
#' logical vector through `[` operator.
#' 
#' `duplicateMethod` determines the behavior of the duplication function of 
#' a wrapper. When a duplication is required, `duplicateMethod="duplicate"` will
#' duplicate the object and result in a normal R vector; 
#' `duplicateMethod="sameObject"` will return the same object without performing
#' the duplication; `duplicateMethod="error"` will trigger an error and prevent
#' the wrapper from duplication.
#' 
#' Similarly, `coerceMethod` determines the behavior of the coercion function.
#' If `coerceMethod="coerce"`, the wrapper can be coerced to the other data type
#' and results in a normal R vector. If `coerceMethod="error"`, an error will
#' be triggered and prevent the wrapper from duplication.
#' 
#' `finalizer` controls whether to release the memory associated with a pointer.
#' `finalizer="none"` means not releasing the memory. `finalizer="delete"`
#' will release the memory through `delete` operator in C++ and `finalizer="free"`
#' will release it through `free` operator.
#' 
#' 
#' 
#' @examples 
#' n=10
#' 
#' ## This is a C++ function to create an integer
#' ## vector pointer of length n. It is for illustration purpose 
#' ## only and user should not call this function.
#' ## The return value is an ExternalPtr object
#' 
#' intPtr=AltrepPointer:::test_int(n)
#' 
#' ## Create a wrapper for the ExternalPtr object
#' ## No duplication and coercion is allowed in order
#' ## to make sure the object always uses data from the
#' ## pointer.
#' 
#' intVec=wrapPointer(intPtr,n,dataType="integer",
#'                    duplicateMethod = "error",
#'                    coerceMethod = "error",
#'                    finalizer="delete",
#'                    duplicateErrorMessage = "duplication is not allowed",
#'                    coerceErrorMessage = "coercion is not allowed"
#'                   )
#' 
#' intVec
#' 
#' ## There is no difference between the wrapper and a integer vector in R
#' 
#' typeof(intVec)
#' is.atomic(intVec)
#' is.integer(intVec)
#' 
#' ## You can change the value in the vector point
#' 
#' intVec[1]=10L
#' 
#' ## If multiple variable name is associated with the same object,
#' ## A memery dulplication will be triggered and an error will be
#' ## thrown out to prevent the pointer from duplication.
#' 
#' tmp=intVec
#' tryCatch({intVec[1]=10L},error = function(e) message(e))
#' 
#' ## You can change the duplication method to "sameObject" and force R
#' ## to keep the same wrapper object after the duplication. The side effect
#' ## is that `tmp` variable will also be modified since they share the same
#' ## vector pointer
#' 
#' setDuplicateMethod(intVec,"sameObject")
#' intVec[1]=10L
#' intVec[1]
#' 
#' tmp[1]
#' 
#' @export
wrapPointer<-function(x,length,
                       dataType=c("raw","logical","integer","real"),
                       duplicateMethod=c("duplicate","sameObject","error"),
                       coerceMethod=c("coerce","error"),
                       finalizer=c("none","delete","free"),
                       attributes=NULL,
                       duplicateErrorMessage=NULL,
                       coerceErrorMessage=NULL
){
  if(missing(x)){
    stop("Data pointer has to be specified.")
  }
  if(missing(length)){
    stop("Data length has to be specified.")
  }
  if(missing(dataType)){
    stop("Data type has to be specified.")
  }
  
  
  length=C_format_lenght(length)
  dataType<- match.arg(dataType)
  duplicateMethod <- match.arg(duplicateMethod)
  coerceMethod <- match.arg(coerceMethod)
  finalizer <- match.arg(finalizer)
  finalizerObject=C_get_finalizer(x,finalizer,dataType)
  state=list("AltrepPointer",length,dataType,
             duplicateMethod=duplicateMethod,coerceMethod=coerceMethod,finalizer,
             duplicateErrorMessage,coerceErrorMessage,finalizerObject)
  res=C_create_altrep(x,state)
  
  for(i in names(attributes)){
    C_attachAttr(res,i,attributes[[i]])
  }
  C_reset_reference_count(environment(),"res",res)
}

#' Get/Set the properties of the wrapper of a vector pointer
#' 
#' @param x an ALTREP vector, the return value of `wrapPointer` function
#' @param value character, the value you want to set.
#' @return 
#' `getExternalPtr`: the externalPtr object associated with the wrapper
#' `getDuplicateMethod` : character, the duplicate method
#' `getCoerceMethod` : character, the coerce method
#' @rdname property
#' @export
getExternalPtr<-function(x){
  return(C_get_altrep_data1(x))
}

#' @rdname property
#' @export
getDuplicateMethod<-function(x){
  satus=C_get_altrep_data2(x)
  return(satus[["duplicateMethod"]])
}
#' @rdname property
#' @export
getCoerceMethod<-function(x){
  satus=C_get_altrep_data2(x)
  return(satus[["coerceMethod"]])
}


#' @rdname property
#' @export
setDuplicateMethod<-function(x,value=c("duplicate","sameObject","error")){
  value <- match.arg(value)
  C_set_duplicate_method(x,value)
}


#' @rdname property
#' @export
setCoerceMethod<-function(x,value=c("coerce","error")){
  value <- match.arg(value)
  C_set_coerce_method(x,value)
}


