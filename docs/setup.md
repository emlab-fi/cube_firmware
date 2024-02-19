# Setup
- currently for vscode

## Dependencies
- vscode
- CMake 3.16 and above
- c and c++ compiler
- arm compiler with libraries
- openocd

### vscode extensions
- cmake tools
- cortex-debug

## Build
- open project
```bash
git clone ...
cd cube_firmware
code cube_firmware.code-workspace
```
- select active project: firmware
- select kit: arm-compiler (arm_none_eabi)
- resolve potential cmake issues ^^

## Upload
- TBD
- basic pipeline in .vscode/launch.json
