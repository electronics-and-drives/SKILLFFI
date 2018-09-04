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

Start by loading _ffi.il_ in the CIW:

```scheme
(load "ffi.il")
```

This should print a confirmation

```scheme
FFI up and running
```

To load a shared library either the complete and absolute path must be provided
or it has to be located in the LD\_LIBRARY\_PATH environment variable.

Loading the C math library is accomplished like so:

```scheme
(setq libm (ffiOpenLib "libm.so")
```

```ffiOpenLib``` loads the library and returns a memory address as handle, which
in this example will be stored in the variable ```libm```.
Since there is no access to header files in SKILL the function prototype must be
defined manually.

```scheme
(setq sqrtf (ffiDefun libm "sqrtf" "float" (list "float")))
```

If successful ```ffiDefun``` returns a handle to the prototyped function, which
can be called with ```ffiCallFun```.

```scheme
(setq res (ffiCallfun fnSqrt (list 144.0)))
```

## Function Reference

**ffiOpenLib**

```(ffiOpenLib libName) => libHandle```

Opens a shared library and returns a handle to it. _libName_ can either be the
entire absolute path to a library or just its name if the path is in the
LD\_LIBRARY\_PATH environment variable.

+ _libName_ is a string with the entire absolute path to the library.

**ffiDefun**

```(ffiDefun libHandle fnName rType aTypes) => fnHandle```

Defines the prototype for a C function and returns a handle to it if succesful.
The _libHandle_ is a handle returned by ```ffiOpenLib```. The name of the function
has to be known in advance and passed as _fnName_. The return type of the function
is the string _rType_. _aTypes_ is a string list with the types of arguments.

+ _libHandle_ is a string containing a handle to an open shared library.
+ The name of the function is a string in _fnName_.
+ _rType_ is the return type of the function as a string.
+ _aTypes_ is a string list containing the types of the function arguments.

**ffiCallfun**

```(ffiCallfun fnHandle fnArgs) => result```

Calls a previously defined function with the given arguemnts in _fnArgs_ and
returns whatever this function call retured.

+ _fnHandle_ is a string containing a function handle obtained by ```ffiDefun```.
+ _fnArgs_ is a list of arguments corresponding to the correct types given in the definition.

**ffiCloseLib**

```ffiCloseLib libHandle)```

Closes the library.