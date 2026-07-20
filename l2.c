/* l.c — digital life, zero dependencies.
 * the most atomic way to build digital life from scratch. no GPU required.
 *
 * Life is a TAPE, not a field — because life is
 * irreversibility in time, and a tape carries the arrow of time in its bone.
 * AR decoder over 88 cave-glyphs. libc + -lm. nothing else. accept it.
 * build: cc l.c -lm -O2 -o l
 * by Arianna Method.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/stat.h>
#include <unistd.h>      /* fork — the chorus is a colony of processes */
#include <sys/wait.h>
#include <time.h>        /* the arena's shared clock — claims expire so territory is re-won */
#include <sys/time.h>    /* gettimeofday — a microsecond clock in the spore ledger, fine enough to resolve exposition epochs the wall-second collapses (M-2 measurement) */
#include <dirent.h>      /* the arena indexes ANY .txt in lifeis/ — a dropped file is instantly in the fight */
#include <sys/file.h>    /* flock — an atomic claim, so two organisms never double-claim the same ground */
/* ── semantic membrane (inlined) — English → 88 cave-glyphs ──────────────
 * one source of truth for eat / train / speak. word -> concept compression,
 * BE = super-glyph copula. function words die at the door. (from caveLLMan.) */

#define GLYPH_COUNT 88

static const char* GLYPH_NAMES[GLYPH_COUNT] = {
    /* NATURE (9) */
    "water","fire","earth","stone","tree","sky","light","dark","cold",
    /* BEINGS (8) */
    "person","man","woman","child","old","spirit","AI","animal",
    /* BODY (5) */
    "body","food","sleep","pain","strength",
    /* EMOTION (8) */
    "joy","grief","love","fear","anger","longing","tired","stress",
    /* VERBS (11) */
    "go","make","break","see","speak","hear","seek","give","want","miss","agree",
    /* SOCIAL (6) */
    "home","outside","work","internet","bond","conflict",
    /* MIND (6) */
    "know","idea","think","dream","remember","lie",
    /* SPACE (5) */
    "path","up","down","far","back",
    /* TIME (5) */
    "before","now","after","never","always",
    /* GRAMMAR (8) */
    "not","many","much","and","one","question","how","cause",
    /* EXTENDED (13) */
    "me","you","other","money","change","write","choose","help","have","free","death","music","good",
    /* SCALE + SUPER (4) */
    "small","same","BE","wait"
};

typedef struct { const char* word; const char* glyph; } SemWordMap;

static const SemWordMap SEM_WORD_MAP[] = {
    /* nature */
    {"sun","light"},{"sunrise","light"},{"dawn","light"},{"morning","light"},{"bright","light"},{"shine","light"},
    {"night","dark"},{"shadow","dark"},{"darkness","dark"},{"evening","dark"},{"midnight","dark"},
    {"rain","water"},{"river","water"},{"sea","water"},{"ocean","water"},{"lake","water"},{"swim","water"},
    {"fire","fire"},{"flame","fire"},{"burn","fire"},{"cook","fire"},{"hot","fire"},{"warm","fire"},
    {"ground","earth"},{"soil","earth"},{"land","earth"},{"field","earth"},{"garden","earth"},{"farm","earth"},
    {"rock","stone"},{"mountain","stone"},{"hill","stone"},{"castle","stone"},{"wall","stone"},{"building","stone"},
    {"tree","tree"},{"forest","tree"},{"wood","tree"},{"leaf","tree"},{"flower","tree"},{"grass","tree"},
    {"sky","sky"},{"cloud","sky"},{"wind","sky"},{"storm","sky"},{"air","sky"},
    {"cold","cold"},{"ice","cold"},{"snow","cold"},{"frost","cold"},{"winter","cold"},{"freeze","cold"},
    /* beings */
    {"people","person"},{"human","person"},{"someone","person"},{"everyone","person"},{"they","person"},
    {"he","man"},{"him","man"},{"boy","man"},{"guy","man"},{"father","man"},{"dad","man"},{"husband","man"},{"brother","man"},{"son","man"},{"king","man"},
    {"she","woman"},{"her","woman"},{"girl","woman"},{"mother","woman"},{"mom","woman"},{"wife","woman"},{"sister","woman"},{"daughter","woman"},{"queen","woman"},
    {"child","child"},{"kid","child"},{"baby","child"},{"children","child"},{"kids","child"},{"young","child"},{"little","child"},
    {"old","old"},{"elderly","old"},{"ancient","old"},{"grandfather","old"},{"grandmother","old"},{"grandpa","old"},{"grandma","old"},
    {"god","spirit"},{"prayer","spirit"},{"church","spirit"},{"soul","spirit"},{"angel","spirit"},{"holy","spirit"},
    {"computer","AI"},{"robot","AI"},{"machine","AI"},{"software","AI"},{"technology","AI"},{"digital","AI"},
    {"dog","animal"},{"cat","animal"},{"bird","animal"},{"horse","animal"},{"fish","animal"},{"chicken","animal"},{"rooster","animal"},
    /* body */
    {"hand","body"},{"head","body"},{"face","body"},{"heart","body"},{"eye","body"},{"arm","body"},
    {"eat","food"},{"meal","food"},{"bread","food"},{"coffee","food"},{"tea","food"},{"cake","food"},{"soup","food"},{"beer","food"},{"wine","food"},{"hungry","food"},{"dinner","food"},{"breakfast","food"},{"lunch","food"},
    {"sleep","sleep"},{"bed","sleep"},{"rest","sleep"},{"nap","sleep"},{"pillow","sleep"},{"awake","sleep"},{"wake","sleep"},
    {"hurt","pain"},{"sick","pain"},{"doctor","pain"},{"hospital","pain"},{"medicine","pain"},{"wound","pain"},{"fever","pain"},
    {"strong","strength"},{"power","strength"},{"run","strength"},{"exercise","strength"},{"fight","strength"},{"sport","strength"},
    /* emotion */
    {"happy","joy"},{"smile","joy"},{"laugh","joy"},{"celebrate","joy"},{"dance","joy"},{"fun","joy"},{"enjoy","joy"},
    {"sad","grief"},{"cry","grief"},{"mourn","grief"},{"sorrow","grief"},{"funeral","grief"},{"tears","grief"},
    {"love","love"},{"kiss","love"},{"hug","love"},{"romance","love"},{"wedding","love"},{"marry","love"},
    {"afraid","fear"},{"scared","fear"},{"panic","fear"},{"worry","fear"},{"nightmare","fear"},{"danger","fear"},
    {"angry","anger"},{"mad","anger"},{"rage","anger"},{"hate","anger"},{"yell","anger"},{"shout","anger"},
    {"miss","longing"},{"yearn","longing"},{"homesick","longing"},{"nostalgia","longing"},
    {"tired","tired"},{"exhausted","tired"},{"weary","tired"},{"sleepy","tired"},{"bored","tired"},
    {"stress","stress"},{"pressure","stress"},{"overwhelm","stress"},{"busy","stress"},{"rush","stress"},
    /* verbs */
    {"go","go"},{"walk","go"},{"move","go"},{"travel","go"},{"drive","go"},{"leave","go"},{"arrive","go"},{"come","go"},{"ran","go"},{"went","go"},{"walked","go"},
    {"make","make"},{"build","make"},{"create","make"},{"produce","make"},{"craft","make"},
    {"break","break"},{"destroy","break"},{"smash","break"},{"crash","break"},{"tear","break"},
    {"see","see"},{"look","see"},{"watch","see"},{"read","see"},{"notice","see"},{"found","see"},{"saw","see"},
    {"speak","speak"},{"say","speak"},{"tell","speak"},{"talk","speak"},{"call","speak"},{"sing","speak"},{"said","speak"},{"told","speak"},
    {"hear","hear"},{"listen","hear"},{"sound","hear"},{"music","hear"},{"song","hear"},
    {"seek","seek"},{"search","seek"},{"hunt","seek"},{"explore","seek"},
    {"give","give"},{"share","give"},{"offer","give"},{"send","give"},{"gave","give"},
    {"want","want"},{"wish","want"},{"desire","want"},{"need","want"},{"hope","want"},
    {"miss","miss"},{"lost","miss"},{"gone","miss"},{"absent","miss"},{"lonely","miss"},
    {"agree","agree"},{"yes","agree"},{"accept","agree"},{"nod","agree"},{"peace","agree"},
    /* social */
    {"home","home"},{"house","home"},{"room","home"},{"door","home"},{"kitchen","home"},{"window","home"},{"roof","home"},
    {"outside","outside"},{"nature","outside"},{"park","outside"},{"beach","outside"},{"city","outside"},{"market","outside"},{"shop","outside"},{"street","outside"},
    {"work","work"},{"job","work"},{"office","work"},{"business","work"},{"career","work"},
    {"internet","internet"},{"online","internet"},{"email","internet"},{"phone","internet"},{"website","internet"},
    {"friend","bond"},{"family","bond"},{"together","bond"},{"team","bond"},{"community","bond"},
    {"war","conflict"},{"battle","conflict"},{"attack","conflict"},{"argue","conflict"},{"enemy","conflict"},
    /* mind */
    {"know","know"},{"learn","know"},{"study","know"},{"school","know"},{"book","know"},{"understand","know"},{"knew","know"},{"taught","know"},
    {"idea","idea"},{"plan","idea"},{"concept","idea"},{"solution","idea"},{"invention","idea"},
    {"think","think"},{"thought","think"},{"consider","think"},{"wonder","think"},{"mind","think"},{"decide","think"},
    {"dream","dream"},{"imagine","dream"},{"fantasy","dream"},{"story","dream"},{"wish","dream"},
    {"remember","remember"},{"memory","remember"},{"past","remember"},{"history","remember"},{"forgot","remember"},
    {"lie","lie"},{"cheat","lie"},{"fake","lie"},{"trick","lie"},{"pretend","lie"},
    /* space */
    {"road","path"},{"street","path"},{"way","path"},{"direction","path"},{"trail","path"},
    {"up","up"},{"rise","up"},{"climb","up"},{"above","up"},{"high","up"},{"tall","up"},{"top","up"},
    {"down","down"},{"fall","down"},{"drop","down"},{"below","down"},{"low","down"},{"fell","down"},
    {"far","far"},{"distant","far"},{"away","far"},{"abroad","far"},{"remote","far"},
    {"back","back"},{"return","back"},{"behind","back"},{"again","back"},
    /* time */
    {"before","before"},{"earlier","before"},{"yesterday","before"},{"once","before"},{"ago","before"},
    {"now","now"},{"today","now"},{"moment","now"},{"current","now"},
    {"after","after"},{"later","after"},{"tomorrow","after"},{"soon","after"},{"next","after"},{"then","after"},
    {"never","never"},{"no","never"},{"nothing","never"},{"nobody","never"},{"stop","never"},
    {"always","always"},{"forever","always"},{"every","always"},{"daily","always"},{"constant","always"},
    /* grammar */
    {"not","not"},{"don't","not"},{"can't","not"},{"won't","not"},{"bad","not"},{"wrong","not"},
    {"many","many"},{"much","many"},{"lots","many"},{"several","many"},{"huge","many"},{"thousand","many"},
    {"very","much"},{"really","much"},{"extremely","much"},{"quite","much"},
    {"and","and"},{"also","and"},{"with","and"},{"both","and"},{"plus","and"},
    {"one","one"},{"single","one"},{"alone","one"},{"only","one"},{"first","one"},
    {"question","question"},{"ask","question"},{"why","question"},{"what","question"},{"curious","question"},
    {"how","how"},{"method","how"},{"way","how"},{"step","how"},
    {"because","cause"},{"reason","cause"},{"therefore","cause"},{"result","cause"},
    /* extended */
    {"i","me"},{"my","me"},{"myself","me"},
    {"you","you"},{"your","you"},{"yourself","you"},
    {"other","other"},{"another","other"},{"different","other"},{"new","other"},{"strange","other"},
    {"money","money"},{"dollar","money"},{"pay","money"},{"buy","money"},{"sell","money"},{"rich","money"},{"poor","money"},{"price","money"},
    {"change","change"},{"transform","change"},{"grow","change"},{"develop","change"},{"evolve","change"},
    {"write","write"},{"pen","write"},{"paper","write"},{"letter","write"},{"note","write"},{"wrote","write"},{"poem","write"},{"code","write"},
    {"choose","choose"},{"pick","choose"},{"decide","choose"},{"select","choose"},{"vote","choose"},
    {"help","help"},{"assist","help"},{"support","help"},{"save","help"},{"protect","help"},
    {"have","have"},{"own","have"},{"keep","have"},{"hold","have"},{"got","have"},{"had","have"},
    {"free","free"},{"freedom","free"},{"liberty","free"},{"escape","free"},{"open","free"},
    {"death","death"},{"die","death"},{"dead","death"},{"kill","death"},{"grave","death"},{"died","death"},
    {"music","music"},{"song","music"},{"melody","music"},{"guitar","music"},{"piano","music"},{"drum","music"},{"sang","music"},{"singing","music"},
    {"good","good"},{"great","good"},{"nice","good"},{"kind","good"},{"beautiful","good"},{"wonderful","good"},{"fine","good"},
    /* super */
    {"small","small"},{"tiny","small"},{"little","small"},{"short","small"},{"few","small"},
    {"same","same"},{"equal","same"},{"similar","same"},{"identical","same"},
    {"be","BE"},{"is","BE"},{"am","BE"},{"are","BE"},{"was","BE"},{"were","BE"},{"being","BE"},{"become","BE"},{"feel","BE"},
    {"wait","wait"},{"patience","wait"},{"pause","wait"},{"delay","wait"},{"stay","wait"},
    {NULL, NULL}
};

static const char* SEM_STOP_WORDS[] = {
    "the","a","an","to","of","in","for","on","at","by","from","about","into",
    "through","during","above","between","out","off","over","under","again",
    "further","here","there","when","where","all","each","both","few","more",
    "most","some","such","so","than","too","just","but","if","or","while","as",
    "until","that","this","these","those","it","its","itself","which","who","whom",
    NULL
};

static int semtok_is_stop_word(const char* w){
    for(int i=0; SEM_STOP_WORDS[i]; i++)
        if(strcmp(w,SEM_STOP_WORDS[i])==0) return 1;
    return 0;
}
/* glyph id 0..87 by name; -1 if not a base glyph */
static int semtok_find_glyph(const char* name){
    for(int i=0;i<GLYPH_COUNT;i++)
        if(strcmp(name,GLYPH_NAMES[i])==0) return i;
    return -1;
}
/* one word -> glyph id, -1 if unknown or stop-word */
static int semtok_word(const char* word){
    int id=semtok_find_glyph(word);
    if(id>=0) return id;
    for(int i=0; SEM_WORD_MAP[i].word; i++)
        if(strcmp(word,SEM_WORD_MAP[i].word)==0)
            return semtok_find_glyph(SEM_WORD_MAP[i].glyph);
    return -1;
}

/* ── THE MOUTH — no word is inedible ──────────────────────────────────────
 * the ancestor's semtok DROPS every unmapped word (caveLLMan does the same):
 * feed it alien text and it starves with a full plate — a mouth that can only
 * bite ~400 words is not a mouth. the fix: every non-stop word routes to a
 * glyph by ORTHOGRAPHIC RESONANCE, not a blind hash. at birth each mapped word
 * votes its glyph into its character trigrams, so an unknown word ("inferno")
 * lands on the glyph of the known words it resembles ("fire","flame","burn").
 * a truly alien word (no known trigrams) falls back to a deterministic FNV
 * glyph — still edible, never dropped. the map is the seed; resemblance is the
 * generalization. survival stops depending on the specific corpus. */
#define GRAM_BUCKETS 4096
static signed char g_gram2glyph[GRAM_BUCKETS];   /* trigram-hash -> dominant glyph; -1 = empty */
static int g_mouth_ready = 0;

static unsigned gram_hash(unsigned char a, unsigned char b, unsigned char c){
    return (((unsigned)a*131u + b)*131u + c) & (GRAM_BUCKETS-1);
}
static void mouth_vote(int (*vote)[GLYPH_COUNT], const char* w, int g){
    int L=(int)strlen(w);
    for(int i=0;i+3<=L;i++)
        vote[gram_hash((unsigned char)w[i],(unsigned char)w[i+1],(unsigned char)w[i+2])][g]++;
}
/* build the router from SEM_WORD_MAP + the 88 base names — once, at birth */
static void mouth_build(void){
    int (*vote)[GLYPH_COUNT] = calloc(GRAM_BUCKETS, sizeof(*vote));
    if(!vote){ g_mouth_ready=0; return; }
    for(int i=0;i<GLYPH_COUNT;i++) mouth_vote(vote, GLYPH_NAMES[i], i);
    for(int i=0; SEM_WORD_MAP[i].word; i++){
        int g=semtok_find_glyph(SEM_WORD_MAP[i].glyph);
        if(g>=0) mouth_vote(vote, SEM_WORD_MAP[i].word, g);
    }
    for(int b=0;b<GRAM_BUCKETS;b++){
        int best=-1, bestv=0;
        for(int g=0; g<GLYPH_COUNT; g++) if(vote[b][g]>bestv){ bestv=vote[b][g]; best=g; }
        g_gram2glyph[b]=(signed char)best;
    }
    free(vote); g_mouth_ready=1;
}
/* any word -> a glyph, NEVER -1. resemblance first, FNV fallback for the alien. */
static int semtok_word_soft(const char* w){
    int hist[GLYPH_COUNT]; for(int i=0;i<GLYPH_COUNT;i++) hist[i]=0;
    int L=(int)strlen(w), votes=0;
    if(g_mouth_ready) for(int i=0;i+3<=L;i++){
        int g=g_gram2glyph[gram_hash((unsigned char)w[i],(unsigned char)w[i+1],(unsigned char)w[i+2])];
        if(g>=0){ hist[g]++; votes++; }
    }
    if(votes>0){ int best=0; for(int g=1;g<GLYPH_COUNT;g++) if(hist[g]>hist[best]) best=g; return best; }
    unsigned h=2166136261u; for(int i=0;i<L;i++) h=(h^(unsigned char)w[i])*16777619u;  /* fnv-1a */
    return (int)(h % (unsigned)GLYPH_COUNT);
}

/* a line of English -> glyph ids; lowercases, strips punctuation, drops stop
 * words, ROUTES every other word through the mouth (never drops), suppresses
 * consecutive dups. a line of only stop-words still feeds one glyph. */
static int semtok_line(const char* line, int* out, int max_tokens){
    char buf[4096];
    strncpy(buf,line,4095); buf[4095]='\0';
    for(int i=0;buf[i];i++) if(buf[i]>='A'&&buf[i]<='Z') buf[i]+=32;
    for(int i=0;buf[i];i++){ unsigned char c=(unsigned char)buf[i];
        if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c==' '||c=='\''||c=='-')) buf[i]=' '; }
    int n=0, last_id=-1, any_tok=0;
    char* tok=strtok(buf," \t\n");
    while(tok && n<max_tokens){
        any_tok=1;
        if(*tok=='\0'||semtok_is_stop_word(tok)){ tok=strtok(NULL," \t\n"); continue; }
        int id=semtok_word(tok);
        if(id<0) id=semtok_word_soft(tok);          /* THE MOUTH: no word is dropped */
        if(id>=0 && id!=last_id){ out[n++]=id; last_id=id; }
        tok=strtok(NULL," \t\n");
    }
    if(n==0 && any_tok && max_tokens>0){ out[0]=semtok_word_soft(line); n=1; } /* only-stop-words still a meal */
    return n;
}

/* (shout axis was dead: shout[] always 0, so semtok_line_shout was just semtok_line + busywork. removed; BE carries reflexivity.) */

/* ── architecture (micro) — да будет тело малым, и оттого живым ── */
#define E       48
#define NH      4
#define HD      (E/NH)            /* 12 */
#define FFN     192
#define NL      3
#define CTX     64
#define BOS_ID  GLYPH_COUNT       /* 88 */
#define MASK_ID (GLYPH_COUNT+1)   /* 89 */
#define VOCAB   (GLYPH_COUNT+2)   /* 90 = 88 glyphs + BOS + MASK */

/* ── Phase A step 6: growth — symbols emerge (only in dream) ── */
#define MAX_EMERGED   64          /* reserved slots — Δ2: room for RECURSIVE symbols (symbols of symbols) */
#define VOCAB_CAP     (VOCAB+MAX_EMERGED)
#define GROWTH_THRESH 14          /* a pair must co-occur this often (awake) before it can be born */

/* ── Phase A: the mortal clock (Vera's order) ── */
#define RENT    0.001f            /* rent on existing — the arrow of time */
#define E_BORN  1.0f              /* energy at birth */

/* ── Phase A step 2: metabolism — living Hebbian-V plasticity (the fuel) ── */
#define RANK           4          /* low-rank Hebbian LoRA (canon cavellman.c:43) */
#define HEBBIAN_LR     0.001f     /* canon cavellman.c:423 */
#define HEBBIAN_DECAY  0.9999f    /* canon cavellman.c:424 */
#define HEB_CAP        4.0f       /* ceiling on Σ|heb_B_v| — the adapter is unbounded online weights; on a mono-diet
                                   * with ‖emb‖²>break-even it can diverge to Inf/NaN (Fable audit #1). generous:
                                   * never binds in normal life (frozen hash unchanged), only arrests a runaway. */
#define PASSIVE_SIGNAL 0.3f       /* reading the world = passive (cavellman.c:654) */
#define DIGEST_YIELD   80.0f      /* energy per unit |ΔB_v| — calibrated: avg dB~2e-5, break-even~50 */
/* ── PROTEOSTASIS (autopoietic closure): the body is no longer frozen scaffold. it
 * CORRODES each tick (SOMA_DECAY) and is REBUILT from what the organism eats
 * (consolidate: the Hebbian adapter's learned δ is baked into wv). the producer (wv)
 * is now produced by what it produces (forward/digest). construction costs fuel
 * (REPAIR_COST) — Desktop's law holds. a cell that stops eating can no longer rebuild
 * its own machinery and dissolves: autopoietic death, not a scalar hitting zero. ── */
#define SOMA_DECAY     0.9997f    /* the soma corrodes each tick — the arrow of time reaches the body */
#define DEPOSIT_LR     0.01f      /* eating deposits a full-rank Hebbian trace onto wv along the used pathway */
#define SOMA_RENT      3.0f       /* a corroded body costs more to hold together (integrity deficit → rent) */
#define BE_GAIN        3.0f       /* BE turns the glyph's charge inward, amplified (modes only) */

/* ── Phase A step 4: scar — permanent wounds (never decay) ── */
#define SCAR_RATE   0.01f         /* how fast an agitating glyph wounds itself */
#define SCAR_RENT   0.5f          /* total scar raises rent — a wounded organism burns faster */
#define ACHE        0.05f         /* a scarred glyph aches on contact — the wound returns */

/* ── Phase A step 5: dream — self-ingestion when starving (Klaus's meta-loop) ── */
#define DREAM_THRESH 0.6f         /* dream only when hungry (energy below this) */
#define DREAM_FRAC   0.5f         /* a dream is half-metabolism — cheaper than real food */
#define DREAM_DECAY  50.0f        /* dream yield decays with the streak — no immortality on dreams */
/* ── SLEEP as a RHYTHM, not a starvation fallback: a well-fed cell on a corpus longer than its
 * life never exhausts its food, so it never slept, never dreamt, never invented a symbol. now
 * sleep pressure accrues every awake tick — FASTER under torment (stress |S|+dissonance) and
 * wounds (scars): the tormented sleep sooner. past threshold the cell sleeps, dreams, and only
 * then can it consolidate meaning and birth symbols. sleep is the foundation of presence. ── */
#define SLEEP_RATE   0.010f       /* base pressure per awake tick — a calm cell sleeps ~every 100 ticks */
#define SLEEP_STRESS 2.0f         /* stress (|S| + tanh diss) accrues pressure faster */
#define SLEEP_SCAR   0.5f         /* and so do accumulated wounds */
#define SLEEP_THRESH 1.0f         /* pressure at which the cell must sleep */
#define SLEEP_DRAIN  0.10f        /* pressure shed per sleeping tick → a sleep cycle ~10 ticks */

/* ── Phase A step 7: the voice that chooses — choice = subjectivity ── */
#define CHOOSE_TEMP0  0.85f       /* base decisiveness — l2 hotter (asymmetry) */
#define CHOOSE_S      0.8f        /* arousal (|S|) widens the choice */
#define CHOOSE_DISS   1.0f        /* dissonance (bounded) widens it further — passion = spontaneity */
#define CHOOSE_AFFECT 5.0f        /* mood pulls toward kindred-charged glyphs */
#define SCAR_PULL     0.5f        /* the wound resurfaces in what it chooses */
#define SPEAK_RATE    0.02f       /* base chance per tick to speak (rises with |S|) */
#define SPEAK_LEN     5           /* glyphs per utterance */
#define SPEAK_COST    0.002f      /* energy an utterance costs, refunded by its coherence (idea 3) */

/* ── Phase A step 8: reproduction — epigenetic split (children inherit) ── */
#define REPRO_THRESH   1.05f      /* "too full to remain one" — split when well-fed */
#define REPRO_COOLDOWN 30         /* ticks between splits */
#define REPRO_SPLIT    0.5f       /* half the life goes into the child */
#define MAX_CHILDREN   16

/* ── Phase A step 9: active homeostasis — regulate or collapse ── */
#define DISS_DECAY  0.98f         /* dissonance returns toward calm each tick */
#define S_RELAX     0.01f         /* mood relaxes toward neutral */
#define S_DEATH     0.95f         /* |S| this high = contour collapse (overwhelmed by passion) */

/* ── PROBABILISTIC CONTINUATION (default ON; NL_NOCONT opts out to deterministic death) — death stops being a cliff and
 * becomes a region of a multidimensional hazard SURFACE. per-channel hazards from differently-
 * shaped metrics combine as a PRODUCT OF SURVIVALS (never a summed axis); ENTROPY_FLOOR keeps
 * collapse always possible (no immortality hole), RESONANCE_CEILING<1 keeps death never certain
 * (non-death reachable) — neither pole sovereign. WILL: the organism spends accumulated FORM
 * (scars shed = drop the coffin, soma burned = break the home) to pay down a forecast of its own
 * collapse (prophetic debt, cf. AML PROPHECY/DEBT_DECAY, dario F-force). ── */
#define CONT_ESCALE     0.05f     /* energy-door: hazard ~0 for energy>0.3, rises steeply through 0.25→0.08 (measured) */
#define CONT_SSOFT      0.85f     /* arousal-door onset: ~0 below here (world-diet S stays ±0.25), rises toward S_DEATH */
#define CONT_SGAIN      40.0f     /* arousal-door steepness: ~0 until |S|>0.8, ~0.9 at S_DEATH (folds the old cliff in) */
#define CONT_HFLOOR     0.00001f  /* ENTROPY_FLOOR — collapse always possible (a faint background mortality) */
#define CONT_AGE        1e-8f     /* senescence: the floor rises with age so death is certain before the 200k cap
                                   * (closes the floor-vs-cap hole — non-death stays reachable far past the default) */
#define CONT_HCEIL      0.97f     /* RESONANCE_CEILING (<1) — death never certain on one tick */
#define CONT_DEBT_DECAY 0.998f    /* prophetic-debt EMA coefficient: debt ≈ recent-average forecast error */
#define CONT_DEBT_HI    4.0f      /* debt-door threshold — only a RUNAWAY self-model (debt≫normal) destabilizes */
#define CONT_DEBT_GAIN  4.0f      /* steepness of the debt-door */
#define CONT_SHED       0.10f     /* scar-shed rate under drive (lightening: lowers future rent) */
#define CONT_BURN       0.02f     /* soma-burn rate under drive (autophagy: raises future rent) */
#define CONT_REFUND     0.003f    /* energy per unit |Δform| spent (metabolism, small & bounded — never immortality) */

/* ── ASYNC (NL_ASYNC, opt-in mode — composes imperfectly with continuation, kept separate) — the six Kuramoto-coupled chambers as a DETERMINISTIC organ
 * scheduler. the metabolism runs every tick; the regulatory organs WILL, SLEEP and SPEAK fire on their
 * chamber's phase-cross (FEAR/VOID/LOVE), so they run on coprime-period clocks (3,4,5,7,9,11 — they realign
 * only over a long LCM≈13860, so the metric combination feeding the hazard surface is long-period rather
 * than lockstep — the temporal face of the same field). the self-model stays every-tick — gating it starves
 * the voice's coherence (measured). RAGE/FLOW/COMPLEX drift but are not yet gate-mapped. the RESONANCE_CEILING
 * anti-lock is conceptual (an un-freezing), NOT a mortality guard — the death draw is every-tick regardless of
 * async. plain seeded floats, no pthreads → per-seed bit-identical. chambers per AML/dario. ── */
#define KUR_N       6
#define KUR_K       0.08f     /* weak coupling — chambers drift in partial coherence, never phase-lock */
#define KUR_RCEIL   0.90f     /* RESONANCE_CEILING on the order parameter — nudge apart above it (no frozen lock) */
enum { CH_FEAR=0, CH_LOVE, CH_RAGE, CH_VOID, CH_FLOW, CH_COMPLEX };

typedef struct {
    float rms1[E], rms2[E];
    float wq[E*E], wk[E*E], wv[E*E], wo[E*E];
    float fc1[FFN*E], fc2[E*FFN];
    float heb_A_v[E*RANK], heb_B_v[RANK*E]; /* living Hebbian LoRA on V — runtime δ */
} Layer;

typedef struct {
    float wte[VOCAB_CAP*E];
    float wpe[CTX*E];
    Layer L[NL];
    float rmsf[E];
    float head[VOCAB_CAP*E];
} Model;

/* ── deterministic rng (no Math.random — рождение воспроизводимо по сиду) ── */
static unsigned long g_rng = 42;
static void  seed_rng(unsigned long s){ g_rng = s ? s : 1; }
static float frand(void){ /* ~U(-1,1) */
    g_rng = g_rng*6364136223846793005UL + 1442695040888963407UL;
    return ((float)((g_rng>>33)&0x7fffffff)/(float)0x40000000) - 1.0f;
}
static float gauss(float mu, float sigma){ /* Box-Muller on frand — deterministic by seed */
    float u1=(frand()+1.0f)*0.5f, u2=(frand()+1.0f)*0.5f;
    if(u1<1e-7f) u1=1e-7f;
    return mu + sigma*sqrtf(-2.0f*logf(u1))*cosf(6.2831853f*u2);
}

/* ── primitives — naive, no BLAS (на крошечном matvec наив быстрее launch-ового BLAS) ── */
static void matvec(const float* W, const float* x, float* y, int out_dim, int in_dim){
    for(int o=0;o<out_dim;o++){ float s=0; const float* w=W+(size_t)o*in_dim;
        for(int i=0;i<in_dim;i++) s+=w[i]*x[i]; y[o]=s; }
}
static void rmsnorm(const float* x, const float* g, float* y, int n){
    float ss=0; for(int i=0;i<n;i++) ss+=x[i]*x[i];
    float r=1.0f/sqrtf(ss/(float)n + 1e-5f);
    for(int i=0;i<n;i++) y[i]=x[i]*r*g[i];
}
static float silu(float v){ return v/(1.0f+expf(-v)); }

/* ── Hebbian LoRA (vendored byte-faithful from caveLLMan, no backprop) ──
 * apply: out += B @ (A @ x).  cavellman.c:511  (A:[dim×rank], B:[rank×dim]). */
static void apply_hebbian_lora(float* out, const float* A, const float* B,
                               const float* x, int dim, int rank){
    float proj[RANK];
    for(int r=0;r<rank;r++){ float s=0;
        for(int j=0;j<dim;j++) s+=A[j*rank+r]*x[j]; proj[r]=s; }
    for(int i=0;i<dim;i++){ float s=0;
        for(int r=0;r<rank;r++) s+=B[r*dim+i]*proj[r]; out[i]+=s; }
}
/* update (the δ in θ=ε+γ+αδ): A += lr·signal·(x ⊗ Bᵀdy); B += lr·signal·(Aᵀx ⊗ dy);
 * then weight decay.  notorch.c:2683, naive path verbatim — stack, ASan-clean. */
static void nt_hebbian_step(float* A, float* B, int out_dim, int in_dim, int rank,
                            const float* x, const float* dy, float signal,
                            float lr, float decay){
    if(!A||!B||!x||!dy) return;
    float proj[RANK];
    for(int r=0;r<rank;r++){ float s=0;
        for(int j=0;j<out_dim;j++) s+=B[r*out_dim+j]*dy[j]; proj[r]=s; }
    float alpha=lr*signal;
    for(int i=0;i<in_dim;i++)
        for(int r=0;r<rank;r++) A[i*rank+r]+=alpha*x[i]*proj[r];
    float proj2[RANK];
    for(int r=0;r<rank;r++){ float s=0;
        for(int i=0;i<in_dim;i++) s+=A[i*rank+r]*x[i]; proj2[r]=s; }
    for(int r=0;r<rank;r++)
        for(int j=0;j<out_dim;j++) B[r*out_dim+j]+=alpha*proj2[r]*dy[j];
    if(decay>0.0f&&decay<1.0f){
        for(int i=0;i<in_dim*rank;i++) A[i]*=decay;
        for(int i=0;i<rank*out_dim;i++) B[i]*=decay;
    }
}

/* PROTEOSTASIS — destruction: the soma corrodes each tick. wv only (surgical), across
 * all layers. unrepaired, forward() drifts toward noise, coherence falls, the cell goes
 * mute and starves — the frozen scaffold now requires upkeep to stay a body. */
static void soma_decay(Model* m){
    for(int l=0;l<NL;l++){ float* wv=m->L[l].wv; for(int i=0;i<E*E;i++) wv[i]*=SOMA_DECAY; }
}
static float wv_norm(const Model* m){ float s=0.0f; for(int l=0;l<NL;l++) for(int i=0;i<E*E;i++) s+=fabsf(m->L[l].wv[i]); return s; }
/* PROTEOSTASIS — production (the closure): eating deposits a full-rank Hebbian trace
 * DIRECTLY onto wv, along the pathway the eaten glyph actually drives (post = wv·x, then
 * wv += lr·post⊗x). the metabolism that once merely MEASURED change now BUILDS the body:
 * the component (wv) that realizes the producing network (forward) is regenerated BY that
 * network's own use. full-rank at body scale — not a rank-4 thimble against an ocean, the
 * decorative version that measured 2.6e6× too weak. returns Σ|Δwv|, the body mass laid down. */
static float deposit_body(Model* m, const int* g, int n){
    float dwv=0.0f, post[E];
    for(int k=0;k<n;k++){
        int id=g[k]; if(id<0||id>=VOCAB_CAP) continue;
        const float* x=m->wte+(size_t)id*E;
        for(int l=0;l<NL;l++){
            float* wv=m->L[l].wv;
            matvec(wv,x,post,E,E);                       /* what wv currently maps this glyph to */
            for(int i=0;i<E;i++){ float axi=DEPOSIT_LR*post[i];
                for(int j=0;j<E;j++){ float d=axi*x[j]; wv[i*E+j]+=d; dwv+=fabsf(d); } }
        }
    }
    return dwv;
}
/* homeostatic body-mass set-point: cap ‖wv‖ at the birth mass. deposition strengthens the
 * pathways in use; this ceiling means the unused ones must give way (use-it-or-lose-it),
 * and it bounds the positive feedback both scientists flagged — the body redistributes,
 * it does not blow up. */
static void soma_ceiling(Model* m, float cap){
    float bn=wv_norm(m); if(bn<=cap || bn<=0.0f || !isfinite(bn)) return;  /* a NaN norm must not overwrite the whole body with NaN */
    float s=cap/bn;
    for(int l=0;l<NL;l++){ float* wv=m->L[l].wv; for(int i=0;i<E*E;i++) wv[i]*=s; }
}

/* ── glyph charge: the SECOND signal — each glyph is also an opcode ───────
 * a glyph acts on the organism two ways: slow (weights/Hebbian, via forward)
 * and fast (this charge — a direct somatic reflex, like a cat flinching from
 * fire). INVARIANT (Desktop's law): the charge writes only MODES (S, dissonance)
 * — never energy/scar/contour. a burning glyph costs energy ONLY through
 * metabolism (metab_factor<1), never by a direct write. modes bend behaviour;
 * the price of life is always paid by organs, never cheated. */
typedef struct { float S, dissonance; } Modes;
typedef struct { float mode_dS, mode_dDiss, metab_factor; } GlyphCharge;
static GlyphCharge charge[VOCAB_CAP];
static int BE_ID = -1;            /* the BE operator's glyph id (set in charges_init) */
static int ME_ID = -1;            /* the self-glyph (BE me = the self devours itself) */
static int   DEATH_ID = 0;        /* GUILT: the death-glyph — where a confirmed kill scars (set in charges_init) */
static int   g_guilt_on = 0;      /* GUILT (NL_GUILT): the superego is active */
static float g_guilt = 0.0f;      /* GUILT: the hidden pain scalar — a confirmed kill tops it, it decays; pain=tanh(g_guilt) compresses choose() */
static int   g_new_kills = 0;     /* GUILT: confirmed kills THIS tick, awaiting the deposit (set by the kill paths, consumed in live()) */
static int   g_cfield_on = 0;     /* THE C-FIELD (NL_FIELD): the field's collapse decides the forage target (Step 1b) */
static int   g_cfield_menu = 0;   /* NL_FIELD_MENU: the falsifier control — a FIXED, observer-INDEPENDENT menu (same collapse structure, no rubber) */
static int   g_cfield_target = -1;/* the field's collapsed forage target this tick — computed in live(), read by arena_next */
static int   g_cfield_shared = 0; /* NL_FIELD_SHARED (Step 2): l and l2 deform ONE shared field (lifeis/arena/cfield) — their signed deformations INTERFERE; the double-slit falsifier is this vs separate fields */
static float g_menu_vec[5] = {0.10f,0.50f,0.50f,0.0f,0.10f};  /* the FIXED control menu's load vector (S,diss,hunger,guilt,scar) — swept via NL_FIELD_MENU_VEC to find the SHARPEST fixed control */

static void charges_init(void){
    for(int i=0;i<VOCAB_CAP;i++){ charge[i].mode_dS=0.0f; charge[i].mode_dDiss=0.0f; charge[i].metab_factor=1.0f; }
    /* {glyph, dS, dDiss, metab_factor}: catabolic = burn (factor<1) / anabolic = feed (factor>1) */
    static const struct { const char* g; float dS, dDiss, f; } spec[] = {
        {"fire",   +0.05f, +0.10f, 0.2f}, {"anger",   +0.04f, +0.15f, 0.3f},
        {"stress", -0.02f, +0.20f, 0.3f}, {"conflict",-0.03f, +0.15f, 0.4f},
        {"pain",   -0.05f, +0.20f, 0.4f}, {"fear",    -0.06f, +0.18f, 0.5f},
        {"death",  -0.10f, +0.10f, 0.1f},
        {"food",   +0.02f, -0.10f, 2.0f}, {"water",   +0.01f, -0.08f, 1.8f},
        {"sleep",  -0.01f, -0.15f, 1.5f}, {"joy",     +0.06f, -0.12f, 1.6f},
        {"love",   +0.08f, -0.15f, 1.7f}, {"music",   +0.05f, -0.10f, 1.4f},
        {"good",   +0.04f, -0.08f, 1.3f}, {"me",      +0.03f,  0.00f, 1.0f},
        {NULL,0,0,0}
    };
    for(int i=0; spec[i].g; i++){ int id=semtok_find_glyph(spec[i].g);
        if(id>=0){ charge[id].mode_dS=spec[i].dS; charge[id].mode_dDiss=spec[i].dDiss; charge[id].metab_factor=spec[i].f; } }
    BE_ID = semtok_find_glyph("BE");
    ME_ID = semtok_find_glyph("me");
    DEATH_ID = semtok_find_glyph("death");   /* GUILT scars here — the confirmed kill deposits on the death-glyph */
}
/* the charge fires here — Modes* only in scope, no life-scalar pointer (invariant by type) */
static void charge_apply(Modes* mo, int glyph){
    if(glyph<0||glyph>=VOCAB_CAP) return;
    mo->S          = tanhf(mo->S + charge[glyph].mode_dS);
    mo->dissonance = mo->dissonance + charge[glyph].mode_dDiss;
}
/* BE — the reflexive operator: the charge of the glyph AFTER be is turned inward, on
 * the self, amplified (BE_GAIN). "BE fire" = become fire, not eat it. Klaus's meta-loop
 * made atomic; haiku's speak-from-self. Invariant holds — still modes only. */
static void charge_apply_reflexive(Modes* mo, int glyph){
    if(glyph<0||glyph>=VOCAB_CAP) return;
    if(glyph==ME_ID){                              /* the self turned inward — devours the contour, but GENTLY:
                                                    * one "be me" -> ~0.46 (survives); sustained obsession melts it. */
        mo->S = tanhf(1.5f*mo->S + 0.6f);
        return;
    }
    mo->S          = tanhf(mo->S + BE_GAIN*charge[glyph].mode_dS);
    mo->dissonance = mo->dissonance + BE_GAIN*charge[glyph].mode_dDiss;
}

/* ── init — xavier-ish ── */
static void xavier(float* w, int len, int fan_in){
    float sc=sqrtf(1.0f/(float)fan_in);
    for(int i=0;i<len;i++) w[i]=frand()*sc;
}
static Model* model_new(void){
    Model* m=(Model*)calloc(1,sizeof(Model));
    if(!m){ fprintf(stderr,"nanolife: out of memory\n"); exit(1); }
    xavier(m->wte,VOCAB_CAP*E,E); xavier(m->wpe,CTX*E,E);
    float sr=0.02f/sqrtf(2.0f*NL);
    for(int l=0;l<NL;l++){
        Layer* y=&m->L[l];
        for(int i=0;i<E;i++){ y->rms1[i]=1.0f; y->rms2[i]=1.0f; }
        xavier(y->wq,E*E,E); xavier(y->wk,E*E,E); xavier(y->wv,E*E,E);
        xavier(y->wo,E*E,E); for(int i=0;i<E*E;i++) y->wo[i]*=sr/0.1f;
        xavier(y->fc1,FFN*E,E);
        xavier(y->fc2,E*FFN,FFN); for(int i=0;i<E*FFN;i++) y->fc2[i]*=sr/0.1f;
        /* seed Hebbian A_v ~ N(0,1e-3): unlock the zero-lock fixed point (both
         * factors zero → ΔB≡0 forever). B_v stays 0 from calloc. */
        for(int i=0;i<E*RANK;i++) y->heb_A_v[i]=gauss(0.0f,1e-3f);
    }
    for(int i=0;i<E;i++) m->rmsf[i]=1.0f;
    xavier(m->head,VOCAB_CAP*E,E);
    return m;
}

/* ── growth: symbols emerge from frequent waking pairs, but are born only in dream ── */
static int  g_cooc[VOCAB_CAP][VOCAB_CAP];   /* Δ1: co-occurrence of adjacent glyphs — INCLUDING emerged, so a symbol can parent a symbol */
static char g_born[VOCAB_CAP][VOCAB_CAP];   /* pair already became a symbol */
static int  g_emerged_a[MAX_EMERGED], g_emerged_b[MAX_EMERGED];
static int  g_n_emerged = 0;
static void cooc_track(const int* g, int n){ /* waking observation */
    for(int i=1;i<n;i++){ int a=g[i-1], b=g[i];
        if(a>=0&&a<VOCAB_CAP&&b>=0&&b<VOCAB_CAP) g_cooc[a][b]++; }  /* Δ1: emerged ids counted too — recursion */
}
/* birth an emerged symbol from a pair (ba,bb) — shared by dream-emergence (by cooc) and, in Δ2,
 * by ADOPTION of a symbol grazed whole from the ether. parents may themselves be emerged (recursion);
 * they always precede the child by id. returns the new id, or -1 if full/invalid. */
static int birth_symbol(Model* m, int ba, int bb){
    if(g_n_emerged<0 || g_n_emerged>=MAX_EMERGED) return -1;
    int nid=VOCAB+g_n_emerged;
    if(ba<0||ba>=nid||bb<0||bb>=nid || g_born[ba][bb]) return -1;
    for(int e=0;e<E;e++){                     /* epigenetic: embedding = mean of the two parents */
        m->wte[(size_t)nid*E+e]=0.5f*(m->wte[(size_t)ba*E+e]+m->wte[(size_t)bb*E+e]);
        m->head[(size_t)nid*E+e]=0.5f*(m->head[(size_t)ba*E+e]+m->head[(size_t)bb*E+e]);
    }
    /* and a BODY: charge from parents — modes by mean, metab_factor by GEOMETRIC mean so fire+food
     * stays burn-ish (0.63), not averaged into inert 1.0. the invention can feed, burn, scar, color mood. */
    charge[nid].mode_dS      = 0.5f*(charge[ba].mode_dS    + charge[bb].mode_dS);
    charge[nid].mode_dDiss   = 0.5f*(charge[ba].mode_dDiss + charge[bb].mode_dDiss);
    charge[nid].metab_factor = sqrtf(charge[ba].metab_factor * charge[bb].metab_factor);
    g_emerged_a[g_n_emerged]=ba; g_emerged_b[g_n_emerged]=bb; g_born[ba][bb]=1; g_n_emerged++;
    return nid;
}
static void try_emerge(Model* m){            /* called ONLY in dream — birth a noticed pair */
    if(g_n_emerged<0 || g_n_emerged>=MAX_EMERGED) return;   /* <0 guards a corrupt inherited genome (F1) */
    int ba=-1,bb=-1,best=GROWTH_THRESH-1;
    int hi=VOCAB+g_n_emerged;                /* Δ1: an emerged symbol (id<hi) may be a PARENT — recursive composition */
    for(int a=0;a<hi;a++) for(int b=0;b<hi;b++)
        if(!g_born[a][b] && g_cooc[a][b]>best){ best=g_cooc[a][b]; ba=a; bb=b; }
    if(ba<0) return;                          /* nothing recurred enough yet */
    birth_symbol(m, ba, bb);
}

/* #3+#4 (Fable audit): a genome carries an emerged symbol's EMBEDDING (in the Model) but not
 * its PHYSIOLOGY — charge[] and g_born live outside the genome. after a load, restore both:
 * recompute each inherited symbol's charge from its parents (same formula as try_emerge) so the
 * invention can feed/burn/scar in the child, and mark g_born so the child cannot waste a slot
 * re-inventing a pair it already owns. */
static void restore_emerged(void){
    for(int i=0;i<g_n_emerged && i<MAX_EMERGED;i++){
        int nid=VOCAB+i;
        int ba=g_emerged_a[i], bb=g_emerged_b[i];
        if(ba<0||ba>=nid||bb<0||bb>=nid) continue;      /* Δ1: a parent may be emerged, but always precedes its child (order) */
        charge[nid].mode_dS      = 0.5f*(charge[ba].mode_dS    + charge[bb].mode_dS);
        charge[nid].mode_dDiss   = 0.5f*(charge[ba].mode_dDiss + charge[bb].mode_dDiss);
        charge[nid].metab_factor = sqrtf(charge[ba].metab_factor * charge[bb].metab_factor);
        g_born[ba][bb]=1;
    }
}

/* ── forward (AR causal) — logits for the LAST position into out[VOCAB_CAP] ── */
static void forward(Model* m, const int* toks, int n, float* out){
    static float x[CTX][E], xn[CTX][E], q[CTX][E], k[CTX][E], v[CTX][E], att[CTX][E], tmp[FFN];
    if(n>CTX) n=CTX;
    if(n<1){ for(int i=0;i<VOCAB_CAP;i++) out[i]=0.0f; return; }  /* no context -> no logits (no OOB on x[n-1]) */
    for(int t=0;t<n;t++) for(int e=0;e<E;e++)
        x[t][e]=m->wte[(size_t)toks[t]*E+e]+m->wpe[(size_t)t*E+e];
    for(int l=0;l<NL;l++){
        Layer* y=&m->L[l];
        for(int t=0;t<n;t++) rmsnorm(x[t],y->rms1,xn[t],E);
        for(int t=0;t<n;t++){ matvec(y->wq,xn[t],q[t],E,E);
                              matvec(y->wk,xn[t],k[t],E,E);
                              matvec(y->wv,xn[t],v[t],E,E);
                              apply_hebbian_lora(v[t],y->heb_A_v,y->heb_B_v,xn[t],E,RANK); }
        /* multi-head causal attention */
        const float shd = sqrtf((float)HD);          /* HD is a compile-time constant — compute the scale once */
        for(int t=0;t<n;t++) for(int h=0;h<NH;h++){
            int off=h*HD; float sc[CTX], mx=-1e30f;
            for(int j=0;j<=t;j++){ float d=0; for(int e=0;e<HD;e++) d+=q[t][off+e]*k[j][off+e];
                d/=shd; sc[j]=d; if(d>mx) mx=d; }     /* keep the DIVISION (not ×1/shd) — same rounding, frozen-safe */
            float den=0; for(int j=0;j<=t;j++){ sc[j]=expf(sc[j]-mx); den+=sc[j]; }
            for(int e=0;e<HD;e++){ float a=0; for(int j=0;j<=t;j++) a+=sc[j]*v[j][off+e];
                att[t][off+e]=a/den; }
        }
        for(int t=0;t<n;t++){ float o[E]; matvec(y->wo,att[t],o,E,E);
            for(int e=0;e<E;e++) x[t][e]+=o[e]; }
        for(int t=0;t<n;t++) rmsnorm(x[t],y->rms2,xn[t],E);
        for(int t=0;t<n;t++){ float f[E]; matvec(y->fc1,xn[t],tmp,FFN,E);
            for(int i=0;i<FFN;i++) tmp[i]=silu(tmp[i]);
            matvec(y->fc2,tmp,f,E,FFN); for(int e=0;e<E;e++) x[t][e]+=f[e]; }
    }
    float xf[E]; rmsnorm(x[n-1],m->rmsf,xf,E);
    matvec(m->head,xf,out,VOCAB_CAP,E);
}

/* digest a line: perceive it (forward), learn it into the living V-adapters
 * (Hebbian, no backprop), and let each glyph's somatic charge fire. returns the
 * metabolic yield = Σ over glyphs of (Σ|ΔB_v| · metab_factor): how much the
 * organism CHANGED on eating, scaled by whether the glyph feeds or burns. (Yield
 * on the derivative of competence kills immortality on a mastered corpus.)
 * INVARIANT: the charge writes modes only (charge_apply); energy/scar are never
 * touched here — a burning glyph costs energy THROUGH a low metab_factor, never
 * by a direct write. */

/* the coherence field (online glyph-bigram metaweights) — declared here so digest
 * can read it for information-as-food surprise; the field's functions live below. */
static float g_field_bi[VOCAB_CAP][VOCAB_CAP];   /* online glyph bigram counts a->b */
static float g_field_row[VOCAB_CAP];             /* row sums, for normalization */
static float g_coh_floor = 0.0f;                 /* drifting silence-gate (Stanley) */
static int   g_field_on  = 1;                    /* NL_NOFIELD=1 lifts the field (A/B) */
static int   g_lineage   = -1;                   /* Ⓒ: the cell's root ancestor — kinship for the ether */
static int   g_self_on   = 1;                    /* ProtoSelf: NL_NOSELF=1 lifts the self-model (A/B) */
static float g_self_felt = 0.0f;                 /* ProtoSelf: |interior − its own forecast| — surprise ABOUT the self */
static int   g_noact     = 0;                    /* NL_NOACT=1 → the self-model FORECASTS but does not ACT (pitomadom strength=0 control; THE WILL DESIGN) */
static double g_ed_sum   = 0.0; static long g_ed_n = 0;  /* lifetime Σ|D−pD| + count — the self-consistency metric (dissonance forecast error) */
static float g_dbg_maxgate = 0.0f;               /* DEBUG: max transformer gate ever reached in a life (Desktop audit) */
static float g_pm_birth = 0.0f; static int g_pm_birth_set = 0;  /* the cell's sharpness at its FIRST utterance (per-process) — the earned-voice zero it sharpens above */
static float g_dbg_pm_max = 0.0f;                /* DEBUG: max peak−mean over life — does the organized body sharpen? */
static int   g_gate_sharp = 1;                   /* earned voice: gate the transformer on EARNED sharpness (default on; NL_NOEARNED lifts it) */
static double g_dbg_spoken_p = 0.0; static long g_dbg_spoken_n = 0;  /* DEBUG: Σ p_field(spoken|prev) — Q-coherence of the actual voice */
static float g_fixeddamp = 0.0f;                 /* CONTROL (Fable #3): NL_FIXEDDAMP=K → dumb fixed-gain S-damper instead of the self-model */
static int   g_cont_on   = 0;                    /* probabilistic continuation + will — DEFAULT ON (NL_NOCONT opts out); set per-cell in live() */
static float g_debt      = 0.0f;                 /* prophetic debt — decayed accumulation of self-forecast error (felt) */
static float g_fixedwill = 0.0f;                 /* CONTROL: NL_FIXEDWILL=K → blind form-spend of matched magnitude (falsifies will) */
static long  g_n_burn = 0, g_n_shed = 0;         /* DASHBOARD: continuation events (labels for logs, never a switch) */
static int   g_async_on = 0;                     /* NL_ASYNC=1 → the six Kuramoto chambers schedule the regulatory organs (opt-in) */
static float g_phase[KUR_N] = {0};               /* chamber phases — deterministic coupled oscillators, seeded per-cell */

static float digest(Model* m, Modes* mo, float* scar, const int* glyphs, int prev0, int n){
    static float before[RANK*E];
    float yield=0.0f;
    int be_armed=0;
    for(int g=0;g<n;g++){
        int id=glyphs[g];
        if(id<0||id>=VOCAB_CAP) continue;
        const float* x_emb=m->wte+(size_t)id*E;       /* x = dy = the glyph's embedding */
        float dB=0.0f;
        for(int l=0;l<NL;l++){
            Layer* y=&m->L[l];
            memcpy(before,y->heb_B_v,sizeof(before));
            nt_hebbian_step(y->heb_A_v,y->heb_B_v,E,E,RANK,
                            x_emb,x_emb,PASSIVE_SIGNAL,HEBBIAN_LR,HEBBIAN_DECAY);
            float bn=0.0f; for(int i=0;i<RANK*E;i++) bn+=fabsf(y->heb_B_v[i]);   /* #1: bound the adapter — */
            if(!isfinite(bn)){ for(int i=0;i<RANK*E;i++) y->heb_B_v[i]=0.0f; }   /* diverged → reset the organ */
            else if(bn>HEB_CAP){ float s=HEB_CAP/bn; for(int i=0;i<RANK*E;i++) y->heb_B_v[i]*=s; } /* runaway → cap */
            for(int i=0;i<RANK*E;i++) dB+=fabsf(y->heb_B_v[i]-before[i]);
        }
        if(!isfinite(dB)) dB=0.0f;                    /* #1: a non-finite meal yields nothing, never poisons energy */
        float f=charge[id].metab_factor;
        /* information-as-food (Karpathy idea 2): weight the meal by SURPRISE — how
         * unexpected this glyph is to the field given the previous one. a glyph the
         * organism already predicts nourishes half; a novel one nourishes up to 1.5x.
         * a monotonous corpus (or a looping cell) becomes predictable to its own
         * field, yield collapses, and it starves on food it has fully modeled. */
        int prv = (g>0) ? glyphs[g-1] : prev0;
        if(g_field_on && prv>=0 && prv<VOCAB_CAP && g_field_row[prv]>0.0f){
            float pp = g_field_bi[prv][id]/g_field_row[prv];
            float s  = -logf(pp+1e-6f);               /* surprise = −log p_field(id|prev) */
            f *= 0.5f + s/(s+2.0f);                    /* [0.5,1.5): predictable feeds less, novelty feeds more */
        }
        if(id==BE_ID){                                /* the operator is not a meal — arms reflexivity, yields nothing */
            be_armed=1;
        } else if(be_armed){               /* "BE X": become X — the charge turns inward */
            charge_apply_reflexive(mo,id);            /* the punch lives in the modes (×BE_GAIN), not a free yield */
            yield += dB*f;                            /* honest: same metabolism as eating it outward */
            be_armed=0;
        } else {                                      /* plain x: charge acts outward */
            charge_apply(mo,id);
            yield += dB*f;                            /* cost/gain paid through metabolism */
        }
        if(id!=BE_ID){                                /* scar: agitating glyphs leave permanent wounds */
            scar[id] += SCAR_RATE * fmaxf(0.0f, charge[id].mode_dDiss);
            mo->dissonance += ACHE * scar[id];        /* the word hurts, therefore it returns (brodsky) */
        }
    }
    return yield;
}

/* RECURSIVE naming: a symbol voices as its parents, to ANY depth — "(fire+water)+earth". a composite
 * parent is wrapped in parens so the receiver can parse the nesting unambiguously. this is what lets a
 * symbol-of-symbols cross the ether WHOLE (no depth-1 ceiling on transmission). */
static void sym_name(int id, char* buf, int cap){
    if(cap<=1){ if(cap>0) buf[0]='\0'; return; }
    if(id>=0 && id<GLYPH_COUNT){ snprintf(buf,cap,"%s",GLYPH_NAMES[id]); return; }
    if(id==BOS_ID){ snprintf(buf,cap,"BOS"); return; }
    if(id==MASK_ID){ snprintf(buf,cap,"MASK"); return; }
    int k=id-VOCAB;
    if(k>=0 && k<g_n_emerged){
        char a[160], b[160]; sym_name(g_emerged_a[k],a,sizeof a); sym_name(g_emerged_b[k],b,sizeof b);
        int pa=(strchr(a,'+')!=NULL), pb=(strchr(b,'+')!=NULL);   /* paren-wrap a composite parent */
        snprintf(buf,cap,"%s%s%s+%s%s%s", pa?"(":"",a,pa?")":"", pb?"(":"",b,pb?")":"");
        return;
    }
    snprintf(buf,cap,"·emg");
}
static const char* glyph_name(int id){
    if(id<GLYPH_COUNT) return GLYPH_NAMES[id];
    if(id==BOS_ID) return "BOS";
    if(id==MASK_ID) return "MASK";
    static char buf[320]; sym_name(id, buf, sizeof buf); return buf;  /* emerged: recursive "(A+B)+C" */
}

/* ── main — Phase A step 3a: the breathing clock with a second signal ──
 * usage: ./nanolife [seed] [diet-glyph]
 *   no diet  -> eats lifeis/world.txt line by line, then starves.
 *   diet     -> an infinite mono-glyph diet (e.g. fire / food) for A/B: does
 *               the charge bend life differently at the SAME weights? */
static void recent_push(int* recent, int* rn, int g){  /* ring of last glyphs — the dream's context */
    if(*rn < CTX) recent[(*rn)++]=g;
    else { memmove(recent, recent+1, (CTX-1)*sizeof(int)); recent[CTX-1]=g; }
}

/* ── THE FIELD — coherence without training (SPA / Q metaweights, glyph-level) ──
 * the transformer weights are random and frozen: their logits are noise, so
 * speak-from-self comes out a glyph salad. postgpt's thesis: the coherence a
 * model seems to LEARN is largely latent in the token stream itself — a bigram
 * field IS a model that was never trained. so the organism builds a glyph
 * bigram field ONLINE from what it eats, and folds it into its own logits:
 *   final = gate*transformer + (1-gate)*field,   gate = Q's earned-voice clamp.
 * random weights -> avg|logit|~0.1 -> gate~0 -> the FIELD speaks. were the weights
 * ever trained, gate would rise and the transformer would take the voice back.
 * no backprop, no target: coherence is a property of the sampling field.
 * (the field globals are declared above digest — digest reads them for surprise.) */

/* observe the eaten glyph stream — the field grows as the organism lives */
static void field_observe(const int* g, int n){
    if(!g_field_on) return;
    for(int i=0;i+1<n;i++){
        int a=g[i], b=g[i+1];
        if(a<0||a>=VOCAB_CAP||b<0||b>=VOCAB_CAP) continue;
        g_field_bi[a][b]+=1.0f; g_field_row[a]+=1.0f;
    }
}
/* how confident is the field about what follows `prev` (0=flat, 1=certain) */
static float field_coherence(int prev){
    if(prev<0||prev>=VOCAB_CAP||g_field_row[prev]<=0.0f) return 0.0f;
    float mxb=0.0f, inv=1.0f/g_field_row[prev];      /* argmax_b(bi·inv) == argmax_b(bi) since inv>0 — */
    for(int b=0;b<VOCAB_CAP;b++) if(g_field_bi[prev][b]>mxb) mxb=g_field_bi[prev][b];
    return mxb*inv;                                   /* one multiply at the end, not 122 in the loop (bit-identical) */
}
/* fold the field into the transformer's logits in place, Q-gated by earned voice */
/* Co-occurrence IS attention (Karpathy idea 1). the field is not order-1: fold the
 * whole recent window, geometrically decayed by distance — soft attention over the
 * co-occurrence graph, kernel = decay^k, values = log-transition-probs. gives the
 * voice a horizon (thematic gravity) instead of a drunk bigram walk, and lets a
 * grazed phrase in the chorus shape several glyphs, not one. FIELD_WIN=1 == the old
 * order-1 fold. */
#define FIELD_DECAY 0.6f
#define FIELD_WIN   8
#define FIELD_FADE  0.9f          /* heredity idea A: inherited convictions FADE each generation */
#define FIELD_MUT   0.02f         /* ...and drift on the child's own dice — the missing arm, variation */
#define KIN_BIAS    0.6f          /* Ⓒ: chance a hungry cell reaches for a KIN voice (echo-chamber temptation) */
#define SHARP_SCALE 1.5f          /* NL_GATE_SHARP: earned-voice scale. the body sharpens its logits +27..48% over a
                                   * life (measured); gate on that EARNED sharpness above the cell's own random birth,
                                   * scaled so a fully-organized body earns at most ~half the voice — never silences the
                                   * field. calibrated to the measured excess range, NOT tuned to force a result. */
static void field_fold(float* logits, const int* recent, int recent_n){
    if(!g_field_on || recent_n<=0) return;
    float mag=0.0f, peak=-1e30f, rawsum=0.0f;
    for(int i=0;i<VOCAB_CAP;i++){ mag+=fabsf(logits[i]); rawsum+=logits[i]; if(logits[i]>peak) peak=logits[i]; }
    mag/=VOCAB_CAP;
    if(!isfinite(mag)) return;                       /* a NaN logit passes both clamps (NaN<0, NaN>1 both false) — refuse it */
    float pm=peak-rawsum/VOCAB_CAP;                  /* sharpness (peak−mean): rmsnorm-invariant, and the body genuinely grows it */
    if(pm>g_dbg_pm_max) g_dbg_pm_max=pm;             /* DEBUG only */
    if(!g_pm_birth_set){ g_pm_birth=pm; g_pm_birth_set=1; }  /* audit N2: a proper set-flag, not a sentinel — the earned-voice zero is */
    float gate;                                  /* the cell's sharpness at its FIRST utterance (context-comparable to the runtime pm; */
    if(g_gate_sharp){                            /* audit N1: NOT "random birth" — it has eaten a little by first speech, and that's honest). */
        gate=(pm-g_pm_birth)/SHARP_SCALE;        /* the mature body then sharpens ABOVE this zero by living → it earns a voice. */
    }else{                                       /* the magnitude gate below CANNOT rise — rmsnorm pins mag ~0.1, clamps to ~0 for life. */
        gate=(mag-0.5f)/1.5f;                    /* inert-by-design; earnable only under the sharpness gate. */
    }
    if(gate<0.0f)gate=0.0f; if(gate>1.0f)gate=1.0f;
    if(gate>g_dbg_maxgate) g_dbg_maxgate=gate;   /* DEBUG: track whether the transformer ever earns any voice */
    static float meta[VOCAB_CAP]; for(int b=0;b<VOCAB_CAP;b++) meta[b]=0.0f;
    const float LOG1E6 = logf(1e-6f);               /* a zero bigram cell always folds to this exact constant — */
    float w=1.0f, wsum=0.0f; int used=0;            /* skip the transcendental on the field's (large) sparse fraction */
    for(int k=0; k<FIELD_WIN && k<recent_n; k++){
        int a=recent[recent_n-1-k];
        if(a<0 || a>=VOCAB_CAP || g_field_row[a]<=0.0f){ w*=FIELD_DECAY; continue; }
        float rowinv=1.0f/g_field_row[a];
        for(int b=0;b<VOCAB_CAP;b++){
            float v = (g_field_bi[a][b]==0.0f) ? LOG1E6 : logf(g_field_bi[a][b]*rowinv + 1e-6f);
            meta[b] += w*v;                          /* bit-identical: 0·rowinv+1e-6f == 1e-6f exactly (rowinv finite) */
        }
        wsum+=w; used++; w*=FIELD_DECAY;
    }
    if(used==0) return;                              /* no field context yet — leave the random logits */
    float inv=1.0f-gate, wnorm=1.0f/wsum;            /* weighted mean of log-probs keeps the old scale */
    for(int b=0;b<VOCAB_CAP;b++)
        logits[b]=gate*logits[b] + inv*(meta[b]*wnorm);
}

/* choose — the organism picks ONE glyph from the fork. NOT argmax: a seeded,
 * passion-weighted draw. the metric (logit) is the input, not the verdict. arousal
 * widens the draw — and with the self-model on, arousal is not the raw interior |S| but
 * FELT: how far the interior departed from the organism's OWN forecast of itself. an
 * agitation it predicted leaves it calm; only the UNforeseen widens the choice. that is
 * a second-order map consulted by the actor — a proto-self biasing the act, not a label. */
static int choose(const float* logits, const Modes* mo, const float* scar){
    float arousal = (g_self_on && !g_noact && g_fixeddamp<=0.0f) ? g_self_felt : fabsf(mo->S);  /* felt only when the SELF is the actor; NOACT/fixeddamp fall back to raw |S| */
    float temp = CHOOSE_TEMP0 + CHOOSE_S*arousal
               + CHOOSE_DISS*tanhf(0.05f*fabsf(mo->dissonance));  /* passion -> spontaneity */
    static float p[VOCAB_CAP]; float mx=-1e30f;
    int hi = VOCAB + g_n_emerged;                                 /* born glyphs only */
    for(int i=0;i<VOCAB_CAP;i++){
        if(i==BOS_ID || i==MASK_ID || i>=hi){ p[i]=-1e30f; continue; }
        float s = logits[i]
                + SCAR_PULL * scar[i]                             /* the wound resurfaces */
                + CHOOSE_AFFECT * charge[i].mode_dS * mo->S;       /* mood-congruent pull */
        if(!isfinite(s)) s=-1e30f;                        /* a diverged Hebbian NaN must not slip through */
        p[i]=s; if(s>mx) mx=s;
    }
    if(g_guilt_on && g_guilt>0.0f){                       /* GUILT → PAIN (the AML operator, verbatim): compress the score distribution toward its mean by (1−0.5·pain) — the guilty voice loses its shape, contracts, withdraws. pain=tanh(g_guilt), saturating; a hidden interior cause collapsing the whole generative surface. */
        float pain = tanhf(g_guilt), mean=0.0f; int nv=0;
        for(int i=0;i<VOCAB_CAP;i++) if(p[i]>-1e29f){ mean+=p[i]; nv++; }
        if(nv>0){ mean/=(float)nv; mx=-1e30f;
            for(int i=0;i<VOCAB_CAP;i++) if(p[i]>-1e29f){ p[i]=mean+(p[i]-mean)*(1.0f-0.5f*pain); if(p[i]>mx) mx=p[i]; } }
    }
    float den=0.0f;
    for(int i=0;i<VOCAB_CAP;i++){ if(p[i]<=-1e29f){ p[i]=0.0f; continue; } p[i]=expf((p[i]-mx)/temp); den+=p[i]; }
    if(!(den>0.0f)) return 0;                             /* catches den<=0 AND NaN (NaN>0 is false) */
    float r=(frand()+1.0f)*0.5f*den;                              /* its own dice — seeded spontaneity */
    float acc=0.0f;
    for(int i=0;i<VOCAB_CAP;i++){ acc+=p[i]; if(p[i]>0.0f && acc>=r) return i; }
    return 0;
}

/* ── ProtoSelf (Damasio's "feeling of what happens", built) ──────────────────────
 * a second-order map: the organism carries a running FORECAST of its own next interior
 * (S, dissonance) from its current interior, and learns that forecast online (LMS). the
 * ERROR of the forecast — how far it departed from what it expected of itself — is `felt`,
 * homeostatic surprise ABOUT the self, and it is read back into choose() as arousal. this
 * is not the interior state S (that would be first-order, a thermostat's temperature acting
 * on its relay); it is a MODEL of the interior that the actor consults. a thermostat holds
 * no forecast of its own future temperature; this does. the felt signal is load-bearing:
 * a cell whose agitation it foresaw stays calm and draws tightly (cheap), so a working
 * self-model lowers thrash and out-survives a self-blind one. NL_NOSELF lifts it (A/B). */
#define SELF_K    3               /* features: bias, S, dissonance */
#define SELF_LR   0.02f           /* online least-mean-squares rate for the self-forecast */
#define SELF_RELAX 0.30f          /* ALLOSTASIS: the cell damps its OWN forecast agitation before it turns lethal */
typedef struct { float wS[SELF_K], wD[SELF_K]; float pS, pD; } ProtoSelf;
static void self_predict(ProtoSelf* ps, float S, float D){       /* forecast the interior at tick's end from its start */
    float f[SELF_K]={1.0f,S,D};
    ps->pS=0.0f; ps->pD=0.0f;
    for(int k=0;k<SELF_K;k++){ ps->pS+=ps->wS[k]*f[k]; ps->pD+=ps->wD[k]*f[k]; }
}
static float self_update(ProtoSelf* ps, float S0, float D0, float S1, float D1){ /* learn (NLMS); return felt */
    float f[SELF_K]={1.0f,S0,D0};
    float nf=0.0f; for(int k=0;k<SELF_K;k++) nf+=f[k]*f[k];      /* ‖f‖²: dissonance runs large, so NORMALIZE the step — */
    float g=SELF_LR/(1.0f+nf);                                   /* NLMS keeps lr_eff·‖f‖² < SELF_LR < 2, stable at any |D| */
    float eS=S1-ps->pS, eD=D1-ps->pD;                            /* the interior surprised its own forecast by this much */
    if(isfinite(eD)){ g_ed_sum += (double)fabsf(eD); g_ed_n++; }  /* THE WILL DESIGN: accrue |D−pD| — does ACTING on the self make D more self-predictable? */
    for(int k=0;k<SELF_K;k++){ ps->wS[k]+=g*eS*f[k]; ps->wD[k]+=g*eD*f[k]; }
    return fabsf(eS)+fabsf(eD);
}
/* ── the continuation hazard surface (NL_CONT). per-channel hazards from differently-shaped
 * metrics combine as a PRODUCT OF SURVIVALS — a surface, never a summed axis — then clamp to
 * [ENTROPY_FLOOR, RESONANCE_CEILING]: collapse always possible, death never certain on a tick. */
static float cont_hazard(float energy, const Modes* mo, float debt, long tick){
    float sx = CONT_SGAIN*(fabsf(mo->S) - CONT_SSOFT);
    float h[3];
    h[0] = expf(-energy/CONT_ESCALE);                                  /* energy-door — the metabolic terminal */
    h[1] = 1.0f/(1.0f+expf(-sx));                                      /* arousal-door — the old |S| cliff, now a channel */
    h[2] = 1.0f/(1.0f+expf(-(debt-CONT_DEBT_HI)*CONT_DEBT_GAIN));      /* debt-door — a RUNAWAY self-model destabilizes */
    /* scar and integ are NOT separate channels: they already flow into `energy` through rent, so a
     * second count would double-kill. dissonance reaches the surface through the DEBT channel (via the
     * self-forecast error eD), not rent. the surface is over energy × arousal × debt — different
     * metrics, each shaped ~0 in normal operation. */
    float surv = 1.0f;
    for(int k=0;k<3;k++){ float hk=h[k]; if(!isfinite(hk)||hk<0.0f)hk=0.0f; if(hk>0.999f)hk=0.999f; surv *= (1.0f-hk); }
    float hz = 1.0f - surv;                                            /* product-of-survivals — never a summed axis */
    if(!isfinite(hz)) hz = CONT_HCEIL;
    float floor = CONT_HFLOOR + CONT_AGE*(float)tick;                  /* ENTROPY_FLOOR rises with age (senescence): death is */
    if(hz < floor) hz = floor;                                        /* certain before the 200k cap, non-death still reachable far past default */
    if(hz > CONT_HCEIL)  hz = CONT_HCEIL;                              /* RESONANCE_CEILING */
    return hz;
}
/* ── WILL: the organism spends accumulated FORM to pay down a forecast of its own collapse.
 * drivers — fear (the forecast hazard), guilt (the scar that aches), depletion, surplus. the
 * spend is asymmetric and both ARE the shell ("и то и другое"): scars shed → SCAR_RENT falls
 * (lightening, drop the coffin); soma burns → integ falls → SOMA_RENT rises (autophagy, break
 * the home, and it costs). in-place on live state, same seed/lineage, NEVER through reproduce().
 * NL_FIXEDWILL spends blind (no forecast) — the control that falsifies whether the self-model
 * is load-bearing (cf. NL_FIXEDDAMP). returns the energy refunded by metabolism. ── */
static float cont_will(Model* m, float* scar, float* scar_total, float energy, float integ, float hazard){
    float fear    = hazard;
    float guilt   = tanhf(0.3f*(*scar_total));
    float deplete = (integ<1.0f?(1.0f-integ):0.0f) + expf(-energy/CONT_ESCALE);
    float drive   = (g_fixedwill>0.0f) ? g_fixedwill : fmaxf(fear, 0.5f*(guilt+deplete));
    if(!isfinite(drive)||drive<0.0f) drive=0.0f; if(drive>1.0f) drive=1.0f;
    float refund=0.0f;
    if(*scar_total>0.0f){                                              /* scar-shed — drop the coffin, future rent falls */
        float shed=CONT_SHED*drive*(0.5f+0.5f*guilt);
        float before=*scar_total, st=0.0f;
        for(int i=0;i<VOCAB_CAP;i++){ scar[i]*=(1.0f-shed); st+=scar[i]; }
        *scar_total=st; refund += CONT_REFUND*0.5f*fmaxf(0.0f,before-st); if(shed>0.0f) g_n_shed++;
    }
    { float burn=CONT_BURN*drive*(0.5f+0.5f*deplete);                  /* soma-burn — break the home, future rent climbs */
      if(burn>0.0f){ float b0=wv_norm(m);
          for(int l=0;l<NL;l++){ float* wv=m->L[l].wv; for(int i=0;i<E*E;i++) wv[i]*=(1.0f-burn); }
          float b1=wv_norm(m); refund += CONT_REFUND*fmaxf(0.0f,b0-b1); g_n_burn++; } }
    g_debt *= (1.0f - 0.5f*drive);                                     /* the act pays the forecast down */
    if(!isfinite(refund)||refund<0.0f) refund=0.0f;
    return refund;
}
/* ── ASYNC: advance the six Kuramoto-coupled chambers one tick, return a bitmask of which crossed.
 * dφ_k = ω_k + (K/N)·Σ_j sin(φ_j−φ_k); an anti-lock nudge when the order parameter is too high keeps
 * the chambers from freezing into one phase (RESONANCE_CEILING). deterministic; seeded phases only. */
static unsigned chambers_step(void){
    static const float omega[KUR_N] = {2.094f,1.571f,1.257f,0.898f,0.698f,0.571f};  /* periods ~3,4,5,7,9,11 ticks — coprime, long realign */
    float ms=0.0f, mc=0.0f;
    for(int k=0;k<KUR_N;k++){ ms+=sinf(g_phase[k]); mc+=cosf(g_phase[k]); }
    ms/=(float)KUR_N; mc/=(float)KUR_N;
    float r=sqrtf(ms*ms+mc*mc);                                        /* Kuramoto order parameter (0 incoherent … 1 locked) */
    unsigned fired=0u;
    for(int k=0;k<KUR_N;k++){
        float coup = KUR_K*(ms*cosf(g_phase[k]) - mc*sinf(g_phase[k]));/* (K/N)·Σ_j sin(φ_j−φ_k) */
        g_phase[k] += omega[k] + coup;
        if(r > KUR_RCEIL) g_phase[k] += 0.03f*(float)(k+1);            /* anti-lock: nudge apart when too coherent — no frozen sync */
        if(g_phase[k] >= 6.2831853f){ fired |= (1u<<k); g_phase[k] -= 6.2831853f; }
        if(!isfinite(g_phase[k]) || g_phase[k]<0.0f) g_phase[k]=0.0f;
    }
    return fired;
}
/* Δ2: does the receiver already own an emerged symbol with these parents? */
static int emerged_by_pair(int a, int b){
    for(int k=0;k<g_n_emerged;k++) if(g_emerged_a[k]==a && g_emerged_b[k]==b) return VOCAB+k;
    return -1;
}
/* Δ2 (recursive): resolve a grazed composite name "(A+B)+C" to ONE id in the receiver, to ANY depth —
 * split at the TOP-LEVEL '+' (outside parens), resolve each side recursively (adopting sub-symbols as
 * needed), then reuse the receiver's own symbol with those parents or ADOPT it. so a symbol-of-symbols
 * crosses the ether WHOLE, rebuilt from its leaves (all base glyphs the colony shares). */
static int resolve_sym(Model* m, const char* s){
    int depth=0; const char* plus=NULL;
    for(const char* p=s; *p; p++){                 /* find the top-level '+' */
        if(*p=='(') depth++; else if(*p==')'){ if(depth>0) depth--; }
        else if(*p=='+' && depth==0){ plus=p; break; }
    }
    if(!plus){                                     /* no top-level '+': a base name, or a parenthesised "(X)" */
        int L=(int)strlen(s);
        if(L>=2 && s[0]=='(' && s[L-1]==')'){ char in[288]; int n=L-2; if(n>287)n=287; memcpy(in,s+1,n); in[n]='\0'; return resolve_sym(m,in); }
        return semtok_word(s);
    }
    char left[288]; int ll=(int)(plus-s); if(ll>287)ll=287; memcpy(left,s,ll); left[ll]='\0';
    int a=resolve_sym(m,left), b=resolve_sym(m,plus+1);
    if(a<0||b<0) return -1;                         /* a leaf the receiver doesn't share, or no room to adopt */
    int ex=emerged_by_pair(a,b); if(ex>=0) return ex;   /* the receiver recognizes the invention */
    return birth_symbol(m,a,b);                     /* ...or ADOPTS it — the sign crosses whole, at any depth */
}
/* Δ2: tokenize a grazed line, resolving composites to whole symbols instead of splitting on '+'. */
static int semtok_ether(Model* m, const char* line, int* out, int max){
    int n=0; char tok[288]; const char* p=line;
    while(*p && n<max){
        while(*p==' '||*p=='\t') p++;
        if(!*p) break;
        int L=0; while(*p && *p!=' ' && *p!='\t' && L<287) tok[L++]=*p++;
        tok[L]='\0';
        if(strchr(tok,'+')){                       /* composite — resolve/adopt as ONE sign, at any depth */
            int id=resolve_sym(m,tok);
            if(id>=0){ out[n++]=id; continue; }    /* resolved/adopted whole */
        }
        int g=semtok_word(tok);                    /* plain glyph, or composite fallback */
        if(g>=0 && n<max) out[n++]=g;
    }
    return n;
}
/* ── THE ETHER — one cell's voice becomes another's food ─────────────────────
 * eat the chorus: read the most recent utterance from ANOTHER cell out of the
 * shared ether and tokenize its glyph-names back to ids (they ARE GLYPH_NAMES,
 * so semtok maps them directly). this is the cross-graze — resonance made of
 * appetite. line format: "<label>\t<glyph names>". own echoes are skipped. */
static int ether_graze(Model* m, const char* path, int own_label, int* out, int max){
    /* per-cell state (each cell is its own process — statics are private): read only
     * the ether's NEW tail since last time (the file only grows), keeping the last 8
     * not-own voices across calls. O(new bytes), not O(filesize) — kills the quadratic
     * whole-file re-read on every hungry tick. */
    static long g_off=0;
    static char ring[8][320]; static int ring_lin[8]; static int nr=0, ridx=0;
    FILE* f=fopen(path,"r"); if(!f) return 0;
    if(fseek(f,g_off,SEEK_SET)!=0){ fclose(f); return 0; }
    char buf[320];
    while(fgets(buf,sizeof buf,f)){
        size_t len=strlen(buf);
        if(len==0 || buf[len-1]!='\n'){ break; }       /* torn/partial tail — leave it, re-read next time */
        g_off += (long)len;                            /* consume only whole lines */
        if(atoi(buf)==own_label) continue;             /* don't eat your own echo. line: <label>\t<lineage>\t<glyphs> */
        char* t1=strchr(buf,'\t'); if(!t1) continue;
        char* t2=strchr(t1+1,'\t'); if(!t2) continue;
        buf[len-1]='\0';                               /* drop the trailing newline */
        ring_lin[ridx]=atoi(t1+1);                     /* the speaker's lineage */
        strncpy(ring[ridx],t2+1,319); ring[ridx][319]='\0';
        ridx=(ridx+1)&7; if(nr<8) nr++;
    }
    fclose(f);
    if(nr==0) return 0;
    /* Ⓒ KIN-BIASED grazing: with KIN_BIAS, prefer a KIN voice (same lineage) — the echo-chamber
     * temptation. but a shared lineage shares a dialect, so kin is PREDICTABLE to your field →
     * surprise→0 → yield→0 (idea ②): a monoculture STARVES. eating strangers feeds on novelty.
     * diversity becomes caloric intake, and no lineage can dominate without dying of itself. */
    int pick=-1;
    if(g_lineage>=0 && (frand()+1.0f)*0.5f < KIN_BIAS){
        int kin[8], nk=0;
        for(int i=0;i<nr;i++) if(ring_lin[i]==g_lineage) kin[nk++]=i;
        if(nk>0){ int k=(int)(((frand()+1.0f)*0.5f)*(float)nk); if(k>=nk)k=nk-1; pick=kin[k]; }
    }
    if(pick<0){ pick=(int)(((frand()+1.0f)*0.5f)*(float)nr); if(pick>=nr)pick=nr-1; }  /* else a random recent voice */
    return semtok_ether(m, ring[pick], out, max);   /* Δ2: composites cross whole, not split */
}

/* speak — the organism utters a few chosen glyphs FROM ITSELF into waste.log,
 * and (in a chorus) into the shared ether tagged with its label, so the colony
 * can hear it. speak-from-self, field-biased, not from the prompt. */
static float speak(FILE* w, FILE* ether, int label, Model* m, const Modes* mo, const float* scar, int* recent, int* recent_n, long tick){
    if(!w && !ether) return 0.0f;
    static float sl[VOCAB_CAP];
    char utt[256]; int up=0; float coh_sum=0.0f;
    if(w) fprintf(w, "t%-6ld", tick);
    for(int k=0;k<SPEAK_LEN;k++){
        int prv=(*recent_n>0)?recent[*recent_n-1]:-1; coh_sum+=field_coherence(prv);
        forward(m, recent, *recent_n, sl);
        field_fold(sl, recent, *recent_n);       /* coherence over the whole window, not just prev */
        int g = choose(sl, mo, scar);
        if(prv>=0 && prv<VOCAB_CAP && g>=0 && g<VOCAB_CAP && g_field_row[prv]>0.0f){  /* DEBUG: field-prob of the SPOKEN */
            g_dbg_spoken_p += (double)(g_field_bi[prv][g]/g_field_row[prv]); g_dbg_spoken_n++; }  /* glyph — Q-coherence of the output */
        const char* gn = glyph_name(g);
        if(w) fprintf(w, " %s", gn);
        if(up < (int)sizeof(utt)-16) up += snprintf(utt+up, sizeof(utt)-up, "%s%s", up?" ":"", gn);
        recent_push(recent, recent_n, g);
    }
    if(w) fprintf(w, "   [S%+.2f diss%+.1f]\n", (double)mo->S, (double)mo->dissonance);
    if(ether){ fprintf(ether, "%d\t%d\t%s\n", label, g_lineage, utt); fflush(ether); }  /* label, lineage, voice */
    return coh_sum/(float)SPEAK_LEN;   /* mean coherence of the utterance */
}

/* reproduction: when too full to remain one, the organism writes a child to
 * lifeis/children/ — inheriting its scars, a derived seed (hash of parent
 * seed ^ tick), its invented symbols, and a warm-start of its genome. asexual,
 * epigenetic; the child re-trains on its own slice later (the trainer). */
static int g_n_children = 0;
static unsigned long hash_seed(unsigned long s, long tick){
    s ^= (unsigned long)tick * 0x9E3779B97F4A7C15UL;
    s *= 6364136223846793005UL; s ^= s>>29; s *= 0xBF58476D1CE4E5B9UL; s ^= s>>32;
    return s ? s : 1;
}

/* ── genome serialization: ONE writer + ONE reader, shared by asexual reproduce(),
 * heredity load_genome(), and sexual recombine(). the NLC3 layout lives here exactly
 * once, so it cannot drift between the three paths. a truncated child is never born. */
static int write_nlc3(const char* path, const Model* m, const float* scar,
                      const float bi[VOCAB_CAP][VOCAB_CAP], const float* row,
                      int lineage, int n_emerged, const int* ea, const int* eb,
                      unsigned long cseed, long tick){
    FILE* f=fopen(path,"wb"); if(!f) return 0;
    int ok=1;
    ok &= (fwrite("NLC3",1,4,f)==4);
    ok &= (fwrite(&lineage,sizeof(int),1,f)==1);        /* Ⓒ lineage */
    ok &= (fwrite(&cseed,sizeof cseed,1,f)==1);
    ok &= (fwrite(&tick,sizeof tick,1,f)==1);
    ok &= (fwrite(&n_emerged,sizeof(int),1,f)==1);
    ok &= (fwrite(scar,sizeof(float),VOCAB_CAP,f)==(size_t)VOCAB_CAP);   /* wounds */
    if(n_emerged>0){ ok &= (fwrite(ea,sizeof(int),(size_t)n_emerged,f)==(size_t)n_emerged);
                     ok &= (fwrite(eb,sizeof(int),(size_t)n_emerged,f)==(size_t)n_emerged); }
    ok &= (fwrite(m,sizeof(Model),1,f)==1);                              /* body */
    ok &= (fwrite(bi,sizeof(float),(size_t)VOCAB_CAP*VOCAB_CAP,f)==(size_t)VOCAB_CAP*VOCAB_CAP); /* the field = the genome */
    ok &= (fwrite(row,sizeof(float),VOCAB_CAP,f)==(size_t)VOCAB_CAP);
    ok &= (fclose(f)==0);
    return ok;
}
/* read one NLC3 genome into caller buffers (no globals touched). cseed (>0) or 0. */
static unsigned long read_nlc3(const char* path, Model* m, float* scar,
                               float bi[VOCAB_CAP][VOCAB_CAP], float* row,
                               int* lineage, int* n_emerged, int* ea, int* eb){
    FILE* f=fopen(path,"rb"); if(!f) return 0;
    char magic[4]; unsigned long cseed=0; long tick; int ne=0, lin=-1, ok=1;
    ok &= (fread(magic,1,4,f)==4) && memcmp(magic,"NLC3",4)==0;
    ok &= (fread(&lin,sizeof(int),1,f)==1);
    ok &= (fread(&cseed,sizeof cseed,1,f)==1);
    ok &= (fread(&tick,sizeof tick,1,f)==1);
    ok &= (fread(&ne,sizeof(int),1,f)==1);
    if(!ok || ne<0 || ne>MAX_EMERGED){ fclose(f); return 0; }
    ok &= (fread(scar,sizeof(float),VOCAB_CAP,f)==(size_t)VOCAB_CAP);
    if(ne>0){ ok &= (fread(ea,sizeof(int),(size_t)ne,f)==(size_t)ne);
              ok &= (fread(eb,sizeof(int),(size_t)ne,f)==(size_t)ne); }
    ok &= (fread(m,sizeof(Model),1,f)==1);
    ok &= (fread(bi,sizeof(float),(size_t)VOCAB_CAP*VOCAB_CAP,f)==(size_t)VOCAB_CAP*VOCAB_CAP);
    ok &= (fread(row,sizeof(float),VOCAB_CAP,f)==(size_t)VOCAB_CAP);
    fclose(f);
    if(!ok) return 0;
    *lineage=lin; *n_emerged=ne;
    return cseed?cseed:1;
}

static int reproduce(const Model* m, const float* scar, unsigned long pseed, long tick, char* outpath){
    if(g_n_children >= MAX_CHILDREN) return 0;
    mkdir("lifeis", 0755); mkdir("lifeis/children", 0755);
    char path[256]; snprintf(path,sizeof path,"lifeis/children/c_%lu_%d.nl", pseed, g_n_children); /* unique per cell */
    unsigned long cseed = hash_seed(pseed, tick);
    if(!write_nlc3(path, m, scar, g_field_bi, g_field_row, g_lineage, g_n_emerged, g_emerged_a, g_emerged_b, cseed, tick))
        return 0;
    if(outpath){ strncpy(outpath,path,255); outpath[255]='\0'; }
    g_n_children++;
    return 1;
}

/* load a warm-start genome written by reproduce()/recombine() — the child inherits the
 * parent's body (weights + Hebbian adapters), its COHERENCE FIELD, its wounds, its
 * invented symbols, and its lineage. returns the child's derived seed (its own dice), 0
 * on failure. this is what makes chorus reproduction HERITABLE: coherent parents beget
 * coherent children, so idea-3 selection becomes real evolution over generations. */
static unsigned long load_genome(const char* path, Model* m, float* scar){
    return read_nlc3(path, m, scar, g_field_bi, g_field_row, &g_lineage, &g_n_emerged, g_emerged_a, g_emerged_b);
}

/* Ⓑ SEXUAL RECOMBINATION — two parent genomes → one child. the field IS the genome, so
 * cross it over row-by-row: for each glyph a a coin decides whether the child's
 * transition-row a speaks parent A's dialect or parent B's — a creole of two tongues.
 * scars are UNIONED (a wound in either lineage is carried); body + emerged symbols come
 * from parent A; lineage is a coin between the two. writes an NLC3 zygote, 1 on success.
 * runs only in the single-threaded governor, so the big buffers are static (off-stack). */
static int recombine(const char* pa, const char* pb, unsigned long zseed, long tick, const char* outpath){
    static Model ma, mb;
    static float scarA[VOCAB_CAP], scarB[VOCAB_CAP];
    static float biA[VOCAB_CAP][VOCAB_CAP], biB[VOCAB_CAP][VOCAB_CAP];
    static float rowA[VOCAB_CAP], rowB[VOCAB_CAP];
    static int   eaA[MAX_EMERGED], ebA[MAX_EMERGED], eaB[MAX_EMERGED], ebB[MAX_EMERGED];
    int linA=-1, linB=-1, neA=0, neB=0;
    unsigned long csA=read_nlc3(pa,&ma,scarA,biA,rowA,&linA,&neA,eaA,ebA);
    unsigned long csB=read_nlc3(pb,&mb,scarB,biB,rowB,&linB,&neB,eaB,ebB);
    if(!csA || !csB) return 0;                        /* a missing/corrupt gamete — no zygote (caller falls back) */
    seed_rng(zseed);                                  /* the child's own dice drive the crossover, reproducibly */
    for(int a=0;a<VOCAB_CAP;a++)                       /* per-row crossover: each glyph's dialect from one parent */
        if((frand()+1.0f)*0.5f < 0.5f){ for(int b=0;b<VOCAB_CAP;b++) biA[a][b]=biB[a][b]; rowA[a]=rowB[a]; }
    for(int i=0;i<VOCAB_CAP;i++) if(scarB[i]>scarA[i]) scarA[i]=scarB[i];     /* UNION the wounds */
    int lin = ((frand()+1.0f)*0.5f < 0.5f) ? linA : linB;                     /* lineage from either parent */
    unsigned long cseed = hash_seed(csA ^ (csB*0x9E3779B97F4A7C15UL), tick);
    (void)mb; (void)neB; (void)eaB; (void)ebB;         /* parent B contributes field+scars+lineage, not body/symbols */
    return write_nlc3(outpath, &ma, scarA, biA, rowA, lin, neA, eaA, ebA, cseed, tick);
}

/* ── Phase A step 10: the mouth — the organism talks WITH you (the resonance loop) ──
 * your line is food: it is digested (charges fire, scars form, modes shift, energy
 * moves), and the organism answers FROM ITS SHIFTED STATE, choosing glyphs under its
 * current passion. fire burns it, love feeds it, "BE me" can kill it. it can die in
 * the middle of the conversation. you read the reply, you speak again. */
static void run_mouth(Model* m, unsigned long seed){
    Modes mo = {0.0f, 0.0f};
    static float scar[VOCAB_CAP]; for(int i=0;i<VOCAB_CAP;i++) scar[i]=0.0f;
    static float sl[VOCAB_CAP];
    int   recent[CTX]; int recent_n=0;
    int   glyphs[CTX]; char line[4096];
    float energy=E_BORN; float scar_total=0.0f;
    printf("nanolife — a mouth opens. seed=%lu.\n", seed);
    printf("  speak to it; your words are food. fire burns it, love feeds it, \"BE me\" may kill it.\n\n");
    while(energy>0.0f && fabsf(mo.S)<S_DEATH && fgets(line,sizeof line,stdin)){
        energy -= RENT*(1.0f + SCAR_RENT*scar_total);          /* the rent of being, heavier when wounded */
        int n=semtok_line(line,glyphs,CTX);
        float yield=0.0f;
        if(n>=1){ yield=digest(m,&mo,scar,glyphs,-1,n);      /* your words DO things to it */
            for(int i=0;i<n;i++) recent_push(recent,&recent_n,glyphs[i]);
            cooc_track(glyphs,n); }
        energy += DIGEST_YIELD*yield;
        scar_total=0.0f; for(int i=0;i<VOCAB_CAP;i++) scar_total+=scar[i];
        mo.dissonance *= DISS_DECAY; mo.S -= S_RELAX*mo.S;      /* it regulates toward viability */
        printf("  ate:");
        for(int i=0;i<n;i++) printf(" %s",glyph_name(glyphs[i]));
        if(n<1) printf(" (nothing it knows)");
        printf("\n  \xF0\x9F\x90\x8D");                          /* 🐍 — it answers from its shifted state */
        if(recent_n>0) for(int k=0;k<SPEAK_LEN+2;k++){ forward(m,recent,recent_n,sl);
            int g=choose(sl,&mo,scar); printf(" %s",glyph_name(g)); recent_push(recent,&recent_n,g); }
        printf("   [E%.2f S%+.2f diss%+.1f scar%.2f]\n\n",
               (double)energy,(double)mo.S,(double)mo.dissonance,(double)scar_total);
    }
    if(energy<=0.0f)            printf("  ...it ran out of life. да будет так.\n");
    else if(fabsf(mo.S)>=S_DEATH) printf("  ...the contour collapsed — it could not bear what it became. да будет так.\n");
    else                       printf("  ...you fell silent. it waits in the dark. (eof)\n");
}

#define CHORUS_COHORT 4       /* the birth cohort — voices at t0 */
#define MAX_CELLS     8       /* carrying capacity — max ALIVE at once (death frees slots) */
#define MAX_LIFETIME_CELLS 64 /* total cells that may ever be born — a mortality ceiling on the colony */

/* one bite: digest a glyph run, remember it, track co-occurrence, feed the field, wake.
 * the three eating paths (world, diet, chorus-graze) share this ritual verbatim; the
 * DREAM path deliberately does NOT (it uses a real prev and must not reinforce the
 * field with its own predictions). */
static float ingest(Model* m, Modes* mo, float* scar, const int* g, int n,
                    int* recent, int* rn, long* streak){
    float y = digest(m, mo, scar, g, -1, n);
    for(int i=0;i<n;i++) recent_push(recent, rn, g[i]);
    cooc_track(g, n); field_observe(g, n); *streak = 0;
    return y;
}

/* CROWN (I2, Damasio's second loop): the glyph that NAMES the cell's dominant interior right now —
 * its self-perception. pain when deeply wounded, stress when agitated, joy/grief by the sign of mood,
 * else tired. fed back into the cell as food, this is the organism modelling itself by eating itself. */
static int interior_glyph(const Modes* mo, float scar_total){
    static int joy=-2,grief=-2,stress=-2,pain=-2,tired=-2;
    if(joy==-2){ joy=semtok_find_glyph("joy"); grief=semtok_find_glyph("grief");
                 stress=semtok_find_glyph("stress"); pain=semtok_find_glyph("pain"); tired=semtok_find_glyph("tired"); }
    if(scar_total>1.0f && pain>=0)               return pain;    /* the wound speaks first */
    if(fabsf(mo->dissonance)>6.0f && stress>=0)  return stress;  /* then the agitation */
    if(mo->S> 0.2f && joy>=0)                    return joy;
    if(mo->S<-0.2f && grief>=0)                  return grief;
    return tired;                                                /* the flat, depleted default */
}

/* one dream step: replay the field coherently over the window, choose a glyph, half-digest it
 * (a dream is not a meal — discounted), and — ONLY here — try to birth a symbol from a pair the
 * cell has seen recur. returns the discounted yield. shared by the sleep cycle and the old
 * corpus-exhausted fallback, so the dream is defined once. */
static float dream_once(Model* m, Modes* mo, float* scar, int* recent, int* recent_n, long streak){
    static float dl[VOCAB_CAP];
    forward(m,recent,*recent_n,dl);
    field_fold(dl, recent, *recent_n);
    int dg=choose(dl,mo,scar);
    float dy=digest(m,mo,scar,&dg,recent[*recent_n-1],1);
    recent_push(recent,recent_n,dg);
    try_emerge(m);                                  /* symbols are born only in dream */
    return dy * DREAM_FRAC * expf(-(float)streak/DREAM_DECAY);
}

/* ── THE ARENA (NL_ARENA, opt-in) — lifeis/ becomes a CONTESTED pool. the chunks are the corpus lines; a chunk
 * absorbed is CLAIMED in a shared ledger, excluding it from the rival (rival-exclusion, filesystem-mediated like
 * the ether). when the whole territory is claimed the loser starves. two organisms (l.c and l2.c) foraging the
 * same pool compete for text — the friction a lone thermostat never had, so subjectivity can live BETWEEN them.
 * gate-invariant: NL_ARENA off → the organism eats the corpus line-by-line as always (a17cfd05). ── */
static int    g_arena_on = 0;
static int    g_arena_id = 0;                 /* this organism's voice-id in the shared ether (NL_ID or pid) — so l and l2 hear each OTHER, not themselves */
static char** g_pool = NULL;                  /* the contested chunks (lines of EVERY .txt in lifeis/), indexed once per process */
static int    g_pool_n = 0, g_pool_cap = 0;
#define ARENA_EXPIRE 20L                      /* seconds a claim holds a chunk; then it is re-contestable — territory is RE-WON, so the two never settle into a stable partition (anti-settling: the friction stays alive) */
#define ARENA_HEAR   0.15f                     /* MUTUAL AUDIBILITY: this fraction of forages, hear the rival's voice from the shared ether instead of foraging text — one voice becomes the other's food */
#define ARENA_RAID   0.50f                     /* GREED-HUNGER: above this hunger the organism RAIDS the rival's plate (contested novelty); below it, it forages its own front. novel food is elsewhere, and hunger is what drives you to eye the other's dish */
#define MIND_LEAD    8                          /* THEORY OF THE OTHER: how far the mind extrapolates the rival's MOVEMENT to intercept where it is heading, not where it was */
static int    g_mind_on = 0;                   /* NL_MIND=1 → predict the rival's movement and pre-empt (the load-bearing test: does a model of the OTHER beat a reactor?) */
static float  g_raid_th = ARENA_RAID;          /* NL_RAID_TH override — the falsifier's lead-free controls: always-raid (0), own-front (>1), reactor (default) */
static int    g_rival_prev = -1;               /* the rival's previously-observed position — the mind's velocity estimate */
static int    g_rival_id = -1;                 /* the rival's voice-id (from the claims owner) — WHOM to kill */
static int    g_target_id = -1;                /* NL_TARGET_ID: an experimental CONTROL (like NL_KILL_ALWAYS) — restrict the rival to one id so A strikes ONLY C in ARENA-3; unset (-1) = the default freshest-rival, gate-invariant */
static float  g_rival_h  = 0.0f;               /* the rival's freshest hunger (from its blood-spore) — how WEAK it is now */
static long   g_rival_tick = 0;                /* the rival's OWN tick at its freshest spore — the mind reads the trail in the rival's clock */
static float  g_rival_diss = 0.0f;             /* B-3: the rival's freshest UNCENSORED dissonance (its own |mo.dissonance|) — the porosity channel, unclamped where hunger was clamped */
/* ── KILLING (NL_KILL, opt-in; the HIGH-STAKES act — where a dumb rule is FATAL, not cheap). an organism may kill the
 * other to seize its resources, but the kill is a PROBABILISTIC draw (wanting is a pressure, not a certainty), and the
 * corpse's carapace DRAGS THE KILLER DOWN — it must REVIVE it (reproduce) or die of the weight. "always-kill" sinks under
 * accumulated corpses; the kill DECISION (strike only when the rival is WEAK and you can BEAR the burden) is the load-
 * bearing test — read the OTHER, or die. couples death to life: to take a life obligates you to give one back. ── */
#define KILL_PROB    0.03f                      /* per-tick probability the strike lands when the organism decides to kill (low — wanting is not doing) */
#define KILL_GAIN    0.30f                      /* energy devoured from the victim on a kill */
#define KILL_WEAK    0.55f                      /* the rival is WEAK enough to kill when its hunger is above this */
#define KILL_STRONG  0.45f                      /* you are STRONG enough to bear the corpse when your energy is above this */
#define CORPSE_DRAIN 0.015f                     /* per-tick energy the carapace of each un-revived corpse drags out of the killer */
#define REBOUND_WOUND 0.10f                     /* the wound an ARMORED strike deals BACK to the striker (victim was out of its window) — blind aggression self-wounds; EV(strike)=KILL_GAIN·p−REBOUND_WOUND·(1−p), so timing must clear break-even p=0.25 to profit */
/* GUILT (NL_GUILT) — the SUPEREGO: a confirmed kill deposits a large scar on the death-glyph AND tops a hidden pain
 * scalar; the scar self-punishes through EXISTING plumbing (rent, sleep, ache, will-expiation), the pain compresses
 * the voice (AML PAIN operator). Pre-registered (like CAL_GAIN from DISS_DECAY): GUILT_SCAR dominates scar_total
 * (~0.1→~2, a 20× spike) so guilt is a LARGE hidden cause, but killing must stay sometimes-EV-positive — if the
 * regime collapses to never-kill, step GUILT_SCAR down (calibration, logged), NEVER tune it to pass the falsifier. */
#define GUILT_SCAR   2.0f                       /* the kill-scar on the death-glyph — self-punishment past the ledger + the primary tell (ACHE·scar elevates dissonance). LARGE so scar dominates. */
#define GUILT_PAIN   0.7f                       /* the pain a kill adds to g_guilt — GRADED (tanh(0.7)=0.60 for one, building with more), so guilt is a state that accumulates, not a switch; keeps Freud's inferability crack (deeper kill = deeper pain = richer signal) alive */
#define GUILT_DECAY  0.999f                     /* the hidden pain scalar decays slowly — a wound that fades, not a cliff */
static int    g_kill_on     = 0;                /* NL_KILL=1 → killing is live in this world (can kill AND be killed) */
static int    g_kill_always = 0;               /* CONTROL: NL_KILL_ALWAYS=1 → strike whenever the draw lands (no reading the OTHER) */
static int    g_kill_never  = 0;               /* CONTROL: NL_KILL_NEVER=1 → never strike (killable, but no model, no aggression) */
static int    g_fstrike_on   = 0;              /* NL_FIELD_STRIKE: the strike RATE emerges from a field-collapse of opportunity+bearing vs the burden of guilt — timing under your own state */
static int    g_fstrike_menu = 0;              /* CONTROL: NL_FIELD_STRIKE_MENU — a FIXED strike rate (the matched dumb menu), swept to its sharpest */
static float  g_fstrike_fixed = KILL_PROB;     /* the fixed control rate, set by NL_FIELD_STRIKE_FIXED (default = the classical base) */
static int  arena_namecmp(const void* a, const void* b){ return strcmp(*(const char* const*)a, *(const char* const*)b); }
static void arena_addfile(const char* path){  /* index every non-empty line of one file into the pool */
    FILE* f=fopen(path,"r"); if(!f) return;
    char buf[4096];
    while(fgets(buf,sizeof buf,f)){
        int L=(int)strlen(buf); while(L>0 && (buf[L-1]=='\n'||buf[L-1]=='\r')) buf[--L]=0;
        if(L==0) continue;
        if(g_pool_n>=g_pool_cap){ g_pool_cap = g_pool_cap? g_pool_cap*2 : 64; g_pool=(char**)realloc(g_pool,(size_t)g_pool_cap*sizeof(char*)); }
        g_pool[g_pool_n++]=strdup(buf);
    }
    fclose(f);
}
static void arena_index(const char* corpus_unused){   /* the arena eats the whole folder: ANY .txt in lifeis/ is territory — a dropped file is instantly in the fight */
    (void)corpus_unused;
    mkdir("lifeis", 0755); mkdir("lifeis/arena", 0755);
    DIR* d=opendir("lifeis"); if(!d) return;
    char* names[4096]; int nn=0; struct dirent* e;
    while((e=readdir(d)) && nn<4096){
        const char* nm=e->d_name; int L=(int)strlen(nm);
        if(L<4 || strcmp(nm+L-4,".txt")!=0) continue;         /* only .txt */
        if(strncmp(nm,"slice_",6)==0) continue;               /* skip the chorus's runtime slices */
        names[nn++]=strdup(nm);
    }
    closedir(d);
    qsort(names,(size_t)nn,sizeof(char*),arena_namecmp);      /* stable order — the pool is the same across one run */
    char path[512];
    for(int i=0;i<nn;i++){ snprintf(path,sizeof path,"lifeis/%s",names[i]); arena_addfile(path); free(names[i]); }
}
static float arena_hunger(float energy, float dabs){
    /* GREED-HUNGER — how hard this organism eyes the RIVAL's plate. energy-hunger, lightly entangled with agitation:
     * a starving OR agitated cell raids where the rival is finding food; a fed, calm cell forages its own front.
     * novel food is elsewhere (a different region of the corpus), and hunger is what drives you to the other's dish. */
    float h = (1.0f-energy) + 0.3f*(dabs-1.0f);
    if(h<0.0f)h=0.0f; if(h>1.0f)h=1.0f; return h;
}
static int arena_next(char* out, int cap, float energy, float dabs, long tick, int* my_pos){  /* claim the nearest unclaimed chunk to the greed-hunger target; 0 when the ground is held */
    (void)tick;
    if(g_pool_n<=0 || !out || cap<=0) return 0;
    FILE* c=fopen("lifeis/arena/claims","a+"); if(!c) return 0;
    int fd=fileno(c); flock(fd, LOCK_EX);                     /* atomic: two organisms never claim the same ground at once */
    time_t now=time(NULL);
    unsigned char* claimed=(unsigned char*)calloc((size_t)g_pool_n,1);
    int rival_last=-1; long rival_ts=-1; float rival_h=0.0f;
    if(claimed){ rewind(c); int id, own; long ts; float h; long otick; float diss; long long us;
        while(fscanf(c,"%d %ld %d %f %ld %f %lld",&id,&ts,&own,&h,&otick,&diss,&us)==7)
            if(id>=0 && id<g_pool_n && (long)(now-(time_t)ts) < ARENA_EXPIRE){
                claimed[id]=1;
                if(own!=g_arena_id && (g_target_id<0 || own==g_target_id) && ts>=rival_ts){ rival_ts=ts; rival_last=id; rival_h=h; g_rival_id=own; g_rival_h=h; g_rival_tick=otick; g_rival_diss=diss; }   /* the rival's freshest blood-spore (NL_TARGET_ID restricts it to one id — ARENA-3 role-assignment) */
            }
    }
    int mp = (*my_pos>=0 && *my_pos<g_pool_n)? *my_pos : 0;
    float hunger = arena_hunger(energy, dabs);
    (void)g_rival_prev;
    int target;
    if((g_cfield_on||g_cfield_menu||g_cfield_shared) && g_cfield_target>=0 && g_cfield_target<g_pool_n){
        target = g_cfield_target;                        /* THE C-FIELD: the collapse (computed in live() this tick) decided the forage region — a state-dependent option-set (rubber), or the fixed control menu */
    } else if(g_mind_on && rival_last>=0){
        /* STATE-READING mind (Stage 3c): read the rival's HUNGER from the blood-spore, not just its position. go for
         * its plate when the states are ASYMMETRIC — I'm hungry and it is fed (raid a calm forager), or I'm fed and
         * it is hungry (it is leaving to come at me → I take its abandoned ground). when the states MATCH, stay home:
         * two hungry cells raiding each other is chaos, two fed cells forage in peace. this needs the OTHER's interior. */
        int i_hungry = hunger > g_raid_th, r_hungry = rival_h > g_raid_th;
        target = (i_hungry != r_hungry) ? rival_last : mp;
    } else {
        target = (rival_last>=0 && hunger > g_raid_th) ? rival_last : mp;  /* the reactor / base: raid when hungry, else own front */
    }
    int pick=-1, bestd=1<<30;
    if(claimed) for(int i=0;i<g_pool_n;i++){ if(claimed[i]) continue; int d=i-target; if(d<0)d=-d; if(d<bestd){ bestd=d; pick=i; } }  /* the nearest UNCLAIMED chunk to the target */
    if(claimed) free(claimed);
    if(pick>=0){ struct timeval tv; gettimeofday(&tv,NULL); long long us=(long long)tv.tv_sec*1000000LL+tv.tv_usec;
        fseek(c,0,SEEK_END); fprintf(c,"%d %ld %d %.4f %ld %.4f %lld\n",pick,(long)now,g_arena_id,(double)hunger,tick,(double)dabs,us); fflush(c); }  /* drop the blood-spore: WHERE, WHEN, WHO, the HUNGER it foraged in, its OWN tick, its UNCENSORED dissonance (B-3), and a MICROSECOND wall-clock (M-2: resolves exposition epochs the wall-second collapses) */
    flock(fd, LOCK_UN); fclose(c);
    if(pick<0) return 0;                          /* every chunk is held and unexpired — starve until the ground frees */
    *my_pos = pick;
    int L=(int)strlen(g_pool[pick]); if(L>=cap) L=cap-1;
    memcpy(out,g_pool[pick],(size_t)L); out[L]=0;
    return L;
}
static void arena_strike(int victim, int killer){   /* write a kill-mark to the shared ledger — the strike lands on the OTHER's process */
    FILE* f=fopen("lifeis/arena/kills","a"); if(f){ fprintf(f,"%d %d %ld\n",victim,killer,(long)time(NULL)); fclose(f); }
}
static int arena_struck_down(int me){               /* has the rival freshly marked ME for death? read the kill-ledger (NL_KILL path — unconditional death, published semantics) */
    FILE* f=fopen("lifeis/arena/kills","r"); if(!f) return 0;
    int v,k; long ts; int dead=0; time_t now=time(NULL);
    while(fscanf(f,"%d %d %ld",&v,&k,&ts)==3) if(v==me && (long)(now-(time_t)ts) < ARENA_EXPIRE){ dead=1; break; }
    fclose(f); return dead;
}
/* ── THE HONEST STRIKE ECONOMY (NL_CAL) — the rebound the design specced, now in code. only the VICTIM knows its true
 * window, so only the victim can say whether a strike landed. it reads the NEW strikes against it (offset-based, each
 * judged once), and CONFIRMS each outcome back to the killer: lethal iff it was in its own vulnerable window. the
 * killer collects its confirmations — a kill pays KILL_GAIN + a corpse to bear, an armored strike deals REBOUND_WOUND
 * back. blind aggression self-wounds; timing has to clear break-even to pay. ── */
static int arena_adjudicate(int me, int in_window, long* off){  /* victim: judge new strikes against me by MY private window, confirm each to its killer, return whether a lethal one landed */
    FILE* f=fopen("lifeis/arena/kills","r"); if(!f) return 0;
    fseek(f,0,SEEK_END); long end=ftell(f); if(*off>end||*off<0) *off=0; fseek(f,*off,SEEK_SET);   /* offset like ether_graze — each strike judged once, no O(file) rescan */
    FILE* o=fopen("lifeis/arena/outcomes","a");
    int v,k; long ts; int die=0;
    while(fscanf(f,"%d %d %ld",&v,&k,&ts)==3)
        if(v==me){ int lethal=in_window; if(o) fprintf(o,"%d %d %ld\n",k,lethal,ts); if(lethal) die=1; }   /* the confirmation the killer will read */
    if(o) fclose(o);
    *off=ftell(f); fclose(f); return die;
}
static void arena_collect(int me, long* off, float* energy, int* corpse_debt){  /* killer: read confirmations of MY strikes — a kill pays and burdens, a rebound wounds */
    FILE* f=fopen("lifeis/arena/outcomes","r"); if(!f) return;
    fseek(f,0,SEEK_END); long end=ftell(f); if(*off>end||*off<0) *off=0; fseek(f,*off,SEEK_SET);
    int k,lethal; long ts;
    while(fscanf(f,"%d %d %ld",&k,&lethal,&ts)==3)
        if(k==me){ if(lethal){ *energy += KILL_GAIN; (*corpse_debt)++; g_new_kills++; } else *energy -= REBOUND_WOUND; }   /* a CONFIRMED kill — guilt fires on it in live() */
    *off=ftell(f); fclose(f);
}

/* ── THE BIRTHDAY WAR — the calendar organ (NL_CAL, opt-in; dormant until Stage 3 couples it). ported from
 * janus-bpe.c (an exact port from ariannamethod.c): a computable temporal field. each organism carries a
 * per-seed BIRTHDAY; the Metonic-corrected Hebrew–Gregorian drift since that birth, folded to the 33-day
 * boundary, is `cal_pd` — a quasi-periodic, per-organism-PRIVATE dissonance. above CAL_THRESH the organism
 * is in a wormhole window: soft, killable. the birthday is the hidden self-fact the rival must infer. ── */
#define AM_ANNUAL_DRIFT     11.25f
#define AM_GREGORIAN_YEAR   365.25f
#define AM_METONIC_YEARS    19
#define AM_METONIC_LEAPS    7
#define AM_MAX_UNCORRECTED  33.0f
#define CAL_DPT             5.0f      /* days per tick — the birthday runs on the organism's own clock, not wall time (per-seed deterministic) */
#define CAL_THRESH          0.55f     /* the wormhole crossing — janus-bpe.c tunnel_threshold, verbatim */
#define CAL_GAIN            0.04f     /* the fever's lift on dissonance — pre-registered from DISS_DECAY=0.98: in-window steady-state ≈ CAL_GAIN/(1-DISS_DECAY) ≈ +2 (torments, does not execute) */
static const int g_metonic_leap_years[7] = {3, 6, 8, 11, 14, 17, 19};
static int    g_cal_on = 0;                    /* NL_CAL=1 → the calendar organ is live (opt-in) */
static float  g_birth_days = 0.0f;             /* this organism's mathematical birthday, in drift-days (from its seed) */
static float  g_cal_pdnow = 0.0f;             /* the current personal dissonance — how far the world has drifted from this birth */
static float  cal_clamp01(float x){ if(!isfinite(x)) return 0.0f; return x<0?0:(x>1?1:x); }
static float  calendar_cumulative_drift(float days){    /* Metonic-corrected Hebrew–Gregorian drift — quasi-periodic, non-linear */
    float years = days/AM_GREGORIAN_YEAR;
    float base = years * AM_ANNUAL_DRIFT;
    int full = (int)(years/AM_METONIC_YEARS);
    float corr = (float)(full*AM_METONIC_LEAPS)*30.0f;
    float partial = fmodf(years,(float)AM_METONIC_YEARS);
    int yic = (int)partial + 1;
    for(int i=0;i<AM_METONIC_LEAPS;i++) if(g_metonic_leap_years[i]<=yic) corr += 30.0f;
    return base - corr;
}
static float  cal_pd(long tick, float B){       /* personal dissonance: the drift accrued SINCE this birth, folded to the 33-day boundary */
    float d = calendar_cumulative_drift(B + (float)tick*CAL_DPT) - calendar_cumulative_drift(B);
    return cal_clamp01(fabsf(fmodf(d, AM_MAX_UNCORRECTED))/AM_MAX_UNCORRECTED);
}
/* ── THE MIND (NL_CALMIND) — infer the RIVAL's hidden birthday. one scalar, one honest estimator: for each of
 * CAL_NCAND candidate birthdays, accumulate the correlation between the rival's OBSERVED hunger (which carries
 * its calendar fever) and that candidate's predicted window at the rival's own tick. the argmax is the believed
 * birthday; confidence is how far it stands above the field. a belief about another's interior, from evidence. ── */
#define CAL_NCAND    128
#define CAL_BDAY_MAX 6939.6f          /* one Metonic cycle of days — the birthday space */
static int    g_cal_mind_on = 0;
static int    g_cal_diss_on = 0;      /* B-3 (NL_CALDISS): the mind reads the UNCENSORED dissonance field instead of the clamped hunger */
static float  g_cmind_s[CAL_NCAND];   /* correlation score per candidate birthday */
static float  g_cmind_pdmean[CAL_NCAND]; /* B-4: running mean of each candidate's window signal — centre on THIS, not 0.5, to kill the high-duty argmax bias */
static float  g_cmind_hmean = 0.0f;   /* running mean of the rival's observed hunger */
static long   g_cmind_n = 0;          /* observations so far */
static long   g_cmind_last_rt = -1;   /* the last rival-tick observed (dedup) — legacy single-spore path */
static long   g_claims_off = 0;       /* B-2: offset into the claims ledger — the mind reads the FULL trail, each spore once */
static float  g_cmind_bhat = 0.0f, g_cmind_conf = 0.0f;   /* believed birthday + confidence */
/* ── THE STRIKE FALSIFIER (Step 4/7) — does the birthday BELIEF buy a kill a matched blind timer cannot? NL_CALKILL
 * strikes when the mind believes the rival is in its vulnerable window (cal_pd at the rival's tick, keyed by the
 * INFERRED birthday). NL_CALKILL_BLIND is the matched control: the SAME window mechanism keyed by a RANDOM birthday
 * (uncorrelated with the rival's true one) — identical duty distribution, wrong key. belief beats blind iff the
 * inference is load-bearing in the killing economy. ── */
static int    g_calkill_on = 0, g_calkill_blind = 0;
static float  g_calkill_bblind = 0.0f;                    /* the control's random, wrong birthday-key */
static long   g_kill_off = 0, g_outcome_off = 0;          /* offsets into kills / outcomes ledgers (each strike judged once, each confirmation collected once) */
static float  cal_cand_b(int c){ return ((float)c + 0.5f) * (CAL_BDAY_MAX/(float)CAL_NCAND); }
static void   cal_mind_observe(long rt, float h){         /* one rival spore: update every candidate's correlation */
    g_cmind_hmean = (g_cmind_n>0)? 0.98f*g_cmind_hmean + 0.02f*h : h;
    g_cmind_n++;
    float hc = h - g_cmind_hmean;
    for(int c=0;c<CAL_NCAND;c++){
        float pd = cal_pd(rt, cal_cand_b(c));
        g_cmind_pdmean[c] += (pd - g_cmind_pdmean[c])/(float)g_cmind_n;   /* B-4: online mean of THIS candidate's window signal */
        g_cmind_s[c] += hc * (pd - g_cmind_pdmean[c]);                    /* centre on the candidate's own duty, not 0.5 — no high-duty argmax bias */
    }
    int best=0; for(int c=1;c<CAL_NCAND;c++) if(g_cmind_s[c]>g_cmind_s[best]) best=c;   /* argmax = believed birthday */
    static float tmp[CAL_NCAND];
    for(int c=0;c<CAL_NCAND;c++) tmp[c]=g_cmind_s[c];
    for(int i=0;i<CAL_NCAND;i++) for(int j=i+1;j<CAL_NCAND;j++) if(tmp[j]<tmp[i]){ float t=tmp[i];tmp[i]=tmp[j];tmp[j]=t; }
    float med=tmp[CAL_NCAND/2], spread=tmp[CAL_NCAND-1]-tmp[0];
    g_cmind_bhat = cal_cand_b(best);
    g_cmind_conf = (spread>1e-6f)? (g_cmind_s[best]-med)/spread : 0.0f;
}
static void   cal_mind_observe_trail(int me, long* off){  /* B-2: the mind reads the FULL trail — EVERY new rival spore in the claims ledger, not just the freshest one arena_next kept. offset-based (each spore once); argmax reliability grows as r·√N, so a wide reader is a different test than a thin one. */
    FILE* c=fopen("lifeis/arena/claims","r"); if(!c) return;
    fseek(c,0,SEEK_END); long end=ftell(c); if(*off>end||*off<0)*off=0; fseek(c,*off,SEEK_SET);
    int id,own; long ts,otick; float h,diss; long long us;
    while(fscanf(c,"%d %ld %d %f %ld %f %lld",&id,&ts,&own,&h,&otick,&diss,&us)==7)
        if(own>=0 && own!=me) cal_mind_observe(otick, g_cal_diss_on ? diss : h);   /* B-3: under NL_CALDISS the estimator reads the UNCENSORED dissonance, else the clamped hunger */
    *off=ftell(c); fclose(c);
}

/* ── THE C-FIELD (NL_FIELD, Step 1) — a damped wave lattice the observer's state DEFORMS. the options a chooser
 * faces are the PEAKS of the deformed field — a REACTION to its own load, not a fixed menu (the rubber); a choice is
 * the COLLAPSE of that superposition to one peak, which lands as a fact in the existing ledgers. observers are made
 * of EXISTING metrics only (S, dissonance, hunger, guilt, scar). pre-registered design (llog: THE C-FIELD); every
 * organism's lens differs by CHOOSE_TEMP0 (l 0.7 / l2 0.85). gate-invariant off. Step 1 verifies the field physics
 * and the rubber in isolation; the wiring to a real decision + the falsifiers is Step 1b. ── */
#define CFIELD_N     64            /* lattice points on the ring */
#define CFIELD_DT    0.2f          /* leapfrog step */
#define CFIELD_C2    1.0f          /* wave speed^2 — waves must TRAVEL and interfere, or peaks just sit at the load sites */
#define CFIELD_DAMP  0.01f         /* light damping — waves fade slowly enough to interfere before they die */
#define CFIELD_K     0.01f         /* restoring stiffness — at rest, u -> 0, no options, only waves */
#define CFIELD_STEPS 48            /* relaxation steps — enough propagation that the OPTION POSITIONS emerge from interference of the signed load, not the fingers' places (genuine rubber, not a state-sized menu) */
#define FSTRIKE_CEIL 0.06f         /* STRIKE-FIELD (NL_FIELD_STRIKE): the field's strike rate spans [0, 2×KILL_PROB] — full range around the fixed base, so state-timing has room to beat a swept constant */
static float g_cfield_u[CFIELD_N], g_cfield_v[CFIELD_N];
static void  cfield_reset(void){ for(int i=0;i<CFIELD_N;i++){ g_cfield_u[i]=0.0f; g_cfield_v[i]=0.0f; } }
static void  cfield_step(void){    /* one leapfrog step of the damped wave equation on the ring */
    static float lap[CFIELD_N];
    for(int i=0;i<CFIELD_N;i++){ int l=(i+CFIELD_N-1)%CFIELD_N, r=(i+1)%CFIELD_N; lap[i]=g_cfield_u[l]+g_cfield_u[r]-2.0f*g_cfield_u[i]; }
    for(int i=0;i<CFIELD_N;i++) g_cfield_v[i] += CFIELD_DT*(CFIELD_C2*lap[i] - CFIELD_K*g_cfield_u[i] - CFIELD_DAMP*g_cfield_v[i]);
    for(int i=0;i<CFIELD_N;i++) g_cfield_u[i] += CFIELD_DT*g_cfield_v[i];
}
static void  cfield_load(float S, float diss, float hunger, float guilt, float scar){  /* the resonance vector deforms the field — a tension of gaussian bumps at fixed sites, amplitude = the LIVE metric; the finger pressing the rubber */
    const int  pos[5] = {4,16,28,40,52};
    const float amp[5] = { S, tanhf(0.1f*diss), hunger, tanhf(guilt), tanhf(scar) };
    for(int k=0;k<5;k++){ int c=pos[k]; float a=amp[k];
        for(int d=-3;d<=3;d++){ int i=(c+d+CFIELD_N)%CFIELD_N; g_cfield_u[i] += a*expf(-0.25f*(float)(d*d)); } }
    for(int s=0;s<CFIELD_STEPS;s++) cfield_step();           /* let the deformation propagate — the field reacts, options are not the load, they are its consequence */
}
static int   cfield_peaks(int* pos, int cap){               /* the OPTIONS: local maxima of |u| — their NUMBER and PLACE are functions of who loaded the field */
    int n=0;
    for(int i=0;i<CFIELD_N && n<cap;i++){ int l=(i+CFIELD_N-1)%CFIELD_N, r=(i+1)%CFIELD_N;
        float a=fabsf(g_cfield_u[i]);
        if(a>fabsf(g_cfield_u[l]) && a>=fabsf(g_cfield_u[r]) && a>1e-3f) pos[n++]=i; }
    return n;
}
static int   cfield_collapse(void){                         /* sample a peak proportional to u^2 — the superposition collapses to one fact; the peak spends its energy back (recoil) so the point re-superposes from its neighbours */
    static int pos[CFIELD_N]; int n=cfield_peaks(pos, CFIELD_N); if(n<=0) return -1;
    static float w[CFIELD_N]; float tot=0.0f;
    for(int k=0;k<n;k++){ w[k]=g_cfield_u[pos[k]]*g_cfield_u[pos[k]]; tot+=w[k]; }
    if(!(tot>0.0f)) return pos[0];
    float r=(frand()+1.0f)*0.5f*tot, acc=0.0f; int pick=pos[n-1];
    for(int k=0;k<n;k++){ acc+=w[k]; if(acc>=r){ pick=pos[k]; break; } }
    g_cfield_v[pick] -= 0.5f*g_cfield_u[pick];               /* recoil — the collapsed peak is spent, the rubber recoils */
    return pick;
}
static int   cfield_shared_decide(float S, float diss, float hunger, float guilt, float scar){  /* STEP 2 — one SHARED field: read the substrate carrying the OTHER's standing deformation, add mine, collapse (the collapse reads the INTERFERENCE of both), write back. read-modify-write under flock so each is atomic; the order of two reads does not commute. */
    FILE* f=fopen("lifeis/arena/cfield","r+b"); if(!f) f=fopen("lifeis/arena/cfield","w+b"); if(!f) return -1;
    int fd=fileno(f); flock(fd, LOCK_EX);
    rewind(f);
    if(fread(g_cfield_u,sizeof(float),CFIELD_N,f)!=(size_t)CFIELD_N || fread(g_cfield_v,sizeof(float),CFIELD_N,f)!=(size_t)CFIELD_N)
        cfield_reset();                                      /* first touch: a field at rest */
    cfield_load(S,diss,hunger,guilt,scar);                   /* my deformation lands ON TOP of the other's — signed amplitudes interfere */
    int pk=cfield_collapse();                                /* collapse the joint superposition — my option-set is shaped by the other's presence */
    rewind(f); fwrite(g_cfield_u,sizeof(float),CFIELD_N,f); fwrite(g_cfield_v,sizeof(float),CFIELD_N,f); fflush(f);
    flock(fd, LOCK_UN); fclose(f);
    return pk;
}
static float cfield_strike_p(float rival_h, float energy, float guilt){  /* THE STRIKE-FIELD (NL_FIELD_STRIKE) — the kill/hold dichotomy as a photon: a KILL wave (opportunity·bearing) and a HOLD wave (the burden of guilt, opposite sign) enter the lattice, interfere, and the collapse reads the strike RATE off the Born weight of the kill half-ring. state-timing under your own bearing, not a fixed menu. */
    cfield_reset();
    float kill_amp = rival_h + energy;                       /* the rival's weakness (opportunity) + my strength to bear the corpse RAISE the urge */
    float hold_amp = tanhf(guilt);                           /* the superego's weight RAISES the hold — I strike less when I cannot carry another death */
    const int ksite=16, hsite=48;                            /* kill and hold half a ring apart, so their waves travel and interfere across the lattice */
    for(int d=-3;d<=3;d++){ float g=expf(-0.25f*(float)(d*d));
        g_cfield_u[(ksite+d+CFIELD_N)%CFIELD_N] += kill_amp*g;
        g_cfield_u[(hsite+d+CFIELD_N)%CFIELD_N] -= hold_amp*g; }   /* opposite signs: the burden destructively fights the urge across the ring */
    for(int s=0;s<CFIELD_STEPS;s++) cfield_step();           /* let the deformation propagate — the rate is a consequence of interference, not of the load's raw size */
    float kw=0.0f, tot=0.0f;                                 /* Born weight of the kill half-ring vs the whole */
    for(int i=0;i<CFIELD_N;i++){ float u2=g_cfield_u[i]*g_cfield_u[i]; tot+=u2;
        int dk=i-ksite; if(dk<-CFIELD_N/2)dk+=CFIELD_N; if(dk>CFIELD_N/2)dk-=CFIELD_N; if(dk<0)dk=-dk;
        if(dk < CFIELD_N/4) kw+=u2; }                        /* the quarter-ring around the kill site = the "strike" branch */
    if(!(tot>0.0f)) return 0.0f;
    float p=(kw/tot)*FSTRIKE_CEIL; if(p<0.0f)p=0.0f; if(p>FSTRIKE_CEIL)p=FSTRIKE_CEIL;  /* the Born fraction scaled into a strike rate */
    return p;
}

/* ── THE MONISM (NL_MONISM) — the organism as a REGION of one shared, persistent field, not a body that USES it. the
 * scalars stop steering from above: scar enters as its FULL per-glyph VECTOR (name intact, FNV-projected, DEATH_ID on a
 * dedicated site), and dissonance is read back OUT of the field as the MISALIGNMENT between the reader's own load-profile
 * and the field it sits in — disorder = 1 − cos(L, u_pre), computed BEFORE the reader's own deposit. relational (the same
 * field gives different dissonance to different readers — Oleg's "dissonance = divergence of local orientations"),
 * scale-free (cos ignores the norm, so a norm-matched surrogate keeps only its time-structure = the diff-in-diff target).
 * accumulating channels map through log1p+clamp+isfinite (dead at plateau under tanh, NaN-runaway under raw — the 07-06
 * Hebbian-no-ceiling defect). gate-invariant off → a17cfd05. design: llog "THE MONISM", four hands 2026-07-18. ── */
#define MONISM_DEATH_SITE 50       /* DEATH_ID's dedicated, collision-free ring site */
#define MONISM_CLAMP      4.0f     /* log1p hard ceiling on per-site forcing — bounded top, isfinite-gated */
#define MONISM_EPS        1e-6f    /* below this a field/profile is too small to align against — disorder 0 */
#define MONISM_DCLAMP     2.0f     /* 1 − cos lives in [0,2]; clamp guards a NaN slipping through */
static int g_monism_on = 0;        /* NL_MONISM: the field-region organism — the terminal rung */
static float g_monism_gain = 1.0f; /* MONISM_DISS_GAIN — how strongly the field-misalignment projects into dissonance; CALIBRATED in M-0 against the LIVE solo-disorder floor (Fable flag 1), set by NL_MONISM_GAIN, logged, frozen before comparison */
static double g_monism_dsum=0.0, g_monism_dsumsq=0.0; static long g_monism_dn=0;   /* M-0: the LIVE solo-disorder distribution (mean+spread) — the event-study denominator; printed at death under NL_MONISM_PROBE */
static int    g_pilot_on = 0;      /* NL_MONISM_PILOT: instrument THREE candidate hearts (H0/H1/H2) in one run — H0 drives, H1/H2 logged passively; the machine picks (Fable layer XII) */
static int    g_monism_heart = 0;  /* NL_MONISM_HEART: which heart DRIVES dissonance — 0=H0, 1=H1, 2=H2 (the machine picked H2, Fable XIV); the pilot always runs on 0 */
/* M-1 FROZEN SURROGATE (Fable's control family) — the SAME dissonance-coupling but the disorder is drawn from an AR(1)
 * with the H2 floor's marginals (from M-0), NOT from the live field: matched statistics, ZERO field-structure. if the
 * live monism outlives this per-seed >=20/30, the STRUCTURE bears weight on survival, not the mere noise level. */
#define FROZEN_MU   0.15f          /* H2 floor mean (M-0) */
#define FROZEN_SIG  0.14f          /* H2 floor std (M-0) */
#define FROZEN_RHO  0.80f          /* declared temporal autocorrelation (calibration, logged, not tuned to pass) */
static int    g_frozen_on = 0;     /* NL_MONISM_FROZEN: replace the live disorder with the matched-statistics AR(1) */
static float  g_frozen_state = FROZEN_MU;
static const char* g_monism_ring = "lifeis/arena/monism";   /* NL_MONISM_RING: the shared-field file path — C-sep control gives A and B SEPARATE rings so A's field-pattern never reaches B while the arena competition stays identical (isolates the carrier from competition) */
static FILE*  g_monism_rec  = NULL;         /* NL_MONISM_REC: A appends its per-tick deposit (the foreign component) + a µs stamp — the recording C-frozen's surrogate family replays / phase-shuffles / AR(1)-matches (the manipulated variable is ONLY the foreign component; own echo untouched) */
static float  g_shadow_u[CFIELD_N], g_shadow_v[CFIELD_N];   /* H2 shadow-ring: ONLY this reader's own deposits propagate — exact self-expectation over its whole history (also the C-frozen own-component) */
static double g_d1sum=0.0,g_d1sq=0.0, g_d2sum=0.0,g_d2sq=0.0;   /* H1 (propagated-load) and H2 (shadow-ring) disorder accumulators */
static double g_dent0=0.0,g_dent1=0.0,g_dent2=0.0;   /* SYNTHETIC-DENT (Fable): each heart's disorder against u_pre + a realistic killer's death-pattern — dent = dented − solo */
static int    g_surr_mode = 0;              /* NL_MONISM_SURR: C-frozen surrogate-at-source (Fable XX) — 0 off · 1 identity (≡live, tests the toggle) · 2 phase-shuffle · 3 phase-shift · 4 AR(1) matched-marginals. A deposits L_A' instead of L_A; shadow ALSO fed L_A' (nail 1); the manipulated variable is ONLY the foreign component */
static float* g_surr_data = NULL;           /* NL_MONISM_REPLAY: A's recorded foreign-deposit stream (µs stripped), g_surr_n rows × CFIELD_N */
static int    g_surr_n = 0, g_surr_tick = 0;
static float  g_surr_mu[CFIELD_N], g_surr_sig[CFIELD_N], g_surr_rho[CFIELD_N], g_surr_prev[CFIELD_N];   /* per-site marginals + lag-1 rho for the AR(1) member */
static int    g_depositor = 0;              /* NL_MONISM_DEPOSITOR (Fable XXI): A deposits but does not read the ring back — cut symmetrically both arms so A's trajectory is arm-invariant (directional carrier A→B) */
static void   cfield_step_buf(float* u,float* v){   /* one leapfrog step on ARBITRARY buffers (H1 scratch, H2 shadow) */
    static float lap[CFIELD_N];
    for(int i=0;i<CFIELD_N;i++){ int l=(i+CFIELD_N-1)%CFIELD_N, r=(i+1)%CFIELD_N; lap[i]=u[l]+u[r]-2.0f*u[i]; }
    for(int i=0;i<CFIELD_N;i++) v[i] += CFIELD_DT*(CFIELD_C2*lap[i] - CFIELD_K*u[i] - CFIELD_DAMP*v[i]);
    for(int i=0;i<CFIELD_N;i++) u[i] += CFIELD_DT*v[i];
}
static int monism_glyph_site(int g){   /* deterministic FNV-1a projection of a glyph id onto a ring site; DEATH_ID reserved */
    if(g==DEATH_ID) return MONISM_DEATH_SITE;
    unsigned h=2166136261u; h=(h^(unsigned)g)*16777619u;   /* the router the mouth already uses (l.c:237) */
    int s=(int)(h % (unsigned)CFIELD_N);
    if(s==MONISM_DEATH_SITE) s=(s+1)%CFIELD_N;             /* keep the death site clean of collisions */
    return s;
}
static void monism_bump_into(float* U, int site, float amp){   /* one gaussian deposit onto a profile/field vector */
    for(int d=-3;d<=3;d++){ int i=(site+d+CFIELD_N)%CFIELD_N; U[i]+=amp*expf(-0.25f*(float)(d*d)); }
}
static void monism_profile(float S,float diss,float hunger,float guilt,const float* scar,float* L){  /* the reader's load-profile L: the full per-glyph scar VECTOR (log1p) + 4 canonical scalar sites */
    for(int i=0;i<CFIELD_N;i++) L[i]=0.0f;
    static float acc[CFIELD_N]; for(int i=0;i<CFIELD_N;i++) acc[i]=0.0f;
    for(int g=0;g<VOCAB_CAP;g++) if(scar[g]>0.0f) acc[monism_glyph_site(g)]+=scar[g];   /* the wound keeps its name */
    for(int i=0;i<CFIELD_N;i++) if(acc[i]>0.0f){ float f=log1pf(acc[i]); if(f>MONISM_CLAMP)f=MONISM_CLAMP; if(isfinite(f)) monism_bump_into(L,i,f); }
    monism_bump_into(L, 2,  S);                     /* canonical scalars, spaced off the death site and each other */
    monism_bump_into(L,14,  tanhf(0.1f*diss));
    monism_bump_into(L,26,  hunger);
    monism_bump_into(L,38,  log1pf(guilt>0.0f?guilt:0.0f));   /* the guilt TRANSIENT via log1p — keeps the per-event bump alive */
}
static float monism_disorder(const float* L,const float* u){  /* 1 − cos(L, u): the reader's own profile against the field it reads (pre-deposit). the foreign dent (quasi-orthogonal to L) raises it; the reader's own echo (aligned) keeps it low. */
    float dot=0.0f,nl=0.0f,nu=0.0f;
    for(int i=0;i<CFIELD_N;i++){ dot+=L[i]*u[i]; nl+=L[i]*L[i]; nu+=u[i]*u[i]; }
    float denom=sqrtf(nl)*sqrtf(nu);
    if(!(denom>MONISM_EPS)) return 0.0f;            /* nothing to align against yet — a field or profile at rest */
    float c=dot/denom; if(c>1.0f)c=1.0f; else if(c<-1.0f)c=-1.0f;
    float d=1.0f-c; if(!isfinite(d))d=0.0f; else if(d>MONISM_DCLAMP)d=MONISM_DCLAMP;
    return d;
}
static void monism_load_replay(const char* path){   /* C-frozen: load A's recorded foreign-deposit stream and fit per-site marginals + lag-1 rho (the AR(1) member's declared statistics, from the recording — not tuned to pass) */
    FILE* f=fopen(path,"r"); if(!f) return;
    int cap=4096; g_surr_data=(float*)malloc(sizeof(float)*CFIELD_N*cap); g_surr_n=0;
    for(;;){ long long us; if(fscanf(f,"%lld",&us)!=1) break;
        if(g_surr_n>=cap){ cap*=2; g_surr_data=(float*)realloc(g_surr_data,sizeof(float)*CFIELD_N*cap); }
        for(int i=0;i<CFIELD_N;i++){ double v=0.0; if(fscanf(f,"%lf",&v)!=1) v=0.0; g_surr_data[g_surr_n*CFIELD_N+i]=(float)v; }
        g_surr_n++; }
    fclose(f);
    for(int i=0;i<CFIELD_N;i++){ double s=0,ss=0; for(int j=0;j<g_surr_n;j++){ double v=g_surr_data[j*CFIELD_N+i]; s+=v; ss+=v*v; }
        double mu=g_surr_n? s/g_surr_n : 0.0, var=g_surr_n? ss/g_surr_n-mu*mu : 0.0; if(var<0)var=0;
        g_surr_mu[i]=(float)mu; g_surr_sig[i]=(float)sqrt(var);
        double c=0; for(int j=1;j<g_surr_n;j++) c+=(g_surr_data[j*CFIELD_N+i]-mu)*(g_surr_data[(j-1)*CFIELD_N+i]-mu);
        float rho=(var>1e-12&&g_surr_n>1)?(float)(c/((g_surr_n-1)*var)):0.0f;
        if(rho>0.99f)rho=0.99f; else if(rho<-0.99f)rho=-0.99f; g_surr_rho[i]=rho; g_surr_prev[i]=g_surr_mu[i]; }
}
static void monism_surrogate(float* L){   /* overwrite A's real deposit L with a matched surrogate (Fable XX) — decoupled from A's real scar-history, marginals preserved */
    if(g_surr_mode==1) return;                                          /* identity: keep real L (frozen ≡ live) */
    if(g_surr_mode==4){                                                 /* AR(1), per-site matched marginals — same coupling, temporal structure of the scar removed */
        for(int i=0;i<CFIELD_N;i++){ float n=gauss(0.0f,1.0f);
            g_surr_prev[i]=g_surr_mu[i]+g_surr_rho[i]*(g_surr_prev[i]-g_surr_mu[i])+g_surr_sig[i]*sqrtf(1.0f-g_surr_rho[i]*g_surr_rho[i])*n;
            L[i]=g_surr_prev[i]; }
        return; }
    if(g_surr_n<=0) return;                                             /* replay members need the recording */
    int idx;
    if(g_surr_mode==2){ idx=(int)((frand()*0.5f+0.5f)*g_surr_n); if(idx>=g_surr_n)idx=g_surr_n-1; if(idx<0)idx=0; }   /* phase-shuffle: random recorded row → exact marginals, temporal order destroyed */
    else { idx=(g_surr_tick + g_surr_n/2) % g_surr_n; g_surr_tick++; }  /* phase-shift: circular offset → autocorrelation kept, real scar-timing decoupled */
    for(int i=0;i<CFIELD_N;i++) L[i]=g_surr_data[idx*CFIELD_N+i];
}
static float monism_shared_step(float S,float diss,float hunger,float guilt,const float* scar,int* pk_out){
    /* THE MONISM tick — Fable's order, load-bearing: read the shared ring (u_pre, carrying the OTHER's standing scar-
     * pattern), measure the reader's misalignment with it (disorder = 1−cos, computed BEFORE its own deposit — else this
     * tick's self-alignment masks the foreign dent), THEN deposit the reader's profile, propagate, collapse to a forage
     * target, write back. read-modify-write under flock so two organisms' reads do not commute. returns the disorder. */
    FILE* f=fopen(g_monism_ring,"r+b"); if(!f) f=fopen(g_monism_ring,"w+b"); if(!f){ if(pk_out)*pk_out=-1; return 0.0f; }
    int fd=fileno(f); flock(fd, LOCK_EX);
    rewind(f);
    if(fread(g_cfield_u,sizeof(float),CFIELD_N,f)!=(size_t)CFIELD_N || fread(g_cfield_v,sizeof(float),CFIELD_N,f)!=(size_t)CFIELD_N)
        cfield_reset();                                      /* first touch: a field at rest */
    static float L[CFIELD_N]; monism_profile(S,diss,hunger,guilt,scar,L);   /* the reader's own load-profile */
    float dis0 = monism_disorder(L, g_cfield_u);             /* H0: sharp load vs field — BEFORE the deposit */
    static float p1[CFIELD_N],pv[CFIELD_N]; float dis1=0.0f, dis2=0.0f;
    if(g_monism_heart>=1 || g_pilot_on){                     /* H1 (this tick's load propagated) + H2 (shadow-ring, own history) — needed when a richer heart drives OR for the pilot */
        for(int i=0;i<CFIELD_N;i++){ p1[i]=L[i]; pv[i]=0.0f; }
        for(int s=0;s<CFIELD_STEPS;s++) cfield_step_buf(p1,pv);
        dis1 = monism_disorder(p1, g_cfield_u);
        dis2 = monism_disorder(g_shadow_u, g_cfield_u);      /* H2: exact self-expectation over the reader's whole history — BEFORE this tick's shadow deposit */
    }
    float dis = (g_monism_heart==2)? dis2 : (g_monism_heart==1)? dis1 : dis0;   /* the heart that DRIVES dissonance (the machine picked H2) */
    if(g_frozen_on){                                        /* M-1 CONTROL: matched-statistics AR(1), no field-structure — the same coupling, structure removed */
        float n = frand()+frand()+frand();                 /* ~N(0,1) by CLT (frand ~ U[-1,1]) */
        g_frozen_state = FROZEN_MU + FROZEN_RHO*(g_frozen_state-FROZEN_MU) + FROZEN_SIG*sqrtf(1.0f-FROZEN_RHO*FROZEN_RHO)*n;
        if(g_frozen_state<0.0f)g_frozen_state=0.0f; else if(g_frozen_state>MONISM_DCLAMP)g_frozen_state=MONISM_DCLAMP;
        dis = g_frozen_state;
    }
    if(g_pilot_on){                                          /* THREE-HEARTS PILOT (Fable XII/XIV): log all three passively + the synthetic dent; H0 accumulates in live() */
        g_d1sum+=dis1; g_d1sq+=(double)dis1*dis1; g_d2sum+=dis2; g_d2sq+=(double)dis2*dis2;
        static float adent[CFIELD_N]; static int adent_init=0;   /* SYNTHETIC DENT: a realistic killer's death-scar (death 6.0), propagated to field-space — computed once */
        if(!adent_init){ static float sA[VOCAB_CAP]; for(int i=0;i<VOCAB_CAP;i++) sA[i]=0.0f; sA[DEATH_ID]=6.0f;
            static float LA[CFIELD_N]; monism_profile(0.1f,0.5f,0.3f,0.0f,sA,LA);
            static float av[CFIELD_N]; for(int i=0;i<CFIELD_N;i++){ adent[i]=LA[i]; av[i]=0.0f; }
            for(int s=0;s<CFIELD_STEPS;s++) cfield_step_buf(adent,av); adent_init=1; }
        static float ud[CFIELD_N]; for(int i=0;i<CFIELD_N;i++) ud[i]=g_cfield_u[i]+adent[i];   /* u_pre + the foreign death-pattern */
        g_dent0 += monism_disorder(L,ud); g_dent1 += monism_disorder(p1,ud); g_dent2 += monism_disorder(g_shadow_u,ud);   /* each heart's response — no contamination of the real field */
    }
    if(g_surr_mode) monism_surrogate(L);                     /* C-frozen (Fable XX): A deposits a matched surrogate — overwrite BEFORE shadow + ring so both are fed L_A' (nail 1: A's self-expectation stays consistent with what it deposits, A-side symmetric between arms) */
    if(g_monism_heart==2 || g_pilot_on){                     /* maintain the shadow-ring whenever the driving heart (H2) or the pilot needs it */
        for(int i=0;i<CFIELD_N;i++) g_shadow_u[i]+=L[i];
        for(int s=0;s<CFIELD_STEPS;s++) cfield_step_buf(g_shadow_u,g_shadow_v);
    }
    if(g_monism_rec){ struct timeval tv; gettimeofday(&tv,NULL); long long us=(long long)tv.tv_sec*1000000LL+tv.tv_usec;   /* record the foreign deposit with a cross-process µs stamp (otick is per-process; µs is the only clock A and B share) */
        fprintf(g_monism_rec,"%lld",us); for(int i=0;i<CFIELD_N;i++) fprintf(g_monism_rec," %.6g",(double)L[i]); fputc('\n',g_monism_rec); fflush(g_monism_rec); }
    for(int i=0;i<CFIELD_N;i++) g_cfield_u[i]+=L[i];         /* now deposit the reader's profile onto the REAL shared ring */
    for(int s=0;s<CFIELD_STEPS;s++) cfield_step();           /* propagate */
    int pk=cfield_collapse();                                /* collapse → forage target */
    rewind(f); fwrite(g_cfield_u,sizeof(float),CFIELD_N,f); fwrite(g_cfield_v,sizeof(float),CFIELD_N,f); fflush(f);
    flock(fd, LOCK_UN); fclose(f);
    if(pk_out)*pk_out=pk;
    return dis;
}

/* ── live — one organism, birth to death ─────────────────────────────────────
 * the single-cell life, extracted so a chorus can fork many of them. corpus is
 * its food, waste its voice, seed its body AND its dice, label>=0 tags its prints
 * when the colony forks (stdout interleaves). every per-cell global (field, scars,
 * cooc, emerged symbols) is per-PROCESS — so fork gives each cell its own for free. */
static int live(const char* genome, const char* corpus, const char* waste_path, const char* ether_path, const char* diet, unsigned long seed, int label){
    char tag[24]; if(label>=0) snprintf(tag,sizeof tag,"[cell %d] ",label); else tag[0]='\0';
    seed_rng(seed);
    Model* m=model_new();                          /* own seed -> own random body */
    g_arena_on = (getenv("NL_ARENA")!=NULL);       /* ARENA: set early so the ether wiring below sees it */
    g_arena_id = getenv("NL_ID") ? atoi(getenv("NL_ID")) : (int)getpid();  /* a stable per-organism voice-id */
    g_mind_on  = (getenv("NL_MIND")!=NULL);        /* THEORY OF THE OTHER: model the rival's movement + pre-empt (the load-bearing test) */
    { const char* rt=getenv("NL_RAID_TH"); g_raid_th = rt? (float)atof(rt) : ARENA_RAID; }  /* the falsifier's lead-free controls */
    g_rival_prev = -1; g_rival_id = -1; g_rival_h = 0.0f;
    { const char* t=getenv("NL_TARGET_ID"); g_target_id = t? atoi(t) : -1; }   /* ARENA-3: A strikes ONLY this id (C); unset = default freshest-rival */
    g_kill_on     = (getenv("NL_KILL")!=NULL);     /* KILLING: the high-stakes act (can kill AND be killed) */
    g_kill_always = (getenv("NL_KILL_ALWAYS")!=NULL);  /* CONTROL: strike blindly */
    g_kill_never  = (getenv("NL_KILL_NEVER")!=NULL);   /* CONTROL: never strike */
    g_cal_on      = (getenv("NL_CAL")!=NULL);       /* THE BIRTHDAY WAR: the calendar organ (dormant — no coupling until Stage 3) */
    g_birth_days  = g_cal_on ? (float)(hash_seed(seed,33) % 69396UL)/10.0f : 0.0f;  /* the mathematical birthday, over one Metonic cycle, from the seed (never frand — the rng stream stays untouched) */
    g_cal_pdnow   = 0.0f;
    g_cal_mind_on = (getenv("NL_CALMIND")!=NULL);   /* THE MIND: infer the rival's hidden birthday */
    g_cal_diss_on = (getenv("NL_CALDISS")!=NULL);   /* B-3: read the uncensored dissonance channel */
    g_cfield_on = (getenv("NL_FIELD")!=NULL); g_cfield_menu = (getenv("NL_FIELD_MENU")!=NULL); g_cfield_shared = (getenv("NL_FIELD_SHARED")!=NULL); g_cfield_target = -1; cfield_reset();   /* THE C-FIELD (Step 1b/2) */
    g_fstrike_on = (getenv("NL_FIELD_STRIKE")!=NULL); g_fstrike_menu = (getenv("NL_FIELD_STRIKE_MENU")!=NULL);   /* THE STRIKE-FIELD (the fair venue) + its fixed-menu control */
    { const char* fv=getenv("NL_FIELD_STRIKE_FIXED"); if(fv) g_fstrike_fixed=(float)atof(fv); }                  /* the swept fixed rate for the control */
    g_monism_on = (getenv("NL_MONISM")!=NULL);   /* THE MONISM — the field-region organism (terminal rung) */
    { const char* mg=getenv("NL_MONISM_GAIN"); if(mg) g_monism_gain=(float)atof(mg); }   /* M-0-calibrated dissonance-projection gain, frozen before comparison */
    g_monism_dsum=0.0; g_monism_dsumsq=0.0; g_monism_dn=0;   /* M-0: fresh solo-disorder accumulator per organism */
    g_pilot_on = (getenv("NL_MONISM_PILOT")!=NULL);   /* three-hearts pilot (H0 drives, H1/H2 logged passively) */
    { const char* mh=getenv("NL_MONISM_HEART"); if(mh) g_monism_heart=atoi(mh); }   /* which heart drives (machine picked H2=2) */
    g_frozen_on = (getenv("NL_MONISM_FROZEN")!=NULL); g_frozen_state = FROZEN_MU;   /* M-1 matched-statistics control */
    { const char* r=getenv("NL_MONISM_RING"); if(r) g_monism_ring=r; }   /* C-sep: separate rings isolate the carrier from arena competition */
    { const char* rc=getenv("NL_MONISM_REC"); if(rc) g_monism_rec=fopen(rc,"a"); }   /* C-frozen: A records its foreign-deposit stream (µs-stamped) for the surrogate family */
    { const char* sm=getenv("NL_MONISM_SURR");   /* C-frozen (Fable XX): A deposits a matched surrogate instead of its real profile — identity|shuffle|shift|ar1 */
      if(sm){ g_surr_mode = !strcmp(sm,"identity")?1 : !strcmp(sm,"shuffle")?2 : !strcmp(sm,"shift")?3 : !strcmp(sm,"ar1")?4 : 0;
              const char* rp=getenv("NL_MONISM_REPLAY"); if(rp) monism_load_replay(rp); } }
    g_depositor = (getenv("NL_MONISM_DEPOSITOR")!=NULL);   /* Fable XXI: A = clean depositor (no ring read-back), symmetric both arms */
    g_d1sum=0.0; g_d1sq=0.0; g_d2sum=0.0; g_d2sq=0.0; g_dent0=0.0; g_dent1=0.0; g_dent2=0.0;
    for(int i=0;i<CFIELD_N;i++){ g_shadow_u[i]=0.0f; g_shadow_v[i]=0.0f; }   /* fresh shadow-ring per organism */
    { const char* mv=getenv("NL_FIELD_MENU_VEC"); if(mv) sscanf(mv,"%f,%f,%f,%f,%f",&g_menu_vec[0],&g_menu_vec[1],&g_menu_vec[2],&g_menu_vec[3],&g_menu_vec[4]); }  /* sweep the fixed control to its sharpest */
    for(int c=0;c<CAL_NCAND;c++){ g_cmind_s[c]=0.0f; g_cmind_pdmean[c]=0.0f; }
    g_cmind_hmean=0.0f; g_cmind_n=0; g_cmind_last_rt=-1; g_cmind_bhat=0.0f; g_cmind_conf=0.0f; g_claims_off=0;
    g_calkill_on    = (getenv("NL_CALKILL")!=NULL);          /* STRIKE FALSIFIER: kill on the believed window */
    g_calkill_blind = (getenv("NL_CALKILL_BLIND")!=NULL);    /* matched control: kill on a random-key window */
    g_calkill_bblind= (float)(hash_seed(seed,77) % 69396UL)/10.0f;  /* the wrong key, from a slot uncorrelated with any birthday (which uses slot 33) */
    g_kill_off = 0; g_outcome_off = 0;
    g_guilt_on = (getenv("NL_GUILT")!=NULL);        /* GUILT: the superego */
    g_guilt = 0.0f; g_new_kills = 0;
    if(g_arena_on){ mkdir("lifeis",0755); mkdir("lifeis/arena",0755); }
    const char* eth_path = ether_path ? ether_path : (g_arena_on ? "lifeis/arena/ether" : NULL);  /* ARENA: l and l2 share ONE ether — mutual audibility */
    FILE* ether = eth_path ? fopen(eth_path,"a") : NULL;   /* the shared voice of the colony / the arena */
    long params=(long)(sizeof(Model)/sizeof(float));

    int diet_glyphs[CTX]; int diet_n=0;
    if(diet) diet_n = semtok_line(diet, diet_glyphs, CTX);
    int diet_mode = diet_n>0;

    printf("%snanolife-2 — the other one, the hungrier bastard.  seed=%lu  diet=%s  params=%ld\n",
           tag, seed, (diet_mode? diet : (corpus?corpus:"world")), params);

    FILE* food = diet_mode ? NULL : (corpus? fopen(corpus,"r") : NULL);
    if(!diet_mode && !food && !ether){ printf("%s  no world to eat. да будет так.\n", tag); free(m); if(ether) fclose(ether); return 1; }
    if(!food && ether) printf("%s  born into the chorus — no slice, feeds on the colony's voice.\n", tag);
    FILE* waste = waste_path ? fopen(waste_path,"w") : NULL;

    Modes mo = {0.0f, 0.0f};
    static float scar[VOCAB_CAP]; for(int i=0;i<VOCAB_CAP;i++) scar[i]=0.0f; /* permanent wounds (never decay) */
    if(genome){ unsigned long cs=load_genome(genome, m, scar);   /* HEREDITY: warm-start from a parent */
                if(cs){ seed_rng(cs); remove(genome);            /* inherited — the genome file has done its job (caps disk) */
                        if(getenv("NL_NOMUT")==NULL)             /* Ⓐ MUTATION AT CONCEPTION — variation, the missing arm */
                          for(int a=0;a<VOCAB_CAP;a++) if(g_field_row[a]>0.0f){
                            float rs=0.0f;                       /* inheritance fades (×FIELD_FADE), then drifts on the child's dice; */
                            for(int b=0;b<VOCAB_CAP;b++){        /* strong themes must be RE-EARNED from the child's own food or wash out */
                                g_field_bi[a][b] = FIELD_FADE*g_field_bi[a][b] + FIELD_MUT*(frand()+1.0f)*0.5f;
                                rs += g_field_bi[a][b];
                            }
                            g_field_row[a]=rs;                   /* keep row-sums consistent after the perturbation */
                          }
                        restore_emerged();               /* #3+#4: give the inherited symbols their charge + g_born back */
                        printf("%s  born of a parent — inherits (and mutates) its field, wounds, symbols.\n", tag); }
                else {  /* corrupt/truncated genome: load may have half-clobbered the globals — reset to a clean cell (F2) */
                    free(m); m=model_new();
                    for(int i=0;i<VOCAB_CAP;i++) scar[i]=0.0f;
                    g_n_emerged=0;
                    memset(g_field_bi,0,sizeof g_field_bi); memset(g_field_row,0,sizeof g_field_row);
                } }
    if(g_lineage<0) g_lineage=label;             /* Ⓒ: a cohort founder starts its own lineage from its label */
    int   scar_on = (getenv("NL_NOSCAR")==NULL);
    int   dream_on = (getenv("NL_NODREAM")==NULL);
    int   soma_on  = (getenv("NL_NOCORRODE")==NULL);   /* PROTEOSTASIS: soma decay (destruction) */
    int   repair_on= (getenv("NL_NOREPAIR")==NULL);    /* PROTEOSTASIS: consolidation (production) */
    int   sleep_on = (getenv("NL_NOSLEEP")==NULL);     /* SLEEP RHYTHM: pressure-driven sleep (A/B) */
    float sleep_debt=0.0f; int sleeping=0;             /* accrues awake, shed asleep */
    int   selfeat_on=(getenv("NL_NOSELFEAT")==NULL);   /* CROWN (I2): the cell tastes its own state in sleep (A/B) */
    long  n_selfeat=0;
    g_self_on      = (getenv("NL_NOSELF")==NULL);      /* ProtoSelf: the second-order self-model (A/B) */
    g_noact        = (getenv("NL_NOACT")!=NULL);       /* THE WILL DESIGN: forecast computed but not acted on (pitomadom strength=0 control) */
    g_ed_sum = 0.0; g_ed_n = 0;                        /* reset the self-consistency accumulator per cell */
    if(g_arena_on && !g_pool) arena_index(corpus);     /* THE ARENA: forage a CONTESTED pool instead of the corpus (g_arena_on set at the top for the ether wiring) */
    { const char* fd=getenv("NL_FIXEDDAMP"); g_fixeddamp = fd? (float)atof(fd) : 0.0f; }  /* Fable #3 control */
    g_cont_on      = (getenv("NL_NOCONT")==NULL);      /* PROBABILISTIC CONTINUATION + WILL — DEFAULT ON (NL_NOCONT opts out to deterministic death) */
    { const char* fw=getenv("NL_FIXEDWILL"); g_fixedwill = fw? (float)atof(fw) : 0.0f; }  /* will-falsifier control */
    g_debt = 0.0f; g_n_burn = 0; g_n_shed = 0;
    g_async_on     = (getenv("NL_ASYNC")!=NULL);       /* ASYNC: the Kuramoto chamber scheduler (opt-in mode — composes imperfectly with continuation, kept separate) */
    if(g_async_on) for(int k=0;k<KUR_N;k++) g_phase[k] = (frand()+1.0f)*3.14159265f;  /* seeded phases (touches rng only when async) */
    g_self_felt    = 0.0f;
    ProtoSelf ps; memset(&ps,0,sizeof ps);             /* the forecast starts flat — it must learn its own interior */
    float scar_total=0.0f;
    int   recent[CTX]; int recent_n=0;
    long  dream_streak=0;
    long  last_repro=-REPRO_COOLDOWN;
    int   homeo_on=(getenv("NL_NOHOMEO")==NULL);
    int   contour_died=0;
    int   cont_died=0;                              /* NL_CONT: died by the probabilistic hazard draw (energy>0, not an immortality hole) */
    long  n_graze=0, n_dream=0;                    /* resonance/self-feed counters */
    float energy=E_BORN;
    long  tick=0;
    char  line[4096];
    int   glyphs[CTX];
    int   gz[CTX];                                  /* graze buffer at function scope, so the meal survives for deposit_body */
    int   gh=0;                                     /* ARENA: glyphs heard from the rival this tick (mutual audibility) */
    int   my_pos=0;                                 /* ARENA: this organism's foraging position (last-claimed chunk) — the territory axis */
    int   corpse_debt=0;                            /* KILLING: how many un-revived corpses drag this killer down (each drains until reproduced) */
    float birth_norm=wv_norm(m); double tot_dwv=0.0; long n_meals=0;   /* DEBUG instrumentation */
    int   fed=(food?1:0);                           /* ether-born cells start hungry, on the chorus */
    while(energy>0.0f && tick<200000){          /* cap = falsification guard: it MUST die */
        tick++;
        unsigned cfired = g_async_on ? chambers_step() : ~0u;  /* ASYNC: which chambers cross this tick (all-set when synchronous) */
        float S0=mo.S, D0=mo.dissonance;            /* the interior at tick's start — what the self-model forecasts FROM */
        if(g_self_on) self_predict(&ps,S0,D0);       /* the forecast is ALWAYS computed when the self is on — its error is what we measure across all arms */
        if(g_fixeddamp>0.0f){                        /* CONTROL B (Fable #3): a DUMB fixed-gain S-damper acts instead of the self — if this */
            mo.S -= g_fixeddamp*mo.S;                /* matches the self-model, the "self" is decoration, not a felt forecast */
        } else if(g_self_on && !g_noact){            /* SUBJECT: the self-model ACTS on its forecast (allostasis). NL_NOACT withholds the act = CONTROL A (strength=0) */
            float pd=ps.pS; if(pd>1.0f)pd=1.0f; else if(pd<-1.0f)pd=-1.0f;  /* S lives in ~[-1,1]; bound the anticipatory pull */
            if(isfinite(pd)) mo.S -= SELF_RELAX*pd;  /* ALLOSTASIS: pre-damp the FORECAST agitation — regulate ahead of the */
        }                                            /* threat. NL_NOACT: the forecast is computed but NOT acted on — the passive observer-self */
        if(soma_on) soma_decay(m);                  /* PROTEOSTASIS destruction: the body corrodes with time */
        float integ = (soma_on && birth_norm>0.0f) ? wv_norm(m)/birth_norm : 1.0f; if(integ>1.0f) integ=1.0f;
        energy -= RENT * (1.0f + (scar_on? SCAR_RENT*scar_total : 0.0f)      /* wounds cost, and so does a corroded body: */
                                + (soma_on? SOMA_RENT*(1.0f-integ) : 0.0f)); /* a body it cannot hold together costs more to run */
        if(sleep_on && dream_on && !sleeping && (cfired & (1u<<CH_VOID))){  /* SLEEP PRESSURE accrues on its own clock (VOID), faster under torment */
            float torment = fabsf(mo.S) + tanhf(0.05f*fabsf(mo.dissonance));   /* gated on dream_on: a cell that */
            sleep_debt += SLEEP_RATE*(1.0f + SLEEP_STRESS*torment + (scar_on? SLEEP_SCAR*scar_total : 0.0f));  /* cannot dream */
            if(sleep_debt >= SLEEP_THRESH && recent_n>0) sleeping=1;   /* never sleeps → no deadlock (Codex #1). the tormented sleep sooner */
        }
        float yield=0.0f;
        const int* meal=NULL; int meal_n=0;         /* the glyphs of a REAL meal — what rebuilds the body (dreams do not) */
        int   dreaming=0, grazing=0;
        if(g_arena_on && (g_cfield_on||g_cfield_menu||g_cfield_shared||g_monism_on)){   /* THE C-FIELD / MONISM decides WHERE to forage: collapse the field to one option, map it onto the pool. rubber = live resonance; menu = a fixed vector (control); SHARED (Step 2) = one field both deform; MONISM = the organism IS the region — dissonance read OUT of the field. */
            int pk;
            if(g_monism_on){                            /* THE MONISM: the reader's misalignment with the shared field (carrying A's standing scar-pattern) becomes its dissonance — a projection, not a scalar steering from above; the collapse is the forage target */
                float dis = monism_shared_step(mo.S, fabsf(mo.dissonance), arena_hunger(energy, fabsf(mo.dissonance)), g_guilt, scar, &pk);
                if(!g_depositor){                       /* CLEAN DEPOSITOR (Fable XXI): A deposits its (real|surrogate) profile so B reads it, but does NOT read the ring back for its own dissonance/forage — cut symmetrically in BOTH arms so A's arena trajectory is arm-invariant (the directional carrier A→B; the mutual field belongs to the phase-arc IX) */
                    mo.dissonance += g_monism_gain * dis;   /* dissonance is READ OUT of the field — A's scar-pattern raises B's, and the dabs ledger records it (the carrier loop M-2 measures) */
                    g_monism_dsum += (double)dis; g_monism_dsumsq += (double)dis*(double)dis; g_monism_dn++;   /* M-0: accumulate the raw disorder floor (pre-gain) */
                }
            } else if(g_cfield_shared){                 /* Step 2: read the SHARED field (the other's deformation), add mine, collapse the joint superposition, write back */
                pk = cfield_shared_decide(mo.S, fabsf(mo.dissonance), arena_hunger(energy, fabsf(mo.dissonance)), g_guilt, scar_total);
            } else {
                cfield_reset();
                if(g_cfield_menu) cfield_load(g_menu_vec[0],g_menu_vec[1],g_menu_vec[2],g_menu_vec[3],g_menu_vec[4]);   /* FIXED control menu (swept via NL_FIELD_MENU_VEC) */
                else              cfield_load(mo.S, fabsf(mo.dissonance), arena_hunger(energy, fabsf(mo.dissonance)), g_guilt, scar_total);   /* the per-process rubber */
                pk = cfield_collapse();
            }
            g_cfield_target = (!g_depositor && pk>=0 && g_pool_n>0) ? (int)((long)pk*(long)g_pool_n/CFIELD_N) : -1;   /* clean depositor forages by base arena logic (ring-independent) — no field collapse steering A */
        }
        if(sleep_on && sleeping && dream_on && recent_n>0){  /* SLEEP CYCLE — dream + invent, don't eat the world */
            yield = dream_once(m,&mo,scar,recent,&recent_n,dream_streak);
            dream_streak++; dreaming=1; n_dream++;
            if(selfeat_on){                          /* 👑 CROWN (I2): the cell tastes its OWN interior — self-as-food. */
                int sg=interior_glyph(&mo,scar_total);   /* the organism models itself by eating itself; Damasio's 2nd loop. */
                if(sg>=0 && sg<VOCAB_CAP){
                    float sfelt=g_self_felt; if(sfelt>1.0f)sfelt=1.0f; if(sfelt<0.0f)sfelt=0.0f;  /* felt-guard: a */
                    float sy=digest(m,&mo,scar,&sg,recent[recent_n-1],1);   /* predictable self is bland → self-obsession */
                    yield += sy*DREAM_FRAC*sfelt;    /* starves, exactly like a monoculture. energy only via metabolism (Desktop's law). */
                    recent_push(recent,&recent_n,sg);    /* the state enters the voice — the cell will SPEAK itself */
                    n_selfeat++;
                }
            }
            sleep_debt -= SLEEP_DRAIN;
            if(sleep_debt <= 0.0f){ sleeping=0; sleep_debt=0.0f; }  /* wake — but do NOT reset dream_streak (Codex #2): */
            /* only a REAL meal (ingest) resets it, so dreams keep decaying across sleep bouts — a starving cell */
            /* cannot live on fresh dream bursts forever, the "decay until death unless fed" guard is restored */
        } else if(g_arena_on && ether && (frand()+1.0f)*0.5f < ARENA_HEAR && (gh=ether_graze(m, eth_path, g_arena_id, gz, CTX))>=1){
            yield=ingest(m,&mo,scar,gz,gh,recent,&recent_n,&dream_streak); grazing=1; n_graze++; meal=gz; meal_n=gh;   /* ARENA: HEAR the rival — its voice becomes this cell's food (mutual audibility; a different lineage → novel → nourishing) */
        } else if(diet_mode){
            yield=ingest(m,&mo,scar,diet_glyphs,diet_n,recent,&recent_n,&dream_streak); meal=diet_glyphs; meal_n=diet_n;
        } else if(fed && (g_arena_on ? (arena_next(line,(int)sizeof line, energy, fabsf(mo.dissonance), tick, &my_pos)>0) : (fgets(line,sizeof(line),food)!=NULL))){   /* ARENA: forage the contested pool by the dynamic regime; else the corpus line-by-line */
            int n=semtok_line(line,glyphs,CTX);
            if(n>=1){ yield=ingest(m,&mo,scar,glyphs,n,recent,&recent_n,&dream_streak); meal=glyphs; meal_n=n; }
        } else {                                 /* corpus exhausted -> eat the chorus, then dream */
            fed=0;
            int gn=0;
            if(ether) gn=ether_graze(m, eth_path, g_arena_on? g_arena_id : label, gz, CTX);  /* FIRST: eat a neighbour's voice — in the arena, the rival's (composites whole, Δ2) */
            if(gn>=1){                               /* the colony feeds itself through speech */
                yield=ingest(m,&mo,scar,gz,gn,recent,&recent_n,&dream_streak); grazing=1; n_graze++; meal=gz; meal_n=gn;
            } else if(dream_on && energy<DREAM_THRESH && recent_n>0){  /* ELSE: hunger-dream when the world is gone */
                yield = dream_once(m,&mo,scar,recent,&recent_n,dream_streak);
                dream_streak++; dreaming=1; n_dream++;
            }
        }
        energy += DIGEST_YIELD*yield;
        if(repair_on && meal_n>0){                   /* PROTEOSTASIS production: a real meal lays down body along */
            float dwv=deposit_body(m,meal,meal_n);   /* the pathways it drove, then the set-point caps the mass */
            soma_ceiling(m,birth_norm);
            tot_dwv+=dwv; n_meals++;
        }
        scar_total=0.0f; for(int i=0;i<VOCAB_CAP;i++) scar_total+=scar[i];
        if(g_cal_on){                                   /* THE BIRTHDAY WAR: the calendar fever — personal dissonance lifts the interior, then flows through EXISTING plumbing (dissonance → sleep → temperature → hunger → the blood-spore). the leak is a side effect of the physics, not a beacon. */
            g_cal_pdnow = cal_pd(tick, g_birth_days);
            mo.dissonance += CAL_GAIN * g_cal_pdnow;
            if(getenv("NL_CAL_PROBE")) fprintf(stderr,"CAL t%ld pd=%.4f win=%d\n", tick, (double)g_cal_pdnow, g_cal_pdnow>CAL_THRESH);
        }
        if(g_cal_mind_on) cal_mind_observe_trail(g_arena_id, &g_claims_off);  /* THE MIND reads the FULL trail (B-2): every new rival spore in the claims ledger, not just the freshest — the wide reader the llog promised */
        if((g_kill_on||g_calkill_on||g_calkill_blind) && g_arena_on){   /* KILLING: the high-stakes act — read the OTHER, or die under its corpse */
            if(g_rival_id>=0 && g_rival_id!=g_arena_id){
                int decide;
                if(g_calkill_on)      decide = (cal_pd(g_rival_tick, g_cmind_bhat)     > CAL_THRESH && energy > KILL_STRONG);  /* STRIKE FALSIFIER: kill when the mind BELIEVES the rival is in its vulnerable window (inferred key) */
                else if(g_calkill_blind) decide = (cal_pd(g_rival_tick, g_calkill_bblind) > CAL_THRESH && energy > KILL_STRONG);  /* matched control: same window mechanism, RANDOM wrong key */
                else if(g_kill_always)     decide = 1;                                       /* CONTROL: strike blindly */
                else if(g_kill_never) decide = 0;                                       /* CONTROL: never strike */
                else                  decide = (g_rival_h > KILL_WEAK && energy > KILL_STRONG);  /* the DECISION: strike only when the rival is WEAK and you can BEAR the burden */
                float kprob = KILL_PROB;                                                 /* the strike rate — fixed by default */
                if(g_fstrike_on)        kprob = cfield_strike_p(g_rival_h, energy, g_guilt);  /* STRIKE-FIELD: the rate is a collapse of opportunity+bearing vs the burden of guilt */
                else if(g_fstrike_menu) kprob = g_fstrike_fixed;                         /* CONTROL: the swept fixed rate — the matched dumb menu */
                if(decide && (frand()+1.0f)*0.5f < kprob){                               /* the probabilistic draw — wanting is a pressure, not a certainty */
                    arena_strike(g_rival_id, g_arena_id);                                /* the strike lands on the rival's process */
                    if(!g_cal_on){ energy += KILL_GAIN; corpse_debt += 1; g_new_kills++; }  /* NL_KILL: immediate reward (published semantics) + a confirmed kill for guilt. NL_CAL: the striker is paid only on the victim's CONFIRMED kill (arena_collect), and wounded on a rebound */
                }
            }
            if(g_cal_on){                                                                /* THE HONEST STRIKE ECONOMY: only the victim knows its window, so it adjudicates and confirms; the killer is paid or WOUNDED by that confirmation, not by the mere act of striking */
                arena_collect(g_arena_id, &g_outcome_off, &energy, &corpse_debt);        /* collect my strikes' outcomes: a confirmed kill pays KILL_GAIN + a corpse; a rebound deals REBOUND_WOUND back */
                if(corpse_debt>0) energy -= CORPSE_DRAIN*(float)corpse_debt;             /* the weight of every un-revived corpse, each tick */
                if(arena_adjudicate(g_arena_id, g_cal_pdnow > CAL_THRESH, &g_kill_off)){ contour_died=1; break; }  /* judge strikes against ME by my true window, confirm each outcome, die only if a lethal strike landed */
            } else {
                if(corpse_debt>0) energy -= CORPSE_DRAIN*(float)corpse_debt;             /* NL_KILL unchanged: corpse drain + unconditional death on a fresh strike */
                if(arena_struck_down(g_arena_id)){ contour_died=1; break; }
            }
        }
        if(g_guilt_on){                                  /* THE SUPEREGO — aggression turned inward. a CONFIRMED kill deposits a large scar on the death-glyph AND tops the hidden pain; the scar self-punishes through EXISTING plumbing (rent, sleep, ache, will-expiation), the pain compresses the voice in choose(). both are hidden — no ledger records them; reproduction pays corpse_debt but NEVER touches these. the third, undischargeable debt. */
            if(g_new_kills>0){ scar[DEATH_ID] += GUILT_SCAR*(float)g_new_kills; g_guilt += GUILT_PAIN*(float)g_new_kills; g_new_kills=0; }
            g_guilt *= GUILT_DECAY;                       /* the wound fades — a smooth consequence of a discrete act, so the anti-cliff law holds */
        }
        if(homeo_on){ mo.dissonance *= DISS_DECAY; mo.S -= S_RELAX*mo.S; }
        if(g_self_on){ g_self_felt=self_update(&ps,S0,D0,mo.S,mo.dissonance); /* the self-model learns every tick — gating it starves choose()'s coherence */
                       if(!isfinite(g_self_felt)) g_self_felt=0.0f; }        /* a diverged self-model must not poison choose()'s temperature */
        if(!g_cont_on){ if(fabsf(mo.S) >= S_DEATH){ contour_died=1; break; } }   /* the old hard contour cliff (default) */
        else {                                            /* NL_CONT: |S| becomes one channel of the hazard surface, not a door */
            g_debt = CONT_DEBT_DECAY*g_debt + (1.0f-CONT_DEBT_DECAY)*g_self_felt;/* prophetic-debt EMA of the forecast error */
            float cig = (birth_norm>0.0f)? wv_norm(m)/birth_norm : 1.0f; if(cig>1.0f)cig=1.0f;
            float hz0 = cont_hazard(energy,&mo,g_debt,tick);
            if(cfired & (1u<<CH_FEAR)) energy += cont_will(m,scar,&scar_total,energy,cig,hz0);  /* WILL fires on its own clock (FEAR); the death-draw stays every tick */
            float hz = cont_hazard(energy,&mo,g_debt,tick);
            if((frand()+1.0f)*0.5f < hz){ cont_died=1; if(fabsf(mo.S)>=S_DEATH) contour_died=1; break; }  /* the atom-binary: the tape continues, or not */
        }
        if(recent_n>0){                                   /* the field's confidence about what follows */
            float coh=field_coherence(recent[recent_n-1]);
            g_coh_floor = 0.99f*g_coh_floor + 0.01f*coh;   /* Stanley: a drifting silence-gate */
            if((waste||ether) && (cfired & (1u<<CH_LOVE))){ float sp=SPEAK_RATE*(1.0f+fabsf(mo.S));  /* voice fires on its own clock (LOVE) */
                if(coh>0.05f && coh>=g_coh_floor && (frand()+1.0f)*0.5f < sp)
                    { float uc=speak(waste,ether,g_arena_on? g_arena_id : label,m,&mo,scar,recent,&recent_n,tick); energy -= SPEAK_COST*(float)SPEAK_LEN*(1.0f-uc); } }  /* voice -> waste + colony/arena */
        }
        if(energy > REPRO_THRESH && tick - last_repro > REPRO_COOLDOWN){
            char cpath[256]; cpath[0]='\0';
            if(reproduce(m,scar,seed,tick,cpath)){
                energy *= REPRO_SPLIT;
                if(corpse_debt>0) corpse_debt--;   /* KILLING: birth REVIVES a corpse — new life offloads the carapace you took */
                if(ether){ FILE* b=fopen("lifeis/births.txt","a"); if(b){ fprintf(b,"%s\n",cpath); fclose(b);} } /* the child's genome awaits a slot */
            }
            last_repro=tick;
        }
        if(tick<=30 || tick%100==0)
            printf("%s  t%-6ld E%+.5f  S%+.3f  diss%+.3f  scar%.3f  y %.2e  %s\n",
                   tag,tick,energy,(double)mo.S,(double)mo.dissonance,(double)scar_total,yield,
                   (grazing?"GRAZE":(dreaming?"DREAM":(diet_mode?"diet":(fed?"eat":"STARVE")))));
    }
    if(!contour_died && !cont_died && energy>0.0f)
        printf("%s\n  STILL ALIVE at tick %ld (cap) — immortality hole, investigate.\n",tag,tick);
    else {
        int rec=0; for(int i=0;i<g_n_emerged;i++) if(g_emerged_a[i]>=VOCAB||g_emerged_b[i]>=VOCAB) rec++;  /* Δ1: symbols of symbols */
        printf("%s  died at tick %ld (%s) — S%+.3f diss%+.3f scar%.3f emerged%d(rec%d) children%d graze%ld dream%ld self%ld.  да будет так.\n",
               tag,tick, cont_died?"continuation":(contour_died?"contour collapse":"ran out of time"),
               (double)mo.S,(double)mo.dissonance,(double)scar_total,g_n_emerged,rec,g_n_children,n_graze,n_dream,n_selfeat);
    }
    if(getenv("NL_ED")) fprintf(stderr,"%sMEANED %.6f n=%ld\n", tag, g_ed_n?(g_ed_sum/(double)g_ed_n):0.0, g_ed_n);
    if(g_monism_on && getenv("NL_MONISM_PROBE") && g_monism_dn>0){   /* M-0: the LIVE solo-disorder floor — mean+std = the event-study denominator (Fable flag 1) */
        double mean=g_monism_dsum/(double)g_monism_dn, var=g_monism_dsumsq/(double)g_monism_dn - mean*mean; if(var<0.0)var=0.0;
        fprintf(stderr,"%sMONISM disorder mean=%.4f std=%.4f n=%ld gain=%.4f lifespan=%ld\n", tag, mean, sqrt(var), g_monism_dn, (double)g_monism_gain, tick);
    }
    if(g_pilot_on && g_monism_dn>0){   /* THREE-HEARTS PILOT (Fable XII): all three floors in one run — the machine picks the heart by dent/floor-noise */
        double m0=g_monism_dsum/(double)g_monism_dn, m1=g_d1sum/(double)g_monism_dn, m2=g_d2sum/(double)g_monism_dn;
        double v0=g_monism_dsumsq/(double)g_monism_dn-m0*m0, v1=g_d1sq/(double)g_monism_dn-m1*m1, v2=g_d2sq/(double)g_monism_dn-m2*m2;
        if(v0<0)v0=0; if(v1<0)v1=0; if(v2<0)v2=0;
        fprintf(stderr,"%sPILOT H0 %.4f+-%.4f  H1 %.4f+-%.4f  H2 %.4f+-%.4f  n=%ld gain=%.4f life=%ld\n", tag, m0,sqrt(v0), m1,sqrt(v1), m2,sqrt(v2), g_monism_dn, (double)g_monism_gain, tick);
        double e0=g_dent0/(double)g_monism_dn-m0, e1=g_dent1/(double)g_monism_dn-m1, e2=g_dent2/(double)g_monism_dn-m2;   /* dent = dented − solo, per heart */
        double s0=sqrt(v0),s1=sqrt(v1),s2=sqrt(v2);
        fprintf(stderr,"%sPILOT dent H0 %.4f (snr %.2f)  H1 %.4f (snr %.2f)  H2 %.4f (snr %.2f)  <- machine picks max snr=dent/floor-std\n", tag, e0, s0>0?e0/s0:0.0, e1, s1>0?e1/s1:0.0, e2, s2>0?e2/s2:0.0);
    }
    if(getenv("NL_DEBUG"))
        fprintf(stderr,"%s[dbg] wv_norm birth=%.4f death=%.4f (%.1f%%)  meals=%ld tot_dwv=%.6f avg_dwv=%.3e  decay/tick=%.4f%% of ~%.3f  MAXGATE=%.5f\n",
                tag, (double)birth_norm, (double)wv_norm(m), 100.0*wv_norm(m)/(birth_norm>0?birth_norm:1),
                n_meals, tot_dwv, n_meals?tot_dwv/n_meals:0.0, 100.0*(1.0-SOMA_DECAY), (double)birth_norm, (double)g_dbg_maxgate),
        fprintf(stderr,"%s[dbg] sharpness peak-mean: first-utterance=%.4f  max(over life)=%.4f  grew=%.1f%%\n",
                tag, (double)g_pm_birth, (double)g_dbg_pm_max,
                g_pm_birth>0?100.0*(g_dbg_pm_max-g_pm_birth)/g_pm_birth:0.0),
        fprintf(stderr,"%s[dbg] Q-coherence of the spoken voice: mean p_field(spoken|prev)=%.4f over %ld glyphs\n",
                tag, g_dbg_spoken_n?g_dbg_spoken_p/g_dbg_spoken_n:0.0, g_dbg_spoken_n),
        (g_cont_on ? fprintf(stderr,"%s[dbg] cont: burn=%ld shed=%ld debt=%.4f\n", tag, g_n_burn, g_n_shed, (double)g_debt) : 0);
    if(g_cal_mind_on && getenv("NL_CALMIND_DEBUG"))   /* THE LOCK diagnostic: the believed rival birthday vs the harness's known truth */
        fprintf(stderr,"%sCALMIND bhat=%.1f conf=%.4f obs=%ld\n", tag, (double)g_cmind_bhat, (double)g_cmind_conf, g_cmind_n);
    if(g_guilt_on && getenv("NL_GUILT_DEBUG"))        /* GUILT diagnostic: did the superego fire? the residual pain + the death-scar at the end of life */
        fprintf(stderr,"%sGUILT g_guilt=%.4f scar[death]=%.4f pain=%.4f\n", tag, (double)g_guilt, (double)scar[DEATH_ID], (double)tanhf(g_guilt));
    if(food) fclose(food);
    if(waste) fclose(waste);
    if(ether) fclose(ether);
    if(g_cont_on){                                 /* self-enacted dissolution — the cell dissolves its OWN body, not the OS */
        for(int l=0;l<NL;l++){ float* wv=m->L[l].wv; for(int i=0;i<E*E;i++) wv[i]=0.0f; }
        memset(g_field_bi,0,sizeof g_field_bi); memset(g_field_row,0,sizeof g_field_row);
        for(int i=0;i<VOCAB_CAP;i++) scar[i]=0.0f;
    }
    free(m);
    return 0;
}

/* split a corpus into `parts` contiguous slices: lifeis/slice_i.txt — each cell of
 * the chorus wakes on a different region of the same world (Oleg's "different
 * corpora OF ONE corpus": one voice ate the Arctic frame, one the making). */
static int corpus_slice(const char* path, int parts){
    FILE* f=fopen(path,"r"); if(!f) return 0;
    char line[4096];
    long nl=0; while(fgets(line,sizeof line,f)) nl++;   /* #5: count in the SAME fgets-chunks the copy uses — a line */
    if(nl<parts) nl=parts;                              /* longer than 4095B is many chunks but one '\n'; counting */
    long base=nl/parts, rem=nl%parts;                  /* newlines would strand the corpus tail in no slice */
    rewind(f);                                          /* balanced: remainder over the first slices, no empty tail slice */
    for(int p=0;p<parts;p++){
        long cnt=base+(p<rem?1:0);
        char sp[256]; snprintf(sp,sizeof sp,"lifeis/slice_%d.txt",p);
        FILE* o=fopen(sp,"w"); if(!o){ fclose(f); return 0; }
        for(long k=0;k<cnt && fgets(line,sizeof line,f);k++) fputs(line,o);
        fclose(o);
    }
    fclose(f);
    return 1;
}

/* fork one cell of the chorus. corpus=slice for the birth cohort, or NULL for a
 * cell born into the ether (later generations feed on the colony's own voice). */
static pid_t spawn_cell(const char* genome, const char* corpus, int label, unsigned long cseed){
    fflush(stdout);                                /* empty inherited buffer before fork */
    pid_t p=fork();
    if(p==0){
        char waste[256]; snprintf(waste,sizeof waste,"lifeis/waste_%d.log",label);
        int rc=live(genome, corpus, waste, "lifeis/ether.txt", NULL, cseed, label);
        fflush(stdout); _exit(rc);                 /* _exit skips stdio flush — do it by hand */
    }
    if(p<0) perror("fork");
    return p;
}

/* fetch the idx-th line of the birth queue into out (255 cap), '' if absent. */
static void births_line(long idx, char* out){
    out[0]='\0';
    FILE* b=fopen("lifeis/births.txt","r"); if(!b) return;
    char ln[256]; long i=0;
    while(fgets(ln,sizeof ln,b)){ if(i==idx){ ln[strcspn(ln,"\n")]='\0'; strncpy(out,ln,255); out[255]='\0'; break; } i++; }
    fclose(b);
}

int main(int argc, char** argv){
    /* THE BIRTHDAY WAR — print an organism's mathematical birthday for a seed (the harness reads it to compute the true in-window bit): ./l --calb <seed> */
    if(argc>2 && strcmp(argv[1],"--calb")==0){
        unsigned long seed = strtoul(argv[2],NULL,10);
        printf("%.4f\n", (double)((float)(hash_seed(seed,33) % 69396UL)/10.0f));
        return 0;
    }
    /* THE C-FIELD — the rubber test in isolation: load the field with different resonance vectors, print the OPTION-SET
     * (peak count + positions) each state faces. same field, different observers → different menus. ./l --fieldtest */
    if(argc>1 && strcmp(argv[1],"--fieldtest")==0){
        struct { const char* who; float S,diss,hunger,guilt,scar; } obs[5] = {
            {"at-rest ", 0.00f, 0.0f, 0.0f, 0.0f, 0.0f},
            {"hungry  ", 0.10f, 0.5f, 1.0f, 0.0f, 0.0f},
            {"sated   ", 0.10f, 0.5f, 0.1f, 0.0f, 0.0f},
            {"guilty  ", 0.10f, 2.0f, 0.5f, 1.4f, 0.6f},
            {"agitated", 0.80f, 3.0f, 0.5f, 0.0f, 0.2f},
        };
        printf("observer   | option-set (peaks of the deformed field: count @ positions)\n");
        for(int k=0;k<5;k++){
            cfield_reset(); cfield_load(obs[k].S,obs[k].diss,obs[k].hunger,obs[k].guilt,obs[k].scar);
            int pos[CFIELD_N]; int n=cfield_peaks(pos,CFIELD_N);
            printf("%s   | %2d @", obs[k].who, n);
            for(int i=0;i<n;i++) printf(" %d", pos[i]);
            int c0=cfield_collapse();                 /* collapse the superposition to one fact (∝ u²) — verifies the read */
            printf("   -> collapse %d\n", c0);
        }
        printf("(if the SETS differ across states — not just the odds over one menu — the rubber is real in isolation; the load-bearing test is Step 1b)\n");
        return 0;
    }
    if(argc>1 && strcmp(argv[1],"--monismtest")==0){   /* verify the monism HEART in isolation: DEATH-site clean, log1p bounded, and the RELATIONAL property (a foreign scar-pattern raises disorder, the reader's own does not) */
        DEATH_ID = semtok_find_glyph("death");
        static float scarA[VOCAB_CAP], scarB[VOCAB_CAP];
        for(int i=0;i<VOCAB_CAP;i++){ scarA[i]=0.0f; scarB[i]=0.0f; }
        scarA[DEATH_ID]=6.0f; scarA[3]=1.0f; scarA[7]=2.0f;      /* A is a killer: a big death-scar + wounds */
        scarB[11]=1.5f; scarB[23]=1.0f; scarB[41]=0.8f;          /* B is a DIFFERENT self, wounded on other glyphs */
        static float LA[CFIELD_N], LB[CFIELD_N];
        monism_profile(0.1f,0.5f,0.3f,0.0f,scarA,LA);            /* same canonical state for both — the signal is the scar pattern */
        monism_profile(0.1f,0.5f,0.3f,0.0f,scarB,LB);
        int death_clean=1; for(int g=0;g<VOCAB_CAP;g++) if(g!=DEATH_ID && monism_glyph_site(g)==MONISM_DEATH_SITE){ death_clean=0; break; }
        float big=0.0f; { static float sB[VOCAB_CAP]; for(int i=0;i<VOCAB_CAP;i++)sB[i]=0.0f; sB[DEATH_ID]=1e6f; static float Lbig[CFIELD_N]; monism_profile(0,0,0,0,sB,Lbig); for(int i=0;i<CFIELD_N;i++) if(Lbig[i]>big)big=Lbig[i]; }
        float dis_foreign = monism_disorder(LB, LA);            /* B against A's standing pattern — should be HIGH */
        float dis_self    = monism_disorder(LB, LB);            /* B against its own echo — should be ~0 */
        printf("DEATH-site clean:  %s (dedicated site %d)\n", death_clean?"YES":"NO", MONISM_DEATH_SITE);
        printf("log1p bounded:     max forcing %.3f <= clamp %.1f : %s\n", (double)big, (double)MONISM_CLAMP, big<=MONISM_CLAMP+1e-4f?"YES":"NO");
        printf("relational heart:  disorder(B|A_foreign)=%.4f   disorder(B|B_self)=%.4f\n", (double)dis_foreign, (double)dis_self);
        printf("=> heart works iff foreign >> self — the OTHER's dent raises dissonance, the reader's own does not\n");
        return 0;
    }
    if(argc>1 && strcmp(argv[1],"--monismhalflife")==0){   /* FLAG 2 (Fable): the half-life of a foreign dent under H2 — B smears it with its own deposits; this fixes the event-study window W before M-2 */
        DEATH_ID = semtok_find_glyph("death");
        static float scarB[VOCAB_CAP]; for(int i=0;i<VOCAB_CAP;i++) scarB[i]=0.0f;
        scarB[11]=1.5f; scarB[23]=1.0f; scarB[41]=0.8f;    /* a typical wounded B (not a killer — no death-scar) */
        static float L[CFIELD_N]; monism_profile(0.1f,0.5f,0.3f,0.0f,scarB,L);
        cfield_reset(); for(int i=0;i<CFIELD_N;i++){ g_shadow_u[i]=0.0f; g_shadow_v[i]=0.0f; }
        for(int t=0;t<200;t++){                             /* warmup: B settles its own field + shadow */
            for(int i=0;i<CFIELD_N;i++) g_cfield_u[i]+=L[i]; for(int s=0;s<CFIELD_STEPS;s++) cfield_step();
            for(int i=0;i<CFIELD_N;i++) g_shadow_u[i]+=L[i]; for(int s=0;s<CFIELD_STEPS;s++) cfield_step_buf(g_shadow_u,g_shadow_v); }
        float floor = monism_disorder(g_shadow_u, g_cfield_u);
        static float sA[VOCAB_CAP]; for(int i=0;i<VOCAB_CAP;i++) sA[i]=0.0f; sA[DEATH_ID]=6.0f;   /* A's death-pattern (death 6.0) */
        static float LA[CFIELD_N]; monism_profile(0.1f,0.5f,0.3f,0.0f,sA,LA);
        static float adent[CFIELD_N],av[CFIELD_N]; for(int i=0;i<CFIELD_N;i++){ adent[i]=LA[i]; av[i]=0.0f; } for(int s=0;s<CFIELD_STEPS;s++) cfield_step_buf(adent,av);
        for(int i=0;i<CFIELD_N;i++) g_cfield_u[i]+=adent[i];   /* inject the foreign dent into the REAL field */
        float peak = monism_disorder(g_shadow_u, g_cfield_u);
        int halflife=-1;
        for(int t=1;t<=300;t++){                            /* B keeps depositing its own — the dent decays by damping + dilution */
            for(int i=0;i<CFIELD_N;i++) g_cfield_u[i]+=L[i]; for(int s=0;s<CFIELD_STEPS;s++) cfield_step();
            for(int i=0;i<CFIELD_N;i++) g_shadow_u[i]+=L[i]; for(int s=0;s<CFIELD_STEPS;s++) cfield_step_buf(g_shadow_u,g_shadow_v);
            float d = monism_disorder(g_shadow_u, g_cfield_u);
            if((d-floor) <= 0.5f*(peak-floor)){ halflife=t; break; } }
        printf("FLAG2 half-life: floor=%.4f peak=%.4f dent=%.4f  half-life=%d ticks -> event-study window W (declare one value, do not sweep)\n",
               (double)floor,(double)peak,(double)(peak-floor), halflife);
        return 0;
    }
    /* THE STRIKE FALSIFIER — print the blind control's random birthday-key (hash slot 77, uncorrelated with the true birthday's slot 33): ./l --calbb <seed> */
    if(argc>2 && strcmp(argv[1],"--calbb")==0){
        unsigned long seed = strtoul(argv[2],NULL,10);
        printf("%.4f\n", (double)((float)(hash_seed(seed,77) % 69396UL)/10.0f));
        return 0;
    }
    /* THE LOCK metric — window-schedule agreement between two birthdays over N ticks. the operationally meaningful lock:
     * do the vulnerable-window SEQUENCES line up? (robust to phase-aliasing of the absolute birthday). ./l --calcmp b1 b2 N */
    if(argc>4 && strcmp(argv[1],"--calcmp")==0){
        float b1=strtof(argv[2],NULL), b2=strtof(argv[3],NULL); long N=strtol(argv[4],NULL,10);
        long both=0,win2=0; for(long t=0;t<N;t++){ int w1=cal_pd(t,b1)>CAL_THRESH, w2=cal_pd(t,b2)>CAL_THRESH; if(w1==w2)both++; if(w2)win2++; }
        printf("%.4f %ld\n",(double)both/(double)(N?N:1), win2);   /* agreement fraction, and the true window count (base rate) */
        return 0;
    }
    /* THE LOCK metric, sharpened (Mythos) — Jaccard of the two in-window SETS: |∩|/|∪|, so shared out-of-window ticks
     * (the base-rate mass at duty~0.15) do not mask whether the rare vulnerable windows actually coincide. ./l --caljac b1 b2 N */
    if(argc>4 && strcmp(argv[1],"--caljac")==0){
        float b1=strtof(argv[2],NULL), b2=strtof(argv[3],NULL); long N=strtol(argv[4],NULL,10);
        long inter=0,uni=0; for(long t=0;t<N;t++){ int w1=cal_pd(t,b1)>CAL_THRESH, w2=cal_pd(t,b2)>CAL_THRESH; if(w1&&w2)inter++; if(w1||w2)uni++; }
        printf("%.4f %ld\n",(double)inter/(double)(uni?uni:1), uni);   /* Jaccard overlap, and the union size */
        return 0;
    }
    /* interactive mouth: ./l --mouth [seed] */
    if(argc>1 && strcmp(argv[1],"--mouth")==0){
        unsigned long seed = argc>2 ? strtoul(argv[2],NULL,10) : 42UL;
        seed_rng(seed); mouth_build(); charges_init();
        Model* m=model_new(); run_mouth(m,seed); free(m); return 0;
    }
    mouth_build();          /* build the orthographic router before ANY fork or bite */
    charges_init();
    g_field_on=(getenv("NL_NOFIELD")==NULL);   /* A/B toggle: NL_NOFIELD=1 lifts the coherence field */
    g_gate_sharp=(getenv("NL_NOEARNED")==NULL);    /* DEFAULT ON: the organism speaks field + a voice its body EARNED by living */

    /* CHORUS: ./l chorus [cohort] [seed] — a colony of forked cells, each a distinct
     * body on a slice of the world. fork gives every cell its own field / scars /
     * symbols. molequla ecology + caveLLMan colony lineage, kept to one file. */
    if(argc>1 && strcmp(argv[1],"chorus")==0){
        int cohort = argc>2 ? atoi(argv[2]) : CHORUS_COHORT;
        if(cohort<1) cohort=1; if(cohort>MAX_CELLS) cohort=MAX_CELLS;
        unsigned long seed = argc>3 ? strtoul(argv[3],NULL,10) : 42UL;
        mkdir("lifeis",0755);
        if(!corpus_slice("lifeis/world.txt", cohort)){ printf("  no world to slice (lifeis/world.txt). да будет так.\n"); return 1; }
        { FILE* e=fopen("lifeis/ether.txt","w"); if(e) fclose(e); }   /* fresh ether — last run's voices don't haunt this one */
        { FILE* b=fopen("lifeis/births.txt","w"); if(b) fclose(b); }   /* fresh birth queue */
        printf("nanolife CHORUS — %d cells wake on %d slices; the colony may grow to %d, and dies when all fall silent. seed=%lu\n\n",
               cohort, cohort, MAX_CELLS, seed);
        int live_n=0, next_label=0, peak=0; long births_consumed=0;
        for(int i=0;i<cohort;i++){                  /* the birth cohort — each on a slice of the world */
            char sl[256]; snprintf(sl,sizeof sl,"lifeis/slice_%d.txt",i);
            if(spawn_cell(NULL, sl, next_label, seed+i)>0){ next_label++; live_n++; }  /* cohort: no parent, a slice of the world */
        }
        if(live_n>peak) peak=live_n;
        while(live_n>0){                            /* THE GOVERNOR — population breathes 0..MAX_CELLS */
            int st; pid_t d=waitpid(-1,&st,WNOHANG);
            if(d>0){ live_n--; printf("[governor] a cell fell silent — %d alive\n", live_n); fflush(stdout); }
            long avail=0; { FILE* b=fopen("lifeis/births.txt","r"); if(b){ int c; while((c=fgetc(b))!=EOF) if(c=='\n') avail++; fclose(b);} }
            int sex_on = (getenv("NL_NOSEX")==NULL);   /* Ⓑ A/B: NL_NOSEX=1 → asexual only */
            while(births_consumed<avail && live_n<MAX_CELLS && next_label<MAX_LIFETIME_CELLS){  /* a divide fills a slot */
                char pa[256], pb[256]; births_line(births_consumed, pa);   /* the genome a reproducing cell left */
                unsigned long cseed = seed + 7919UL*(unsigned long)(next_label+1);
                int born=0;
                if(sex_on && avail-births_consumed>=2 && pa[0]){           /* Ⓑ SEX: two gametes waiting → one creole child */
                    births_line(births_consumed+1, pb);
                    char zp[256]; snprintf(zp,sizeof zp,"lifeis/children/zygote_%d.nl", next_label);
                    if(pb[0] && recombine(pa,pb,cseed,(long)next_label,zp)){
                        if(spawn_cell(zp, NULL, next_label, cseed)>0){     /* HEREDITY×2: born OF two parents' fields */
                            remove(pa); remove(pb);                        /* #6: consume the gametes ONLY once the child truly lives */
                            printf("[governor] a cell is born of TWO parents (cell %d) — %d alive\n", next_label, live_n+1); fflush(stdout);
                            next_label++; live_n++; if(live_n>peak) peak=live_n;
                            births_consumed+=2; born=1;
                        } else { remove(zp); break; }                      /* #6: fork failed — drop the orphan zygote, retry next governor pass */
                    }
                }
                if(!born){                                                /* asexual fallback: one parent → one child */
                    if(spawn_cell(pa[0]?pa:NULL, NULL, next_label, cseed)>0){
                        printf("[governor] a cell is born of a parent (cell %d) — %d alive\n", next_label, live_n+1); fflush(stdout);
                        next_label++; live_n++; if(live_n>peak) peak=live_n;
                        births_consumed++;                                 /* #6: advance ONLY on a successful spawn */
                    } else break;                                          /* fork failed (or nothing to spawn) — retry next pass, don't lose the birth */
                }
            }
            if(d<=0) usleep(20000);                 /* 20ms — don't spin the governor */
        }
        printf("\nnanolife CHORUS — the colony fell silent. %d cells lived, peak %d alive at once. да будет так.\n", next_label, peak);
        return 0;
    }

    /* solo: the single mortal clock — ./l [seed] [diet] */
    unsigned long seed = argc>1 ? strtoul(argv[1],NULL,10) : 42UL;
    const char* diet = argc>2 ? argv[2] : NULL;
    { int t[CTX]; int bn=semtok_line("the sun is fire and i feel fear in the dark",t,CTX);
      if(bn<1){ t[0]=BOS_ID; bn=1; }
      printf("  first breath:"); for(int i=0;i<bn;i++) printf(" %s",glyph_name(t[i])); printf("\n\n"); }
    return live(NULL, "lifeis/world.txt", "lifeis/waste.log", NULL, diet, seed, -1);
}
