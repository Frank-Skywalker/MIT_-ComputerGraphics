# Statistics



##### scene6_01_sphere.txt

```shell
raytracer -input scene6_01_sphere.txt -output output6_01a.tga -size 200 200 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -1.000000 -1.000000 -1.000000  -> 1.000000 1.000000 1.000000
  num grid cells             NULL
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        40000
  total cells traversed      0
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      1.0
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_01_sphere.txt -output output6_01b.tga -size 200 200 -grid 10 10 10 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -1.000000 -1.000000 -1.000000  -> 1.000000 1.000000 1.000000
  num grid cells             1000 (10x10x10)
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        5810
  total cells traversed      25204
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      0.1
  cells traversed per ray    0.6
********************************************
```



```shell
raytracer -input scene6_01_sphere.txt -output output6_01c.tga -size 200 200 -grid 10 10 10 -visualize_grid
```





##### scene6_02_sphere_triangles.txt

```shell
raytracer -input scene6_02_sphere_triangles.txt -output output6_02a.tga -size 200 200 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -3.000000 -1.000000 -3.000000  -> 3.000000 1.000000 3.000000
  num grid cells             NULL
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        120000
  total cells traversed      0
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      3.0
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_02_sphere_triangles.txt -output output6_02b.tga -size 200 200 -grid 10 10 10 -stats 
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -3.000000 -1.000000 -3.000000  -> 3.000000 1.000000 3.000000
  num grid cells             1000 (10x10x10)
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        33484
  total cells traversed      218996
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      0.8
  cells traversed per ray    5.5
********************************************
```



```shell
raytracer -input scene6_02_sphere_triangles.txt -output output6_02c.tga -size 200 200 -stats -shadows
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -3.000000 -1.000000 -3.000000  -> 3.000000 1.000000 3.000000
  num grid cells             NULL
  num non-shadow rays        40000
  num shadow rays            16030
  total intersections        163186
  total cells traversed      0
  rays per second            56030.0
  rays per pixel             1.4
  intersections per ray      2.9
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_02_sphere_triangles.txt -output output6_02d.tga -size 200 200 -grid 10 10 10 -stats -shadows 
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -3.000000 -1.000000 -3.000000  -> 3.000000 1.000000 3.000000
  num grid cells             1000 (10x10x10)
  num non-shadow rays        40000
  num shadow rays            16030
  total intersections        72776
  total cells traversed      339800
  rays per second            56030.0
  rays per pixel             1.4
  intersections per ray      1.3
  cells traversed per ray    6.1
********************************************
```



```shell
raytracer -input scene6_02_sphere_triangles.txt -output output6_02e.tga -size 200 200 -grid 10 10 10 -visualize_grid
```





##### scene6_03_sphere_plane.txt

```shell
raytracer -input scene6_03_sphere_plane.txt -output output6_03a.tga -size 200 200 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -1.000000 -1.000000 -1.000000  -> 1.000000 1.000000 1.000000
  num grid cells             NULL
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        80000
  total cells traversed      0
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      2.0
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_03_sphere_plane.txt -output output6_03b.tga -size 200 200 -grid 10 10 10 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -1.000000 -1.000000 -1.000000  -> 1.000000 1.000000 1.000000
  num grid cells             1000 (10x10x10)
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        45810
  total cells traversed      25204
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      1.1
  cells traversed per ray    0.6
```



```shell
raytracer -input scene6_03_sphere_plane.txt -output output6_03c.tga -size 200 200 -stats -shadows
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -1.000000 -1.000000 -1.000000  -> 1.000000 1.000000 1.000000
  num grid cells             NULL
  num non-shadow rays        40000
  num shadow rays            34800
  total intersections        147148
  total cells traversed      0
  rays per second            74800.0
  rays per pixel             1.9
  intersections per ray      2.0
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_03_sphere_plane.txt -output output6_03d.tga -size 200 200 -grid 10 10 10 -stats -shadows
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -1.000000 -1.000000 -1.000000  -> 1.000000 1.000000 1.000000
  num grid cells             1000 (10x10x10)
  num non-shadow rays        40000
  num shadow rays            34800
  total intersections        54640
  total cells traversed      39012
  rays per second            74800.0
  rays per pixel             1.9
  intersections per ray      0.7
  cells traversed per ray    0.5
********************************************
```



```shell
raytracer -input scene6_03_sphere_plane.txt -output output6_03e.tga -size 200 200 -grid 10 10 10 -visualize_grid
```





##### scene6_04_bunny_mesh_200.txt

```shell
raytracer -input scene6_04_bunny_mesh_200.txt -output output6_04a.tga -size 200 200 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:20
  num pixels                 40000 (200x200)
  scene bounds               -0.191055 0.067364 -0.114066  -> 0.121072 0.372852 0.116906
  num grid cells             NULL
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        8040000
  total cells traversed      0
  rays per second            2000.0
  rays per pixel             1.0
  intersections per ray      201.0
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_04_bunny_mesh_200.txt -output output6_04b.tga -size 200 200 -grid 10 10 7 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:01
  num pixels                 40000 (200x200)
  scene bounds               -0.191055 0.067364 -0.114066  -> 0.121072 0.372852 0.116906
  num grid cells             700 (10x10x7)
  num non-shadow rays        40000
  num shadow rays            0
  total intersections        242699
  total cells traversed      118788
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      6.1
  cells traversed per ray    3.0
********************************************
```



```shell
raytracer -input scene6_04_bunny_mesh_200.txt -output output6_04c.tga -size 200 200 -stats -shadows
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:50
  num pixels                 40000 (200x200)
  scene bounds               -0.191055 0.067364 -0.114066  -> 0.121072 0.372852 0.116906
  num grid cells             NULL
  num non-shadow rays        40000
  num shadow rays            66514
  total intersections        20813622
  total cells traversed      0
  rays per second            2130.3
  rays per pixel             2.7
  intersections per ray      195.4
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_04_bunny_mesh_200.txt -output output6_04d.tga -size 200 200 -grid 10 10 7 -stats -shadows
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:03
  num pixels                 40000 (200x200)
  scene bounds               -0.191055 0.067364 -0.114066  -> 0.121072 0.372852 0.116906
  num grid cells             700 (10x10x7)
  num non-shadow rays        40000
  num shadow rays            66514
  total intersections        827026
  total cells traversed      261917
  rays per second            35504.7
  rays per pixel             2.7
  intersections per ray      7.8
  cells traversed per ray    2.5
********************************************
```



```shell
raytracer -input scene6_04_bunny_mesh_200.txt -output output6_04e.tga -size 200 200 -grid 10 10 7 -visualize_grid
```





##### scene6_05_bunny_mesh_1k.txt

```shell
raytracer -input scene6_05_bunny_mesh_1k.txt  -output output6_05.tga -size 200 200 -grid 15 15 12 -stats -shadows
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:04
  num pixels                 40000 (200x200)
  scene bounds               -0.188888 0.066800 -0.123311  -> 0.122835 0.374981 0.118204
  num grid cells             2700 (15x15x12)
  num non-shadow rays        40000
  num shadow rays            66782
  total intersections        1097279
  total cells traversed      407801
  rays per second            26695.5
  rays per pixel             2.7
  intersections per ray      10.3
  cells traversed per ray    3.8
********************************************
```





##### scene6_06_bunny_mesh_5k.txt

````shell
raytracer -input scene6_06_bunny_mesh_5k.txt  -output output6_06.tga -size 200 200 -grid 20 20 15 -stats -shadows
````

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:05
  num pixels                 40000 (200x200)
  scene bounds               -0.189399 0.066625 -0.123574  -> 0.122050 0.374198 0.117532
  num grid cells             6000 (20x20x15)
  num non-shadow rays        40000
  num shadow rays            66832
  total intersections        1802799
  total cells traversed      513421
  rays per second            21366.4
  rays per pixel             2.7
  intersections per ray      16.9
  cells traversed per ray    4.8
********************************************
```





##### scene6_07_bunny_mesh_40k.txt

```shell
raytracer -input scene6_07_bunny_mesh_40k.txt -output output6_07.tga -size 200 200 -grid 40 40 33 -stats -shadows
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:08
  num pixels                 40000 (200x200)
  scene bounds               -0.189320 0.066660 -0.123542  -> 0.121998 0.374318 0.117505
  num grid cells             52800 (40x40x33)
  num non-shadow rays        40000
  num shadow rays            66834
  total intersections        2633551
  total cells traversed      1051043
  rays per second            13354.2
  rays per pixel             2.7
  intersections per ray      24.7
  cells traversed per ray    9.8
********************************************
```





##### scene6_11_mirrored_floor.txt

```shell
raytracer -input scene6_11_mirrored_floor.txt -size 200 200 -output output6_11a.tga -shadows -bounces 1 -weight 0.01 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:15
  num pixels                 40000 (200x200)
  scene bounds               -5.000000 0.000000 -5.000000  -> 5.000000 2.600000 5.000000
  num grid cells             NULL
  num non-shadow rays        66192
  num shadow rays            70070
  total intersections        5116487
  total cells traversed      0
  rays per second            9084.1
  rays per pixel             3.4
  intersections per ray      37.5
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_11_mirrored_floor.txt -size 200 200 -output output6_11b.tga -shadows -bounces 1 -weight 0.01 -grid 40 10 40 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:05
  num pixels                 40000 (200x200)
  scene bounds               -5.000000 0.000000 -5.000000  -> 5.000000 2.600000 5.000000
  num grid cells             16000 (40x10x40)
  num non-shadow rays        66192
  num shadow rays            70070
  total intersections        660244
  total cells traversed      2971308
  rays per second            27252.4
  rays per pixel             3.4
  intersections per ray      4.8
  cells traversed per ray    21.8
********************************************
```



```shell
raytracer -input scene6_11_mirrored_floor.txt -size 200 200 -output output6_11c.tga -grid 40 10 40 -visualize_grid
```





##### scene6_12_faceted_gem.txt

```shell
raytracer -input scene6_12_faceted_gem.txt -size 200 200 -output output6_12a.tga -shadows -shade_back -bounces 5 -weight 0.01 -stats
```

```
********************************************
RAY TRACING STATISTICS
  total time                 0:02:19
  num pixels                 40000 (200x200)
  scene bounds               -2.000000 -1.804689 -1.999999  -> 2.000000 0.998174 1.999999
  num grid cells             NULL
  num non-shadow rays        213094
  num shadow rays            379284
  total intersections        56647300
  total cells traversed      0
  rays per second            4261.7
  rays per pixel             14.8
  intersections per ray      95.6
  cells traversed per ray    0.0
********************************************
```



```shell
raytracer -input scene6_12_faceted_gem.txt -size 200 200 -output output6_12b.tga -shadows -shade_back -bounces 5 -weight 0.01 -grid 20 20 20 -stats
```

```shell
********************************************
RAY TRACING STATISTICS
  total time                 0:00:56
  num pixels                 40000 (200x200)
  scene bounds               -2.000000 -1.804689 -1.999999  -> 2.000000 0.998174 1.999999
  num grid cells             8000 (20x20x20)
  num non-shadow rays        213094
  num shadow rays            379284
  total intersections        8100243
  total cells traversed      5168908
  rays per second            10578.2
  rays per pixel             14.8
  intersections per ray      13.7
  cells traversed per ray    8.7
********************************************
```



```shell
raytracer -input scene6_12_faceted_gem.txt -size 200 200 -output output6_12c.tga -grid 20 20 20 -visualize_grid
```
