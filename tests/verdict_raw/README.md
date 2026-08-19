# verdict raw — preserved measurement material

Sol's spine demand (and Fable's fix 1): a verdict run preserves its raw so the numbers can be re-measured,
not re-trusted. This holds the material behind the natural-love retrial null and the detectability controls
that bounded it (the death-site amplitude ladder + the matched grief-site control).

## `results/` (committed, human-readable — the measured numbers)

The full per-seed tables from the three runs of `tests/forced_detect.sh`, exactly as produced:

- `forced_dose_site50_0-0.8-20.txt` — the first detectability dose-response (site-50, amps 0 / 0.8 / 20).
  FIELD-EFFECT −0.016 / +0.022 / +0.071.
- `ladder_site50_1-2-4-8.txt` — the amplitude ladder locating the death-site detection floor. FIELD-EFFECT
  +0.024 / +0.021 / +0.039 / +0.056 (t = 2.14 / 2.02 / 3.23 / 3.70); the floor (t>2) sits at ~amp 1.0, so
  the physical death-scar (0.8, t=1.70) lands just under it.
- `matched_site38_0.56-2-8-20.txt` — the matched control (`NL_FORCE_SITE=38`) at grief's own magnitude and a
  sanity ladder. FIELD-EFFECT +0.003 / +0.024 / +0.059 / +0.053 (t = 0.24 / 2.25 / 4.55 / 4.95): the
  grief-site is wired (decisive at 8/20) but natural grief's magnitude 0.56 is null — a below-floor null.
- `grief_presence_amp0_site50.txt` — the **second leg** of the verdict: was natural grief present in A's
  window, and at what magnitude? Per-seed in-window count + mean L[38] for the natural condition (amp=0). The
  null is not vacuous — grief is present in 27/30 seeds at mean L[38]=0.56, right where the matched control
  says it falls under the floor. (Re-derivable from the `recB_L38`/`recC_L38` extractions in the core below.)

## `forced_core.tgz` (committed, ~1.1 MB — the crux raw)

The judicial readout for the two NULL conditions that ARE the verdict: natural (amp=0, site-50) and matched
(amp=0.56, site-38), 30 seeds × 2 arms each — **both legs of the floor**. Per dir: `loveA.log` (leg 1, the
observer's per-tick action ledger — 13 cols; spare∪yield rate = `sum(($3==1)+($6==1))/sum(($4>0)+($5==1))`
over `NR∈[10,120] && $13==1`); `recB_L38.txt`/`recC_L38.txt` (leg 2, the grief-signal timeline — µs + L[38]
for every deposit with L[38]>0.01, the reduced form of the 64-float field-deposit logs, enough to recompute
in-window grief presence and magnitude); `seqB.txt`/`seqC.txt` (the exact respawn-seed sequences — the run is
only statistically reproducible); and `manifest.txt` (instrument hashes + regime). Unpack: `tar xzf
forced_core.tgz`.

## full raw (not committed)

Every amplitude at both sites, including the 64-float-per-tick field-deposit logs (`recB`/`recC`, needed to
recover in-window grief presence — mean L[38]=0.56, present 27/30) — ~19 MB — is archived off-repo (too large
to commit). The committed core plus the results tables re-measure the verdict, and the harnesses regenerate
the full raw from scratch.

## reproduce from scratch

- retrial null: `bash tests/rerun_natural.sh 1 2 … 30`
- detectability + ladder: `AL_AMPS="0 0.8 1 2 4 8 20" bash tests/forced_detect.sh 1 … 30`
- matched grief-site control: `AL_FORCE_SITE=38 AL_AMPS="0.56 2 8 20" bash tests/forced_detect.sh 1 … 30`
