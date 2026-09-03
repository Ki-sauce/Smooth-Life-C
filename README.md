# Smooth Life in C(PU)

A small C implementation of SmoothLife.

The implementation is based on Stephan Rafler's paper:

**Generalization of Conway's "Game of Life" to a continuous domain - SmoothLife**
Stephan Rafler, 2011.

[Paper on arXiv](https://arxiv.org/abs/1111.1567)

## Build

Requires:

* GCC
* GNU Make
* FFmpeg

```bash
make
```

## Run

```bash
make run
```

This generates the simulation frames as PPM files:

```text
output_000.ppm
output_001.ppm
output_002.ppm
...
```

## Video

```bash
make video
```

This runs the simulation and creates:

```text
animation.mp4
```

at 30 FPS using FFmpeg.

To start over:

```bash
make rebuild-run-video
```

To remove generated files:

```bash
make clean
```

## Simulation

The grid contains floating-point values between `0.0` and `1.0`.

Each cell is evaluated using an inner circular neighborhood and an outer ring. The resulting neighborhood values are passed through the smooth transition functions from the SmoothLife model.

The grid wraps around at the edges.

The initial state is a randomly generated set of `10 × 10` blocks near the center.

## Parameters

```c
H  = 300
W  = 300
F  = 300

ra = 21
ri = 7

b1 = 0.278
b2 = 0.365
d1 = 0.267
d2 = 0.445
an = 0.028
am = 0.147
```

Fuck around and find out type shit.

## Performance

It runs on the CPU. Need i say more?

There is no God.

End my Suffering.

