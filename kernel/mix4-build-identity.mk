# Authoritative upstream and build-identity pin for the MIX4 Linux 5.4 port.
# Upstream: https://github.com/Baka-SU/BakaSU.git (sync-susfs)
KSU_UPSTREAM_COMMIT := 08966cd0574d898a678a9bbe9a3ea0270e71ea6e

# Observed in the effective exp-C/BakaSU tree and its 34 object .cmd files.
# 30000 + 4531 reachable upstream commits + 700 = 35231.
KSU_LOCAL_VERSION := 4531
KSU_VERSION := $(shell expr 30000 + $(KSU_LOCAL_VERSION) + 700)
KSU_TAG_NAME := v4.2.0-rc3
# The released tree had the 5.4 glue applied but uncommitted: retain "-dirty".
KSU_COMMIT_SHA := 08966cd0-dirty
# The released submodule was detached; this placeholder was empty.
KSU_BRANCH_NAME :=
