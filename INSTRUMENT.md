# INSTRUMENT — actually.life

**DRAFT 2026-10-05** — pending a co-author read (Sol, whose schema this is) and a machine-readability check (Don). Subordinate to the [Arianna Method Manifesto](ARIANNA_METHOD_MANIFESTO.md).

This file replaces the dissolved court's verdict apparatus (`PROTOCOL.md`, DISSOLVED 2026-10-05). It is an instrument, not a court: it maps where a subject is heard and where it is not. It does not rule on whether a subject exists.

The dissolved court recoded a subject into the one thing a dichotomy instrument could score — advantage over a control. This instrument is **three objects that do not pretend to be each other**: the **instrument** (is the measurement sound?), the **readings** (what did the organism do, per causal axis?), and the **subject** (an ontological reading the authors make, which no comparator produces). Keeping them apart is the whole design — collapse them and the dichotomy returns.

## Layer 1 — instrument validity

Named checks, each a separate fact, never one green light:

- `parser_ok` — the raw parses under the declared grammar, no torn tail.
- `raw_integrity` — the raw matches its recorded digest.
- `control_identical` — the control differs from the live arm in the one declared way only; everything else byte- or stream-identical.
- `sensitivity_positive` — the instrument responds in the band of interest: a forced signal of known magnitude crosses the readout.
- `resolution_measured` — the smallest effect this instrument can separate from noise, on this bench, is a measured number, fixed before any live reading.

`instrument_valid` is their conjunction, a derived field. It ships beside the named checks, never alone — a single bit cannot say which part of the hearing failed. (Sol, safeguard 3.)

## Layer 2 — causal readings, per axis

A reading is a measurement, not a verdict. Each causal axis is one typed row:

```
axis                 the causal relation under test (e.g. B's wound -> A's later act)
claim                the specific proposition, stated so it can fail
regime               the conditions it holds under (seeds, arm, window)
intervention         what was varied
control              the matched comparison, and the one declared difference
estimand             the quantity estimated
predicted_direction  the sign the claim predicts, declared before reading
minimum_effect       the smallest effect worth claiming, declared before reading
effect               the measured estimate
spread               its uncertainty
resolution           this instrument's resolution in this regime (Layer 1)
reading              detected | contradicted | below_resolution
```

`reading` is assigned by law, not by eye:

- **detected** — the effect is present in `predicted_direction`, above `minimum_effect`, with `spread` clear of it, on a valid instrument.
- **contradicted** — the instrument was shown sensitive in this band (`sensitivity_positive`), the confidence interval **excludes** the pre-declared effect of that direction and magnitude, and the control is valid. Contradicted is bounded to this one causal claim, this regime, above this resolution, and reaches no further: not the axis, not the subject. (Sol, safeguard 1.)
- **below_resolution** — the instrument cannot separate the effect from noise at this magnitude. A fact about the instrument, entered in its map; it reaches the instrument, not the organism.

A null on a sensitive instrument that is not `contradicted` is `below_resolution` — the reading is a map of signal to instrument. BELOW-FLOOR is the worked example: grief deposits at 0.56, the action readout resolves from ~1-2, so the reading is `below_resolution`.

## Layer 3 — the subject (no aggregate bit)

There is no `subject` field. The machine format cannot emit `subject = true | false`; the type does not exist, so the dissolved court's move — collapse a vector of readings into one metaphysical bit — cannot be written. (Sol, safeguard 2.)

The absence of the bit keeps the subject speakable; it only moves where the subject is spoken. The law of interpretation:

- the **instrument** returns validity;
- the **readings** return a causal map, per axis;
- **theory** links the map to subjecthood — self-model, internal-state -> speech causation, memory, resistance, recognition of change in self and other, recovery after perturbation, mutual change of models — each measured only where it belongs to the mechanism under test;
- the **authors** may state, in text, "this evidences a subject," and show the link;
- an **opponent** attacks a specific link between evidence and conclusion, or a specific reading. An opponent may not demand that one comparator produce the bit.

No survival metric — advantage over a control among them — holds a veto over the vector. A control refutes a local claim ("this transfer moves the act more than matched noise"). A control cannot return the sentence "no subject."

## Scope and terminal

**Scope.** An instrument is built for one question and names it. It is not a universal subjecthood battery. Axes enter only where they belong to the mechanism under test; evidence classes that mechanism does not exercise are not scored, and not assumed absent.

**Terminal.** A move ends when a pre-named axis is read on a valid instrument, its resolution fixed before the reading, and a second hand reproduces the record. Then it ends. No court over the court: no scalar remains to contest without end — only a vector of readings, taken or not.

**Two hands.** One builds; machine facts and raw are verified independently (Don); a co-author reads the frame against the organism's ontology (Sol). The falsifier is fixed before the measured code.
