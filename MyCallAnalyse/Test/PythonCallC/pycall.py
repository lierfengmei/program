import ctypes
ll = ctypes.cdll.LoadLibrary
lib = ll("./libpycall.so")
a =int(input("Please input the first integer:"))
b = int(input("Please input the second integer:"))
result=lib.foo(a,b)
print("result is:",result)
print("***finish*****")
