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
  total intersections        13076
  total cells traversed      25204
  rays per second            40000.0
  rays per pixel             1.0
  intersections per ray      0.3
  cells traversed per ray    0.6
********************************************
```

avoid duplicate intersection

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
  num shadow rays            12270
  total intersections        63038
  total cells traversed      304942
  rays per second            52270.0
  rays per pixel             1.3
  intersections per ray      1.2
  cells traversed per ray    5.8
********************************************
```



```shell
raytracer -input scene6_02_sphere_triangles.txt -output output6_02e.tga -size 200 200 -grid 10 10 10 -visualize_grid
```

