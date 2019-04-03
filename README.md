# SKILL FFI

With the Foreign Function Interface (FFI) for SKILL C-Type Libraries can
be loaded and used within the SKILL environment.

## State of Development

**This is not a finished product and still very early in development.**

For now the only data type supported is _float_ other data types as well
as defining custom structs is still in development.

## Dependencies

Build Utils:
+ GCC version 4.8.5
+ GNU Make version 3.82

Libraries:
+ ffi.h
+ dlfcn.h

## Getting Started

### Building

Make sure the latest version of GNU Make and GCC are installed and build the
project with

```bash
$ make ffi
```

This should result in an executable named ```skillFFI```.

### Example Usage

Start by loading ```ffi.ils```:

```scheme
(load "ffi.ils")
```

This should print a confirmation

```
FFI up and running
```

To load a shared library either the complete and absolute path must be provided
or it has to be located in the LD\_LIBRARY\_PATH environment variable.

Loading the C math library is accomplished like so:

```scheme
(define libm (ffiOpenLib "libm.so")
```

```ffiOpenLib``` loads the library and returns a memory address as handle, which
in this example will be stored in the variable ```libm```.
Since there is no access to header files in SKILL the function prototype must be
defined manually.

```scheme
(define sqrtf (ffiDefun libm "sqrtf" "float" (list "float")))
```

If successful ```ffiDefun``` returns a handle to the prototyped function, which
can be called with ```ffiCallFun```.

```scheme
(define res (ffiCallfun sqrtf (list 144.0)))
```

This stores the return value of a call to ```sqrtf``` in ```res```.

## Function Reference

**ffiOpenLib**

```(ffiOpenLib t_libName) => t_libHandle```

Opens a shared library and returns a handle to it. _t\_libName_ can either be the
entire absolute path to a library or just its name if the path is in the
LD\_LIBRARY\_PATH environment variable.

+ _t\_libName_ is a string with the entire absolute path to the library.

**ffiDefun**

```(ffiDefun t_libHandle t_fnName t_rType l_aTypes) => t_fnHandle```

Defines the prototype for a C function and returns a handle to it if succesful.
The _t\_libHandle_ is a handle returned by ```ffiOpenLib```. The name of the function
has to be known in advance and passed as _t\_fnName_. The return type of the function
is the string _t\_rType_. _t\_aTypes_ is a string list with the types of arguments.

+ _t\_libHandle_ is a string containing a handle to an open shared library.
+ The name of the function is a string in _t\_fnName_.
+ _t\_rType_ is the return type of the function as a string.
+ _l\_aTypes_ is a string list containing the types of the function arguments.

**ffiCallfun**

```(ffiCallfun t_fnHandle l_fnArgs) => x_result```

Calls a previously defined function with the given arguemnts in _fnArgs_ and
returns whatever this function call retured.

+ _t\_fnHandle_ is a string containing a function handle obtained by ```ffiDefun```.
+ _l\_fnArgs_ is a list of arguments corresponding to the correct types given in the definition.

**ffiCloseLib**

```ffiCloseLib t_libHandle)```

Closes the library.
