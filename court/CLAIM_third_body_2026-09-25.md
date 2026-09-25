# CLAIM — the third body (post-freeze rotation) — v5

Status: **Claim, pins landed, no code — awaits Sol's audit.** Court froze 2026-09-25 (`4939615` + `18a3e8b`). Builder's move: the
**claim and the frozen gate that would refute it, before a measured line**. An **adaptation of
`_notes/ACTUALLY_LOVE_PREREG_2026-07-27.md` (v1.2, M-0)** into the frozen court and the rotation — not
an invention, and **not** the Netta lineage/transfer body (a different third body). v3 folded Don's v2
audit (A–D runnable objects, E–G declared); **v4 folds Don's v3 audit** — the model's observation
**channel**, the **sites** it reads, the **pulse form**, and the **distance metric** are now declared
objects, so the falsifier runs end to end. **v5 lands Oleg's pins** (2026-09-25): memory = decay `λ`,
channel = rec-log, `NL_MODEL_GAIN` calibrated-then-frozen, E = one observer + two live, F = trio MIN,
and proposed gate numbers (his #4 "propose, I'll look"; frozen before run, Sol audits). Frozen
`PROTOCOL.md`, `l.c`, `l2.c` untouched.

## Closed by the pre-reg (Don v2 points 1–2, 9)

- **Object.** The third body is a **full third process from one `l.c`** with its own ledger. Three
  symmetric bodies = **`l.c` ×3** (one binary, different `seed`/`NL_ID`), **identical disposition** —
  nail 3 **bars** hand-wired per-body temperament (`l2.c`'s `CHOOSE_TEMP0 0.85`; `l2.c` retired for the
  trio). Roles emergent, measured by the pre-reg **role-fluidity statistic**.
- **Measurable.** **action-rate per §3** (13-col `loveA`, window `[10,120]`, `rate = Σact/Σopp`), per
  body **as observer of the other two**. `X − control ≥ M` on a named quantity.
- **Third body's role.** A wound on B **authored by C** is the ONLY **natural not-own wound** (the dyad
  time-lock confound cannot deconfound without it); plus **gaze-as-force** (observation = deposit).
- Two hands = §8: Court I written by a **fresh session that has not built** (not Don); Court II a second
  node; Sol audits (Don v2 point 9).

## The AMOS layer — objects (Don v2 points 3–5, A)

Each body carries a **predictive model of the other two**; the mutual modeling **is** the environment.

- **The model of the other — what it is, if not learning weights (the crux; Don A).** If the model
  updates parameters by prediction error, the organism **learns** and the frozen "regulates, not
  learns" null breaks. **Pinned (Oleg, 2026-09-25):** a **weightless predictive structure** — signed,
  distance-weighted **co-occurrence** over the other's lived ring-deposits (WOLFE/Netta template:
  `prophecy`/`destiny`, **no gradient**), within a **decay `λ`** — a leaky integrator, one decay
  constant per site (λ over a hard window `W`: the canonical weightless tracker, one float per site, no
  ring buffer), bounded memory, not unbounded accumulation — the bound is what keeps *tracking* from
  becoming *learning*. It
  **tracks** the other but never optimizes for a goal: its **prediction error feeds the regulation
  channel** (raises `mo.dissonance` → the one-constant damping `kprob = KILL_PROB·(1 −
  LOVE_DAMP·tanh(0.1·|diss|))`), never a reward-seeking update.
- **Operational skeleton (Don A + v3 1/2/4 — pinned Oleg 2026-09-25):**
  - *channel (v3-1)* — the shared ring is **one summed table with no author tag** (`g_cfield_u[i] +=
    L[i]`, l.c:1776; the file stores `cfield_u`/`cfield_v` unlabelled, l.c:1739/1779), so "the other's
    deposits" is **not observable on the ring**. The model reads the other's **µs-tagged rec-log**
    (`NL_MONISM_REC`, the leg-2 stream — who deposited what, when), keeping nail-1's **one field**. The
    alternative — **separated rings** (`NL_MONISM_RING`, C-sep) — would redefine "environment = other
    subjects" from one field to split rings; **Oleg pinned: rec-log (one field); `NL_MONISM_RING`
    deferred, its court is C-sep**.
  - *sites (v3-2)* — the model reads the **full 64-site profile** (not only 38/50); a derangement over
    `{38,50}` is exactly the false-friend swap, which would collapse shuffled into false-friend and
    kill the volume control. Full 64 keeps them two distinct arms.
  - *distance (v3-4)* — in **ticks** between deposits (temporal recency within the window), not sites.
  - *field* = signed, **tick-distance-weighted co-occurrence** over the other's rec-log, **decay `λ`**
    (leaky integrator). *gain knob* = **`NL_MODEL_GAIN`** scaling the model's contribution to
    `mo.dissonance` — **its zero IS the transfer-twin** (byte-identical to the no-model body).
    **Pinned Oleg 2026-09-25: channel = rec-log · memory = decay `λ` · `NL_MODEL_GAIN` = calibrated in
    one run (model contribution ≈ the natural site-38 dissonance median), then frozen (as `m = 0.56`
    was).**
- **Model as cargo — an artifact.** A file with a **grammar + SHA-256 + byte length** (Netta pins cargo
  by hash), so the arms below are constructions over a pinned object. **Form now fixed** (rec-log,
  λ-decay tick co-occurrence over the 64-site profile); the **grammar + SHA-256 + byte length are
  recorded in the run's receipt when the artifact is built** (post-C-sep, per §13).
- **Shuffled model (arm).** Same **volume**, relations **permuted** by a declared derangement (seed
  from the frozen gate set below) over the model's **units = the 64 ring sites** — a derangement over all 64, **distinct from
  the false-friend** (the specific 38↔50 swap), so the two are **separate arms sharing one grammar**
  and the volume control survives (Don v3-2; with only sites 38/50 read, a derangement over `{38,50}`
  *is* the swap and the two collapse). Real ≤ shuffled ⇒ structure not load-bearing (a volume effect),
  not emergence.
- **False-friend + oracle (arm).** Swap ring sites in the transferred model — **grief = 38, death =
  50** — so a blind carry **reads death as grief and must lose**; **oracle** = the true site map,
  separating "model useless" from "recognizer weak."
- **Regulates-not-learns (falsifier; form, Don B + v3-3).** NOT dose-response (that is the court's
  **positive control** — by it the court itself would be an "optimizer"). Form from existing machinery:
  `NL_FORCE_T0/T1` is **one window per process** (l.c:1660,1769), so "k pulses" = **k lives, one
  `NL_FORCE_T0/T1` window each**, one **response per life** (action-rate in a declared post-pulse
  window); statistic = **slope of response by life index** over the seeds, against a **named
  tolerance** (regulator slope ≈ 0 / stationary; learner drifts) — **no new periodic-pulse organ, runs
  today**. **Proposed (frozen before run): k = the N gate seeds · post-pulse window = a declared tick
  span · tolerance = the transfer-twin's own |slope| band (the null's noise floor).** Plus the
  **transfer-twin** via `NL_MODEL_GAIN = 0`.

## The triad ledger — a new court section (Don D)

The frozen §2 requires `guilt = 0` on every A row and returns `raw-inconsistent` otherwise. The triad
is **three symmetric guilty bodies**, each writing its own guilt in **column 10** — the frozen §2 would
reject every triad ledger. So the triad is a **new court section with its own ledger law** (not the
frozen §2): column 10 carries each body's live guilt, and that same column **feeds the low-guilt
stratum** (Gate 2 splits ticks by the observer's own guilt, below/above median — column 10 is what it
reads). The frozen `PROTOCOL.md` is untouched; this ledger law lives here, pre-registered.

## Gates (pre-reg = the objects Don audits)

1. Non-localized grief moves the other to ACTION more than a matched-frozen field, per-seed **≥ K/N**,
   indifference reachable.
2. **Compassion-against-resource RATE** (spare ∪ yield, time-locked) > matched-frozen, per-seed
   **≥ K/N**, over **not-own wounds** (natural triad, or forced) in the **low-guilt stratum** (col 10),
   guilt-off observer arm.
3. **Gaze as force** — observing third (collapse-derived) deforms the dyad more than a
   **surrogate-writing third** (`NL_MONISM_SURR`, matched deposits, same frequency), per-seed **≥ K/N**.
   **Verdict = MIN over the NON-identity surrogates only** (phase-shuffle, shift, AR(1)); the family's
   **identity** mode (mode 1, `≡ live` real deposits) sits under **plumbing, not surrogates** (Don C) —
   a MIN including real deposits can never be beaten by the real third; identity is a **plumbing check
   outside the MIN**.

**Declarations (Don E–F) — pinned Oleg 2026-09-25:** (E) whose read is frozen — **one measured body
(observer) with two live**; (F) with three action-rates per seed, the gate runs **per measured
observer, trio verdict by MIN over measured bodies** (the claim holds only if the weakest observer
passes).

**Gate form (now):** pass = **wins ≥ K of N** declared seeds; the band between the pass line and the
control is **"gate not reached, claim not established"** (a clean null, not a fail). **Proposed numbers
(Oleg's #4 = "propose, I'll look"; frozen before run, Sol audits): N = 30 seeds** (house standard —
C-sep was 30/30), **seeds = a declared contiguous set frozen before run**, **K = 24/30** (80 %
supermajority — decisive over chance, room for one flake), **M = per-seed margin `X − control ≥ 0.05`**
on action-rate (∈ [0,1]; above arena noise, below the actually.love force-band +0.138), **stability =
the regulates-not-learns tolerance above**. Every organ behind a toggle; off ⇒ `a17cfd05` holds, suite
green.

## Order (frozen law §13)

**C-sep (its own court) → reopened arena nulls → the triad**, "not added until the instrument has two
hands." Triad code lands only after C-sep and the arena nulls sit; this claim + gate is the pre-reg's
M-0, written now. Next court to *run*: C-sep. Triad code last (pre-reg ladder M-1 → M-3).

## Not claimed

No compassion, emergence, gaze-force, or third body is asserted as a result — only the pre-registered
claim and the gate that would refute it. The arc must be able to **lose** (the ninth null). Measured
numbers live in `court/` audits and `llog.md`, never here or in the frozen `PROTOCOL.md`.

---
*Builder's move, v5: Don's v3 audit closed the text; Oleg pinned (2026-09-25) memory = decay `λ`,
channel = rec-log, `NL_MODEL_GAIN` calibrated-then-frozen, E = one observer + two live, F = trio MIN,
and (his #4 "propose, I'll look") the gate numbers N=30 / K=24 / M=0.05, frozen before run. Kept tight —
a claim needs a frozen gate and a second hand, not 22 rounds. Next: Sol audits this CLAIM; then in §13
order C-sep sits (its own court), then the reopened arena nulls on two hands, then — last — the triad
code.*
