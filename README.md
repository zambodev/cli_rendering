# cli_rendering

Building a DOOM style image rendering in the command line


## How to use
```bash
# Compile
clang src/main.c -o render -lm 

# Run
./render <width> <height> <view_direnction>

```

NOTE: view_direction is in degrees

## Showcase

### Two walls corner render, FOV 90, ANGLE 45

Map:
![alt text](./showcase/corner_map.png)
Camera view:
![alt text](./showcase/corner_detect.png)
Actual render:
![alt text](./showcase/corner_render.png)
