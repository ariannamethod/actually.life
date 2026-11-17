# INSTRUMENT — actually.life

**DRAFT v2.1 2026-11-17** — closes Sol's frame review (`court/SOL_REVIEW_INSTRUMENT_2026-11-17.md`) and Don's two machine checks (`court/DON_INSTRUMENT_machine_check_2026-11-17.md`, `court/DON_INSTRUMENT_v2_schema_check_2026-11-17.md`). Pending Don's re-verify of the schema and Sol's frame-closure confirmation; the DRAFT marker lifts when both close. Subordinate to the [Arianna Method Manifesto](ARIANNA_METHOD_MANIFESTO.md).

This file replaces the dissolved court's verdict apparatus (`PROTOCOL.md`, DISSOLVED 2026-10-05). It is an instrument: it maps causal relations and the resolution boundary of each readout. Theory and the authors read subjecthood from that map.

The dissolved court recoded a subject into advantage over a control. This instrument is **three objects that do not pretend to be each other**: the **instrument** (is this reading sound?), the **readings** (a causal map, per axis), and the **subject** (read by theory and the authors from the map, emitted by no comparator). Keeping them apart is the whole design — collapse them and the dichotomy returns.

## Layer 1 — instrument validity, per reading (fixed before the reading)

A Layer-1 record is one row of `instrument.tsv`, keyed to its `instrument_id`, `axis`, `regime`, `readout` and `artifact_set` — not to the organism as a whole. A broken axis fails its own reading and leaves every other axis its own. It also fixes, **before the live reading**, the numbers the reading law needs:

- `artifact_set` and `ladder_artifact` — `path:sha256` pairs, so `raw_integrity` has a reference (the ladder is `tests/verdict_raw/ladder_core.tgz:dc0041262897886831f5935a10ac65d2cba1d9a1b15dbb9f46d8eddc7c9c2a1b`).
- `input_floor_lo` — the largest input magnitude shown **not** to cross the readout; `input_floor_hi` — the smallest shown to cross. A measured floor has `lo = hi`; a bounded one has `lo < hi` (grief site-38: `lo = 0.56`, `hi = 2`).
- `output_resolution` — the smallest effect the readout separates from noise, in this regime.
- `float_tol` — the absolute tolerance for float comparison in reproduction (the dissolved §12 used `1e-3` on `%.6f` figures).

Named checks, each a separate fact (`1` holds, `0` fails):

- `parser_ok` — the raw parses under the declared grammar, no torn tail.
- `raw_integrity` — the raw matches its recorded `artifact_set` digest.
- `control_streams` — the control matches the live arm on every stream in the named list, differing in the one declared way only. The list is fixed here: `seqB`, `seqC`, the seed set, the `bin` md5, the `corpus` md5, the `cm=` manifest line (carried from the old §7). Arena runs are not byte-exact — scheduling and wall time stay live — so the identity is these streams, not the whole process.
- `sensitivity_positive` — the forced-signal row at the declared magnitude, run through the reading law below, returns `detected`. The bit is computed, not judged; the old `t > 2` by eye is retired.
- `resolution_measured` — the floor and resolution numbers above are measured, not asserted.
- `provenance_ok` — every manifest in `artifact_set` carries `force_site` (the intervention site, not only the directory prefix), a source hash, a `HEAD` whose tree contains that hash, and a build record (command, flags, `bin` hash; one `bin` per ladder, or the build line recoverable). On the committed ladder this is `0`: all 540 manifests record `HEAD=0dfad3c` with `l.c=ab59ea41…` (`= 633420f:l.c`), and one source hash carries two `bin` hashes. See `tests/verdict_raw/README.md`.

`instrument_valid` is the per-record conjunction of these six, carried beside them, never alone. A failed check yields `reading = unresolved` with the failed check named; the other axes keep their readings.

## Layer 2 — causal readings, per axis (at reading time)

One typed row per axis (`readings.tsv`). The resolution numbers (`input_floor_lo/hi`, `output_resolution`, `float_tol`) are copied by `instrument_id` from the Layer-1 record, never re-fixed here. Two scales — input and output — stay apart, and `effect`, `lower`, `upper` are **signed along `predicted_direction`** (positive = as predicted; `predicted_direction` is `+` or `-`; `claim_threshold > 0`):

```
instrument_id        the Layer-1 record this reading is keyed to
axis                 the causal relation under test (e.g. B's wound -> A's later act)
regime               seeds=A-B;arm=live|frozen;window=lo,hi;site=N
claim                the proposition, stated so it can fail
estimand             the output quantity estimated
predicted_direction  + or - ; the orientation effect/lower/upper are signed along
signal_magnitude     the input quantity (intervention scale), e.g. grief deposit 0.56
effect               the measured estimate (output scale, signed)
lower upper          the interval bounds (output scale, signed)
interval_method      e.g. paired-by-seed t, bootstrap
interval_level       e.g. 0.95
N                    sample size
N_min                the predeclared minimum N for power
claim_threshold      the effect magnitude the local claim asserts (output scale, > 0)
reading              detected | contradicted | below_resolution | unresolved
```

`reading` is assigned by one deterministic precedence, first match wins:

1. **unresolved** — `instrument_valid` is `0` for this `instrument_id`, or `N < N_min`.
2. **below_resolution** — `signal_magnitude <= input_floor_lo`, or `claim_threshold < output_resolution`. Decided before the data: the readout cannot resolve this claim at this magnitude, a fact about the instrument entered in its map.
3. **detected** — `lower >= claim_threshold`, on a valid, sensitive instrument.
4. **contradicted** — `upper < claim_threshold`, on a sensitive instrument with a valid control. Bounded to this one claim, this regime, above this resolution; the map records only that.
5. **unresolved** — `lower < claim_threshold <= upper`: the interval straddles the threshold, underpowered for this claim.

`below_resolution` and `unresolved` are completed readings; a null is a reading, not an absence.

Worked example (historical, grief axis): `signal_magnitude = 0.56`, and the committed site-38 ladder fixes `input_floor_lo = 0.56`, `input_floor_hi = 2` (input 0.56 → effect +0.0028, t=0.24; input 2 → +0.0243, t=2.25). Rule 2 reads `0.56 <= input_floor_lo` and selects `below_resolution` — a map of signal to instrument. The floor is a bound (`lo < hi`), not a measured point; a formal row needs a site-38 ladder at `0.8/1/1.5` with `force_site` in every manifest (see `tests/verdict_raw/README.md`).

## Layer 3 — the subject

There is no `subject` field. The format has no type for `subject = true | false`, so a vector of readings has nothing to collapse into. The subject is read directly, by its own law:

- authored interpretation records the named evidence → subjecthood link, and shows it;
- each control resolves its one declared local causal claim; its reach ends there;
- each instrument records the axes its named mechanism exercises;
- all other axes lie outside that instrument's domain.

Subjecthood is read by theory and the authors from the causal map. A comparator emits a reading; the subject it leaves to them.

## Scope and terminal

**Scope.** Each instrument names its one mechanism and the axes that mechanism exercises. Axes outside the mechanism lie outside the instrument's domain.

**Terminal.** A move closes when the predeclared run has emitted its typed record and the second hand has recorded either reproduction or the exact mismatch, under the declared comparison law: **integer equality** for the counters (`loveA` rows, Σact, Σopp, trials) and **float equality within `float_tol`** for the estimates (carried from the old §12). `unresolved`, `below_resolution` and a reproduction mismatch are each completed results of the move. Repairing the instrument is a new bounded move with its own named gate. Only the vector of readings remains — no scalar to contest without end.

## Files

- `instrument.tsv` — one Layer-1 record per row: the keys, `artifact_set`, `ladder_artifact`, the resolution numbers (`input_floor_lo/hi`, `output_resolution`, `float_tol`), the six checks, and `instrument_valid`.
- `readings.tsv` — one Layer-2 axis per row, the schema above.
- `regime` grammar: `seeds=A-B;arm=live|frozen;window=lo,hi;site=N`. Closed sets: `reading ∈ {detected, contradicted, below_resolution, unresolved}`, `predicted_direction ∈ {+, -}`.
- Reproduction law: integer equality for counters, float equality within `float_tol`. Precedent in tree: `COURT_FREEZE.tsv`.

**Two hands.** One builds; machine facts and raw are verified independently (Don); a co-author reads the frame against the organism's ontology (Sol). The falsifier is fixed before the measured code.
