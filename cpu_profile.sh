cd cmake-build-debug/websocket_server
LD_PRELOAD=/usr/local/lib/libprofiler.so CPUPROFILE=prof.out ./distributed_websockets_server
pprof --callgrind distributed_websockets_server prof.out > prof.callgrind
kcachegrind prof.callgrind
cd ../..
