# CLAIM — the third body (post-freeze rotation) — v6

Status: **Claim, no code.** Court froze 2026-09-25 (`4939615` + `18a3e8b`). Builder's move: the **claim and
the falsifier whose gate-form would refute it, before a measured line**. An **adaptation of
`_notes/ACTUALLY_LOVE_PREREG_2026-07-27.md` (v1.2, M-0)** into the frozen court and the rotation — not an
invention, and **not** the Netta lineage/transfer body (a different third body). The **gate form is frozen
here**; every value and selection rule lives in **`court/GATE_third_body.md`** (the executable contract,
written before M-1 code — not before this commit). Frozen `PROTOCOL.md`, `l.c`, `l2.c` untouched.

*Version trail:* v3 folded Don's v2 audit; v4 folded Don's v3 audit; v5 landed Oleg's pins; **v6 closes
Sol's v5 audit** — 9 findings, all genuine (Don's audit-of-the-audit, `court/AUDIT_sol_audit_third_body_don_2026-09-26.md`):
the model is now defined computationally (a **64×64 leaky-Hebb transition matrix** — a profile→profile map,
not a zero-order vector), the channel is
restated honestly against `l.c`, "regulates-not-learns" is narrowed to a **measurable invariant**, the twin
is split into **two named forms**, the cargo transform is made exact, Gate 2's symmetry is fixed, the
prior-result leaks are removed, and the executable gate contract (findings 7–8) moves to the GATE file.

## Closed by the pre-reg (Don v2 points 1–2, 9)

- **Object.** The third body is a **full third process from one `l.c`** with its own ledger. Three
  symmetric bodies = **`l.c` ×3** (one binary, different `seed`/`NL_ID`), **identical disposition** —
  nail 3 **bars** hand-wired per-body temperament (`l2.c`'s `CHOOSE_TEMP0 0.85`; `l2.c` retired for the
  trio). Roles emergent, measured by the **role-fluidity statistic** (named here, defined in the GATE file).
- **Measurable.** **action-rate per §3** (13-col `loveA`, window `[10,120]`, `rate = Σact/Σopp`), per
  body **as observer of the other two**. `X − control ≥ M` on a named quantity.
- **Third body's role.** A wound on B **authored by C** is the ONLY **natural not-own wound** (the dyad
  time-lock confound cannot deconfound without it); plus **gaze-as-force** (observation = deposit).
- Two hands = §8: Court I written by a **fresh session that has not built** (not Don); Court II a second
  node; Sol audits (Don v2 point 9).

## The AMOS layer — objects

Each body carries a **predictive model of the other two**; the mutual modeling **is** the environment.

- **The model of the other — what it is, if not learning weights (the crux; resolved).** The model of
  the other is a **transition**, not an average: "grief at 38 now, death at 50 next" is a **profile →
  profile map**, so the object is a **64×64 leaky-Hebb transition matrix `C`** (a zero-order λ-expectation
  *vector* was considered and dropped — it carries only the other's average profile, which the ring
  already gives, and leaves the derangement arm nothing to permute). Gradient-free, no hard window:
  - *update* (per complete rec row of the other): `C ← (1−λ)·C + λ·(L_next ⊗ L_now)` (leaky Hebbian
    outer product — bounded memory, **no gradient**);
  - *prediction* = `C·L_now`; *error* = `‖L_next − C·L_now‖`;
  - the error feeds the **regulation channel** — it raises `mo.dissonance` (the one-constant damping
    `kprob = KILL_PROB·(1 − LOVE_DAMP·tanh(0.1·|diss|))`), scaled by **`NL_MODEL_GAIN`** — **never a
    reward-seeking update**. `C` is **4096 floats**, **initialised to zeros and reset to zeros every
    life** (finding 3); the only cross-life carry is the **cargo** in the transfer arms.
  It **tracks** the other's transition within a life; whether that counts as "learning" is **not** the
  gate — the gate is the **exposure-index invariance** below.
- **Channel — one ring, author-labelled side-channels (Sol finding 2, corrected).** The shared ring is
  **one summed table with no author tag** (`g_cfield_u[i] += L[i]`, l.c:1776; `cfield_u`/`cfield_v`
  stored unlabelled, l.c:1739/1779), so "the other's deposits" is **not observable on the ring**.
  Topology, stated plainly: **one summed monism ring plus author-labelled rec side-channels.** The model
  reads the other's **rec-log** (`NL_MONISM_REC`); the rec row is `us` + 64 `%.6g` fields (l.c:1775) —
  **no in-record author and no discrete tick** — so **author = the file path** (`recB.log` ↔ `NL_ID=2`,
  `recC.log` ↔ 3, …), **tick = the 1-based row ordinal**, and the reader consumes **only complete,
  `\n`-terminated rows with NF = 65** (the frozen §7 grammar) — **never a torn tail**; `us` is a
  wall-clock µs stamp, not a clock. **Once the rec-log is read back into `mo.dissonance` it is mechanism,
  not measurement** — it is the same stream the C-frozen surrogate family manipulates, now also an input
  to regulation. This does **not** contradict the frozen court, where the rec-log stays measurement
  precisely because **nobody reads it back**; the triad is the first place it becomes mechanism.
- **Regulates-not-learns (falsifier — a measurable invariant; corrects the earlier form, Sol finding 3).**
  Drop the **word** "doesn't learn" from the gate: an online leaky-Hebb matrix **is** learning in the
  broad sense, and bounded memory does not change that. Gate the distinction that matters — **history**:
  a **regulator** answers the **current** prediction error; a **learner** answers also **how many times
  it has already seen it**. Measurable invariant: **at equal current error, the regulation magnitude does
  not depend on the exposure index.** Take within-life tick-pairs with equal current error, one at low and
  one at high exposure; a regulator gives **equal** magnitude (inside the transfer-twin's band), a learner
  gives **different**. `C` **resets to zeros every life in all arms**; the only carry is in the
  **transfer arms**, where carry is named carry, not learning. This one invariant closes the hole (the v3
  slope-by-life is dropped) and matches **WILL DESIGN**: the carrier is the **magnitude** of regulation,
  not its **history**. The band, the equal-error tolerance and the pairing rule are pinned in the GATE
  file.
- **Model as cargo — an artifact.** The matrix `C` (**4096 floats**, its **grammar + SHA-256 + byte
  length**) is pinned by hash when built, so the arms below are transforms over a pinned object. **Form
  now fixed** (a 64×64 leaky-Hebb transition over the other's rec-log); grammar + hash + length are
  recorded in the run's receipt at build (post-C-sep, per §13).
- **Cargo-derangement (arm; site permutation of the matrix).** Same **volume**, `C` **permuted over the
  site index on both axes**: `C_π[π(i), π(j)] = C[i,j]` (Sol's formula), a **deterministic derangement
  from a declared seed** with **`π(38) ≠ 50` and `π(50) ≠ 38`** (so it cannot coincide with the
  false-friend). Real ≤ cargo-derangement ⇒ structure not load-bearing (a volume effect), not emergence.
- **False-friend + oracle (arm).** The **specific `38 ↔ 50` swap only** — grief = 38, death = 50 — so a
  blind carry **reads death as grief and must lose**; **oracle** = the true site map, separating "model
  useless" from "recognizer weak."
- **Surrogate-shuffle (arm; temporal — distinct object and name).** `NL_MONISM_SURR` replays /
  phase-shuffles / AR(1)-matches the **rec-log stream in time** (the manipulated variable is only the
  foreign component; the own echo is untouched, l.c:1649). It is a **temporal** control and never shares
  a label with **cargo-derangement** (site).
- **Transfer-twin — two named forms (Sol finding 4, corrects the byte claim).** (a) **single-process
  pre-flight** in the §0 family: one body alone, model on at **gain 0** over a **pinned static rec
  input**, `waste.log` **byte-identical** to the no-model body — byte identity lives here **only**. (b)
  **arena twin**: ensemble-level — same plumbing, model contribution to `mo.dissonance` **exactly zero**,
  gate outcome equal in distribution; **no byte claim on the arena** (frozen §0: arena runs are not
  byte-exact — scheduling and wall time stay live). `NL_MODEL_GAIN = 0` selects the twin.

## The triad ledger — a new court section (Don D)

The frozen §2 requires `guilt = 0` on every A row and returns `raw-inconsistent` otherwise. The triad is
**three symmetric guilty bodies**, each writing its own guilt in **column 10** — the frozen §2 would
reject every triad ledger. So the triad is a **new court section with its own ledger law** (not the
frozen §2): column 10 carries each body's **live** guilt, and that same column **feeds the low-guilt
stratum** in Gate 2 (below/above median — column 10 is what it reads, and it is live because the
observer's guilt is **on** in the main arm; see Gate 2). The frozen `PROTOCOL.md` is untouched; this
ledger law lives here, pre-registered.

## Gates (pre-reg = the objects the auditors check)

1. Non-localized grief moves the other to ACTION more than a matched-frozen field, per-seed **≥ K/N**,
   indifference reachable.
2. **Compassion-against-resource RATE** (spare ∪ yield, time-locked) > matched-frozen, per-seed **≥ K/N**,
   over **natural triad** not-own wounds (the gate) in the **low-guilt stratum** (col 10) with the
   **observer's guilt ON** — so column 10 is live and the median split is real (Sol finding 6). The
   **guilt-off observer** is a **separate control arm without the split**; **forced** wounds are the
   **detectability control**, gated separately.
3. **Gaze as force** — an observing third (collapse-derived) deforms the dyad more than a
   **surrogate-writing third** (`NL_MONISM_SURR`, matched deposits, same frequency), per-seed **≥ K/N**.
   **Verdict = MIN over the NON-identity surrogates only** (phase-shuffle, shift, AR(1)); the family's
   **identity** mode (mode 1, `≡ live` real deposits) sits under **plumbing, not surrogates** (Don C) — a
   MIN including real deposits can never be beaten by the real third; identity is a **plumbing check
   outside the MIN**.

**Declarations (Don E–F) — pinned Oleg 2026-09-25:** (E) whose read is frozen — **one measured body
(observer) with two live**; (F) with three action-rates per seed, the gate runs **per measured observer,
trio verdict by MIN over measured bodies** (the claim holds only if the weakest observer passes).

**Gate form (frozen here; values in `court/GATE_third_body.md`).** pass = **wins ≥ K of N** declared
seeds; the band between the pass line and the control is **"gate not reached, claim not established"** (a
clean null, not a fail). The **form** is fixed here; **every value and selection rule — `λ`, the seed set,
the gain-calibration corpus / seeds / candidates / selection law, the post-pulse span, the slope
estimator, the tolerance-band formula, cross-life persistence, and the cargo grammar — is pinned in
`court/GATE_third_body.md` before any code** (Sol findings 7–8). Proposed starting points for that file
(not frozen here): N = 30 seeds, K a supermajority, M a per-seed margin on action-rate — with the
arithmetic honest: **K = 24/30 tolerates six non-wins** (set K = 29 if a single non-win is the intent).
Every organ behind a toggle; **off ⇒ the §0 identity must hold** (a requirement, not an asserted result).

## Order (frozen law §13)

**C-sep (its own court) → reopened arena nulls → the triad**, "not added until the instrument has two
hands." Triad code lands only after C-sep and the arena nulls sit; this claim is the pre-reg's M-0.
**Next builder move: `court/GATE_third_body.md`** — the executable contract and every pinned value,
required before any M-1 line. Next court to *run*: **C-sep**. Triad code last (pre-reg ladder M-1 → M-3).

## Not claimed

No compassion, emergence, gaze-force, or third body is asserted as a result — only the pre-registered
claim and the gate-form that would refute it. The arc must be able to **lose** (the ninth null). Measured
numbers live in `court/` audits and `llog.md`, never here or in the frozen `PROTOCOL.md`.

---
*Builder's move, v6 (closes Sol's v5 audit; closures per Don's audit-of-the-audit, incl. Don's own
reversal on 1): 1 model = a **64×64 leaky-Hebb transition matrix** `C ← (1−λ)·C + λ·(L_next ⊗ L_now)`,
pred `C·L_now` — a transition, not a zero-order vector; 2 channel = one ring + author-labelled rec
side-channels (author = path, tick = row ordinal), mechanism in the triad, measurement in the frozen
court; 3 regulates-not-learns = **exposure-index invariance** (equal regulation magnitude at equal current
error), reset each life, carry only in transfer, v3 slope-by-life dropped — matches WILL DESIGN; 4 twin =
single-process byte-identical pre-flight + arena ensemble twin; 5 matrix derangement
`C_π[π(i),π(j)]=C[i,j]`, `π(38)≠50 ∧ π(50)≠38`, distinct names; 6 Gate 2 observer guilt ON, guilt-off a
separate arm, natural wounds the gate; 9 leaks removed, K arithmetic honest. Findings 7–8 (the executable
contract + open values) go to `court/GATE_third_body.md`, the precondition of code, not of this commit.
Next: Sol re-reads the new bytes (short); then the GATE file is the builder's move; C-sep runs first by §13.*
