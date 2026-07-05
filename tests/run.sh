#!/usr/bin/env bash
# tests/run.sh — the behavioral test suite for actually.life (`l.c`).
#
# black-box on purpose: `l.c` is a single-file organism, so we do not poke its
# internals — we assert the observable CRITERIA OF LIFE. each test maps to one:
# heredity, variation, selection, metabolism, mortality, an eating mouth. if the
# organism stops meeting a criterion, a test goes red. no framework, no deps.
#
# usage:  tests/run.sh          # fast suite (build, determinism, life criteria, A/B)
#         tests/run.sh --asan   # + AddressSanitizer/UBSan pass (slower, strongest)
#         CC=gcc tests/run.sh   # pick a compiler
set -u
cd "$(dirname "$0")/.." || exit 2
ROOT=$(pwd)
CC=${CC:-cc}
L=/tmp/al_test.$$
SOLO_MD5=a490a453858581bc11a9d9624d1a95b3   # md5 of ./l 42 waste.log — the frozen solo trajectory

PASS=0; FAIL=0
ok(){ PASS=$((PASS+1)); printf '  \033[32m✓\033[0m %s\n' "$1"; }
no(){ FAIL=$((FAIL+1)); printf '  \033[31m✗ %s\033[0m\n' "$1"; [ -n "${2:-}" ] && printf '      %s\n' "$2" >&2; }
md5of(){ if command -v md5sum >/dev/null 2>&1; then md5sum "$1"|awk '{print $1}'; else md5 -q "$1"; fi; }

echo "actually.life — test suite  (root: $ROOT, cc: $CC)"
[ -s lifeis/world.txt ] || { echo "FATAL: lifeis/world.txt missing — no world to eat"; exit 2; }

# ── 1. it compiles, clean and warning-free ────────────────────────────────────
echo; echo "build"
BUILD=$("$CC" -std=c11 -O2 -Wall -Wextra -o "$L" l.c -lm 2>&1)
if [ $? -eq 0 ]; then
  [ -z "$BUILD" ] && ok "compiles clean (-Wall -Wextra, zero warnings)" \
                  || no "compiles but warns" "$BUILD"
else
  no "compile FAILED" "$BUILD"; echo; echo "cannot continue"; exit 1
fi

# ── 2. determinism: same seed → bit-identical life (the arrow of time is lawful)
echo; echo "determinism"
"$L" 42 >/dev/null 2>&1; A=$(md5of lifeis/waste.log)
"$L" 42 >/dev/null 2>&1; B=$(md5of lifeis/waste.log)
[ "$A" = "$B" ] && ok "solo is reproducible (two runs bit-identical)" \
                || no "solo is NON-deterministic" "$A vs $B"
[ "$A" = "$SOLO_MD5" ] && ok "solo trajectory matches the frozen hash ($SOLO_MD5)" \
                       || no "solo trajectory DRIFTED from the frozen hash" "got $A"

# ── 3. mortality: it MUST die — immortality is a garbage-collector fantasy ─────
echo; echo "mortality"
SOLO=$("$L" 42 2>&1)
echo "$SOLO" | grep -q 'died at tick' && ok "solo dies (metabolism runs out)" \
                                      || no "solo did not report death"
echo "$SOLO" | grep -q 'immortality hole' && no "solo hit the immortality cap (never died)" \
                                          || ok "solo never hit the immortality cap"

CH=$("$L" chorus 4 7 2>&1)
echo "$CH" | grep -q 'the colony fell silent' && ok "chorus dies (every cell mortal)" \
                                              || no "chorus never fell silent"
echo "$CH" | grep -q 'immortality hole' && no "a chorus cell became immortal" \
                                        || ok "no chorus cell became immortal"

# ── 4. heredity: children are born OF parents (the field is inherited) ─────────
echo; echo "heredity + evolution"
echo "$CH" | grep -q 'born of a parent'   && ok "heredity fires (a cell is born of a parent)" \
                                          || no "no inherited births observed"

# ── 5. Ⓑ variation by recombination: two gametes → one creole child ───────────
echo "$CH" | grep -q 'born of TWO parents' && ok "sexual recombination fires (born of TWO parents)" \
                                           || no "no two-parent births observed"

# ── 6. selection has a substrate: distinct lineages coexist and compete ───────
LINES=$(echo "$CH" | grep -c 'born of' )
[ "$LINES" -ge 2 ] && ok "reproduction is repeated ($LINES births — a population, not a pair)" \
                   || no "too few births to call it a population" "$LINES"

# ── 7. A/B toggles: each arm of evolution can be lifted, life persists ─────────
echo; echo "A/B controls (each knob off → still a living, mortal organism)"
NS=$(NL_NOSEX=1 "$L" chorus 4 7 2>&1)
{ echo "$NS" | grep -q 'the colony fell silent' && ! echo "$NS" | grep -q 'born of TWO parents'; } \
  && ok "NL_NOSEX=1 → asexual only (0 recombinations), still mortal" \
  || no "NL_NOSEX did not cleanly disable sex"
NF=$(NL_NOFIELD=1 "$L" 42 2>&1)
echo "$NF" | grep -q 'died at tick' && ok "NL_NOFIELD=1 → field lifted, organism still lives & dies" \
                                    || no "NL_NOFIELD broke the organism"
NM=$(NL_NOMUT=1 "$L" chorus 3 7 2>&1)
echo "$NM" | grep -q 'the colony fell silent' && ok "NL_NOMUT=1 → no mutation, colony still mortal" \
                                              || no "NL_NOMUT broke the chorus"

# ── 8. the mouth eats ANYTHING: unknown words route by resemblance, no crash ──
echo; echo "the mouth (eats anything you give it)"
MOUTH=$(printf 'inferno dragons quantum spaghetti obliteration\nlove fire death rain\n\n' \
        | "$L" --mouth 42 2>&1)
MRC=$?
[ "$MRC" -eq 0 ] && ok "mouth exits cleanly on arbitrary input (rc=0)" \
                 || no "mouth exited non-zero on arbitrary input" "rc=$MRC"
echo "$MOUTH" | grep -q 'a mouth opens' && ok "mouth opens and reads a plate of unknown words" \
                                        || no "mouth never opened"
echo "$MOUTH" | grep -q 'ate:' && ok "mouth digests input (no word spat back uneaten)" \
                               || no "mouth reported eating nothing"

# ── 9. AddressSanitizer / UBSan (opt-in: the strongest correctness pass) ───────
if [ "${1:-}" = "--asan" ]; then
  echo; echo "sanitizers (ASan + UBSan)"
  SB=$("$CC" -std=c11 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -o "$L.asan" l.c -lm 2>&1)
  if [ $? -eq 0 ]; then
    SLOG=$(ASAN_OPTIONS=detect_leaks=0 "$L.asan" 42 2>&1; ASAN_OPTIONS=detect_leaks=0 "$L.asan" chorus 4 7 2>&1)
    HITS=$(echo "$SLOG" | grep -cE 'runtime error|ERROR: AddressSanitizer|heap-buffer|stack-buffer|use-after')
    [ "$HITS" -eq 0 ] && ok "solo + chorus run clean under ASan/UBSan (0 hits)" \
                      || no "sanitizer found $HITS issue(s)" "$(echo "$SLOG" | grep -E 'runtime error|ERROR' | head -3)"
    rm -f "$L.asan"
  else
    no "sanitizer build failed" "$SB"
  fi
fi

# ── tally ─────────────────────────────────────────────────────────────────────
rm -f "$L"
echo; echo "──────────────────────────────────────────"
printf 'passed %d   failed %d\n' "$PASS" "$FAIL"
[ "$FAIL" -eq 0 ] && { echo "да будет так. — the organism meets every criterion under test."; exit 0; } \
                  || { echo "a criterion of life regressed. see ✗ above."; exit 1; }
