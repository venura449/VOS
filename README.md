# VelocityOS

VelocityOS is an experimental operating system project aimed at fast boot, strong isolation, and a developer-first userspace.

## Architecture Direction

- Kernel model: microkernel. Keep scheduling, IPC, and memory management in the kernel and move drivers, filesystems, and network services to userspace.
- Boot path: minimal first, then measured boot later. This scaffold currently boots with GRUB Multiboot so the project can move immediately into kernel work.
- Security model: capability-based services instead of a broad superuser model.
- Userspace direction: C for early kernel bring-up, Rust for most userspace services once the syscall and IPC surface exists.

## Current State

This repository now includes phase 1 of the roadmap:

- a bootable 32-bit kernel entry point
- a linker script
- a GRUB boot configuration
- VGA text-mode output
- Make targets for build, run, and GDB attach

On boot, the kernel clears the VGA buffer and prints a simple VelocityOS status screen in QEMU.

## Repository Layout

```text
.
|-- Makefile
|-- grub/
|   `-- grub.cfg
|-- linker.ld
|-- scripts/
|   `-- gdb.init
`-- src/
	|-- boot.asm
	`-- kernel.c
```

## Toolchain

Recommended host packages on Linux:

```bash
sudo apt install build-essential nasm grub-pc-bin grub-common xorriso qemu-system-x86
```

Recommended compiler toolchain:

Use a dedicated `i686-elf-gcc` cross-compiler and keep that target stable as the kernel grows. The Makefile expects `i686-elf-gcc` by default.

If you are developing on Windows, the simplest path is to build from WSL2 with the same toolchain names used above.

## Build

```bash
make
```

Artifacts:

- `build/velocityos.elf`
- `build/velocityos.iso`

## Run

```bash
make run
```

This launches QEMU and shows VGA output in the emulator window.

## Debug

Terminal 1:

```bash
make debug
```

Terminal 2:

```bash
gdb -x scripts/gdb.init
```

## Roadmap

1. Add GDT setup and exception-safe IDT stubs.
2. Add a basic physical memory manager.
3. Introduce timer interrupts and a preemptive scheduler.
4. Define an IPC primitive suitable for a microkernel message-passing model.
5. Split drivers and filesystems into userspace services.
6. Add a capability model, audit logging, and transactional package tooling.

## Notes

- This is intentionally a minimal bring-up scaffold, not yet a real microkernel.
- The boot flow is Multiboot via GRUB for speed of iteration; UEFI measured boot can come later once the kernel core is stable.
- The kernel currently writes to VGA, not serial.
