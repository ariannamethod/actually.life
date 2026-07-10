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
#define CHOOSE_TEMP0  0.7f        /* base decisiveness */
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
static char** g_pool = NULL;                  /* the contested chunks (lines of EVERY .txt in lifeis/), indexed once per process */
static int    g_pool_n = 0, g_pool_cap = 0;
#define ARENA_EXPIRE 20L                      /* seconds a claim holds a chunk; then it is re-contestable — territory is RE-WON, so the two never settle into a stable partition (anti-settling: the friction stays alive) */
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
static int arena_next(char* out, int cap){       /* atomically claim the lowest UNCLAIMED (unexpired) chunk; 0 when the whole territory is held */
    if(g_pool_n<=0 || !out || cap<=0) return 0;
    FILE* c=fopen("lifeis/arena/claims","a+"); if(!c) return 0;
    int fd=fileno(c); flock(fd, LOCK_EX);                     /* atomic: two organisms never claim the same ground at once */
    time_t now=time(NULL);
    unsigned char* claimed=(unsigned char*)calloc((size_t)g_pool_n,1);
    if(claimed){ rewind(c); int id; long ts;
        while(fscanf(c,"%d %ld",&id,&ts)==2)
            if(id>=0 && id<g_pool_n && (long)(now-(time_t)ts) < ARENA_EXPIRE) claimed[id]=1;   /* only UNEXPIRED claims exclude */
    }
    int pick=-1; if(claimed){ for(int i=0;i<g_pool_n;i++) if(!claimed[i]){ pick=i; break; } free(claimed); }
    if(pick>=0){ fseek(c,0,SEEK_END); fprintf(c,"%d %ld\n",pick,(long)now); fflush(c); }  /* stake, timestamped, flushed before we unlock */
    flock(fd, LOCK_UN); fclose(c);
    if(pick<0) return 0;                          /* every chunk is held and unexpired — starve until the ground frees */
    int L=(int)strlen(g_pool[pick]); if(L>=cap) L=cap-1;
    memcpy(out,g_pool[pick],(size_t)L); out[L]=0;
    return L;
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
    FILE* ether = ether_path ? fopen(ether_path,"a") : NULL;   /* the shared voice of the colony */
    long params=(long)(sizeof(Model)/sizeof(float));

    int diet_glyphs[CTX]; int diet_n=0;
    if(diet) diet_n = semtok_line(diet, diet_glyphs, CTX);
    int diet_mode = diet_n>0;

    printf("%snanolife — a mortal clock that can eat.  seed=%lu  diet=%s  params=%ld\n",
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
    g_arena_on     = (getenv("NL_ARENA")!=NULL);       /* THE ARENA: forage a CONTESTED pool instead of the corpus (opt-in; gate-invariant off) */
    if(g_arena_on && !g_pool) arena_index(corpus);
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
        } else if(diet_mode){
            yield=ingest(m,&mo,scar,diet_glyphs,diet_n,recent,&recent_n,&dream_streak); meal=diet_glyphs; meal_n=diet_n;
        } else if(fed && (g_arena_on ? (arena_next(line,(int)sizeof line)>0) : (fgets(line,sizeof(line),food)!=NULL))){   /* ARENA: forage the contested pool; else the corpus line-by-line (fed==1 implies food!=NULL) */
            int n=semtok_line(line,glyphs,CTX);
            if(n>=1){ yield=ingest(m,&mo,scar,glyphs,n,recent,&recent_n,&dream_streak); meal=glyphs; meal_n=n; }
        } else {                                 /* corpus exhausted -> eat the chorus, then dream */
            fed=0;
            int gn=0;
            if(ether) gn=ether_graze(m, ether_path, label, gz, CTX);  /* FIRST: eat a neighbour's voice (composites whole, Δ2) */
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
                    { float uc=speak(waste,ether,label,m,&mo,scar,recent,&recent_n,tick); energy -= SPEAK_COST*(float)SPEAK_LEN*(1.0f-uc); } }  /* voice -> waste + colony */
        }
        if(energy > REPRO_THRESH && tick - last_repro > REPRO_COOLDOWN){
            char cpath[256]; cpath[0]='\0';
            if(reproduce(m,scar,seed,tick,cpath)){
                energy *= REPRO_SPLIT;
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
