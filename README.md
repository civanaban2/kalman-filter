# Bearing-Only Target Tracking

A C implementation of a two-dimensional target-tracking workflow using bearing
measurements from three fixed sensors. Gauss-Newton initialization estimates the
starting position, then an Extended Kalman Filter (EKF) tracks position and
velocity. MATLAB scripts visualize logged estimates and position error.

Developed for the **MTH 407 - Fundamentals of Automatic Target Tracking** course
by **Civan Rumet Arı** and **Yaren Sakarya**.

## Components

```text
simulation -> data.txt -> kalman -> log.txt -> MATLAB plots
```

- **Simulation:** generate target motion and noisy sensor bearings, then write
  measurements with timestamp-based delays.
- **Filter:** follow newly appended measurements, initialize from the first
  three records, and estimate the state `[x, y, vx, vy]`.
- **Analysis:** display the estimated trajectory and position error against the
  configured ground-truth motion.
- **Report:** a Turkish LaTeX course report and example figures in `report/`.

This is an educational, file-based tracking demo, not a hard real-time system
or an integration with physical radar hardware.

## Build

Requirements: a C compiler, Make, and POSIX file/time functions. macOS and Linux
are the intended environments. MATLAB and LaTeX are optional and not needed
to build the two C programs.

```sh
git clone https://github.com/civanaban2/kalman-filter.git
cd kalman-filter
make
```

Outputs:

- `bin/simulation`
- `bin/kalman`

Use `make CC=clang` to select Clang, or the default `gcc`. The filter links against `libm`. The current simulation link command does not
include `-lm`, which may cause unresolved math symbols on Linux. Object files
are stored in `.obj/`; the current build does not generate header dependencies.

## Run the live demonstration

Use two terminals, both in the repository root. Stop any previous runs first.

**Terminal 1:** create an empty measurement file and start the filter:

```sh
: > data.txt
./bin/kalman
```

**Terminal 2:** start the measurement producer:

```sh
./bin/simulation
```

The filter initially seeks to the end of `data.txt`, so it must be running
before the simulation begins emitting measurements. The producer overwrites
`data.txt` and waits three seconds before writing its paced stream. The filter
overwrites `log.txt` at startup.

The simulator stops after writing its generated measurements. The filter keeps
polling and must be stopped with **Ctrl+C**. Back up existing logs before a new
run if you want to keep them. The filter is not an offline replay command for
an already populated measurement file.

On macOS, `make start` opens the two programs in Terminal.app through AppleScript.
It is a platform-specific convenience target; the manual commands above avoid
that dependency.

## Simulation settings

The source defines these fixed settings:

| Setting | Value |
| --- | --- |
| Sensor positions | `(0, 0)`, `(500, 0)`, `(250, 400)` |
| Initial target position | `(100, 200)` |
| Target velocity | `(5, 3)` |
| Simulation integration step | `0.0001` seconds |
| Sensor bearing noise standard deviations | 0.5, 1.0, and 1.5 degrees |
| Simulated measurement drop probability | 10% per generated record |
| Candidate measurement count | 100 before randomized drops |

The simulator seeds its random generator from the current time. Each run can
therefore produce different measurements and retained-record counts.

## Filter outline

The first three measurements initialize position through Gauss-Newton
least-squares iterations with wrapped angular residuals. Initial velocity is
set to zero. Each subsequent record drives a constant-velocity EKF prediction
using the timestamp difference, followed by a nonlinear bearing update.

Measurement files store bearings in degrees; the filter uses radians in its
measurement equation. The current measurement variance is formed directly from
the sensor-specific degree values, without converting that variance to radians.
The configured process covariance is `Q = 10 I`;
initial position variances are 10 and velocity variances are 100.

The current covariance update uses `(I - K H) P`. It does not implement a
Joseph-form update or explicit covariance stabilization.

## File formats

`data.txt` records contain a timestamp, sensor ID (1 through 3), and bearing in
degrees. The producer uses a fixed-width representation:

```text
1.04    1    62.5321
```

`log.txt` contains five whitespace-separated numeric columns:

```text
timestamp x_estimate y_estimate vx_estimate vy_estimate
```

That line describes the columns; the actual log contains numeric rows without
a header. The filter prints headings separately to the terminal.

## MATLAB analysis

After generating a nonempty `log.txt`, open MATLAB and run:

```matlab
cd src/analysis
plot_master
```

`plot_master` repeatedly reads new estimates and displays them alongside
`x = 100 + 5*t`, `y = 200 + 3*t`. It runs until interrupted.

For the position-error chart, run `plot_rmse` from the same directory in a
separate session or after stopping the live plot. Despite its historical name,
that script plots the **instantaneous Euclidean position error**, not an RMS
aggregate across repeated runs.

`make plot` and `make plot_rmse` currently use the hard-coded macOS installation
path `/Applications/MATLAB_R2025a.app/bin/matlab`. Use MATLAB directly if your
installation differs. Script paths assume the working directory is
`src/analysis`.

## Build targets

| Command | Action |
| --- | --- |
| `make` | Build both C programs |
| `make simulation` / `make kalman` | Build an individual program |
| `make start` | Launch the demo through Terminal.app on macOS |
| `make plot` / `make plot_rmse` | Run analysis using the configured MATLAB path |
| `make report` | Attempt LaTeX report generation |
| `make clean` | Remove objects, report auxiliary files, and **data.txt/log.txt** |
| `make fclean` | Also remove executables and generated report PDFs |
| `make re` | Clean generated files and rebuild |
| `make help` | Display target descriptions |

There is no `make analysis` target or C analysis executable.

## Report

`report/main.tex` contains the course report and references
`plot_small.png` and `rmse_small.png`. These figures are saved historical
examples, not results regenerated during this documentation update.

The report requires a TeX installation with its referenced packages and Turkish
language support. The Makefile's report target suppresses compiler output and
can finish after warnings; check that `report/main.pdf` was actually generated.
For visible diagnostics, run `pdflatex -interaction=nonstopmode -halt-on-error
main.tex` from `report/`.

## Scope and limitations

The following issues are observations about the existing code; this documentation
update does not change the implementation.

- The simulator calls `free()` on fixed-size arrays embedded in a stack object,
  which can cause a crash at shutdown.
- The Gauss-Newton normal-equation accumulators are not reset between iterations;
  the EKF covariance multiplication and angular-noise units also need review.

- The file follower assumes small, fixed-width, timestamp-paced records. It
  is not a robust general-purpose parser: missing files, partial writes,
  malformed records, or multiple records arriving between polls are not
  handled reliably.
- Initialization uses the first three records without enforcing distinct
  sensors or checking geometric observability.
- Sensor locations, motion, noise, and filter settings are compiled constants,
  not command-line options.
- The simulation's bearing-alignment logic and fixed noise tuning are simplified
  models; tracking accuracy is not guaranteed across arbitrary scenarios.
- MATLAB and LaTeX output must be regenerated to evaluate a changed algorithm;
  saved report images are not a numerical regression test.

## License

MIT. See [LICENSE](LICENSE).
