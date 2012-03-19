

## Commandline (dcpl)

`$  curl 'http://example.com/some-data' |dcpl| process-data`

Works as a basic (and in this case somewhat unnecessary) pipe buffer. 


dcpl [PATH]
      PATH: The decoupler file (the buffer / file w/ complete input data so
              far).  Defaults to a temporary file
 -d         Go dormant after stdin is finished instead of finalizing. (EOF is
              not passed to the destinations yet). Can be used to allow for
              resumption if the input somehow gets killed or if you want to
              concatenate from multiple inputs (sequentially).
 -i         Print state of specified decoupler

   # (Possible future enhancements)
 -r [RATE]  Rate-limit throughput
 -m         Multiplex/interlace multiple inputs instead of applying them
            sequentially (not supported, and won't support until someone really
              needs it).




## DIO
 (defaults to nonblocking)

dio\_open\_reader   (path, start\_offset=0)
dio\_open\_writer   (path, ?..)

dio\_close  (
dio\_read
dio\_write


Gives back simple struct w/ file-descriptor that's epollable.


(read -> write-to-socket
 vs
 sendfile
 vs
 splice -> unix socket -> splice-to-socket
)
(because if the third one isn't faster than the first, and ideally about as
fast as the second, that's a problem...)

### Decoupler States

- **Expecting** Subscribers but no source yet.
- **Active**    Data flowing into the decoupler, may or may not be currently flowing out
- **Flushing**  Data has ceased flowing in but is still flowing out to remaining sinks
- **Dormant**   No active source pushing data in, but not marked as "finished" either.
- **Finished**  Current end of file actually gets passed along as an EOF





# auto-writer



# auto-reader




| dcpl state   | mgr state | action | result                                          |
|--------------|-----------|--------|-------------------------------------------------|
| undetermined | open      | reader | pass-reader; [dcpl:expecting]                   |
| undetermined | open      | writer | pass-writer; [touch]; [dcpl:active]             |
| undetermined | none      | reader | exec; pass-reader; [dcpl:expecting]             |
| undetermined | none      | writer | exec; pass-writer; [touch]; [dcpl:active]       |
|              |           |        |                                                 |
| expecting    | open      | reader | pass-reader                                     |
| expecting    | open      | writer | pass-writer; [touch]; [dcpl:active]             |
| expecting    | none      | reader | warn; exec; pass-reader                         |
| expecting    | none      | writer | warn; exec; pass-writer; [touch]; [dcpl:active] |
|              |           |        |
| active       | open      | reader |
| active       | open      | writer |
| active       | none      | reader |
| active       | none      | writer |
|              |           |        |
| flushing     |
|              |           |        |
| dormant    |
|              |           |        |
| finished   |


no manager and state != undetermined --->

| Finish type                       | Has-content | Set-state                        |
|-----------------------------------|-------------|----------------------------------|
| writer fd EOF'ed                  | yes         | finished/dormant                 |
| writer fd EOF'ed                  | no          | finished/dormant                 |
| writer fd errored/premature-close | yes         | finished/dormant                 |
| writer fd errored/premature-close | no          | write-error --> dormant          |
| mgr errored out / killed grizzly  | yes         | a/fl --> finished/dormant        |
| mgr errored out / killed grizzly  | no          | e/a  --> write-error --> dormant |
| mgr killed nicely                 | yes         | finished/dormant (ovr w/ signal) |
| mgr killed nicely                 | no          | finished/dormant (ovr w/ signal) |

reader errors out- no problem (depending on error maybe info/debug/warn)


(add-reader when in finished state- still do socket-pair and then sendfile to
socket so that it's pollable in an expected way)

need to store:
 - will-be-dormant vs. will-be-finished (possibly changable via api)
 - current state
