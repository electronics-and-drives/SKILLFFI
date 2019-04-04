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

Start by setting the ```FFIPATH``` environment variable and
loading ```ffi.ils```:

```scheme
(setShellEnvVar "FFIPATH" "/path/to/skillFFI")
(load "ffi.ils")
```

This should print a confirmation

```
FFI up and running
```

If it doesn't, it's probably due to a wrong path in ```FFIPATH```.

To load a shared library either, the complete and absolute path must be provided
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
(define sqrtf (ffiDefun libm "sqrtf" "float" "float"))
```

If successful ```ffiDefun``` returns a handle to the prototyped function, which
can be called with ```ffiCallFun```.

```scheme
(define res (ffiCallfun sqrtf 144.0))
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

```(ffiDefun t_libHandle t_fnName t_rType t_aType1 [ t_aType2 ... ]) => u_function```

Defines the prototype for a C function and returns a function object to it if succesful.
The _t\_libHandle_ is a handle returned by ```ffiOpenLib```. The name of the function
has to be known in advance and passed as _t\_fnName_. The return type of the function
is the string _t\_rType_. _t\_aTypes_ is a string list with the types of arguments.

+ _t\_libHandle_ is a string containing a handle to an open shared library.
+ The name of the function is a string in _t\_fnName_.
+ _t\_rType_ is the return type of the function as a string.
+ _t\_aTypes_ are strings containing the types of the function arguments.

**ffiCallFun**

```(ffiCallFun t_fnHandle x_fnArg1 [ x_fnArg2 ... ]) => x_result```

Calls a previously defined function with the given arguemnts in _fnArgs_ and
returns whatever this function call retured. The returned function object of 
```ffiDefun``` calls this function.

+ _t\_fnHandle_ is a string containing a function handle obtained by ```ffiDefun```.
+ _x\_fnArgs_ are arguments corresponding to the correct types given in the definition.

**ffiCloseLib**

```ffiCloseLib t_libHandle)```

Closes the library.
