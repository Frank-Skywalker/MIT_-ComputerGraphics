#  List of Command Line Arguments



### Assignment 1

| `-input *<filename>*`           | the input scene                                              |
| ------------------------------- | ------------------------------------------------------------ |
| `-output *<filename>*`          | where the raytraced image should be saved                    |
| `-size *<width> <height>*`      | the resolution of the output                                 |
| `-depth *<filename> <t0> <t1>*` | where the depth visualization image should be saved, and the range of t values which should be represented with shades of grey |

### Assignment 2

| `-normals *<filename>*` | where the surface normal visualization image should be saved |
| ----------------------- | ------------------------------------------------------------ |
| `-shade_back`           | perform lighting calculations for both sides of the objects default: light only the "front" side |

### Assignment 3

| `-gui`                                  | previsualize this scene with OpenGL                          |
| --------------------------------------- | ------------------------------------------------------------ |
| `-tessellation *<num theta> <num phi>*` | resolution of sphere primitive polygonization in OpenGL previsualization |
| `-gouraud`                              | use gouraud interpolation to previsualize sphere primitives default: use flat (polygonal) shading |

### Assignment 4

| `-bounces *<num>*`   | maximum number of bounces traced per eye ray                 |
| -------------------- | ------------------------------------------------------------ |
| `-weight *<weight>*` | the recursive ray tree is pruned for branches which contribute less than this amount to the final pixel color |

### Assignment 5

| `-grid *<nx> <ny> <nz>*` | the size of the grid                                     |
| ------------------------ | -------------------------------------------------------- |
| `-visualize_grid`        | visualize the occupied grid cells                        |
| `-visualize_grid_count`  | OPTIONAL: visualize the number of elements per grid cell |

# OpenGL Previsualization Options

### Assignment 3



------

| key '`r`' | render the scene                                             |
| --------- | ------------------------------------------------------------ |
| key '`t`' | visualize the ray tree for the pixel rotate the scene after pressing this button to see the ray tree |