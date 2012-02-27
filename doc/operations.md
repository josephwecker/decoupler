



OUTPUT
 * file   : Immediately send all data available. Non-blocking on Linux.
 * socket : "Pre-send" before even epolling- will work a lot of the time. Wait for epoll
 * pipe   : As epoll allows



- General signal
- Input has data ready to read
- Input closed normal
- Input closed abnormal
- Output ready to write
- Output closed


- canonical is being appended to without libdio


handoff: give it a new reader or writer.





