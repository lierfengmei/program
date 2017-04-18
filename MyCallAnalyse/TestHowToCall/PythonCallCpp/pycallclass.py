import ctypes
so = ctypes.cdll.LoadLibrary
lib = so("./libpycallclass.so")
print("display()")
lib.display()
print("display(300)")
lib.display_int(300)

