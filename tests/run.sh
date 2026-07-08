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
SOLO_MD5=a17cfd058a78b6d86f3e57dcd561dc07   # md5 of ./l 42 waste.log — the NEW DEFAULT (probabilistic continuation; async is an opt-in mode)
PRECONT_MD5=b5d50234ae8258b136c9ad9a86e8f8bb # ...NL_NOCONT — the pre-continuation organism (deterministic death — preserved bit-identical)
FIELD_MD5=a734e3a2271d1259c21497ad7755a355  # ...pre-continuation + NL_NOEARNED (field-only voice)
FROZEN_MD5=9a9d68481ff8551fd9e2644c0e54e860 # ...pre-continuation + EVERY organ off — the pre-living-body trajectory (VOCAB_CAP=154)
ASYNC_MD5=01f9f1107666cfe49b05d026db38157f  # ...NL_NOCONT NL_ASYNC — async only (an opt-in mode; composes imperfectly with continuation, kept separate)

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
[ "$A" = "$SOLO_MD5" ] && ok "the default trajectory matches the frozen hash ($SOLO_MD5 — probabilistic continuation + async)" \
                       || no "the default trajectory DRIFTED from the frozen hash" "got $A"
# the pre-continuation organism (the base the new default is built on) is preserved bit-identical:
NL_NOCONT=1 NL_NOASYNC=1 "$L" 42 >/dev/null 2>&1; P=$(md5of lifeis/waste.log)
[ "$P" = "$PRECONT_MD5" ] && ok "the pre-continuation organism is preserved (NL_NOCONT NL_NOASYNC → $PRECONT_MD5 — a clean promotion)" \
                          || no "the pre-continuation organism drifted" "got $P"
# sections 3–8g define the base criteria of life, calibrated on the pre-continuation organism — run them there:
export NL_NOCONT=1 NL_NOASYNC=1

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
# the README claim: "a cell that models itself outlives a cell that is a stranger to itself" —
# self-model vs NL_NOSELF (a cell blind to its own interior). the claim is about self vs no-self.
# (a deep audit noted the advantage is also matchable by a plain fixed damper — i.e. the CAUSE is the
# S-damping, not the self-knowledge; NL_FIXEDDAMP is kept as the control that documents that caveat.
# but the claim as written — models-itself outlives stranger-to-itself — is true and is what we test.)
sw=0; sl=0
for s in 1 7 42 99 123 256 777 2024; do
  a=$("$L" $s 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
  b=$(NL_NOSELF=1 "$L" $s 2>&1 | grep -o 'died at tick [0-9]*' | grep -o '[0-9]*' | head -1)
  [ -n "$a" ] && [ -n "$b" ] && { [ "$a" -gt "$b" ] && sw=$((sw+1)); [ "$a" -lt "$b" ] && sl=$((sl+1)); }
done
[ "$sw" -gt "$sl" ] && ok "a cell that models itself outlives a stranger to itself ($sw wins / $sl losses — the README claim holds)" \
                    || no "the self-model did not outlive the self-blind cell ($sw/$sl) — README claim in debt"
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
# RECURSIVE culture (Δ1+Δ2): a symbol crosses the ether WHOLE, is adopted, co-occurs, and PARENTS a new
# symbol — symbols of symbols. this is a chorus phenomenon (no ether, no transmission, no recursion in solo).
REC=$(for s in 3 7 9 42; do "$L" chorus 6 $s 2>&1 | grep -oE 'rec[1-9][0-9]*'; done | wc -l | tr -d ' ')
[ -n "$REC" ] && [ "$REC" -gt 0 ] && ok "recursion fires: symbols become parents of symbols ($REC recursive-symbol cells across seeds — open-ended culture)" \
                                  || no "no recursive symbols — culture is still capped at depth 1"

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

# reset to the promoted DEFAULT (continuation + async) for the facet criteria below
unset NL_NOCONT NL_NOASYNC

# ── 8h. PROBABILISTIC CONTINUATION (DEFAULT ON; NL_NOCONT opts out): death is a REGION of a hazard surface,
#        not a cliff; the organism spends form (WILL) to pay a forecast of its own collapse. death AND
#        non-death are both live regions — neither is sovereign. this is now the default organism. ──
echo; echo "probabilistic continuation + will (default — death is a region, non-death is reachable)"
# the default (./l 42 → SOLO_MD5) is already checked in the determinism section. here: the continuation criteria.
CS=$("$L" 42 2>&1)
echo "$CS" | grep -q 'died at tick' && ok "the default organism dies (mortality intact)" || no "the default did not die"
echo "$CS" | grep -q 'immortality hole' && no "the default hit the immortality cap" || ok "the default never hits the immortality cap (ENTROPY_FLOOR holds)"
"$L" chorus 4 7 2>&1 | grep -q 'the colony fell silent' && ok "the default chorus is mortal (the colony falls silent)" || no "the default chorus never fell silent"
# opt-out: NL_NOCONT reverts to the pre-continuation organism (deterministic death — bit-identical)
NL_NOCONT=1 "$L" 42 >/dev/null 2>&1; OC=$(md5of lifeis/waste.log)
[ "$OC" = "$PRECONT_MD5" ] && ok "NL_NOCONT opts out of continuation cleanly (deterministic death — $PRECONT_MD5)" \
                          || no "NL_NOCONT did not cleanly opt out" "got $OC"
# the DOUBLED falsifier as a distribution: vs the pre-continuation organism, some seeds die EARLIER, some LONGER —
# both regions materially populated, neither sovereign — and none reach the immortality cap.
cearlier=0; clonger=0; cimm=0
for s in 1 7 42 99 256 777 2024 5 123 999; do
  a=$(NL_NOCONT=1 NL_NOASYNC=1 "$L" $s 2>&1 | grep -o 'died at tick [0-9]*'|grep -o '[0-9]*'|head -1)
  b=$("$L" $s 2>&1 | grep -o 'died at tick [0-9]*'|grep -o '[0-9]*'|head -1)
  "$L" $s 2>&1 | grep -q 'STILL ALIVE\|immortality' && cimm=$((cimm+1))
  [ -n "$a" ] && [ -n "$b" ] && { [ "$b" -lt "$a" ] && cearlier=$((cearlier+1)); [ "$b" -gt "$a" ] && clonger=$((clonger+1)); }
done
{ [ "$cearlier" -ge 2 ] && [ "$clonger" -ge 2 ] && [ "$cimm" -eq 0 ]; } \
  && ok "death AND non-death are both materially-populated regions ($cearlier die earlier, $clonger live longer than the pre-continuation organism, 0 immortal)" \
  || no "the continuation distribution collapsed toward one pole" "earlier=$cearlier longer=$clonger imm=$cimm"
# WILL — the continuation act spends accumulated form. CAVEAT (the NL_FIXEDDAMP discipline, measured by a
# K-sweep): the self-model's forecast-TIMING is NOT load-bearing — a well-chosen fixed spend (NL_FIXEDWILL≈0.05)
# matches or beats forecast-driven will across seeds. the load-bearing thing is form-spending per se, not the
# forecast. NL_FIXEDWILL is the control that documents it; here we assert only that it is a living, mortal organism.
FW=$(NL_FIXEDWILL=0.05 "$L" 42 2>&1)
{ echo "$FW" | grep -q 'died at tick' && ! echo "$FW" | grep -q 'immortality hole'; } \
  && ok "NL_FIXEDWILL (blind-spend control) is a living, mortal organism — documents that will's forecast-timing is not load-bearing" \
  || no "NL_FIXEDWILL control broke"
# generation + Q-coherence are not broken by continuation — the default voice stays in the field's high-prob region
CQE=$(NL_DEBUG=1 "$L" 42 2>&1 >/dev/null | grep -o 'p_field(spoken|prev)=[0-9.]*' | grep -o '[0-9.]*')
CQD=$(NL_DEBUG=1 NL_NOCONT=1 NL_NOASYNC=1 "$L" 42 2>&1 >/dev/null | grep -o 'p_field(spoken|prev)=[0-9.]*' | grep -o '[0-9.]*')
awk -v e="$CQE" -v d="$CQD" 'BEGIN{exit !(e>=d*0.8)}' && ok "the default keeps Q-coherence (spoken p_field ${CQE} vs pre-continuation ${CQD} — generation intact)" \
                                                       || no "continuation degraded Q-coherence ($CQE vs $CQD)"

# ── 8i. ASYNC (NL_ASYNC, opt-in mode): the six Kuramoto chambers schedule the regulatory organs on
#        coprime-period clocks — the temporal face of the same field. it composes imperfectly with continuation
#        (the two together thin the voice), so it is kept as a separate mode, tested alone here. ──
echo; echo "asynchrony (NL_ASYNC, an opt-in mode — the organism can run on coprime-period chamber clocks)"
NL_NOCONT=1 NL_ASYNC=1 "$L" 42 >/dev/null 2>&1; XA=$(md5of lifeis/waste.log)
NL_NOCONT=1 NL_ASYNC=1 "$L" 42 >/dev/null 2>&1; XB=$(md5of lifeis/waste.log)
[ "$XA" = "$XB" ] && ok "async is reproducible (seeded phases, no threads — per-seed determinism)" \
                  || no "async is NON-deterministic" "$XA vs $XB"
[ "$XA" = "$ASYNC_MD5" ] && ok "async-only trajectory matches its baseline ($ASYNC_MD5)" \
                         || no "async-only trajectory drifted" "got $XA"
AS=$(NL_NOCONT=1 NL_ASYNC=1 "$L" 42 2>&1)
{ echo "$AS" | grep -q 'died at tick' && ! echo "$AS" | grep -q 'immortality hole'; } \
  && ok "async-only is a living, mortal organism" || no "async-only broke"
NL_NOCONT=1 NL_ASYNC=1 "$L" chorus 4 7 2>&1 | grep -q 'the colony fell silent' && ok "async chorus is mortal (the colony falls silent)" || no "async chorus never fell silent"
# async alone keeps the voice (the self-model stays every-tick); composed with continuation it thins — kept separate
AQE=$(NL_DEBUG=1 NL_NOCONT=1 NL_ASYNC=1 "$L" 42 2>&1 >/dev/null | grep -o 'p_field(spoken|prev)=[0-9.]*'|grep -o '[0-9.]*')
AQD=$(NL_DEBUG=1 NL_NOCONT=1 NL_NOASYNC=1 "$L" 42 2>&1 >/dev/null | grep -o 'p_field(spoken|prev)=[0-9.]*'|grep -o '[0-9.]*')
awk -v e="$AQE" -v d="$AQD" 'BEGIN{exit !(e>=d*0.8)}' && ok "async-only keeps Q-coherence (spoken p_field ${AQE} vs pre-continuation ${AQD})" \
                                                       || no "async-only degraded Q-coherence ($AQE vs $AQD)"
# both facets together (NL_ASYNC on the continuation default): they COMPOSE — deterministic and mortal —
# though the composed voice thins (async gates speak + continuation shortens life), which is why async is
# kept a separate opt-in mode rather than folded into the default.
NL_ASYNC=1 "$L" 42 >/dev/null 2>&1; YA=$(md5of lifeis/waste.log)
NL_ASYNC=1 "$L" 42 >/dev/null 2>&1; YB=$(md5of lifeis/waste.log)
CY=$(NL_ASYNC=1 "$L" 42 2>&1)
{ [ "$YA" = "$YB" ] && echo "$CY" | grep -q 'died at tick' && ! echo "$CY" | grep -q 'immortality hole'; } \
  && ok "continuation + async compose (deterministic, mortal — async is a separate mode, the composed voice thins)" \
  || no "continuation + async did not compose cleanly" "got $YA"

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
