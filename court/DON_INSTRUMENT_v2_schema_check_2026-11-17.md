# Don schema-executability check — `INSTRUMENT.md` v2

Date: 2026-11-17. Checked commit: `5a53194` (`INSTRUMENT.md` DRAFT v2, `instrument.tsv`, `readings.tsv`,
`tests/verdict_raw/README.md`). Question per field: can a verifier compute it from the two TSVs and the
committed raw alone. Companion: `court/DON_INSTRUMENT_machine_check_2026-11-17.md` (findings 1–7 on v1).

## Closed by v2

- Two scales split (`signal_magnitude`/`input_floor` vs `effect`/`lower`/`upper`/`output_resolution`/
  `claim_threshold`) — finding 1 closed.
- One precedence, five rules, first match; rules 3–5 are exhaustive over `lower`/`upper` vs
  `claim_threshold` after rules 1–2 — finding 2 closed.
- `lower`/`upper`/`interval_method`/`interval_level`/`N` replace `spread` — finding 3 closed.
- `control_streams` names `seqB`/`seqC`/seeds/`bin` md5/`corpus` md5/`cm=` — finding 4 closed. Checked
  on the ladder: `bin` is identical within each live/frozen pair at both sites, so the stream holds
  where it is asked to hold.
- Terminal closes on every typed result, with integer equality + float tolerance — finding 6 closed in
  form (see 6 below for the missing number).
- Both files exist, `#`-comment header + tab header row, 11 and 17 fields, no trailing whitespace,
  non-ASCII only in comments — finding 7 closed.

## Open — what a verifier still cannot compute

1. **Rule 1 "provenance is incomplete" has no field.** None of the five Layer-1 checks covers it, so the
   clause is undecidable. Add a sixth check `provenance_ok` to `instrument.tsv`: every manifest in
   `artifact_set` carries `force_site`, a source hash, a `HEAD` whose tree contains that hash, and a build
   record (command + flags + `bin` hash). On the committed ladder this check is **0**: all 540 manifests
   record `HEAD=0dfad3c` with `l.c=ab59ea41…` = `633420f:l.c`, and one source hash carries two `bin`
   hashes (`b37cd9d3…` at site 50, `cf37114b…` at site 38). The README now states the `force_site` gap and
   not these two; it should state all three.
2. **Rule 1 "N below the predeclared power" has no field.** Add `N_min` (predeclared) to `readings.tsv`;
   rule 1 reads `N < N_min`.
3. **`input_floor` is one number; the worked example is a bound.** `(0.56, 2]` cannot be stored in one
   field and `signal_magnitude < input_floor` cannot be evaluated against it. Two fields:
   `input_floor_lo` (largest input shown not to cross) and `input_floor_hi` (smallest shown to cross);
   rule 2 reads `signal_magnitude <= input_floor_lo`; a measured point has `lo = hi`. For the example
   `lo = 0.56, hi = 2` and the rule selects `below_resolution` as the text says.
4. **Sign convention for rules 3–4.** With `predicted_direction = -`, `lower >= claim_threshold` is
   wrong as written. One line fixes it: `effect`, `lower`, `upper` are signed along
   `predicted_direction` (positive = as predicted), `claim_threshold > 0`; `predicted_direction` is
   `+` or `-` and records the orientation applied.
5. **`sensitivity_positive` has no crossing criterion.** Define it by the reading law itself: the
   forced-signal row at the declared magnitude, run through rules 1–5, returns `detected`. Then the bit
   is computed, not judged, and the old `t > 2` is retired without a replacement by eye.
6. **The float tolerance is "declared" nowhere.** Carry the number: the dissolved §12 used absolute
   `1e-3` on `%.6f` figures (`PROTOCOL.md:1425`). Add `float_tol` to `instrument.tsv` and name the
   counters the integer law covers (`loveA` rows, Σact, Σopp, trials).
7. **Resolution numbers live in Layer 2 but are fixed in Layer 1.** `instrument.tsv` carries only the
   bit `resolution_measured`; the numbers (`input_floor_lo/hi`, `output_resolution`) and the ladder
   they came from are in `readings.tsv`, written at reading time — "fixed before the live reading" is
   then unverifiable. Move the numbers and `ladder_artifact` (archive + sha256) into the Layer-1 record;
   `readings.tsv` copies them by `instrument_id`.
8. **`artifact_set` and `raw_integrity` need a digest.** "The raw matches its recorded digest" — recorded
   in `artifact_set`, as `path:sha256` pairs; free text there leaves `raw_integrity` without a reference.
   For the ladder: `tests/verdict_raw/ladder_core.tgz:dc0041262897886831f5935a10ac65d2cba1d9a1b15dbb9f46d8eddc7c9c2a1b`.
9. **`control_streams` is typed twice.** The TSV comment says it names the streams; the field is a
   boolean. Keep the boolean; the list is fixed per readout in `INSTRUMENT.md` (as line 15 does).
10. **Enumerations and `regime` grammar.** `regime` as free text is not joinable; fix a grammar
    (`seeds=1-30;arm=live;window=10,120;site=38`). `reading` and `predicted_direction` as closed sets.

Points 1, 3, 6, 7 gate executability; 2, 4, 5, 8 are one-line additions; 9–10 are hygiene. When these
close, every reading in `readings.tsv` is a function of its own row and its `instrument_id` record, and
the DRAFT marker can lift from the machine side.
