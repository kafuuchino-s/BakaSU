# MIX4 Linux 5.4 maintenance

`mix4-5.4` carries the existing MIX4 BakaSU port, based on upstream
[`08966cd0574d898a678a9bbe9a3ea0270e71ea6e`](https://github.com/Baka-SU/BakaSU/commit/08966cd0574d898a678a9bbe9a3ea0270e71ea6e)
from `sync-susfs`. Upstream history is retained. The first downstream commit
applies the existing 15-file 5.4 glue; the following commit pins its build identity.

```sh
git clone --branch mix4-5.4 https://github.com/kafuuchino-s/BakaSU.git
git -C BakaSU remote add upstream https://github.com/Baka-SU/BakaSU.git
```

Use this repository as the `BakaSU` submodule of
[`kernel_xiaomi_odin`](https://github.com/kafuuchino-s/kernel_xiaomi_odin).
Keep `drivers/kernelsu` pointing to `BakaSU/kernel`. The parent kernel owns the
Linux 5.4 hook sites, SELinux changes, and applied SUSFS sources; this fork owns
the BakaSU compatibility code. The parent kernel pins SUSFS 2.3.0 by its
submodule commit. The MIX4 configuration uses ARM64,
`CONFIG_KSU=y`, `CONFIG_KSU_SUSFS=y`, and
`CONFIG_KSU_FULL_NAME_FORMAT="%TAG_NAME%-%COMMIT_SHA%@%REPO_NAME%"`.

## Build identity

[`kernel/mix4-build-identity.mk`](kernel/mix4-build-identity.mk) is the authoritative
released build identity pin. Its `KSU_UPSTREAM_COMMIT` records the upstream
revision used for that identity; the source upstream revision is recorded below
for the candidate branch. Kbuild uses this file instead of Git commit count, tags, HEAD,
branch, or dirty state. It does not fetch history during a build. The existing
`CONFIG_KSU_FULL_NAME_FORMAT` placeholders still work.

The effective source was the detached upstream commit with the 5.4 glue applied.
Its configuration and all 34 saved BakaSU object compile commands in the existing
MIX4 build agree on:

```text
KSU_VERSION=35231
KSU_VERSION_FULL="v4.2.0-rc3-08966cd0-dirty@BakaSU"
```

`35231 = 30000 + 4531 + 700`; `v4.2.0-rc3` was Git's nearest tag. The literal
`-dirty` records the original patched source state, even when this fork is clean.
The branch placeholder was empty because the original submodule was detached.
New fork commits therefore retain the compiled identity needed to reproduce the
existing kernel. The fork commit identifies source revisions independently.

## Updating the port

Choose and record an exact upstream commit, merge it into `mix4-5.4`, and resolve
the Linux 5.4 interface differences in the maintained branch. For the 5.4.302
migration, use `mix4-5.4-upstream-24.0` until the parent integration is accepted.
Keep `kernel/mix4-build-identity.mk` fixed for this candidate, including
`KSU_UPSTREAM_COMMIT=08966cd0574d898a678a9bbe9a3ea0270e71ea6e`; merging upstream
does not authorize deriving a new identity from history or dirty state. Any later
identity change must explicitly review the numeric version, tag, commit label,
and branch placeholder together. Keep format selection in the parent kernel
configuration.

Run an incremental `drivers/kernelsu/` build with the integration owner's
configured Linux 5.4 output directory and clang 11 first, reusing the actual
`build.sh` make arguments and prefix maps. Prepare a new output directory through
the parent build entry before requesting this target. The integration owner
then runs the complete kernel build, ABI and duplicate-export gates, and compares
the resulting Image with the existing release. Device validation and release tags
belong to the parent release workflow.

## Linux 5.4.302 candidate

`mix4-5.4-upstream-24.0` merges upstream `sync-susfs` at
[`10cada74698ccf7eed04d0e38bc7d624e5f54395`](https://github.com/Baka-SU/BakaSU/commit/10cada74698ccf7eed04d0e38bc7d624e5f54395),
with the existing MIX4 port as its first parent. The merge retains all ten new
upstream commits and preserves the released build identity above.

The only merge conflict was `ksu_sys_read()` in
`kernel/runtime/ksud_integration.c`. It keeps the MIX4 declaration
`void ksu_handle_sys_read(unsigned int fd)` and calls that hook once, then uses
upstream's native/compat syscall dispatch. The unused buffer/count locals are
removed. The parent kernel must continue to provide `linux/odin_ksu_hooks.h` and
its matching hook sites.

Upstream `d7260ca1` checks `context_add_hash()` before both backup and live
`sidtab_context_to_sid()` operations. The MIX4 port already implements those
checks in `kernel/feature/selinux_hide.c`, guarded by
`KSU_COMPAT_HAS_CONTEXT_ADD_HASH`, detected in `kernel/tools/kernel_compat.mk`.
The merge retains that implementation. Upstream also removes SELinux helpers
for kernels >= 6.6; the Linux 5.4 implementation remains selected.

Upstream `522081c5` adds ARM64 compat register handling and generated syscall
numbers. `kernel/tools/code_generation/syscall_no/syscall_no.mk` registers a
host tool with `hostprogs-y` as well as `hostprogs`, and for Linux 5.4 with
`CONFIG_COMPAT=y` reads `arch/arm64/include/asm/unistd32.h`. The output header is
`out/drivers/kernelsu/compat/syscall_no.h`; it is a build output, not a source
file to copy or commit. Keep the upstream generator and its object dependencies
when integrating the candidate.

The BakaSU owner has checked merge ancestry, downstream differences, conflict
resolution, and preservation of `mix4-build-identity.mk`. Kernel compilation,
CFI linking, ABI validation, and runtime behavior must be checked by the parent
kernel integration; the source review alone does not establish those results.
