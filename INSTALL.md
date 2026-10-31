# Instructions

These instructions explain how to set up the tools required to build the
projects in this repository, which assemble and compile the source files into
the original images.

Everything the build needs is in the repository, apart from a few packages
from your system. The instructions are for Linux; macOS should work the same
way with the equivalent packages.

## Prerequisites

* `arm-none-eabi` binutils (`as`, `ld`, `objcopy`, `objdump`)
* a host `gcc`, `g++`, `make` and `cpp`
* libpng and its headers, for `gbagfx`
* Python 3 with `pyelftools` and Pillow, for the build's helper scripts

On Debian or Ubuntu:

```bash
sudo apt install binutils-arm-none-eabi build-essential git libpng-dev python3 python3-pyelftools python3-pil
```

`arm-none-eabi-gcc` is not needed: C is compiled with agbcc, built below,
and preprocessed with the host `cpp` (agbcc is driven with `-nostdinc
-undef`).

## Installation

Clone the repository, fetch agbcc's source and build the shared tools:

```bash
# tools/agbcc-src (pret/agbcc) is already included in this archive
make tools
```

`make tools` builds agbcc (`agbcc`, `old_agbcc` and `agbcc_arm`, with their
libgcc and libc) from the `tools/agbcc-src` submodule and installs it into
`tools/agbcc`, then builds pret's `gbagfx` and `preproc`, whose sources are
copied into `tools/gbagfx` and `tools/preproc` (pokeemerald commit
4aafe5904, plus `characters.h` beside preproc). Every project uses these
through [config.mk](config.mk); a project reports `run make tools` if they
are missing.

## Build the images

From this directory:

```bash
make compare    # build every project and check every image's sha1
```

Or from a project directory, for example `coupon/`:

```bash
make            # build coupon.mb
make compare    # build it and check its sha1
make clean
```

If an image matches, `make compare` prints `<image>: OK`.

The multiboot projects with several versions (`client/`, `colbtl/`) build
and check every version by default; a version name builds just that one:

```bash
make colbtl_f                   # build and check one version
make VERSION=colbtl_g compare   # any single-version target
```

## Building guidance

### Parallel builds

Building with several jobs is much faster:

```bash
make -j$(nproc) compare
```

### Another agbcc, gbagfx or preproc

`config.mk` points at the tools in `tools/`. To try another build of any of
them, override `AGBCC_DIR`, `GBAGFX` or `PREPROC` on the command line:

```bash
make AGBCC_DIR=/path/to/pokeemerald/tools/agbcc compare
```

### Building with the original images

No project needs its original image to build.
