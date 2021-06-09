default: cmake/HunterGate.cmake
	cmake -H. -B_builds -DHUNTER_STATUS_DEBUG=ON -DCMAKE_BUILD_TYPE=Release
	cmake --build _builds --config Release

cmake:
	mkdir cmake

cmake/HunterGate.cmake: cmake
	curl https://raw.githubusercontent.com/cpp-pm/gate/master/cmake/HunterGate.cmake > cmake/HunterGate.cmake
