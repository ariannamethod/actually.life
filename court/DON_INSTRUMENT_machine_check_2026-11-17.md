# Don machine-readability check — `INSTRUMENT.md`

Date: 2026-11-17. Checked commit: `8476f86` (`INSTRUMENT.md` DRAFT 2026-10-05 + line-5 reword).
Companion: `court/SOL_REVIEW_INSTRUMENT_2026-11-17.md` (Sol, frame). This file is the machine side:
for each field, can a verifier compute it from the file and the committed raw alone.

## Findings on the typed format

1. **`resolution` mixes two scales.** In the BELOW-FLOOR worked example the floor (~1.0, `llog.md:1032`)
   is an *input* quantity (deposit amplitude needed to move the readout); `effect / spread /
   minimum_effect` are *output* quantities (action-rate difference). The row has no field for the natural
   signal magnitude (0.56). The rule for the example is `signal_magnitude < input_floor`; without a
   `signal_magnitude` field and a scale on `resolution` the reading is not computable.
2. **`below_resolution` has two assignment rules that diverge.** (a) "cannot separate the effect from
   noise at this magnitude" (a pre-fixed number); (b) "a null on a sensitive instrument that is not
   `contradicted`". Effect above resolution with a wide interval is (b) but not (a). One total law over
   pre-fixed numbers: `detected` = interval lower bound ≥ claim threshold; `contradicted` = interval upper
   bound < claim threshold ∧ claim threshold ≥ output resolution; `below_resolution` = claim threshold <
   output resolution, or `signal_magnitude < input_floor` — decidable before data, so the instrument
   declines the read; everything else `unresolved`.
3. **`spread` undefined.** "Confidence interval excludes" names no estimator (paired per-seed difference,
   t, bootstrap), level, or N. The old criterion was `t > 2 at amp=1` (`llog.md:1032`).
4. **`control_identical` names no streams.** Arena runs are not byte-exact (scheduling, wall time). The
   verifier needs the list that must match: `seqB`/`seqC`, seeds, `bin`/`corpus` md5, the manifest `cm=`
   line. This is the one part of the dissolved §7 worth carrying.
5. **`resolution_measured` for the grief axis — provenance.** Corrected after Sol's byte check: the
   site-38 rungs ARE in `tests/verdict_raw/ladder_core.tgz` (`forced_site38/`, amplitudes 2/8/20, two
   arms, 30 seeds = 180 sets). My first statement that they were absent came from a truncated listing and
   is withdrawn. What stands, sharpened by the manifests themselves:
   - no manifest records `NL_FORCE_SITE=38`; the site lives only in the directory prefix. A machine can
     count site-38 data but cannot establish the intervention site from the record.
   - the historical site-38 bound is `(0.56, 2]` (input 0.56 → effect +0.0028, t=0.24; input 2 → +0.0243,
     t=2.25), not a measured `~1–2`; the site-50 rung at 1 belongs to another site.
   - **`HEAD=` contradicts `l.c=` in every ladder manifest.** All 540 sets (site-50 and site-38) record
     `HEAD=0dfad3c l.c=ab59ea41c56f1b5b8376c0a3a3eb2882`. `0dfad3c:l.c` has md5 `b202f3b15a67523f7907da806ba3f22e`;
     `ab59ea41…` is `633420f:l.c`, the commit that introduced `NL_FORCE_SITE` (2026-08-18). The ladder ran on
     a working copy later committed as `633420f` while `HEAD` was taken from the checkout of the moment. A
     verifier cross-checking `HEAD` against `l.c` returns invalid on the whole ladder. Rule for v2: the
     manifest carries the hash of the source the binary was built from, and `HEAD` only when that commit
     contains that hash.
   - **one source, two binaries.** With identical `l.c=ab59ea41…`, site-50 rungs carry
     `bin=b37cd9d3652128a01363e08e6c794f6d`, site-38 rungs `bin=cf37114b8532a2a4bf9c04cb5f5becef`. Binaries
     are not committed and no build line is recorded, so which build carried what is not recoverable from
     the repo. Rule for v2: one `bin` per ladder, or build command + flags + hash in the manifest.
   - a formal Layer-1 row for this axis needs site-38 rungs around the boundary (0.8/1/1.5) with
     `force_site` written into every manifest before the run.
6. **Terminal without a reproduction comparator.** "A second hand reproduces the record" — which integers
   must be equal, which floats within what tolerance. The dissolved §12 had this; without it the terminal
   is not machine-checkable.
7. **The typed row has no file.** Precedent exists in the tree (`COURT_FREEZE.tsv`): `instrument.tsv`
   (Layer 1, one check per line, keyed per Sol's finding 1 to instrument/axis/regime/readout) and
   `readings.tsv` (Layer 2, one axis per line).

Layer 3 raises no machine objection: a field that does not exist cannot be emitted.

## What I check on v2

That `instrument.tsv` and `readings.tsv` are executable from their fields alone: every reading in the
total law is selected by named numeric fields; the stream list for `control_identical` is enumerable; the
reproduction law names its integer and float comparators; manifests carry `force_site`, a source hash
consistent with `HEAD`, and a build record; `tests/verdict_raw/README.md` states the archive contents
and the provenance boundary above exactly.
