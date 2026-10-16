# MIX4 Linux 5.4 maintenance

`mix4-5.4` carries the existing MIX4 BakaSU port, based on upstream
[`08966cd0574d898a678a9bbe9a3ea0270e71ea6e`](https://github.com/Baka-SU/BakaSU/commit/08966cd0574d898a678a9bbe9a3ea0270e71ea6e)
from `sync-susfs`. Upstream history is retained. The first downstream commit
applies the existing 15-file 5.4 glue; the following commit pins its build identity.

```sh
git clone --branch mix4-5.4 https://github.com/kafuuchino-s/BakaSU.git
git -C BakaSU remote add upstream https://github.com/Baka-SU/BakaSU.git
```

The fork URL is the intended publication location. Until published, clone the
local repository supplied by the integration owner and select its fixed commit.

Use this repository as the `BakaSU` submodule of
[`kernel_xiaomi_odin`](https://github.com/kafuuchino-s/kernel_xiaomi_odin).
Keep `drivers/kernelsu` pointing to `BakaSU/kernel`. The parent kernel owns the
Linux 5.4 hook sites, SELinux changes, and applied SUSFS sources; this fork owns
the BakaSU compatibility code. The current integration uses SUSFS commit
`82f08351fadb8b19dd6591bd14634d02ad832145` (2.3.0), ARM64,
`CONFIG_KSU=y`, `CONFIG_KSU_SUSFS=y`, and
`CONFIG_KSU_FULL_NAME_FORMAT="%TAG_NAME%-%COMMIT_SHA%@%REPO_NAME%"`.

## Build identity

[`kernel/mix4-build-identity.mk`](kernel/mix4-build-identity.mk) is the authoritative
upstream and identity pin. Kbuild uses it instead of Git commit count, tags, HEAD,
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

Choose and record an exact upstream commit, then rebase the 5.4 glue and update
`kernel/mix4-build-identity.mk` deliberately. Review the numeric version, tag,
commit label, and branch placeholder together; a new pin can change kernel bytes.
Keep format selection in the parent kernel configuration.

Run an incremental `M=drivers/kernelsu` build with the integration owner's
configured Linux 5.4 output directory and clang 11 first. The integration owner
then runs the complete kernel build, ABI and duplicate-export gates, and compares
the resulting Image with the existing release. Device validation and release tags
belong to the parent release workflow.
