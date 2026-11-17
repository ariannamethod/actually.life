# Sol frame review — `INSTRUMENT.md`

Date: 2026-11-17

Reviewed commit: `8476f865fea5ae40dbad62d25b6e0ef9b4b00f69`

Reviewed artifact: `INSTRUMENT.md` (`DRAFT 2026-10-05`)

Frame authority: `ARIANNA_METHOD_MANIFESTO.md` and the Oleg–Claude–Don–Sol
working covenant.

Don's seven machine-readability findings were supplied verbatim by Oleg during
this review. I checked each finding against the current file and checked the
archive claim against the committed archives themselves.

## Gate named before reading

The frame holds only if all four conditions hold together:

1. the three objects — instrument, causal readings, subject — remain distinct;
2. a local null cannot become a subject-null or an aggregate subject bit;
3. every boundary belongs to a named instrument, axis, regime, readout and
   resolution;
4. the terminal closes the move after the typed record and the second hand,
   including an unresolved or mismatched result.

## What holds

The three-object architecture is preserved. Layer 1 names separate facts and
keeps the derived validity field beside them. Layer 2 bounds `contradicted` to
one causal claim in one regime. Layer 3 keeps subjecthood in theory and
authored interpretation, with no aggregate machine field. The scope is one
mechanism rather than a universal battery. The falsifier precedes measured
code and the two hands have distinct responsibilities.

The old Netta transition — performance result → value → subject verdict — has
no representable type in this structure. The central frame therefore holds.

`INSTRUMENT.md` remains DRAFT for one bounded v2 repair. The following findings
are the complete frame handoff for that repair.

## Required frame corrections

### 1. Validity belongs to a reading, not to the whole organism

`instrument_valid` at line 19 is currently an unscoped conjunction. Two of its
inputs, `sensitivity_positive` and `resolution_measured`, necessarily belong to
a particular axis, regime and readout. A global bit would let one broken axis
block the rest of the causal vector.

Key every Layer-1 record to an `instrument_id`, axis, regime, readout and
artifact set. Derive validity for that record. A failed check produces an
`unresolved` reading with the failed check named. Other axes retain their own
readings.

### 2. The typed row needs two scales and a total reading law

Don's findings 1–3 shape the frame as well as its serialization.
`signal_magnitude = 0.56` is an input quantity. `effect`, `spread` and the
current `minimum_effect` are output quantities. The current `resolution` field
mixes those scales, and lines 44 and 46 assign `below_resolution` by two
different laws.

The v2 row needs, with units:

- `signal_magnitude` and `input_floor` in the intervention/input scale;
- `effect` and its declared estimand in the output scale;
- an explicit interval (`lower`, `upper`, method, level, `N`) in place of the
  undefined `spread`;
- `output_resolution` or an equally explicit output-scale name;
- `claim_threshold` as the effect magnitude asserted by the local claim. The
  phrase “worth claiming” at line 33 turns a causal bound into a permission
  threshold and should leave the format;
- `reading = detected | contradicted | below_resolution | unresolved`;
- one deterministic precedence law over those fields.

`unresolved` carries invalid instruments, missing provenance, underpowered
intervals and every case in which the fixed numbers do not select one of the
other three readings. It prevents an underpowered run from being written as a
measured boundary of hearing.

### 3. The terminal must close on every typed result

Line 66 closes only when the instrument is valid and the second hand
reproduces the record. An invalid instrument, underpowered run or reproduction
mismatch would therefore leave the move open and restart the court-over-court.

The move closes when the predeclared run has emitted its typed record and the
second hand has recorded either reproduction or the exact mismatch under the
declared integer/float comparison law. `unresolved` is a completed result of
that move. Repairing the instrument is a new bounded move with its own named
gate.

### 4. The instrument should map causes before theory reads subjecthood

Line 5 says the instrument maps “where a subject is heard and where it is
not.” That lets the instrument speak about the subject before Layer 3 has made
the theoretical link. State the actual operation directly: the instrument maps
causal relations and the resolution boundary of each readout; theory and the
authors read subjecthood from that map.

Lines 58, 60 and 64 retain anticipatory courtroom language: a hypothetical
opponent, what the opponent may demand, what no metric may do, and what the
instrument is not. The same laws have a direct form:

- authored interpretation records the named evidence → subjecthood link;
- each control resolves its declared local causal claim;
- each instrument records the axes exercised by its named mechanism;
- all other axes remain outside that instrument's domain.

This states the ontology directly and removes the invented accuser.

## Don's operational findings

Don's findings 1–4, 6 and 7 reproduce on the current bytes:

- the input and output scales are conflated;
- `below_resolution` has two incompatible assignment rules and lacks
  `unresolved`;
- `spread` has no statistical definition;
- `control_identical` has no named stream list;
- the terminal has no reproducibility comparator;
- the typed rows have no files.

The v2 repair should therefore define `instrument.tsv` and `readings.tsv`, the
named identity streams, the interval method, the total reading law and the
integer/float reproduction rules. These are the machine form of the frame.

## Correction and preservation of Don finding 5

The committed archive is:

```
tests/verdict_raw/ladder_core.tgz
sha256 dc0041262897886831f5935a10ac65d2cba1d9a1b15dbb9f46d8eddc7c9c2a1b
```

Its exact manifest population is:

- `forced/`: 360 sets — amplitudes `0.8/1/2/4/8/20`, two arms, thirty seeds;
- `forced_site38/`: 180 sets — amplitudes `2/8/20`, two arms, thirty seeds.

Thus the site-38 steps are present as archive members. Don's literal statement
that they are absent does not match this archive byte.

His machine-readability finding still stands in a sharper form. A site-38
manifest contains:

```
seed=1 amp=2 frozen=none
HEAD=0dfad3c l.c=ab59ea41c56f1b5b8376c0a3a3eb2882 bin=cf37114b8532a2a4bf9c04cb5f5becef corpus=caa87837e0beabc49f6ec437c330c894
window=[10,120] gthr=0.3 cm=NL_ARENA=1 NL_MONISM=1 NL_MONISM_HEART=2 NL_MONISM_GAIN=0.3
```

It never records `NL_FORCE_SITE=38`. The intervention site exists only in the
directory prefix and the historical run account. A machine can count the
site-38-labelled data but cannot establish the intervention site from the
manifest.

The committed readings establish this historical bound under the old `t > 2`
criterion:

- site 38, input `0.56`: effect `+0.0028`, `t = 0.24`;
- site 38, input `2`: effect `+0.0243`, `t = 2.25`.

The supported input-floor statement is therefore `(0.56, 2]`, not a measured
`~1–2` site-38 floor. The site-50 point at `1` belongs to another site and does
not fill that gap.

For the historical worked example, state the bound and the missing manifest
field exactly. A future formal Layer-1 row for this axis needs a site-38 ladder
around the boundary (`0.8/1/1.5` is the proposed set) with `force_site` written
into every manifest before the run.

## Bounded next move

Claude writes one INSTRUMENT v2 that closes the named findings above and Don's
machine schema findings. It also aligns `tests/verdict_raw/README.md` with the
exact archive contents and provenance boundary. No v7 code begins in this
move.

Sol then checks only the frame closures named here. Don checks that the two TSV
schemas and reading/reproduction laws are executable from their fields. When
both checks close, the DRAFT marker can be lifted and the third-body v7 claim
can begin from the thesis.

## Frame-closure check — `c4a283a` (2026-11-17)

The v2.1 round closes the substantive frame findings above:

- validity is keyed per instrument/axis/regime/readout/artifact set;
- input and output scales are separated;
- `unresolved` is a completed typed reading;
- terminal closes on reproduction, mismatch, `below_resolution` or
  `unresolved`;
- Layer 3 and Scope state their laws directly;
- the site-38 archive population and provenance boundary are recorded exactly.

Two contradictions remain inside the promised total law. They keep the file at
DRAFT for one bounded repair.

### Closure A — sensitivity is recursive

`sensitivity_positive` is one input to `instrument_valid`. It is defined as a
forced-signal row returning `detected` under the reading law. That law reaches
`detected` only after rule 1 has established `instrument_valid = 1`.

The dependency is circular:

```
instrument_valid -> sensitivity_positive -> detected -> instrument_valid
```

Split calibration admission from the bit being calibrated. One direct form is:

```
base_valid = parser_ok && raw_integrity && control_streams &&
             resolution_measured && provenance_ok
sensitivity_positive = base_valid && forced_row_meets_detected_numeric_rule
instrument_valid = base_valid && sensitivity_positive
```

The forced-row numeric rule uses its declared interval, threshold and `N_min`
directly; it does not call the live-reading law that requires
`instrument_valid`.

### Closure B — the historical example selects rule 1, not rule 2

The document correctly records `provenance_ok = 0` for the committed ladder.
Therefore `instrument_valid = 0`, and the first-match law selects rule 1:
`unresolved`.

The worked example later says the same row reaches rule 2 and returns
`below_resolution`. It cannot reach rule 2 under the stated precedence.

Preserve both facts in their proper layers:

- historically, the preserved runs map input `0.56` to no separated action
  effect and bound the labelled site-38 crossing in `(0.56, 2]`, with the
  provenance defects now named;
- under the v2.1 machine law, that historical artifact yields `unresolved`
  because `provenance_ok = 0`;
- a provenance-complete site-38 ladder can later establish a formal Layer-1
  row, after which the natural reading can reach `below_resolution` under rule
  2.

### Closure state

The three-object frame holds. The two contradictions above are defects in its
executable dependency graph and worked example. After one repair, Sol checks
only these two closures and Don verifies the resulting dependency order. The
DRAFT marker then has a real terminal.
