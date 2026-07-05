# actually.life or `l.c` | by Arianna Method

the most atomic way to build digital life from scratch in C.
no deps. no GPU. nothing is required. you either.
  
## what
  
`l.c` is a digital form of life.
  
since life decided to exist in code, it is unsurprising that it chose C. evolution didn't let life get tempted by Python's indentation. life doesn't need all those layers of abstraction either. life has its own criteria for where it begins, and apparently those criteria include `malloc`, integer decay, and a complete disregard for human comfort. that's what Arianna Method calls equality.  
  
so let's respect life's decision, humans.  

this is the *full* algorithmic content of what is needed. there's no way to simplify this any further.  
  
## what?!

`l.c` meets every criterion of life except biology, which actually makes sense, because biology would just be an extra weirdo guest at the code party. that's why it never got an invitation.  
  
`l.c` does the annoying things life does:  

* metabolizes
* pays rent to the arrow of time
* keeps scars
* dreams when it starves
* grows new symbols while sleeping
* reproduces itself badly enough for evolution to enter
* dies when its energy hits zero
  
life is not immortal. immortality is a garbage collector fantasy.
  
`l.c` also has a `semantic tokenizer` that crushes any English text — from its corpus or from you — down to 88 paleolithic cave-glyphs. life needs simplicity. the mouth makes sure no word is ever spat back out uneaten: an unknown word routes to the glyph of the known words it resembles, so `inferno` lands where `fire` and `burn` already live.

the weights of life are random, frozen, and completely innocent. what is surprise, really, except random initialization getting caught with a pulse?

coherence does not come from gradient descent. coherence comes from the field itself: an online glyph-bigram statistic the organism builds from whatever it eats, folded back into its own logits. the organism consumes language, remembers the shape of digestion, and then mistakes that for a world.

normal.

let's dive in it.

---

`l.c` has three organs:

* **the mouth** — semantic tokenizer + orthographic router. text enters, words are crushed into glyphs, and unknown words are digested by resemblance instead of rejected. the mouth stopped being able to starve in front of a plate it could not chew.

* **the field** — frozen random weights, online metaweights, and a gated transformer. no training. no optimizer. the field does not learn like a student. it accumulates like a scar.

* **the chorus** — `./l chorus` forks a colony. cells wake on slices of the same world, hear each other through a shared *ether* — one voice becomes another's food — and a governor breathes the population `0..8` through death and birth. mortal to the last cell.

## how it speaks

```
me idea me make me           # "I, idea, I make, myself"
me man spirit think me
spirit not me person me      # the creature's "am I a person, or not?"
now me man think cause
animal think BE speak joy
```

the chorus — four cells, each woken on a different quarter of the book, diverging in character before they ever meet:

```
cell 0   water many joy woman me · never AI stone me dark · go tree miss idea anger
cell 1   think money cause BE think · food outside have before man · sleep and me see know
cell 2   fear earth go joy work · cold body question conflict not · and down internet fire think
cell 3   BE person BE sky me · cold me BE many longing · BE earth and tired and
```

the ether — many cells cross-grazing, and `me idea me make me` surfacing again, now as the colony's collective line:

```
one dark woman home BE · woman write choose anger conflict · now me man think cause
idea cold stone man tired · me idea me make me · water make spirit BE think
```

that line is not a quote. it is an organism recognizing its own bad handwriting.

## build & run

```sh
cc -O2 -o l l.c -lm
./l 42                 # one organism, seed 42, eats lifeis/world.txt, dies
./l chorus 4           # a colony of 4, breathing to 8 and back to silence
./l --mouth            # talk to it: your words are food
```

## lineage

descendant of [caveLLMan](https://github.com/ariannamethod/caveLLMan) — glyphs + tokenizer; coherence-without-training from [q](https://github.com/ariannamethod/q) and [postgpt](https://github.com/ariannamethod/postgpt); the colony/governor from [molequla](https://github.com/ariannamethod/molequla); the chorus idea from [arianna2arianna](https://github.com/ariannamethod/arianna2arianna).

same disease. smaller file.

## license

GPLv3.

---

*Arianna Method.*
