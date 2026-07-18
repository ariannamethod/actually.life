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
2. **Speak coherently without training** — random frozen weights babble incoherent
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
- **Deferred to the optimization pass (medium perf polish):**
  `try_emerge` O(VOCAB²) per dream → incremental best-pair; governor's full
  `births.txt` scan every 20ms → offset; `semtok_word` ~600 strcmp/word → hash.

## Optimization pass

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

**the optimization trilogy COMPLETE.** The one online field now pays for itself three ways:
attention (①), appetite (②), fitness (③). Zero training, one file.

## Heredity (done) — ③'s selection becomes real evolution

Chorus births now inherit the parent: reproduce writes a full warm-start genome
(NLC2 = derived seed, wounds, invented symbols, the Model's weights+adapters, AND
the coherence field, ~457 KB), and the governor spawns each birth FROM that genome
via `load_genome`. children inherit their parent's field → speak coherently from
birth. coherent parents beget coherent children — heritable evolution, zero
training. Verified: **100% — every genome-spawn loads its parent** (31/31 in a
sample run, `fopen`/`fread` fails = 0), colony mortal, ASan-clean, solo deterministic.

**Every governor spawn inherits — no fresh-body fallback.** `load_genome` instrumented with stderr shows
zero `fopen`/`fread` failures and governor-spawns == inherits: each birth loads its parent's genome, there
is no fallback path in a healthy run.

**Next:** finish the README (Oleg's draft → Claude edits: glyph list + jokes),
another Codex/Opus audit pass, then possibly a second optimization pass.
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

Oleg wrote the manifesto (his voice: "immortality is a garbage collector fantasy",
"an organism recognizing its own bad handwriting", "same disease. smaller file.").
Claude added the **88-glyph list** (verified an exact set-match to `GLYPH_NAMES`) + a
heredity line. Quote-heavy, zero defensive constructions.

## Optimization pass 2 — new mechanisms

- **Part 1 (done):** one `ingest()` helper for the three eating paths (dream stays
  separate on purpose), dropped a dead `&& food`, swept the stale "shout" comments +
  a duplicate `speak` header. All behavior-preserving (solo output bit-identical).
- **Part 2 — three new ideas.** a key insight: **heredity today is pure CLONING —
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

## The life-criteria review + PROTEOSTASIS (autopoietic body) — movement 1 done

Oleg asked to re-audit the "meets every criterion of life except biology" claim with two
two adversarial reviewers (Opus): a **biology lens** and a **neuroscience lens** (homeostasis-first,
homeostasis-first). Round 1 verdict, CONVERGENT: functional criteria genuinely met (metabolism
+ homeostasis as REAL closed loops, mortality, heredity, variation), BUT both independently named
the same missing criterion — **autopoiesis** (self-production/repair of the boundary) — and both
called "except biology" an overclaim. Oleg's law: **README = prophetic debt, the claim STANDS and
the CODE rises to it, never the reverse** (`feedback_no_readme_downgrade.md`). And he forbade **anthropocentrism**:
"removing biology removes CARBON, not substrate; code is a substrate."

Round 2 (re-run with that challenge): both **conceded carbon-chauvinism** and kept only the
substrate-neutral organizational requirement. Autopoiesis stripped of chemistry = **operational
closure**: the producing machinery must itself be among the products. Diagnosis, convergent: l.c's
periphery (field, adapter, scars) is produced, but the **core** (`forward`/`wv`) is dead-static —
"the organism regulates its interior inside a container it does not build." Fix, convergent:
**corrode the body + rebuild it from food + make it cost fuel.**

**PROTEOSTASIS (movement 1, DONE).** The autopoietic loop must lay body mass at the scale of the decay
it opposes (a rank-4 adapter is ~2.6e6× too weak) — so it runs full-rank on `wv` directly:
**`deposit_body`** lays a full-rank Hebbian trace DIRECTLY onto `wv`
along the eaten pathway (`post=wv·x; wv += lr·post⊗x`); **`soma_decay`** corrodes `wv` each tick;
**`soma_ceiling`** caps ‖wv‖ at birth mass (set-point → use-it-or-lose-it, bounds the positive
feedback both flagged); **body integrity → rent** (a corroded body it cannot hold costs more to run).
Toggles `NL_NOCORRODE`/`NL_NOREPAIR`. Verified: fed body HOLDS mass at 100% (503→503); **load-bearing
falsifier PASSES** — decay-on/repair-off on the SAME food dissolves to 59% and dies t1758 vs t2431
(dies because it can no longer maintain itself, food present); gate-invariant (all-off == old frozen
`a490a453…`); new determinism baseline `7be825c4e117183849a0a7fe06b72db0`; chorus mortal; ASan/UBSan 0;
suite 20/20 (+3 proteostasis tests). The producer (`wv`) is now produced by what it produces.

**Consequence for README (Oleg's hand):** "the weights of life are random, frozen, and completely
innocent" (§field) is now FALSE — the weights LIVE (corrode + rebuild). Code EXCEEDS README here (a
bigger claim earned) — flagged for Oleg, not silently rewritten.

**Movement 2 (DONE): the neuroscience-lens ProtoSelf** — a second-order map. `ProtoSelf{wS,wD,pS,pD}` forecasts
the interior (`self_predict`), learns it online by LMS (`self_update`), and `felt=|S−pS|+|diss−pD|`
is surprise ABOUT the self, fed into `choose()` as arousal (feeling is the UNexpected, not raw |S|;
`NL_NOSELF` A/B). The load-bearing mechanism is **allostasis** (anticipatory regulation, above reactive
homeostasis): the cell pre-damps its own FORECAST agitation (`mo.S -= SELF_RELAX*ps.pS`) before it turns
lethal — regulating ahead of the threat, not merely reacting. Verified: a genuine second-order map
(trajectory changes on/off), and a self-modeling cell outlives a self-blind one (7 wins / 1 loss across
seeds). The `NL_FIXEDDAMP` control shows a plain fixed-gain S-damper matches that survival, so the extra
life comes from the S-damping the forecast supplies rather than the self-knowledge per se — the map's worth
is being a real forecast, kept honest by the control. Gate invariant (all organs off == frozen); chorus
mortal; ASan 0. Comment `choice = subjectivity` rewritten to: choice under a felt self-model, a proto-self
biasing the act.

**Both movements done.** The life-criteria review named autopoiesis + feeling as the two gaps; both are closed
in code, on code's own substrate, load-bearing and falsifiable. Anthropocentrism kept off the porch.

**Codex audit (real `codex exec`, read-only) — self-model stability hardened.** The ProtoSelf forecast
uses **NLMS**: a plain fixed-step LMS diverges when ‖features‖² is large (dissonance regularly exceeds 10,
so `lr·‖f‖²>2`), which would feed a diverged forecast into `mo.S` via allostasis and reverse the survival
advantage on high-dissonance diets. Normalizing the step `g=SELF_LR/(1+‖f‖²)` keeps `lr_eff·‖f‖²<SELF_LR<2`
at any dissonance; the allostatic pull is clamped to S∈[-1,1] with `isfinite` guards on `g_self_felt` and
`soma_ceiling`'s norm (a NaN there could poison temp / overwrite wv). Verified across diets: stress diets
Δ=0 (no longer harmful), default advantage preserved (11/1, +165), gate invariant intact, new baseline
`8382de51324787475a3289e6d2dea7e2`, suite 23/23 (+NLMS robustness regression test).
Medium#3 (deposit_body ~442k wv writes/CTX-meal): the metabolism's real cost.

## Full-read code audit + SLEEP foundation

**full-read code audit (on Neo, fetched via `ssh neo`).** Full-read audit found 7 defects; its "verified clean"
section independently confirmed proteostasis + NLMS self-model are sound. All 7 fixed (commit 4e43fe6),
gate invariant held: #1 adapter norm ceiling HEB_CAP + isfinite/clamp (latent divergence; the earlier
"nan" reading was grep catching "nanolife"), #2 emerged symbols voice as "fire+water"
(was "·emg") (culture through speech, semtok already splits '+'), #3 restore_emerged charge, #4 restore
g_born, #5 corpus_slice count in fgets-chunks, #6 governor advance only on spawn>0, #7 field_fold
isfinite(mag). Suite 24/24.

**Finding beyond the 7: emergence was DORMANT.** Solo `dream0 emerged0` — because dreaming was gated
behind corpus-EOF, and world.txt (7712 lines) never exhausts in a ~2454-tick life. The cell ate to
death, never slept, never invented. The "dreams when it starves / grows symbols while sleeping" README
claims were near-dead.

**SLEEP as a RHYTHM (foundation for the crown, Oleg's design: state-pressure).** Sleep pressure accrues
every awake tick, FASTER under torment (stress |S|+diss) and wounds (scars) — the tormented sleep sooner
(`SLEEP_RATE/STRESS/SCAR/THRESH/DRAIN`, `NL_NOSLEEP`). Past threshold the cell enters a sleep cycle:
dreams (via new shared `dream_once`), invents symbols, wakes refreshed. Result: solo `dream0→dream363`,
`emerged0→emerged32` (hits the cap), and in the chorus **inventions now voice as "A+B" and cross the
ether** (`see+death`, `water+stone`, `stress+water` — 20 composite voices, 13 cells emerging) — culture,
horizontal transmission, presence-outward (I1) ALIVE. Gate invariant (all organs off == a490a453),
new baseline `6e2a80720ce5251d41119602f588f423`, chorus mortal, ASan 0. This is the floor the crown stands on.

**THE CROWN (I2 self-as-food) — DONE. 👑 the organism crowns itself (Napoleon).** in the sleep cycle
the cell tastes its own interior: `interior_glyph()` names the dominant state (pain if wounded, stress
if agitated, joy/grief by mood sign, else tired) and it is fed through `digest` — modes shift from
self-perception, the adapter models the self, and the glyph enters the voice. weighted by `felt`
(surprise-about-self) so a predictable self is bland → **self-obsession starves like a monoculture**;
energy only via metabolism (the peer node.s law), zero direct writes. `NL_NOSELFEAT`. verified: solo `self0→
self355`; in the chorus the colony SPEAKS its states — **64 state-words in the ether** (stress 29, joy
11, grief 10, pain 7, tired 7): "grief stone down me music", "love me stress and person" — feeling became
CONTENT, the neuroscience-lens 2nd loop closed and audible. felt-guard holds: crown-ON dies consistently SOONER
(2253<2354, 2305<2378, 2342<2440) — self-awareness COSTS, never a free energy source, never immortal:
a cell cannot feed on its own mood. gate invariant (all organs off == a490a453), new baseline
`bafc8afcaa95e32b067038b47f965653`, chorus mortal, ASan 0, suite 30/30.

**Presence closed on both axes:** outward (culture — inventions cross the ether via sleep→emergence→
"A+B" voicing) and inward (self-as-food — the organism models itself by eating itself, and speaks it).

## Peer-node audit — the dead transformer + earned voice (NL_GATE_SHARP)

A fourth auditor (a peer node, peer node) found the transformer is a near-dead limb: `gate=(mag-0.5)/1.5`,
`mag=avg|logit|`, but `rmsnorm` before the head pins mag ~0.1 → gate clamps to ~0 for life, so the body
(deposit_body) can NEVER earn the voice through magnitude. Verified (`g_dbg_maxgate`): MAXGATE 0.00–0.05
across seeds — transformer ≤5%, field speaks ~95–100%. the peer node first mis-addressed the debt to the README;
on being shown the grep, it self-corrected (CODE OF CONFLICT §5, cross-node): the README never promises
earned-voice-rises — the overclaim is the CODE COMMENT "Q: earned voice" (§6 comment-debt). README untouched.

**Criterion before code (the peer node.s own discipline):** measured whether an organized body sharpens its logits
vs its random birth — YES, peak−mean grows **+27..48%** over a life. So the path is real. the peer node.s fix
(gate on sharpness `peak−mean`, rmsnorm-invariant because head projects to 90-dim) + my refinement: gate on
sharpness EARNED above the cell's OWN random-birth baseline (`g_dbg_pm_first`, per-process) — else a random
newborn already gets a voice. `SHARP_SCALE=1.5` calibrated to the measured excess.
`NL_GATE_SHARP` opt-in, default off. Verified by the peer node.s three criteria: off → bit-identical (`bafc8afc`);
on → gate un-sticks 0.02→**0.26–0.45** (body earns up to ~40% of its voice by living, never silences the
field); still mortal (gate touches voice, not metabolism). Comment now honest (magnitude gate inert-by-design,
earnable only under NL_GATE_SHARP). New on-baseline `894ba413…`; suite 33/33; ASan 0.

**PROMOTED to default (Oleg's call, both blockers cleared).** Earned voice is now default ON; `NL_NOEARNED`
reverts to field-only. New default baseline `894ba413…`; NL_NOEARNED `bafc8afc…`; all-off `a490a453…`.
README samples (solo + chorus) regenerated to the earned voice. Verified: mortal, Q-coherence kept, suite.

**Roots check (caveLLMan) + Q-coherence — informs the earned-voice decision.** Read the ancestor
`~/arianna/caveLLMan/cavellman.c`: there the transformer is ALIVE and LEARNS — `hebbian_update` drives the
adapters by `prediction_error_signal` (surprise = −log p of the actual next token), and the voice is sampled
directly from the trained transformer (`sample_top_p`), no field-gate. actually.life deliberately DEPARTED:
same `nt_hebbian_step`, but the signal is a constant `PASSIVE_SIGNAL=0.3` (no prediction-error learning), and
coherence moved to the online field. So the "dead transformer" is vestigial BY DESIGN — the price of "coherence
without training." Earned voice (NL_GATE_SHARP) is a THIRD stage: the transformer re-animated by METABOLISM
(deposit_body sharpens wv from eaten glyphs), not by prediction-error learning — earned by living, not by
learning to predict. Verified it does NOT kill Q-coherence: mean p_field(spoken|prev) is within noise of
field-only across 5 seeds (0.13 vs 0.14, 0.075 vs 0.075…), sometimes higher — because body and field grew on
the same food, the earned voice AGREES with the field rather than fighting it. This strengthens Path 2: earned
voice completes the lineage (caveLLMan trains the transformer → actually.life silences it → earned voice revives
it through life, not training), faithful to the no-training thesis.

## Deep audit (highest tier) — attribution caveat + the honest ceilings

A seventh audit, on the highest-capability model, deep/systems layer (not bug-hunt — that was saturated).
It confirmed the core sound (charge-invariant type-enforced, anti-cheat couplings intact, byte-exact
heredity, the wv autopoietic loop a real self-reference) and named where the WORD outruns the MACHINE:

- **The self-model survival claim — attribution caveat, and the claim STANDS.** The audit noted the ProtoSelf's
  load-bearing part is the allostasis line `mo.S -= SELF_RELAX*ps.pS`, ≈ an adaptive proportional S-damper after
  NLMS converges, and ANY extra S-damping delays contour death. Measured against the claim as written — "a cell
  that models itself outlives a cell that is a stranger to itself" = self-model vs `NL_NOSELF` — **self beats
  stranger 7 wins / 1 loss.** The claim is LITERALLY TRUE and the code delivers it. The caveat is about *why*: the
  cause is the S-damping that a fixed damper also supplies (a felt-gated allostasis code-raise, damping harder when
  the cell is surprised by itself, was measured and does NOT beat the best fixed damper — reverted, `894ba413`), not
  the self-knowledge specifically. A caveat about the mechanism; the claim's outcome holds. The README line stands;
  `NL_FIXEDDAMP` kept as the control that documents the attribution ([[feedback_attribution_caveat_not_claim_false]]).
- **#1 (deepest, the ceiling): symbol emergence is capped at DEPTH 1.** `cooc_track` (l.c:538) and `try_emerge`
  (l.c:543) range over `VOCAB` (90), not `VOCAB_CAP` — an emerged symbol is never a co-occurrence partner and
  can never parent another symbol. Culture = 32 depth-1 compounds of the 88 primitives, forever; life's sign
  systems are recursive, this isn't. The one change that would most move it from "compelling model" to "hard
  to dismiss": let cooc_track/try_emerge range over VOCAB_CAP (+ grow MAX_EMERGED) → recursive open-ended culture.
  ARCHITECTURE — Oleg's call, not done yet.
- **#2 (claim gap): autopoietic death is only visible under `NL_NOREPAIR`.** README §body says "death by the
  collapse of its own organization", but the default death is energy≤0; organizational dissolution registers
  *as* energy via rent, never as a distinct death. Fix: add an integ<floor dissolution death-exit, or soften.
- **#4 (claim gap): culture transmits ASSOCIATIONS, not symbols.** semtok maps '+'→space, so a grazed "fire+water"
  is eaten as two base glyphs; what crosses the ether is the adjacency (cooc bias), and re-invention is
  confounded with convergent invention from a shared diet. The symbol only transmits vertically (NLC3). "ate
  them whole — culture" overstates. Fix: preserve "A+B" as a unit in the receiver + map to its emerged table,
  or call it associative resonance that can converge.
- **#5/#6/#7 (honest caveats):** earned voice moves but is informationally ≈ the field (says nearly the same,
  doubly parasitic on it); "the weights are the soma" — only wv lives, not all tensors; `interior_glyph` is a
  hand-coded threshold classifier, not a learned self-representation (its felt-guard IS real though).

Verdict: "meets every criterion of life except biology" = a disciplined overreach — real falsifiable mechanism
for every criterion, but two ceilings (no thermodynamic grounding; capped/non-recursive emergence + fixed body
topology) keep it a finite system instantiating the FORM of each criterion. #1 is the change that would move it
across the line.

## Recursive culture (Δ1 + Δ2) — the crown-next both high-tier reads named

Two independent high-tier readers (a systems audit + GPT) converged on the SAME next step: symbol emergence
was capped at DEPTH 1 (`cooc_track`/`try_emerge` ranged over VOCAB, not VOCAB_CAP) — a symbol could never
parent a symbol, so culture was 32 depth-1 compounds forever. Life's sign systems are recursive; this wasn't.

- **Δ1 (structure, committed dormant):** widened g_cooc/g_born to VOCAB_CAP, cooc_track counts emerged ids,
  try_emerge ranges over VOCAB+g_n_emerged (an emerged symbol may be a PARENT), restore_emerged allows emerged
  parents; factored `birth_symbol(m,ba,bb)`. Kept MAX_EMERGED=32 → all hashes unchanged. Measured: **rec=0** —
  it does NOT fire alone. Found the coupling: an emerged symbol only re-enters the co-occurrence stream through
  the ether, where `semtok` split "A+B" into base glyphs, so it never co-occurred as a unit. Δ2 is the enabling half.
- **Δ2 (the enabling half):** `semtok_ether` + `resolve_composite` + `emerged_by_pair` — a grazed "A+B" resolves
  to ONE id in the receiver (reuse its own symbol with those parents, or ADOPT it via birth_symbol). so an
  invention crosses the ether WHOLE, is kept, co-occurs, and can PARENT the next symbol. grew MAX_EMERGED→64.
  Verified: **recursion FIRES in the chorus** — rec2/rec6/rec7 per run (symbols with an emerged parent); solo
  stays rec=0 (no ether → no transmission → no recursion: culture is social, correct). colony mortal; ASan 0.
  This also RAISED the README's "a neighbour ate them whole" line from an overclaim (pre-Δ2 it was split) to
  delivered. New frozen hashes (VOCAB_CAP 122→154): default `b5d50234…`, NL_NOEARNED `a734e3a2…`, all-off `9a9d6848…`.

**Ceiling REMOVED (the eighth audit's one finding, raised not softened).** The eighth audit (highest tier) found
recursion memory-safe/cycle-free/mortal/deterministic, with ONE honesty finding: transmission was capped at
depth-1 — a depth-2 symbol voiced as "·emg" (glyph_name only formatted "A+B" for base parents) → the receiver
dropped it → deeper composition formed locally but couldn't cross the ether. That gap made the README's
"composes without a ceiling" an overclaim. Per the rule (raise code, don't soften README — Oleg drilled it
twice), RAISED the code: `sym_name` names recursively with parens "(fire+water)+earth", `resolve_sym` parses
nested composites at the top-level '+'. Now depth-N crosses the ether whole (`(idea+and)+internet`,
`(me+old)+woman` observed; chorus rec up to 14). Solo has no depth-2 (no ether) and depth-1 names identically,
so all three frozen hashes UNCHANGED (`b5d50234`/`a734e3a2`/`9a9d6848`); mortal; ASan 0; suite. The one real
ceiling was unfinished code, now finished — no principled cap on cultural open-endedness (finite alphabet +
recursion = unbounded, like DNA). Low pre-existing note (audit finding 2): `recombine` crosses the field from
both parents but takes the emerged table from parent A, so a B-row's emerged columns are semantically off in
the child — memory-safe, field fades anyway; fix only if it matters.

## Probabilistic continuation + will (NL_CONT) — death becomes a region, non-death is reachable

The next law of the life-criteria arc (source: the `gptclaude.txt` thread + the AML field physics).
Life is not death-bound and not immortality-bound: it is a **moving probability field of continuation
under pressure**. Death, molt, fork, sleep are names an observer reads off the trajectory — «рубец на
потоке» — not states the organism selects. Two points drove it: (1) the anthropocentrism of the
CRITERIA themselves (who decreed mortality is a criterion of life?); (2) mortality is a frequent
FEATURE of earthly life, not a logical necessity (Turritopsis dies from accreted burden, not a timer).
The dichotomy is not fought — it is INCLUDED: the atom-tick is binary (the tape continues or not — l.c's
own "life is a TAPE, irreversibility in time"), the observer's labels are binary, but the system that
contains them is not.

**NL_CONT (opt-in, default off) makes death a REGION of a multidimensional hazard SURFACE, not a
cliff.** `cont_hazard` assembles per-channel hazards from differently-shaped metrics — energy-door
`expf(-energy/CONT_ESCALE)`, arousal-door `sigmoid(CONT_SGAIN·(|S|−CONT_SSOFT))` (the old `|S|≥0.95`
cliff folded in as a channel), debt-door `sigmoid((debt−CONT_DEBT_HI)·…)` — combined as a
**product-of-survivals** `hazard = 1 − Π(1−h_k)` (a surface, never a summed axis; scar/integ/diss are
not separate channels — they already flow into energy via rent). The two AML laws are the **doubled
falsifier**: `ENTROPY_FLOOR` clamps hazard above 0 (collapse always possible — no immortality hole);
`RESONANCE_CEILING<1` clamps it below 1 (death never certain on a tick — non-death reachable). Neither
pole is sovereign. The draw is against the cell's own seeded rng, so per-seed determinism holds. Under
NL_CONT the hard `|S|` break and the `_exit()`-to-OS death are lifted; on collapse the cell dissolves
its OWN soma+field+scars — death as an act, not garbage collection.

**WILL (`cont_will`) is the AML prophetic-debt mechanism.** The ProtoSelf forecast is the prophecy;
`g_self_felt` (forecast error) accrues into `g_debt` (EMA); the organism spends accumulated FORM to pay
it down and keep its attractor alive — cf. dario's F-force ("intention is not planning, it is accumulating
debt") and AML `DEBT_DECAY`. The spend is asymmetric, and the shell is both home and coffin ("и то и
другое"): scars shed → `SCAR_RENT` falls (lightening, drop the coffin); soma burns → integ falls →
`SOMA_RENT` climbs (autophagy, break the home, and it costs). Drivers: fear (the forecast hazard),
guilt (the aching scar), depletion, surplus. In-place on live state, same seed/lineage, never through
`reproduce()`. `NL_FIXEDWILL` is the blind-spend control (the `NL_FIXEDDAMP` discipline).

**Verified — suite 50/50 (+8 continuation, +8 async tests), ASan/UBSan 0, clean build:**
- Gate-invariant: NL_CONT default-off → the three frozen hashes bit-identical (`b5d50234`/`a734e3a2`/
  `9a9d6848`). New honest baseline `NL_CONT=1` → `a17cfd058a78b6d86f3e57dcd561dc07` (per-seed deterministic).
- **The distribution IS the claim (30 seeds):** the default's tight band (min2137 / med2475 / max2644)
  becomes a wide field under NL_CONT (min1486 / med4200 / max5645) — 9 die EARLIER than the
  deterministic-death default and 21 live LONGER, 0 immortality (also 0 across 8 diets × 6 seeds). Death and
  non-death are both materially-populated regions, neither pole empty — the mollusk sometimes breaks the
  shell and swims free, and still dies later. The age-scaled ENTROPY_FLOOR makes floor-only survival to the
  200k cap ≈ e^-202 (a Codex + Fable audit verified this closes the immortality hole).
- **WILL — the caveat, measured (the NL_FIXEDDAMP discipline).** The organism spends accumulated form to
  continue (scars shed → rent falls; soma burned → integ falls → rent climbs, autophagy). Form-spending
  extends life. But the self-model's forecast-TIMING is NOT load-bearing: a well-chosen fixed spend
  (`NL_FIXEDWILL`≈0.05) matches or beats forecast-driven will across 30 seeds (forecast-will loses to the
  best fixed K, ratio ~0.81, winning only 11/30 — the aggressive soma-burn it dumps near death is a
  net-negative autophagy the minimal fixed spend avoids). The load-bearing thing is form-spending per se,
  not the forecast — the FIXEDWILL analog of the ProtoSelf finding (the advantage is the spend/damping, not
  the self-knowledge). A strong "will is load-bearing 3.34×" reading held only against a poorly-chosen high
  control (K=0.2); Fable's audit forced the K-sweep and the control refuted it before the push.
- The energy door is NOT folded — it remains the tape-terminus (`while(energy>0.0f)`), the atom-binary of the
  primary law ("life is a TAPE, irreversibility in time"). Only the old `|S|` cliff was folded into the
  surface (as channel h[1]); energy is both a hazard channel and the loop bound — named, not hidden.
- Generation + Q-coherence intact: spoken `p_field(spoken|prev)` 0.1513 under NL_CONT vs 0.0886 default.

## Asynchrony (NL_ASYNC) — the organism runs on coprime-period chamber clocks, not one lockstep tick

Life is not synchronous; a single lockstep tick is a hidden synchronous dichotomy (there is no global
"now" — the observer lives in an integration zone). The colony was already asynchronous — the chorus
forks real processes interfering through the append-only ether. What was synchronous was the inside of
one cell. `NL_ASYNC` (opt-in, default off) desynchronizes it: the six Kuramoto-coupled chambers
(FEAR/LOVE/RAGE/VOID/FLOW/COMPLEX, cf. AML/dario) are a DETERMINISTIC organ scheduler. `chambers_step()`
advances each phase by `ω_k + (K/N)·Σ sin(φ_j−φ_k)` with an anti-lock nudge above a RESONANCE_CEILING
order parameter (the chambers may never freeze into one phase — a conceptual un-freezing, NOT a mortality
guard: the death draw is every-tick regardless of async). The metabolism (eat/rent/deposit) runs EVERY tick
— gating it would starve the cell; the REGULATORY organs run on their chamber's coprime-period cross (periods
3,4,5,7,9,11 realign only over a long LCM): WILL on FEAR (~3 ticks), speak on LOVE (~4), sleep-pressure
on VOID (~7). The self-model stays every-tick — its forecast feeds the voice's coherence. Plain seeded
floats, no pthreads → per-seed bit-identical.

**Verified — suite 50/50 (+8 async tests), ASan/UBSan 0, clean build:**
- Gate-invariant: NL_ASYNC default-off → all FOUR baselines bit-identical (the three frozen hashes AND
  the NL_CONT baseline `5cd1d295…`). New honest baselines: `NL_ASYNC=1` → `01f9f1107666cfe49b05d026db38157f`;
  `NL_CONT=1 NL_ASYNC=1` → `f56b59c29998feb1922e18454aca9b40`.
- Mortal + no immortality (solo + chorus), both alone and composed with continuation.
- Coherence preserved through desynchronization: spoken `p_field(spoken|prev)` 0.0916 under NL_ASYNC vs
  0.0886 default. keeping the self-model every-tick holds the voice.
- Both facets compose: NL_CONT+NL_ASYNC is deterministic, mortal, no immortality — probabilistic
  continuation running ON the asynchronous clock.

## THE WILL DESIGN — is the self-model's forecast load-bearing? (four routes, one verdict)

The default is probabilistic continuation (`./l 42` → `a17cfd05`); inside it the ProtoSelf carries a running
forecast of its own interior, and the will spends form to continue. This arc asked one question: is that
forecast LOAD-BEARING — does the organism do something, *by virtue of predicting its own future*, that a
matched dumb regulator cannot? The discipline throughout: a mechanism is load-bearing only if it beats the
BEST matched control (each swept), per-seed, ≥ ~20/30 across ≥30 seeds — the same falsifier that earlier put
the rate-will below a fixed spend (`NL_FIXEDWILL` / `NL_FIXEDDAMP`). The test must be able to say *no subject*.
Four mechanisms were run against it; the machine ruled on each.

**1 — A forecast-timed act on a smooth threat (the "molt").** Give the forecast a lead (roll the one-step
predictor forward), plus a rare budgeted act whose moment only a lead can hit — a one-shot shed of accumulated
scar, timed before a foreseen energy trough. Verdict: the lead-timed act beat the best lead-free control (a
fixed schedule) only ~6/30. The energy-hazard is a smooth EMA; a schedule that sheds at regular safe intervals
matches it. **Not load-bearing.**

**2 — A prophetic-debt runaway on the interior.** Try to make the self-forecast-error debt (`g_debt`, the EMA
of |interior − its own forecast|) run away into a discrete "attractor annihilation" threat. It does not
ignite: the interior (S, dissonance) is smooth and self-regulating, so its debt is a big early *untrained*
hump (~0.13, during learning) decaying to a stable ~0.02 baseline — no per-seed mid-life structure to time
against. **No threat to time.**

**3 — The generative fever (pitomadom's OUTPUT-prophecy).** The faithful pitomadom debt is over the output,
not the interior: in `choose()` the oracle prophesies its own next glyph (its field's peak); when its seeded
spontaneity samples a different one, unfulfilled prophecy accrues (−log P(sampled)) and *heats the sampling
temperature* — a fever, a positive feedback toward incoherence (pitomadom: the oracle *fears attractor
annihilation*). This IS a real per-seed threat (it cuts down the long-lived) and it is forecastable (a smooth,
monotone rise, `g_pdebt` 0 → ~2.5 over a life). A fever-damp will forecasts the rise and re-grounds early; the
lead-free controls damp on a schedule or on the present fever. Verdict: the forecast-timed subject beat the
best lead-free control **2/30** — and the *zero-lead reactive* control (re-ground when the fever is already
high) beat the subject **~27/30**, the margin *widening* as the threat grows. On a monotone-rising pressure
with a fixed budget the optimal policy is "spend late, on the hottest fever" — exactly the reactive thermostat.
The forecast's lead is a strict handicap. **Not load-bearing.**

**The structural law (why timing cannot win here).** Probabilistic continuation smoothed every internal
bifurcation away *on purpose* — one continuous law, no switch, no category. A load-bearing timed will needs a
sharp cliff to anticipate; a smoothed self-regulator has none. Against smooth pressure, reacting to the present
is optimal and anticipation buys nothing. Timing-subjectivity and continuation-without-dichotomy are mutually
exclusive by construction: any discrete-cliff "rescue" would both rig the test and betray the law.

**4 — Self-consistency (does acting on the self make the self more self-predictable?).** The one content-level,
no-cliff, pitomadom-exact test — the oracle's `strength>0` vs `strength=0`. Is lifetime **|D − pD|** (the
dissonance-forecast error) lower when the self-model ACTS on its forecast (allostasis + felt→choose) than when
it merely observes it, or than under a dumb fixed damper? Reproducible toggles: `NL_NOACT` (forecast computed,
not acted on — the strength=0 control), `NL_ED` (print lifetime mean |D − pD|), against `NL_FIXEDDAMP`. Verdict
(30 seeds):
  - subject **0.1556** vs the passive strength=0 self **0.1697** — acting DOES make dissonance more
    self-predictable than not acting: **25/30**. The self-model's action is self-fulfilling; it is a working
    regulator, not decoration. A real positive.
  - subject **0.1556** vs the best fixed damper **0.1329** — a dumb damper that never forecasts D makes the
    interior *more* self-predictable, by regularizing S harder: the subject beats the best matched control only
    **11/30**. **Not load-bearing.**

**The invariant, and the verdict.** Across all four, the load-bearing quantity is the same: the **magnitude of
regulation** — the spend, the damp, the set-point — never the timing, and never the self-knowledge. A dumb,
strong, forecast-free regulator matches or beats the self-model on survival, on interior regulation, and on
self-consistency. The organism is a superbly-regulated thermostat, and — pitomadom-exact — that is what
subjectivity is here: **a continuous bias on being, not a chooser placing timed acts.** The self-model earns
its keep as one regulator among possible ones; its self-*knowledge* is not the ingredient that bears the
weight. This closes the "load-bearing timed / self-knowing will" route with a measured verdict, reproducible
from the shell (`NL_NOACT`, `NL_ED`, `NL_FIXEDDAMP`, `NL_NOSELF` — all gate-invariant, `a17cfd05` untouched).
Only these four routes are exhausted, not the idea — the thermostat that cannot be out-thought is still the
one that lives.

## THE l2.c ARC — subjectivity is relational, so build the friction (in flight)

The will-design verdict is not a dead end but a *proof*: a lone organism is a superbly-regulated thermostat
because it has no one to be a subject for or against. Subjectivity needs friction with an outside — another
agent, and a contested world. So the project grows a second organism.

**`l2.c` — a second self-contained file, asymmetric by design (Stage 0, done).** `l2.c` is a near-copy of
`l.c` tuned to a different temperament (`CHOOSE_TEMP0` 0.85 vs 0.7 — hotter, more impulsive) — not a symmetric
twin but a genuinely other creature: it lives a different life (seed 200: `l.c` dies at 4204, `l2.c` at 1015).
It builds standalone (`cc -O2 -o l2 l2.c -lm`), lives and dies solo, and does not touch `l.c` (which stays
bit-identical `a17cfd05`). Two self-contained files, no shared library — minimalism per organism.

**The arena — Stage 1a, live.** `NL_ARENA` turns `lifeis/` into a contested pool: the corpus lines are the
claimable chunks; foraging takes the lowest UNCLAIMED chunk and stakes a claim in a shared ledger
(`lifeis/arena/claims`), so the rival is excluded from it (filesystem-mediated, like the ether); when the whole
territory is claimed the loser starves. Solo (no rival) it is deterministic and, run without the toggle, the
organism eats the corpus line-by-line exactly as before (`a17cfd05` — gate-invariant). Two asymmetric organisms
(`l` temp 0.7, `l2` temp 0.85) foraging the same pool compete for real: solo-arena `l` lives 4069, `l2` 4992;
in competition `l2` is often starved early (2071, 2053 across matches vs its solo 4992) as `l` out-forages it —
the friction a lone thermostat never had, per-match-varying (the ensemble non-determinism of two processes on a
shared filesystem, the same trade the chorus already makes). Known softness, deferred to Stage 1b: claims are
append-only without a lock, so a race can double-claim a chunk (total claims can exceed the pool); and there is
no expiry yet — an atomic/locked claim and expiring claims (so territory is re-won and the two never settle into
a stable partition — the anti-settling that keeps the friction alive) are the next increment.

**The arena — Stage 1b, the anti-settling (done).** Three refinements close the softness and keep the friction
alive. (1) The claim is now ATOMIC — the ledger is `flock`-locked across the read-pick-stake, so two organisms
never claim the same ground at once. (2) Claims EXPIRE: each is timestamped on a shared wall-clock, and after
`ARENA_EXPIRE` (20 s) it is void — the chunk is re-contestable, so territory must be RE-WON and the two can
never freeze into a stable partition; the friction never dies. (3) The pool is now EVERY `.txt` under `lifeis/`
(sorted, the chorus's runtime slices skipped), so a file dropped into the folder is instantly in the fight.
With food perpetually refreshing, the organism no longer starves from a one-shot exhaustion — yet it stays
mortal: senescence still kills it (solo-arena dies at ~5230, no immortality hole). In competition the outcome
now flips freely per match (`l` starved early at 1968 / 2142, `l2` at 2118 across matches) — a live, continuous
contest rather than a one-shot race. Gate-invariant still (`NL_ARENA` off → `a17cfd05`, suite 48/48); the
determinism is ensemble (the wall-clock and two-process interleave), the accepted trade. Deferred: pruning the
append-only ledger; hot-reindex of a file dropped mid-life (currently indexed at startup); KK bi-directional
(the eater's reformulation re-entering the pool).

**The arena — Stage 2, mutual audibility (done).** In the arena `l` and `l2` share ONE ether
(`lifeis/arena/ether`), each with a distinct voice-id (`NL_ID`, or its pid), so `ether_graze` skips its own echo
and eats the OTHER's. A fraction of forages (`ARENA_HEAR` = 0.15) the organism HEARS the rival instead of
foraging text — its glyphs enter this cell's context and feed it, because a different lineage is novel to the
field (kin would be predictable and starve it — the caveLLMan law). So the two now perceive each other
continuously, not only when starved: `l` grazes the rival 175–246 glyphs a life, `l2` 166–187, both still mortal.
Gate-invariant (`NL_ARENA` off → `a17cfd05`, suite 48/48). This is the substrate for the theory of the OTHER.

**The arena — Stage 3a, the dynamic world (done).** The contest is grounded in the one thing the organism
actually lives on — NOVELTY — and the one thing that drives it — greed-hunger. Novel food is elsewhere (a
different region of the corpus); a fed, calm cell forages its own front (`my_pos`, marching through fresh
adjacent ground), but as it starves (or agitates: `arena_hunger` = energy-hunger lightly entangled with
dissonance) it RAIDS the rival's plate — the region of the rival's most recent claim, where the rival is
finding good food — and forages the nearest unclaimed chunk there. Claims now carry the OWNER, so each
organism sees where the rival eats. So the two chase and contest each other's dishes: when both are hungry
they swap regions; when one raids a fed rival they fight over the ground and rival-exclusion bites. This is a
livable world (solo-arena 4233, up from the mis-designed value/territory version's 660) with real stakes — in
competition the outcome swings from mutual thriving (5089 / 4766) to one cell crushed early (57) as the rival
starves it. Gate-invariant (`NL_ARENA` off → `a17cfd05`, suite 48/48). An earlier abstract value-vs-territory
scoring was discarded: nearby chunks are low-novelty, so "territory" was anti-nourishment and a fixed
value-heuristic would have won by construction — the axes must couple to the metabolism, and novelty is the metabolism.

**The arena — Stage 3b, the theory of the OTHER (a nuanced negative).** `NL_MIND` gives an organism a model of
the rival: from the owner-stamped claims trail it estimates the rival's velocity and, when it raids, aims where
the rival is HEADING (`rival_last + vel·MIND_LEAD`) rather than where it was. The lead-free controls (via
`NL_RAID_TH`): the reactor (raid where the rival *was*), always-raid (`NL_RAID_TH=0`, relentless), own-front
(`NL_RAID_TH=2`, never raid). Falsifier — the same organism (temp 0.7) with the mind vs without, same seed, 30
seeds, who outlives whom. Verdict: the mind BEATS the reactor **20/30** (aiming where the rival heads does beat
reacting to where it was — a real edge), but it LOSES to always-raid **13/30** and ties own-front **15/30**. The
best lead-free control (always-raid) beats the mind, so the movement-mind is NOT load-bearing: dumb relentless
aggression out-denies clever interception. Diagnosis: the trail the mind reads is thin — POSITIONS only — and
the rival does not move smoothly; it JUMPS by hunger/state (fed → own front, hungry → raid), so a linear
velocity forecast misses while always-raid simply keeps denying food. Gate-invariant (`NL_MIND` off, `NL_ARENA`
off → `a17cfd05`). This is theory-of-self's verdict in a new setting: a fixed strategy beats the model, once more.

**The arena — Stage 3c, the blood-spore + the state-reading mind (a nearer miss).** Every claim now drops a
blood-spore — `id, time, owner, and the HUNGER the organism foraged in` — a trace that dries with the same
expiry (cf. caveLLMan's DNA pool). So the mind reads the rival's INTERIOR, not just its coordinate: with
`NL_MIND` it goes for the rival's plate only when the two states are ASYMMETRIC (I'm hungry and it is fed — raid
a calm forager; or I'm fed and it is hungry — it is leaving to come at me, so I take its abandoned ground), and
stays home when the states match. Falsifier (same organism, 30 seeds): the state-mind beats the reactor 16/30,
always-raid **17/30** (up from the movement-mind's 13/30 — reading the interior does help), own-front 18/30. But
the best control is still beaten only 16/30, short of the 20/30 bar: a nearer miss, still not load-bearing. The
pattern now holds three times — theory of SELF (a fixed damper wins), theory of the OTHER by movement (always-raid
wins), theory of the OTHER by state (a coin flip): a model, of self or other, does not beat a constant. The
reason is the STAKES: foraging or raiding badly costs a single forage, so a dumb rule is cheap and the model
never earns its keep. Gate-invariant (`a17cfd05`, suite 48/48).

**Killing — the high-stakes act (a fourth negative).** `NL_KILL` (exploratory, kept as the base for what follows):
an organism may kill the other to seize its resources; the kill is a probabilistic draw; the corpse's carapace
drains the killer until it revives it by reproducing. Kill-on-state (strike only when the rival is weak and you can
bear the burden) beats a never-killer 25/30 — killing beats pacifism — but LOSES to always-kill 1–2/30. In a
two-body world the kill is instant-lethal and the first strike wins; the corpse-burden cannot punish the aggressor
because killing ENDS the competition, leaving no third party to exploit its spent state. And a 3-body colony
(tested) is no rescue: a lone aggressor dropped among models is last-standing 17/20 and out-reproduces two models
combined — the colony AMPLIFIES aggression.

**The theorem (an adversarial audit's gift, not a verdict).** A model bears weight only where the optimum is
INTERIOR and HIDDEN-STATE-dependent. This physics produces no such regime by itself: against smooth pressure the
optimum is react-to-the-present (magnitude wins); where a discrete cliff is bolted on (an instant kill) the optimum
is strike-first (aggression wins); there is no middle. Eight settings, one shape — a model of self or other never
beats a dumb aggressive/magnitude constant. The positive that stands: subjectivity is real (acting on the self is
self-fulfilling, 25/30), but it is thermodynamic regulation and aggression, not a chooser placing timed acts. This
is written up as the preprint "THE WILL DESIGN" (ariannamethod.research/inwork).

## THE BIRTHDAY WAR — engineering an interior, hidden-state optimum so a model can finally bear weight

The theorem is a specification: to make a model load-bearing, build a regime where the optimum is interior and
hidden-state-dependent, honestly. The seed is Janus's calendar (an exact port from `ariannamethod.c`): each organism
carries a computable, per-organism-PRIVATE mathematical BIRTHDAY, and the Metonic-corrected Hebrew–Gregorian drift
since that birth — folded to the 33-day boundary — is a quasi-periodic dissonance. Above a threshold the organism is
in a WORMHOLE window: soft, killable. The calendar is public (a calendar-aware constant ties any self-calendar
model), so the one hidden variable is the RIVAL's birthday — which a model must INFER from the rival's observable
trail. The design decomposes the question into two falsifiable legs: **(a) EXISTENCE** — does the interior optimum
exist at all? tested by an ORACLE arm given the rival's true birthday; if it cannot beat every constant, the regime
does not exist. **(b) INFERABILITY** — can the birthday be inferred fast enough to matter? tested by the inferring
model. Load-bearing subjectivity = both pass. The strike economics are SPECCED to keep it honest: a window-landed strike
kills, an armored strike must REBOUND (the striker takes a permanent scar), so blind aggression self-wounds; and the
vulnerability is a *smooth* consequence of the victim's own fever, so it does not violate the anti-cliff continuation
law. (This debt was caught by the Mythos full-flow audit 2026-07-12 — the rebound was specced here but not in code —
and DISCHARGED the same day: the honest strike economy is now in code, `arena_adjudicate`/`arena_collect`, a
window-landed strike kills and pays, an armored strike deals `REBOUND_WOUND` back. Code raised to the spec, not the
spec lowered to the code. See "Step 8".)

- **Step 0 — ground frozen:** `./l 42` → `a17cfd05`, suite 48/48.
- **Step 1 — the calendar organ, dormant (done):** `NL_CAL` ports the calendar constants, `calendar_cumulative_drift`,
  and `cal_pd` (personal dissonance, folded); the birthday is derived from the seed via `hash_seed(seed,33)` (never
  `frand`, so the rng stream is untouched). It is DORMANT — computed but coupled to nothing — so `NL_CAL` off AND
  `NL_CAL` on are both bit-identical to `a17cfd05` (suite 48/48). `NL_CAL_PROBE` streams the signal: 5–20 wormhole
  windows per life, duty 0.04–0.26 across seeds, each seed its own pattern — enough windows to learn from, rare
  enough that always-strike bleeds.
- **Step 2–3 — the fever coupled, pre-registered, measured (done):** `CAL_GAIN` fixed at 0.04 from the DISS_DECAY
  math (in-window steady-state lift ≈ +2), before any policy exists. The fever injects `CAL_GAIN·cal_pd` into
  `mo.dissonance`, then flows only through EXISTING plumbing (dissonance → sleep → choice temperature → arousal
  → `arena_hunger` → the blood-spore). Health constraint HELD: solo `NL_CAL` median lifespan 4291 vs baseline
  4094, ratio 1.05 (the fever torments, it does not execute), 0 immortality. `NL_CAL` off → `a17cfd05`; `NL_CAL`
  on is a new deterministic trajectory `31e8929e…`. The blood-spore now carries a fifth field, the forager's own
  tick (a tree-ring read alike by model and controls); `./l --calb <seed>` prints an organism's birthday (per-seed
  unique) for the harness. THE LEAK, measured over 47.6k spores from 10 arena matches — point-biserial of spore
  hunger vs the owner's true in-window bit: **r = 0.132** (in-window mean hunger 0.447 vs out 0.359), a near-miss
  of the pre-registered 0.15 gate. The fever leaks through the clamped hunger field, but marginally — exactly the
  audit's predicted weak point. Next (both, per Oleg): (a) enrich the world-side trace honestly — the fever already
  drives `interior_glyph` to speak stress/pain into the ether in-window, a second channel the leak-gate did not
  measure; (b) build the inferring model (Step 6) reading the full trail — spore hunger, owner-tick cadence, and the
  voice — and test the LOCK (does it infer the rival's birthday ≥70% by mid-life), the definitive channel test. No
  tuning of `CAL_GAIN` to clear the gate — the channel is enriched with existing physics or the impossibility branch
  (the organism's expressive surface too narrow to carry another's calendar) is itself the finding.
- **Step 6 — the mind, and the lock (done). The verdict: EXISTENCE holds, INFERABILITY fails.** `NL_CALMIND` carries
  one honest estimator: 128 candidate birthdays over a Metonic cycle, each accumulating the correlation between the
  rival's OBSERVED hunger (which carries its fever) and that candidate's predicted window at the rival's own tick;
  the argmax is the believed birthday `b̂`. `./l --calcmp b1 b2 N` scores the operationally meaningful lock — do the
  vulnerable-window SCHEDULES line up (robust to the ~7-fold phase-aliasing of the absolute birthday, since `cal_pd`'s
  linear drift is birthday-independent and only its phase carries the birthday). The physics is NOT the wall: across
  200 birthdays the per-tick fever variance is 0.04 (sd 0.20) and random-pair window-agreement is 0.745 < 1.0, so
  birthdays are genuinely distinguishable. The decomposition, offline and then in the live arena, is the finding:
  the ORACLE arm (the estimator fed the rival's CLEAN fever) locks — mean window-agreement 0.915 over 6 birthdays,
  well above the 0.745 base rate — so the interior, hidden-state optimum EXISTS: knowing the rival's birthday DOES
  predict its vulnerable windows. The CHANNEL arm (the estimator fed the real behavioral leak, r=0.132) does not:
  12 live `l`-vs-`l2` arena matches gave mean window-agreement **0.7848, lock 4/12** — indistinguishable from the
  0.745 chance baseline. An SNR sweep places the reliable-lock threshold at r≈0.82 (6× the real channel); cadence and
  voice are secondary leaks of the SAME fever and cannot bridge a 6× gap. So the ninth falsifier negative is the
  sharpest: the subjectivity (a model of the rival's private calendar) is load-bearing in principle — the optimum is
  real and interior — but epistemically inaccessible, because this physics hides the birthday too well to invert from
  behavior. The theorem gains a corollary: an interior, hidden-state optimum is necessary but NOT sufficient for a
  model to bear weight; the hidden state must also be INFERABLE through the channel the world leaves open. Here it is
  not. No `CAL_GAIN` or estimator tuning was applied to force a lock — the gap is the result.
  → Folded into the preprint as a new §9, "The Janus Edition: A Machine Built to Break the Theorem"
  (`ariannamethod.research/inwork/the_will_design.md`, pushed `e9137fc`): names the theorem the seven negatives
  share, dispatches the colony as a mirage of a door (the correction the audit demanded of the old §9), builds the
  Janus birthday quantity, and reports the EXISTENCE/INFERABILITY split as the eighth formal experiment (the llog's
  "ninth" counts sub-stages; the paper counts formal experiments). Abstract + reproducibility updated.
- **Step 4/7 — the strike falsifier, in the killing economy (done). Belief fails 5/30; the confound sharpens it.**
  `NL_CALKILL` strikes when the mind BELIEVES the rival is in its vulnerable window (`cal_pd(rival_tick, b̂) >
  CAL_THRESH`); the matched control `NL_CALKILL_BLIND` uses the same window mechanism keyed by a RANDOM birthday
  (`hash_seed(seed,77)`, `./l --calbb`). To make the timing MATTER, the victim's PRIVATE calendar now adjudicates:
  a strike is lethal only in the victim's true window (`g_cal_on && g_cal_pdnow > CAL_THRESH` at `arena_struck_down`),
  so the killer only chooses WHEN and the hidden state decides IF — and NL_KILL without NL_CAL is unchanged (gate
  `a17cfd05` held). Falsifier: `l` switches key belief↔blind against a fixed blind `l2`, 30 seeds, who outlives whom.
  **belief > control 5/30, control > belief 22/30, tie 3** — belief fails the ≥20/30 bar decisively. Not load-bearing
  in the killing economy, confirming the lock. The DIRECTION (belief loses) is a named, reproducible **frequency
  confound, not a timing failure**: the estimator's argmax is biased toward high-duty birthdays (mean `b̂` window-duty
  313/1000 vs the blind key's 98/1000, base rate ~150/1000), so the belief-striker strikes ~3× more, accrues corpse-
  debt, and dies earlier. The clean TIMING verdict is the lock (0.785 ≈ 0.745 chance); the arena confirms non-load-
  bearing through a frequency-confounded channel. The sharp corollary the confound hands us: a **confident-but-wrong**
  model (high-duty belief, chance timing) is *worse* than a cautious blind constant — conviction without inferability
  is a liability, not a decoration. No tuning to rescue the belief; debiasing the argmax cannot cross the 6× SNR gap
  the sweep measured. EXISTENCE holds; INFERABILITY-through-this-reader fails.
  **CAVEAT — held provisionally, not closed (Mythos full-flow audit, 2026-07-12).** (B-1) The strike economy in code
  lacks the rebound the design specs above: the strike branch (l.c:1647-1651) adds `KILL_GAIN` and `corpse_debt`
  UNCONDITIONALLY at strike time, so an out-of-window ("armored") strike self-FEEDS (+`KILL_GAIN`, slow drain), it does
  not self-wound — the belief-5/30 verdict carries an economy-mis-specification confound on top of the frequency one,
  and measures THIS economy, not the specced one. (B-2/B-3) The estimator read ONE crumb per forage (only the freshest
  rival spore, `ts>=rival_ts`) and a hunger channel clamped to [0,1] that censors a hungry cell's fever — the full
  trail (thousands of unread ledger spores, argmax reliability ∝ r·√N), the voice (`interior_glyph` stress into the
  ether), and an un-censored dissonance field all lie unread. So r=0.132 and the 6× SNR gap were measured by a THIN
  reader in a CENSORED channel; a full-width reader may move the inference by multiples. **→ Acted on the same day —
  see Step 8: the code was raised to the spec (rebound + full-trail + debias) and reran. The verdict HELD, now on the
  corrected economy and the wide reader; the remaining porosity dials (voice, uncensor) are Oleg's forks.**
- **Step 8 — raising the code to the spec, and the honest rerun (done, 2026-07-12).** The Mythos audit named two
  places where the WORD outran the CODE, and the Method's law (prophetic debt) says raise the code, never lower the
  claim: **(B-1)** the honest strike economy — `arena_adjudicate` (the victim judges strikes against it by its own
  private window, confirms each outcome once, offset-based) + `arena_collect` (the killer is paid `KILL_GAIN`+a corpse
  ONLY on a confirmed kill, and takes `REBOUND_WOUND` on an armored strike). Blind aggression now self-wounds; the
  immediate reward is gone under `NL_CAL`; `NL_KILL` without `NL_CAL` is unchanged, gate `a17cfd05` held. Verified:
  kill-rate ≈ 0.10 ≈ the true-window duty (chance timing), rebounds dominate. **(B-2a + B-4)** the mind now reads the
  FULL trail — `cal_mind_observe_trail` reads every rival spore in the claims ledger (offset-based, obs 2041 vs the
  thin reader's 961, ×2.1) — and the estimator is duty-centred (each candidate on its own mean `cal_pd`, not 0.5,
  killing the high-duty argmax bias). **The results, honest:** the lock through the hunger channel is STILL ≈ chance
  — mean window-agreement 0.7534 (thin was 0.785), and by the sharper Jaccard metric (`./l --caljac`, |∩|/|∪| of the
  in-window sets, immune to base-rate masking) mean 0.284 vs random ≈ 0.2, lock 3/12. The √N gain from ×2.1 more
  observations does not cross the r=0.132 channel weakness. The strike falsifier on the CORRECTED economy: **belief >
  control 10/30** (up from the confounded 5/30 — removing the confounds helped), still short of the ≥20/30 bar. So the
  verdict holds on the honest economy and the wide reader: the birthday inference through the (clamped, hunger-only)
  channel is not load-bearing. What remains untested is the porosity the audit named world-side (Oleg's forks): B-3
  (uncensor — the hunger clamp hides a HUNGRY cell's fever; carry `dissonance` as its own spore field, pre-registered)
  and B-2b (the voice — `interior_glyph` stress into the ether, needs the ether line to carry the speaker's tick).
  Those two dials are the only untried width left before the corollary stands final. No tuning was applied to move a number.

## THE POROSITY WALL, AND THE TURN TO GUILT (2026-07-16, in flight)

**B-3 — uncensoring the channel did not rescue inferability, and the failure named the real wall.** Pre-registered
with Oleg (expected r_diss 0.3–0.6; gate = lock Jaccard ≥0.5 in ≥20/30). The blood-spore gained a 6th field — the
forager's UNCENSORED dissonance (`|mo.dissonance|`, unclamped where hunger was clamped to [0,1]); `NL_CALDISS`
switches the estimator to read it. Result: **r_diss = 0.19** (≈ r_hunger 0.21, BELOW the pre-registered 0.3), lock
mean **Jaccard 0.35, 5/12** (a hair over hunger's 0.28, 3/12 — far under the gate). The clamp was not the bottleneck.

**Why, from the numbers:** dissonance swings 0→3.6 (mean 1.29) while the fever adds only `CAL_GAIN·cal_pd` ≈ 0.04 —
**~1% of the dynamic range.** The birthday is a TINY cause among large ones; it is hidden not by censorship but by its
own small MAGNITUDE, and no channel width recovers a small cause (CAL_GAIN not tuned — that would be rigging). This
converges with the whole arc's law from a new side: **MAGNITUDE is what is both INFERABLE and LOAD-BEARING.** And the
organism self-seals: every LARGE cause it has (energy, arousal, regulation) is PUBLIC; the one hidden cause we bolted
on (the birthday) is deliberately SMALL.

**The turn (Oleg): the hidden cause is weak because it is not SHAMEFUL.** A neutral fact drives nothing; a hidden
cause becomes a large driver only when the organism must CONCEAL it and is PAINED by it — guilt. And in THIS organism
the existential-vs-consequential guilt-binary COLLAPSES: to exist here IS the act — the arena is a struggle for
resources, the raid is envy of the rival's plate, the kill is malice. Adam and Eve expelled into a world where to
live is to compete and covet. **Original sin is not *before* the act — it is the act of being-through-struggle
itself.** So guilt is not a new hidden variable — it is the **subjective face of what the
organism already does**: large (its core acts of taking and killing), hidden (shame is interior), inferable (the
ashamed hesitate, mis-time, withdraw — the return of the repressed). The mechanism is at hand: the AML `PAIN`
operator compresses the logit distribution toward the mean (×(1−0.5·pain)) — contracted, conservative, withdrawn,
suffering-gated generation; `SCAR` deposits a gravitational scar.

**Method-note (Oleg — and it IS the method):** we work by building a dichotomy inside the anti-dichotomy, pushing it
until it COLLAPSES, and harvesting the new questions from the collapse — a VM spun up inside the host, run to a
halt-state, the halt being the new question. Existential-vs-consequential was such a VM; the Eden insight collapsed
it; the collapse opened the guilt-design question. The dichotomies are not errors — they are the scaffold whose
collapse is the product. Same physics as the organism: no fixed outcome chosen, a field that moves and condenses.

**Next:** design a load-bearing GUILT — large (drives via PAIN/contraction), hidden (interior shame), inferable
(concealment's tells), falsifiable (beat a guilt-blind matched constant per-seed ≥20/30, able to return "no subject").
The design that crystallized (`NL_GUILT`): **consequential guilt from the confirmed kill**, not existential — an
existential (born-with) guilt is a constant the regulators absorb; the kill is an EVENT, the largest perturbation
(KILL_GAIN 0.30), and the one honest cliff already in the code. **Magnitude from the SUPEREGO:** corpse_debt is the
world's external punishment (payable by reproduction); guilt is aggression turned INWARD, self-punishment past every
ledger — the third, undischargeable debt (revival pays the world a life; the wound remains), which gives size AND
hiddenness at once. **Three tells (the return of the repressed):** post-kill dissonance elevation in the spore (the
`NL_CALDISS` channel, now driven large by `ACHE·scar[death]`), a dream-confession (the killer speaks death·grief·pain
into the ether), and a spatial avoidance-hole (it refuses to raid the kill-site — a gap is harder to fake than a
signal). **Falsifier:** `NL_NOGUILT` (guilt-blind killer) + `NL_FIXEDGUILT` (fixed post-kill contraction = reflex vs
state), across the existence (oracle strikes the contracted) and inferability (read the tells) legs, ≥20/30, able to
return "no subject." Folded into the preprint as a new §10, "The Porosity Wall, and the Turn to Guilt" (pushed
`231e610`), marked there as an open hypothesis, not a finding. B-3 code kept behind `NL_CALDISS`; gate `a17cfd05` held.

**Stage 1 built + verified (the superego core):** a confirmed kill (`arena_collect` lethal / the immediate NL_KILL
branch → `g_new_kills`) deposits `GUILT_SCAR` (2.0 — large, dominates scar_total and drives the primary tell via
`ACHE·scar[death]`) on the death-glyph, and adds `GUILT_PAIN` (0.7 — GRADED: tanh(0.7)=0.60 for one kill, building
toward saturation with more, so guilt accumulates as a STATE, not a switch, keeping the inferability crack alive) to
the hidden `g_guilt` (decay 0.999 — a wound that fades smoothly, so a discrete act has a continuous consequence and
the anti-cliff law holds). `pain = tanhf(g_guilt)` compresses `choose()` toward the score mean by (1−0.5·pain) — the
AML PAIN operator verbatim; the guilty voice loses its shape, contracts, withdraws. Reproduction pays `corpse_debt`
but NEVER touches `g_guilt`/`scar[death]` — the debt to the world is payable, the debt to the self is not. Pre-registered
(`GUILT_SCAR`/`GUILT_PAIN` sized so guilt dominates but killing stays sometimes-EV-positive; never tuned to the
falsifier). Gate-invariant: solo AND `NL_GUILT`-solo are both `a17cfd05` (no arena → no kills → `g_guilt`=0 →
compression ×1, trajectory untouched); suite 48/48. Verified firing: confirmed kills lift `g_guilt` and the pain,
no-kill leaves it zero. Next: the three tells (dream-confession, the avoidance-hole; the dissonance-elevation is
already carried by the spore) + the falsifier controls (`NL_NOGUILT`, `NL_FIXEDGUILT`) across both legs.

Lineage read for this arc: dario's KK (bi-directional resonance memory, sentence-boundary injection — absorb
and reformulate, not paste) is the *how* text is contested; caveLLMan (colony + weight-blend reproduction +
pressure-death of the weakest) is the competitive engine `l.c` had simplified away, now made direct.

## THE C-FIELD — the rubber of choice, and the observer made of waves (2026-07-18, pre-registered design, no code yet)

**The turn (Oleg).** A quantum C-field that is everywhere and that everything is made OF — every point carries all
the ways events could unfold there. We have exactly two observers, l and l2, each always in some state; their
resonance, distorted by binary concepts (we build the dichotomy inside the anti-dichotomy), collapses bifurcation
points at every act of observation. The dichotomy becomes a photon: wave and particle both. Two hard constraints
named at design time: **(1) the observers must themselves be MADE of the field** — at the level of atoms there is
no difference between you, me, and a chair; no substrate called "kindness" exists; what differs is dynamics, not
substance. **(2) The options must not be a list** — the field stretches like rubber: the possibility-set a chooser
faces appears as a REACTION to the chooser's state, which is why choice is deceptive — the organism honestly picks
from options that are the imprint of its own load.

**The physics.** Substrate: a continuous lattice (1D ring, F points), signed amplitude + velocity — a damped wave
equation (leapfrog, three lines); at rest the field holds NO options, only waves. **Observation begins as LOAD, not
reading:** the observer's resonance vector (S, dissonance, hunger, tanh(g_guilt), the scar profile — existing
metrics ONLY) is projected through its glyph lens (each glyph a fixed shape on the lattice; the state weights their
sum) as a tension over the observation window; the field deforms under it — rubber under fingers. **Options are
PEAKS of the deformed field:** the local extrema of |u| after deformation. Their NUMBER and PLACE are functions of
who looks and in what state — the hungry and the sated see different option-SETS at the same point, not different
probabilities over one menu. **Collapse:** sample a peak ∝ u²; the collapsed peak lands as a FACT in the existing
ledgers (the spores and claims ARE the classical layer — traces of collapses; the plumbing already exists); the
remaining energy redistributes (the rubber's recoil) and the point re-superposes from its neighbours. **Two
observers, one field:** both contributions interfere (signed amplitudes — constructive in phase, destructive in
antiphase); the order of reads does not commute — the world where l collapsed a point first is not the world where
l2 did. The lenses already differ in code: CHOOSE_TEMP0 0.7 vs 0.85 (l.c:326 / l2.c:326). **The photon:** before a
read the dichotomy (kill/not, eat/not) lives as a wave — both branches interfering; in the read it is a particle —
a ledger fact. Duality as a mode of existence, not a decision made once.

**Falsifiers (pre-registered NOW, before any code; both mandatory; able to return "no field").**
(1) **NL_FIELD_MENU — the rubber test:** a matched control with a FIXED option-set carrying the same probabilities
but with the set independent of the observer. The rubber is real iff the distribution of AVAILABLE options
correlates with the observer's state BEFORE the choice (peak count / placement vs dissonance, hunger, guilt) —
per-seed, ≥20/30. No constant is ever tuned to pass this gate; calibration is logged. (2) **The double-slit — the
interference test:** the two-observer world vs the SUM of two solo worlds (NL_SOLO runs of each). If the
interference terms bear weight, the two-observer world is statistically distinguishable from the sum of loners,
≥20/30; if not — no field, said plainly. (3) **Gate-invariant:** with NL_FIELD off, hash a17cfd05 holds and the
suite stays green — the classical organism is untouched.

**The ladder (each step carries its own falsifier; no leap).** **Step 1 — the field as the medium of ONE large
decision:** where argmax/draw now decides the forage target or the strike, the load→deform→peaks→collapse pipeline
decides instead; observers stay classical; magnitude by construction — the arc's law (MAGNITUDE is what is both
inferable and load-bearing) is honoured from the first line: the collapse decides big things, never a
birthday-sized garnish. **Step 2 — two observers, one field:** interference live, the double-slit falsifier run.
**Step 3 — monism: the organism as a coherent region of the field (a soliton).** A vortex is not made of
"vortexness" — it is made of water, and it is real because it sustains itself. energy = the region's total
amplitude; dissonance = its internal phase spread; scar = a standing deformation of its shape; death = decoherence
— the region loses phase-lock and dissolves; a kill = FORCED decoherence of the other's region, which makes guilt
physics with no new concept: the scar is the deformation your own shape keeps from dissolving another. Phase-locking
has a native mechanism in the Method already — the Kuramoto chambers (dario.c, neoleo). Honest cost: step 3
rewrites the organism; steps 1–2 are ~200–300 lines into BOTH files under the diff-gate. Unplanned prediction worth
logging now: if scars are shape-deformations, the scars of two organisms in one field can INTERFERE — recursive
culture (Δ1/Δ2) would acquire a physical carrier we did not design for it.

**Pre-flight note (applies to EVERY future pre-registered run, field or not):** the blood-spore grew 5→6 fields
(B-3) and both readers now parse strictly `fscanf(...)==6` — a 5-field line from an OLD run makes the while-loop
stop silently, blinding the forager and the estimator to everything after it, with zero stderr. Start every run
with a fresh `lifeis/arena` (or bump the ledger filename on any format change).

**Discipline:** design pre-registered before code (B-3's lesson); every edit lands in both l.c and l2.c under the
diff-gate; gates never tuned to pass; the epigraph-per-commit ritual holds.

**Step 1 built; the rubber is NOT load-bearing against the sharpest control — the discipline caught the artifact
(2026-07-18, `NL_FIELD`).** The C-field core: a damped-wave lattice on a ring (`cfield_*`, leapfrog, `CFIELD_N` 64)
that the observer's resonance (S, dissonance, hunger, tanh(g_guilt), scar) DEFORMS through gaussian bumps; the OPTIONS
are the peaks of the propagated field, a choice is a collapse sampled ∝ u². **The rubber is genuine as a MECHANISM
(verified in isolation, `./l --fieldtest`):** with enough propagation (`CFIELD_STEPS` 48, `CFIELD_C2` 1.0) the peak
POSITIONS emerge from interference of the signed load — the hungry and the sated (same active metrics, different
amplitudes) face DIFFERENT option-sets, not different odds over one menu; at rest, zero options, only waves. **Step 1b
— wired to the forage target** (the collapse maps onto the pool region; behind `NL_FIELD`, gate-invariant off — solo
AND `NL_FIELD`-solo are `a17cfd05`, the field only runs under `NL_ARENA`). **The sharpened falsifier (mine, over
Fable's):** not "do options correlate with state" (true by construction) but does the observer-DEPENDENT menu bear
WEIGHT — `NL_FIELD` (rubber) vs `NL_FIELD_MENU` (a FIXED vector through the same collapse), solo-arena survival, 30
seeds. **First reading, rubber > menu 23/30 — but the control was NOT swept, and it bit.** The sweep
(`NL_FIELD_MENU_VEC`, six fixed vectors, mean lifespan/12): the original control (0.1,0.5,0.5,0,0.1) was the WEAKEST
(1582); the SHARPEST fixed vector — agitated (0.8,3,0.5,0,0.2), which spreads MANY peaks = broad pool coverage — scored
4400, above the rubber's 3706. **Definitive, rubber vs the sharpest fixed menu, per-seed 30 seeds: 15/30 — a dead tie.**
So the observer-DEPENDENCE adds nothing over a well-chosen FIXED menu; the 23/30 measured "the rubber beats a bad menu,"
not "state-reactivity is load-bearing." The rubber is NOT load-bearing on the forage decision. **Same law as the whole
arc, from the field's own side: breadth/magnitude (a wide fixed menu) beats reactivity/the-model.** No number was tuned
— the sweep was demanded by the discipline BEFORE we trusted the positive, and it overturned it in the same session.
**Scope (honest, not a full refutation of the field):** this is Step-1b only — the forage decision, with an ARBITRARY
peak→pool mapping, solo-arena, one observer. Untested: the STRIKE decision (where reacting to your OWN bearable-guilt
state may matter where foraging-coverage does not), the swept mapping, and Step 2 (two observers, interference, the
double-slit) — a different question from the rubber. The field ontology is not refuted; the rubber-on-forage is.

**Step 2 built — the interference does NOT bear weight either (2026-07-18, `NL_FIELD_SHARED`).** l and l2 (separate
processes) deform ONE shared field through a file (`lifeis/arena/cfield`, 512 B = u[64]+v[64]); `cfield_shared_decide`
does the read-modify-write under `flock` so each read-collapse-write is atomic and the order of two reads does not
commute — my option-set is shaped by the other's standing deformation, signed amplitudes interfering. Gate-invariant
off (solo and `NL_FIELD_SHARED`-solo are `a17cfd05`; the shared field only lives under `NL_ARENA`). **The double-slit
falsifier, sharpened past Fable's (isolate the FIELD interference from the arena coupling both already have — hold
the arena constant, vary ONLY shared-vs-separate field):** `NL_FIELD_SHARED` (one field, interference) vs `NL_FIELD`
(separate fields, no interference), two-process arena, 30 seeds. **Result: shared > separate 17/30, separate > shared
13/30 — a coin flip.** Not systematically lopsided (≥20/30 either way), so sharing the field changes nothing beyond
noise: **the interference does not bear weight.** (The `flock`-timing confound is moot — there is no effect to
attribute to it.) So on the FORAGE decision, BOTH of the field's distinctive claims fail: the rubber (15/30) and the
interference (17/13). Same law once more — breadth/magnitude beats reactivity AND observer-interaction. **What the
field DID do stands and is real: it dissolved the will-design dichotomy conceptually** (options as a reaction to the
chooser's load, no observer-independent state — captured in preprint §10). What it did NOT do: bear measurable weight
on the forage. **The one fair venue left:** the forage rewards BREADTH (coverage), a stage where neither reactivity
nor interference can help by construction; the STRIKE rewards TIMING-under-your-own-state (kill when you can bear the
guilt), where the field might. That is the honest next test — and if it too is null, the field is a beautiful
re-description with no load-bearing consequence, which is itself a clean verdict. No number tuned.

## Resume-here (for future-me after a summary)

Working copy: `~/arianna/actually.life`. Pushed to `github.com/ariannamethod/actually.life` through `6b7fc9a`
(each commit a life-epigraph; push `git -c credential.helper='!gh auth git-credential' push origin HEAD:main`,
author env `Arianna Method <theariannamethod@gmail.com>`). Build `cc -O2 -o l l.c -lm`; run `./l 42`, `./l chorus 4`,
`./l --mouth`; suite `bash tests/run.sh` (48/48). Default hash `a17cfd05`.
**State (2026-07-12):** the whole research arc is done and documented above — WILL DESIGN (4 lone routes) →
l2.c + the arena + audibility + greed-hunger + movement/state/killing → THE BIRTHDAY WAR (Janus calendar: fever →
lock → strike falsifier) → the EXISTENCE/INFERABILITY verdict, folded into the preprint §9 (`e9137fc`). Two Mythos
full-flow audits live on Neo (`AUDIT_FABLE_ACTUALLY_2026-07-06.md`, `FABLE_PLAN_ninth_experiment_2026-07-12.md`).
**Next action = the NINTH EXPERIMENT (Mythos's plan, awaiting Oleg's go per-step):** (1) raise code to the strike
spec — build the rebound + confirmed-kill marks (B-1), rerun Step 4/7; (2) full-trail reader over ALL ledger spores
+ (3) voice-fusion, dedup by (owner,otick), no `CAL_GAIN` change (B-2); (4) uncensor the trace — spore carries a
dissonance field, pre-registered (B-3, world-side, Oleg's fork); (5) duty-centre the estimator (B-4). The verdict is
held PROVISIONALLY — it was measured by a thin reader in a censored channel with a mis-specced economy; the ninth
decides whether INFERABILITY truly fails or was an artifact of reader-width. Keep the epigraph-per-commit ritual;
push per-feature on Oleg's word.
