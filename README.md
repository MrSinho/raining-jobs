# Raining jobs

_See the windows and linux build logs:_

[![](.shci/windows/exit_code.svg)](.shci/windows/log.md)
[![](.shci/linux/exit_code.svg)](.shci/linux/log.md)

"Raining jobs" is a simple and stable algorithm used to quickly sort numbers (with size ranging from one byte to 8 bytes, floats and integers, signed and unsigned) without exceeding with too much extra memory and iterations: in the worst case scenario, with a 64 bit resolution, the maximum number of checks is `n_samples * 64`, while the memory usage is always stable and equal to three times the original set size, with the destination buffer included. The library is written in C.

---

## Build and implementation

```bash
git clone --recursive https://github.com/mrsinho/raining-jobs.git
cd raining-jobs
mkdir build
cd build
cmake .. -DRAINING_JOBS_BUILD_EXAMPLES=ON
cmake --build .
```

See the [`raining-example.c`](./examples/src/raining-example.c) for reference.

---

## Supported formats

The raining-jobs library supports all of the following memory formats and memory types: 

|Memory format         | Supported memory/data types                                                                     | 
|----------------------|-------------------------------------------------------------------------------------------------|
|Magnitude complement  | all signed and unsigned integer types, from 8 to 64 bits                                        |
|One complement        | all signed and unsigned integer types, from 8 to 64 bits                                        |
|Two complement        | all signed and unsigned integer types, from 8 to 64 bits                                        |
|IEEE 754              | signed and unsigned floating point numbers with single (32 bits) and double precision (64 bits) |

## Benchmarks

The following tests have been performed on the following system (release build):
```
system        : Windows
version       : 10.0.22621
platform      : Windows-10-10.0.22621-SP0
processor     : AMD64 Family 23 Model 17 Stepping 0, AuthenticAMD
max frequency : 3500.0 MHz
```

Note:
> The algorithm has a notation of `O(n)`

With 10 thousand values to sort:

|Bit resolution/type | source length        | mean elapsed time (5 tests) |
|--------------------|----------------------|-----------------------------|
|`float` (32 bits)   | 1E4                  | `1.8 ms`                    |
|`double` (64 bits)  | 1E4                  | `1.6 ms`                    |

With 100 thousand values to sort:

|Bit resolution/type | source length        | mean elapsed time |
|--------------------|----------------------|-------------------|
|`float` (32 bits)   | 1E5                  | `19.0 ms`         |
|`double` (64 bits)  | 1E5                  | `20.6 ms`         |

With a million values to sort:

|Bit resolution/type | source length        | mean elapsed time |
|--------------------|----------------------|-------------------|
|`float` (32 bits)   | 1E6                  | `243.0 ms`        |
|`double` (64 bits)  | 1E6                  | `250.0 ms`        |


---