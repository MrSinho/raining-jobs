# Raining jobs

_See the windows and linux build logs:_

[![](.shci/windows/exit_code.svg)](.shci/windows/log.md)
[![](.shci/linux/exit_code.svg)](.shci/linux/log.md)

"Raining jobs" is a simple and stable algorithm used to quickly sort numbers (with size ranging from one byte to 8 bytes, floats and integers, signed and unsigned) without exceeding with too much extra memory and iterations: in the worst case scenario, with a 64 bit resolution, the maximum number of checks is `n_samples * 64`, while the memory usage is always stable and equal to three times the original set size, destination buffer included. The library is written in C.

---

## Benchmarks and comparison with other known sorting algorithms

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

## Dealing with negative numbers

2's complement
1's complement
Magnitude complement --> done

---

## Known issues

There is certainly room for improvements. The algorithm was first meant to be used within a multi-threading library, since it was designed to not create conflicts with read-write operations between multiple threads. Currently the library runs on the main application thread only.

---