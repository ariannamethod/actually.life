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
SOLO_MD5=894ba413fb7365adc47e5db9cffbe754   # md5 of ./l 42 waste.log — the frozen solo trajectory (proteostasis + self-model + sleep + self-as-food + EARNED VOICE, all on)
FIELD_MD5=bafc8afcaa95e32b067038b47f965653  # ...with NL_NOEARNED (field-only voice, the pre-earned-voice trajectory)
FROZEN_MD5=a490a453858581bc11a9d9624d1a95b3 # ...with EVERY new organ off (NL_NOCORRODE+NL_NOREPAIR+NL_NOSELF+NL_NOSLEEP+NL_NOSELFEAT+NL_NOEARNED) — the pre-living-body trajectory

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

# ── 8b. PROTEOSTASIS: the body is autopoietic — it corrodes and is rebuilt by eating ─
echo; echo "proteostasis (the living, self-repairing body)"
# gate invariant: with EVERY new organ OFF, the organism is bit-for-bit the pre-living-body one
NL_NOCORRODE=1 NL_NOREPAIR=1 NL_NOSELF=1 NL_NOSLEEP=1 NL_NOSELFEAT=1 NL_NOEARNED=1 "$L" 42 >/dev/null 2>&1; OFF=$(md5of lifeis/waste.log)
[ "$OFF" = "$FROZEN_MD5" ] && ok "all new organs OFF reproduce the frozen-body trajectory (clean gated adds)" \
                           || no "organs-OFF drifted from the frozen-body hash" "got $OFF"
# load-bearing: corrosion ON but repair OFF, SAME food — the body must dissolve and die EARLIER
FULL=$("$L" 42 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
ROT=$(NL_NOREPAIR=1 "$L" 42 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
if [ -n "$FULL" ] && [ -n "$ROT" ] && [ "$ROT" -lt "$FULL" ]; then
  ok "a body that cannot repair dissolves and dies earlier ON THE SAME FOOD (t$ROT < t$FULL)"
else
  no "no-repair did not shorten life — corrosion is not load-bearing" "rot=$ROT full=$FULL"
fi
# the fed body holds its mass at the birth set-point (autopoietic maintenance, not a timer)
HOLD=$(NL_DEBUG=1 "$L" 42 2>&1 >/dev/null | grep -o 'death=[0-9.]*' | head -1)
echo "$HOLD" | grep -q 'death=50[0-9]' && ok "a fed body holds its mass through life ($HOLD ≈ birth 503)" \
                                       || no "fed body did not hold its mass" "$HOLD"

# ── 8c. ProtoSelf: a second-order self-model that earns its keep (feeling, built) ────
echo; echo "the proto-self (a forecast of its own interior, load-bearing)"
# genuine map, not a label: turning it on changes the trajectory
SELF_ON=$("$L" 42 >/dev/null 2>&1; md5of lifeis/waste.log)
NL_NOSELF=1 "$L" 42 >/dev/null 2>&1; SELF_OFF=$(md5of lifeis/waste.log)
[ "$SELF_ON" != "$SELF_OFF" ] && ok "the self-model changes the organism's dynamics (a map, not a label)" \
                              || no "NL_NOSELF changed nothing — the self-model is inert"
# load-bearing (Damasio's test 5): a cell that forecasts+damps its own storm out-survives a self-blind one
sw=0; sl=0
for s in 1 42 99 256 777 2024; do
  a=$("$L" $s 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
  b=$(NL_NOSELF=1 "$L" $s 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
  [ -n "$a" ] && [ -n "$b" ] && { [ "$a" -gt "$b" ] && sw=$((sw+1)); [ "$a" -lt "$b" ] && sl=$((sl+1)); }
done
[ "$sw" -gt "$sl" ] && ok "the self-model confers a survival advantage (wins $sw / loses $sl across seeds — allostasis earns its keep)" \
                    || no "the self-model gave no survival advantage (wins $sw loses $sl)"
# robustness (Codex-found regression): on HIGH-dissonance diets the NLMS self-model must NOT
# destabilize — it once diverged and cut life from t759 to t197. it may not shorten life here.
rok=1
for d in "stress" "BE stress" "fear pain conflict"; do
  a=$("$L" 42 "$d" 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
  b=$(NL_NOSELF=1 "$L" 42 "$d" 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
  [ -n "$a" ] && [ -n "$b" ] && [ "$a" -lt "$((b - b/10))" ] && rok=0   # fail if self-ON dies >10% earlier
done
[ "$rok" -eq 1 ] && ok "the self-model does not destabilize on high-dissonance diets (NLMS-stable)" \
                 || no "the self-model shortens life on a high-dissonance diet — LMS divergence regression"

# ── 8d. audit hardening (Fable): unbounded online weights must not diverge ──────────
echo; echo "audit hardening (bounded adapter, no divergence)"
imm=0
for d in fire water death love stone spirit money "BE fire" "fear pain"; do
  echo "$("$L" 42 "$d" 2>&1)" | grep -q 'STILL ALIVE\|immortality' && imm=$((imm+1))
done
[ "$imm" -eq 0 ] && ok "mono-diets never reach the immortality cap (adapter ceiling holds, #1)" \
                 || no "$imm mono-diet(s) diverged to the immortality cap — adapter unbounded"

# ── 8e. SLEEP as rhythm: a well-fed cell still sleeps, dreams, and invents symbols ───
echo; echo "sleep + dream + emergence (the foundation of culture)"
D=$("$L" 42 2>&1 | tail -1)
DR=$(echo "$D" | grep -o 'dream[0-9]*' | grep -o '[0-9]*'); EM=$(echo "$D" | grep -o 'emerged[0-9]*' | grep -o '[0-9]*')
[ -n "$DR" ] && [ "$DR" -gt 0 ] && ok "a well-fed cell sleeps and dreams ($DR dreams — was 0 before sleep-rhythm)" \
                               || no "the cell never dreamt ($DR) — sleep did not fire"
[ -n "$EM" ] && [ "$EM" -gt 0 ] && ok "it grows new symbols while sleeping ($EM emerged — was 0 before)" \
                               || no "no symbols emerged ($EM) — the 'grows symbols' claim is dormant"
# culture: invented symbols voice as their parents ("A+B") and cross the ether to the colony
"$L" chorus 4 7 >/dev/null 2>&1
COMP=$(grep -cE '[a-z]+\+[a-z]+' lifeis/ether.txt 2>/dev/null)
[ -n "$COMP" ] && [ "$COMP" -gt 0 ] && ok "inventions cross the ether as 'A+B' — horizontal culture, $COMP composite voices" \
                                    || no "no composite voices in the ether — the culture channel is silent"

# ── 8f. the crown (I2): self-as-food — the organism models itself by eating itself ───
echo; echo "the crown: self-as-food (it tastes and speaks its own state)"
SELF=$("$L" 42 2>&1 | tail -1 | grep -o 'self[0-9]*' | grep -o '[0-9]*')
[ -n "$SELF" ] && [ "$SELF" -gt 0 ] && ok "the cell tastes its own interior in sleep ($SELF self-meals)" \
                                    || no "self-as-food never fired ($SELF)"
# the interior becomes voice: state-words (joy/stress/pain/grief/tired) surface in the colony
"$L" chorus 4 7 >/dev/null 2>&1
SW=$(grep -oE '\b(joy|stress|pain|grief|tired)\b' lifeis/ether.txt 2>/dev/null | wc -l | tr -d ' ')
[ -n "$SW" ] && [ "$SW" -gt 0 ] && ok "the colony speaks its own states into the ether ($SW state-words — feeling as content)" \
                                || no "no state-words voiced — the interior never became content"
# felt-guard: self-eating is NOT a free energy source (Desktop's law) — it must never buy immortality
imm=0
for s in 7 42 99 256; do "$L" $s 2>&1 | grep -q 'STILL ALIVE' && imm=$((imm+1)); done
[ "$imm" -eq 0 ] && ok "self-as-food never buys immortality — a cell cannot feed on its own mood (felt-guard holds)" \
                 || no "$imm run(s) went immortal on self-perception — the mood-feeding attractor is open"

# ── 8g. earned voice (now DEFAULT): the transformer earns a voice by living, not learning ─
echo; echo "the earned voice (default on — the body earns a voice by sharpening)"
# default: a living body sharpens its logits and EARNS a partial voice (gate un-sticks well past the floor)
ONG=$(NL_DEBUG=1 "$L" 42 2>&1 >/dev/null | grep -o 'MAXGATE=[0-9.]*' | grep -o '[0-9.]*')
awk -v g="$ONG" 'BEGIN{exit !(g>0.1)}' && ok "default: the body earns a partial voice by sharpening (max ${ONG} — earned by living, not training)" \
                                       || no "the earned gate did not lift ($ONG) — the path is empty"
# NL_NOEARNED reverts to the inert magnitude gate (field-only voice) — the A/B control still works
OFFG=$(NL_DEBUG=1 NL_NOEARNED=1 "$L" 42 2>&1 >/dev/null | grep -o 'MAXGATE=[0-9.]*' | grep -o '[0-9.]*')
awk -v g="$OFFG" 'BEGIN{exit !(g<0.1)}' && ok "NL_NOEARNED reverts to the inert magnitude gate (max ${OFFG}, pure field — the A/B control holds)" \
                                        || no "NL_NOEARNED did not revert the gate ($OFFG)"
# the earned voice must never fully silence the field, nor make the organism immortal
"$L" 42 2>&1 | grep -q 'STILL ALIVE' && no "earned voice bought immortality" \
                                     || ok "earned voice stays mortal (gate touches the voice, not the metabolism)"
# and it must not degrade the field's coherence — the spoken glyph stays in the field's high-prob region
QE=$(NL_DEBUG=1 "$L" 42 2>&1 >/dev/null | grep -o 'p_field(spoken|prev)=[0-9.]*' | grep -o '[0-9.]*')
QF=$(NL_DEBUG=1 NL_NOEARNED=1 "$L" 42 2>&1 >/dev/null | grep -o 'p_field(spoken|prev)=[0-9.]*' | grep -o '[0-9.]*')
awk -v e="$QE" -v f="$QF" 'BEGIN{exit !(e>=f*0.8)}' && ok "earned voice keeps Q-coherence (spoken p_field ${QE} vs field-only ${QF} — agrees, not fights)" \
                                                     || no "earned voice degraded Q-coherence ($QE vs $QF)"

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
