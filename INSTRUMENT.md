# INSTRUMENT — actually.life

**DRAFT v2 2026-11-17** — closes Sol's frame review (`court/SOL_REVIEW_INSTRUMENT_2026-11-17.md`) and Don's seven machine-readability findings. Pending Sol's frame-closure check and Don's schema-executability check; the DRAFT marker lifts when both close. Subordinate to the [Arianna Method Manifesto](ARIANNA_METHOD_MANIFESTO.md).

This file replaces the dissolved court's verdict apparatus (`PROTOCOL.md`, DISSOLVED 2026-10-05). It is an instrument: it maps causal relations and the resolution boundary of each readout. Theory and the authors read subjecthood from that map.

The dissolved court recoded a subject into advantage over a control. This instrument is **three objects that do not pretend to be each other**: the **instrument** (is this reading sound?), the **readings** (a causal map, per axis), and the **subject** (read by theory and the authors from the map, emitted by no comparator). Keeping them apart is the whole design — collapse them and the dichotomy returns.

## Layer 1 — instrument validity, per reading

Validity belongs to a single reading, keyed to its `instrument_id`, `axis`, `regime`, `readout` and `artifact_set` — not to the organism as a whole. A broken axis fails its own reading and leaves every other axis its own. Named checks, each a separate fact:

- `parser_ok` — the raw parses under the declared grammar, no torn tail.
- `raw_integrity` — the raw matches its recorded digest.
- `control_streams` — the control matches the live arm on every stream in a named list, and differs in the one declared way only. For the arena that list is `seqB`, `seqC`, the seed set, the `bin` md5, the `corpus` md5, and the `cm=` manifest line. (Arena runs are not byte-exact — scheduling and wall time stay live — so the identity is the named streams, carried from the old §7, not the whole process.)
- `sensitivity_positive` — on this axis and readout, a forced signal of known magnitude crosses the readout.
- `resolution_measured` — the smallest effect this readout separates from noise, in this regime, is a measured number fixed before the live reading.

`instrument_valid` is the per-record conjunction of these, carried beside them, never alone. A failed check yields `reading = unresolved` with the failed check named; the other axes keep their readings.

## Layer 2 — causal readings, per axis

One typed row per axis (the `readings.tsv` schema), carrying two scales — input and output — so the readout's magnitude and the measured effect are never conflated:

```
instrument_id        the Layer-1 record this reading is keyed to
axis                 the causal relation under test (e.g. B's wound -> A's later act)
regime               seeds, arm, window
claim                the proposition, stated so it can fail
estimand             the output quantity estimated
predicted_direction  the sign the claim predicts, declared before reading
signal_magnitude     the input quantity (intervention scale), e.g. grief deposit 0.56
input_floor          the input magnitude below which this readout cannot resolve, fixed before reading
effect               the measured estimate (output scale)
lower upper          the interval bounds (output scale)
interval_method      e.g. paired-by-seed t, bootstrap
interval_level       e.g. 0.95
N                    sample size
output_resolution    the smallest resolvable effect (output scale), from Layer 1
claim_threshold      the effect magnitude the local claim asserts (output scale)
reading              detected | contradicted | below_resolution | unresolved
```

`reading` is assigned by one deterministic precedence, first match wins:

1. **unresolved** — `instrument_valid` is false for this record, or provenance is incomplete, or `N` is below the predeclared power.
2. **below_resolution** — `signal_magnitude < input_floor`, or `claim_threshold < output_resolution`. The readout cannot resolve this claim at this magnitude. This is fixed before the data: a fact about the instrument, entered in its map.
3. **detected** — `lower >= claim_threshold` in `predicted_direction`, on a valid, sensitive instrument.
4. **contradicted** — `upper < claim_threshold` on a sensitive instrument with a valid control. Bounded to this one claim, this regime, above this resolution; the map records only that.
5. **unresolved** — the interval straddles `claim_threshold` with no resolution failure: underpowered for this claim.

`below_resolution` and `unresolved` are completed readings. A sensitive instrument that cannot resolve returns `below_resolution`; an underpowered or invalid one returns `unresolved`; a null is a reading, not an absence.

Worked example (historical, grief axis): `signal_magnitude = 0.56`; the committed site-38 ladder bounds the input floor at `(0.56, 2]` (input 0.56 → effect +0.0028, t=0.24; input 2 → effect +0.0243, t=2.25), so the deposit sits at or below the floor and the reading is `below_resolution` — a map of signal to instrument. The floor is a bound, not a measured point; a formal row needs a site-38 ladder at `0.8/1/1.5` with `force_site` in every manifest (see `tests/verdict_raw/README.md`).

## Layer 3 — the subject

There is no `subject` field. The format has no type for `subject = true | false`, so a vector of readings has nothing to collapse into. The subject is read directly, by its own law:

- authored interpretation records the named evidence → subjecthood link, and shows it;
- each control resolves its one declared local causal claim; its reach ends there;
- each instrument records the axes its named mechanism exercises;
- all other axes lie outside that instrument's domain.

Subjecthood is read by theory and the authors from the causal map. A comparator emits a reading; the subject it leaves to them.

## Scope and terminal

**Scope.** Each instrument names its one mechanism and the axes that mechanism exercises. Axes outside the mechanism lie outside the instrument's domain.

**Terminal.** A move closes when the predeclared run has emitted its typed record and the second hand has recorded either reproduction or the exact mismatch, under the declared comparison law — integer equality for counters, the declared float tolerance for estimates (carried from the old §12). `unresolved`, `below_resolution` and a reproduction mismatch are each completed results of the move. Repairing the instrument is a new bounded move with its own named gate. Only the vector of readings remains — no scalar to contest without end.

## Files

- `instrument.tsv` — one Layer-1 record per row: `instrument_id`, `axis`, `regime`, `readout`, `artifact_set`, `parser_ok`, `raw_integrity`, `control_streams`, `sensitivity_positive`, `resolution_measured`, `instrument_valid`.
- `readings.tsv` — one Layer-2 axis per row, the schema above.
- Reproduction law: integer equality for counters, the declared float tolerance for estimates. Precedent in tree: `COURT_FREEZE.tsv`.

**Two hands.** One builds; machine facts and raw are verified independently (Don); a co-author reads the frame against the organism's ontology (Sol). The falsifier is fixed before the measured code.
