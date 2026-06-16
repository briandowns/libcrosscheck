# Crosscheck

[![Build and Deploy](https://github.com/briandowns/crosscheck/actions/workflows/main.yml/badge.svg)](https://github.com/briandowns/crosscheck/actions/workflows/main.yml/badge.svg)

Crosscheck is a very simple unit testing library for C code.

## Build and Install

```sh
make
```

```sh
sudo make install
```

You can also build it as a static library.

```sh
make static
```

## Usage

Initialize the library with the below macro.

```c
CC_INIT;
```

Running tests.

```c
CC_RUN(test_func);
```

Complete testing and print out results, and set return code.

```c
CC_COMPLETE;
```

## Test Functions

Test functions have 1 requirement. They need to return a `cc_result_t` type. This is done automatically for you using any of the provided assert macros for failure scenarios and a simple macro is provided (`CC_SUCCESS`) to be put at the end of the function. Check out the example.c file for a full examples.

Though not a requirement, all test function names should be prepended with "test_".

## Asserts

The library is meant to be as simple as possible by providing obvious macros that take 2 arguments; the actual value and the expected value.

### Equality

* CC_ASSERT_NULL
* CC_ASSERT_TRUE
* CC_ASSERT_CHAR_EQUAL
* CC_ASSERT_FLOAT_EQUAL
* CC_ASSERT_DOUBLE_EQUAL
* CC_ASSERT_LONG_EQUAL
* CC_ASSERT_LONG_LONG_EQUAL
* CC_ASSERT_INT_EQUAL
* CC_ASSERT_INT8_EQUAL
* CC_ASSERT_INT16_EQUAL
* CC_ASSERT_INT32_EQUAL
* CC_ASSERT_INT64_EQUAL
* CC_ASSERT_UINT_EQUAL
* CC_ASSERT_UINT8_EQUAL
* CC_ASSERT_UINT16_EQUAL
* CC_ASSERT_UINT32_EQUAL
* CC_ASSERT_UINT64_EQUAL
* CC_ASSERT_STRING_EQUAL

### Inequality

* CC_ASSERT_NOT_NULL
* CC_ASSERT_FALSE
* CC_ASSERT_CHAR_NOT_EQUAL
* CC_ASSERT_FLOAT_NOT_EQUAL
* CC_ASSERT_DOUBLE_NOT_EQUAL
* CC_ASSERT_LONG_NOT_EQUAL
* CC_ASSERT_LONG_LONG_NOT_EQUAL
* CC_ASSERT_INT_NOT_EQUAL
* CC_ASSERT_INT8_NOT_EQUAL
* CC_ASSERT_INT16_NOT_EQUAL
* CC_ASSERT_INT32_NOT_EQUAL
* CC_ASSERT_INT64_NOT_EQUAL
* CC_ASSERT_UINT_NOT_EQUAL
* CC_ASSERT_UINT8_NOT_EQUAL
* CC_ASSERT_UINT16_NOT_EQUAL
* CC_ASSERT_UINT32_NOT_EQUAL
* CC_ASSERT_UINT64_NOT_EQUAL
* CC_ASSERT_STRING_NOT_EQUAL

## Contributing

Please feel free to open a PR!

## Contact

Brian Downs [@bdowns328](http://twitter.com/bdowns328)

## License

BSD 2 Clause [License](/LICENSE).
