# CG1112 ALEX Project

## TLS

### Running server
go to ./PI/TLS_server
command to start server :
```
./tls-alex-server
```

### Compiling server files
go to ./PI/TLS_server
run this command to compile:
```
g++ tls-alex-server.cpp tls_server_lib.cpp tls_pthread.cpp make_tls_server.cpp tls_common_lib.cpp serial.cpp serialize.cpp -pthread -lssl -lcrypto -o tls-alex-server

```

### Running client
go to ./Laptop/TLS_client
command to start client :
```
./tls-alex-client
```

### Compiling client files
go to ./Laptop/TLS_client
run this command to compile:
```
g++ tls-alex-client.cpp make_tls_client.cpp tls_client_lib.cpp tls_pthread.cpp tls_common_lib.cpp -pthread -lssl -lcrypto -o tls-alex-client

```

## LIDAR

### Running LIDAR and RVIZ

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

### Configure ROS master and slave

* Under own laptop
  add the following line to .bashrc
  ```
  export ROS_MASTER_URI=http://172.20.10.10:11311/
  ```
* In Raspberry Pi
  add the following line to .bashrc
  ```
  export ROS_MASTER_URI=http://lcoalhost:11311/
  ```

## Power saving

On Raspberry Pi, run the following lines
* Turn off HDMI:
   ```
   /usr/bin/tvservice -o
   ```








