Server and client for measuring uplink and dowlink speed  on a network
To compile server cmake and c compiler is required on a posix complient system:

    cd throughput-mes/server/bin-server/
    cmake ../
    make

To start the server invoke the binary with an optional port number on which the server listens:

    cd throughput-mes/server/bin-server/
    ThroughputMesServer [-p port_number]


The client needs python3 installed on the machine. To start the client invoke the script
with the name or ip of the remote host, with the optional port on which the client
tries to connect to the server:

    client_main.py [-p port_number]  remote_server


Verfied on:

    Ubuntu 16.04
    gcc version 5.4.0
    cmake version 3.5.1
    python3 version 3.5.2


Further improvements:

* Remove forking from the server and make it start on a fix number of threads
  (can be the number of cores in the CPU). Make the sockets non-blocking and use epoll()
  to constantly check whether there is data on them.
* Improve tracing, use python logger in the client
* Recieving the cotnrol word on the server can happen in more than one go, currently the
  server is unable to handle this


Example test result
Server: AWS EC2 Ubuntu 16.04
Client: Home desktop Ubuntu 16.04 over Virtualbox Windows 8

client_main.py -p 55555 ec2-35-160-72-197.us-west-2.compute.amazonaws.com
Measure (D)ownlink, (U)plink or (E)xit? Please press one alternative:d
Please add the length of the measurement stream between 0 and 2^32:10000
Time of download 0.4115593659998922
Approximate downlink speed is 23.728350772127875 kilobytes /sec

Measure (D)ownlink, (U)plink or (E)xit? Please press one alternative:u
Please add the length of the measurement stream between 0 and 2^32:10000
Time of upload 1.52188960400008
Approximate uplink speed is 6.416776206587115 kilobytes /sec

