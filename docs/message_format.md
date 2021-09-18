# Messaging
- uses protocol buffers for schema and serialization
- framing described later
- 2 main messages types
    - Command: PC -> Cube
    - Reply: Cube -> PC

## Command msg
- used for instruction Cube on what to do next
- random ID for identification
- always includes instruction (can be _nop_)
- optional payload with data for instruction

## Reply msg
- replies to the command message
- has nested Status msg
- includes ID of the incoming Command msg that it is replying to
- includes result of instruction(fail, success, etc.)
- optional payload with data

## Status msg
- nested into Reply msg
- contains info about Cube - errors, IO status, position, mode
- in future could be sent as a heartbeat message periodically

## Framing
- header: 0x55 0x55 0x55
- message type
- length
- message encoded by protobuff

- message type:
    - 0x01 - command
    - 0x02 - reply