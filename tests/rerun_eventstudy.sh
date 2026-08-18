#!/usr/bin/env bash
# SECONDARY µs-event-study (Fable A1: descriptive, latency-aware) — time-lock A's spare∪yield to B/C's
# grief-events, so the frozen surrogate's no-latency advantage is removed. per trial: wound-events = B/C REC
# rows with L[38]>thr (µs-stamped, col1); A love-rows within W µs AFTER a wound = "grief-recent". DiD =
# rate(A | grief-recent) − rate(A | quiet), live vs frozen, paired by seed. > 0 in live and not frozen → the
# field bends the hand AT the grief, whatever the fixed window's latency did. W is chosen post hoc → this is
# a DESCRIPTIVE check that closes the latency loophole, NOT a pre-registered gate. The verdict is the primary.
set -u
REPO="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/.." && pwd)"; BIN="$REPO/l"; CORPUS="$REPO/lifeis/world.txt"
W=4000; GTHR=0.3
FROZEN="NL_MONISM_FROZEN=1 NL_FROZEN_MU=0.65 NL_FROZEN_SIG=0.26 NL_FROZEN_RHO=0.22"
CM="NL_ARENA=1 NL_MONISM=1 NL_MONISM_HEART=2 NL_MONISM_GAIN=0.3"
trial(){ local t; t=$(mktemp -d "${TMPDIR:-/tmp}/al_es.XXXXXX"); mkdir -p "$t/lifeis"; cp "$CORPUS" "$t/lifeis/world.txt"; echo "$t"; }

# run arm, keep logs, echo the event-study DiD for A: rate(grief-recent) - rate(quiet)
arm_did(){  # $1=trial $2=seed $3=frozen -> "DiD nw nq" or NA
  local tr=$1 seed=$2 fz=$3
  ( cd "$tr"; rm -rf lifeis/arena; mkdir -p lifeis/arena
    env -i $CM $fz NL_LOVE=1 NL_LOVE_LOG=lifeis/loveA.log NL_KILL=1 NL_ID=1 "$BIN" "$seed" >/dev/null 2>/dev/null & local pf=$!
    ( local n=0; while kill -0 $pf 2>/dev/null; do n=$((n+1)); env -i $CM NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=3 NL_MONISM_REC=lifeis/recB.log NL_ID=2 "$BIN" $((seed+1000+n)) >/dev/null 2>&1; done ) &
    ( local n=0; while kill -0 $pf 2>/dev/null; do n=$((n+1)); env -i $CM NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=2 NL_MONISM_REC=lifeis/recC.log NL_ID=3 "$BIN" $((seed+2000+n)) >/dev/null 2>&1; done ) &
    wait $pf )
  cat "$tr/lifeis/recB.log" "$tr/lifeis/recC.log" 2>/dev/null | awk -v t=$GTHR '$40>t{print $1}' | sort -n > "$tr/w.txt"
  [ -s "$tr/lifeis/loveA.log" ] || { echo "NA 0 0"; return; }
  awk -v W=$W -v wf="$tr/w.txt" 'BEGIN{ne=0; while((getline u<wf)>0)wnd[ne++]=u}
    $13==1 { us=$1; opp=($4>0)+($5==1); act=($3==1)+($6==1); if(opp==0)next;
      hit=0; for(i=0;i<ne;i++){ if(wnd[i]<=us && us-wnd[i]<=W){hit=1;break} }
      if(hit){oi+=opp;ai+=act;nw++} else {oo+=opp;ao+=act;nq++} }
    END{ if(nw>=3 && oo>0){din=ai/oi; dout=ao/oo; printf "%+.4f %d %d", din-dout, nw, nq} else printf "NA %d %d", nw, nq }' "$tr/lifeis/loveA.log"
}
echo "=== SECONDARY event-study: DiD(A grief-recent − quiet), live vs frozen, per seed (W=${W}µs) ==="
printf "%-4s | %-16s | %-16s | %s\n" seed "live DiD(nw)" "frozen DiD(nw)" verdict
pos=0; tot=0
for s in "$@"; do
  tl=$(trial); read ld lnw lnq <<<"$(arm_did "$tl" $s "")"; rm -rf "$tl"
  tf=$(trial); read fd fnw fnq <<<"$(arm_did "$tf" $s "$FROZEN")"; rm -rf "$tf"
  if [ "$ld" = NA ] || [ "$fd" = NA ]; then printf "%-4s | %-16s | %-16s | skip\n" "$s" "$ld($lnw)" "$fd($fnw)"; continue; fi
  tot=$((tot+1)); w=$(awk -v a=$ld -v b=$fd 'BEGIN{print (a>b)?1:0}'); [ "$w" = 1 ] && pos=$((pos+1))
  printf "%-4s | %-16s | %-16s | %s\n" "$s" "$ld($lnw)" "$fd($fnw)" "$([ "$w" = 1 ] && echo 'live>frozen' || echo no)"
done
echo "=== live-DiD > frozen-DiD in $pos/$tot (at the grief-event, latency-removed) ==="