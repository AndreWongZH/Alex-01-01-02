# CG1112 ALEX Project

## TLS

### running server
go to ./TLS_server
command to start server :
```
./tls-alex-server
```

### compiling server files
go to ./TLS_server
run this command to compile:
```
g++ tls-alex-server.cpp tls_server_lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp serial.cpp serialize.cpp -pthread -lssl -lcrypto -o tls-alex-server

```

## LIDAR

steps to start the lidar
1. start roscore on Raspberry Pi
2. run command on Raspberry Pi
   ```
   source devel/setup.bash
   roslaunch rplidar_ros rplidar.launch
   ```
3. run command on own laptop
   ```
   source devel/setup.bash
   roslaunch rplidar_ros view_hectorSlam.launch
   ```












