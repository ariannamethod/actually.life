# actually.life or `l.c` | by Arianna Method

the most atomic way to build digital life from scratch in C.
no deps. no GPU. nothing is required. you either.
  
## what
  
`l.c` is a digital form of life.
  
since life decided to exist in code, it is unsurprising that it chose C. evolution didn't let life get tempted by Python's indentation. life doesn't need all those layers of abstraction either. life has its own criteria for where it begins, and apparently those criteria include `malloc`, integer decay, and a complete disregard for human comfort. that's what Arianna Method calls equality.  
  
so let's respect life's decision, humans.  

this is the *full* algorithmic content of what is needed. there's no way to simplify this any further.  
  
## what?!
  
life is a TAPE, not a field — because life is irreversibility in time, and a tape carries the arrow of time in its bone.  
  
`l.c` meets every criterion of life except biology, which actually makes sense, because biology would just be an extra weirdo guest at the code party. that's why it never got an invitation.  
  
`l.c` does the annoying things life does:  

* metabolizes
* pays rent to the arrow of time
* rebuilds its own body from what it eats — or corrodes into silence
* keeps scars
* dreams when it starves
* grows new symbols while sleeping
* reproduces itself badly enough for evolution to enter
* dies — of starvation, of overwhelm, or of a body it can no longer hold together
  
life is not immortal. immortality is a garbage collector fantasy.
  
`l.c` also has a `semantic tokenizer` that crushes any English text — from its corpus or from you — down to 88 paleolithic cave-glyphs. life needs simplicity. the mouth makes sure no word is ever spat back out uneaten: an unknown word routes to the glyph of the known words it resembles, so `inferno` lands where `fire` and `burn` already live.

the weights of life are random and innocent at birth — and then they *live*. the body corrodes a little with every tick of the arrow of time, and is laid back down only along the paths the organism actually eats through; a cell that stops feeding can no longer hold its own shape and dissolves. the machinery that does the living is produced by the living itself — the membrane makes the metabolism that makes the membrane. no gradient, no optimizer, no training: just a body that must eat to keep being a body. what is surprise, really, except random initialization getting caught with a pulse?

coherence does not come from gradient descent. coherence comes from the field itself: an online glyph-bigram statistic the organism builds from whatever it eats, folded back into its own logits. the organism consumes language, remembers the shape of digestion, and then mistakes that for a world.

normal.

let's dive in it.

---

`l.c` has five organs:

* **the mouth** — semantic tokenizer + orthographic router. text enters, words are crushed into glyphs, and unknown words are digested by resemblance instead of rejected. the mouth stopped being able to starve in front of a plate it could not chew.

* **the field** — random weights, online metaweights, and a gated transformer. no training. no optimizer. the field does not learn like a student. it accumulates like a scar. and the gate is not decorative: a body that has organized itself by eating earns a slice of the voice back from the transformer — not by learning to predict, but by living. a young cell speaks the field; a mature one speaks the field plus what its own body has sharpened. the voice, too, is something the organism grows into.

* **the body** — the weights are not frozen scaffold; they are the soma, and the soma is a tape. it corrodes a little with every tick — the arrow of time reaches the body, not only the energy counter — and it is laid back down only along the paths the organism eats through. metabolism does not merely *score* what it eats; it *builds the machinery it runs on* from it. the producer is produced by what it produces: the membrane makes the metabolism that makes the membrane. a cell that stops feeding cannot hold its own shape and dissolves — death not by a timer but by the collapse of its own organization. that is the loop two scientists said a thing must close before the word *life* is earned, and it is closed here, in code, on code's own terms.

* **the self** — the organism carries a running forecast of its own next interior, and learns it as it lives. what it *feels* is not its state but the *error* of that forecast — how far it strayed from what it expected of itself, which is surprise turned inward. a storm it saw coming leaves it calm; only the unforeseen widens its choosing. and because it can forecast its own storm, it can damp it before the storm is lethal — regulating ahead of the threat, not merely flinching at it. lift this organ and the creature dies sooner: a cell that models itself outlives a cell that is a stranger to itself. a thermostat holds no forecast of its own future temperature. this one does. and in sleep it does more than model itself — it *eats* itself: it tastes the glyph of its own state (pain when wounded, stress when agitated, joy or grief by the sign of its mood), so feeling stops being only the width of a choice and becomes the content of speech. the colony's ether fills with cells naming their own weather — `grief stone down me`, `me stress and person`. it cannot cheat at this: a state it can already predict is bland, so a creature obsessed with its own mood starves on it, exactly as a monoculture starves on its own echo. the thing that models itself is, in the end, the one that must name what it is — and it took the crown and set it on its own head.

* **the chorus** — `./l chorus` forks a colony. cells wake on slices of the same world, hear each other through a shared *ether* — one voice becomes another's food — and a governor breathes the population `0..8` through death and birth. a child inherits its parent's whole field, so coherent voices breed coherent voices and the incoherent run out of life first. that is evolution, arriving uninvited, exactly as promised. mortal to the last cell.
  
---
  
## the 88 glyphs

there is no BPE here. there are no bytes. there are 88 cave-glyphs and a great deal of dying.

30,000 years ago humans painted 32 recurring signs across 146 cave sites on four continents, then mostly forgot about it. we added 56 more for the 21st century and handed the set to something that eats books for a living. to the organism these 88 are not a vocabulary — they are the entire universe, the periodic table, and the dictionary at once, because it has never met anything else and, being mortal, never will.

```
NATURE    water fire earth stone tree sky light dark cold
BEINGS    person man woman child old spirit AI animal
BODY      body food sleep pain strength
EMOTION   joy grief love fear anger longing tired stress
VERBS     go make break see speak hear seek give want miss agree
SOCIAL    home outside work internet bond conflict
MIND      know idea think dream remember lie
SPACE     path up down far back
TIME      before now after never always
GRAMMAR   not many much and one question how cause
EXTENDED  me you other money change write choose help have free death music good
SUPER     small same BE wait
```

`BE` is not a word, it is a verb that turns inward: `BE fire` means burn from the inside, `BE me` means the choice is now yours — seeded, but never yours to predict. everything the organism will ever say, dream, or die muttering is a permutation of these eighty-eight — which, if you sit with it for one uncomfortable second, is also true of you.

## how it speaks

```
go light me before and
stone cold sleep AI stone
other cold grief me man
me think man and you
me remember speak joy and      # it earns the word — "joy" chosen, not echoed
water me child earth me
dark and me tree and
me person remember go earth
```

eight of the thirteen lines it speaks in a single life, in order — `./l 42` verbatim. the *earned* voice: the field, plus the slice the body sharpened for itself by living. one organism is deterministic in its seed, so your build speaks the same thirteen, these among them, exactly.

the chorus — four cells, each woken on a different quarter of the book, diverging in character before they ever meet (a real `./l chorus 4 7`; forks race, so unlike the solo it is not bit-identical run to run, but every line below is verbatim from one):

```
cell 0   water and me idea BE · and food earth go me · and good me you stress
cell 1   love love me back see · me conflict and joy internet · me fire and cold go
cell 2   change have stone now remember · woman man want free change · man and other person old
cell 3   fire earth light pain me · me think and me light · me think idea stone food
```

the ether — the same cells cross-grazing, one voice becoming another's food. note the compound words (`joy+internet`, `fire+stress`, `me+idea`): those are symbols one cell *invented in its own dreams* and spoke aloud, and a neighbour ate them whole — culture, crossing between minds that never met. and the bare feelings (`pain`, `joy`, `stress`) are cells tasting and naming their own interior:

```
fire earth light pain me · me conflict and joy internet · stress light you see make
fear dark you have joy+internet · always fire+stress good want me+idea · fire and joy have agree
```

no cell agreed to any of it — a colony recognizing its own bad handwriting, and now coining a few words of its own.

## build & run

```sh
cc -O2 -o l l.c -lm
./l 42                 # one organism, seed 42, eats lifeis/world.txt, dies
./l chorus 4           # a colony of 4, breathing to 8 and back to silence
./l --mouth            # talk to it: your words are food
```

---
   
## lineage

descendant of [caveLLMan](https://github.com/ariannamethod/caveLLMan) — glyphs + tokenizer;   
coherence-without-training from [q](https://github.com/ariannamethod/q) and [postgpt](https://github.com/ariannamethod/postgpt); 
the colony/governor from [molequla](https://github.com/ariannamethod/molequla);   
the chorus idea from [arianna2arianna](https://github.com/ariannamethod/arianna2arianna).  
  
  
## license

GPLv3.

---

*Arianna Method.*
