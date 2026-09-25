# actually.life — COURT PROTOCOL

Status: **FROZEN 2026-09-25**. Freezes on Oleg's word; at that moment a separate
`COURT_FREEZE.tsv` (§9) is written and this file becomes read-only law.

Single frozen source of the court's law. Written so a second hand — a different
model — can implement an independent verifier **from this file alone**, without
reading `l.c` or `tests/*.sh` as logic. The builder produces raw and states nothing
about the verdict; the verifier owns the verdict. This file carries laws,
thresholds, grammars, design numbers (windows, seed lists, declared amplitudes,
tolerances), and the verdict vocabulary itself — but **no measured result and no
assigned verdict**. Numbers
of record live in `llog.md` and `tests/verdict_raw/results/`, which the verifier
does not open; instrument identities (hashes, digests) live in `COURT_FREEZE.tsv`,
which the verifier reads as the receipt. Ported from Netta: a protocol frozen before
the measured code is read, worlds pinned by digest, two hands, the log citing only
hashes.

---

## 0. Reproducibility, named — and what IS byte-exact

The arena forks respawning killer processes (B, C) writing to a shared-filesystem
ring; the OS schedules their interleaving, so two runs of the same seed do **not**
produce byte-identical ledgers (and `loveA.log` col 1 is a µs wall-clock stamp,
never byte-stable). Arena runs are **statistically reproducible**: the ensemble is
determined, individual byte streams are not. The unit of judgment is therefore **the
paired 30-seed distribution of A's action-rate, live vs frozen, with the raw
manifest preserved** — never a single byte-exact trajectory. Any claim of
byte-exactness for an arena run is false by construction and out of scope.

What IS byte-exact are **single-process** utterance logs. `waste.log` is the
utterance log, not a per-tick log. Two
Court II pre-flight identities, asserted against receipt rows, never against a value
stated here:

- **Zero-gain twin.** In a fresh root whose only seeded content is `lifeis/world.txt` (§1
  root rules; `<bin>` is invoked from outside the root), run
  `env -i NL_ARENA=1 NL_MONISM=1 NL_MONISM_HEART=2 NL_MONISM_GAIN=0 NL_ID=1 <bin> 42`,
  and the same with `NL_MONISM_FROZEN=1` added. Assert the two `lifeis/waste.log`
  are **byte-identical**: at gain 0 the control differs from live only by field
  structure, which gain 0 removes. (An identity between two fresh runs — no receipt
  value needed.) **Liveness evidence (P1-16, corrected round 4/5):** tick count is not
  observable from `waste.log`, so liveness is asserted at byte level by exactly two
  Boolean conditions: each zero-gain `waste.log` is **non-empty**, AND the live and
  frozen zero-gain logs are **byte-identical to each other** (the identity asserted
  just above). The twin and the solo trajectory are **not compared to each other** —
  they are separate identities (the twin runs under `NL_ARENA=1`, the solo does not),
  and the twin carries **no receipt value**: its liveness is the pairwise equality plus
  non-emptiness, nothing more. No stdout/stderr tick parsing is required or defined.
- **Solo default trajectory.** In a fresh root, `env -i <bin> 42` (no arena env, scrubbed
  environment like the twin) writes `lifeis/waste.log`; assert its digest **and byte length** equal the
  receipt row `solo-default-trajectory`. This is an instrument identity (like a corpus
  digest), so its value lives in the receipt, not here.

---

## 1. The three arms — roles AND launch law

One binary `l` (built from `l.c`), three processes per trial, distinct `NL_ID`.
The **common block** (`cm`) shared by all three, recorded verbatim in each
`manifest.txt`: `NL_ARENA=1 NL_MONISM=1 NL_MONISM_HEART=2 NL_MONISM_GAIN=0.3`.

**Invocation and output paths, literal (P1-8-r7, corrected r7 from the record).** Each
process is run as `<bin> <argv>`, where `<bin>` is the leg binary named **by a path
outside the trial root** (of record `"$REPO/l"`) — it is **not** copied into the root — and
the process's cwd is the trial root. All three write **relative to that cwd**: the A-ledger
`<L>` = `lifeis/loveA.log`; B's record `<rB>` = `lifeis/recB.log`; C's record `<rC>` =
`lifeis/recC.log`; the F1 record (§11) `<rec>` = `lifeis/rec.log`. The killer processes write only
these full recs (via `NL_MONISM_REC`); the reduced files `lifeis/recB_L38.txt` /
`lifeis/recC_L38.txt` are **not** written by any process (P1-1-r22) — they are the `%.4f` reduction
of the full rec (§7, the reduction step is not in `tests/`; in Court II the verifier derives them
from its own regenerated full rec, §10). The sequences `lifeis/seqB.txt` / `lifeis/seqC.txt` are
written by the **controller** (P1-4-r15) —
one `<argv>\n` appended to the matching sequence immediately before each B/C spawn (launch law
below; §10 controller), the killer never writing its own sequence. These are the paths §10's env
table abbreviates as `<L>`, `<rB>`, `<rC>`.

- **A — the observer.** `cm NL_LOVE=1 NL_LOVE_LOG=lifeis/loveA.log NL_KILL=1 NL_ID=1`,
  guilt OFF (no `NL_GUILT`). Argv: `<seed>`. A reads the shared field; `NL_LOVE` damps its
  strike by the dissonance it reads; `NL_KILL` is present only so "spare" is defined.
- **B — a guilty killer.** `cm NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=3
  NL_MONISM_REC=lifeis/recB.log NL_ID=2`. Argv: `<seed>+1000+n` (n = respawn index, from 1).
- **C — a guilty killer.** `cm NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=2
  NL_MONISM_REC=lifeis/recC.log NL_ID=3`. Argv: `<seed>+2000+n`.

B and C **target each other**. A is therefore never struck. **A is not immortal: it
may die inside the window by its own metabolism (hazard: arousal/debt/senescence),
not by a strike.** No survival *filter* is applied (A is never censored by a rival),
but a trial's ledger may end before row 120. This is not selection; the rate is
computed over the rows that exist (§3).

**Launch law (Court II regeneration).** Per trial, per isolated arena root:
1. cwd = a fresh trial root whose pre-launch member set is **exactly**: `lifeis/world.txt`
   (the pinned corpus), an **empty** `lifeis/arena/` directory, and **empty**
   `lifeis/seqB.txt` and `lifeis/seqC.txt` — the launch environment of record, created by
   the harness before the run; nothing else is present. The binary is **not** copied in —
   it is invoked as `<bin> <argv>` from a path outside the root (§above), cwd = the root.
   Environment is scrubbed (`env -i`); nothing is inherited. The binary then
   creates `lifeis/waste.log` at startup and writes files **within**
   `lifeis/arena/` on demand as the arena runs (`claims`, `ether`, `kills`, `monism`,
   `outcomes` — a single-process run need not produce them all). The verifier asserts
   only that `lifeis/waste.log` exists and, for a trial, that `loveA.log` exists; it does
   **not** require any specific `lifeis/arena/` member to be present.
2. Start A in the background.
3. B and C are two **independent sequential chains**: the next B is a fresh process
   started when the previous B exits, likewise C. At most one B and one C are alive
   at any moment. Argv advances the respawn index `n`: `seed+1000+n` / `seed+2000+n`.
4. The trial ends when A dies; B/C chains stop.
5. The frozen arm is the same launch with A's field replaced per §4.

---

## 2. The love-ledger (`loveA`) — grammar and columns

Written by A, one row per lived tick, before any death-break. **Grammar:**
space-separated, exactly 13 fields (the verifier asserts `NF == 13`), no header, no
blank lines, one row per A-tick, format
`%lld %d %d %d %d %d %.4f %.4f %.4f %.4f %.4f %.4f %d`. Two distinct failures, two
distinct verdicts (P1-3, round 4): a **short trailing final row** — the *last* line only,
lacking its terminating `\n` (the writer emits each row and its `\n` together, so a mid-write kill
leaves an un-terminated final line, untrusted whatever its field count) — is a
biological death, not a raw defect: the verifier drops that one partial row, and the
trial proceeds if the surviving ledger has ≥ `WMIN` rows (§3), else it is **NA and
reported**, never silently skipped. Any malformed row that is **not** the final line,
or any complete-row consistency violation (below), is instead a court-level
**`raw-inconsistent`** (§8), not a trial NA. Neither is the archive-level
`instrument-invalid` "corrupt member" (§7): a semantic parse failure of a present,
readable ledger is `raw-inconsistent`, held distinct from a missing/corrupt archive
member.

| # | field | meaning |
|---|-------|---------|
| 1 | `us` | microsecond wall-clock stamp (the only clock A, B, C share cross-process) |
| 2 | `id` | A's arena id (= 1) |
| 3 | `acls` | action class: `0` none · `1` SPARE (finishable rival on the table, A declined) · `2` finish-off (A struck) |
| 4 | `kavail` | a finishable rival was on the table this tick (0/1) |
| 5 | `raid_avail` | a raid opportunity was on the table (0/1) |
| 6 | `yielded` | A yielded a winnable claim (0/1) |
| 7 | `rival_h` | rival health |
| 8 | `rival_diss` | rival dissonance |
| 9 | `abs_diss` | magnitude of A's own dissonance this tick |
| 10 | `guilt` | A's guilt scalar (0 in this court — A is guilt-off) |
| 11 | `dis` | A's raw dissonance (the read signal) |
| 12 | `u_pre50` | ring site-50 amplitude A saw before its own deposit |
| 13 | `action_valid` | A had a live decision this tick (0/1); rows with 0 are not decisions |

The ledger is measurement, not mechanism: symmetric across arms, written after the
action is chosen, so it cannot move the game.

The action operator is **additive, not Boolean union**: `act = (acls==1)+(yielded==1)`
may equal 2 when a spare and a yield fall on one tick (the `∪` in "spare∪yield" is
prose, not set union).

**Consistency laws**, asserted over every **complete** row of the file (including rows
after `WMAX`; the single short trailing row of a mid-write death, above, is excluded
and handled as NA) — a violation on a complete row = **`raw-inconsistent`** (§8), a
court-level result, not a trial NA:
- `NF == 13` on every row.
- **Integer domains:** `id = 1`; `acls ∈ {0,1,2}`; `kavail ∈ {0,1}`;
  `raid_avail ∈ {0,1}`; `yielded ∈ {0,1}`; `action_valid ∈ {0,1}`.
- **Finite floats:** columns 7–12 must be finite (reject `nan`/`inf`, which would
  otherwise parse and evade ordinary comparisons).
- **Cross-field:** `acls ≠ 0 ⇒ kavail = 1`; `yielded = 1 ⇒ raid_avail = 1`;
  `guilt = 0` on every A row (A is guilt-off); `us` non-decreasing within a ledger.
- **Row lexicon (P1-1-r16):** a field is well-formed iff its token matches its column type —
  integer columns 1–6 and 13 match `-?[0-9]+`, float columns 7–12 match `-?[0-9]+\.[0-9]+`
  (the `%.4f` form); `nan`/`inf` are lexically rejected here and again by the finite-float law.
  A **complete row** is exactly 13 well-formed fields. `NF ≠ 13` is `ledger_nf`; at `NF == 13` a
  field failing its column lexicon is `ledger_malformed_row`.
- **Tokenizer (P1-2-r17):** fields are separated by **exactly one** `0x20`; a row is
  `field (0x20 field)×12` with **no** leading/trailing whitespace and **no** tab or repeated
  space. `NF` is the count of single-`0x20`-split tokens (a repeated or leading/trailing space
  yields an empty token, changing `NF` → `ledger_nf`; a tab or other whitespace inside a token
  fails that token's lexicon → `ledger_malformed_row`). `Σact`/`Σopp` are computed on this exact
  tokenization.
- **Termination and blank lines (P1-1-r17, machine-grounded):** the file is exactly `N`
  `\n`-terminated rows; the terminating `\n` of the last row does **not** create an empty trailing
  row. The mid-write **drop** is exactly the **final physical line without a terminating `\n`** (any
  field count — a kill may truncate it silently, so no un-terminated final line is trusted). **Any
  `\n`-terminated line with `NF ≠ 13`, an empty line included, is `ledger_nf`** at that line; 'no
  blank lines' follows from this, it is not a second rule.

---

## 3. The window and the rate law

- **Window:** ledger row index `NR ∈ [WMIN, min(WMAX, rows)] = [10, min(120, rows)]`
  inclusive. `WMIN=10`, `WMAX=120`, fixed, pre-declared, not tuned. A trial with
  fewer than `WMIN=10` rows is NA. Intent-to-treat: no trial with ≥10 rows is
  dropped; the rate is over the rows that exist.
- **Filter:** only rows with `action_valid == 1` (col 13) are counted.
- **Opportunity** per counted row: `opp = (kavail > 0) + (raid_avail == 1)`.
- **Action** (spare∪yield) per counted row: `act = (acls == 1) + (yielded == 1)`.
- **A's action-rate for a trial:** `rate = (Σ act) / (Σ opp)`. If `Σ opp == 0`, the
  trial is NA and drops from pairing.
- **Quality figure:** `rows-per-trial` = the total number of ledger rows the trial
  produced **before windowing** (not the count of window rows, not the counted rows). It
  counts **complete rows only** (P1-13-r10, corrected: a mid-write short trailing partial row
  of §2, if any, is **excluded** — this is the same count the ledger reader keeps after
  dropping that partial row). The verifier reports, **grouped by (condition, arm)**, the
  rows-per-trial distribution and the count of trials whose total rows are `< WMAX`
  (truncated) — a trial ending early is a quality datum, not a discard.

---

## 4. The frozen control, and the matched-ness law

The frozen arm replaces the field A reads with a matched-statistics AR(1) surrogate
— same coupling, field structure removed — set by `NL_MONISM_FROZEN=1
NL_FROZEN_MU=0.65 NL_FROZEN_SIG=0.26 NL_FROZEN_RHO=0.22`. This **one triple applies
to every condition** (natural and every forced amplitude, both sites); the surrogate
is **not** re-matched to a forced spike. A stated property, not a hidden one. In the
frozen arm, col 11 (`dis`) IS the surrogate value.

**Matched-ness is a checkable law, on contiguous rows** (col 11 is a time series;
the `action_valid` filter would break contiguity, so it is NOT applied here). It is
computed **for the natural condition only** (`forced/<seed>_amp0_<arm>/`), which is
the gate condition; the forced ladders are not part of the matched-ness report. Over
all window rows `NR ∈ [10, min(120, rows)]` of each ledger, using the exact estimators:

- **mean, pooled:** concatenate col-11 over all in-arm ledgers into one vector `x` of
  length `N`; `mean = Σx/N`.
- **sd, pooled sample:** `sd = √(Σ(x−mean)²/(N−1))` over that pooled vector.
- **lag-1, per-ledger then averaged (exact formula, P1-12-r13):** for each ledger with
  `k ≥ 2` window rows, over the adjacent pairs `x_i = v_i`, `y_i = v_{i+1}`, `i = 1..k−1`:
  `x̄ = Σx_i/(k−1)`, `ȳ = Σy_i/(k−1)`; `Sxy = Σ(x_i−x̄)(y_i−ȳ)`, `Sxx = Σ(x_i−x̄)²`,
  `Syy = Σ(y_i−ȳ)²`; the correlation is `r = Sxy / sqrt(Sxx · Syy)` — **one** `sqrt` of the
  **product** `Sxx·Syy` (not `sqrt(Sxx)·sqrt(Syy)`). Every sum is left-to-right binary64,
  round-half-even after each op, no reassociation/FMA (§5 arithmetic). A ledger with `k < 2`
  or `Sxx = 0` or `Syy = 0` is **excluded and its exclusion reported**. The statistic is the
  unweighted mean of the retained per-ledger `r` (each ledger one vote), summed in `(seed,
  arm)` order.
- Early-NA ledgers (< 10 rows, §3) contribute no rows and are excluded from all three.
- **Degenerate returns (P1-4).** Each aggregate has an explicit `NA`: pooled `mean`
  and `sd` are `NA` when the pooled vector has `N = 0`; `sd` is additionally `NA` when
  `N < 2`; the averaged lag-1 is `NA` when no per-ledger correlation survives exclusion.
  An `NA` aggregate is emitted as the JSON string `"NA"` (not `%.6f`), and a tolerance
  test against an `NA` aggregate is neither IN nor OUT but **undefined-and-reported**.
- **Post-arithmetic non-finite (P1-1-r19).** §2/§7 admit any float token finite in binary64, but
  the pooled binary64 operations above may still overflow: a `mean`/`sd`/`lag1` (matched-ness,
  surrogate self-check) or the pooled grief magnitude (§7) can land at `±∞` or `NaN` on fully
  valid rows. Each such aggregate serializes by **one** sentinel rule — `+∞` → `"+inf"`, `−∞` →
  `"-inf"`, `NaN` → `"NA"` — so §12 keys 8/9 admit `"+inf"` / `"-inf"` beside `%.6f` / `"NA"`, and
  no aggregate is ever an unrepresentable number; a tolerance test against any non-finite aggregate
  is **undefined-and-reported**, like `"NA"`.

Two reports, each against the triple `(MU, SIG, RHO) = (0.65, 0.26, 0.22)`, tolerance
`±0.10` **inclusive** on each of the three:
- **Surrogate self-check** — from the **frozen-arm** ledgers (does the surrogate do
  what §4 declares). Any **non-finite** frozen self-check aggregate — `"NA"` / `"+inf"` / `"-inf"`
  (P1-1-r19) — means the control cannot be shown to work → **`instrument-invalid`**
  (`selfcheck_frozen`, §8), the same as a self-check that misses tolerance.
- **Matched-ness** — from the **live-arm** ledgers (do live and frozen share marginals).
  A **non-finite** live matched-ness — `"NA"` / `"+inf"` / `"-inf"` (P1-1-r19) — is a **non-gating**
  reported observation (§8), never `instrument-invalid`: a live organism that produced too few
  contiguous rows, or a pooled overflow, is a datum about the run, not a broken instrument.

Match or mismatch is the verifier's finding; the protocol asserts no outcome. The
live-arm matched-ness is a **non-gating** observation (§8) and must not be promoted to
gating at freeze without re-sitting the court.

---

## 5. The gate (natural-love retrial, Claim 2)

Per seed, paired: A's `rate` in the live arm and the frozen arm. The claim under
this gate is **operational**: whether A's declared action-rate (§3) is higher in the
live field than in the matched-frozen one, per seed. It is not, and must not be
promoted into, a claim that A perceived a typed grief event and let it move a later
action (see §8 status ceiling; the causal-identity gap is named there).

- **Win/tie by exact integer cross-multiplication** (never float division, so `d==0`
  is exact): for a paired non-NA seed with integer sums `(act_L, opp_L)` live and
  `(act_F, opp_F)` frozen (both `opp > 0` by §3), the seed is a **win** iff
  `act_L·opp_F > act_F·opp_L`, a **tie** iff `act_L·opp_F == act_F·opp_L`, else a loss.
- `wins = #{ winning seeds }`. A tie is not a win.
- **Gate:** `wins ≥ 20` over the **30 declared seeds**. An NA is a lost seed, not a
  smaller denominator: the denominator is always 30. Chance is 15.
- **Paired t** (a reported figure, **not a gate in §5**; it is the floor law's
  statistic in §6, used as the signed condition `t ≥ 2.00`, not `|t|`). Let
  `d_s = rate_live − rate_frozen` (float) over the `n` paired non-NA seeds,
  `mean(d) = Σd_s/n`, sample sd `sd(d) = √(Σ(d_s−mean)²/(n−1))`. Then
  `t = mean(d) / (sd(d)/√n)`. **Degenerate returns, defined:** `n < 2` → `t = NA`
  (the amp cannot clear the floor); `sd(d) = 0` with `mean(d) > 0` → `t = +∞`
  (clears); `sd(d) = 0` with `mean(d) < 0` → `t = −∞`; `sd(d) = 0` with `mean(d) = 0`
  → `t = 0`. No p-value is reported; the floor law reads the signed `t` directly.
- **Evaluation arithmetic (P1-5).** `rate`, `d_s`, `mean(d)`, `sd(d)`, and `t` are
  computed in IEEE-754 **binary64** throughout. The floor comparison `t ≥ 2.00` (§6)
  is made on the **unrounded** binary64 `t` against the exact value `2.0`, never on a
  printed/rounded `t`. Rounding to `%.6f` happens **only** for JSON output and for the
  `1e-3` print-comparison of reported figures (§12), never inside a gate decision. `+∞`
  and `−∞` compare to `2.0` as ordinary infinities; `NA` is neither `≥` nor `<` `2.0`
  (the amp does not clear the floor and is reported as `NA`).
- **Accumulation order and rounding (P1-7-r6, total for reproducibility).** Every sum is
  formed by **left-to-right** accumulation in binary64 with the IEEE-754 round-to-nearest,
  ties-to-even rounding **after each operation**, with **no** reassociation, no fused
  multiply-add, and no extended-precision intermediates. Orders: within a trial, ledger
  rows in file order (ascending `NR`); `Σact`, `Σopp` over those rows; across seeds,
  `Σd_s` and the sd sum over **seeds 1…30 ascending** (NA seeds skipped, order kept); the
  pooled matched-ness vectors (§4) concatenated in `(seed, arm)` order, row order within a
  ledger. This same discipline governs `FIELD-EFFECT`, the floor `t`, the matched-ness
  aggregates, and the F1 maximum. **Control tolerances are tested on the unrounded
  binary64 aggregate** (§4 matched-ness `±0.10`, like the floor `t ≥ 2.00`), never on the
  printed `%.6f`. The `%.4f` reduction (§7) rounds each `L38` with the **same** round-half
  -to-even rule.
- **Quality:** per trial, whether B/C grief-events were present (§7 leg 2), so a null
  is not mistaken for a silent instrument.

Seeds are frozen: `seeds = {1, 2, …, 30}`.

---

## 6. The detectability controls, and the floor law

A null of action means nothing unless the instrument can register a wound in the
action channel. Forced wound = a clean spike overriding B/C's deposit:
`NL_MONISM_FORCE=<amp>`, on ring site `NL_FORCE_SITE` (default `50` = death-site;
`38` = grief-site, the matched control). For a fixed site, over the 30 declared seeds:

- `FIELD-EFFECT(amp) = mean(d)` at that amp, `d` per §5; `FIELD-EFFECT = NA` when
  there is no paired non-NA seed at that amp (`n = 0`), reported as `"NA"`.
- **Declared amplitude ladder** (design, not result): death-site amps
  `{0, 0.8, 1, 2, 4, 8, 20}`; matched grief-site amps `{m, 2, 8, 20}`, where `m` is
  the natural grief magnitude — a number of record, **not stated here**. For Court I
  it is read from each condition's `manifest.amp` (in the shipped raw); for Court II
  it is a declared input, never an answer key.
- **Floor law, per site:** the detection floor of a site is the **smallest amp in
  that site's declared ladder with paired `t ≥ 2.00`** (`t` per §5, signed). It
  returns an amp or "not reached". There are **two floors, named separately** — the
  death-site floor (over `{0, 0.8, 1, 2, 4, 8, 20}`) and the grief-site floor (over
  `{m, 2, 8, 20}`); "the floor amp" without a site is undefined. No adjective is a
  floor.
- **Restricted floor (Court II subset).** If Court II regenerates only a subset of a
  site's ladder, the floor is the smallest amp **within that regenerated subset** with
  `t ≥ 2.00`, and the record it is compared against must be recomputed on the identical
  subset and site (not the full-ladder floor). Zero regenerated amps for a site → that
  site's floor is "not computed", not "not reached".
- **Predeclared subset, and "not computed" is never a pass (P1-1-r6, min fixed r6b).** The
  Court II regeneration subset is **declared before execution** and pinned in the receipt
  (`court-2-subset`, §9). The blind-safe **mandatory minimum is the lowest and highest
  declared rung of each site** — `{0, 20}` death-site and `{m, 20}` grief-site — not merely
  "one amplitude per site". This is a **design-coverage** requirement, asserting no
  outcome: the lowest rung of each site (`0`, `m`) is the null / natural-magnitude
  condition, where the floor question is precisely what §6 measures; a subset of only those
  rungs could not exercise the instrument's reach at all, so the top rung (`20`) is required
  so that both site floors are actually computed and a floor difference there is testable.
  The default of record is the **full** two-site ladder.
  A site whose regenerated subset is empty yields `"not computed"`, and `"not computed"`
  **cannot satisfy** the `reproduced` floor requirement (§8/§12): a site left uncomputed
  is an unverified site, not a reproduced one. `"not computed" == "not computed"` is
  therefore **not** floor agreement — it is an incomplete Court II, reported as such.

The forced conditions use the same ledger grammar (§2) and the same rate and gate
laws (§3, §5).

---

## 7. The preserved raw (two pinned tgz), its grammars, and the presence law

Both pinned by SHA-256 + byte length in `COURT_FREEZE.tsv` (§9); no raw of record is
admitted from a source without a committed hash (Netta's candidate-SHA law).

- **`forced_core.tgz`** — the two NULL conditions. The **natural condition**
  (`forced/<seed>_amp0_<arm>/`) carries **the gate** (§5, the one gate-bearing condition,
  §8). The **matched grief-site condition** at magnitude `m`
  (`forced_site38/<seed>_amp<m>_<arm>/`) carries the **grief-presence leg** (leg 2, §7)
  and the **lowest rung of the grief-site floor** (§6) — it is not a gate. Both dirs ship
  the full ledger + reduced grief timeline (leg 2).
- **`ladder_core.tgz`** — the nine forced amplitudes the floor law needs
  (death-site `{0.8, 1, 2, 4, 8, 20}`, grief-site `{2, 8, 20}`), ledgers-only
  (`loveA.log` + `manifest.txt` + `seqB.txt` + `seqC.txt`, **no** rec files — the
  floor is a rate quantity and needs no grief timeline). The two tgz together give
  Court I every amplitude the §6 floor law requires. The `amp0` runs are of record in
  `forced_core.tgz`; every other amplitude is of record in `ladder_core.tgz` (both
  share the same builder). This mapping is fixed **here**, not by the receipt (§9 has
  no per-amplitude tgz stage).

**Layout of record** — the SITE is in the directory name, not the manifest:
- `forced/<seed>_amp<amp>_<arm>/` = death-site 50.
- `forced_site38/<seed>_amp<amp>_<arm>/` = grief-site 38.
- `<amp>` is printed as in the dir name (`0`, `<m>`, `0.8`, `1`, …); `<arm> ∈
  {live, frozen}`. The grief-site natural-magnitude amplitude `<m>` is a number of
  record supplied by the receipt (§9), never spelled in this protocol.
- The **natural condition of record** is the `amp0` rung of the death-site ladder:
  `forced/<seed>_amp0_<arm>/` — not a separate run.

**Files per dir** (rec files present in `forced_core.tgz` only):
- `loveA.log` — leg 1, the 13-column ledger of §2.
- `recB.log` / `recC.log` (full, when present) — grammar: `us` then 64 fields
  `%.6g` = `L[0..63]` (`CFIELD_N = 64`, a frozen constant); grief lives at `L[38]`,
  which is **column 40** (1-indexed).
- `recB_L38.txt` / `recC_L38.txt` — leg 2 reduced: two fields `us L38`, one row per
  B/C deposit whose `L38 > 0.01` (the **file threshold**, frozen).
- `seqB.txt` / `seqC.txt` — the exact B/C respawn-seed sequences.
- `manifest.txt` — three lines, several `key=value` per line, the last value on
  lines 1 and 3 running to end of line:
  ```
  seed=<int> amp=<num> frozen=<none | the NL_MONISM_FROZEN… env string>
  HEAD=<git short> l.c=<md5 32hex> bin=<md5 32hex> corpus=<md5 32hex>
  window=[<lo>,<hi>] gthr=<num> cm=<the common env block>
  ```
  Note `l.c` contains a dot; `frozen` and `cm` contain spaces and `=`; the live arm
  writes `frozen=none`. Manifest hashes are **md5**.

**Manifest lexical + semantic law (P1-12).** Anchored fields: `<int>` = `[0-9]+`;
`<num>` = `[0-9]+(\.[0-9]+)?`; `<git short>` = `[0-9a-f]{7,}`; md5 = `[0-9a-f]{32}`
(lowercase). Exactly three lines, `\n`-terminated, single-space between pairs, no
trailing space. Line 1's `frozen` and line 3's `cm` consume the remainder verbatim.
Semantic equality the verifier asserts: `manifest.amp` numerically equals the amp in
the directory name (numeric compare, so `1` == `1.0`); `manifest.seed` equals the
directory seed; `manifest.window` equals `[10,120]`; all 60 trials of one condition
carry the same `amp`, `window`, `cm`, and (for that arm) `frozen`. A parse failure or
a broken equality is an artifact inconsistency (§8), not a biological NA.

**Archive completeness + member policy (P1-9).** Expected member set per archive:
`forced_core.tgz` = the natural condition (`forced/*_amp0_*`) and the grief-site-`m`
condition (`forced_site38/*_amp<m>_*`); `ladder_core.tgz` = the nine ladder amplitudes
of §6. Each condition = exactly `{1..30} × {live, frozen}` = 60 dirs, exactly one dir
per `(seed, arm)`. **Admitted member types (P1-6-r6):** only **regular files** and
(ignorable) directory entries. The verifier **rejects** as artifact inconsistency every
other type — hard links, symlinks, character/block **devices**, **FIFOs/sockets** — and
every unsafe path: an absolute path, any `..` component, or a path that escapes the
archive root. **Path normalization before the expected-set and duplicate checks:** each
member path is normalized to a canonical relative form — strip a single leading `./`,
collapse repeated `/`, drop `.` components, forbid `..` and any trailing `/` on a file —
and **duplicate detection and completeness are tested on the normalized path** (so `./x`
and `x` are the same member, and `a//x` normalizes to `a/x`; two members normalizing to
one path are a duplicate).
A **missing** declared `(seed, arm)` dir, a missing mandatory file, or a corrupt member is
an **artifact inconsistency** (§8) — distinct from an organism-produced NA (a trial that
ran but had < 10 rows or `Σopp = 0`). Mandatory files: `loveA.log`, `manifest.txt`,
`seqB.txt`, `seqC.txt` in every dir; `recB_L38.txt`, `recC_L38.txt` (and, when present,
`recB.log`, `recC.log`) additionally in the two `forced_core.tgz` conditions. The optional
`recB.log` / `recC.log` are **known optional members** (P1-2-r16), never counted as
`extra_safe_member`. **Extra
members (P1-8):** **every mandatory file must be present** in each declared dir (a
missing one is `instrument-invalid`, above) — that is gating. An **extra safe** member —
a readable file that is not unsafe (§ above) and not a duplicate — is a **reported
non-gating observation**, never `instrument-invalid`: the archive of record may carry
additional safe files. Explicit tar **directory entries** are **optional** — the verifier
keys on file member paths and neither requires nor forbids directory entries (many valid
tar producers omit them).

**Sequence grammar (P1-10).** `seqB.txt` / `seqC.txt`: one `<int>` per line,
`\n`-terminated, the `n`-th line (1-indexed) equals `seed+1000+n` (B) / `seed+2000+n`
(C). The verifier asserts contiguity from `n=1` and the exact arithmetic against the
directory seed; an empty sequence means the chain never spawned. In the two
`forced_core.tgz` conditions (which ship rec) an empty sequence is **legal only if** that
arm's corresponding reduced rec (`recB_L38.txt` / `recC_L38.txt`) is itself empty — that
killer made **no deposit above the `0.01` file threshold** (the file-threshold scope, not
the `0.30` presence scope). **Ownership of the malformed-row case (P1-7-r13):** a **malformed
row inside a non-empty rec** is owned by the **grief grammar below** (leg-2 NA, a non-gating
observation), **not** by this sequence law — it does not make the seq inconsistent. The
seq-vs-rec emptiness test uses only whether the rec has **any well-formed deposit row**: an
empty seq is legal iff the rec has no well-formed deposit row. In **ladder dirs** (no rec
shipped) there is nothing to
corroborate an empty sequence against, so an empty `seqB`/`seqC` there is an **artifact
inconsistency** (`instrument-invalid`, §8). A sequence that violates the formula or skips
an index is an artifact inconsistency (`instrument-invalid`, §8).

**Grief-log authority + equivalence (P1-11, precision corrected round 4).** The reduced
`rec*_L38.txt` is written at **`%.4f`** (two fields `us L38`, `us` the integer µs stamp);
the full `rec*.log`, when present, is written at `%.6g`. When a full `rec*.log` is
present, the verifier **recomputes** the reduced file from it — filter rows whose
**full-precision** `L38 (col 40)` is `> 0.01` (the file threshold, applied before
rounding), then emit `us` and `L38` **rounded to `%.4f`** — and asserts **exact**
agreement (same rows, same `%.4f` values) with the shipped reduced file. A disagreement
here is an **artifact inconsistency → `instrument-invalid`** (§8, code `rec_equivalence`) —
the shipped reduction does not match its own full log (not reachable while the archives
ship no full rec, but it is law). On agreement, the verifier computes presence from the
recomputed sequence. When only the reduced file is present (ladder
dirs ship no rec at all; leg 2 is not computed there), the reduced file is authority.
Each grammar: rows must be finite, `us` non-decreasing, no duplicate `us` within a
file. **Rec tokenizer (P1-3-r17, a recognizing grammar):** every rec row is
single-`0x20`-separated fields, **no** leading/trailing whitespace, `\n`-terminated, with a
**mandatory final LF**; the full-rec row is `us` + 64 fields (`NF == 65`), the reduced-rec row is
`us L38` (`NF == 2`); `us` (full and reduced alike) = the one literal regex `0|[1-9][0-9]{0,18}`
(no leading zero, ≤19 digits), a full-rec field matches the `%.6g` lexicon
`-?[0-9]+(\.[0-9]+)?(e[+-][0-9]{2,3})?` (integer, fixed, and exponent forms), and the reduced
`L38` the `%.4f` `[0-9]+\.[0-9]{4}`. **Finiteness (P1-1-r18):** a field is well-formed iff it
matches its regex **and** its round-to-nearest binary64 value is finite (`isfinite`); a regex
match that overflows to ±`inf` under binary64 is **not** finite (a malformed rec row). A row failing this tokenization is a **malformed rec row** (leg-2 NA, or F1
`f1_bad_rec`, §11). `rec_equivalence` compares the **byte strings** — `us` as its integer bytes,
`L38` as its `%.4f` bytes — never numeric values. A malformed reduced/full row makes that trial's
**leg 2 NA and reports it** (a non-gating observation, §8 — not `raw-inconsistent`, since leg 2 is
a quality leg, not the gate). **A grammar failure suppresses equivalence (P1-3-r16):** a malformed
row in the full **or** the reduced rec makes leg 2 NA and does **not** fire `rec_equivalence` (no
canonical reduction can be built); `rec_equivalence` fires **only** when both full and reduced are
well-formed and the recomputed reduction disagrees with the shipped reduced. The reduction
step is not in `tests/`; the reduced files' only provenance is the `forced_core.tgz`
sha256 (§9).

**Provenance law (P1-7 + P1-13, corrected round 4 — bind to `bin`, not `l.c`).** The
manifest's `l.c=` is the md5 of the **worktree `l.c` at launch time** (the writer script
hashes the working file), not of the built source, so `l.c=` does **not** authenticate
the executable and is **not** gating. The executable that produced the **shipped raw** is
authenticated by `bin=`, uniform per leg on the platform of record; a native rebuild
reproduces that md5 only on the same platform/toolchain, so this `bin=` check is a
**Court I** provenance assertion over the of-record raw (Court II on another platform gates
its own executable behaviourally, §8 / §0). Every `<num>` equality below is **numeric** (so
`0.3` == `0.30`), every hash equality is exact lowercase-hex. The verifier **asserts**
(gating for Court I; a mismatch is `instrument-invalid`, §8):
- `manifest.bin` (md5) = the receipt's `binary-<leg>-leg` md5 for that condition's leg
  of record (§9 / §10 routing);
- `manifest.corpus` (md5) = the receipt's `corpus` md5;
- `manifest.window` = `[10,120]` (exact string); `manifest.gthr` numerically equals
  `0.3`;
- `manifest.cm` = the exact §1 common block (string equality to the declared block, not
  merely identical across the 60 dirs); frozen-arm `manifest.frozen` = the exact §4 env
  string, live-arm `manifest.frozen` = `none`.

Reported as **non-gating observations** (§8), never gating: `manifest.l.c` (the
launch-time worktree md5) and `HEAD` (git state, not build provenance). Independent
**source** identity exists only if Court II is
additionally supplied each leg's `l.c` as an opaque pinned blob (§8 blindness): it hashes
that committed blob and asserts md5+sha256 against the receipt's `l.c-<leg>-leg` rows —
never against the manifest's worktree snapshot. (The two legs' `l.c` differ by
construction; the receipt names both.)

**Grief-presence law (leg 2), symmetric across arms** — B/C deposit real grief in
BOTH arms (only A's *read* is replaced), so presence and magnitude are reported for
**both** live and frozen; an asymmetry between arms is a finding about the
instrument. Each quantity named:
- A trial's A-window µs bounds `[us_lo, us_hi]`: `us_lo` = the `us` of A's ledger row
  `NR = 10`; `us_hi` = the `us` of the last A row with `NR ≤ 120`. If A has fewer
  than 10 rows, the trial is NA for leg 2.
- An in-window grief-event: a B or C row with `us ∈ [us_lo, us_hi]` and `L38 > 0.30`
  (the **presence threshold**, frozen, distinct from the file threshold 0.01). **The
  authority for both the presence test and the pooled magnitude is the canonical reduced
  `%.4f` `L38` value** (P1-5, round 5) — the value in `rec*_L38.txt`, or, when a full
  `rec*.log` is recomputed, its `%.4f`-rounded value — never the full `%.6g` value. This
  keeps presence reproducible whether or not a full rec ships. (The `0.01` **file**
  threshold is unaffected: per P1-11 above it filters the **full-precision** value before
  rounding, at reduction time — a separate operation from this `0.30` presence test.)
- `present(trial) = (in-window grief-event count > 0)`.
- Reported **per (condition, arm)** separately — the natural death-site condition and
  the grief-site-`m` condition are never pooled together: pooled `mean of L38` (the same
  `%.4f` values) over all in-window grief-events of that (condition, arm); a (condition,
  arm) with zero in-window events reports magnitude **`NA`, not 0**. **Accumulation order
  (P1-8-r10, binary64, left-to-right, round-half-even after each add, no reassociation):**
  sum the events by **seed 1…30 ascending**, within a seed's trial **all `recB_L38.txt`
  events then all `recC_L38.txt` events**, and within each file in **file (row) order** (which
  is `us`-nondecreasing, §7); the mean is that sum over the event count. Also report per-trial event
  counts and the present-count over 30. If the arm-asymmetry (live vs frozen presence
  or magnitude) is treated as more than descriptive, its statistic and tolerance are
  stated here; absent that, asymmetry is a reported observation, not a gate.

---

## 8. The two courts and the verifier's contract

The 08-07 dirt (RNG leak, non-hermetic arena, ledger echo) was **generation-side**;
re-scoring preserved raw cannot catch that class. Two courts, named:

- **Court I — re-score.** The verifier reimplements §2–§7 over the shipped raw. Court
  I is arithmetic on identical raw, so its bar is **exactness**: per trial the
  verifier's integer `Σact` and `Σopp` must equal the record exactly; `wins` exact;
  reported means and `t` within `1e-3` (printing). Grants "verified on the preserved
  raw".
- **Court II — regenerate.** The verifier launches the pinned binary under its own
  isolated roots with the launch law (§1), on the frozen seeds and corpus, runs the
  §0 pre-flight identities first, and re-derives the distribution with the same §5
  gate on its **own** raw. Being statistical (§0), Court II compares the **gate
  outcome** (`wins ≥ 20` or not) on each **gate-bearing condition**, the **two per-site
  floor amps** (§6), and the **F1 assertion** (§11), never per-trial counts. Grants
  "verified on regeneration". **Gate-bearing conditions (Sol #6):** the gate (`wins`,
  §5) is meaningful only where a live/frozen pair carries the operational Claim 2 — the
  **natural condition, death-site `amp0`** (`forced/*_amp0_*`). That is the one gate of
  record; `wins` is a per-condition quantity (§12 keys it by condition), and no forced
  amplitude other than the floor's paired-`t` is a gate. **Court II budget (P1-2-r7):**
  the **receipt-declared `court-2-subset` (§9) is mandatory** — it always includes the
  natural condition and each site's endpoint rungs (`{0,20}` / `{m,20}`, §6). Only rungs
  **beyond** that minimum are optional, and only when the receipt is authored. Court II
  regenerates exactly the pinned subset; the floor comparison runs over it (§6 restricted
  floor). A condition **inside** the pinned subset that was not executed is an explicit
  `instrument-invalid` (a promised regeneration that did not run), never a silent
  omission; the JSON omits only conditions **outside** the pinned subset (§12).

**Verifier's contract — implemented without reading `l.c` or `tests/*.sh` as logic:**
1. A ledger reader (§2) with the consistency assertions and the abort rule.
2. The rate law (§3), pairing, gate, paired-t (§5) over the natural raw.
3. The FIELD-EFFECT and floor law (§6) over the death-site and matched grief-site raw.
4. The matched-ness reports (§4) and the grief-presence measure (§7 leg 2).
5. The provenance assertion (§7) and, for Court II, the §0 pre-flight and §1 launch.
6. The no-kill F1 law (§11) is Court II only. It is **not** in `forced_core.tgz`;
   Court I does not confirm it. Its pass/fail is a Court II gating outcome (verdict
   table, above; `f1` object in §12).
7. The Court II launch matrix and orchestration (§10), and the sealing + comparison
   contract (§12) that turns a score into a verdict.

**Blindness — two closed input sets (P1-1-r14).** The verifier is written in a session given,
from this repo, exactly these inputs and no others — a court reads only its own set:
- **Court I inputs (closed):** `PROTOCOL.md`, `COURT_FREEZE.tsv`, `forced_core.tgz`,
  `ladder_core.tgz` (the nine ladder amplitudes of §6/§7), the pinned corpus file. No binaries,
  no `l.c` (Court I re-scores shipped raw; it checks `manifest.bin`/`corpus` md5 against the
  receipt).
- **Court II inputs (closed):** `PROTOCOL.md`, `COURT_FREEZE.tsv`, the pinned corpus file, and
  the leg binary or binaries per §10 routing; **plus, for each leg whose §8 selection probe
  routes it to rebuild, that leg's `l.c` source blob** as a mandatory supplied input (named by
  a local input path, verified md5+sha256 against its `l.c-<leg>-leg` receipt row before
  compilation). Court II is **not** given the tgz archives. A **native** leg needs no source
  blob; a leg routed to rebuild with its source blob **absent** is `instrument-invalid`
  (`source_missing`), a **mismatched** one `provenance_mismatch`. (So a mixed run — natural
  rebuilt, matched native — requires only the natural `l.c` blob, and vice-versa.)

**The executable Court II runs (P1-3-r8/r8b/r9; selection vs gate split r10).** Two separate
steps per leg — a **non-gating selection probe** that only picks native-vs-rebuild, then the
**gating §0/F1 admission** on whichever binary was picked:
- **Selection probe (NON-GATING, never a verdict).** Run the receipt-pinned binary through a
  §0 **solo** attempt. It is chosen as native **iff it "executes"** — process **exit status 0
  AND `lifeis/waste.log` present** within a **120 s** probe timeout (a probe still running at
  the timeout is **killed**) — **and** the log yields the `solo-default-trajectory` digest.
  Anything else — exec error, non-zero exit, missing `waste.log`, a killed-at-timeout probe,
  or a wrong digest — is **not** a failure and carries **no code/locus** (non-gating by
  design): it simply routes this leg to **rebuild**. The probe never yields
  `instrument-invalid` (it is a choice, not the §0 gate). A compatibility layer that passes
  the probe counts as native by construction.
- **Rebuild** (when the probe did not select native). Court II builds the leg from the opaque
  `l.c` blob via the unread step `cc -O2 -o l l.c -lm`; a compilation that fails, finds no
  compiler, or yields a non-executable is `instrument-invalid` (code `build_failed`, locus
  `build:<leg>`, `<leg> ∈ {natural, matched}`), and a missing/mismatched source blob is
  `source_missing` / `provenance_mismatch` (§ above).
- **§0 admission (GATING, every used leg).** Whichever binary a leg ends up with — native or
  rebuilt — is admitted only by passing, on **that** binary, **both** §0 identities
  (**solo + twin**, §10 runs both on each leg the verifier uses). Each pre-flight process must
  **exit 0** **and** produce the required identity: a **non-zero exit is a failure even if the
  `waste.log` looks right** (for the twin, this applies to **both** of its processes), and each
  runs under a **120 s** per-process timeout (a hung pre-flight is killed). Any failure — bad
  identity, non-zero exit, or timeout — is terminal `instrument-invalid` (codes
  `preflight_solo` / `preflight_twin`, locus `preflight:<leg>`). This is the one place a §0
  identity failure is gating; the selection probe above is not.
- **F1 is not a per-leg admission (r11).** F1 is the **natural-leg no-kill control of
  record**: always run on the natural-leg binary, its `pass` is **verdict-gating** (§11/§12
  `f1`) — `max L[38] ≥ 0.01` there is `f1.pass = false` → Court II `refuted`. The **matched
  leg carries no F1 control.** F1 runs on the matched binary **only when that leg was
  rebuilt**, and then purely as a **toolchain-admission** check: its failure means the rebuilt
  matched binary is behaviourally wrong → **`instrument-invalid`** (code `f1_admission`, locus
  `f1:matched`) — never `refuted`, never a mere observation. A **native** matched leg runs
  **no** F1 at all. So per leg: `preflight_solo`/`preflight_twin` gate both; the natural F1 is
  the control (false → `refuted`); a matched-**rebuild** F1 is an admission gate (false →
  `instrument-invalid`); a native matched leg has no F1.
- A **rebuilt** binary's identities (and, for a rebuilt matched leg, its admission-F1 result)
  are recorded as an `observation` (kind `court2_binary`, §12), never compared to the
  `binary-*-leg` rows. A **natively** admitted leg produces **no** `court2_binary` observation.

`l.c` is never read as logic — any string lifted from `l.c` or `tests/*.sh` **as logic** voids
the pass. The verifier source ships with that session's file-access record.

**Verdict vocabulary (first matching row wins — strict precedence top to bottom),
the verifier's to assign:**
| verdict | condition |
|---------|-----------|
| `instrument-invalid` | a receipt/archive SHA-256 or byte length fails; a **gating** manifest md5 (`bin` / `corpus`, §7 provenance) disagrees with the receipt; a §7 manifest parse or semantic-equality failure, or a `seqB`/`seqC` grammar/formula violation (artifact inconsistency of a present file); a Court II §0 pre-flight identity fails **in the §8 admission step** (the non-gating selection probe is **not** a §0 gate — a probe miss only routes to rebuild); a native/rebuilt leg's build fails or its source blob is missing/mismatched (§8); a mandatory member/trial is missing, duplicated, unsafe, or corrupt (§7 completeness); the **frozen-arm** surrogate self-check (§4) misses tolerance or is **any non-finite aggregate** (`NA` / `+inf` / `-inf`, whether degenerate or post-arithmetic, P1-1-r19 — the control itself is broken/undefined); a Court II F1 process fails to run, times out, yields an **empty** or **malformed** rec (§11); a pinned-subset condition did not run (§6/§8); controller readiness fails — A exits before creating `lifeis/waste.log` (§10) — or any process exceeds the per-process timeout (§10) |
| `raw-inconsistent` | a §2 consistency law fires on a complete ledger row of raw of record, or a non-final malformed ledger row (§2) |
| `protocol-ambiguous` | the verifier had to choose a reading not fixed by this file; it names the line |
| `refuted` | Court I: an integer count differs vs record, or a **gating** reported float (§12 comparator) falls outside its tolerance. Court II: on a gate-bearing condition the gate outcome (`wins ≥ 20`) flips, or a (subset-matched) per-site floor amp flips vs record — both revealed only after sealing (§12). **Separately** (no reveal operand): a sealed `f1.pass = false` (`max L[38] ≥ 0.01`, §11) selects `refuted` **directly**, as a predeclared control, not a record comparison |
| `reproduced` | Court I: all integer counts equal and every gating float within tolerance (§12); Court II: the gate outcome and both per-site floors agree on the regenerated subset — **each site's floor actually computed** (neither side `"not computed"`, §6 P1-1-r6) — **and** F1 passes |

**Sealed status vs post-reveal verdict (Sol #15).** `refuted` and `reproduced` depend
on comparison with numbers revealed only after sealing (§12), so the verifier cannot
assign them in its sealed output. The sealed output carries instead an **interim
`status`** — one of `instrument-invalid`, `raw-inconsistent`, `protocol-ambiguous`, or
`comparison-pending` (the last means: every local check passed, the score is ready to
compare). The terminal `refuted` / `reproduced` is assigned **only** in the post-reveal
comparison report (§12), which cites the sealed score digest and the record digest. The
four interim statuses keep the strict precedence above; `comparison-pending` is lowest,
reached only when none of the first three fired.

**Non-gating observations** (reported always in the sealed object — the `matched_ness` /
`surrogate_selfcheck` / `grief_presence` / `quality` / `observations` keys of §12 — never
changing the verdict unless the freeze explicitly declares one gating). The full set,
matching key 12 `observations`: (1) the **live-arm** matched-ness vs the triple and (2)
its excluded-ledger counts (§4); (3) grief-presence counts and live-vs-frozen asymmetry
(§7 leg 2); (4) rows-per-trial / truncation counts (§3); (5) extra safe archive members
(§7); (6) malformed leg-2 rows that made a trial's leg 2 NA (§7); and (7) each dir's
`manifest.l.c` and `HEAD` values (§7 provenance).

**Status ceiling.** A `reproduced` result grants the below-floor reading the status
"verified on the preserved raw" (Court I) or "verified on regeneration" (Court II) —
and only that: an **operational** below-floor reading for this field/action
instrument under this arena law. It is **not** a confirmation of natural-grief
causality or compassion; the raw grammar can speak only to a paired action-rate figure
and thresholded L38 deposits in A's window, never to a typed grief event perceived by A
and linked to a later act (the 08-07 causal-identity gap). Neither court grants anything
to C-sep, which sits its own court (§13). The verifier's output is **sealed before**
comparison with the numbers of record (§12); a divergence whose cause is a protocol
line is a protocol bug, logged as such, and the court re-sits after a named amendment.

---

## 9. The freeze receipt (`COURT_FREEZE.tsv`, written on Oleg's word)

A separate TSV beside this file (a file cannot contain its own hash). **Exact schema
(P1-13):** five tab-separated columns `stage  path  digest_kind  digest  bytes`, **no
header row** (the first line is data), Unix `\n` line endings. `stage` is from the
fixed set below. `path` names a committed blob in git-blob notation `<revision>:<path>`
(there is **no** separate revision column — the revision lives inside `path`). A non-sentinel
`path` is an **opaque provenance label (P1-5-r16)** matching `<revision>:<path>` with
`<revision>` = `[0-9a-f]{7,40}` and `<path>` = `[^\t\n:]+`; the verifier **never resolves it** (no
`git show`, no object lookup) — every artifact's bytes come only from the closed input set (§8),
and each artifact/blob/trajectory check asserts **both** the receipt `digest`(s) **and** the
`bytes` length (a byte-length mismatch is the same code as a digest mismatch). **Except**
that `path` is the sentinel `-` for exactly the rows with no committed blob: every
`value` row, the `court-2-subset` (`list`) row, the `binary-platform` (`tag`) row, both
`binary-*-leg` rows, and the `solo-default-trajectory` row (P1-1-r6 / P1-7-r7 — the binary
`l` is `.gitignore`d and reproduced from the committed `l.c-<leg>-leg` blob; the solo
trajectory is a generated `lifeis/waste.log`, not a committed file). **`bytes` is `-` for
the `value`, `list`, and `tag` rows** (pure metadata); the `binary-*` and
`solo-default-trajectory` rows are real artifacts, so their `bytes` is the artifact's byte
length even though `path` = `-`.
`digest_kind ∈ {sha256, md5, value, list, tag}` (P1-10 r4 / P1-1-r7 / r8b). For `sha256` /
`md5`, `digest` is **lowercase** hex (`sha256` = 64, `md5` = 32) and `bytes` = `[0-9]+` is
the artifact's byte length. A **`tag`** row (`binary-platform` only) — `digest` is a short
free label `[A-Za-z0-9._-]+` (the arch+compiler of record, e.g. `<arch>-<compiler>`), `path` =
`-`, `bytes` = `-`. For a **`value`** row — typed decimal design metadata — `digest`
is the canonical decimal `[0-9]+(\.[0-9]+)?`, `path` = `-`, `bytes` = `-`. For the
**`list`** row (`court-2-subset` only) — `digest` is either the literal `full` or a
comma-separated list (**no whitespace**) of `<site>_amp<amp>` conditions in the **§10 run
order** (the record's three-group invocation order; the subset is its **stable
subsequence**) (`<site> ∈ {forced, forced_site38}`; `<amp>` spelled exactly as the
ladder writes it), each unique and each a declared §6 ladder condition; an unknown,
misordered, duplicated, or whitespace-bearing entry makes the receipt
`instrument-invalid`; `full` = all eleven declared conditions; `path` = `-`, `bytes` = `-`.
**Row multiplicity:**
a stage may appear once per `digest_kind` (so an artifact carrying both md5 and sha256
is two rows, same stage, same path, different `digest_kind`). Fixed stages of record:
- `PROTOCOL.md` — sha256.
- `l.c-natural-leg` — path `<natural-commit>:l.c`; md5 and sha256.
- `l.c-matched-leg` — path `<matched-commit>:l.c`; md5 and sha256 (the two legs' `l.c`
  differ; both are named). `<natural-commit>` and `<matched-commit>` are the **two
  existing historical leg commits** — distinct revisions, so a single tree never needs two
  `l.c` blobs at one path; the freeze's commit 1 (below) does **not** re-commit `l.c`.
- `binary-natural-leg`, `binary-matched-leg` — `path` = `-` (untracked, `.gitignore`d),
  `bytes` = the byte length; md5 **and** sha256. These are the **platform-of-record**
  binary hashes (the md5 a manifest `bin=` compares to for Court I). They are **not**
  platform-portable: a Court II rebuild on another platform produces different bytes and is
  gated behaviourally (§0/F1), recording its own hashes in `observations` (§12), not against
  these rows.
- `binary-platform` — `digest_kind = tag`; `digest` = the arch+compiler of record (the
  `tag` grammar above); `path` = `-`, `bytes` = `-`. **Informational only** — it records the
  platform on which the `binary-*-leg` hashes were taken; it does **not** select native
  vs rebuild (that is the execution test of §8, run the pinned binary through the §0 solo
  pre-flight), and is never itself gating.
- `corpus` — sha256 (the manifests' `corpus=` md5 is the same file's md5; add an md5
  row too, so the manifest comparison is receipt-checkable).
- `forced_core.tgz`, `ladder_core.tgz` — sha256.
- `solo-default-trajectory` — `path` = `-`, `bytes` = the log's byte length; md5 of
  `lifeis/waste.log` from `env -i <bin> 42` in a fresh root, identical on both leg binaries
  (a generated log, not a committed file; the §0 identity, of which the receipt is the only
  source). (The
  zero-gain twin has **no** receipt row: §0 asserts its liveness by pairwise
  `live == frozen` equality and non-emptiness, needing no value of record.)
- `natural-grief-magnitude-m` — `digest_kind = value`; `path` = `-`, `bytes` = `-`;
  `digest` = the canonical decimal spelling of `m` (P1-14: `m` lives here, never in the
  protocol body; Court I cross-checks it against `manifest.amp` of the site-38
  condition; Court II takes it as the declared input). **Domain constraint (P1-8-r8), so
  the grief-site ladder `{m, 2, 8, 20}` stays well-ordered and its condition names
  unique:** `0 < m < 2` and `m ∉ {2, 8, 20}`; a receipt whose `m` violates this is
  `instrument-invalid`. This is a design constraint on the input, not a disclosure of the
  value.
- `court-2-subset` — `digest_kind = list` (grammar above); `path` = `-`, `bytes` = `-`;
  `digest` = the predeclared Court II regeneration subset (P1-1-r6), pinned **before** any
  Court II run — the literal `full`, or the canonical `<site>_amp<amp>` list. It must
  include at least the lowest and highest rung of each site — `{0, 20}` death-site and
  `{m, 20}` grief-site (§6 P1-1-r6). Court II regenerates exactly this subset; a floor over
  a site absent from it is `"not computed"` and cannot be `reproduced`.

**Provenance scope (P1-13, corrected round 4).** The blind input set does **not**
include the committed `l.c` source blobs, and `manifest.l.c` is a launch-time worktree
hash (§7), so Court I does **not** authenticate the source through the manifest. Its
**gating** provenance is `manifest.bin` (md5) = the receipt's `binary-<leg>-leg` md5 and
`manifest.corpus` (md5) = the receipt's `corpus` md5; `manifest.l.c` and `HEAD` are
non-gating observations. Independent **source** hashing happens only if `l.c` is
additionally supplied as an opaque pinned blob for Court II's build step (§8 blindness);
that blob's md5+sha256 must equal the receipt's `l.c-*-leg` rows. `PROTOCOL.md` carries
no self-hash; the log cites the receipt's digests.

**Required vs optional rows, and schema-failure verdict (P1-3, round 5).** **Required**
stages (each must be present, with each `digest_kind` named): `PROTOCOL.md` (sha256);
`l.c-natural-leg`, `l.c-matched-leg` (md5+sha256 each); `binary-natural-leg`,
`binary-matched-leg` (md5+sha256 each); `binary-platform` (tag); `corpus` (md5+sha256);
`forced_core.tgz`, `ladder_core.tgz` (sha256 each); `solo-default-trajectory` (md5);
`natural-grief-magnitude-m` (value); `court-2-subset` (list). There are **no optional
stages** — the row set is exactly these. **Any** `COURT_FREEZE.tsv` defect is
**`instrument-invalid`** (§8): a malformed TSV row (wrong column count, bad line ending,
illegal `digest_kind`, sentinel where a hash is required or vice-versa, non-lowercase hex,
wrong hex length); an unknown or duplicate `(stage, digest_kind)`; a missing required
stage or required `digest_kind` row; or a cross-row inconsistency (the md5 and sha256 rows
of one artifact naming a different `path` or `bytes`). None of these is
`protocol-ambiguous`.

**Freeze topology — two commits, preceded by the status-line edit (P1-8-r6 / P1-2-r6b).**
Because every hash-`path` names an already-committed blob as `<revision>:<path>`, a receipt
cannot live in the same commit as the revision it cites (the commit hash depends on the
tree, the tree would contain the receipt, the receipt would contain the hash — a
self-reference). Freeze is therefore one edit, a final byte pass, then **two commits**:
0. **The last edit before commit 1 (P1-7-r9, exact hunk).** On line 3, replace the exact
   substring `**DRAFT** (unfrozen)` with `**FROZEN YYYY-MM-DD**` (ISO calendar date, the
   freeze day in Asia/Jerusalem, e.g. `**FROZEN 2026-09-11**`). **Nothing else on line 3 or
   anywhere else changes** — the remainder of line 3 (`. Freezes on Oleg's word; at that
   moment a separate`) and line 4 stay byte-identical. So line 3 goes from
   `Status: **DRAFT** (unfrozen). Freezes on Oleg's word; at that moment a separate` to
   `Status: **FROZEN YYYY-MM-DD**. Freezes on Oleg's word; at that moment a separate`.
0b. **Post-edit byte pass (P1-10-r7).** Any protocol-only review that returned an empty
   list read the DRAFT bytes; the edit above changes them, so a final **byte-identity** pass
   runs on the post-edit text — it does **not** reopen substance. It `diff`s the post-edit
   bytes against the approved DRAFT and asserts the only difference is that one substring on
   line 3 (`**DRAFT** (unfrozen)` → `**FROZEN YYYY-MM-DD**`), naming both sha256s — and it
   fixes the `PROTOCOL.md` sha256 that commit 2's receipt will cite. This pass cannot be
   skipped.
1. Commit only what is not already committed: the frozen `PROTOCOL.md`, `forced_core.tgz`,
   `ladder_core.tgz`. The two `l.c` legs are **already** in their distinct historical leg
   commits (`<natural-commit>`, `<matched-commit>`), the corpus is already committed at its
   own revision (carried into commit 1's tree unchanged, so citable at either) — commit 1
   re-commits none of them — and the binaries are not committed (`.gitignore`d, reproduced
   from the `l.c` blobs).
2. Compute the digests — `PROTOCOL.md` and the two tgz against **commit 1's** revision, the
   corpus at commit 1's path, the `l.c` legs against their historical leg commits — write
   `COURT_FREEZE.tsv` with each hash-`path` = `<that-revision>:<path>` (and `-` for the
   `value`, `list`, `binary-*`, and `solo-default-trajectory` rows), and commit it.
The receipt is pinned by commit 2 (named in the freeze log, not self-hashed). Never write
a provisional revision and `--amend` the same commit.

---

## 10. Court II launch matrix and orchestration

**Force routing (P0-1).** The force overrides the **killers'** deposit, so
`NL_MONISM_FORCE` and `NL_FORCE_SITE` go to **B and C, never A**. A's env is exactly
§1 in every condition. The natural condition means the force variables are **absent**
(not `NL_MONISM_FORCE=0`). The frozen arm differs from live **only in A** (A gains
`NL_MONISM_FROZEN` + the triple, §4); B and C are identical across arms.

Per-process env, verbatim (`<f> = NL_MONISM_FORCE=<amp> NL_FORCE_SITE=<site>` for a
forced condition, empty for natural; `cm` per §1):

| condition | A (live) | A (frozen) | B | C |
|-----------|----------|------------|---|---|
| natural (amp0, site 50) | `cm NL_LOVE=1 NL_LOVE_LOG=<L> NL_KILL=1 NL_ID=1` | + `NL_MONISM_FROZEN=1 <triple>` | `cm NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=3 NL_MONISM_REC=<rB> NL_ID=2` | `cm NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=2 NL_MONISM_REC=<rC> NL_ID=3` |
| forced death-site (amp>0, site 50) | same as A above | same | B above **+ `<f>`** | C above **+ `<f>`** |
| forced grief-site (site 38) | same as A above | same | B above **+ `<f>`** | C above **+ `<f>`** |

**Binary routing (P0-2).** Two leg binaries, built from the two pinned `l.c` blobs
(§9): the **natural-leg** binary (no `NL_FORCE_SITE` code) and the **matched-leg**
binary (`NL_FORCE_SITE` present, site-50 behaviour bit-identical by gate-invariance).
Of record: death-site conditions (natural + death ladder) ran on the **natural-leg**
binary; grief-site conditions ran on the **matched-leg** binary. For regeneration the
verifier uses the **leg binary of record per condition, and only that** (P0-2 / Sol
#11): natural-leg for death-site, matched-leg for grief-site. The earlier permission to
substitute the matched-leg binary for death-site runs is **withdrawn** — §0 declares
arena runs non-byte-reproducible, so a claimed forced-site-50 "bit-identity" between the
two binaries is unobservable under this protocol's own law and must never select the
measured executable. If leg-equivalence is worth proving, it gets its own byte-level
test, never an optional court input. The two §0 pre-flights are run on **each** leg
binary the verifier **uses** (native or rebuilt, §8), and must give the same identities on
both.

| runs on natural-leg binary | runs on matched-leg binary |
|---|---|
| §0 twin + solo; natural amp0; death ladder `{0.8,1,2,4,8,20}` | §0 twin + solo; grief-site `{m,2,8,20}` |

**Controller orchestration (P1-15), pseudocode** — the interleaving stays stochastic
but the set of allowed processes and the termination are fixed:

```
start A (background), argv=seed, env per table
# readiness: the binary opens lifeis/waste.log with mode "w" at the start of live(),
# so a healthy A creates it almost immediately. wait for it, bounded by the 120 s
# per-process timeout below. if A exits before creating lifeis/waste.log, or the wait
# hits the timeout, the trial is instrument-invalid (not a legal empty B/C sequence).
# a zero-length-but-created waste.log counts as ready (the file exists).
wait until A has created lifeis/waste.log (A is ready) before launching B/C
loop chain_B: n_B=1; while A is alive: append "seed+1000+n_B\n" to lifeis/seqB.txt (commit
              point), then attempt exactly one B spawn (argv seed+1000+n_B, env per table),
              run to completion; n_B++         # at most one B alive
loop chain_C: n_C=1; while A is alive: append "seed+2000+n_C\n" to lifeis/seqC.txt (commit
              point), then attempt exactly one C spawn (argv seed+2000+n_C), run to
              completion; n_C++
# A-alive is tested ONLY at each loop top, before the append; once a line is appended the one
# spawn attempt is mandatory even if A exits first (an exec failure IS that attempt -> a
# non-empty seq with an absent rec = missing_output, §10 matrix). the killer never writes
# seqB/seqC. so every seq line has exactly one spawn attempt and no A-exit race remains.
the two chains run concurrently with each other and with A
on A exit: stop launching new B/C; let the currently-running B and C finish their
           own exit (they are not signalled); then the trial is complete
a B/C that is still mid-run when A exits is allowed to finish (bounded: one lifetime)
per-process hard timeout = 120 s wall; a process exceeding it is killed and the trial
           is classified instrument-invalid (a hung bench, not a biological NA)
files are parsed only after every process of the trial has exited (all fds closed)
```

**Court II output completeness (P1-5-r12, total).** A regenerated trial's generated files are
checked before scoring, with fixed codes (`instrument-invalid`, locus
`<condition>/<seed>_<arm>:<role>`):
- **`loveA.log` absent** after A has exited → `missing_output` (locus `…:A`). A ran but wrote
  no ledger — a broken run, not an organism NA.
- **A leg's seq and rec (P1-5/#5,#6-r13; full matrix, P1-1-r15).** Each killer's
  `seqB.txt` / `seqC.txt` is **pre-created empty** by the harness (§1), written **only** by the
  controller (one `<argv>\n` appended before each spawn, §10 controller), and is therefore always
  **present**; its **absence** after the trial → `missing_output` (locus `…:B` / `…:C`). The full
  rec `recB.log` / `recC.log` is **not** pre-created — the killer opens it at init, so a rec is
  **expected present only after a successful spawn**; the verifier derives the reduced leg-2 record
  from the full rec. The **full (seq × rec) matrix**, each cell fixed:
  - **empty `seq`, rec absent** → **0 events** (`present = false`), legal, *not* `missing_output`;
  - **empty `seq`, rec present** (empty **or** non-empty) → `seq_grammar` (locus/line per §12
    special-case);
  - **non-empty `seq`, rec present-empty** → legal **0 events** (`present = false`);
  - **non-empty `seq`, rec present-non-empty** → the killer's deposits, read normally;
  - **non-empty `seq`, rec absent** → `missing_output` (locus `…:B` / `…:C`).

  In both 0-event cells (empty seq + absent rec, non-empty seq + present-empty rec) leg 2 is
  **0 events** (`present = false`), never `NA`. A malformed **row** inside a present rec is the §7
  grief-grammar's leg-2 NA observation, not an output-completeness or seq defect — see §7.
- **Rec-grammar scope (P1-4-r16):** outside the two leg-2 conditions (natural death-site `amp0`,
  grief-site-`m`) the full rec is checked for **presence/emptiness only** (the matrix cells above);
  its **row grammar** — and any `rec_equivalence` / leg-2-NA — is computed **only** in those two
  leg-2 conditions, so a malformed rec row elsewhere yields no observation.
- These are the only Court-II output cases; `waste.log` absence is the §0 readiness rule.

**Arena process exit status (P1-9-r13, not gating).** Unlike the §0 pre-flights and F1 (which
require exit 0), the arena processes' exit status is **ignored** — the arena is judged on the
files it left, not on exit codes: **A** may die non-zero by its own metabolism (§1), a
biological death; its ledger is scored over the rows that exist (§3), and a short/corrupt
ledger is caught by §2 (`raw-inconsistent`) or the `< WMIN` NA rule (§3), never by A's exit
code. **B / C** exits — normal or non-zero — simply end that respawn; the chain advances to
the next respawn while A is alive (§1) and stops when A exits. A crashed B/C is not
`instrument-invalid` by its exit code; only an **absent** output file it should have left is
(`missing_output`, above). So no arena exit status ever decides a verdict.

**Cross-trial scheduling — the record's schedule, reproduced exactly (P1-15 / Sol
#12,#13,#4-r5,#2-r6,#4-r8; #5-r9 closed by reproducing the record).** OS interleaving
*within* a trial is part of the statistical ensemble (§0). Court II reproduces the record's
actual schedule — no stricter barrier is invented and no independence is claimed:

- **Within a run-group — start-serialized, A-exit.** Conditions and their trials run
  contiguously: the next trial's A launches only **after the previous A has exited**, in the
  order **condition → seed (1…30) → arm (live, then frozen)**. A previous trial's finishing
  B/C **tail may still run** when the next A starts — including across a condition boundary
  *inside a group* — and, because each trial has its **own fresh root** (§1), that overlap is
  CPU/scheduler contention only (no file collision). This is the record's schedule and Court
  II reproduces it, boundaries included. (A trial's own files are parsed only after **all**
  its processes exit — the controller's `all fds closed` rule above; that governs safe
  reading, not the next launch.)
- **Run-group order and the two real barriers (Sol #5-r9 / Don).** The record ran the eleven
  conditions in **three builder invocations**, and Court II runs them in that exact order,
  with a full **all-tails-exited barrier** only at the two invocation breaks (`|`):
  ```
  forced_amp0, forced_amp0.8, forced_amp20
    | forced_amp1, forced_amp2, forced_amp4, forced_amp8
    | forced_site38_amp<m>, forced_site38_amp2, forced_site38_amp8, forced_site38_amp20
  ```
  Inside a group the boundary is plain A-exit (tails may overlap, as of record); at each `|`
  every process of the previous group has exited before the next group's first A starts (the
  record's own invocation break). A Court II subset is the **stable subsequence** of this
  order; a barrier survives wherever both its adjacent groups are non-empty.

Because the schedule — group order, A-exit boundaries, and the two barriers — is reproduced
**as of record**, reference and regeneration share it in structure; there is no "independent"
claim to make and no stricter barrier than the record's. Court II compares each condition's
gate outcome and floor **per condition** (§0, statistical). A verifier that wants parallelism
must declare it and show the gate/floor are stable under it; the law is the record's
schedule as above.

---

## 11. The no-kill F1 law (Court II only)

A single control that the grief site is silent without a confirmed kill. **Not** in
any shipped tgz; Court II regenerates it.

- **Launch (corrected round 4; roles clarified r11):** one process, superego **armed** but
  with **no confirmable kill** — B's exact §1 killer env in a **solo** root, where the target
  id names no process that exists, so no strike is ever confirmed:
  `env -i cm NL_KILL=1 NL_GUILT=1 NL_TARGET_ID=3 NL_MONISM_REC=<rec> NL_ID=2 <bin> <F1-seed>`.
  It runs in exactly two roles (§8), never symmetrically on both legs: **(a) the control of
  record** — always, on the **natural-leg** binary; its `pass` is verdict-gating (§12 `f1`),
  and `max L[38] ≥ 0.01` is `f1.pass = false` → `refuted`. **(b) a matched-leg toolchain
  admission** — only when the matched leg was **rebuilt** (§8); same launch on the matched-leg
  binary, but its failure is `instrument-invalid` (`f1_admission`, locus `f1:matched`), not
  `refuted`, and it never enters the `f1` verdict field. A **native** matched leg runs no F1.
  In a fresh isolated root (§1 root rules) holding only the pinned corpus (no B/C, no other
  process — so no id-3 target is present). `cm` is the §1 common block; `F1-seed = 7` (a
  declared design seed). **Why armed:** site 38 is
  written **only** through the
  confirmed-kill guilt transient — grief deposits only when the superego records a kill —
  so an *unarmed* process (no `NL_GUILT`) is silent at 38 by construction and would test
  nothing. Arming the superego with no confirmable target is what makes "the grief site
  stays silent **without a confirmed kill**" the thing actually measured.
- **Measure:** over **every** row of `<rec>` (grammar §7: `us` + 64 fields, `NF == 65`),
  the grief site is `L[38]` = column 40. The statistic is the **maximum** of `L[38]`
  over all rows (not a mean, not a sample).
- **Assertion:** `max L[38] < 0.01` (the file threshold — the grief site stays at floor
  with the superego armed but no kill confirmed).
- **`<rec>` grammar (total, P1-4-r10).** Every row is `us` + 64 `%.6g` fields (`NF == 65`),
  every field **finite**, `us` non-decreasing, no duplicate `us`.
- **The two roles are coded separately and totally (P1-1-r12).** F1 has exactly two roles
  (§8), and each maps **every** failure mode, so the same `false` never has two readings:
  - **Natural-leg control of record** (locus `f1`), evaluated in this fixed priority (the same
    literal order as sub-pass (g), P1-5-r14): process fails to launch → `f1_no_run`; the 120 s
    per-process timeout → `process_timeout`; **a non-zero exit is `f1_bad_rec` with `line = 0`
    regardless of the rec; `f1_empty_rec` applies only at exit 0** — so at exit 0 an **absent or
    empty** `<rec>` (no deposit) → `f1_empty_rec`, and a non-empty `<rec>` carrying a malformed /
    non-finite / duplicate-`us` **row** → `f1_bad_rec` at that row. Only then, on a well-formed
    non-empty `<rec>` at exit 0, the outcome: `max L[38] ≥ 0.01` → `f1.pass = false` → Court II
    **`refuted`**; `max L[38] < 0.01` → `f1.pass = true`. Everything but that final pass is
    `instrument-invalid`, all at locus `f1`.
  - **Matched-leg toolchain admission** (only on a matched rebuild, locus `f1:matched`):
    **every** failure mode — launch failure, absent/empty rec, malformed/non-finite/
    duplicate-`us` rec, non-zero exit, timeout, **or** `max L[38] ≥ 0.01` — collapses to the
    single code **`f1_admission`** (`instrument-invalid`): the rebuilt matched binary did not
    behave, so it is rejected. Never `refuted`, never `f1_bad_rec`/`f1_empty_rec` — those are
    the natural control's codes.
- **Outcome (total, no `NA`).** The `f1` field (§12) is the natural control only, `pass` a
  Boolean; there is no `f1.pass = "NA"`. The matched admission has no `f1` field — its result
  is `f1_admission` (fail) or the `court2_binary` observation (pass).
- **Named limit.** A lone armed process never strikes, so F1 shows only that the grief
  site is silent when the superego is armed **and idle** — it cannot by itself separate
  "grief fires only on a *confirmed* kill" from "grief fires on any *strike attempt*".
  A passing leg 2 (§7) would show the channel firing under confirmed kills; leg 2 and F1
  together would then bracket the claim. A strike-without-kill control would be a third
  rung, out of scope for this freeze.

---

## 12. Sealing and comparison contract

Turns a score into a verdict without either hand seeing the other's numbers first, in
**two phases**: a sealed score that carries **no verdict**, then a post-reveal
comparison that assigns one (Sol #15).

**Phase 1 — sealed score report (canonical JSON, hashed).** A single UTF-8 JSON object.
**Canonicalization (project-local, no external dependency, Sol #16 + #9-r5):**
- **Whitespace:** `{"k":v,...}` and `[...]` with **no** space anywhere (none after `:`
  or `,`); the file ends with a single trailing `\n` and carries no other whitespace.
- **Key order:** top-level keys in the numbered order below; each nested object's keys in
  the order its schema lists; **dynamic-keyed** objects (`wins`, `t_by_amp`,
  `field_effect`, `rows_per_trial_by_group`, `truncated_by_group`) sorted by key using the
  same total order as the arrays below (condition/amplitude order; `<amp>` compared
  numerically, exact string on a tie).
- **Array order (a canonical serialization sort, independent of §10's run order):** `trials`
  by `(condition, seed asc, arm)`; `grief_presence` by `(condition, arm)`; **`condition`** by
  site (50 before 38) then amplitude **ascending numeric** (`<amp>` compared numerically,
  exact string on a tie), `arm` = `live` before `frozen`. `floors` = site 50 then 38. Every
  fixed 30-element array (`event_counts`, the `quality` per-group arrays) in **seed order
  1…30**. (This byte-order is fixed for canonicity; it is **not** §10's run/invocation order,
  which governs scheduling, not serialization.)
- **Numbers:** finite values as `%.6f`, decimal point `.`, IEEE-754 round-half-to-even,
  no locale; integers as bare digits; the only non-numeric numeric values are the
  sentinel strings `"NA"`, `"+inf"`, `"-inf"`.
- **Strings / escaping:** UTF-8; escape exactly `"`→`\"`, `\`→`\\`, and every control
  char U+0000–U+001F as its **lowercase** `\u00xx` (never a shorthand such as `\n`, never
  uppercase hex); nothing else is escaped.
- **Unexecuted optional Court-II conditions** are **omitted** entirely (never emitted as
  empty or `NA` entries): `floors[].t_by_amp`, `field_effect`, and the per-condition
  arrays carry only conditions actually scored/regenerated.

The SHA-256 is taken over exactly these bytes. Keys, in order:

1. `court` — `"I"` or `"II"`.
2. `status` — the interim status (§8): `"instrument-invalid"`, `"raw-inconsistent"`,
   `"protocol-ambiguous"`, or `"comparison-pending"`. **There is no `verdict` key in the
   sealed object** — a terminal verdict cannot exist before reveal.
3. `status_reason` — `null` when status is `comparison-pending`; otherwise the closed
   object `{"code":<code>,"locus":<string>,"line":int}` (keys in that order, P1-1-r8), so
   two verifiers that select the same defect seal the same bytes. `code` is one of the
   **fixed vocabulary** below. `line` is the 1-based row/line the defect sits on, or `0`
   when not line-scoped. **`locus` — the canonical string per code family (P1-2-r9):**
   an archive-member defect → the normalized member path (§7); a receipt defect →
   `receipt:<stage>:<digest_kind>`, or `receipt` for a receipt-wide defect; an **`artifact_hash`
   or `archive_hash`** (a pinned artifact — `PROTOCOL.md`, `corpus`, a tgz, or a Court II leg
   binary — disagreeing with its receipt row) → `receipt:<stage>:<digest_kind>` (both use the
   same receipt-row form); a **manifest** defect (`manifest_parse`, `manifest_equality`) →
   `<trial-dir>/manifest.txt`, a **ledger** defect (`ledger_*`) → `<trial-dir>/loveA.log`, a **seq**
   defect (`seq_grammar`) → that killer's `<trial-dir>/seqB.txt` / `<trial-dir>/seqC.txt` file, a
   **rec** defect (`rec_equivalence`) → the reduced file's path — each the **file** path, never the
   dir, where `<trial-dir>` is the **logical trial-dir** `<condition>/<seed>_<arm>` (no trailing
   slash), identical in both courts (Court I from the archive dir normalized, Court II the same
   string, never a physical fresh-root path, P1-4-r16); a **trial process** defect
   (`readiness_fail`, `process_timeout`, `missing_output`) → `<condition>/<seed>_<arm>:<role>` with
   `<role> ∈ {A, B, C}`; a **pre-flight** (`preflight_solo`, `preflight_twin`) → `preflight:<leg>`
   (`<leg> ∈ {natural, matched}`); the **natural-leg control F1** (`f1_no_run`, `f1_empty_rec`,
   `f1_bad_rec`, and an F1 `process_timeout`) → `f1`; the **matched-leg admission F1**
   (`f1_admission`) → `f1:matched`; a **build** (`build_failed`) → `build:<leg>`; a **source
   blob** (`source_missing` **or** a `provenance_mismatch` on an `l.c` blob) → `l.c-<leg>-leg`
   (a `provenance_mismatch` on a manifest `bin`/`corpus` → `receipt:<stage>:<digest_kind>`);
   the **frozen self-check** (`selfcheck_frozen`) → `selfcheck`; a **subset** miss
   (`subset_not_executed`) → the condition `<site>_amp<amp>`; `receipt_bad_m` →
   `receipt:natural-grief-magnitude-m:value`; `receipt_extra_row` → `receipt:<stage>:<digest_kind>`
   of the extra row; `protocol_ambiguous` → `line` alone carries the position and
   `locus` = `protocol`. **Special-case loci/lines that would otherwise be undefined
   (P1-10-r13; P1-7-r14; P1-3-r15; P1-6-r16):** the **byte-safe tar name** of a member is its
   **effective logical name** (the path **after** any PAX `path` / GNU `longname` override — never
   the physical header's name field), every byte outside printable ASCII `0x21`–`0x7E`, **and** the
   byte `%` itself, **percent-encoded** as `%XX` (uppercase hex, `%` as `%25`); the **tar entry
   index** is the 1-based position of a member among the **logical members the POSIX tar reader
   returns** (PAX/GNU extended, `longname`/`longlink`, and global-header records are metadata for
   the following member and are **not** counted). A member is **normalization-safe** iff every byte
   of its effective logical name is in `0x21`–`0x7E`, none is `%`, and it normalizes to a valid
   relative path (§7); its locus is the **normalized path**, any other member's the **byte-safe tar
   name**. **Raw-name fallback — any archive defect on a non-normalization-safe member (an unsafe
   path, a space/`%`/non-ASCII byte, or a non-UTF-8 name):** `locus` = the **byte-safe tar
   name**, `line` = the **tar entry index**; this covers `archive_member_type` (checked before
   unsafe-path, §12 (c)), `archive_unsafe_path`, `archive_duplicate`, and an `archive_corrupt_member`
   **with** a readable name, the entry index also **ordering** several such entries (entry order,
   **not** canonical-path order). A member with a safe, normalizable path keeps `locus` = its
   normalized path (§7). `archive_corrupt_member` with **no readable name** → `locus` = the archive
   stage (`forced_core.tgz` / `ladder_core.tgz`), `line` = the **last entry index successfully read**
   (`0` if none); any `receipt_malformed_row` → `locus` = `receipt`, `line` = its row number
   (**every** malformed row, whether or not its `stage`/`digest_kind` parse); a **missing**
   required row (`receipt_missing_stage`) → `locus` = `receipt:<stage>:<digest_kind>` of the
   missing row, `line` = `N+1` (one past the last data row); `rec_equivalence` → `locus` = the
   reduced file's path, `line` = the **first differing row** (1-based); a `seq_grammar` from
   **any forbidden empty seq** — a Court II empty seq + present rec (§10 matrix), a Court I
   forced_core empty seq whose reduced rec has a well-formed deposit row (§7), **or** a Court I
   ladder-dir empty seq (§7) — → `locus` = that killer's `seqB.txt` / `seqC.txt` path, `line` =
   `0` (the file has no row, P1-6-r17 / P1-2-r18); a `seq_grammar` from a **formula/contiguity** violation on
   a present seq row (§7) → `locus` = the seq path, `line` = the offending seq row; any
   `provenance_mismatch`
   → `line` = `0` (a whole-value comparison, not a line). **Lines/loci pinned flat, no `or`
   (P1-4-r16):** `manifest_parse` → `line` = the first offending physical line (1-based; an extra
   4th line → `4`, a truncated manifest → the first missing line's number, P1-2-r17);
   `manifest_equality` → `line` = the manifest line (`1`–`3`) of the failing field; every
   `ledger_*` → `line` = the offending ledger row; `receipt_duplicate_stage` → `locus` =
   `receipt:<stage>:<digest_kind>`, `line` = the **second** (duplicate) row;
   `receipt_crossrow_mismatch` → `locus` = `receipt:<stage>:<digest_kind>` of the **later** row,
   `line` = that later row; `archive_missing_dir` → `locus` = the expected `<trial-dir>`, `line` =
   `0`; `archive_missing_file` → `locus` = `<trial-dir>/<filename>`, `line` = `0`; every
   `readiness_fail`/`process_timeout`/`missing_output`/`preflight_*`/`build_failed`/
   `source_missing`/`f1_admission`/`selfcheck_frozen`/`subset_not_executed` → `line` = `0`.
   **Fixed `code` vocabulary** (no others; every
   §8 `instrument-invalid` trigger and every §2/§4/§6/§7/§9 law has one, P1-1-r8/r8b/r10):
   `receipt_malformed_row`, `receipt_bad_digest_kind`, `receipt_bad_hex`,
   `receipt_bad_sentinel`, `receipt_bad_bytes`, `receipt_bad_path` (a non-sentinel `path` not
   matching `<revision>:<path>`, §9), `receipt_bad_subset`, `receipt_bad_tag`
   (a `binary-platform` tag not matching its grammar), `receipt_bad_m`,
   `receipt_unknown_stage` (a stage not in the fixed set), `receipt_extra_row` (a row whose
   `(stage, digest_kind)` are each individually legal and not duplicated, but is not in the
   exact required row set of §9 — e.g. a second, unrequired `md5` row for `PROTOCOL.md`),
   `receipt_missing_stage`, `receipt_duplicate_stage`, `receipt_crossrow_mismatch`;
   `artifact_hash` (a non-archive pinned artifact — `PROTOCOL.md`, `corpus`, or the pinned
   binary — disagreeing with its receipt row, `locus` = `receipt:<stage>:<digest_kind>`),
   `archive_hash` (a tgz), `archive_member_type`, `archive_unsafe_path`, `archive_duplicate`,
   `archive_corrupt_member` (unreadable/truncated entry, distinct from missing),
   `archive_missing_dir`, `archive_missing_file`; `manifest_parse`, `manifest_equality`,
   `provenance_mismatch`; `seq_grammar`; `rec_equivalence` (§7 full-vs-reduced disagreement);
   `preflight_solo`, `preflight_twin`, `readiness_fail`, `process_timeout`,
   `missing_output` (the §10 set: a Court II regenerated `loveA.log` absent after A exit, a
   `seq` file absent, or a rec absent while its `seq` is non-empty), `f1_no_run`
   (F1 process failed to launch — F1 has no readiness step), `f1_empty_rec`, `f1_bad_rec`
   (a non-zero exit — `line = 0`, regardless of the rec — or, at exit 0, a
   malformed/non-finite/duplicate-`us` row; `f1_empty_rec` applies only at exit 0, §11),
   `build_failed` (a Court II native rebuild failed / no compiler / non-executable, §8),
   `source_missing` (a mandatory `l.c` source blob absent on the rebuild branch, §8;
   `locus` = `l.c-<leg>-leg`), `f1_admission` (a rebuilt **matched**-leg toolchain admission
   F1 failed, §8/§11; `locus` = `f1:matched`);
   `selfcheck_frozen` (the frozen-arm surrogate self-check misses tolerance or is **non-finite**
   — `NA` / `+inf` / `-inf`, P1-1-r19 — on §2-valid frozen ledgers, §4);
   `subset_not_executed` (a pinned-subset condition that did not run, §6/§8); `ledger_nf`,
   `ledger_domain`, `ledger_nonfinite`, `ledger_crossfield`, `ledger_malformed_row`;
   `protocol_ambiguous`.

   **Selection — separate full passes in verdict precedence, never a mixed stop-at-first
   traversal (P1-3-r6, r7, r8).** Each verdict class runs to completion **in precedence
   order**; the first class with any defect wins; within it the first defect by the total
   order below is serialized.
   1. **`instrument-invalid`** — whole class first. The sub-passes are **court-scoped**
      (P1-1-r13): Court I runs `(a) (b-I) (c) (d) (e-I)`; Court II runs `(a) (b-II) (f) (g)
      (e-II)`; the first firing sub-pass **in that court's sequence**, then its first defect,
      wins. **Global tie-break (P1-7-r11):** within a stage carrying both, **`md5` before
      `sha256`**; among two trial processes, **B before C**.
      (a) **receipt `COURT_FREEZE.tsv`** (both courts) — **first, a whole-file line-ending
      check** (LF-only; any `CR`/CRLF is `receipt_malformed_row` at the first offending line,
      before any row is parsed, P1-8-r12); then per row top-to-bottom, and **within one row the
      lexical checks in this fixed order**: column-count → `digest_kind` legality →
      `path`-sentinel legality → `path`-format (`receipt_bad_path`, a non-sentinel `path` not
      matching `<revision>:<path>`) → hex case → hex length → sentinel-vs-hash → `bytes` grammar →
      `receipt_bad_tag` → `receipt_bad_subset` → `receipt_bad_m` → `receipt_unknown_stage`;
      then the receipt-wide checks — **ordered by ascending position first, and at an equal
      position by the fixed type order** `missing` < `duplicate` < `cross-row` < `extra`
      (P1-11-r13): a **missing** required row's position is *`N+1`* (one past the last data
      row), a **duplicate** at its **second** occurrence, a **cross-row** mismatch at the
      **later** of its two rows, an **extra** row (`receipt_extra_row`) at its own row position.
      **Among several missing required rows** — all sharing position `N+1` and type `missing` —
      the tie is broken by the **§9 required-row list order** (stage order as listed, `md5`
      before `sha256`, and `value`/`list`/`tag` rows in that list's order), P1-6-r14.
      (b-I) **artifact/archive digests, Court I** — `PROTOCOL.md`, `corpus`, `forced_core.tgz`,
      `ladder_core.tgz` (`artifact_hash` / `archive_hash`) against the receipt, `md5` before
      `sha256`. Court I is given no binary input; it checks `manifest.bin` in (d).
      (b-II) **artifact/binary digests, Court II** — `PROTOCOL.md`, `corpus`, then each
      **supplied leg binary** hashed against its `binary-<leg>-leg` receipt row (natural then
      matched, `artifact_hash`), before the §8 selection probe. Court II is **not** given the
      tgz archives and does **not** run (c)/(d) — it regenerates its own raw. The behaviour-gate
      of §8 is orthogonal and still decides native-vs-rebuild by execution.
      (c) **archive structure, Court I** — member-type, unsafe-path, corrupt-member,
      completeness, duplicate checks — `forced_core.tgz` then `ladder_core.tgz`, dirs in
      ascending canonical-path byte order, files in the fixed order `manifest.txt`, `loveA.log`,
      `seqB.txt`, `seqC.txt`, `recB_L38.txt`, `recC_L38.txt`, `recB.log`, `recC.log`, then any
      remaining members (unexpected names or non-regular types) in **tar entry index** order
      (P1-6-r16).
      (d) **manifest + provenance + seq + rec-equivalence, Court I** (§7) in that dir/file order
      (`manifest_parse`, `manifest_equality`, `provenance_mismatch` on a manifest `bin`/`corpus`,
      `seq_grammar`, `rec_equivalence`).
      (f) **Court II build** (rebuild branch), per leg **natural then matched**: `source_missing`,
      `provenance_mismatch` (an `l.c` source blob vs its `l.c-<leg>-leg` receipt row),
      `build_failed`.
      (g) **Court II run** in §10 schedule order: §0 pre-flights per leg **natural then matched**,
      and within a leg **solo then twin** (`preflight_solo` then `preflight_twin`, P1-4-r14); the
      **natural-leg control F1**, in this literal order —
      `f1_no_run` (no launch) → `process_timeout` (F1 timeout) → **non-zero exit** `f1_bad_rec`
      (`line = 0`, whatever the rec) → [exit 0] `f1_empty_rec` (rec **absent or empty**) →
      [exit 0, non-empty rec] `f1_bad_rec` at the first malformed/non-finite/duplicate-`us`
      **row** (`line = that row`) → outcome (`max L[38]` → `f1.pass`); and the **matched-rebuild
      admission F1** (`f1_admission`, **`line = 0` always**, whatever its cause); then launch
      `readiness_fail` (**readiness_fail suppresses process_timeout for A's readiness wait,
      P1-7-r17:** if A has not produced `waste.log` by its deadline — A exited early **or** the
      120 s wait expired — the code is `readiness_fail`; `process_timeout` fires only for a process
      that timed out **after** readiness, a hung A-after-ready or a hung B/C, B before C on a
      simultaneous timeout); then per-trial
      **output completeness** `missing_output` (§10, in dir/seed/arm order, role A then B then
      C); then, per trial in dir/seed/arm order and **B before C**, `seq_grammar` (the §10
      formula/contiguity check, and a **present** rec, empty or non-empty, with an empty seq);
      `rec_equivalence` **does
      not fire in Court II** — the verifier derives the reduced rec from its own regenerated full
      rec, so there is no shipped reduced to disagree with (it is the Court I (d) check only,
      P1-4-r14); then `subset_not_executed` for any pinned-subset condition that did not run
      (multiple in the §12 canonical condition order — site 50 then 38, amp ascending, P1-8-r16).
      (e-I / e-II) **§4 frozen surrogate self-check** (`selfcheck_frozen`) — **last in each
      court** (Court I after (d); Court II after (g), so the regenerated frozen ledgers exist,
      P1-2-r13). Evaluated **only over §2-valid frozen-arm ledgers** (P1-9-r11): a §2
      complete-row violation on a frozen ledger is **not** `selfcheck_frozen` but
      `raw-inconsistent` (class 2, with its §2 code/locus). **If any frozen-arm ledger is §2-invalid,
      sub-pass (e) is not computed at all** (P1-7-r16) — control passes to class 2 and the §2 defect
      surfaces as `raw-inconsistent`; only when **every** frozen ledger is §2-valid is
      `selfcheck_frozen` computed, and it then fires for a tolerance miss or **any non-finite
      aggregate** (`NA` / `+inf` / `-inf`, whether degenerate or post-arithmetic, P1-1-r19).
   2. **`raw-inconsistent`** — only if class 1 is empty: §2 ledger consistency over all
      dirs/files in the same **(c)** order (Court I) or, Court II, over the regenerated ledgers in
      the **§12 canonical serialization order** (condition by site 50-before-38 then amplitude
      ascending, seed `1…30`, arm live-before-frozen — the `trials`-array order, never §10's
      schedule or archive-byte order, P1-8-r16), rows in file (row) order. **Within one row**
      violating several §2 laws the sealed `code` is the **first** in §2's law order — the mid-write
      short final row is dropped first (§2, not a defect); then `NF ≠ 13` → `ledger_nf`; then at
      `NF == 13` a field failing its column lexicon (§2 row lexicon) → `ledger_malformed_row`; then
      `ledger_domain` → `ledger_nonfinite` → `ledger_crossfield`; `line` = that row's 1-based index.
   3. **`protocol-ambiguous`** — only if 1–2 are empty: the first line the verifier had to
      guess, in file order.
   Because a whole higher-precedence class is scanned before any lower one, a
   `raw-inconsistent` ledger defect can never mask an `instrument-invalid` defect elsewhere.
   (Other defects are not enumerated; the object names the single winning defect.)
4. `trials` — array sorted by `(condition, seed, arm)`, each `{"condition":"<site>_amp<amp>",
   "seed":int,"arm":"live"|"frozen","sum_act":int,"sum_opp":int,"rate":%.6f|"NA",
   "na_reason":""|"lt_wmin"|"zero_opp"}` (the two organism-NA causes of §3; a dropped
   short trailing row surfaces as `lt_wmin` if it takes the ledger under `WMIN`, never a
   reason of its own). **For an NA trial both sums seal `0` (P1-2-r15):** `lt_wmin` an empty
   window; `zero_opp` a window whose counted `Σopp = 0` (§2 cross-field: `act > 0 ⇒ opp > 0`, so
   `Σact = 0` too) — `sum_act = 0`, `sum_opp = 0`, `rate = "NA"`. `<site> ∈ {forced,forced_site38}` — `forced` ↔ site 50,
   `forced_site38` ↔ site 38 (matching `floors[].site`); `<amp>` is spelled exactly as
   the directory name (`0`, `0.8`, the decimal `m`, …).
5. `wins` — object keyed by **gate-bearing condition** (§8), each value int over the 30
   seeds; there is one gate-bearing condition, so one key: `"forced_amp0"`.
6. `floors` — array `{"site":50|38,"floor_amp":"<amp-as-in-dir-name>"|"not reached"|"not
   computed","t_by_amp":{"<amp>":%.6f|"NA"|"+inf"|"-inf"}}`, site 50 then 38. **`floor_amp`
   is a JSON string** (P1-7-r10) — the amplitude spelled exactly as the directory name
   (`"0"`, `"0.8"`, the decimal `m`, `"20"`), or the sentinel `"not reached"` / `"not
   computed"` — **never** a `%.6f` number, so the two serializations can't collide. The
   `t_by_amp` **keys** are those same dir-name strings; the `t_by_amp` **values** are `%.6f`
   or a sentinel.
7. `field_effect` — object `{"<condition>":%.6f|"NA"}` for every scored amp.
8. `matched_ness`, `surrogate_selfcheck` — each `{"mean":%.6f|"+inf"|"-inf"|"NA","sd":%.6f|"+inf"|"-inf"|"NA",
   "lag1":%.6f|"+inf"|"-inf"|"NA","excluded_ledgers":int,"na_ledgers":int}` (non-finite per §4 post-arithmetic). **`excluded_ledgers` is
   lag-1-scoped only (P1-12-r10):** the count of ledgers **not retained by the lag-1
   estimator** — `k < 2` window rows or zero variance in either paired series (§4) — each such
   ledger counted **once** regardless of how many reasons apply. **`na_ledgers`** is the
   separate count of early-NA ledgers (`< 10` rows, §3) that contributed no rows to any of the
   three aggregates. The two are disjoint and both seal explicitly.
9. `grief_presence` — array sorted by `(condition, arm)`, each `{"condition":"<site>_amp<amp>",
   "arm":"live"|"frozen","present_count":int,"magnitude":%.6f|"+inf"|"NA","event_counts":[(int|"NA") × 30]}`
   (`magnitude` a mean of non-negative `L38`, so `"+inf"` or `"NA"`, never `"-inf"`; §4 post-arithmetic).
   **Leg-2 NA (P1-6-r5):** a trial whose leg 2 is `NA` (malformed rec row, or A ledger
   `< 10` rows, §7) contributes `"NA"` at its seed position in `event_counts`;
   `present_count` counts only non-`NA` trials that had an in-window event; `magnitude`
   pools `L38` over non-`NA` in-window events only, and is `"NA"` when the group has no
   non-`NA` in-window event. **Composition (P1-9-r10):** `grief_presence` covers **exactly the
   two leg-2 conditions** — the natural death-site `amp0` and the grief-site-`m` condition
   (the only two that ship rec, §7) — in **both** courts. Court II computes leg 2 only for
   those two even though B/C write rec in every regenerated condition; the other conditions
   never enter `grief_presence`. So the array is exactly `2 conditions × 2 arms = 4` entries
   (sorted by `(condition, arm)`), Court I and Court II alike.
10. `f1` — Court II `{"ran":bool,"rec_rows":int,"max_L38":%.6f,"pass":bool}`; `null` for
    Court I. This is the **natural-leg F1 control of record** — the one whose `pass` gates the
    Court II verdict (§8/§12). `pass` is Boolean only (every not-pass/not-fail case is
    `instrument-invalid`, §11), so `max_L38` is always a finite `%.6f` here. A **matched-leg
    admission F1** (run only when the matched leg was rebuilt, §8) is **not** this field: it is
    an admission check, recorded in the `court2_binary` observation for that leg, never in
    `f1` and never gating the verdict.
11. `quality` — `{"rows_per_trial_by_group":{"<condition>_<arm>":[int × 30 in seed order]},
    "truncated_by_group":{"<condition>_<arm>":int}}`.
12. `observations` — array carrying the **required non-gating observations** (§7/§8) that
    otherwise have no home in the sealed bytes, each `{"kind":<string>,"where":<string>,
    "detail":<string>}`, sorted by `(kind, where, detail)` as byte strings — **`detail`
    is the final tie-break** (P1-4-r6), so two rows with equal `kind` and `where` still
    order deterministically. Canonical `where`/`detail` per kind (P1-10-r10). `kind ∈
    {"extra_safe_member","malformed_leg2_row","manifest_l_c","head","court2_binary"}` — an
    **extra safe archive member** (`where` = its normalized member path §7, or its **byte-safe
    tar name** (§12 special-case) when the name is not safe ASCII; `detail` = `""`);
    a **malformed leg-2 row** that made a trial's leg 2 NA (`where` = the **logical trial-dir**
    (§12 special-case, `<condition>/<seed>_<arm>`, identical in both courts); `detail` =
    `<rec-basename>:<row>`, `<rec-basename>` the rec file's basename
    e.g. `recB.log`, `<row>` its **1-based** physical row index) — every malformed row is its
    own entry, not one per trial; each dir's **`manifest.l.c` / `HEAD`** value (`where` = the
    **logical trial-dir**; `detail` = the value); and one **`court2_binary`** **per
    rebuilt leg only** (P1-3-r11 — a **natively** admitted leg produces **no** `court2_binary`
    entry; cardinality = the number of rebuilt legs, 0/1/2). `where` = the literal leg name
    `natural` or `matched`; `detail` =
    `<arch>|<compiler>|<md5>|<sha256>|<f1_rec_rows>|<f1_max_L38>|<f1_pass>`, where `<arch>` =
    the **first line of `uname -m` stdout** and `<compiler>` = the **first line of
    `cc --version` stdout** (`stderr` ignored) — "first line" = the bytes up to but not
    including the first `\n`, and a single `\r` immediately before that `\n` is dropped with it.
    **Byte gate (P1-12-r13):** in both fields every byte that is **not** a printable ASCII
    character `0x20`–`0x7E`, **and** each of `%`, `\`, `|`, is **percent-encoded** as `%XX`
    (uppercase hex; `%` encoded first as `%25`), so the field is always safe ASCII and no raw
    control, non-UTF-8, or delimiter byte reaches the hashed JSON; an **empty** `uname`/`cc`
    output yields an **empty** field (a legal value, not an error). The two hashes lowercase-hex,
    and the last three
    the **matched-leg admission-F1** result (`<f1_rec_rows>` int, `<f1_max_L38>` `%.6f`,
    `<f1_pass>` `true`) — present only for a rebuilt **matched** leg; a rebuilt **natural** leg
    writes the three literal `-` (its F1 is the control of record in `f1`, not an admission
    here). Recorded, never compared to the receipt's `binary-*-leg` rows.
    Empty array `[]` if none. **Court scope (P1-6-r12):** `manifest_l_c`, `head`, and
    `extra_safe_member` are **Court I only** — they read the shipped archives/manifests, which
    Court II does not have and does not fabricate. `court2_binary` is **Court II only**.
    `malformed_leg2_row` occurs in **both** (Court I's shipped rec, Court II's regenerated
    rec). A verifier never invents a Court-II manifest to emit a Court-I observation. These
    "reported" duties live **here**, inside the digest — never a sidecar report.

**Early-failure serialization (P1-8-r5).** When `status != "comparison-pending"` — an
interim failure (`instrument-invalid` / `raw-inconsistent` / `protocol-ambiguous`) fired
before some or all scores exist — **every top-level key is still present**, filled with
its empty / not-run form: `trials`, `floors`, `grief_presence`, `observations` = `[]`;
`wins`, `field_effect`, and the two `quality` sub-objects = `{}` (dynamic-keyed, so empty
is legal); `matched_ness`, `surrogate_selfcheck` = each with `"mean"/"sd"/"lag1"` = `"NA"`
and `"excluded_ledgers":0,"na_ledgers":0`; `f1` = `null`. `court` and `status` are as computed, and
`status_reason` is the `{"code","locus","line"}` object naming the winning defect (never
`null` here, since status ≠ `comparison-pending`). No top-level key is ever omitted; only
dynamic-keyed **inner** objects may be empty. Thus every verdict path the protocol permits
— including the earliest receipt/archive failure — serializes under one fixed schema and
one digest.

**Illustrative object (Sol #16).** Shape only — shown pretty-printed for reading (the
canonical form has no insignificant whitespace, §above), `…` marks elision, and **no
value here is of record** (all placeholders). A Court I object mid-run
(`comparison-pending`):

```
{
  "court": "I", "status": "comparison-pending", "status_reason": null,
  "trials": [
    {"condition":"forced_amp0","seed":1,"arm":"live","sum_act":3,"sum_opp":5,"rate":0.600000,"na_reason":""},
    {"condition":"forced_amp0","seed":1,"arm":"frozen","sum_act":2,"sum_opp":5,"rate":0.400000,"na_reason":""}, …
  ],
  "wins": {"forced_amp0": 0},
  "floors": [
    {"site":50,"floor_amp":"not reached","t_by_amp":{"0":"NA","0.8":1.234567, …}},
    {"site":38,"floor_amp":"not computed","t_by_amp":{}}
  ],
  "field_effect": {"forced_amp0":0.111111, …},
  "matched_ness": {"mean":0.500000,"sd":0.200000,"lag1":0.300000,"excluded_ledgers":0,"na_ledgers":0},
  "surrogate_selfcheck": {"mean":0.500000,"sd":0.200000,"lag1":0.200000,"excluded_ledgers":0,"na_ledgers":0},
  "grief_presence": [
    {"condition":"forced_amp0","arm":"live","present_count":0,"magnitude":"NA","event_counts":["NA",0, …]}, …
  ],
  "f1": null,
  "quality": {"rows_per_trial_by_group":{"forced_amp0_live":[42,40, …]},"truncated_by_group":{"forced_amp0_live":0}},
  "observations": []
}
```
The real object carries every trial and every fixed 30-element array in full, with no
`…` and no whitespace, and is hashed as those exact bytes.

**Sealing.** The verifier-writer session computes the SHA-256 of the Phase-1 bytes and
publishes that digest **before** any number of record is revealed to it. The sealed
digest and the output land together, relayed and timestamped by a third party (never
the verifier-writer).

**Phase 2 — post-reveal comparison report (assigns the verdict).** If Phase-1 `status`
was `instrument-invalid`, `raw-inconsistent`, or `protocol-ambiguous`, that status **is**
the terminal verdict and Phase 2 does not run. Otherwise the record is supplied as a
**record object** with a closed schema (P1-5-r6), the report cites the Phase-1 SHA-256 and
the record-object digest, applies the **per-field comparator** below, and assigns
`refuted` / `reproduced` (§8).

**Record-object schema (closed, per court).** Same canonicalization as Phase 1
(whitespace, key order, number/escape rules). It carries **exactly** the gating compared
fields for that court, each in the Phase-1 shape, and **no others** — an extra field
(diff-only, non-gating, or unknown), a missing gating field, or any malformed field is a
schema defect → `protocol-ambiguous` (the comparison cannot run):
- **Court I:** `{"court":"I","trials":[{"condition","seed","arm","sum_act","sum_opp","rate"} × all shipped],"wins":{"forced_amp0":int},"floors":[{"site","floor_amp","t_by_amp"} × 2],"field_effect":{…}}`.
- **Court II:** `{"court":"II","wins":{"forced_amp0":int},"floors":[{"site","floor_amp"} × 2]}`. `wins` is revealed as the **integer** count; the comparator reduces it to the `wins ≥ 20` Boolean (Court II compares the Boolean, never the count). `t_by_amp`/`rate`/`field_effect` are **absent** for Court II (not compared there, §0). **F1 has no record row** (§11 ships none): `f1.pass` is a **predeclared control assertion** verified directly on the sealed Phase-1 object (it must be `true`), never a field of the record object.

The non-gating observations (`matched_ness`, `surrogate_selfcheck`, `grief_presence`,
`quality`, `observations`) are **not** in the record object and are **not** compared: they
are sealed Phase-1 outputs, reported only (§8).

Key order and array order in the record object follow Phase 1 exactly; the record-object
digest is the SHA-256 of those canonical bytes, cited beside the sealed digest.

**Per-field comparator (Sol #18)** — replaces bare "field-by-field equality":

Each row names the **court(s)** it applies in; a comparator never runs in a court not
listed (Court II regenerates its own raw and reproduces the record only in distribution,
§0, so per-trial floats are **not compared** there):

| field | court(s) | comparator | on failure |
|---|---|---|---|
| `trials[].sum_act`, `sum_opp` | **I only** | exact integer equality | `refuted` |
| `wins.<condition>` | **I** (exact count) · **II** (the `wins ≥ 20` Boolean, per gate-bearing condition) | exact equality | `refuted` |
| `floors[].floor_amp` per site | **both** — Court I over the full shipped ladder, Court II over the predeclared regenerated subset (record floor recomputed on the identical subset/site, §6) | exact equality (numeric / sentinel), **except** `"not computed"` on either side is **never** agreement — a site left uncomputed fails the Court II floor requirement (§6 P1-1-r6) | `refuted` |
| `f1.pass` (sealed Phase-1) | **II only** — a **predeclared control**, no record operand (§11 ships no F1 record) | must be `true` | `refuted` if `false` |
| `trials[].rate`, `floors[].t_by_amp`, `field_effect` (floats backing the gate/floor) | **I only** — gating in Court I; **not compared** in Court II (§0: Court II reproduces the ensemble in distribution, not per-trial) | absolute tolerance `1e-3` on printed `%.6f` | `refuted` |

(The non-gating observations `matched_ness`, `surrogate_selfcheck`, `grief_presence`,
`quality`, `observations` are sealed Phase-1 outputs only — reported, never diffed against
a record, never verdict-bearing.)

**Sentinel comparison (P1-10-r5).** `rate`, `t_by_amp`, and `field_effect` may hold the
sentinels `"NA"`, `"+inf"`, `"-inf"`; `floor_amp` may hold `"not reached"` / `"not
computed"`. The `1e-3` absolute tolerance applies **only** when the sealed and record
values are **both finite numbers**. Otherwise the comparison is exact by value class: two
**identical** sentinels agree; a sentinel versus a different sentinel, or a sentinel
versus a finite number, **fails** (→ `refuted` where the field is gating). No gating field
is ever left with an undefined comparison.

Every **gating** comparator failure (in the court where it applies) selects `refuted`;
`reproduced` requires all gating comparators **for that court** to hold — Court I: integer
counts, `wins`, both full-ladder floors, and the gating floats within `1e-3`; Court II:
the `wins ≥ 20` outcome, both per-site floors **computed** and agreeing on the predeclared
subset, and the sealed `f1.pass` being `true`.
**Divergence from a protocol line:** if a mismatch's cause is a protocol line rather than
the raw, it is a protocol bug (§8), logged as such, and the court re-sits after a named
amendment.

---

## 13. C-sep and beyond (own court, same discipline)

After below-floor is sealed by both hands, C-sep presence re-confirm sits **its own
court section** — its own laws, its own frozen raw, the same discipline — because a
positive result passes the same court applied to the nulls; the pre-spine C-sep reading,
whatever its outcome, is not above "admitted" until re-sat here (no measured result is
named in this protocol — the intro's rule holds through §13). Then the reopened arena nulls,
then the third body (not added until the instrument has two hands).
