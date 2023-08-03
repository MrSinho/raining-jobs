
# raining-jobs

![windows-badge](exit_code.svg)

## [windows build logs:](https://github.com/mrsinho/shci)

```
system        : Windows
version       : 10.0.22621
platform      : Windows-10-10.0.22621-SP0
processor     : AMD64 Family 23 Model 17 Stepping 0, AuthenticAMD
min frequency : 0.0 MHz
max frequency : 3500.0 MHz
cores         : 4
```

build ran for `10.42 s` and terminated with exit code `0`

---

```bash
> .shci/windows/prerequisites-output.txt (
    choco install -y curl mingw cmake
    mingw-get install gcc mingw32-make gdb
)
```

```bash
Chocolatey v2.0.0
Installing the following packages:
curl;mingw;cmake
By installing, you accept licenses for the packages.
curl v8.1.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

Chocolatey installed 0/3 packages. 
 See the log for details (C:\ProgramData\chocolatey\logs\chocolatey.log).

Warnings:
 - cmake - cmake v3.26.4 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - curl - curl v8.1.2 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.
 - mingw - mingw v12.2.0.03042023 already installed.
 Use --force to reinstall, specify a version to install, or try upgrade.

```

---
    
```bash
> .shci/windows/build-output.txt (
    mkdir windows
    cd windows
    cmake -DRAINING_JOBS_BUILD_EXAMPLES=ON ..
    cmake --build .
)

```

```bash
-- Building for: Visual Studio 17 2022
-- Selecting Windows SDK version 10.0.22000.0 to target Windows 10.0.22621.
-- The C compiler identification is MSVC 19.36.32532.0
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: D:/Program Files/Microsoft Visual Studio/2022/Community/VC/Tools/MSVC/14.36.32532/bin/Hostx64/x64/cl.exe - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done (4.4s)
-- Generating done (0.1s)
-- Build files have been written to: D:/home/desktop/GitHub/raining-jobs/windows
MSBuild version 17.6.3+07e294721 for .NET Framework

  Checking Build System
  Building Custom Rule D:/home/desktop/GitHub/raining-jobs/CMakeLists.txt
  rainingJobs.c
D:\home\desktop\GitHub\raining-jobs\raining-jobs\src\rainingJobs.c(64,3): warning C4013: 'memcpy' non definita. Verr… considerata come esterna che restituisce un int. [D:\home\desktop\GitHub\raining-jobs\windows\raining-jobs.vcxproj]
D:\home\desktop\GitHub\raining-jobs\raining-jobs\src\rainingJobs.c(305,2): warning C4013: 'memset' non definita. Verr… considerata come esterna che restituisce un int. [D:\home\desktop\GitHub\raining-jobs\windows\raining-jobs.vcxproj]
  raining-jobs.vcxproj -> D:\home\desktop\GitHub\raining-jobs\bin\Debug\raining-jobs.lib
  Building Custom Rule D:/home/desktop/GitHub/raining-jobs/CMakeLists.txt
  raining-example.c
  raining-example.vcxproj -> D:\home\desktop\GitHub\raining-jobs\bin\Debug\raining-example.exe
  Building Custom Rule D:/home/desktop/GitHub/raining-jobs/CMakeLists.txt

```

---
    
build ran for `10.42 s` and terminated with exit code `0`

---

