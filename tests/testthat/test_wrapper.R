context("Test wrapper")

test_that("creation",{
  n=10
  ptr=test_int(n)
  expect_error(wrapPointer(ptr,n))
  
  res=wrapPointer(ptr,n,dataType = "integer")
})

test_that("duplicate error",{
  n=10
  ptr=test_int(n)
  res=wrapPointer(ptr,n,dataType = "integer",duplicateMethod = "error")
  
  expect_error({res[1]=1L})
  
  expect_equal(getDuplicateMethod(res),"error")
  setDuplicateMethod(res,"sameObject")
  expect_equal(getDuplicateMethod(res),"sameObject")
  
})


test_that("coerce error",{
  n=10
  ptr=test_int(n)
  res=wrapPointer(ptr,n,dataType = "integer",coerceMethod="error")
  C_set_reference_count(res,0)
  
  expect_error({res[1]=1})
  
  expect_equal(getCoerceMethod(res),"error")
  setCoerceMethod(res,"coerce")
  expect_equal(getCoerceMethod(res),"coerce")
  
})



