![js++Doc](logo.png)

## JS++ Documentation Generator

This is the *new* JS++ documentation generator. The previous documentation 
generator did not generate multiple pages (one page for each class, field, 
method) - which is necessary for the Standard Library documentation.

### Build

```
$ apt-get install libpcre3-dev
$ git submodule update --init --recursive
```

```
$ mkdir build && cd $_
$ cmake ..
$ make
```

### Code Coverage

Code coverage is only available for Linux `gcc` via `lcov`/`gcov`.

```
$ mkdir build && cd $_
$ cmake -DCMAKE_BUILD_TYPE=Coverage ..
$ make
$ make docgen_coverage
```

The generated code coverage files will be available in `build/docgen_coverage`.

### Documentation

To generate documentation with `Doxygen`:

```
$ mkdir docs && cd $_
$ doxygen ../Doxyfile
```

### Legal

Copyright (c) 2017 Onux Corporation
