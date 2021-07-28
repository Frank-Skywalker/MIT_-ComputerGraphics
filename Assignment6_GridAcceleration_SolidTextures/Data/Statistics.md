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



```shell
raytracer -input scene6_01_sphere.txt -output output6_01c.tga -size 200 200 -grid 10 10 10 -visualize_grid
```



