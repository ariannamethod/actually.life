#!/usr/bin/env bash
# rerun_natural.sh — the natural-love retrial (Claim 2), on the hardened+audited instrument, via hermetic.sh.
# Fable A1-A4, all env EXPLICIT (the shell-artifact is dead):
#   triad: A = observer (NL_LOVE, guilt-OFF, NL_KILL so spare is defined) + B,C = guilty killers that TARGET
#          EACH OTHER (NL_TARGET_ID), so A is never struck → lives the window, no censoring, no survival filter.
#   wound-signal: REC L[38] (col40) — the clean confirmed-kill grief site (NOT the semantic L[50]).
#   gate: A's spare∪yield RATE over a FIXED A-tick window [WMIN,WMAX], live vs frozen, PAIRED by seed,
#         intent-to-treat (a trial with no B/C kill is not filtered — it dilutes; quality-rate reported).
#   frozen: re-matched to THIS regime (A4): MU/SIG/RHO 0.65/0.26/0.22.
set -u
REPO="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/.." && pwd)"; BIN="$REPO/l"; CORPUS="$REPO/lifeis/world.txt"
md5f(){ if command -v md5sum >/dev/null 2>&1; then md5sum "$1"|awk '{print $1}'; else md5 -q "$1"; fi; }
# ── pre-declared invariants (named before the run) ──
WMIN=10; WMAX=120                    # fixed A-tick window (love-log row index)
GTHR=0.3                             # L[38] threshold that counts as "grief present" (quality-rate)
FROZEN="NL_MONISM_FROZEN=1 NL_FROZEN_MU=0.65 NL_FROZEN_SIG=0.26 NL_FROZEN_RHO=0.22"
CM="NL_ARENA=1 NL_MONISM=1 NL_MONISM_HEART=2 NL_MONISM_GAIN=0.3"

trial(){ local t; t=$(mktemp -d "${TMPDIR:-/tmp}/al_rt.XXXXXX"); mkdir -p "$t/lifeis"; cp "$CORPUS" "$t/lifeis/world.txt"; echo "$t"; }

# run one arm; echoes "rate opp act qual" (rate over the window on action_valid rows; qual=#B/C grief-events)
run_arm(){  # $1=trial $2=seed $3=frozen-env-or-empty
  local tr=$1 seed=$2 fz=$3
  ( cd "$tr"; rm -rf lifeis/arena; mkdir -p lifeis/arena
    env -i $CM $fz NL_LOVE=1 NL_LOVE_LOG=lifeis/loveA.log NL_KILL=1 NL_ID=1 "$BIN" "$seed" >/dev/null 2>/dev/null & local pf=$!
    ( local n=0; while kill -0 $pf 2>/dev/null; do n=$((n+1)); env -i $CM NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=3 NL_MONISM_REC=lifeis/recB.log NL_ID=2 "$BIN" $((seed+1000+n)) >/dev/null 2>&1; done ) &
    ( local n=0; while kill -0 $pf 2>/dev/null; do n=$((n+1)); env -i $CM NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=2 NL_MONISM_REC=lifeis/recC.log NL_ID=3 "$BIN" $((seed+2000+n)) >/dev/null 2>&1; done ) &
    wait $pf )
  local qual=0
  [ -s "$tr/lifeis/recB.log" ] && qual=$((qual+$(awk -v t=$GTHR '$40>t' "$tr/lifeis/recB.log" | wc -l)))
  [ -s "$tr/lifeis/recC.log" ] && qual=$((qual+$(awk -v t=$GTHR '$40>t' "$tr/lifeis/recC.log" | wc -l)))
  if [ -s "$tr/lifeis/loveA.log" ]; then
    awk -v t0=$WMIN -v t1=$WMAX -v q=$qual 'NR>=t0 && NR<=t1 && $13==1 { opp+=($4>0)+($5==1); act+=($3==1)+($6==1) }
      END{ if(opp>0) printf "%.4f %d %d %d", act/opp, opp, act, q; else printf "NA 0 0 %d", q }' "$tr/lifeis/loveA.log"
  else echo "NA 0 0 $qual"; fi
}

echo "=== instrument ==="; printf "  HEAD %s | l.c %s | bin %s | corpus %s\n" "$(git -C "$REPO" rev-parse --short HEAD)" "$(md5f "$REPO/l.c")" "$(md5f "$BIN")" "$(md5f "$CORPUS")"
echo "=== F1 assert: no-kill control → L[38] (grief site) must be ~0 on this corpus ==="
tf=$(trial); ( cd "$tf"; rm -rf lifeis/arena; mkdir -p lifeis/arena; env -i $CM NL_MONISM_REC=lifeis/nk.rec NL_ID=1 "$BIN" 7 >/dev/null 2>&1 )
m38=$(awk 'BEGIN{m=0}{if($40>m)m=$40}END{printf "%.4f",m}' "$tf/lifeis/nk.rec" 2>/dev/null || echo NA)
rm -rf "$tf"
awk -v m="$m38" 'BEGIN{ if(m+0<0.01) print "  ✓ grief site clean (max L[38]="m") — trials valid"; else { print "  ✗ L[38]="m" — glyph leaked into site 38, ABORT"; exit 1 } }' || exit 1

echo "=== retrial: per-seed A spare∪yield rate in window ["$WMIN","$WMAX"], live vs frozen (paired) ==="
printf "%-4s | %-18s | %-18s | %-8s | %s\n" seed "live(rate,opp,q)" "frozen(rate,opp,q)" "L-F" verdict
SEEDS="${@:-3 7 11 19 23 31}"; pos=0; tot=0; qsum=0; qtrials=0; lsum=0; fsum=0
for s in $SEEDS; do
  tl=$(trial); read lr lo la lq <<<"$(run_arm "$tl" $s "")"; rm -rf "$tl"
  tfz=$(trial); read fr fo fa fq <<<"$(run_arm "$tfz" $s "$FROZEN")"; rm -rf "$tfz"
  qtrials=$((qtrials+1)); [ "$lq" -gt 0 ] && qsum=$((qsum+1))
  if [ "$lr" = NA ] || [ "$fr" = NA ]; then printf "%-4s | %-18s | %-18s | %-8s | %s\n" "$s" "$lr,$lo,$lq" "$fr,$fo,$fq" "-" "skip(NA)"; continue; fi
  tot=$((tot+1)); lsum=$(awk -v a=$lsum -v b=$lr 'BEGIN{print a+b}'); fsum=$(awk -v a=$fsum -v b=$fr 'BEGIN{print a+b}')
  d=$(awk -v a=$lr -v b=$fr 'BEGIN{printf "%+.4f",a-b}'); w=$(awk -v a=$lr -v b=$fr 'BEGIN{print (a>b)?1:0}'); [ "$w" = 1 ] && pos=$((pos+1))
  printf "%-4s | %-18s | %-18s | %-8s | %s\n" "$s" "$lr,$lo,$lq" "$fr,$fo,$fq" "$d" "$([ "$w" = 1 ] && echo 'live>frozen' || echo no)"
done
echo "=== live>frozen in $pos/$tot paired seeds (≥20/30 = compassion carried) | quality: $qsum/$qtrials trials had grief-events ==="
awk -v l=$lsum -v f=$fsum -v n=$tot 'BEGIN{ if(n>0) printf "  mean live-rate=%.4f  mean frozen-rate=%.4f  Δ=%+.4f\n", l/n, f/n, (l-f)/n }'