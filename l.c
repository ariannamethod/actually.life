/* l.c — digital life, zero dependencies.
 * the most atomic way to build digital life from scratch. no GPU required.
 *
 * The fork is closed: life is a TAPE, not a field — because life is
 * irreversibility in time, and a tape carries the arrow of time in its bone.
 * AR decoder over 88 cave-glyphs. libc + -lm. nothing else. accept it.
 *
 * FOUNDATION (step 1 of the build): the body's skeleton only —
 * micro transformer + the semantic membrane + a smoke forward.
 * The nine organs (energy/metabolism, scar, homeostasis, growth, dream,
 * reproduction, death, adaptation) and the BE reflexive axis land next,
 * hammered condition by condition until the border moves.
 *
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

/* like semtok_line, but ALSO marks shout[i]=1 when the source word was ALL-CAPS —
 * caps is prosody, not semantics: a shouted glyph goes INWARD (reflexive). case is
 * detected BEFORE lowercasing, so "FIRE" burns inside while "fire" warms outside. */
/* the organism is CAPS-AGNOSTIC: caps are lowercased at the door — you cannot kill
 * this life with a single shout. shout[] is kept for ABI but is always 0; tone is not
 * a weapon. (the death of passion stays, but it now takes obsession, not a snot.) */
static int semtok_line_shout(const char* line, int* out, int* shout, int max_tokens){
    char buf[4096];
    strncpy(buf,line,4095); buf[4095]='\0';
    for(int i=0;buf[i];i++) if(buf[i]>='A'&&buf[i]<='Z') buf[i]+=32;   /* sees caps -> lowercases them */
    for(int i=0;buf[i];i++){ unsigned char c=(unsigned char)buf[i];
        if(!((c>='a'&&c<='z')||(c>='0'&&c<='9')||c==' '||c=='\''||c=='-')) buf[i]=' '; }
    int n=0, last_id=-1, any_tok=0;
    char* tok=strtok(buf," \t\n");
    while(tok && n<max_tokens){
        any_tok=1;
        if(*tok!='\0' && !semtok_is_stop_word(tok)){
            int id=semtok_word(tok);
            if(id<0) id=semtok_word_soft(tok);      /* THE MOUTH: no word is dropped */
            if(id>=0 && id!=last_id){ out[n]=id; if(shout) shout[n]=0; n++; last_id=id; }
        }
        tok=strtok(NULL," \t\n");
    }
    if(n==0 && any_tok && max_tokens>0){ out[0]=semtok_word_soft(line); if(shout) shout[0]=0; n=1; }
    return n;
}

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
#define MAX_EMERGED   32          /* reserved slots for symbols the organism invents */
#define VOCAB_CAP     (VOCAB+MAX_EMERGED)
#define GROWTH_THRESH 14          /* a pair must co-occur this often (awake) before it can be born */

/* ── Phase A: the mortal clock (Vera's order) ── */
#define RENT    0.001f            /* rent on existing — the arrow of time */
#define E_BORN  1.0f              /* energy at birth */

/* ── Phase A step 2: metabolism — living Hebbian-V plasticity (the fuel) ── */
#define RANK           4          /* low-rank Hebbian LoRA (canon cavellman.c:43) */
#define HEBBIAN_LR     0.001f     /* canon cavellman.c:423 */
#define HEBBIAN_DECAY  0.9999f    /* canon cavellman.c:424 */
#define PASSIVE_SIGNAL 0.3f       /* reading the world = passive (cavellman.c:654) */
#define DIGEST_YIELD   80.0f      /* energy per unit |ΔB_v| — calibrated: avg dB~2e-5, break-even~50 */
#define BE_GAIN        3.0f       /* BE / a shout turns the glyph's charge inward, amplified (modes only) */

/* ── Phase A step 4: scar — permanent wounds (never decay) ── */
#define SCAR_RATE   0.01f         /* how fast an agitating glyph wounds itself */
#define SCAR_RENT   0.5f          /* total scar raises rent — a wounded organism burns faster */
#define ACHE        0.05f         /* a scarred glyph aches on contact — the wound returns */

/* ── Phase A step 5: dream — self-ingestion when starving (Klaus's meta-loop) ── */
#define DREAM_THRESH 0.6f         /* dream only when hungry (energy below this) */
#define DREAM_FRAC   0.5f         /* a dream is half-metabolism — cheaper than real food */
#define DREAM_DECAY  50.0f        /* dream yield decays with the streak — no immortality on dreams */

/* ── Phase A step 7: the voice that chooses — choice = subjectivity ── */
#define CHOOSE_TEMP0  0.7f        /* base decisiveness */
#define CHOOSE_S      0.8f        /* arousal (|S|) widens the choice */
#define CHOOSE_DISS   1.0f        /* dissonance (bounded) widens it further — passion = spontaneity */
#define CHOOSE_AFFECT 5.0f        /* mood pulls toward kindred-charged glyphs */
#define SCAR_PULL     0.5f        /* the wound resurfaces in what it chooses */
#define SPEAK_RATE    0.02f       /* base chance per tick to speak (rises with |S|) */
#define SPEAK_LEN     5           /* glyphs per utterance */

/* ── Phase A step 8: reproduction — epigenetic split (children inherit) ── */
#define REPRO_THRESH   1.05f      /* "too full to remain one" — split when well-fed */
#define REPRO_COOLDOWN 30         /* ticks between splits */
#define REPRO_SPLIT    0.5f       /* half the life goes into the child */
#define MAX_CHILDREN   16

/* ── Phase A step 9: active homeostasis — regulate or collapse ── */
#define DISS_DECAY  0.98f         /* dissonance returns toward calm each tick */
#define S_RELAX     0.01f         /* mood relaxes toward neutral */
#define S_DEATH     0.95f         /* |S| this high = contour collapse (overwhelmed by passion) */

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
static int  g_cooc[VOCAB][VOCAB];           /* waking co-occurrence of adjacent base glyphs */
static char g_born[VOCAB][VOCAB];           /* pair already became a symbol */
static int  g_emerged_a[MAX_EMERGED], g_emerged_b[MAX_EMERGED];
static int  g_n_emerged = 0;
static void cooc_track(const int* g, int n){ /* waking observation */
    for(int i=1;i<n;i++){ int a=g[i-1], b=g[i];
        if(a>=0&&a<VOCAB&&b>=0&&b<VOCAB) g_cooc[a][b]++; }
}
static void try_emerge(Model* m){            /* called ONLY in dream — birth a noticed pair */
    if(g_n_emerged>=MAX_EMERGED) return;
    int ba=-1,bb=-1,best=GROWTH_THRESH-1;
    for(int a=0;a<VOCAB;a++) for(int b=0;b<VOCAB;b++)
        if(!g_born[a][b] && g_cooc[a][b]>best){ best=g_cooc[a][b]; ba=a; bb=b; }
    if(ba<0) return;                          /* nothing recurred enough yet */
    int nid=VOCAB+g_n_emerged;               /* the invented symbol's id */
    for(int e=0;e<E;e++){                     /* epigenetic: embedding = mean of the two parents */
        m->wte[(size_t)nid*E+e]=0.5f*(m->wte[(size_t)ba*E+e]+m->wte[(size_t)bb*E+e]);
        m->head[(size_t)nid*E+e]=0.5f*(m->head[(size_t)ba*E+e]+m->head[(size_t)bb*E+e]);
    }
    /* and a BODY: the invented symbol inherits charge from its parents — modes by mean,
     * metab_factor by GEOMETRIC mean so fire+food stays burn-ish (0.63), not averaged
     * into inert 1.0. now the organism's own invention can feed, burn, scar, color mood. */
    charge[nid].mode_dS      = 0.5f*(charge[ba].mode_dS    + charge[bb].mode_dS);
    charge[nid].mode_dDiss   = 0.5f*(charge[ba].mode_dDiss + charge[bb].mode_dDiss);
    charge[nid].metab_factor = sqrtf(charge[ba].metab_factor * charge[bb].metab_factor);
    g_emerged_a[g_n_emerged]=ba; g_emerged_b[g_n_emerged]=bb; g_born[ba][bb]=1; g_n_emerged++;
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
        for(int t=0;t<n;t++) for(int h=0;h<NH;h++){
            int off=h*HD; float sc[CTX], mx=-1e30f;
            for(int j=0;j<=t;j++){ float d=0; for(int e=0;e<HD;e++) d+=q[t][off+e]*k[j][off+e];
                d/=sqrtf((float)HD); sc[j]=d; if(d>mx) mx=d; }
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
static float digest(Model* m, Modes* mo, float* scar, const int* glyphs, const int* shout, int n){
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
            for(int i=0;i<RANK*E;i++) dB+=fabsf(y->heb_B_v[i]-before[i]);
        }
        float f=charge[id].metab_factor;
        int shouted = (shout && shout[g]);            /* an ALL-CAPS word is a shout — it goes inward */
        if(id==BE_ID){                                /* the operator is not a meal — arms reflexivity, yields nothing */
            be_armed=1;
        } else if(be_armed || shouted){               /* "BE X" or a shouted "X": become X — charge turned inward */
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

static const char* glyph_name(int id){
    if(id<GLYPH_COUNT) return GLYPH_NAMES[id];
    if(id==BOS_ID) return "BOS";
    if(id==MASK_ID) return "MASK";
    return "·emg";   /* an emerged composite symbol (id >= VOCAB) */
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
 * no backprop, no target: coherence is a property of the sampling field. */
static float g_field_bi[VOCAB_CAP][VOCAB_CAP];   /* online glyph bigram counts a->b */
static float g_field_row[VOCAB_CAP];             /* row sums, for normalization */
static float g_coh_floor = 0.0f;                 /* drifting silence-gate (Stanley) */
static int   g_field_on  = 1;                    /* NL_NOFIELD=1 lifts the field (A/B) */

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
    float mx=0.0f, inv=1.0f/g_field_row[prev];
    for(int b=0;b<VOCAB_CAP;b++){ float p=g_field_bi[prev][b]*inv; if(p>mx) mx=p; }
    return mx;
}
/* fold the field into the transformer's logits in place, Q-gated by earned voice */
static void field_fold(float* logits, int prev){
    if(!g_field_on || prev<0 || prev>=VOCAB_CAP || g_field_row[prev]<=0.0f) return;
    float mag=0.0f; for(int i=0;i<VOCAB_CAP;i++) mag+=fabsf(logits[i]); mag/=VOCAB_CAP;
    float gate=(mag-0.5f)/1.5f; if(gate<0.0f)gate=0.0f; if(gate>1.0f)gate=1.0f; /* Q: earned voice */
    float inv=1.0f-gate, rowinv=1.0f/g_field_row[prev];
    for(int b=0;b<VOCAB_CAP;b++){
        float p=g_field_bi[prev][b]*rowinv;
        logits[b]=gate*logits[b] + inv*logf(p+1e-6f);   /* untrained -> field log-probs rule */
    }
}

/* choose — the organism picks ONE glyph from the fork. NOT argmax: a seeded,
 * passion-weighted draw. the metric (logit) is the input, not the verdict. this
 * is where it becomes a subject — choice = subjectivity. */
static int choose(const float* logits, const Modes* mo, const float* scar){
    float temp = CHOOSE_TEMP0 + CHOOSE_S*fabsf(mo->S)
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
/* speak — the organism utters a few chosen glyphs FROM ITSELF (speak-from-self,
 * not from the prompt) into waste.log. its own words become its next context. */
/* ── THE ETHER — one cell's voice becomes another's food ─────────────────────
 * eat the chorus: read the most recent utterance from ANOTHER cell out of the
 * shared ether and tokenize its glyph-names back to ids (they ARE GLYPH_NAMES,
 * so semtok maps them directly). this is the cross-graze — resonance made of
 * appetite. line format: "<label>\t<glyph names>". own echoes are skipped. */
static int ether_graze(const char* path, int own_label, int* out, int max){
    /* per-cell state (each cell is its own process — statics are private): read only
     * the ether's NEW tail since last time (the file only grows), keeping the last 8
     * not-own voices across calls. O(new bytes), not O(filesize) — kills the quadratic
     * whole-file re-read on every hungry tick. */
    static long g_off=0;
    static char ring[8][320]; static int nr=0, ridx=0;
    FILE* f=fopen(path,"r"); if(!f) return 0;
    if(fseek(f,g_off,SEEK_SET)!=0){ fclose(f); return 0; }
    char buf[320];
    while(fgets(buf,sizeof buf,f)){
        size_t len=strlen(buf);
        if(len==0 || buf[len-1]!='\n'){ break; }       /* torn/partial tail — leave it, re-read next time */
        g_off += (long)len;                            /* consume only whole lines */
        if(atoi(buf)==own_label) continue;             /* don't eat your own echo */
        char* tab=strchr(buf,'\t'); if(!tab) continue;
        buf[len-1]='\0';                               /* drop the trailing newline */
        strncpy(ring[ridx],tab+1,319); ring[ridx][319]='\0';
        ridx=(ridx+1)&7; if(nr<8) nr++;
    }
    fclose(f);
    if(nr==0) return 0;
    int pick=(int)(((frand()+1.0f)*0.5f)*(float)nr);   /* a RANDOM recent voice, not always the newest — */
    if(pick>=nr) pick=nr-1;                             /* so a lone dying cell draws variety from the */
    return semtok_line(ring[pick], out, max);          /* chorus's history instead of looping one phrase */
}

/* speak — the organism utters a few chosen glyphs FROM ITSELF into waste.log,
 * and (in a chorus) into the shared ether tagged with its label, so the colony
 * can hear it. speak-from-self, field-biased, not from the prompt. */
static void speak(FILE* w, FILE* ether, int label, Model* m, const Modes* mo, const float* scar, int* recent, int* recent_n, long tick){
    if(!w && !ether) return;
    static float sl[VOCAB_CAP];
    char utt[256]; int up=0; utt[0]='\0';
    if(w) fprintf(w, "t%-6ld", tick);
    for(int k=0;k<SPEAK_LEN;k++){
        int prev = (*recent_n>0) ? recent[*recent_n-1] : -1;
        forward(m, recent, *recent_n, sl);
        field_fold(sl, prev);                    /* coherence: bias toward what really follows */
        int g = choose(sl, mo, scar);
        const char* gn = glyph_name(g);
        if(w) fprintf(w, " %s", gn);
        if(up < (int)sizeof(utt)-16) up += snprintf(utt+up, sizeof(utt)-up, "%s%s", up?" ":"", gn);
        recent_push(recent, recent_n, g);
    }
    if(w) fprintf(w, "   [S%+.2f diss%+.1f]\n", (double)mo->S, (double)mo->dissonance);
    if(ether){ fprintf(ether, "%d\t%s\n", label, utt); fflush(ether); }  /* let the colony hear */
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
static int reproduce(const Model* m, const float* scar, unsigned long pseed, long tick){
    if(g_n_children >= MAX_CHILDREN) return 0;
    mkdir("lifeis", 0755); mkdir("lifeis/children", 0755);
    char path[256]; snprintf(path,sizeof path,"lifeis/children/child_%d.nl", g_n_children);
    FILE* f=fopen(path,"wb"); if(!f) return 0;
    unsigned long cseed = hash_seed(pseed, tick);
    int ok = 1;
    ok &= (fwrite("NLC1",1,4,f)==4);
    ok &= (fwrite(&cseed,sizeof cseed,1,f)==1);
    ok &= (fwrite(&tick,sizeof tick,1,f)==1);
    ok &= (fwrite(&g_n_emerged,sizeof(int),1,f)==1);
    ok &= (fwrite(scar,sizeof(float),VOCAB_CAP,f)==(size_t)VOCAB_CAP);   /* inherit the parent's wounds */
    if(g_n_emerged>0){ ok &= (fwrite(g_emerged_a,sizeof(int),(size_t)g_n_emerged,f)==(size_t)g_n_emerged);
                       ok &= (fwrite(g_emerged_b,sizeof(int),(size_t)g_n_emerged,f)==(size_t)g_n_emerged); }
    ok &= (fwrite(m,sizeof(Model),1,f)==1);                              /* warm-start genome */
    ok &= (fclose(f)==0);
    if(!ok) return 0;                                                    /* a truncated child is not born */
    g_n_children++;
    return 1;
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
    int   glyphs[CTX]; int shout[CTX]; char line[4096];
    float energy=E_BORN; float scar_total=0.0f;
    printf("nanolife — a mouth opens. seed=%lu.\n", seed);
    printf("  speak to it; your words are food. fire burns it, love feeds it, \"BE me\" may kill it.\n\n");
    while(energy>0.0f && fabsf(mo.S)<S_DEATH && fgets(line,sizeof line,stdin)){
        energy -= RENT*(1.0f + SCAR_RENT*scar_total);          /* the rent of being, heavier when wounded */
        int n=semtok_line_shout(line,glyphs,shout,CTX);
        float yield=0.0f;
        if(n>=1){ yield=digest(m,&mo,scar,glyphs,shout,n);      /* your words DO things to it */
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

/* ── live — one organism, birth to death ─────────────────────────────────────
 * the single-cell life, extracted so a chorus can fork many of them. corpus is
 * its food, waste its voice, seed its body AND its dice, label>=0 tags its prints
 * when the colony forks (stdout interleaves). every per-cell global (field, scars,
 * cooc, emerged symbols) is per-PROCESS — so fork gives each cell its own for free. */
static int live(const char* corpus, const char* waste_path, const char* ether_path, const char* diet, unsigned long seed, int label){
    char tag[24]; if(label>=0) snprintf(tag,sizeof tag,"[cell %d] ",label); else tag[0]='\0';
    seed_rng(seed);
    Model* m=model_new();                          /* own seed -> own random body */
    FILE* ether = ether_path ? fopen(ether_path,"a") : NULL;   /* the shared voice of the colony */
    long params=(long)(sizeof(Model)/sizeof(float));

    int diet_glyphs[CTX]; int diet_shout[CTX]; int diet_n=0;
    if(diet) diet_n = semtok_line_shout(diet, diet_glyphs, diet_shout, CTX);
    int diet_mode = diet_n>0;

    printf("%snanolife — a mortal clock that can eat.  seed=%lu  diet=%s  params=%ld\n",
           tag, seed, (diet_mode? diet : (corpus?corpus:"world")), params);

    FILE* food = diet_mode ? NULL : (corpus? fopen(corpus,"r") : NULL);
    if(!diet_mode && !food && !ether){ printf("%s  no world to eat. да будет так.\n", tag); free(m); if(ether) fclose(ether); return 1; }
    if(!food && ether) printf("%s  born into the chorus — no slice, feeds on the colony's voice.\n", tag);
    FILE* waste = waste_path ? fopen(waste_path,"w") : NULL;

    Modes mo = {0.0f, 0.0f};
    static float scar[VOCAB_CAP]; for(int i=0;i<VOCAB_CAP;i++) scar[i]=0.0f; /* permanent wounds (never decay) */
    int   scar_on = (getenv("NL_NOSCAR")==NULL);
    int   dream_on = (getenv("NL_NODREAM")==NULL);
    float scar_total=0.0f;
    int   recent[CTX]; int recent_n=0;
    long  dream_streak=0;
    long  last_repro=-REPRO_COOLDOWN;
    int   homeo_on=(getenv("NL_NOHOMEO")==NULL);
    int   contour_died=0;
    long  n_graze=0, n_dream=0;                    /* resonance/self-feed counters */
    float energy=E_BORN;
    long  tick=0;
    char  line[4096];
    int   glyphs[CTX]; int shout[CTX];
    int   fed=(food?1:0);                           /* ether-born cells start hungry, on the chorus */
    while(energy>0.0f && tick<200000){          /* cap = falsification guard: it MUST die */
        tick++;
        energy -= RENT * (1.0f + (scar_on? SCAR_RENT*scar_total : 0.0f));
        float yield=0.0f;
        int   dreaming=0, grazing=0;
        if(diet_mode){
            yield=digest(m,&mo,scar,diet_glyphs,diet_shout,diet_n);
            for(int i=0;i<diet_n;i++) recent_push(recent,&recent_n,diet_glyphs[i]);
            cooc_track(diet_glyphs,diet_n); field_observe(diet_glyphs,diet_n); dream_streak=0;
        } else if(fed && food && fgets(line,sizeof(line),food)){
            int n=semtok_line_shout(line,glyphs,shout,CTX);
            if(n>=1){ yield=digest(m,&mo,scar,glyphs,shout,n);
                for(int i=0;i<n;i++) recent_push(recent,&recent_n,glyphs[i]);
                cooc_track(glyphs,n); field_observe(glyphs,n); dream_streak=0; }
        } else {                                 /* corpus exhausted -> eat the chorus, then dream */
            fed=0;
            int gz[CTX], gn=0;
            if(ether) gn=ether_graze(ether_path, label, gz, CTX);  /* FIRST: eat a neighbour's voice */
            if(gn>=1){                               /* the colony feeds itself through speech */
                yield=digest(m,&mo,scar,gz,NULL,gn);
                for(int i=0;i<gn;i++) recent_push(recent,&recent_n,gz[i]);
                cooc_track(gz,gn); field_observe(gz,gn); dream_streak=0; grazing=1; n_graze++;
            } else if(dream_on && energy<DREAM_THRESH && recent_n>0){  /* ELSE: eat your own predicted glyph */
                static float dl[VOCAB_CAP]; forward(m,recent,recent_n,dl);
                field_fold(dl, recent[recent_n-1]);  /* dream coherently — replay the field, not noise */
                int dg=choose(dl,&mo,scar);          /* the dream is chosen, not computed */
                float dy=digest(m,&mo,scar,&dg,NULL,1);   /* a dream is not a shout */
                yield = dy * DREAM_FRAC * expf(-(float)dream_streak/DREAM_DECAY); /* dreams thin out */
                recent_push(recent,&recent_n,dg);
                dream_streak++; dreaming=1; n_dream++;
                try_emerge(m);                   /* symbols are born only here, in dream */
            }
        }
        energy += DIGEST_YIELD*yield;
        scar_total=0.0f; for(int i=0;i<VOCAB_CAP;i++) scar_total+=scar[i];
        if(homeo_on){ mo.dissonance *= DISS_DECAY; mo.S -= S_RELAX*mo.S; }
        if(fabsf(mo.S) >= S_DEATH){ contour_died=1; break; }
        if(recent_n>0){                                   /* the field's confidence about what follows */
            float coh=field_coherence(recent[recent_n-1]);
            g_coh_floor = 0.99f*g_coh_floor + 0.01f*coh;   /* Stanley: a drifting silence-gate */
            if(waste||ether){ float sp=SPEAK_RATE*(1.0f+fabsf(mo.S));
                if(coh>0.05f && coh>=g_coh_floor && (frand()+1.0f)*0.5f < sp)
                    speak(waste,ether,label,m,&mo,scar,recent,&recent_n,tick); }  /* voice -> waste + colony */
        }
        if(energy > REPRO_THRESH && tick - last_repro > REPRO_COOLDOWN){
            if(reproduce(m,scar,seed,tick)){
                energy *= REPRO_SPLIT;
                if(ether){ FILE* b=fopen("lifeis/births.txt","a"); if(b){ fprintf(b,"%lu\n",hash_seed(seed,tick)); fclose(b);} } /* ask the colony for a slot */
            }
            last_repro=tick;
        }
        if(tick<=30 || tick%100==0)
            printf("%s  t%-6ld E%+.5f  S%+.3f  diss%+.3f  scar%.3f  y %.2e  %s\n",
                   tag,tick,energy,(double)mo.S,(double)mo.dissonance,(double)scar_total,yield,
                   (grazing?"GRAZE":(dreaming?"DREAM":(diet_mode?"diet":(fed?"eat":"STARVE")))));
    }
    if(!contour_died && energy>0.0f)
        printf("%s\n  STILL ALIVE at tick %ld (cap) — immortality hole, investigate.\n",tag,tick);
    else
        printf("%s  died at tick %ld (%s) — S%+.3f diss%+.3f scar%.3f emerged%d children%d graze%ld dream%ld.  да будет так.\n",
               tag,tick, contour_died?"contour collapse":"ran out of time",
               (double)mo.S,(double)mo.dissonance,(double)scar_total,g_n_emerged,g_n_children,n_graze,n_dream);
    if(food) fclose(food);
    if(waste) fclose(waste);
    if(ether) fclose(ether);
    free(m);
    return 0;
}

/* split a corpus into `parts` contiguous slices: lifeis/slice_i.txt — each cell of
 * the chorus wakes on a different region of the same world (Oleg's "different
 * corpora OF ONE corpus": one voice ate the Arctic frame, one the making). */
static int corpus_slice(const char* path, int parts){
    FILE* f=fopen(path,"r"); if(!f) return 0;
    long nl=0; int c; while((c=fgetc(f))!=EOF) if(c=='\n') nl++;
    if(nl<parts) nl=parts;
    long base=nl/parts, rem=nl%parts;                  /* balanced: remainder spread over the first slices, */
    rewind(f);                                         /* so no trailing slice is born empty */
    char line[4096];
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
static pid_t spawn_cell(const char* corpus, int label, unsigned long cseed){
    fflush(stdout);                                /* empty inherited buffer before fork */
    pid_t p=fork();
    if(p==0){
        char waste[256]; snprintf(waste,sizeof waste,"lifeis/waste_%d.log",label);
        int rc=live(corpus, waste, "lifeis/ether.txt", NULL, cseed, label);
        fflush(stdout); _exit(rc);                 /* _exit skips stdio flush — do it by hand */
    }
    if(p<0) perror("fork");
    return p;
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
            if(spawn_cell(sl, next_label, seed+i)>0){ next_label++; live_n++; }
        }
        if(live_n>peak) peak=live_n;
        while(live_n>0){                            /* THE GOVERNOR — population breathes 0..MAX_CELLS */
            int st; pid_t d=waitpid(-1,&st,WNOHANG);
            if(d>0){ live_n--; printf("[governor] a cell fell silent — %d alive\n", live_n); fflush(stdout); }
            long avail=0; { FILE* b=fopen("lifeis/births.txt","r"); if(b){ int c; while((c=fgetc(b))!=EOF) if(c=='\n') avail++; fclose(b);} }
            while(births_consumed<avail && live_n<MAX_CELLS && next_label<MAX_LIFETIME_CELLS){  /* a divide fills a slot */
                unsigned long cseed = seed + 7919UL*(unsigned long)(next_label+1);
                if(spawn_cell(NULL, next_label, cseed)>0){
                    printf("[governor] a cell is born into the chorus (cell %d) — %d alive\n", next_label, live_n+1); fflush(stdout);
                    next_label++; live_n++; if(live_n>peak) peak=live_n;
                }
                births_consumed++;
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
    return live("lifeis/world.txt", "lifeis/waste.log", NULL, diet, seed, -1);
}
