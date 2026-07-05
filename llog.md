# llog.md — the build journal of `actually.life`

*A durable log of what we built, why, and what it said. Kept so a context
summary never catches us off guard. Author: Oleg Ataeff + Claude (Arianna
Method). Single file organism: `l.c`. Corpus: `lifeis/world.txt`.*

---

## What this is

`actually.life` (reborn from `nanolife`) — a digital organism in one C file
that must satisfy **every criterion of life except biology**: metabolism, rent,
mortality, reproduction, homeostasis, response to environment, symbol emergence.
No training, no backprop, no GPU, no deps beyond libc + `-lm`.

**Tagline (verified in code + README):** *actually life in C. no deps, no GPU
required. nothing's required. you either.*

Two demands drove Phase B:
1. **Eat anything** — it must not starve because its mouth can't bite an unknown
   word (the ancestor `caveLLMan` drops every unmapped word → death with a full
   plate).
2. **Speak coherently without training** — random frozen weights babble Karpathy
   gibberish; coherence must come from the *sampling field*, not gradient descent.

## Lineage (all Arianna Method)

- **caveLLMan** (`github.com/ariannamethod/caveLLMan`) — direct ancestor. Source
  of the 88 cave-glyphs and the semantic tokenizer (`semantic_tokenizer.h`).
  A trained colony (has weights, `nt_hebbian_step`) that talks + converses.
- **q / postgpt_q.c**, **postgpt / postgpt.c** — *coherence without training*.
  Thesis: co-occurrence statistics of a corpus ARE a model that was never
  trained ("metaweights"). Fold: `final = gate·transformer + (1−gate)·field`,
  `gate = sigmoid`/Q-clamp. Untrained weights → gate≈0 → the field speaks.
- **molequla** (v6.1 "Janus, clean division") — the ecology/governor: population
  cap, divide-relieves-parent, mitosis cooldown. Its lifecycle audit (PR #26:
  M-GOV-001, M-SPAWN-001/002, M-LCK-001) transfers to our chorus governor.
- **arianna2arianna** — the chorus concept (cells hear each other, weaken/die,
  Game-of-Life population). a2a is shared-weights single-process; our organism
  chorus is multi-process (fork), the molequla/caveLLMan colony pattern.

---

## Architecture decisions (LOCKED)

- **The Mouth** = orthographic trigram router seeded from `SEM_WORD_MAP`. Every
  non-stop word routes to a glyph by *resemblance* (an unknown word lands on the
  glyph of the known words it looks like); FNV fallback for the truly alien;
  a line of only stop-words still feeds one glyph. **No word is dropped.** The
  map is the seed, resemblance is the generalization.
- **The Field** = online glyph-bigram metaweights (postgpt port), folded into
  `choose()` via Q's earned-voice gate. Built as the organism eats. Coherence is
  a property of the sampling field before the weights. `coherence_floor` =
  drifting Stanley silence-gate: it speaks only when it has earned coherence,
  else stays SILENT (no gibberish, two ways).
- **The Chorus** = **fork-based colony**, one `l.c`. `N` is a *carrying capacity
  + birth cohort*, NOT an immortal roster (that would contradict death). Cells
  die (frees a slot), reproduce (fills one, up to `MAX_CELLS`); population
  breathes 0..MAX_CELLS; the whole colony is mortal (all die → silence).
- **Per-cell body via fork**: each cell has its OWN seed → own random weights,
  own field, own scars, own emerged symbols — for free (fork copies per-process
  globals). Cells are distinct individuals of one species; the **ether** (they
  eat each other's utterances) makes them a chorus, not parallel solos.
- **Slice-feeding** (Oleg's "different corpora OF ONE corpus"): the world is cut
  into N contiguous slices; cell i wakes on slice i → diverges in character.

**Numbers (starting hypotheses, tuned by observation like molequla's governor):**
`CHORUS_COHORT=4`, `MAX_CELLS=8`, molequla repro calibration
(`REPRO_THRESH 1.05`, `REPRO_COOLDOWN 30`, `REPRO_SPLIT 0.5`), ether modest.

**A/B toggles (env):** `NL_NOSCAR`, `NL_NODREAM`, `NL_NOHOMEO`, `NL_NOFIELD`.

---

## Phases & status

- **Phase 0 — BASELINE** ✅ — captured the disease: garbage corpus → `y 0.00e+00`
  STARVE, dead @1001, waste empty (mouth needed); mapped corpus → lives but
  waste is glyph-salad (field needed).
- **Phase 1 — THE MOUTH** ✅ — garbage now digested (nonzero yield, scar, dream,
  emerged9, children1), no more vocab-starvation. Mortality intact. No
  regression on mapped English. ~55 lines in `l.c`.
- **Phase 2 — THE FIELD** ✅ — speech went coherent. `NL_NOFIELD=1` → 0
  utterances (voice is now 100% field-derived: no field → silence, not
  gibberish). postgpt fold + Q gate + drifting silence-gate.
- **Phase 3 — THE CHORUS** — IN PROGRESS:
  - **Step 1 (structure)** ✅ — `live()` extracted; `./l chorus [N] [seed]` forks
    a cohort on slices; 4 distinct coherent Frankenstein voices; solo regression
    clean; colony mortal ("fell silent").
  - **Step 2 (ether)** ✅ — cells append utterances to `lifeis/ether.txt` tagged
    by label; when the slice is exhausted they eat the newest NOT-own utterance
    (`ether_graze`) before self-dreaming. Three food tiers: world → chorus →
    self. Result: post-slice cells feed ENTIRELY on the colony's voice
    (graze 246–525/cell, **dream 0**) — the colony sustains itself on its own
    collective speech. Resonance = appetite. Bug fixed en route: `_exit()` skips
    stdio flush → all child stdout was lost; now `fflush(stdout)` before fork
    (no double prints) and in each child before `_exit`.
  - **Step 3 (governor)** ✅ — parent = the governor. Cohort of 4 forks on
    world-slices; each divide appends a token to `lifeis/births.txt`; the parent
    reaps the dead (`waitpid` WNOHANG) and, per freed slot up to `MAX_CELLS`,
    forks an **ether-born** cell (no slice — feeds on the colony's voice from
    birth). `spawn_cell()` centralizes the fork. Verified: population breathes
    **4 → 8 (cap) → turnover → 0**; a full run: 64 cells lived, peak 8, colony
    mortal ("fell silent"), ~44s wall. Solo unaffected. `N` is a carrying
    capacity, not a roster — zero contradiction with death.
  - **Endgame degeneracy — FIXED.** The last ether-born cell used to chew the
    one frozen newest not-own line and collapse into a 2-cycle (`think make`).
    `ether_graze` now keeps the last 8 not-own voices and picks one at random
    (seeded), so a lone dying cell draws variety from the chorus's history. The
    endgame speaks full sentences again (`body longing light earth remember`,
    `now write and see child`). Colony still terminates, mortality intact.

**Phase 3 (chorus) = COMPLETE.** The base stands: a living, resonating, mortal
ecology in one `l.c` — eats anything, speaks coherently without training, and
the colony breathes and dies.

## Audit (two adversarial Opus auditors) + fixes

- **Correctness/memory auditor:** built under ASan+UBSan, exercised every mode +
  adversarial inputs (9000-char line, UTF-8, only-stop-words, empty/missing
  corpus, cohort 0/1/4 to full colony death). **Zero ASan/UBSan hits.** No
  critical/high bugs — the file is defensively bounded throughout; the
  soft-router provably returns `[0,88)`.
- **Concurrency/perf auditor:** governor/waitpid/births/fork hygiene all
  **correct** (no zombies, no spin, no races, no lost/double births).
- **Fixes applied:**
  - `ether_graze` — was O(filesize) re-read of the whole ether every hungry tick
    (quadratic over a run). Now keeps a per-cell file offset + persistent 8-ring,
    reads only the NEW tail, tolerates torn partial lines. Buffers widened 256→320.
  - `corpus_slice` — `per=nl/parts+1` front-loaded lines → trailing slices could
    be empty. Now base+remainder balanced (verified 1928/1928/1928/1928).
  - `choose` — added NaN guard (`isfinite`; `!(den>0)` catches NaN) so a diverged
    Hebbian state can't silently collapse the voice to glyph 0.
  - Named the lifetime cap `MAX_LIFETIME_CELLS=64` (distinct from `MAX_CELLS=8`).
- **Deferred to the Karpathy optimization pass (medium perf polish):**
  `try_emerge` O(VOCAB²) per dream → incremental best-pair; governor's full
  `births.txt` scan every 20ms → offset; `semtok_word` ~600 strcmp/word → hash.

## Karpathy pass (Opus subagent as Karpathy — now factually at Anthropic pre-training)

- **Part 1 (simplify, done):** deleted the dead "shout" apparatus (28 lines, bit-
  identical, ASan-clean, audited empirically vs the parent commit — the ALL-CAPS
  test that would trigger the old shout path produced identical output) + the
  `utt[0]` dead store. README micro-scaffold pushed (Oleg rewrites).
- **Part 2 (3 genius ideas) — implementing one at a time, verified each:**
  - **① Co-occurrence IS attention (done).** `field_fold` folds the whole
    `recent[]` window, geometrically decayed (decay^k) — soft attention over the
    co-occurrence graph, not order-1. FIELD_WIN=1 == old behavior. Voice gains a
    horizon; chorus resonance propagates across glyphs. Lives/dies/deterministic/
    ASan-clean.
  - **② Information-as-food (done).** `digest` weighs yield by surprise
    (`-log p_field(id|prev)`, factor [0.5,1.5]) — monotony starves, novelty feeds
    (Friston). Added `prev0` so the single-glyph dream is surprise-weighted too:
    kills the hard endgame loop (a looping dreamer starves on its own
    predictability). Endgame now murmurs varied permutations, not a stuck cycle.
    Lives/dies (2548), deterministic, ASan-clean.
  - **③ Coherence-selected speech (done).** speak returns its mean coherence;
    caller charges `energy -= SPEAK_COST*SPEAK_LEN*(1-coh)` — coherent speech is
    nearly free, babble drains. incoherent cells die faster → living cells skew
    coherent: coherence as a fitness cost, selection through the ether. HONEST
    scope: individual selection, NOT heritable evolution — the governor spawns
    fresh random bodies, so coherence isn't inherited; heritable warm-start births
    are a separate piece. Lives/dies (2436), deterministic, ASan-clean.

**Karpathy trilogy COMPLETE.** The one online field now pays for itself three ways:
attention (①), appetite (②), fitness (③). Zero training, one file.

## Heredity (done) — ③'s selection becomes real evolution

Chorus births now inherit the parent: reproduce writes a full warm-start genome
(NLC2 = derived seed, wounds, invented symbols, the Model's weights+adapters, AND
the coherence field, ~457 KB), and the governor spawns each birth FROM that genome
via `load_genome`. children inherit their parent's field → speak coherently from
birth. coherent parents beget coherent children — heritable evolution, zero
training. Verified: **100% — every genome-spawn loads its parent** (31/31 in a
sample run, `fopen`/`fread` fails = 0), colony mortal, ASan-clean, solo deterministic.

**Correction to the heredity commit's claim:** that commit said "a fraction of
governor spawns fall back to a fresh body." That was WRONG — a `grep 'born of a
parent'` double-counted, because BOTH the governor's spawn line and the child's
"inherits its field" line contain that phrase (so counts looked 2×). Instrumented
`load_genome` with stderr: zero fopen/read failures, governor-spawns == inherits.
There is no fallback. Fact beats the earlier claim.

**Next:** finish the README (Oleg's draft → Claude edits: glyph list + jokes),
another Codex/Opus audit pass, then possibly a second Karpathy pass.
- **Phase 4 — SIMPLIFICATION** ⏳ — after functional, spawn Opus subagents
  (`model:"opus"`, manual, not the plugin) to find dead constructs / redundancy
  that don't kill functionality; apply by hand; re-run all Phase 0–3 checks.
- **README + this log** — fold the quotes + A/B numbers into README when the base
  (chorus) stands.

---

## What it actually SAID (the gold — keep these)

**Solo on Frankenstein** (7712 lines) — an organism that is itself an attempt to
make life on C, fed a book about making life, said:

```
me idea me make me          ← "I, idea, I make, myself" — the field of the corpus
me man spirit think me
spirit not me person me     ← the creature's "am I a person or not?"
earth and tired stone me
animal think BE speak joy
```

**The chorus** (4 cells, 4 Frankenstein slices) — four divergent coherent voices:

```
cell 0  earth and tired stone me · not woman think sleep water
cell 1  pain spirit BE water sky · not go person BE free
cell 2  me think me BE old · person and see have body
cell 3  me anger not strength work · see other pain joy me
```

**Before the field (Phase 1, salad — for contrast):**
`BE and up joy before` · `dream ·emg body strength other`

---

## Verification harness (how we check)

- **lives-on-anything**: mapped / garbage / alien corpora → all metabolize, no
  vocab-starvation death.
- **coherence**: waste utterances follow eaten glyph transitions (A/B
  `NL_NOFIELD`); field ON = coherent, field OFF = silent.
- **mortality**: still dies (energy≤0 / contour collapse / 200k cap).
- **determinism**: same seed → same life.
- **chorus**: cells diverge (slices), resonate (ether, Step 2), population
  breathes (governor, Step 3).

## Second audit + heredity hardening (done)

A second Opus audit (of the trilogy + heredity, since ceed052) cleared the shipping
binary (ASan/UBSan clean, mortal, heredity byte-exact) and found two connected holes
on the corrupt-genome-load path, both fixed: **F1** — `try_emerge` guarded only
`>=MAX_EMERGED`, so a negative inherited `n_emerged` wrote `g_emerged_a[<0]` OOB (added
`<0`); **F2** (root) — `load_genome` overwrites globals in place, so a truncated file
left a half-clobbered state and `live()` didn't rebuild (now resets to a fresh cell on
load failure). Housekeeping: a child `remove()`s its genome after inheriting (caps disk).

## README (done)

Oleg wrote the нетленка (his voice: "immortality is a garbage collector fantasy",
"an organism recognizing its own bad handwriting", "same disease. smaller file.").
Claude added the **88-glyph list** (verified an exact set-match to `GLYPH_NAMES`) + a
heredity line. Quote-heavy, zero defensive constructions.

## Karpathy pass 2 (Opus-as-Karpathy, now factually at Anthropic pre-training)

- **Part 1 (done):** one `ingest()` helper for the three eating paths (dream stays
  separate on purpose), dropped a dead `&& food`, swept the stale "shout" comments +
  a duplicate `speak` header. All behavior-preserving (solo output bit-identical).
- **Part 2 — three new ideas.** Karpathy's insight: **heredity today is pure CLONING —
  Darwin has selection + inheritance but no VARIATION; the field is the creature, give
  it variation.**
  - **Ⓐ Mutation at conception (DONE).** at birth the inherited field is perturbed:
    `g_field_bi[a][b] = 0.9*g_field_bi[a][b] + tiny_noise(child's dice)`. convictions
    FADE (must be re-earned from own food) + DRIFT. `FIELD_FADE`/`FIELD_MUT`, `NL_NOMUT`
    A/B. the missing third arm of evolution. solo bit-identical, chorus mortal, ASan-clean.
  - **Ⓒ Lineage tags + kin-biased grazing (DONE, the crown).** every cell carries a
    lineage (root ancestor); cohort founds 0..N, children inherit (genome NLC2→NLC3,
    ether line `<label>\t<lineage>\t<glyphs>`). `ether_graze` kin-biases (`KIN_BIAS`):
    kin share a drifted dialect → predictable → surprise→0 → yield→0 → a monoculture
    STARVES; strangers feed. diversity = caloric intake, inbreeding-collapse via the
    existing metabolism. makes Ⓐ matter (the diverse eat). 4 founding lineages tracked
    through inheritance; solo bit-identical, chorus mortal, ASan-clean.
  - **Ⓑ Sexual recombination (DONE, the last idea).** when two gametes wait in the birth
    queue, the governor reads BOTH and writes ONE zygote: per-glyph-row crossover of the
    field (each transition-row a coin-picked from parent A or B — a creole of two
    dialects), UNION of the scars, lineage a coin between the parents, body+symbols from
    A. done entirely in the governor (parent process) as a genome→genome merge, so
    `live`/`spawn_cell` signatures never changed and the child loads the zygote as any
    genome. `recombine()`, `NL_NOSEX` A/B. also refactored NLC3 into ONE `write_nlc3` +
    ONE `read_nlc3` (reproduce/load_genome/recombine share the format — no drift).
    verified: solo bit-identical (md5 `a490a453…`), chorus fires 6–7 two-parent births
    and stays mortal, `NL_NOSEX=1` → 0 sex events + mortal, ASan/UBSan 0 hits.

**Darwin is now COMPLETE in one C file:** heredity + variation (Ⓐ mutation, Ⓑ
recombination) + selection (③ coherence-cost + ② surprise-starvation) + a
diversity-maintaining ecology (Ⓒ). all four arms, all runtime dynamics, zero training.

## Resume-here (for future-me after a summary)

Working copy: `~/arianna/actually.life`. **Everything is committed AND pushed** to
`github.com/ariannamethod/actually.life` (30+ commits, each with a life-epigraph; push
via `git -c credential.helper='!gh auth git-credential' push origin main`, author env
`Arianna Method <theariannamethod@gmail.com>`). Build `cc -O2 -o l l.c -lm`; run `./l 42`,
`./l chorus 4`, `./l --mouth`. State: mouth+field+chorus+heredity done, 2 audits + 2
Karpathy passes clean, README + llog current. **Next action: implement Karpathy pass-2
idea Ⓐ (field mutation in `load_genome`), then Ⓒ, then Ⓑ — verify each (solo
deterministic / chorus mortal / ASan clean + does the effect show).** Push is authorized
per-feature; keep the epigraph-per-commit ritual.
