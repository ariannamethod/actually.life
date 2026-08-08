#!/usr/bin/env bash
# tests/hermetic.sh — the hermetic experiment runner (measurement-spine step 6).
#
# every trial gets a fresh isolated root, an immutable copy of the base corpus, and a SCRUBBED
# environment (`env -i` — no inherited NL_*), so neither a stray shell variable nor a previous run's
# runtime tail can change what an organism eats or does. the instrument's identity (source, binary,
# corpus hashes) is recorded with every batch; raw outputs are kept per trial. reproducible by
# construction: same seed, same world, same result.
#
# use as a library:   source tests/hermetic.sh
#   al_manifest                                   -> the instrument's identity (HEAD, hashes, uname)
#   al_trial                                      -> mint a fresh isolated trial root, echo its path
#   al_solo  <trial> <seed> <NL_ENV...>           -> run one organism in <trial>, echo its waste md5
#   al_triad <trial> <seed> "<focal NL_ENV>" "<rival NL_ENV>" ["<rival NL_ENV>"...]
#                                                 -> focal (waited) + rivals (respawn until focal dies),
#                                                    one shared arena, all with scrubbed env; echo focal waste md5
#
# self-test:   bash tests/hermetic.sh --selftest   (reproducibility + env-scrub + isolation)

set -u
AL_REPO="$(cd "$(dirname "${BASH_SOURCE[0]:-$0}")/.." && pwd)"
AL_BIN="$AL_REPO/l"
AL_CORPUS="$AL_REPO/lifeis/world.txt"
al_md5(){ if command -v md5sum >/dev/null 2>&1; then md5sum "$1"|awk '{print $1}'; else md5 -q "$1"; fi; }

al_manifest(){
  printf 'git_head  %s\n' "$(git -C "$AL_REPO" rev-parse --short HEAD 2>/dev/null || echo none)"
  printf 'l.c       %s\n' "$(al_md5 "$AL_REPO/l.c")"
  printf 'binary    %s\n' "$(al_md5 "$AL_BIN")"
  printf 'corpus    %s  (%s non-empty lines)\n' "$(al_md5 "$AL_CORPUS")" "$(grep -cve '^[[:space:]]*$' "$AL_CORPUS")"
  printf 'uname     %s\n' "$(uname -mrs)"
}

al_trial(){
  local t; t=$(mktemp -d "${TMPDIR:-/tmp}/al_trial.XXXXXX") || return 1
  mkdir -p "$t/lifeis" && cp "$AL_CORPUS" "$t/lifeis/world.txt" && echo "$t"
}

# al_solo <trial> <seed> <NL_ENV...>   (env passed as separate VAR=val args)
al_solo(){
  local trial=$1 seed=$2; shift 2
  ( cd "$trial" && env -i "$@" "$AL_BIN" "$seed" >lifeis/stdout.txt 2>lifeis/stderr.txt )
  al_md5 "$trial/lifeis/waste.log"
}

# al_triad <trial> <seed> "<focal env>" "<rival env>"...   (each env string is one quoted arg)
al_triad(){
  local trial=$1 seed=$2 focal=$3; shift 3
  ( cd "$trial"
    env -i $focal "$AL_BIN" "$seed" >lifeis/focal.out 2>lifeis/focal.err & local pf=$!
    local i=0
    for r in "$@"; do
      i=$((i+1))
      ( local n=0; while kill -0 "$pf" 2>/dev/null; do n=$((n+1)); env -i $r "$AL_BIN" $((seed+1000*i+n)) >/dev/null 2>&1; done ) &
    done
    wait "$pf"
  )
  [ -s "$trial/lifeis/waste.log" ] && al_md5 "$trial/lifeis/waste.log" || echo NA
}

# ── self-test ────────────────────────────────────────────────────────────────
if [ "${1:-}" = "--selftest" ]; then
  [ -x "$AL_BIN" ] || { echo "FATAL: build first (cc -O2 -o l l.c -lm)"; exit 2; }
  echo "=== instrument ==="; al_manifest
  fail=0
  echo "=== reproducibility: same seed, isolated trials, identical result ==="
  t1=$(al_trial); t2=$(al_trial)
  a=$(al_solo "$t1" 42 NL_ARENA=1 NL_MONISM=1); b=$(al_solo "$t2" 42 NL_ARENA=1 NL_MONISM=1)
  if [ "$a" = "$b" ]; then echo "  ok  $a == $b"; else echo "  FAIL  $a != $b"; fail=1; fi
  echo "=== env-scrub: a stray NL_LOVE in the shell must NOT reach the trial ==="
  export NL_LOVE=1 NL_MONISM_FROZEN=1   # pollute the shell
  t3=$(al_trial); c=$(al_solo "$t3" 42 NL_ARENA=1 NL_MONISM=1)
  unset NL_LOVE NL_MONISM_FROZEN
  if [ "$c" = "$a" ]; then echo "  ok  $c == $a (stray vars scrubbed by env -i)"; else echo "  FAIL  $c != $a (env leaked!)"; fail=1; fi
  echo "=== isolation: a decoy file in one trial cannot reach another ==="
  t4=$(al_trial); printf 'DECOY %.0s\n' {1..300} > "$t4/lifeis/decoy.txt"
  d=$(al_solo "$t4" 42 NL_ARENA=1 NL_MONISM=1)
  if [ "$d" = "$a" ]; then echo "  ok  $d == $a (decoy ignored — hermetic manifest)"; else echo "  FAIL  $d != $a (corpus leaked!)"; fail=1; fi
  rm -rf "$t1" "$t2" "$t3" "$t4"
  echo "=== $([ $fail = 0 ] && echo 'HERMETIC RUNNER OK — reproducible, scrubbed, isolated' || echo 'SELF-TEST FAILED') ==="
  exit $fail
fi
