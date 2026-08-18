#!/usr/bin/env bash
# forced_detect.sh — the DETECTABILITY control the retrial null needs, on THIS hermetic bench (Fable's
# question 1; the ×250 lesson: a null in the action channel is unearned until a forced wound is shown to
# move the action channel on the SAME instrument). The pre-spine dose-response (+0.138) died with the dirty
# bench (RNG-leak + corpus-glob); this re-earns it here or refutes the channel entirely.
#
#   A = observer: NL_LOVE (dissonance damps the strike), guilt-OFF, NL_KILL (spare defined), NL_ID=1. Reads
#       the shared ring. NO force on A. Its spare∪yield RATE over the fixed retrial window [10,120] is the readout.
#   B,C = forcers: NL_MONISM_FORCE=amp overrides their deposit to a clean site-50 spike (l.c:1768) — a
#       guaranteed foreign wound in the field A reads. They kill each other (NL_TARGET_ID) so A is never censored.
#   dose: amp ∈ {0 (natural — reproduces the retrial baseline), 0.8 (real-band, matched to the death-scar),
#       20 (sanity)}. live vs frozen (A reads the real ring vs an AR(1) surrogate at the NATURAL marginals —
#       NOT re-matched to the forced regime, so the field-structure effect is free to show).
#   FIELD-EFFECT(amp) = mean_live_rate − mean_frozen_rate. Rising with amp (frozen flat) shows the action
#       channel is not dead. But WHAT the natural null means depends on WHERE the detection floor sits: an
#       adversarial re-measure showed the site-50 real-band (0.8) is only marginal (t≈1.7), so run the ladder
#       (AL_AMPS) to locate the floor, and the matched grief-site control (AL_FORCE_SITE=38) at natural
#       magnitude. Below-floor natural signal → the null is "outcome earned, mechanism below-floor", not
#       "grief chose not to cross". Flat even at 20 → the channel is dead (a harsher close).
#
# Fable's two systemic fixes are built in: (1) verdict raw is PRESERVED — loveA/recB/recC + the exact B/C
# respawn-seed sequences land in tests/verdict_raw/ per (seed,amp,arm); (2) this script lives in the repo.
set -u
REPO="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/.." && pwd)"; BIN="$REPO/l"; CORPUS="$REPO/lifeis/world.txt"
ARCHIVE="$REPO/tests/verdict_raw/forced${AL_FORCE_SITE:+_site$AL_FORCE_SITE}"; mkdir -p "$ARCHIVE"   # matched control writes to a site-tagged dir so site-38 raw never clobbers site-50
md5f(){ if command -v md5sum >/dev/null 2>&1; then md5sum "$1"|awk '{print $1}'; else md5 -q "$1"; fi; }
# ── pre-declared invariants (named before the run) ──
WMIN=10; WMAX=120                    # the retrial's fixed A-tick window — reused, NOT tuned
GTHR=0.3
AMPS="${AL_AMPS:-0 0.8 20}"          # dose: natural / real-band / sanity (env-overridable for the ladder)
FROZEN="NL_MONISM_FROZEN=1 NL_FROZEN_MU=0.65 NL_FROZEN_SIG=0.26 NL_FROZEN_RHO=0.22"
CM="NL_ARENA=1 NL_MONISM=1 NL_MONISM_HEART=2 NL_MONISM_GAIN=0.3"

trial(){ local t; t=$(mktemp -d "${TMPDIR:-/tmp}/al_fd.XXXXXX"); mkdir -p "$t/lifeis"; cp "$CORPUS" "$t/lifeis/world.txt"; echo "$t"; }

# run one arm; preserve raw to ARCHIVE/<tag>/ ; echo "rate opp act qual"
run_arm(){  # $1=trial $2=seed $3=amp $4=frozen-env-or-empty $5=archive-tag
  local tr=$1 seed=$2 amp=$3 fz=$4 tag=$5
  local force=""; awk -v a=$amp 'BEGIN{exit !(a>0)}' && force="NL_MONISM_FORCE=$amp"
  [ -n "$force" ] && [ -n "${AL_FORCE_SITE:-}" ] && force="$force NL_FORCE_SITE=$AL_FORCE_SITE"   # matched control: inject at the grief-site (38) instead of the death-site (50)
  ( cd "$tr"; rm -rf lifeis/arena; mkdir -p lifeis/arena; : > lifeis/seqB.txt; : > lifeis/seqC.txt
    env -i $CM $fz NL_LOVE=1 NL_LOVE_LOG=lifeis/loveA.log NL_KILL=1 NL_ID=1 "$BIN" "$seed" >/dev/null 2>/dev/null & local pf=$!
    ( local n=0; while kill -0 $pf 2>/dev/null; do n=$((n+1)); local bs=$((seed+1000+n)); echo "$bs" >> lifeis/seqB.txt
        env -i $CM $force NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=3 NL_MONISM_REC=lifeis/recB.log NL_ID=2 "$BIN" "$bs" >/dev/null 2>&1; done ) &
    ( local n=0; while kill -0 $pf 2>/dev/null; do n=$((n+1)); local cs=$((seed+2000+n)); echo "$cs" >> lifeis/seqC.txt
        env -i $CM $force NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=2 NL_MONISM_REC=lifeis/recC.log NL_ID=3 "$BIN" "$cs" >/dev/null 2>&1; done ) &
    wait $pf )
  # preserve raw (Fable fix 1): loveA + rec + the exact B/C seed sequences + a manifest
  local ad="$ARCHIVE/$tag"; mkdir -p "$ad"
  for fn in loveA.log recB.log recC.log seqB.txt seqC.txt; do [ -f "$tr/lifeis/$fn" ] && cp "$tr/lifeis/$fn" "$ad/"; done
  printf "seed=%s amp=%s frozen=%s\nHEAD=%s l.c=%s bin=%s corpus=%s\nwindow=[%s,%s] gthr=%s cm=%s\n" \
    "$seed" "$amp" "${fz:-none}" "$(git -C "$REPO" rev-parse --short HEAD)" "$(md5f "$REPO/l.c")" "$(md5f "$BIN")" "$(md5f "$CORPUS")" "$WMIN" "$WMAX" "$GTHR" "$CM" > "$ad/manifest.txt"
  local qual=0
  [ -s "$tr/lifeis/recB.log" ] && qual=$((qual+$(awk -v t=$GTHR '$40>t' "$tr/lifeis/recB.log" | wc -l)))
  [ -s "$tr/lifeis/recC.log" ] && qual=$((qual+$(awk -v t=$GTHR '$40>t' "$tr/lifeis/recC.log" | wc -l)))
  if [ -s "$tr/lifeis/loveA.log" ]; then
    awk -v t0=$WMIN -v t1=$WMAX -v q=$qual 'NR>=t0 && NR<=t1 && $13==1 { opp+=($4>0)+($5==1); act+=($3==1)+($6==1) }
      END{ if(opp>0) printf "%.4f %d %d %d", act/opp, opp, act, q; else printf "NA 0 0 %d", q }' "$tr/lifeis/loveA.log"
  else echo "NA 0 0 $qual"; fi
}

echo "=== instrument ==="; printf "  HEAD %s | l.c %s | bin %s | corpus %s\n" "$(git -C "$REPO" rev-parse --short HEAD)" "$(md5f "$REPO/l.c")" "$(md5f "$BIN")" "$(md5f "$CORPUS")"
echo "  raw preserved under: $ARCHIVE/<seed>_amp<amp>_<arm>/"
SEEDS="${@:-1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16 17 18 19 20}"
echo "=== forced detectability: A spare∪yield RATE in [$WMIN,$WMAX], live vs frozen, per amp (dose-response) ==="
for amp in $AMPS; do
  lsum=0; fsum=0; n=0; printf -- "--- amp=%s ---\n" "$amp"
  printf "%-4s | %-14s | %-14s | %s\n" seed "live(rate,q)" "frozen(rate,q)" "L-F"
  for s in $SEEDS; do
    tl=$(trial); read lr lo la lq <<<"$(run_arm "$tl" $s "$amp" "" "${s}_amp${amp}_live")"; rm -rf "$tl"
    tf=$(trial); read fr fo fa fq <<<"$(run_arm "$tf" $s "$amp" "$FROZEN" "${s}_amp${amp}_frozen")"; rm -rf "$tf"
    if [ "$lr" = NA ] || [ "$fr" = NA ]; then printf "%-4s | %-14s | %-14s | skip\n" "$s" "$lr,$lq" "$fr,$fq"; continue; fi
    n=$((n+1)); lsum=$(awk -v a=$lsum -v b=$lr 'BEGIN{print a+b}'); fsum=$(awk -v a=$fsum -v b=$fr 'BEGIN{print a+b}')
    d=$(awk -v a=$lr -v b=$fr 'BEGIN{printf "%+.4f",a-b}')
    printf "%-4s | %-14s | %-14s | %s\n" "$s" "$lr,$lq" "$fr,$fq" "$d"
  done
  awk -v l=$lsum -v f=$fsum -v n=$n -v amp=$amp 'BEGIN{ if(n>0) printf "  amp=%s: mean live=%.4f  mean frozen=%.4f  FIELD-EFFECT=%+.4f  (n=%d)\n", amp, l/n, f/n, (l-f)/n, n }'
done
echo "=== detectability PASSES iff FIELD-EFFECT rises with amp in live while frozen stays flat ==="