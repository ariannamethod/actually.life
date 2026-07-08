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

**Verified — suite 42/42 (+8 continuation tests), ASan/UBSan 0, clean build:**
- Gate-invariant: NL_CONT default-off → the three frozen hashes bit-identical (`b5d50234`/`a734e3a2`/
  `9a9d6848`). New honest baseline `NL_CONT=1` → `a17cfd058a78b6d86f3e57dcd561dc07` (per-seed deterministic).
- **The distribution IS the claim:** across 10 seeds, 5 die EARLIER than the deterministic-death default
  and 5 live LONGER (NL_CONT deaths span t1486–t4637 vs the default's tight ~t2400), 0 immortality.
  Death and non-death are both nonzero regions, neither sovereign — the mollusk sometimes breaks the
  shell and swims free, and still dies later.
- **WILL is load-bearing, not decoration:** forecast-driven continuation out-distributes a blind spend
  of matched magnitude 23673 vs 8745 ticks (2.7×) — knowing WHEN to spend (from the self-model's forecast)
  beats spending blindly. The word did not outrun the machine; the control confirms it.
- Generation + Q-coherence intact: spoken `p_field(spoken|prev)` 0.1513 under NL_CONT vs 0.0886 default.

Next movement: `NL_ASYNC` — the six Kuramoto-coupled chambers as a deterministic organ scheduler
(the temporal face of the same surface), all-off hash held, its own honest baseline.

## Resume-here (for future-me after a summary)

Working copy: `~/arianna/actually.life`. **Everything is committed AND pushed** to
`github.com/ariannamethod/actually.life` (30+ commits, each with a life-epigraph; push
via `git -c credential.helper='!gh auth git-credential' push origin main`, author env
`Arianna Method <theariannamethod@gmail.com>`). Build `cc -O2 -o l l.c -lm`; run `./l 42`,
`./l chorus 4`, `./l --mouth`. State: mouth+field+chorus+heredity done, 2 audits + 2
optimization passes clean, Ⓐ/Ⓑ/Ⓒ done, tests/ suite (20/20), optimization pass 3 opts, life-criteria review
(the biology lens+the neuroscience lens) + PROTEOSTASIS movement 1 done. README + llog current EXCEPT the
"frozen weights" lines now need Oleg's hand (weights live). **Next action: movement 2 —
the neuroscience-lens ProtoSelf (second-order self-model → `felt` → `choose`, `NL_NOSELF`), verify each
(solo deterministic w/ new baseline / chorus mortal / ASan clean + survival-advantage test).**
Push is authorized per-feature; keep the epigraph-per-commit ritual. New frozen solo hash:
`7be825c4e117183849a0a7fe06b72db0` (proteostasis on); off-hash `a490a453…`.
