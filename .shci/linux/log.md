
# raining-jobs

![linux-badge](exit_code.svg)

## [linux build logs:](https://github.com/mrsinho/shci)

```
system        : Linux
version       : #1 SMP Fri Jan 27 02:56:13 UTC 2023
platform      : Linux-5.15.90.1-microsoft-standard-WSL2-x86_64-with-glibc2.31
processor     : 
min frequency : 0.0 MHz
max frequency : 0.0 MHz
cores         : 4
```

build ran for `8.07 s` and terminated with exit code `0`

---

```bash
function PREREQUISITES {
    sudo apt -y update
    sudo apt install -y gcc cmake make libc6-dev
}

PREREQUISITES > .shci/linux/prerequisites-output.txt
```

```bash
Hit:1 http://deb.debian.org/debian bullseye InRelease
Hit:2 http://security.debian.org/debian-security bullseye-security InRelease
Hit:3 http://deb.debian.org/debian bullseye-updates InRelease
Hit:4 http://ftp.debian.org/debian bullseye-backports InRelease
Reading package lists...
Building dependency tree...
Reading state information...
6 packages can be upgraded. Run 'apt list --upgradable' to see them.
Reading package lists...
Building dependency tree...
Reading state information...
cmake is already the newest version (3.18.4-2+deb11u1).
gcc is already the newest version (4:10.2.1-1).
libc6-dev is already the newest version (2.31-13+deb11u6).
make is already the newest version (4.3-4.1).
0 upgraded, 0 newly installed, 0 to remove and 6 not upgraded.

```

---
    
```bash
function BUILD {
	mkdir linux
    cd linux
    cmake -DRAINING_JOBS_BUILD_EXAMPLES=ON ..
    cmake --build .
}

BUILD > .shci/linux/build-output.txt

```

```bash
-- The C compiler identification is GNU 10.2.1
-- Detecting C compiler ABI info
-- Detecting C compiler ABI info - done
-- Check for working C compiler: /usr/bin/cc - skipped
-- Detecting C compile features
-- Detecting C compile features - done
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/Github/raining-jobs/linux
Scanning dependencies of target raining-jobs
[ 25%] Building C object CMakeFiles/raining-jobs.dir/raining-jobs/src/rainingJobs.c.o
[ 50%] Linking C static library ../bin/libraining-jobs.a
[ 50%] Built target raining-jobs
Scanning dependencies of target raining-example
[ 75%] Building C object CMakeFiles/raining-example.dir/examples/src/raining-example.c.o
[100%] Linking C executable ../bin/raining-example
[100%] Built target raining-example

```

---
    
build ran for `8.07 s` and terminated with exit code `0`

---

