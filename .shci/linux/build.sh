function BUILD {
	mkdir linux
    cd linux
    cmake -DRAINING_JOBS_BUILD_EXAMPLES=ON ..
    cmake --build .
}

BUILD > .shci/linux/build-output.txt
