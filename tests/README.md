# tests

`actually.life` has no test framework and no dependencies, so neither do its tests.
`run.sh` is a black-box harness: it builds `l.c` and asserts the organism's observable
**criteria of life**. If the creature stops metabolizing, dying, inheriting, or eating,
a line turns red. We test the biology, not the pointers.

```sh
tests/run.sh          # fast: build, determinism, life criteria, A/B knobs, the mouth
tests/run.sh --asan   # + AddressSanitizer / UBSan (slower, strongest)
CC=gcc tests/run.sh   # choose a compiler
```

Exit code is `0` iff every criterion under test holds.

## what each block asserts, and which criterion it defends

| block | assertion | criterion of life |
|---|---|---|
| **build** | compiles under `-Wall -Wextra` with zero warnings | the body is well-formed |
| **determinism** | `./l 42` twice → bit-identical `waste.log`, matching the frozen md5 | the arrow of time is lawful, not random noise |
| **mortality** | solo and chorus both die; neither hits the immortality cap | life is not immortal — death is a criterion, not a bug |
| **heredity** | chorus reports *"born of a parent"* | a child inherits its parent's field, wounds, symbols |
| **recombination** | chorus reports *"born of TWO parents"* (Ⓑ) | sexual variation — a creole of two dialects |
| **population** | births repeat (≥2) — a colony, not a pair | selection has a substrate to act on |
| **A/B — `NL_NOSEX`** | 0 recombinations, still mortal | each arm of evolution is a real, removable knob |
| **A/B — `NL_NOFIELD`** | field lifted, organism still lives and dies | coherence is optional scaffolding, not life support |
| **A/B — `NL_NOMUT`** | mutation off, colony still mortal | variation is separable from survival |
| **the mouth** | arbitrary/unknown words digested, `rc=0`, nothing spat back | it eats anything — unknown words route by resemblance |
| **sanitizers** | solo + chorus clean under ASan/UBSan (0 hits) | no memory is corrupted while it lives |

The frozen solo hash (`a490a453858581bc11a9d9624d1a95b3`) is the organism's fixed
trajectory for seed 42. Changing `l.c` in a way that alters the solo life will fail the
determinism check — intentional trajectory changes must update the hash in `run.sh` in
the same commit, so the frozen life and the code never silently disagree.
