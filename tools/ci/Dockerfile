# Build image for CI: the toolchain plus the two program ROM halves, which are
# not in the repo. The image holds ROM data, so it must stay a private package.
#
#   docker build -f tools/ci/Dockerfile --build-context roms=/path/to/roms \
#       -t ghcr.io/masonc15/tgm2p-build:main tools/ci
#   docker push ghcr.io/masonc15/tgm2p-build:main
#
# Every download is pinned by hash to the copies the local build uses on nuada.
FROM ubuntu:24.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install -y --no-install-recommends \
        binutils-sh-elf ca-certificates curl git make python3 python3-venv \
    && rm -rf /var/lib/apt/lists/*
RUN python3 -m venv /opt/venv && /opt/venv/bin/pip install --no-cache-dir capstone==5.0.9

WORKDIR /opt/compilers
# rof2elf_fillff.py fills alignment gaps with 0xFF, as the EPROMs have them.
RUN curl -sSLo shc.tar.gz https://github.com/decompme/compilers/releases/download/compilers/shc-v5.0r32.tar.gz \
    && curl -sSLo wibo https://github.com/decompals/wibo/releases/download/1.2.0/wibo-x86_64 \
    && curl -sSLo rof2elf.py https://gist.githubusercontent.com/Mc-muffin/c2d3f30e50c5c5749f994973441c503a/raw/18fbf1a19bd720d02774ed634b00b0508eaf474c/rof2elf.py \
    && curl -sSLo /usr/local/bin/objdiff-cli https://github.com/encounter/objdiff/releases/download/v3.8.1/objdiff-cli-linux-x86_64 \
    && sed 's/add = b"\\x00"/add = b"\\xff"/' rof2elf.py > rof2elf_fillff.py \
    && printf '%s\n' \
        "21f26227e48a2c9325f0aaf954e569b29db8db1dd1174aff32623017d713fdf5  shc.tar.gz" \
        "13f86a2d618f0dbe67179d349625345eabf9b46450295cb4c904e49f6aff85af  wibo" \
        "f5a52b69b619e206172da4dc74182ecc97eeac5dcfcf5466c537af5ea6310821  rof2elf.py" \
        "263d95b39e16caa6da3513eaeae0eea1c095094ffe7513851b2e93db517a923f  rof2elf_fillff.py" \
        "c8290281e82114bcc1a06ff73061110d3902a177822e750337de2537188e358f  /usr/local/bin/objdiff-cli" \
        | sha256sum -c - \
    && mkdir shc-v5.0r32 && tar -xzf shc.tar.gz -C shc-v5.0r32 && rm shc.tar.gz \
    && chmod +x wibo /usr/local/bin/objdiff-cli

COPY --from=roms 2b.u21 1b.u22 /orig/

ENV SHC_COMPILERS=/opt/compilers PY=/opt/venv/bin/python OBJDIFF=/usr/local/bin/objdiff-cli
LABEL org.opencontainers.image.source=https://github.com/masonc15/tgm2p-decomp
