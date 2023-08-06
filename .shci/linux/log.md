
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

build ran for `5.60 s` and terminated with exit code `0`

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
-- Configuring done
-- Generating done
-- Build files have been written to: /mnt/d/home/desktop/GitHub/raining-jobs/linux
[ 50%] Built target raining-jobs
[100%] Built target raining-example

```

---
    
build ran for `5.60 s` and terminated with exit code `0`

---

