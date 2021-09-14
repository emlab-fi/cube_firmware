# Error ID
- 32bit unsigned int
- the 32bits are divided such that top 8bits are `namespace`, lower 24bits are `error_code`
- `error_code` is further divided,  higher 8bits `error_category` and lower 16bits `error_id`
- the complete error code is calculated as follow: `namespace + error_category + error_id`
- all zeroes correspond to no error

# Error namespaces and codes
## namespace cube (+ 0xaa000000)
### category message (+ 0x010000)
- 1 : message is not a command message 

### category decode  (+ 0x020000)
- 1 : decoding error
- 2 : wrong payload

### category planner (+ 0x030000)
- 1 : out of bounds
- 2 : conversion error
- 3 : misc error

### category hw_transfers (+ 0x040000)

### category hw_movement (+ 0x050000)
- 1 : error while in velocity mode
- 2 : error while in step mode
- 3 : unexpected endstop triggered

### category hw_gpio (+ 0x060000)

### category hw_misc (+ 0x070000)

### category misc (+ 0x080000)
- 1 : fatal error in cube_lib::controller
- 2 : instruction not implemented
