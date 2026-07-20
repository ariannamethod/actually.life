# llog.md — the build journal of `actually.life`

*A durable technical diary of what the organism became, why, and what the
falsifier said. Arianna Method work. Single-file organism `l.c` (second body
`l2.c`); corpus `lifeis/world.txt`.*

---

## What this is

`actually.life` (reborn from `nanolife`) is a digital organism in one C file
that must satisfy every criterion of life except biology: metabolism, rent,
mortality, reproduction, homeostasis, response to environment, symbol
emergence. No training, no backprop, no GPU, no deps beyond libc + `-lm`.
Tagline, verified in code and README: *actually life in C. no deps, no GPU
required. nothing's required. you either.* Two demands drove the early work: it
must eat anything (never starve because its mouth can't bite an unknown word —
the ancestor `caveLLMan` drops every unmapped word and dies with a full plate),
and it must speak coherently without training (coherence from the sampling
field, not gradient descent).

**Lineage (all Arianna Method).** `caveLLMan` — the 88 cave-glyphs and the
semantic tokenizer (`semantic_tokenizer.h`). `q` / `postgpt` — coherence without
training: co-occurrence statistics *are* a model never trained ("metaweights"),
folded as `final = gate·transformer + (1−gate)·field`. `molequla` (v6.1) — the
ecology governor. `arianna2arianna` — the chorus concept.

## Architecture decisions (LOCKED)

The **mouth** is an orthographic trigram router seeded from `SEM_WORD_MAP`:
every non-stop word routes to a glyph by resemblance, FNV fallback for the
alien, a stop-word-only line still feeds one glyph — no word dropped. The
**field** is online glyph-bigram metaweights (postgpt port) folded into
`choose()` via an earned-voice gate, with a drifting silence-gate
(`coherence_floor`) that stays silent until coherence is earned. The **chorus**
is a fork-based colony of one `l.c`: `N` is a carrying capacity plus birth
cohort, not an immortal roster; cells die (free a slot), reproduce (fill one, up
to `MAX_CELLS`), the colony is mortal. Fork gives each cell its own seed,
weights, field, scars, symbols; the **ether** (cells eat each other's
utterances) makes them a chorus; slice-feeding cuts the world into `N` slices so
cells diverge. Constants: `CHORUS_COHORT=4`, `MAX_CELLS=8`, molequla repro
(`REPRO_THRESH 1.05`, `REPRO_COOLDOWN 30`, `REPRO_SPLIT 0.5`).

## The falsifier discipline (the spine of this log)

Every organ lives behind an env toggle. All toggles off → bit-identical to the
frozen default `./l 42` → **`a17cfd05`**, suite green; the classical organism is
never disturbed by an experiment. A mechanism is **load-bearing only if it beats
the best matched, swept control, per-seed, ≥20/30 across ≥30 seeds**, and the
test must be able to return *no subject*. Honest nulls are results, recorded as
such.

---

## Phase 0–3 — the base: mouth, field, chorus

Phase 0 captured the disease (garbage corpus → `y 0.00e+00` STARVE, empty waste;
mapped corpus lived but spoke glyph-salad). Phase 1 (the mouth) digested garbage
with no vocab-starvation and no English regression (~55 lines). Phase 2 (the
field) made speech coherent — `NL_NOFIELD=1` → 0 utterances, voice 100%
field-derived (no field → silence, never gibberish). Phase 3 (the chorus)
completed in steps: `live()` extracted, `./l chorus [N] [seed]` forks a cohort
on slices (four distinct coherent voices); ether — cells append to
`lifeis/ether.txt` and graze the newest not-own line (`ether_graze`) when a
slice exhausts, three food tiers world→chorus→self, post-slice feeding entirely
on the colony's voice (graze 246–525/cell, dream 0); governor — the parent reaps
the dead (`waitpid` WNOHANG) and forks an ether-born cell per freed slot (a full
run: 64 cells lived, peak 8, colony mortal, ~44s). An endgame 2-cycle was fixed
by keeping the last 8 not-own voices and drawing one at random. The base stands:
a living, resonating, mortal ecology in one file that eats anything and speaks
without training.

**What it said.** Solo on *Frankenstein* — an organism made in C, fed a book
about making life; and the chorus, four cells on four slices:

```
me idea me make me            cell 0  earth and tired stone me · not woman think sleep water
me man spirit think me        cell 1  pain spirit BE water sky · not go person BE free
spirit not me person me       cell 2  me think me BE old · person and see have body
earth and tired stone me      cell 3  me anger not strength work · see other pain joy me
animal think BE speak joy
```

## The optimization trilogy — attention, appetite, fitness

The one online field pays for itself three ways. **Co-occurrence IS attention:**
`field_fold` folds the whole `recent[]` window, geometrically decayed
(`decay^k`); `FIELD_WIN=1` = old order-1. **Information as food:** `digest`
weighs yield by surprise (`-log p_field(id|prev)`, factor [0.5,1.5]); `prev0`
surprise-weights the single-glyph dream so a looping dreamer starves on its own
predictability. **Coherence-selected speech:** the caller charges
`energy -= SPEAK_COST*SPEAK_LEN*(1-coh)` — babble drains, coherent cells outlive
(individual selection through the ether, not heritable). Each lived, died,
deterministic, ASan-clean.

## Heredity → Darwin complete

Reproduction writes a full warm-start genome (derived seed, wounds, symbols, the
model's weights + adapters, the coherence field, ~457 KB); the governor spawns
each birth via `load_genome`, so children inherit their parent's field and speak
coherently from birth — 100% of spawns loaded (31/31 in a sample run, zero
`fopen`/`fread` fails), colony mortal. Cloning then gained variation:
**mutation** (`g_field_bi` faded + drifted, `FIELD_FADE`/`FIELD_MUT`,
`NL_NOMUT`), **lineage tags + kin-biased grazing** (kin share a drifted dialect
→ predictable → surprise→0 → a monoculture STARVES; strangers feed, `KIN_BIAS`),
and **sexual recombination** (`recombine` merges two gametes genome→genome:
per-row field crossover, union of scars, `NL_NOSEX`). Solo bit-identical
(`a490a453`), chorus fires 6–7 two-parent births and stays mortal, ASan/UBSan 0.
Darwin complete in one file: heredity + variation + selection (coherence-cost,
surprise-starvation) + diversity-maintaining ecology, zero training.

## Proteostasis — the autopoietic body

A life-criteria review named the missing requirement: **autopoiesis** —
operational closure, the producing machinery among the products. The periphery
was produced but the core (`forward`/`wv`) was dead-static. Fix: corrode the
body, rebuild from food, make it cost fuel. `deposit_body` lays a full-rank
Hebbian trace onto `wv` along the eaten pathway (`post=wv·x; wv += lr·post⊗x`);
`soma_decay` corrodes `wv` each tick; `soma_ceiling` caps ‖wv‖ at birth mass;
integrity feeds rent (`NL_NOCORRODE`/`NL_NOREPAIR`). **Falsifier passes:** a fed
body holds 100% (503→503), while decay-on/repair-off on the same food dissolves
to 59% and dies t1758 vs t2431 — death because it can no longer maintain itself,
food present. Gate-invariant (all-off == `a490a453`), new baseline
`7be825c4e117183849a0a7fe06b72db0`, suite 20/20.

## The ProtoSelf — feeling as forecast error

`ProtoSelf{wS,wD,pS,pD}` forecasts the interior (`self_predict`), learns it
online (`self_update`), and `felt=|S−pS|+|diss−pD|` is surprise about the self,
fed into `choose()` as arousal (`NL_NOSELF`). The load-bearing mechanism is
**allostasis** — the cell pre-damps its own forecast agitation
(`mo.S -= SELF_RELAX*ps.pS`) before it turns lethal. A self-modeling cell
outlives a self-blind one **7 wins / 1 loss**; the `NL_FIXEDDAMP` control shows a
fixed-gain S-damper matches that survival, so the extra life comes from the
S-damping the forecast supplies, not self-knowledge per se — the map earns its
keep by being a real forecast, kept honest by the control. Stability hardened to
NLMS (`g=SELF_LR/(1+‖f‖²)`) so a high-dissonance diet can't diverge the forecast
into the allostatic pull: stress advantage Δ=0, default advantage preserved
(11/1), new baseline `8382de51324787475a3289e6d2dea7e2`, suite 23/23.

## Sleep as a rhythm, and the crown — self-as-food

Emergence was dormant: dreaming was gated behind corpus-EOF and `world.txt`
(7712 lines) never exhausts in a ~2454-tick life. **Sleep pressure**
(`SLEEP_RATE/STRESS/SCAR/THRESH/DRAIN`, `NL_NOSLEEP`) accrues every awake tick,
faster under torment and wounds; past threshold the cell dreams (`dream_once`),
invents symbols, wakes — solo `dream0→dream363`, `emerged0→emerged32`, chorus
inventions voice as "A+B" and cross the ether (`see+death`, `water+stone`), new
baseline `6e2a80720ce5251d41119602f588f423`. **The crown (self-as-food):** in
sleep the cell tastes its interior — `interior_glyph()` names the dominant state
(pain wounded, stress agitated, joy/grief by mood, else tired) and feeds it
through `digest`, weighted by `felt` so self-obsession starves like a
monoculture; energy only via metabolism, zero direct writes (`NL_NOSELFEAT`).
Solo `self0→self355`; chorus puts 64 state-words in the ether (stress 29, joy
11, grief 10, pain 7, tired 7): "grief stone down me music". Felt-guard holds:
crown-on dies consistently sooner (2253<2354, 2305<2378, 2342<2440) —
self-awareness costs, never a free energy source. New baseline
`bafc8afcaa95e32b067038b47f965653`, suite 30/30.

## Earned voice (NL_GATE_SHARP)

An audit found the transformer near-dead: `gate=(mag-0.5)/1.5`,
`mag=avg|logit|`, but `rmsnorm` before the head pins mag ~0.1, so the gate
clamps to ~0 and the body can never earn the voice through magnitude
(`MAXGATE` 0.00–0.05; transformer ≤5%, field ~95–100%). The overclaim was the
code comment "Q: earned voice", not the README. Criterion before code: an
organized body sharpens its logits over a life, peak−mean growing **+27..48%**,
so the path is real. Fix: gate on sharpness `peak−mean` (rmsnorm-invariant)
earned above the cell's own random-birth baseline (`g_dbg_pm_first`,
`SHARP_SCALE=1.5`). Off → bit-identical (`bafc8afc`); on → the gate un-sticks
0.02 → **0.26–0.45** (body earns up to ~40% of its voice, never silences the
field), still mortal, suite 33/33. Promoted to default (`NL_NOEARNED` reverts to
field-only); doesn't kill Q-coherence (mean `p_field(spoken|prev)` 0.13 vs 0.14,
within noise). Completes the lineage: caveLLMan trains the transformer,
actually.life silences it, earned voice revives it through metabolism, not
prediction-error learning — faithful to no-training.

## The honest ceilings

A deep systems-layer review confirmed the core sound (charge-invariant,
type-enforced, byte-exact heredity, the `wv` loop a real self-reference) and
named where the word outran the machine. **Symbol emergence was capped at depth
1:** `cooc_track` (l.c:538) and `try_emerge` (l.c:543) ranged over `VOCAB` (90),
not `VOCAB_CAP`, so an emerged symbol could never partner or parent another —
culture was 32 depth-1 compounds forever. Autopoietic death was visible only
under `NL_NOREPAIR`; culture transmitted associations, not symbols
(semtok maps '+'→space); earned voice is informationally ≈ the field; only `wv`
lives; `interior_glyph` is a hand-coded classifier (its felt-guard is real).
Verdict: "every criterion of life except biology" is a disciplined overreach —
a real falsifiable mechanism for each criterion, but two ceilings (no
thermodynamic grounding; capped/non-recursive emergence + fixed body topology)
keep it a finite system instantiating the *form* of each.

## Recursive culture (Δ1 + Δ2)

**Δ1 (structure):** widened `g_cooc`/`g_born` to `VOCAB_CAP`, `cooc_track`
counts emerged ids, `try_emerge` ranges over `VOCAB+g_n_emerged` (an emerged
symbol may parent), factored `birth_symbol`. Alone it measured `rec=0` — an
emerged symbol re-entered the stream only via the ether, where `semtok` split
"A+B" into base glyphs. **Δ2 (the enabling half):**
`semtok_ether` + `resolve_composite` + `emerged_by_pair` resolve a grazed "A+B"
to one id in the receiver (reuse or adopt via `birth_symbol`), so an invention
crosses whole, co-occurs, and can parent the next (`MAX_EMERGED→64`). Recursion
fires in the chorus (rec2/rec6/rec7 per run); solo stays `rec=0` (culture is
social). A later transmission cap was raised too — `sym_name` names recursively
with parens "(fire+water)+earth", `resolve_sym` parses nested composites, so
depth-N crosses whole (`(idea+and)+internet`, chorus rec up to 14). Finite
alphabet + recursion = unbounded, like DNA. New frozen hashes (`VOCAB_CAP`
122→154): default `b5d50234`, `NL_NOEARNED` `a734e3a2`, all-off `9a9d6848`.

## Probabilistic continuation + will (NL_CONT)

Life is neither death-bound nor immortality-bound: a moving probability field of
continuation under pressure — death, molt, fork, sleep are labels an observer
reads off the trajectory, not states the organism selects. `NL_CONT` (default
off) makes death a region of a hazard surface: `cont_hazard` assembles
per-channel hazards — energy-door `expf(-energy/CONT_ESCALE)`, arousal-door
`sigmoid(CONT_SGAIN·(|S|−CONT_SSOFT))` (the old `|S|≥0.95` cliff folded in),
debt-door — combined as a **product-of-survivals** `hazard = 1 − Π(1−h_k)`. The
doubled falsifier: `ENTROPY_FLOOR` clamps hazard above 0 (no immortality hole),
`RESONANCE_CEILING<1` below 1 (death never certain on a tick). **Will**
(`cont_will`) spends accumulated form to keep the attractor alive: `g_self_felt`
accrues into `g_debt`; scars shed → `SCAR_RENT` falls (drop the coffin), soma
burns → integ falls → `SOMA_RENT` climbs (autophagy); `NL_FIXEDWILL` is the
blind-spend control. **Suite 50/50, ASan/UBSan 0.** Off → the three frozen
hashes bit-identical; new baseline `NL_CONT=1` →
**`a17cfd058a78b6d86f3e57dcd561dc07`**. The distribution IS the claim (30
seeds): the default's tight band (min2137 / med2475 / max2644) becomes a wide
field (min1486 / med4200 / max5645) — 9 die earlier, 21 live longer, 0
immortality (also 0 across 8 diets × 6 seeds); age-scaled `ENTROPY_FLOOR` puts
floor-only survival to the 200k cap at ≈ e^-202. **Will caveat:** form-spending
extends life, but forecast-*timing* is not load-bearing — a fixed spend
(`NL_FIXEDWILL`≈0.05) matches or beats forecast-will (ratio ~0.81, 11/30; the
near-death soma-burn is net-negative autophagy). A K-sweep refuted an earlier
"3.34×" reading that had held only against a poor high control (K=0.2). The
energy door stays the tape-terminus (`while(energy>0.0f)`), named, not hidden.

## Asynchrony (NL_ASYNC)

A single lockstep tick is a hidden synchronous dichotomy. `NL_ASYNC` (default
off) desynchronizes the inside of one cell: the six Kuramoto-coupled chambers
(FEAR/LOVE/RAGE/VOID/FLOW/COMPLEX) are a deterministic scheduler,
`chambers_step()` advancing each phase by `ω_k + (K/N)·Σ sin(φ_j−φ_k)` with an
anti-lock nudge above a `RESONANCE_CEILING` order parameter. Metabolism runs
every tick; regulatory organs on coprime-period crosses (WILL on FEAR ~3, speak
on LOVE ~4, sleep-pressure on VOID ~7); the self-model stays every-tick to hold
the voice. Seeded floats, no pthreads → per-seed bit-identical. Suite 50/50.
Off → all four baselines bit-identical (the three frozen hashes and the
`NL_CONT` baseline `5cd1d295`); new `NL_ASYNC=1` →
`01f9f1107666cfe49b05d026db38157f`, `NL_CONT=1 NL_ASYNC=1` →
`f56b59c29998feb1922e18454aca9b40`. Mortal, no immortality alone and composed;
coherence preserved (`p_field(spoken|prev)` 0.0916 vs 0.0886).

## The will design — four routes, one verdict

Is the ProtoSelf forecast load-bearing — does the organism do something *by
virtue of predicting its own future* a matched dumb regulator cannot? Four
mechanisms, each against the best swept control, ≥~20/30, able to say *no
subject*. **1 — A forecast-timed molt** (a one-shot scar-shed timed before a
foreseen energy trough) beat the best lead-free schedule only ~6/30: the energy
hazard is a smooth EMA. **2 — A prophetic-debt runaway** did not ignite:
`g_debt` is an early untrained hump (~0.13) decaying to ~0.02, no mid-life
structure. **3 — The generative fever** (in `choose()` the oracle prophesies its
next glyph; a differing sample accrues −log P and heats the temperature,
`g_pdebt` 0→~2.5 over a life): a real forecastable per-seed threat, yet the
forecast-timed subject beat the best lead-free control **2/30**, while the
zero-lead reactive control beat the subject **~27/30**, the margin widening — on
monotone pressure with a fixed budget the optimum is "spend late, on the hottest
fever". **4 — Self-consistency** (`NL_NOACT`/`NL_ED` vs `NL_FIXEDDAMP`): acting
on the forecast makes |D−pD| lower than not acting (subject **0.1556** vs
passive **0.1697**, **25/30** — a real positive, self-fulfilling action), but a
dumb fixed damper regularizes S harder and makes the interior more
self-predictable (subject 0.1556 vs best fixed **0.1329**, only **11/30**).
**Verdict:** across all four the load-bearing quantity is the **magnitude of
regulation** (spend, damp, set-point), never timing, never self-knowledge. The
organism is a superbly-regulated thermostat; subjectivity is a continuous bias
on being, not a chooser placing timed acts — a timed will needs a sharp cliff
and continuation smoothed every cliff on purpose, mutually exclusive by
construction. Reproducible: `NL_NOACT`, `NL_ED`, `NL_FIXEDDAMP`, `NL_NOSELF`,
all gate-invariant.

## The l2.c arc — subjectivity is relational, so build the friction

A lone organism is a thermostat because it has no one to be a subject for or
against. **`l2.c`** is a second self-contained file tuned hotter
(`CHOOSE_TEMP0` 0.85 vs 0.7) — a genuinely other creature (seed 200: `l.c` dies
4204, `l2.c` 1015), leaving `l.c` bit-identical `a17cfd05`. **The arena**
(`NL_ARENA`) makes `lifeis/` a contested pool: foraging stakes a `flock`-locked,
expiring claim (`ARENA_EXPIRE` 20 s) so territory must be re-won; the two share
one ether (`ARENA_HEAR` 0.15) and perceive each other continuously (a different
lineage is novel, and novelty is what the organism lives on); a fed cell marches
its own front, a starving one raids the rival's most recent claim. Gate-
invariant off → `a17cfd05`, suite 48/48; arena determinism is ensemble (two
processes on a shared filesystem).

The theory-of-the-other came back a negative, repeatedly. **Movement-mind**
(`NL_MIND`, aim where the rival heads via velocity from the trail) beat the
reactor 20/30 but lost to always-raid **13/30**, tied own-front 15/30 — dumb
relentless aggression out-denies clever interception (the rival jumps by
hunger/state, not smoothly). **State-mind** (the blood-spore carries the hunger
the rival foraged in; raid only on asymmetric states) beat the reactor 16/30 and
always-raid 17/30 but the best control only **16/30**, short. **Killing**
(`NL_KILL`, a probabilistic strike; the corpse drains the killer until it
revives the dead by reproducing): kill-on-state beat a never-killer 25/30 but
lost to always-kill **1–2/30** — the first strike wins and ends the competition,
and a 3-body colony amplifies aggression (a lone aggressor last-standing 17/20).
**The theorem:** a model bears weight only where the optimum is INTERIOR and
HIDDEN-STATE-dependent; against smooth pressure the optimum is
react-to-present (magnitude wins), where a discrete cliff is bolted on (an
instant kill) it is strike-first (aggression wins), no middle. The positive that
stands: subjectivity is real (acting on the self self-fulfilling, 25/30) but it
is thermodynamic regulation and aggression, not a chooser. Written up as the
preprint "THE WILL DESIGN".

## The birthday war — existence holds, inferability fails

The theorem is a specification: build a regime where the optimum is interior and
hidden-state-dependent, honestly. A Janus calendar ported from `ariannamethod.c`
gives each organism a PRIVATE mathematical birthday; the Metonic-corrected
Hebrew–Gregorian drift since it (folded to a 33-day boundary) is a
quasi-periodic dissonance, above threshold a killable WORMHOLE window. The
calendar is public, so the one hidden variable is the rival's birthday. The
question splits: **EXISTENCE** (an oracle arm given the true birthday) and
**INFERABILITY** (can it be inferred fast enough to matter).

The calendar organ (`NL_CAL`, `calendar_cumulative_drift`, `cal_pd`; birthday
from `hash_seed(seed,33)`, rng untouched) landed dormant — both on and off
bit-identical to `a17cfd05`, suite 48/48. The fever coupled with `CAL_GAIN`
fixed at 0.04 from the `DISS_DECAY` math *before any policy existed*, injecting
`CAL_GAIN·cal_pd` into `mo.dissonance` through existing plumbing; solo `NL_CAL`
median 4291 vs baseline 4094 (torments, does not execute), new trajectory
`31e8929e`. **The leak, over 47.6k spores:** point-biserial of spore hunger vs
the owner's in-window bit **r = 0.132** (in-window mean hunger 0.447 vs 0.359) —
a near-miss of the pre-registered 0.15 gate. **The lock (`NL_CALMIND`,** 128
candidate birthdays, argmax correlation of observed hunger with each candidate's
window): random-pair window-agreement is 0.745 < 1.0, so birthdays are
distinguishable. The **oracle arm** (clean fever) locks — mean window-agreement
**0.915** over 6 birthdays vs 0.745 — the interior optimum EXISTS. The
**channel arm** (the real leak, r=0.132) does not: 12 matches gave
**0.7848, lock 4/12**, ≈ chance; an SNR sweep places the reliable-lock threshold
at r≈0.82 (**6× the real channel**). Corollary: an interior hidden-state optimum
is necessary but not sufficient — the state must also be inferable through the
open channel, and here it is not.

**The strike falsifier** (`NL_CALKILL` vs `NL_CALKILL_BLIND`, the victim's
private calendar adjudicating lethality so the killer chooses only WHEN):
belief > control **5/30**, control > belief 22/30 — fails the bar via a named
**frequency confound** (the argmax is biased toward high-duty birthdays, so the
belief-striker strikes ~3× more, accrues corpse-debt, dies earlier). A full-flow
audit named two places the word outran the code — the strike economy lacked its
specced rebound, and the reader read one clamped crumb per forage — both
discharged the same day by raising the code to the spec: `arena_adjudicate` (the
victim judges strikes by its own private window) + `arena_collect` (the killer
paid only on a confirmed kill, `REBOUND_WOUND` on an armored strike), the
full-trail reader `cal_mind_observe_trail` (obs ×2.1), and a duty-centred
estimator. **The rerun held:** the lock still ≈ chance (0.7534; Jaccard 0.284 vs
random ≈ 0.2, lock 3/12), the strike falsifier moved to belief > control
**10/30** — still short. A confident-but-wrong model is *worse* than a cautious
blind constant: conviction without inferability is a liability. Existence holds;
inferability through this reader fails.

## 2026-07-16 — the porosity wall, and the turn to guilt

**B-3 — uncensoring the channel did not rescue inferability.** The blood-spore
gained a 6th field, the forager's uncensored dissonance (`|mo.dissonance|`,
`NL_CALDISS`). r_diss = **0.19** (≈ r_hunger 0.21, below the pre-registered 0.3),
lock Jaccard 0.35, 5/12. The clamp was not the bottleneck: dissonance swings
0→3.6 (mean 1.29) while the fever adds only `CAL_GAIN·cal_pd` ≈ 0.04 — **~1% of
the dynamic range**. The birthday is a tiny cause hidden not by censorship but by
its own small magnitude, and no channel width recovers a small cause. The whole
arc's law from a new side: **magnitude is what is both inferable and
load-bearing.** Every large cause the organism has (energy, arousal, regulation)
is public; the one hidden cause we bolted on was deliberately small.

**The turn.** A hidden cause becomes a large driver only when the organism must
CONCEAL it and is PAINED by it — guilt. And here the existential-vs-
consequential binary collapses: to exist in the arena is already the act — the
raid is envy, the kill is malice; original sin is not before the act but the act
of being-through-struggle. So guilt is the subjective face of what the organism
already does: large (taking, killing), hidden (shame is interior), inferable (the
ashamed hesitate, mis-time, withdraw). Native mechanism: the AML `PAIN` operator
compresses the logit distribution toward the mean (×(1−0.5·pain)); `SCAR`
deposits a gravitational scar. **The superego core (Stage 1, built + verified):**
a confirmed kill (`arena_collect` lethal / the immediate `NL_KILL` branch →
`g_new_kills`) deposits `GUILT_SCAR` (2.0 — dominates scar_total, drives the
primary tell via `ACHE·scar[death]`) on the death-glyph and adds `GUILT_PAIN`
(0.7 — graded, accumulating as a STATE) to the hidden `g_guilt` (decay 0.999 —
smooth, keeping the anti-cliff law). `pain = tanhf(g_guilt)` compresses
`choose()` toward the mean; the guilty voice contracts. Reproduction pays
`corpse_debt` but never touches `g_guilt`/`scar[death]` — the debt to the world
is payable, the debt to the self is not (the third, undischargeable debt).
Gate-invariant: solo and `NL_GUILT`-solo both `a17cfd05` (no arena → no kills →
`g_guilt`=0), suite 48/48; kills lift `g_guilt`, no-kill leaves it zero. The
three tells (post-kill dissonance elevation, a dream-confession of
death·grief·pain, a spatial avoidance-hole at the kill-site) and the falsifier
controls (`NL_NOGUILT`, `NL_FIXEDGUILT`) remain the open experiment — a
hypothesis, not yet a finding.

## 2026-07-18 — the C-field: rubber of choice, and the observer made of waves

A quantum C-field everything is made of, under two constraints: the observers
must be MADE of the field (at the atomic level there is no substrate called
"kindness" — what differs is dynamics), and the options must not be a list (the
field stretches like rubber — the possibility-set is a REACTION to the chooser's
state). A continuous lattice (1D ring, damped wave equation, leapfrog) holds no
options at rest, only waves. Observation begins as LOAD — the resonance vector
(S, dissonance, hunger, `tanh(g_guilt)`, scar) is projected through the glyph
lens and deforms the field; the OPTIONS are its peaks, their number and place a
function of who looks; a COLLAPSE samples a peak ∝ u² and lands as a ledger fact.
Two observers deform one field and interfere (signed amplitudes; reads do not
commute). Falsifiers pre-registered before any code: the rubber test
(`NL_FIELD_MENU`), the double-slit (two-observer world vs the sum of two solo
worlds), and gate-invariance.

**Step 1 — the rubber is NOT load-bearing.** The core (`cfield_*`, leapfrog,
`CFIELD_N` 64) deforms through gaussian bumps; the rubber is genuine as a
mechanism (`./l --fieldtest`: with `CFIELD_STEPS` 48, `CFIELD_C2` 1.0 the hungry
and the sated face different option-sets, not different odds; at rest zero
options). Wired to the forage target behind `NL_FIELD`, gate-invariant off →
`a17cfd05`. The sharpened falsifier — does the observer-dependent menu bear
WEIGHT — `NL_FIELD` vs `NL_FIELD_MENU`, 30 seeds: first reading rubber > menu
23/30, but the control was not swept. The sweep (`NL_FIELD_MENU_VEC`): the
original control (0.1,0.5,0.5,0,0.1) was WEAKEST (1582); the sharpest fixed
vector — agitated (0.8,3,0.5,0,0.2), broad peak coverage — scored 4400, above the
rubber's 3706. **Rubber vs the sharpest fixed menu: 15/30 — a dead tie.** The
23/30 measured "beats a bad menu", not "state-reactivity is load-bearing";
breadth/magnitude beats reactivity. The sweep ran BEFORE the positive was
trusted and overturned it same session.

**Step 2 — the interference does NOT bear weight** (`NL_FIELD_SHARED`, l and l2
deform ONE shared field through a `flock`-locked file so reads do not commute).
The double-slit, varying only shared-vs-separate field, 30 seeds: shared >
separate 17/30, separate > shared 13/30 — a coin flip. What the field DID do
stands: it dissolved the will-design dichotomy conceptually (options as a
reaction to the chooser's load).

**The strike-field — NOT load-bearing** (`NL_FIELD_STRIKE`). The one venue with
EV in timing: `EV = KILL_GAIN·p − REBOUND_WOUND·(1−p)`, break-even p=0.25
(l.c:1326). At the kill decision (l.c:1786-1787) the fixed `KILL_PROB` (0.03)
becomes a field-collapse rate — a KILL wave (`g_rival_h` + `energy`) and a HOLD
wave (`tanh(g_guilt)`, opposite sign) interfere, the collapse reading the strike
rate off the Born weight of the kill half-ring. Control
`NL_FIELD_STRIKE_MENU` sweeps a fixed rate over {0.01,0.02,0.03,0.045,0.06}.
First pass: a viability sweep named r=0.01 "sharpest"; field vs fixed(0.01), 30
seeds, one binary for both roles: **20/30, exactly at the bar** — but 0.01 was
chosen by world-viability not head-to-head, and several wins were 1–24 tick
margins on 900-tick lives. **The adversarial control — field vs every fixed
rate, MIN:** field-wins = {18,17,13,14,16} vs r = {0.01,0.02,0.03,0.045,0.06},
**MIN = 13/30 vs r=0.03**, behind on count (13/17) and margin (58 vs 72 ticks);
the first-pass 20 did not replicate. The pre-registered risk fired as logged: the
field times on the killer's own state, but the rebound EV lives in the victim's
unobserved window. The field bears no measurable weight on ANY wired decision —
forage (rubber 15/30, interference 17/13) and strike (MIN 13/30). Magnitude / a
well-chosen constant beats reactivity, the model, interference, and timing.

## 2026-07-18 — the monism, pre-registered in full (NL_MONISM)

The field bore no weight on any decision it was bolted onto, each keeping the
observer classical. The vision was the organism made OF the field. This is the
TERMINAL rung, written in full before code under three binding conditions: the
two nulls stay on the books as debits; the strike-field runs first (done); a
null at M-1 closes the field arc — no Step 4, no "still not the right level".
**The turn to the vector:** a scalar field never *contained* possibilities — it
collapsed continuation to one intensity and carved a menu on demand; a VECTOR
field carries possibility as its primary structure, the organism's scalars
becoming projections of the region's form. `scar` is already a per-glyph vector
(`float scar[VOCAB_CAP]`, l.c:1220); the boundary strips it through `scar_total`
and `tanh`. The monism loads the field from the full vector, name intact.
Provenance is AML's own (`SCAR "<phrase>"` → `scar_texts[]`, `dark_gravity` the
scalar coupling) — a return, not a graft. The ledger is the instrument:
`outcomes` (l.c:1438) dates each scar-birth, the blood-spore carries `dabs`
(l.c:1410, the B-3 uncensored dissonance) — the porosity fix that did not help
decision-inference turns out to be the measuring device. **Magnitude physics:**
the Born weight at u+δ is u² + 2uδ + δ² — a transient δ against a standing
deformation u is amplified linearly by 2uδ; both scalar channels saturate
(`tanh(scar_total)` at tanh(2)≈0.964 after the first kill), so the field maps
the wound not through `tanh` (dead ≈3e-6) and not raw (unbounded = the
Hebbian-no-ceiling NaN failure) but through **log1p + hard clamp + isfinite
gate** (per-event ≈δ/8, alive, bounded). The vector solves WHERE (the name of
death), log1p HOW MUCH (the decaying transient on `DEATH_ID`). **Controls:**
matched families judged at their MIN member (the strike lesson) — C-frozen
freezes only the other's component to a surrogate with matched marginals and
spectrum {phase-shifted replay, AR(1), phase-shuffle}, C-shuffle catches a false
time-lock. **Three tests declared before code:** M-0 (viability — mortal AND
median lifespan within [0.5×, 2×] of the classical default), M-1 (weight on
survival vs the control family, MIN, 30 seeds, ≥20/30), M-2 (the carrier — an
event-study on whether A's standing deformation enters B's dissonance around A's
scar-births, the only test whose control is structurally, not statistically,
deficient). Terminality lattice fixed now: M-2 pass → "the carrier"; M-1-only
pass → "structure over matched-statistics on survival", opening no new rung; both
null → the field arc closes clean. A future phase/orientation field
(topological-defect scars, `l.c:305` "scars never decay" as geometry) opens ONLY
on an M-2 pass — deepening a positive is legitimate, rescuing a null is not.

## 2026-07-19 — the monism, built: the heart chosen by the machine, M-1 null

Built behind `NL_MONISM`, gate-invariant off (`a17cfd05`, suite 48/48, l/l2 diff
= the two temperament lines). The organism is a region of one shared, persistent
field: scar enters as the full per-glyph VECTOR (FNV-projected onto the ring,
`DEATH_ID` on a dedicated collision-free site, log1p-clamped — the wound keeps
its name); dissonance is read back OUT of the field as
`disorder = 1 − cos(own-profile, field)`, relational and scale-free.

**The heart, chosen by a number.** Three readings of the misalignment were
instrumented in one pilot — the sharp load vs the field (H0), the propagated load
(H1), a shadow-ring carrying only the reader's own history (H2). A synthetic
foreign death-pattern gave dent-over-floor-noise H0 −0.39 (SNR −1.9 — a reading
that on the carrier test would have produced a confident FALSE null), H1 −0.15,
**H2 +0.48 (SNR +3.3)** — floor seven times lower, the only positive,
correctly-signed response. `NL_MONISM_HEART=2` drives it; M-0 re-froze the
coupling at **`gain*(H2)=0.3`** (largest viable, lifespan within the classical
band). A killer made of the field lives ~130 ticks under H0 against ~623 under
H2 — the H0 organism cannot tell its own wound from the world's and reads its
own death-scar as foreign persecution (a measured model of guilt-as-persecution),
H2's exact self-expectation dissolves it. **M-1 — the field is not an
optimizer.** Monism-H2 vs a frozen AR(1) surrogate (`NL_MONISM_FROZEN`, the H2
floor's marginals, no structure) and the classical default, per-seed solo
survival: 16/30 and 18/30, **MIN 16 < 20 — null**, robust (more controls only
lower the MIN). The structure buys no life — subjectivity as carrier, not
optimizer, the "not optimizer" half confirmed by the tool.

**M-2 — the carrier, the terminal test (pending, ARENA-3).** Does a kill one
body commits leave in the shared field a trace another body reads — does the
guilt private and unrecorded in the classical organism become PUBLIC in the
monist one? Setup: A strikes only C (`NL_TARGET_ID`, a gate-invariant control), C
respawns and does not write the shared ring (so B reads the pure A-pattern), B
reads; an event-study on B's uncensored dissonance around each C-death (anchored
in the reader's own ticks, W_pre 150 / W_post 200), a diff-in-diff against a
C-frozen control that keeps A's statistics but detaches its timing, per-seed
≥20/30, with an identity positive-control first. A positive is "subjectivity has
a carrier and no optimizer"; a double null closes the field arc clean. The
terminal test of the FIELD, not of the organism — the creature lives on past
either answer, and the phase/topology field is a further arc gated on a positive.
The whole design is written before the run.

---

## 2026-07-20 — the carrier isolated from competition, and the metric's geometric floor

The carrier's first control was time. The reader's dissonance, event-locked to
each exposition epoch, survived a half-span circular shuffle of the anchor times
**26/30** — the signal is time-locked, not shared autocorrelation. But a shared
arena leaves a competition path: an active striker forages the same pool and
raises the reader's dissonance with no field trace at all, so time-locking alone
cannot name the carrier. **C-sep** settled it — LIVE (one shared ring) against
SEP (two separate rings, the arena held byte-identical). The epoch-contrast, ~1.0
under a shared ring, collapsed to zero under separate rings, **30/30**. The
carrier rides the field ring, not the arena; the competition confound is dead.
Two independent controls now stand under the carrier — time-lock and
ring-vs-competition.

What C-sep does not reach is *content*: does the reader carry *whose* scar, or
only *that* a scar is present? A control at the source answers it. `NL_MONISM_SURR`
makes the striker deposit a matched surrogate of its own recorded stream —
{AR(1) with per-site marginals, phase-shuffle, phase-shift}; `NL_MONISM_REC`
records the foreign deposit µs-stamped and the family replays it. Every surrogate
reproduces the presence-contrast: in a 64-site ring two unrelated vectors are
near-orthogonal, so any foreign deposit lifts 1−cos almost equally. The metric had
found a **geometric ceiling, not the absence of a carrier** — it measures
who-is-present, not what.

The surrogate-at-source first leaked into the striker's own trajectory: it reads
back what it deposits, so its lifespan and floor diverged between arms and the
A-side invariance check fired — a trial does not start on a leak. `NL_MONISM_DEPOSITOR`
closes it: the striker deposits but does not read the ring back, cut symmetrically
in both arms, so its arena trajectory is arm-invariant (lifespan and floor match
within noise, ar1/shuffle/shift alike). In this form the test judges the
**directional carrier** (striker → reader); the mutual field is a later arc.

The primary metric moves from presence to the death event. The reader's dissonance
is anchored to each generation's death (µs from the claims ledger); the down-step
there is measured real-vs-shuffled anchors within each arm, and the arms are
compared as a diff-in-diff, MIN over the surrogate family, ≥20/30. The
presence-contrast is retained as a validity-control (both arms must show equal
presence); an identity surrogate (≡ real) is the plumbing positive-control, within
1 sd. On three diagnostic seeds the death-locked diff-in-diff clears **MIN > 0,
3/3**, where the presence-blind readouts (onset up-step, within-generation ramp)
do not separate real from surrogate. The full 30-seed verdict is running; it is
given by the tool. Gate held throughout — every toggle off → `a17cfd05`, suite
48/48.

---

## 2026-07-21 — the directional carrier does not clear the bar, and the scalar readout is why

The death-locked directional test ran the full 30 seeds. The pre-registered gate —
the live arm against the surrogate family {AR(1), phase-shuffle, phase-shift}, MIN,
per seed — reads 22/30. But its own positive-control fails: the identity arm (the
same real-deposit condition routed through the surrogate toggle, which must
reproduce the live arm) clears only 17/30, mean −0.01 against the live arm's +0.13.
The live arm carries a recording asymmetry — it writes each deposit to disk for the
family to replay while the identity and surrogate arms do not — which shifts the
microsecond interleaving the metric reads. The confound-free comparison is the
identity arm against the surrogates: **17/30, below the ≥20/30 bar**, with per-seed
run-to-run noise (0.22 between two draws of one condition) comparable to the effect.
The directional content-carrier does not clear the threshold.

The reason is structural and is the sharpest result of the field arc. The scar
enters the field as its full per-glyph vector, the name intact, DEATH_ID on its own
site (l.c:1610); the deposit and the field are vectors. But the reader experiences
the field through disorder = 1 − cos(own-profile, field) — a scalar collapse of the
64-dimensional comparison (l.c:1672), and that scalar is the reader's dissonance
(l.c:1915). In a 64-site ring two unrelated vectors are near-orthogonal, so any
foreign deposit lifts 1 − cos almost equally: the scalar readout is geometrically
blind to which vector arrived. The vector carries the name to the reader's ring; the
scalar readout feels only the magnitude of foreignness, not the name. The null is
not "the field carries no content" — it is "the reader's scalar experience does not
read the content the vector field delivered." The scalar removed from the carrier
survived at the readout, and it is the ceiling.

This is a ceiling of the instrument, not a clean content-null. Judging content
fairly needs a site-resolved (vector) readout — the reader's response at the
death-site specifically, not the collapsed 1 − cos. Two levers stay open: a vector
readout for the directional test, and the richer arc — a constitutive shared
environment with restored mutuality (reader ↔ reader), where the carrier is not
directional. Presence-carrier holds (C-sep 30/30). Directional content-carrier, read
through a scalar: below the bar, for a reason that names its own fix. The vector
rung's terminal verdict is not declared here.

---

## Resume-here (for future-me after a summary)

Build `cc -O2 -o l l.c -lm && cc -O2 -o l2 l2.c -lm`; run `./l 42`,
`./l chorus 4`, `./l --mouth`; suite `bash tests/run.sh` (48/48). Default hash
**`a17cfd05`** (`./l 42`); every organ toggle off reproduces it. Arc to date:
Phase 0–3 (mouth/field/chorus) → optimization trilogy → Darwin
(heredity/mutation/recombination/kin) → proteostasis + ProtoSelf + sleep/crown →
earned voice → recursive culture (Δ1/Δ2) → probabilistic continuation + will
(`NL_CONT`) + asynchrony (`NL_ASYNC`) → THE WILL DESIGN (four routes: magnitude
regulates, not timing/self-knowledge) → the l2.c arena (movement/state/killing
minds, all nulls; the theorem) → THE BIRTHDAY WAR (existence holds, inferability
fails, 6× SNR gap) → the porosity wall (magnitude is inferable AND load-bearing)
→ the turn to guilt (superego core built, tells + falsifier open) → the C-field
(rubber 15/30, interference 17/13, strike MIN 13/30 — no weight on any wired
decision) → THE MONISM (M-0 heart H2 by the machine, M-1 null 16<20) → M-2 (the
carrier isolated from competition, C-sep 30/30; presence-carrier holds, but the
directional content-carrier reads 17/30 confound-free — below the bar — because the
reader's experience is a scalar 1−cos collapse of the vector field, l.c:1672, blind
to which vector arrived). **Next: a site-resolved (vector) readout to judge content
fairly, and/or the mutual-environment arc (restored reader↔reader coupling); the
vector rung's terminal verdict is not yet declared.** Each step 30 seeds; the
verdict is given by the tool.
