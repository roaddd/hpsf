# hpsf
本项目简单实现了基于epoll的服务器框架，主要是学习使用。

## 使用方式
本项目分为两部分，第一部分包括include/、src/组成的基于虚函数回调实现的EchoServer；第二部分在coroutine/中，是基于C++20协程实现的EchoServer。

两部分均可按CMake的常用方式进行编译，即：
* 在主目录中新建build文件夹
* 在build文件夹中执行cmake ../与make
* 在build/bin目录中得到可执行文件

## 性能测试

两种方式测试的请求数量均为100000，并发数为2000，测试结果如下：

### 基于虚函数实现的回调版本

```
ab -k -c2000 -n 100000 http://127.0.0.1:8888/
This is ApacheBench, Version 2.3 <$Revision: 1528965 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient)
Completed 10000 requests
Completed 20000 requests
Completed 30000 requests
Completed 40000 requests
Completed 50000 requests
Completed 60000 requests
Completed 70000 requests
Completed 80000 requests
Completed 90000 requests
Completed 100000 requests
Finished 100000 requests


Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      2000
Time taken for tests:   23.721 seconds
Complete requests:      100000
Failed requests:        0
Non-2xx responses:      100000
Keep-Alive requests:    100000
Total transferred:      10600000 bytes
HTML transferred:       0 bytes
Requests per second:    4215.67 [#/sec] (mean)
Time per request:       474.421 [ms] (mean)
Time per request:       0.237 [ms] (mean, across all concurrent requests)
Transfer rate:          436.39 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   7.8      0      75
Processing:    34  470 154.1    451    1414
Waiting:       32  241 103.7    225    1088
Total:        102  471 153.6    451    1414

Percentage of the requests served within a certain time (ms)
  50%    451
  66%    492
  75%    517
  80%    534
  90%    626
  95%    751
  98%    938
  99%   1052
 100%   1414 (longest request)
```

### 基于C++20协程实现的版本
```
ab -k -c2000 -n 100000 http://127.0.0.1:8888/
This is ApacheBench, Version 2.3 <$Revision: 1528965 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient)
Completed 10000 requests
Completed 20000 requests
Completed 30000 requests
Completed 40000 requests
Completed 50000 requests
Completed 60000 requests
Completed 70000 requests
Completed 80000 requests
Completed 90000 requests
Completed 100000 requests
Finished 100000 requests


Server Software:        
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /
Document Length:        0 bytes

Concurrency Level:      2000
Time taken for tests:   0.854 seconds
Complete requests:      100000
Failed requests:        0
Non-2xx responses:      100000
Keep-Alive requests:    100000
Total transferred:      10600000 bytes
HTML transferred:       0 bytes
Requests per second:    117162.83 [#/sec] (mean)
Time per request:       17.070 [ms] (mean)
Time per request:       0.009 [ms] (mean, across all concurrent requests)
Transfer rate:          12128.18 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0    1   6.3      0      70
Processing:     5   15   3.9     15      36
Waiting:        5   15   3.9     15      36
Total:          5   16   7.9     15      91

Percentage of the requests served within a certain time (ms)
  50%     15
  66%     15
  75%     16
  80%     16
  90%     20
  95%     25
  98%     38
  99%     59
 100%     91 (longest request)
 ```