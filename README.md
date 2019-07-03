# AltrepPointer
A wrapper for a C++ vector pointer, users can access and modify entries in the vector pointer like a regular R vector.

# 1. Simple example
To show the usage of the package, we first allocate a vector of length n in C++ and then we call `wrapPointer` function to create
a wrapper for the vector pointer.

## 1.1 C++ code
The function creates a sequence vector pointer 0:(n-1) and return it as an external pointer object.

```
SEXP test_int(int n) {
	int* res = new int[n];
	for (int i = 0; i < n; i++)res[i] = i;
	return(R_MakeExternalPtr(res, R_NilValue, R_NilValue));
}
```


## 1.2 R code
This R code call the C++ function and allocate a vector of length 10. To create a wrapper for the pointer, we need 
to specify the pointer, length, and pointer type. For this example, they are `intPtr`, `10` and "integer" respectively.
Since the pointer is allocated via `new` operator in C++, we set finalizer to use `delete` operator to free the pointer
after no variable is associated with the pointer wrapper. If finalizer is not specified, the pointer will not be freed
by the wrapper after use.

```
n=10
intPtr=test_int(n)
intPtrVec=wrapPointer(intPtr, n, dataType="integer", finalizer="delete")
```

Then we can access its value through `[` operator and modify it through `[<-` operator.
```
> intPtrVec
[1] 0 1 2 3 4 5 6 7 8 9

> intPtrVec[1]
[1] 0

> intPtrVec[1]=10L

> intPtrVec
[1] 10  1  2  3  4  5  6  7  8  9
```

