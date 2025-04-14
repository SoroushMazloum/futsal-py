## How to use py2d base?

For using py2d base, first clone the base from this repository.

Make sure you have futsal library installed [link](https://github.com/RCSS-IR/futsal-challenge)

After cloning run
 ```
./generate
```
to generate the required gRPC file.

In the next step download the proxy:
```
cd scripts
./download-proxy.sh
```
For starting the players, run rcssserver and monitor then execute this command in base directory:
```
./start.sh --use-random-rpc-port
```


py2d base for futsal challenge is a modified version of py2d in Cross Language Soccer Framework [link](https://github.com/CLSFramework/)

