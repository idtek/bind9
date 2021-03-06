<!--
 - Copyright (C) Internet Systems Consortium, Inc. ("ISC")
 -
 - This Source Code Form is subject to the terms of the Mozilla Public
 - License, v. 2.0. If a copy of the MPL was not distributed with this
 - file, You can obtain one at http://mozilla.org/MPL/2.0/.
 -
 - See the COPYRIGHT file distributed with this work for additional
 - information regarding copyright ownership.
-->
## Supported platforms

In general, this version of BIND will build and run on any POSIX-compliant
system with a C99-compliant C compiler, BSD-style sockets with RFC-compliant
IPv6 support, POSIX-compliant threads, and the OpenSSL cryptography library.
Atomic operations support from the compiler is needed, either in the form of
builtin operations, C11 atomics or the Interlocked family of functions on
Windows.

ISC regularly tests BIND on many operating systems and architectures, but
lacks the resources to test all of them. Consequently, ISC is only able to
offer support on a "best effort" basis for some.

### Regularly tested platforms

As of May 2018, BIND 9.13 is tested on the following systems:

* Debian 8, 9
* Ubuntu 16.04, 18.04
* Fedora 27, 28
* Red Hat/CentOS 6, 7
* FreeBSD 10.x, 11.x
* OpenBSD 6.3

The amd64, i386, armhf and arm64 CPU architectures are all fully supported.

### Best effort

The following are platforms on which BIND is known to build and run,
but on which it is not routinely tested. ISC makes every effort to fix bugs
on these platforms, but may be unable to do so quickly due to lack of
hardware, less familiarity on the part of engineering staff, and other
constraints.

* Windows 10 / x64
* Windows Server 2012 R2, 2016 / x64
* macOS 10.12+
* Solaris 10
* FreeBSD 12+
* OpenBSD 6.2
* NetBSD
* Older or less popular Linux distributions still supported by their vendors, such as:
    * Ubuntu 14.04, 18.10+
    * Gentoo
    * ArchLinux
    * Alpine Linux
* OpenWRT/LEDE 17.0
* Other CPU architectures (mips, mipsel, sparc, ...)

## Unsupported platforms

These are platforms on which BIND is known *not* to build or run:

* Platforms without at least OpenSSL 1.0.2
* Windows 10 / x86
* Windows Server 2012 and older
* Platforms that don't support IPv6 Advanced Socket API (RFC 3542)
* Platforms that don't support atomic operations (via compiler or library)
* Linux without NPTL (Native POSIX Thread Library)

## Platform quirks

### ARM

If the compilation ends with following error:

```
Error: selected processor does not support `yield' in ARM mode
```

You will need to set `-march` compiler option to `native`, so the compiler
recognizes `yield` assembler instruction.  The proper way to set `-march=native`
would be to put it into `CFLAGS`, e.g. run `./configure` like this:
`CFLAGS="-march=native -Os -g" ./configure` plus your usual options.

If that doesn't work, you can enforce the minimum CPU and FPU (taken from Debian
armhf documentation):

* The lowest worthwhile CPU implementation is Armv7-A, therefore the recommended
  build option is `-march=armv7-a`.

* FPU should be set at VFPv3-D16 as they represent the miminum specification of
  the processors to support here, therefore the recommended build option is
  `-mfpu=vfpv3-d16`.

The configure command should look like this:

```
CFLAGS="-march=armv7-a -mfpu=vfpv3-d16 -Os -g" ./configure
```
