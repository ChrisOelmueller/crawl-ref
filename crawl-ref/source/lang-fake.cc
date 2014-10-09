/**
 * @file
 * @brief Translation filters.
 **/

#include "AppHdr.h"

#include "lang-fake.h"

#include <unordered_set>

#include "libutil.h"
#include "options.h"
#include "random.h"
#include "stringutil.h"
#include "unicode.h"

#define UPPER "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define LOWER "abcdefghijklmnopqrstuvwxyz"
#define LETTERS UPPER LOWER

// Any resemblance to the speech of a particular character from Rich Burlew's
// work is entirely accidental.  Suggestion that this table has been produced
// by transcribing the whole corpus of that character's speech and making it
// produce the same output from regular English must be dismissed as a rumour.

// For Perl version, "apt-get install filters".
static const char* dwarven[][4] =
{
  {"^yes$","aye"},            {"there","thar"},             {"eir$","ar"},
  {"about","aboot"},          {"^he$","'e"},                {"them","'em"},
  {"^him","'im"},             {"out of$","outta"},          {"of course","'course"},
  {"^of$","o'"},              {"^and$","an'"},              {"to$","ta"},
  {"tog","tag"},              {"that","tha"},               {"the","tha"},
  {"wouldn't","wouldn'ta"},   {"cannot","cannae"},          {"can't","cannae"},
  {"don't","dinnae"},         {"'re$","r"},                 {"for$","fer"},
  {"ver$","'er"},             {"ber$","b'r"},               {"every$","ev'ry"},
  {"^is$","be"},
  {"en$","'n"},               {"^if$","if'n"},              {"enl","'nl"},
  {"eng","'ng"},              {"ing","in'"},                {"ment","mn't"},
  {"^es","'s"},               {"^ex","'s"},                 {"^not$","na"},
  {"^no$","nay"},             {"n't have","n'tve"},
  {"^are$","be"},             {"have","haf"},               {"abl","'bl"},
  {"^you$","ye"},             {"^your","yer"},              {"^you'","ye'"},
  {"noth","nuth"},            {"^this$","'tis"},            {"^here","'ere"},
  {"doesn't","don't"},        {"at a$","atta"},             {"ith$","it'"},
  {"ered$","'red"},           {"into$","inta"},             {"^before","'fore"},
  {"wit' '","wit '"},         {"wit' t","wit t"},           {"wit' w","wit w"},
  {"wit' y","wit y"},         {"get a","git a"},            {"ally$","'lly"},
  {"^my","me"},               {"^i think$","methinks"},     {"nay w","na w"},
  {"^one$","'un"},            {"^'un a","one a"},           {"at ta$","atta"},
  {"ot ta$","otta"},          {"^isn't$","ain't"},          {"^so th","s'th"},
  {"ned$","n'd"},             {"^because","'cause"},
  {0}
};

// Resemblances to the manner of speech of Jägermonsters from "Girl Genius" by
// Phil and Kaja Foglio are... hrm dammit, using the proper name gives it out.
// And s/dammit/gotterdammerung/.
static const char* jager[][4] =
{
  {"this","dis"},
  {"that","dat"},
  {"they","dey"},
  {"their","der"},
  {"there","dere"},
  {"these","dese"},
  {"those","doze"},
  {"the$","de"},
  {"then","den"},
  {"them","dem"},
  {"yes","yah"},
  {"you","hyu"},
  {"hyur","you"},
  {"have","hef"},
  {"wh","v"},
  {"^w","v"},
  {"w","v", "o"},
  {"^ha","he"},
  {"ha","he", "c"},
  {"any","henny"},
  {"smart","schmot"},
  {"n't","n'"},
  {"ain'","hain't"},
  {"am not","hain't"},
  {"isn'","hain't"},
  {"is not","hain't"},
  {"aren'","hain't"},
  {"are not","haint't"},
  {"good","goot"},
  {"dad","papa"},
  {"stuff","stoff"},
  {"going to","gunna"},
  {"going","gun"},
  {"people","pipple"},
  {"little","liddle"},
  {"love","luv"},
  {"nice","nize"},
  {"bug","bog"},
  {"let me","lemme"},
  {"give me","gimmee"},
  {"castle","kestle"},
  {"clank","klenk"},
  {"better","betta"},
  {"darling","dollink"},
  {"some","zum"},
  {"ouble","ubbel"},
  {"bble","bbel"},
  {"^ex","hex"},
  {"exc","eks"},
  {"ex","eks"},
  {"accident","exident"},
  {"once","vunce"},
  {"id$","eed"},
  {"^one","vun"},
  {"kiss","kees"},
  {"^c","k", 0, "h"},
  {"am$","em"},
  {"^th","t"},
  {"th","dd", "!" LETTERS, "!" LETTERS},
  {"unix","yoonix"},
  {"^un","on"},
  {"^sh","sch", 0, "e"},
  {"^sl","schl"},
  {"^sm","schm"},
  {"^sn","schn"},
  {"^sp","schp"},
  {"^st","scht"},
  {"of","uf"},
  {"qu","qv"},
  {"ing","ink"},
  {"irl","url"},
  {"ish","eesh"},
  {"^i","hy", 0, ")" LETTERS},
  {"ve$","ff", "!" LETTERS},
  {"ect$","eck"},
  {"and","und"},
  {"^all","hall"},
  {"mister","herr"},
  {"mr.","herr"},
  {"^is$","iz"},
  {"god damn it","gotterdammerung"},
  {"goddammit","gotterdammerung"},
  {"goddamnit","gotterdammerung"},
  {"damn it","gotterdammerung"},
  {"goddam","gotterdammerung"},
  {"god damn","gotterdammerung"},
  {"god damn it","gotterdammerung"},
  {"damn","dem"},
  {"god","gott"},
  {"okay","hokay"},
  {"^ok$","hokay"},
  {"just","chust"},
  {0}
};

static const char* runes[][4] =
{
  {"ae",  "ᛇ"},
  {"a",   "ᚨ"},
  {"b",   "ᛒ"},
  {"c",   "ᚲ"},
  {"d",   "ᛞ"},
  {"e",   "ᛖ"},
  {"f",   "ᚠ"},
  {"g",   "ᚷ"},
  {"h",   "ᚺ"},
  {"i",   "ᛁ"},
  {"j",   "ᛃ"},
  {"k",   "ᚲ"},
  {"l",   "ᛚ"},
  {"m",   "ᛗ"},
  {"ng",  "ᛜ"},
  {"n",   "ᚾ"},
  {"o",   "ᛟ"},
  {"ph",  "ᚠ"},
  {"p",   "ᛈ"},
  {"q",   "ᛩ"},
  {"r",   "ᚱ"},
  {"s",   "ᛊ"},
  {"th",  "ᚦ"},
  {"t",   "ᛏ"},
  {"u",   "ᚢ"},
  {"v",   "ᚡ"},
  {"w",   "ᚹ"},
  {"x",   "ᚲᛊ"},
  {"y",   "ᚤ"}, // Anglo-Saxon, sometimes medieval
  {"z",   "ᛉ"},
  {0}
};

static const char* grunt[][4] =
{
  {"^killed$", "annihilated"},
  {"^kill$", "annihilate"},
  {"!", "..."},
  {".", "!", 0, LETTERS "0123456789"},
  {0}
};

static const char* mist[][4] =
{
  // Adjectives, adverbs
  {" angrily$", ""},
  {"^angry ", ""},
  {" loud, deep$", ""},
  {" magically$", ""},
  // Monster shouts seen
  {"hear two shouts!", "it!!"}, // unseen two-headed ogre
  {" shouts.", "!"},
  {" shouts!", "!"},
  {" barks.", "!"},
  {" barks!", "!"},
  {" bellows.", "!"},
  {" bellows!", "!"},
  {" buzzes.", "!"},
  {" buzzes!", "!"},
  {" caws.", "!"},
  {" caws!", "!"},
  {" croaks.", "!"},
  {" croaks!", "!"},
  {" emits a blaring wail!", "!"}, // fsvo monster
  {" gives a chilling moan.", "!"},
  {" gives a chilling moan!", "!"},
  {" gives a hideous shriek.", "!"},
  {" gives a hideous shriek!", "!"},
  {" gives a croak.", "!"},
  {" gives a croak!", "!"},
  {" gives an irritating high-pitched whine.", "!"},
  {" gives an irritating high-pitched whine!", "!"},
  {" growls.", "!"},
  {" growls!", "!"},
  {" gurgles horribly.", "!"},
  {" gurgles horribly!", "!"},
  {" hisses.", "!"},
  {" hisses!", "!"},
  {" roars.", "!"},
  {" roars!", "!"},
  {" screeches.", "!"},
  {" screeches!", "!"},
  {" shouts in stereo.", "!"},
  {" shouts in stereo!", "!"},
  {" trumpets.", "!"},
  {" trumpets!", "!"},
  {" lets out a hideous shriek!", "!"}, // orb pickup
  // Monster shouts unseen (keep distinct but short, add ! where necessary)
  {"hear a shout!", "it!"},
//{"hear a bark!", "it!"},
//{"hear a roar!", "it!"},
  {"hear a hideous shriek!", "shriek!"},
//{"hear a bellow!", "it!"},
  {"hear a trumpeting!", "trumpet!"},
//{"hear a screech!", "it!"},
  {"hear an buzzing noise.", "buzz!"},
  {"hear a chilling moan.", "moan!"},
  {"hear a horrible gurgling sound!", "gurgle!"},
//{"hear an irritating high-pitched whine.", "whine!"},
  {"hear a croak.", "croak!"},
  {"hear an angry growl.", "growl!"},
  {"hear an angry hiss.", "hiss!"},
//{"hear a loud caw!", "it!"},
  {"You hear a grinding noise.", "wall!"},
  // Player shouts
  {" for attention$", ""},
  {" wildly$", ""},
  {"You caterwaul$", "Shout"},
  {"You creak$",     "Shout"},
  {"You hiss$",      "Shout"},
  {"You meow$",      "Shout"},
  {"You roar$",      "Shout"},
  {"You shout$",     "Shout"},
  {"You sporulate$", "Shout"},
  {"You scream$",    "Shout"},
  {"You squeak$",    "Shout"},
  {"You squeal$",    "Shout"},
  {"You whoosh$",    "Shout"},
  {"You yell$",      "Shout"},
  {"You yowl$",      "Shout"},
  ////
  // Monster attacks player (ouch if hit, nope if no damage, miss)
  {"completely misses you$", "miss"},
  {"closely misses you$",    "miss"},
  {"barely misses you$",     "miss"},
  {"misses you$",            "miss"},
  {"hits you$",       "ouch"},
  {"bites you$",      "ouch"},
  {"stings you$",     "ouch"},
  {"touches you$",    "ouch"},
  {"engulfs you$",    "ouch"},
  {"claws you$",      "ouch"},
  {"pecks you$",      "ouch"},
  {"headbutts you$",  "ouch"},
  {"punches you$",    "ouch"},
  {"kicks you$",      "ouch"},
  {"tentacle-slaps you$", "ouch"},
  {"tail-slaps you$", "ouch"},
  {"gores you$",      "ouch"},
  {"constricts you$", "ouch"},
  {"tramples you$",   "ouch"},
  {"trunk-slaps you$","ouch"},
  {"pounces on you$", "ouch"},
  {"stings you$",     "ouch"},
  // Special ouch (monster)
  {"ouch but does no damage", "nope"},
  {"ouch from afar", "reach-ouch"},
  {", but do no damage", " nope"},
  {"but do no damage", "nope"},
  {", but does no damage", " nope"},
  {"but does no damage", "nope"},
  {"appears unharmed", "nope"},
  {"falters for a moment", "nope"}, // anti-magic
  // Special hit (player)
  {"prevents you from hitting", "cause miss"},
  {"You catch the helpless", "Hit helpless"},
  {"momentarily off-guard.", "stab."},
  {"from a blind spot!",    "stab."},
  {"completely off-guard!", "stab!"},
  {"from behind!",         "stab!"},
  {"fails to defend himself.", "stab!!"},
  {"fails to defend herself.", "stab!!"},
  {"fails to defend itself.",  "stab!!"},
  {"You pounce on the unaware", "Hit helpless stab(!!)"}, // felid special case makes for awful syntax
  {"'s magic leaks into the air", " anti-magic"},
  {"' magic leaks into the air", " anti-magic"},
  // Player resist MR-check effect (numbers?)
  {"You resist with almost no effort",  "MR easy"},
  {"You easily resist",                 "MR easy"},
  {"You resist with some effort",       "MR ok"},
  {"You resist with significant effort","MR ok"},
  {"You struggle to resist",            "MR tough"},
  {"You barely resist",                 "MR tough"},
  {"resists with almost no effort",  "resists easy"},
  {"easily resists",                 "resists easy"},
  {"resists with some effort",       "resists ok"},
  {"resists with significant effort","resists ok"},
  {"struggles to resist",            "resists tough"},
  {"barely resists",                 "resists tough"},
  // Combat: Player hit messages (if only regex)
  {"You completely miss$", "Miss far"},
  {"You miss$",            "Miss ok"},
  {"You closely miss$",    "Miss near"},
  {"You barely miss$",     "Miss almost"},
  {"You strike$", "Hit"},
  {"You pounce on$", "Hit"},
  {"You clumsily bash$", "Hit"},
  {"You puncture$", "Hit"},
  {"You impale$", "Hit"},
  {"You spit$", "Hit"},
  {" like the proverbial pig$", ""},
  {" like a pig$", ""},
  {"You skewer$", "Hit"},
  {" like a kebab$", ""},
  {"You stick$", "Hit"},
  {" like a pincushion$", ""},
  {"You perforate$", "Hit"},
  {" like a sieve$", ""},
  {"You slash$", "Hit"},
  {"You dice$", "Hit"},
  {" like an onion$", ""},
  {"You fracture$", "Hit"},
  {" into splinters$", ""},
  {"You carve$", "Hit"},
  {" like the proverbial ham$", ""},
  {" like a pillowcase$", ""},
  {" like a ripe choko$", ""},
  {"You cut$", "Hit"},
  {" into ribbons$", ""},
  {" like a ham$", ""},
  {"You chop$", "Hit"},
  {" into pieces$", ""},
  {"You thump$", "Hit"},
  {"You sock$", "Hit"},
  {"You bludgeon$", "Hit"},
  {"You shatter$", "Hit"},
  {"You crush$", "Hit"},
  {" like a grape$", ""},
  {"You beat$", "Hit"},
  {" like a drum$", ""},
  {"You hammer$", "Hit"},
  {" like a gong$", ""},
  {"You pound$", "Hit"},
  {" like an avil$", ""},
  {"You flatten$", "Hit"},
  {" like a pancake$", ""},
  {"You whack$", "Hit"},
  {"You thrash$", "Hit"},
  {"You devastate$", "Hit"},
  {"You punish$", "Hit"},
  {", causing immense pain$", ""},
  {"You scratch$", "Hit"},
  {"You claw$", "Hit"},
  {"You mangle$", "Hit"},
  {"You eviscerate$", "Hit"},
  {"You batter$", "Hit"},
  {"You punch$", "Hit"},
  {"You pummel$", "Hit"},
  {"You squash$", "Hit"},
  {" like the proverbial ant$", ""},
  {" into fine dust$", ""},
  {" like a punching bag$", ""},
  {"You pulverise$", "Hit"},
  {" like an ant$", ""},
  {" into a bloody pulp$", ""},
  // UC from forms
  {"You hit$", "Hit"},
  {"You bite$", "Hit"},
  {"You maul$", "Hit"},
  {"You shred$", "Hit"},
  {"You claw$", "Hit"},
  {"You smack$", "Hit"},
  {"You touch$", "Hit"},
  {"You engulf$", "Hit"},
  {"You release spores at$", "Hit"},
  {"You nip at$", "Hit"},
  {"You bite$",   "Hit"},
  {"You gouge$",  "Hit"},
  {"You chomp$",  "Hit"},
  // UC from aux attacks
  {"You constrict$", "Hit"},
  {"You kick$", "Hit"},
  {"You headbutt$", "Hit"},
  {"You peck$", "Hit"},
  {"^tail-slap$", "slap"},
  {"^tentacle-slap$", "slap"},
  {"You slap$", "Hit"},
  {"You pseudopods$", "Hit"},////
  // UC from mutations and intrinsics (TODO)
  {"You furiously retaliate!", "Aha."},
  {"is struck by your spines", "spined"},
  // Monster damage from world
  {" engulfs$", " hit"},
  {" engulf$", " hit"},
  {" hits$", " hit"},
  {"is engulfed ", ""},
  {"is struck by ", "in "},
//{"from afar with ", "reach "},
  {"from afar with ", ""},
  // Polymorph
  {"evaporates and reforms as", "now"},
  {"changes into an$", "now"},
  {"changes into a$", "now"},
  {"quivers uncontrollably and liquefies into", "now"},
  {"basks in the mutagenic energy and changes!", "poly."},
  {"basks in the mutagenic energy from its neighbour and changes!", "poly."},
  {"basks in the mutagenic energy from its kin and changes!", "poly."},
  {"Two slime creatures merge to form", "make"},
  {"shudders and is absorbed by its neighbour", "merge"},
  {"'s shape twists and changes as it dies", " was shifter"},
  {"looks momentarily different", "abort poly"},
  // Clinging
  {"clinging", "cling"},
  {"falls off the wall", "no cling"},
  {"falls off the door", "no cling"},
  {"You fall off the wall", "No cling"},
  {"You fall off the door", "No cling"},
  // Monster HP
  {"^destroyed$", "dead"},
  {"^destroy$", "kill"},
  {"^wounded$", "damaged"},
  {"almost dead", "20%"},
  {"severely damaged", "40%"},
  {"heavily damaged", "60%"},
  {"moderately damaged", "80%"},
  {"lightly damaged", "99%"},
  {"not damaged.", ""},
  {"^blow up$", "kill"},
  // Monster spells/abilities
  {"Your body is wracked with pain", "Torment"},
  {"You feel a surge of unholy energy", "No Torment"},
  {"Strange energies run through your body", "Mutate"},
  {"Strange energies course through your body", "Malmutate"},
  {"draws from the surrounding life force!", "drain life."},
  {"Something draws the life force from your surroundings.", "It drain life."},
  {"The surrounding life force dissipates!", "It drain life."},
  {"Something feeds on your intellect!", "Drain int."},
  {"Something tries to feed on your intellect!", "Drain int. resist."},
//{"begins to accept its allies' injuries", "get pain"},
  {"conjures a globe of magical energy", "makes battlesphere"},
  {"The air twists arounds and", "airstrike"},
  {"begins absorbing vital energies!", "???"},
  {"sacrifices itself to reknit", "revives"},
//{"gain new vigour!", "???"},
  {"Pain shoots through your body", "Pain"},
  {"convulses in agony!", "pain."},
  {"'s surroundings become eerily quiet", " silence"},
  {"' surroundings become eerily quiet", " silence"},
  {"begins to radiate toxic energy", "toxic"},
  {"Being near the torpor snail leaves you feeling lethargic", "Slow from snail"},
  {"curls into a ball and ", ""}, // boulder beetle
  {"smashes into", "smash"},      // boulder beetle
  {"stares at you", "stares"},    // ghost moth
  {"Grasping roots rise from the ground around the shambling mangrove!", "Roots."},
  {"puts on a burst of speed", "swift"}, // alligator
  {"swoops through the air toward", "swoop"}, // blue devil
  // Invis
  {"You hear some strange, mumbled words", "It cast"},
  {"feel a bit more experienced.", "kill it!"},
  {"There is a strange disturbance", "it"},
  {"flickers out of sight", "invis"},
  {"flickers and vanishes for a moment", "invis but see"},
  {"flickers and vanishes", "invis"},
  {"appears from out of your range of vision", "from far away"},
  {"appears from out of thin air!", "from it!"},
  {"appears from thin air", "comes here"},
  // Autopickup
  {"; reactivate with ", ""},
  {"Deactivating autopickup",  "Autopickup off"},
  {"Reactivating autopickup.", "Autopickup on."},
  {"Autopickup is now on",  "Autopickup on"},
  {"Autopickup is now off", "Autopickup off"},
  // Submerging
  {"forms itself from the air", "comes back"},
  {"leaps out from its hiding place under the floor", "comes back"},
  {"flickers into view", "comes back"},
  {"Something invisible bursts forth from the water", "It comes back"},
  {"merges itself into the air", "hides"},
  {"hides itself under the floor", "hides"},
  {"submerges", "hides"},
  {"There is a strange disturbance in", "hidden monster in"},
  // Hand/Feet names
  {"^front leg", "hand"},
  {"^front trotter", "hand"},
  {"^scythe-like blade", "hand"},
  {"blade paw",       "hand"},
  {"blade claw",      "hand"},
  {"blade tentacle",  "hand"},
  {"blade front paw", "hand"},
  {"^paw$",  "hand"},
  {"^paws$", "hands"},
//{"^branch$",   "hand"},
//{"^branches$", "hands"},
  {"foreclaw", "hand"},
  {"hyphae", "hands"},
  {"hypha", "hand"},
  {"misty tendril", "hand"},
  {"^your claw$",  "your hand"},
  {"^your claws$", "your hands"},
  {"^your tentacle$",  "your hand"},
  {"^your tentacles$", "your hands"},
  {"^in claw$",  "in hand"},
  {"^in tentacle$",  "in hand"},
  {"^which claw$",  "which hand"},
  {"^which tentacle$",  "which hand"},
  {"scaled arm", "arm"},
  {"feathered arm", "arm"},
  {"bandage-wrapped arm", "arm"},
  {"bony arm", "arm"},
  {"shadowy arm", "arm"},
  // Attack flavours
  {"explosion of spores engulfs", "spores"},
  {"space bends around", "disto"},
  {"space warps horribly around", "disto"},
  {"looks slightly unstable", "tele"},
  {"looks more stable", "no tele"},
  {"stumble backwards", "trample"},
  {"hold your ground!", "no trample."},
  {"holds his ground!", "no trample."},
  {"holds her ground!", "no trample."},
  {"holds its ground!", "no trample."},
  {"sudden explosion of sparks", "elec"},
//{"You are electrocuted", "elec"},
  {"draws strength from your injuries", "vamp"},
  {"You writhe in agony", "Pain"},
  {"writhes in agony!", "pain."},
  {"drains your vigour", "slows"},
  {"is devoured by a tear in reality", "banish"},
  {"corrodes your equipment", "corrodes"},
  {"^his grip$", "its grip"},
  {"^her grip$", "its grip"},
  {"loses its grip on",    "no grab"},
  {"releases its grip on", "no grab"},
  {"struggles to escape constriction", "no escape"},
  {"struggles to blink free from constriction", "blink no escape"},
  {" escapes!", " escape."}, //XXX check what other stuff matches this
  {"^constricts$", "squeeze"},
  // Item equip messages
//{"It bursts into flame!", ""},
//{"It is covered in frost.", ""},
//{"It glows with a cold blue light!", ""},
//{"It softly glows with a divine radiance!", ""},
//{"You hear the crackle of electricity.", ""},
//{"You see sparks fly.", ""},
  {"You feel protected!", ""},
  {"You feel nimbler!", ""},
  {"It begins to drip with poison!", "venom."},
//{"You sense an unholy aura.", ""},
  {"Your hands tingle!", "speed."},
  {"You feel a dreadful hunger.", "vamp."},
  {"You feel an empty sense of dread.", "vamp."},
  {"Space warps around you for a moment!", "disto."},
  {"A searing pain shoots up your arm!",  "pain."},
  {"A searing pain shoots up your hand!", "pain."},
  {"is briefly surrounded by a scintillating aura of random colours", "chaos"},
//{"Its appearance distorts for a moment.", ""},
//{"It is briefly surrounded by shifting shadows.", ""},
  // Item unequip messages //TODO
  {" unwields ", " no "},
  {" wields ", " "},
  {" stops flaming.", " unwield."},
  {" stops glowing.", " unwield."},
  {" stops crackling.", " unwield."},
  {" stops dripping with poison.", " unwield."},
  // Shorten ranged attacks
  {" throws$", ""},
  {" throw$", ""},
  {" shoots out$", ""},
  {" shoots$", ""},
  {" shoot$", ""},
  // Shorten beam names etc.
  {"zaps a wand", "zap"},
  {"bolt of flame",  "flame bolt"},
  {"bolts of flame", "flame bolts"},
  {"bolt of frost",  "frost bolt"},
  {"bolts of frost", "frost bolts"},
  {"bolt of penetration",  "penet bolt"},
  {"bolts of penetration", "penet bolts"},
  {"of penetration", "penet"},
  {"bolt of ", ""},
  {"puff of ", ""},
  {"blast of ", ""},
  {"plume of ", ""},
  {"shard of ", ""},
  {"splash of ", ""},
  {"trail of ", ""},
  {"orb of ", "", LETTERS, UPPER}, // orb of fire
  {"lance of force", "lance"},
  {"poisoned arrow$",  "arrow poison"},
  {"poisoned bolt$",   "bolt poison"},
  {"poisoned bullet$", "bullet poison"},
  {"poisoned stone$",  "stone poison"},
  {"poisoned javelin$",  "javelin poison"},
  //XXX Misc
  {"A demon appears!", "Ok."}, // hi.
  {"Your summoned ally is left behind",    "sum friend gone"},
  {"Your summoned allies are left behind", "sum friends gone"},
  {"Your mindless thrall stays behind", "zombie friend not"},
  {"Your mindless thralls stay behind", "zombie friends not"},
  {"A red film seems to cover your vision as you go berserk!", "Rage."},
  {"You feel as though you will be slow longer", "Slow"},
  {"You are already as slow as you could be", "Max Slow"},
  {"You feel sick", "hurt"},
  {"You feel very sick", "hurt"},
  {"You feel extremely sick.", "Hurt!"},
  {"You feel ill", "ill"},
  {"You are poisoned", "Pois"},
  {"You are more poisoned.", "Pois!"},
  {"You are no longer poisoned", "No Pois"},
  {"You feel your health improve", "No Sick"},
  {"You feel drained", "Drain"},
  {"Your life force feels restored", "No Drain"},
  {"The barbs in your skin will harm you if you move", "Barbs hurt"},
  {"The barbed spikes dig painfully into your body as you move", "Hurt"},
  {"You carefully extract the manticore spikes from your body", "No Barbs"},
  {"You feel momentarily confused", "Res Conf"},
  {"Your divine stamina protects you from confusion!", "Res Conf."},
  {"You feel your power leaking away", "Anti-magic"},
  {"You have lost your strength", "Statdeath Str"},
  {"You have lost your intelligence$", "Statdeath Int"},
  {"You have lost your dexterity$", "Statdeath Dex"},
  // Player enchantments add
  {"You feel agile all of a sudden",      "+Agi"},
  {"You feel more agile all of a sudden", "+Agi"},
  {"You feel agile", "+Agi"},
  {"You feel stronger", "+Str"},
  {"You feel clever", "+Int"},
  {"A red film seems to cover your vision as you go berserk", "Rage"},
  {"You feel yourself moving faster!", "Fast."}, // from Berserk
  {"You feel yourself speed up!", "Fast."}, // from Berserk
  {"You feel as though your hastened speed will last longer.", "Fast!"},
  {"You feel clever all of a sudden",      "+Int"},
  {"You feel more clever all of a sudden", "+Int"},
  {"You choke on the stench", "Meph"},
  {"You are confused",       "Conf"},
  {"You are more confused.", "Conf!"},
  {" to glow brighter", " to glow red"},
  {"s begin to glow red", " begins to glow red"},
//{" begin to glow red", " begins to glow red"}, // remove when ru bug is fixed
  {"Your hand begins to glow red", "Touch"},
  {"You feel your translocation being delayed",     "Delayed Tele"},
  {"You feel the orb delaying this translocation!", "Delayed Tele."},
  {"You feel in control", "cTele"},
  {"You feel your control is inadequate", "No cTele"},
  {"You are outlined in light", "Corona"},
  {"You glow brighter", "Corona"},
  {"You feel strangely conspicuous", "Corona"}, // reduced duration
  {"Malign forces permeate your being, awaiting release", "DChan"},
  {"Zin grants you divine stamina", "Vit"},
  {"Elyvilon grants you divine vigour", "Vigour"},
  {"You are exhausted", "Exh"},
  {"You suddenly feel exhausted", "Exh"}, // wrath
  {"The air around you leaps into flame!", "RoF."},
  {"A film of ice covers your body!", "Ozo."},
  {"Your icy armour thickens", "Ozo"},
  {"Your icy body feels more resilient", "Ozo"}, // ice beast
  {"You are covered in liquid flames", "Sticky"},
  {"Your magical defenses are stripped away", "MR/2"},
  {"A magical shield forms in front of you", "Helm"},
  {"You feel very mighty all of a sudden", "Might"},
  {"You feel mightier all of a sudden",    "Might"},
  {"You feel mighty",                      "Might"},
  {"You suddenly lose the ability to move",    "Para"},
  {"You still don't have the ability to move", "Para"},
  {"You lose consciousness",                   "Para"}, // hunger
  {"You pass out from exhaustion",             "Para"}, // berserk
  {"You suddenly pass out",                    "Para"}, // wrath
  {"You shrug off the repeated paralysis", "Max Para"},
  {"You are slowing down",      "Petr"},
  {"Your limbs are stiffening", "Petr"},
  {"Your limbs have turned to stone", "Stone"},
  {"You have turned to stone.",       "Stone!"},
  {"You feel protected.", "Resist."},
  {"You feel resistant.", "Resist."}, // from helm card
  {"A thick mucus forms on your$", "Slimify"},
  {"You fall asleep.", "Sleep!"},
  {"Your skin hardens",                    "Skin"},
  {"Your skin feels harder",               "Skin"},
  {"Your stone body feels more resilient", "Skin"}, // statue
  {"You feel quick$", "Swift"},
  {"You feel telepathic", "Emp"},
  {"You feel strangely unstable", "Tele"},
  {"Your surroundings flicker for a moment",    "Tele none"},
  {"Your surroundings seem slightly different", "Tele short"},
  {"Your surroundings suddenly seem different", "Tele done"},
  {"You sense a powerful magical force warping space", "No cTele"},
  {"You stand defiantly in death's doorway", "DDoor"},
  {"You seem to hear sand running through an hourglass...", "Hurry!"},
  {"You feel the strange sensation of being on two planes at once", "Shift"},
  {"You feel the material plane grow further away",                 "Shift"},
  {"A profound silence engulfs you",   "Sil"},
  {"An unnatural silence engulfs you", "Sil"}, // miscast
  {"You feel stealthy",     "Stlth"},
  {"You feel more catlike", "Stlth"},
  {"You are terrified of", "Fear from"},
  {"Another wave of unholy energy enters you", "Mirror"},
  {"You gain astral sight",        "Scry"},
  {"You extend your astral sight", "Scry"},
  {"A great vortex of raging winds appears around you",       "Tornado"},
  {"A great vortex of raging winds appears and lifts you up", "Tornado"},
  {"The ground around you becomes liquefied", "Leda"},
  {"You gain the combat prowess of a mighty hero",       "Hero"},
  {"You feel more confident with your borrowed prowess", "Hero"},
  {"You can now deal lightning-fast blows", "Finesse"},
  {"It gets dark",         "Dark"},
  {"It gets a bit darker", "Dark"},
  {"Space distorts slightly along a thin shroud covering your body", "Shroud"},
  {"You renew your shroud",                                          "Shroud"},
  {"A sentinel's mark forms upon you", "Mark"},
  {"The mark upon you grows brighter", "Mark"},
  {"You begin infusing your attacks with magical energy", "Infus"},
  {"You extend your infusion's duration",                 "Infus"},
  {"You start singing a song of slaying", "Slay"},
  {"You start a new song!",               "Slay."},
  {"Terrible wounds open up all over your body",      "Flay"},
  {"Terrible wounds spread across more of your body", "Flay"},
  {"You feel your attacks grow feeble",          "Weak"},
  {"You feel as though you will be weak longer", "Weak"},
  {"You feel firmly anchored in space",      "Anchor"},
  {"You feel more firmly anchored in space", "Anchor"},
  {"You begin to radiate toxic energy",      "Toxic"},
  {"Your toxic radiance grows in intensity", "Toxic"},
  {"You clear your throat and prepare to recite", "Recite"},
  {"Roots grasp at your feet, making movement difficult", "Roots"}, //TODO feet
  {"The grasping roots prevent you from becoming airborne", "Roots stop fly"},
  {"The barbed spikes become lodged in your body", "Barbs"},
  {"You feel yourself grow more vulnerable to poison", "-rP"},
  {"You are encased in ice", "Frozen"},
  {"Your magic feels tainted",      "Taint"},
  {"Your magic feels more tainted", "Taint"},
  {"You begin teleporting projectiles to their destination", "PProj"},
  {"You renew your portal",                                  "PProj"},
  {"A forested plane collides here with a resounding crunch", "Forest"},
  {"You call out to the draconic realm, and the dragon horde roars back", "Dragoncall"},
  {"A dragon arrives to answer your call!", "Dragon comes."},
  {"You begin to abjure the creatures around you!", "Abj."},
  {"You extend your aura of abjuration",            "Abj"},
  {"You feel threatened and lose the ability to drink potions", "-Potion"},
  {"You feel threatened and lose the ability to read scrolls",  "-Scroll"},
  {"The acid corrodes your equipment", "Corr"},
  {"You are filled with a great fortitude", "Fort"},
  {"You become more resolute",              "Fort"},
  {"Monsters! ", ""},
  {"Monsters everywhere! ", ""},
  {"You have to get out of here!",                        "Horror."},
  {"You reel with horror at the sight of these foes!",    "Horror."},
  {"You feel a twist of horror at the sight of this foe", "Horror"},
  {"A crackling disc of dense vapour forms in the air!", "CShield."},
  {"The disc of vapour around you crackles some more",   "CShield"},
  {"A divine shield forms around you!", "Shield."},
  {"Your divine shield is renewed",     "Shield"},
  {"Your shield is strengthened by", "Shield boost by"},
  {"You fade into invisibility!",        "Invis."},
  {"You fade further into invisibility", "Invis"},
  {"You feel more conspicuous",          "Invis"}, // expired corona backlight
  {"You fade completely from view now that you are no longer glowing from magical contamination", "Invis"}, // expired glow backlight
  {"You feel yourself slow down",                "Slow"},
  {"You feel as though you will be slow longer", "Slow"},
  {"You are mesmerised by", "Mesm from"},
  {"Visions of slaying", "Go to"},
  {" flood into your mind", ""},
  {"You feel yourself speed up",                              "Fast"},
  {"You feel as though your hastened speed will last longer", "Fast"},
  {"You already have as much speed as you can handle",        "Max Fast"},
  {"You feel very buoyant",                   "Fly"},
  {"You feel more buoyant",                   "Fly"},
  {"The vortex of raging winds lifts you up", "Fly"}, // tornado
  {"You fly up into the air",                 "Fly"}, // spell
  {"You swoop lightly up into the air",       "Fly"}, // tengu
  {"Your tail turns into legs as you fly out of the water", "Fly"}, // merfolk
  {"The poison in your body grows stronger", "Virulence"},
  {"You become one with your weapon", "Blade"},
  {"Your bond becomes stronger",      "Blade"},
  {"Your skin crawls", "Regen"},
  {"You feel yourself growing ill in the presence of$", "Sick from"},
  {"You feel resistant to hostile enchantments", "MR"},
  {"Your control over your magic is sapped", "Sap"},
  {"Your lungs strain for air", "Engulf"},
  {"You are caught in a web", "Held"},
  {"You become entangled in the net", "Held"},
  {"caught in a web", "held"},
  {"caught in the net", "held"},
  {"slice into the net.", "slice net!"},
  {"tear a large gash into the net.", "slice net!"},
  {"struggle against the net", "slice net"},
  // Forms
  {"You extend your transformation's duration", "Form extend"},
  {"You fail to extend your transformation any further", "Form max"},
  {"Your transformation is almost over", "Form expire"},
  {"Your transformation has ended", "No Form"},
  {"You turn into your old self",   "No Form"},
  {"melds into your body", "meld"},
  {"meld into your body",  "meld"},
  {"unmelds from your body", "unmeld"},
  {"falls away!", "unequip."},
  {"fall away!",  "unequip."},
  {"is pushed off your body!", "unequip."},
  {"You turn into a venomous arachnid creature", "Spiderform"},
  {"You turn into a living statue of rough stone",          "Statueform"},
  {"Your body stiffens and grows slower",                   "Statueform"},
  {"You inwardly fear your resemblance to a lawn ornament", "Statueform"},
  {"You revert to a slightly less stony form", "No Statueform"},
  {"You revert to your normal fleshy form",    "No Statueform"},
  {"You turn into a creature of crystalline ice", "Iceform"},
  {"You warm up again", "No Iceform"},
  {"You turn into a fearsome dragon!", "Dragonform."},
  {"Your body is suffused with negative energy!", "Lichform."},
  {"You feel yourself come back to life",    "No Lichform"},
  {"You feel your undeath return to normal", "No Lichform"},
  {"You turn into a bat",         "Batform"},
  {"You turn into a vampire bat", "Batform"},
  {"You turn into a filthy swine", "Pig"},
  {"You grow a pair of large bovine horns", "App Horns"},
  {"One of your hands grows a vicious spike", "App Spike"},
  {"Your feet morph into talons", "App Talons"},
  {"Your horns disappear", "No App"},
  {"Your horns shrink",    "No App"},
  {"Your hand spike disappears", "No App"},
  {"Your talons disappear", "No App"},
  {"Your talons shrink",    "No App"},
  {"You turn into a tree", "Tree"},
  {"You feel less woody", "No Tree"},
  {"You turn into a spiny porcupine", "Porcupine"},
  {"You turn into an insubstantial wisp", "Wisp"},
  {"You turn into a sentient fungus", "Fungus"},
  {"You stop sporulating", "No Fungus"},
  {"You turn into a swirling mass of dark shadows", "Shadowform"},
  {"You emerge from the shadows", "No Shadowform"},
  {"You feel less shadowy",       "No Shadowform"},
  {"You turn into a", "Now"},
  // Player enchantments remove
  {"Your icy envelope is restored", "Icemail"},
  {"Your icy armour starts to melt", "Icemail expire"},
  {"Your icy armour evaporates", "No Icemail"},
  {"Your hearing returns", "No Sil"},
  {"Your skin is crawling a little less now", "Regen expire"},
  {"Your skin stops crawling", "No Regen"},
  {"You stop regenerating", "No Regen"}, // lichform
  {"Your divine shield starts to fade", "Shield expire"},
  {"Your divine shield fades away", "No Shield"},
  {"Your divine shield disappears", "No Shield"},
  {"You have got your breath back", "Breath"},
  {"You start to feel a little slower", "Swift expire"},
  {"You feel sluggish", "Bad Swift"},
  {"You start to feel a little faster", "Bad Swift expire"},
  {"You no longer feel sluggish", "No Bad Swift"},
  {"Your slowness suddenly goes away", "No Slow"}, // stasis
  {"You start to feel less resistant", "Resist expire"},
  {"Your resistance to elements expires", "No Resist"},
  {"You feel closer to the material plane", "Shift expire"},
  {"You are firmly grounded in the material plane once more", "No Shift"},
  {"You feel less regenerative", "No Regen+"},
  {"You feel less empathic", "No Emp"},
  {"Your icy shield starts to melt", "CShield expire"},
  {"Your icy shield evaporates", "No CShield"},
  {"Your magical shield disappears", "No Helm"},
  {"Your skin feels tender", "No Skin"},
  {"You start to feel a little uncertain", "cTele expire"},
  {"You feel uncertain", "No +cTele"},
  {"Your unholy channel is weakening", "DChan expire"},
  {"Your unholy channel expires", "No DChan"},
  {"You feel less stealthy", "No Stlth"},
  {"You feel less confused", "No Conf"},
  {"Your confusion fades away as the thirst for blood takes over your mind", "No Conf"}, // guess what
  {"You feel less vulnerable to hostile enchantments", "No MR/2"},
  {"Your slime is starting to congeal", "Slimify expire"},
  {"You feel less slimy", "No Slimify"},
  {"You wake up", "No Sleep"},
  {"QUAD DAMAGE IS WEARING OFF", "Quad expire"},
  {"Your dark mirror aura disappears", "No Mirror"},
  {"You feel like a meek peon again", "No Hero"},
  {"Your hands slow down",           "No Finesse"},
  {"You suddenly lose your finesse", "No Finesse"}, // stasis
  {"Your finesse ends abruptly",     "No Finesse"}, // berserk
//{"s stop glowing$", " stop glowing"}, // remove when ru bug is fixed
//{"Your hand stop glowing",  "No Touch"}, // remove when ru bug is fixed
  {"Your hand stops glowing", "No Touch"},
  {"Your hands stop glowing", "No Touch"},
  {"The bond with your blade fades away", "No Blade"},
  {"Space becomes stable", "No Forest"},
  {"You are no longer entranced", "No Mesm"},
  {"You break out of your daze!", "No Mesm."},
  {"Your fear fades away", "No Fear"},
  {"The ice encasing you melts away", "No Frozen"},
  {"You feel strangely stable",    "No Tele"},
  {"Your teleport is interrupted", "No Tele"}, // dimension anchor
  {"You sense the disappearance of a powerful magical force which warped space", "Can cTele"},
  {"You sense the disappearance of a powerful magical force which warped space", "Can cTele"},
  {"You can drink potions again", "No -Potion"},
  {"You can read scrolls again", "No -Scroll"},
  {"You feel less exhausted", "No Exh"},
  {"Your extra speed is starting to run out", "Haste expire"},
//{"You feel yourself slow down", "No Haste"},
  {"The ground is no longer liquid beneath you", "No Leda"},
  {"Your fortitude fades away", "No Fort"},
  {"You feel a little less mighty now", "No Might"},
  {"You feel a little less agile now",  "No Agi"},
  {"You feel a little less clever now", "No Brill"},
  {"You can move again", "No Para"},
  {"You turn to flesh and can move again", "No Stone"},
  {"You turn to bone and can move again",  "No Stone"},
  {"You turn to wood and can move again",  "No Stone"},
  {"You turn to ice and can move again",   "No Stone"},
  {"You feel a strong urge to attack something.", "Rage expire."},
  {"You feel your anger subside.", "Rage expire!"},
  {"Your blood rage is quickly leaving you.", "Rage expire!!"},
  {"You are no longer berserk", "No Rage"},
  {"Your blood-deprived body can't sustain your rage any longer", "No Rage"}, // vp
  {"You violently calm down", "No Rage"}, // stasis
  {"You are no longer glowing", "No Corona"},
  {"The translocation energy dissipates", "No Disjoin"},
  {"The winds around you start to calm down", "Tornado expire"},
  {"The winds around you calm down", "No Tornado"},
  {"You are starting to lose your buoyancy", "Fly expire"},
  {"You feel your flight won't last long",   "Fly expire"},
  {"You float gracefully downwards", "No Fly"},
  {"You feel your flesh rotting away", "Rot"},
  {"Your time is quickly running out", "DDoor expire"},
  {"Your life is in your own hands again!", "No DDoor! Exh."},
  {"Your divine stamina fades away", "No Vit"},
  {"Your divine vigour fades away", "No Vigour"},
  {"^settles down", "no Slide"},
  {"Your astral sight fades away", "No Scry"},
  {"Your divine protection fades away", "No Prot"},
  {"The ambient light returns to normal", "No Dark"},
  {"The divine light dispels your darkness", "No Dark"},
  {"Your shroud begins to fray at the edges", "Shroud expire"},
  {"Your shroud unravels", "No Shroud"},
  {"Your magical infusion is running out", "Infus expire"},
  {"You are no longer magically infusing your attacks", "No Infus"},
  {"Your song is almost over", "Slay expire"},
  {"Your song has ended",                 "No Slay"},
  {"You stop singing",                    "No Slay"}, // stop singing ability
  {"The silence causes your song to end", "No Slay"}, // silence
  {"The sentinel's mark upon you fades away", "No Mark"},
  {"Your attacks no longer feel as feeble", "No Weak"},
  {"You are no longer firmly anchored in space", "No Anchor"},
  {"Your magic seems less tainted", "No Taint"},
  {"You repair your equipment", "No Corr"},
  {"You feel more in control of your magic", "No Sap"},
  {"You regain control over your magic", "No -Mag"},
  {"You gasp with relief as air once again reaches your lungs", "No Engulf"},
  {"The water engulfing you falls away",                        "No Engulf"},
  {"You deftly slip free of the water engulfing you",           "No Engulf"},
  {"With effort, you pull free of the water engulfing you",     "No Engulf"},
  {"The terrible wounds on your body vanish", "No Flay"},
  {"Your toxic aura wanes", "No Toxic"},
  {"You finish reciting", "No Recite"},
  {"You feel short of breath",      "No Breath"},
  {"You have difficulty breathing", "No Breath"}, // curare
  {"You escape the reach of the grasping roots",     "No Roots"},
  {"The grasping roots settle back into the ground", "No Roots"},
  {"You finish recalling your allies", "No Recall"},
  {"You feel less vulnerable to fire", "No -rF"},
  {"You feel less vulnerable to poison", "No -rP"},
  {"You are no longer teleporting projectiles to their destination", "No PProj"},
  {"The roar of the dragon horde subsides", "No Dragoncall"},
  {"You can once more reach out to the dragon horde", "Dragons"},
  {"Your aura of abjuration expires", "No Abj"},
  {"Your protection from fire is fading", "rF+ expire"},
  {"You feel less protected from fire", "No rF+"},
  {"Your resistance to fire fades away", "No rF+"},
  {"Your protection from cold is fading", "rC+ expire"},
  {"You feel less protected from cold", "No rC+"},
  {"Your resistance to cold fades away", "No rC+"},
  {"Your protection from electricity is fading", "rElec expire"},
  {"You feel less protected from electricity", "No rElec"},
  {"Your resistance to electricity fades away", "No rElec"},
  {"Your protection from physical attacks is fading", "AC+ expire"},
  {"You feel less protected from physical attacks", "No AC+"},
  {"Your resistance to physical attacks fades away", "No AC+"},
  {"You flicker for a moment", "Invis expire"},
  {"You flicker back into view", "No Invis"},
  // Monster enchantments
  {"unusually strong", "might"},
  {"unusually agile", "agi"},
  {"seems to grow stronger", "might"},
  {"suddenly seems more agile", "agi"},
  {"seems to speed up", "fast"},
  {"moving quickly", "fast"},
  {"moving somewhat quickly", "swift"},
  {"seems to slow down", "slow"},
  {"moving slowly", "slow"},
  {"shakes off his lethargy", "no slow"},
  {"shakes off her lethargy", "no slow"},
  {"shakes off its lethargy", "no slow"},
  {"bewildered and confused", "conf"},
  {"appears confused", "conf"},
  {"looks rather confused", "conf"},
  {"looks rather more confused", "conf"},
  {"seems less confused", "no conf"},
  {"looks more healthy", "conf"},
  {"stops burning", "no sticky"},
  {"goes berserk", "rage"},
  {"berserk$", "rage"},
  {"paralysed", "para"},
  {"suddenly stops moving", "para"},
  {"looks drowsy...", "sleep."},
  {"falls asleep!", "sleep."},
  {"slowly petrifying", "petr"},
  {"moving more slowly", "petr"}, // ???
  {"turns to stone!", "petr."},
  {"petrified", "stone"},
  {"stops moving altogether", "stone"},
  {"softly glowing", "corona"},
  {"is outlined in light", "corona"},
  {"stops glowing", "no corona"},
  {"glows brighter for a moment", "max corona"},
  {"glows brighter", "corona"},
  {"frenzied and insane", "insane"},
  {"consumed by blood-lust", "frenzy"},
  {"inspired to greatness", "roused"},
  {"slightly transparent", "invis"},
  {"covered in liquid flames", "sticky"},
  {"entangled in a ", ""},
  {"covering ground quickly", "swift"},
  {"covering ground slowly", "slow move"},
  {"standing in death's doorway", "ddoor"},
  {"covered in terrible wounds", "flay"},
  {", constricting", ", grab"},
  {" (constricting", " (grab"},
  {", constricted by", ", squeezed"},
  {" (constricted by", " (squeezed"},
  {", bleeding", ""},
  {" (bleeding)", ""},
  {"begins to bleed from his wounds!", "bleed."},
  {"begins to bleed from her wounds!", "bleed."},
  {"begins to bleed from its wounds!", "bleed."},
  {"friendly", "friend"},
  {"durably summoned", "perm sum"},
  {"summoned", "sum"},
  {"catching his breath", "breath"},
  {"catching her breath", "breath"},
  {"catching its breath", "breath"},
  {"sheltered from injuries", "shelter"},
  {"misshapen and mutated", "mutated"},
  // Player contam
  {"You feel more contaminated with magical energies", "Contam more"},
  {"You feel less contaminated with magical energies", "Contam less"},
  {"Your magical contamination has completely faded away", "No Contam"},
  {"You are very lightly contaminated with residual magic", "Contam 1"},
  {"You are contaminated with residual magics", "Contam 2"},
  {"You are heavily infused with residual magics", "Contam 3"},
  {"You are practically glowing with residual magics", "Contam 4"},
  {"Your entire body has taken on an eerie glow", "Contam 5"},
  {"You are engulfed in a nimbus of crackling magics", "Contam 9000"},
  // Hunger
  {"^less full$", "more hungry"},
  {"^less thirsty$", "less hungry"},
  {"You feel a little less hungry",    "less hungry"}, // jiyva
  {"You feel slightly less hungry",    "less hungry"},
  {"You feel somewhat less hungry",    "less hungry"},
  {"You feel quite a bit less hungry", "less hungry"},
  {"You feel a lot less hungry",       "less hungry"},
  {"You feel slightly more hungry",    "hunger easy"},
  {"You feel somewhat more hungry",    "hunger ok"},
  {"You feel quite a bit more hungry", "hunger tough"},
  {"You feel a lot more hungry.",      "hunger tough!"},
  // Use items
  {"Take off which item", "Remove"},
  {"Put on which piece of jewellery", "Wear"},
  {"Remove which piece of jewellery", "Remove"},
  {"^Use which ability", "Ability"},
  {" which item?", "?"},
  {" which spell?", "?"},
  {"As you read", "Read"},
  {", it crumbles to dust.", "."},
  {"It sticks to your hand!", "Cursed."},
  {"Oops, that feels deathly cold", "Cursed"},
  {"You feel as if something is helping you", "Remove curse"},
  {"You assume a fearsome visage", "Fear"},
  {"You hear a loud clanging noise", "Noise"},
  {"The scroll dissolves into smoke", "Fog"},
  {"The creatures around you are filled with an inner flame!", "Immolation."},
  {"Magic quickly surges around you", "MR/2"},
  {"Your magical effects are unravelling", "Cancel"},
  {"...but the box appears empty, and falls apart", "Poof"},
  {"...and something leaps out!", "things."},
  {"You reach into the bag...", "Spiderbag."},
  {"...but the bag is empty, and unravels at your touch", "Poof"},
  {"...and things crawl out!", "things."},
  {"The disc erupts in an explosion of electricity", "Stormdisc"},
  {"With no energy to recover, the crystal ball of energy is presently useless to you", "MP already full"},
  {"You gaze into the crystal ball", "Crystal ball"},
  // Cantrips TODO gastronok
  {"glows brightly for a moment", "cantrip"},
  {"looks braver", "cantrip"},
  {"becomes somewhat translucent", "cantrip"},
  {"'s eyes start to glow", " cantrip"},
  {"' eyes start to glow", " cantrip"},
  {"shimmers for a moment", "cantrip"},
  {"feel troubled.", "cantrip."},
  {"feel a wave of unholy energy pass over you", "cantrip"},
  // Feature names
  {"staircase leading out of the dungeon", "stairs out"},
  {"stone staircase leading", "stairs"},
  {"stone staircases leading", "stairs"},
  {"escape hatch in the ceiling", "uphatch"},
  {"escape hatch in the floor", "downhatch"},
  {"escape hatches in the ceiling", "uphatches"},
  {"escape hatches in the floor", "downhatches"},
  {"leading back out of this place", "out"},
  {"empty arch of ancient stone", "arch"},
  {"empty arches of ancient stone", "arches"},
  {"unnaturally hard rock wall", "border"},
  {"rock wall",  "wall"},
  {"stone wall", "wall"},
  {"metal wall", "wall"},
  {"wall of green crystal", "wall"},
  {"slime covered wall", "slimy wall"},
  {"translucent", "glass"},
  {"large door", "door"},
  {"large runed door", "runed door"},
  {"huge gate", "gate"},
  {"huge open gate", "open gate"},
  {"huge closed gate", "closed gate"},
  {"runed gate", "runed door"},
  {"large sealed door", "sealed door"},
  {"open gate", "open door"},
  {"closed gate", "closed door"},
  {"granite statue", "statue"},
  {"fountain of clear blue water", "floor"},
  {"some shallow water", "shallow water"},
  {"some deep water", "deep water"},
  {"some lava", "lava"},
  // Cloud names
  {"surrounded by raging winds", "tornado"},
  {"cloud of magical condensation", "useless cloud"},
  {"cloud of translocational energy", "useless cloud"},
  {"cloud of freezing vapour", "freezing cloud"},
  {"cloud of raging winds", "tornado"},
  {"cloud of scalding steam", "steam"},
  {"cloud of sparse dust", "dust"},
  {"cloud of rain", "rain"},
  {"seething ", ""},
  {"freezing vapour", "frost"},
  {"poison gas", "poison"},
  {"calcifying", "calc"},
  {"scalding steam", "steam"},
  {"raging winds", "tornado"},
  {"sparse dust", "dust"},
  {"stinking cloud", "meph"},
  {"noxious fumes",  "meph"},
  {"noxious blast",  "meph"},
  {"Noxious gasses pour", "meph pours"},
  // Exploration: monsters
  {" come into view$",  ""},
  {" comes into view$", ""},
  {"^he is ",  ""},
  {"^she is ", ""},
  {"^it is ",  ""},
  {"^he has ",  ""},
  {"^she has ", ""},
  {"^it has ",  ""},
  {"^a weapon of ",  ""},
  {"moves out of view", "far away"},
  {"move out of view",  "far away"},
  {"You sense a monster nearby", "it nearby"},
  {"Marking area around", "Exclusion"},
  {" as unsafe for travelling.", "."},
  // Exploration: items, features
  {"Found it on ", "From "},
  {"Found ", ""},
  {"(gained ", "(+"},
  {"The hatch slams shut behind you.", "Go down"},
  {"A mysterious force pulls you upwards.", "Go up"},
  {"Found a gateway leading out of the", "Gate out of"},
  {"You enter the deep water", "Deep"},
  {"You fall into the deep water", "Deep"},
  {"You enter the shallow water.", ""},
  {"You fall into the shallow water.", ""},
  {"Moving in this stuff is going to be slow.", "Shallow"}, //XXX
  {"...and don't expect to remain undetected", "no Invis"},
  {"You reach down and ", "You "},
  {"As you open$",  "You open"},
  {"As you close$", "You close"},
  {"You open the door", "Open door"},
  {"You open the gate", "Open door"},
  {"You open the runed door", "Open runed door"},
  {"You close the door", "Close door"},
  {"You close the gate", "Close door"},
  {", it creaks loudly", ". door"},
  {"You climb downwards", "Go down"},
  {"You climb upwards", "Go up"},
  {"You fly downwards", "Go down"},
  {"You fly upwards", "Go up"},
  {"You now have ", ""},
  {"Partly explored, can't reach some places", "Blocked exploring"},
  {"Now all you have to do is get back out of the dungeon", "Orbrun"},
  {"You open the lid...", "Beastbox."},
//{"pack is full", ""}, ////TODO
  // Examine surroundings
  {"Things that are here", "Here"},
  {"Items here", "Here"},
  {"You can't see that place.", "Far away."},
  {"an open door here", "in door"},
  {"a open door here",  "in door"},
  {"This square", "here"},
  {", spattered with blood", ""},
  {"haloed", "halo"},
  {"covered in magical glow", "glow"},
  {"wreathed by an umbra", "umbra"},
  {"wreathed by umbra", "umbra"},
  {"You see ", ""},
  {" here$", ""},
  {"There is something else lying underneath", "Things"},
  // Examine monster
  {"hasn't noticed you", "unaware"},
  {"fire blocked by", "but"},
  {"wielding ", ""},
  {"wearing ", ""},
  {"carrying ", ""},
  {"quivering ", ""},
  {"magical condensation, ", ""},
  {" (magical condensation)", ""},
  {"translocational energy, ", ""},
  {" (translocational energy)", ""},
  // No effect
  {"Nothing appears to happen", "No effect"},
  {"You feel momentarily disoriented", "No effect"},
  {"You feel blessed for a moment", "No effect"},
  {"You feel forgetful for a moment", "No effect"},
  {"The air around you briefly surges with heat, but it dissipates", "No effect"},
  {"You are unaffected", "No effect"},
  {"...but nothing happens", "No effect"},
  {"completely resists", "no effect"},
  {"is unaffected", "no effect"},
  // Prompts
  {"Enter note:", "Note:"},
  {"Are you sure you want to abandon this character and quit the game", "Quit"},
  {"Are you sure you want to stumble around while confused and next to ", "???"},
  {"Really renounce your faith, foregoing its fabulous benefits", "Leave god"},
  {"Are you sure you won't change your mind later", "Really"},
  {"There are friendlies around, are you sure you want to hurt them", "Hurt friends"},
  {"Really walk onto that", "Trigger"},
  {"Do you really want to", "Really"},
  // Interface messages
  {"Okay, then.", "Ok."},
  {"Unknown command.", "Ok."},
  {" game... please wait.", "."},
  {"You start resting", "Rest"},
  {"You start waiting", "Wait"},
  {"You are too injured to fight recklessly!", "Low HP. No Tab."},
  {"too close now for your liking", "nearby"},
  {"skill increases to", "now"},
  {"You have reached level", "Now XL"},
  {"Welcome back to level", "Now XL"},
  {"Your experience leads to an increase in your attributes!", "Stats."},
  {"All of the spellbooks you're carrying are beyond your current level of comprehension", "Hard Books"},
  {" for a list of commands and other information", " for help"},
  {"Can't find anything matching that", "No results"},
  {"There's nothing to open nearby", "No door"},
  {"There's nothing to close nearby", "No door"},
  {"There isn't anything that you can close there!", "No door."},
  {"glows black for a moment", "cursed"},
  {"has mastered the following spells", "Spells"},
  {"has mastered one of the following spellbooks", "Books"},
  {" magical abilities", " special abilities"},
  {" divine abilities",  " special abilities"},
  {"possesses the following special abilities", "Spells"},
  {"possesses one of the following sets of special abilities", "Books"},
  {"^He spells$",  "Spells"},
  {"^She spells$", "Spells"},
  {"^It spells$",  "Spells"},
  {"^He books$",  "Books"},
  {"^She books$", "Books"},
  {"^It books$",  "Books"},
  // Targeters
  {"Out of range", "far away"},
  {"That is beyond the maximum range", "far away"},
  // Food
  {"Eating a$", "eat"},
  {"Eating an$", "eat"},
  {"Eating one of", "eat"},
  {"You finish butchering the", "Butcher"},
  {"This raw flesh tastes terrible.",        "Ok."},
  {"This raw flesh tastes unpleasant.",      "Ok."},
  {"This raw flesh is not very appetising.", "Ok."},
  {"This raw flesh tastes good.",            "Ok."},
  {"This raw flesh tastes very good.",       "Ok."},
  {"This raw flesh tastes great.",           "Ok."},
  {"This raw flesh tastes like chicken.", "This raw flesh tastes like Kenku."},
  // Gods
  // TODO these also match your own death message
//{"barely glow and disappear",   "no piety"}, // Zin
//{"barely glows and disappears", "no piety"},
//{"glow silver and disappear",   "piety"},
//{"glows silver and disappears", "piety"},
//{"glow blindingly silver and disappear",   "piety++"},
//{"glows blindingly silver and disappears", "piety++"},
  // TSO
  // Kiku
  // Yred
  // Veh
  {"corpse slowly burns to ash", "no piety"}, // Oka
  {"corpses slowly burn to ash", "no piety"},
  {"corpse is consumed by flame",   "piety"},
  {"corpses are consumed by flame", "piety"},
  {"corpse is consumed in a burst of flame",   "piety++"},
  {"corpses are consumed in a burst of flame", "piety++"},
  {"corpse disappears without a sign", "no piety"}, // Makh
  {"corpses disappear without a sign", "no piety"},
  {"corpse flares red and disappears", "piety"},
  {"corpses flare red and disappear",  "piety"},
//{"corpse flares blood-red and disappears", "piety++"},
//{"corpses flare blood-red and disappear",  "piety++"},
  // Sif
  {"corpse is slowly consumed by flames",   "no piety"}, // Trog
  {"corpses are slowly consumed by flames", "no piety"},
  {"corpse is consumed in a column of flame",   "piety"},
  {"corpses are consumed in a column of flame", "piety"},
//{"corpse is consumed in a roaring column of flame",   "piety++"},
//{"corpses are consumed in a roaring column of flame", "piety++"},
// Ely
  // Ely holy: TSO reclaims / gladly (piety)
  {"corpse disappears into the void", "no piety"}, // Lucy
  {"corpses disappear into the void", "no piety"},
  {"corpse is consumed by the void",   "piety"},
  {"corpses are consumed by the void", "piety"},
//{"corpse is voraciously consumed by the void",   "piety++"},
//{"corpses are voraciously consumed by the void", "piety++"},
// BOG
  // Jiy/Fed?
  // Chei
  // Ash
  // Dith
  // Gozag
  // Qaz
  // Ru
  // TODO corpses / piety indicators
  {"Something appears at your feet", "Gift"},
  {"Zin accepts your kill",            "ok"},
  {"Zin accepts your collateral kill", "ok"},
  {"The Shining One accepts your kill",            "ok"}, ////
  {"The Shining One accepts your collateral kill", "ok"}, ////
//{"Kikubaaqudgha accepts your kill", "ok"},
  {"Kikubaaqudgha accepts your slave's kill", "ok"},
  {"Yredelemnul accepts your kill",                         "ok"},
  {"Yredelemnul appreciates your killing of a holy being.", "ok!"},
  {"Yredelemnul accepts your slave's kill",                 "ok"},
  {"Vehumet accepts your kill", "ok"},
  {"Okawaru accepts your kill",      "ok"},
  {"Okawaru appreciates your kill.", "ok!"},
  {"Makhleb accepts your kill",            "ok"},
  {"Makhleb accepts your collateral kill", "ok"},
  {"Makhleb accepts your slave's kill",    "ok"},
  {"Trog accepts your kill",                        "ok"},
  {"Trog appreciates your killing of a magic user", "ok"},
  {"Trog accepts your collateral kill",             "ok"},
//{"Elyvilon accepts your kill", "ok"},
  {"Lugonu accepts your kill",            "ok"},
  {"Lugonu claims a new guest",           "ok"},
  {"Lugonu accepts your collateral kill", "ok"},
  {"Lugonu accepts your slave's kill",    "ok"},
  {"Beogh accepts your kill",                            "ok"},
  {"Beogh appreciates your killing of a heretic priest", "ok"},
  {"Beogh accepts your collateral kill",                 "ok"},
  {"Beogh accepts your slave's kill",                    "ok"},
  {"Fedhas accepts your kill", "ok"},
  {"Cheibriados appreciates the change of pace",             "ok"},
  {"Cheibriados thoroughly appreciates the change of pace.", "ok!"},
  {"Dithmenos accepts your kill",                               "ok"},
  {"Dithmenos appreciates your extinguishing a source of fire", "ok"},
  {"Qazlal accepts your kill",            "ok"},
  {"Qazlal accepts your collateral kill", "ok"},
  {"You feel a little guilty", "piety loss 1"},
  {"You feel guilty",          "piety loss 2-4"},
  {"You feel very guilty",     "piety loss 5-9"},
  {"You feel extremely guilty","piety loss 10+"},
  {"You will pay for your transgression, mortal", "Penance"},
  {"You have lost your religion", "Excommunicated"},
  {"does not appreciate desertion",           "Wrath"},
  {"does not appreciate desertion for chaos", "Wrath"},
  {"does not appreciate desertion for evil",  "Wrath"},
  {"Ashenzari warns you:", "Ash:"},
  {"You have a vision of a gate", "Ash: gate"},
  {"You have a vision of multiple gates", "Ash: gates"},
  {"emit a cloud of dark smoke", "emit smoke"},
  {"That shadow isn't sufficiently still", "Not immobile"}, ////
  {"protects you from harm", "lifesave"},
  {"It doesn't seem very happy.", "Hostile!"},
  {"You draw a card... ", "Draw "},
  {"You deal a card... ", "Draw "},
  {"is muted in retribution by your aura", "mute"},
  {"is muted in retribution by your will", "mute"},
  {"is para in retribution by your aura", "para"},
  {"is para in retribution by your will", "para"},
  {"is slowed in retribution by your aura", "slow"},
  {"is slowed in retribution by your will", "slow"},
  {"is blinded in retribution by your aura", "blind"},
  {"is blinded in retribution by your will", "blind"},
  {"is illuminated in retribution by your aura", "corona"},
  {"is illuminated in retribution by your will", "corona"},
  {"is stunned by your will and fails to attack", "nope"},
  {"begins to cast a spell, but is stunned by your will", "nope"},
  {"begins to pray, but is stunned by your will", "nope"},
  {"begins to attack, but is stunned by your will", "nope"},
  {" by your wave of power", ""},////
  {"is rendered silent by the truth", "mute"},
  {"is para by terrible understanding", "para"},
  {"slows down under the weight of truth", "slow"},
  {"You channel some magical energy", "Channel"},
  {"Trog's vigour flows through your veins", "Trog: no Para"},
  // check veh
  {"You call on the dead to rise...", "Animate dead."},
  {"You draw life from your surroundings", "Drain life"},
  {"You hear a choir sing", "Sanctuary"},
  {"You are suddenly bathed in radiance", "Sanctuary"},
  {"The sanctuary starts shrinking", "Sanctuary expire"},
  {"The sanctuary disappears", "No Sanctuary"},
  {"Zin revokes the gift of sanctuary", "No Sanctuary"},
  {"turns to flee the light!", "flee sanctuary."},
  // Items
  {"uncursed ", ""},
  // Simplify combat messages directed at player
  {"Something smites you", "Smite"},
  {" at you$", ""},
  {" against you$", ""},
  {" on you$", ""},
  {" to you$", ""},
  {" upon you$", ""},
  {" to defend him$", ""},
  {" to defend her$", ""},
  {" to defend it$", ""},
  {" while chanting", ""},
  // Summon deaths
//{"disappear in a puff of smoke",  "poof"},
//{"disappears in a puff of smoke", "poof"},
  {"disappear in a smoke",  "poof"}, // puff of
  {"disappears in a smoke", "poof"},
  {"degenerate into a cloud of primal chaos",  "poof"},
  {"degenerates into a cloud of primal chaos", "poof"},
  {"dissolve into shadows",  "poof"},
  {"dissolves into shadows", "poof"},
  {"dissolve into sparkling lights",  "poof"},
  {"dissolves into sparkling lights", "poof"},
  {"dissolve into a puddle of slime",  "poof"},
  {"dissolves into a puddle of slime", "poof"},
  {"fade away",  "poof"},
  {"fades away", "poof"},
  {"return to the deep",  "poof"},
  {"returns to the deep", "poof"},
  {"shimmers and vanishes", "poof"},
  {"That felt strangely unrewarding", "No xp"},
  // Numbers (and hydra heads)
  {"^one$",   "1"},
  {"^two$",   "2"},
  {"^three$", "3"},
  {"^four$",  "4"},
  {"^five$",  "5"},
  {"^six$",   "6"},
  {"^seven$", "7"},
  {"^eight$", "8"},
  {"^nine$",  "9"},
  {"^ten$",  "10"},
  // Simplify messages
  {"You block$", "Block"},
  {"You open$", "Hit"},
  {"You slice$", "Hit"},
  {"completely misses$", "miss"}, // world attacks
  {"closely misses$",    "miss"},
  {"barely misses$",     "miss"},
  {"^misses$",           "miss"},
  {"engulfed in", "in"},
  {"negative energy", "drain"},
  {"living lightning", "lightning"},
  {"'s attack", ""},
  {"' attack", ""},
  {"'s", ""},
  {"You hear ", ""},
  {"There is ", ""},
  {"There are ", ""},
  {" dies$", " dead"},
  {" blown up$", " dead"},
  {"seems to ", ""},
  {"looks ", ""},
  {"seems ", ""},
  {"is no longer", "no"},
  {"^you are ", ""},
  {"your ", ""},
  {" you$", ""},
  {"you ", ""},
//{" of ", " "},
  {" is ", " "},
  {" are ", " "},
  {" and", ",", ",;—"},
  {" with$", ""},
  {" very ", " v."},
  {"∞", "immune"},//
  {"^a -", "7777777777777777777777777777777777777777"}, //XXX hack
  {"^a ", ""},
  {"^7777777777777777777777777777777777777777", "a -"}, // reset hack
  {"^an ", ""},
  {"the ", "", LETTERS, UPPER},
  // Rightfully name something
  {"something you cannot see", "it"},
  {"something unseen", "it"},
  {"^something$", "it"},
  {0}
};

static void _replace_cap_variants(string &str,
                                  string a,
                                  string b,
                                  const char* not_after = 0,
                                  const char* not_before = 0)
{
    ASSERT(!a.empty());
    if (a[0] == '^')
    {
        a.erase(0, 1);
        ASSERT(!not_after);
        not_after = LETTERS;
    }

    ASSERT(!a.empty());
    if (a[a.length() - 1] == '$')
    {
        a.erase(a.length() - 1);
        ASSERT(!not_before);
        not_before = LETTERS;
    }

    ASSERT(!a.empty());

    bool yes_after = false;
    if (not_after && *not_after == '!')
        yes_after = true, not_after++;

    bool yes_before = false;
    if (not_before && *not_before == '!')
        yes_before = true, not_before++;

    for (int captype = 0; captype < 3; captype++)
    {
        string A;
        switch (captype)
        {
        case 0: A = lowercase(a); break;
        case 1: A = uppercase_first(a); break;
        case 2: A = uppercase(a); break;
        default: die("lolwut?");
        }

        size_t pos = 0;
        while ((pos = str.find(A, pos)) != string::npos)
        {
            if (not_after && pos > 0
                && (yes_after == !strchr(not_after, str[pos - 1])))
            {
                pos++;
                continue;
            }

            size_t epos = pos + A.length();
            if (not_before && epos < str.length()
                && (yes_before == !strchr(not_before, str[epos])))
            {
                pos++;
                continue;
            }

            string B;
            switch (captype)
            {
            case 0: B = lowercase(b); break;
            case 1: B = uppercase_first(b); break;
            case 2: B = uppercase(b); break;
            default: die("lolwut?");
            }

            str.erase(pos, A.length());
            str.insert(pos, B);
            pos += B.length();
        }
    }
}

static void _german(string &txt)
{
    /*
    The European Commission has just announced an agreement whereby English will
    be the official language of the European Union rather than German, which was
    the other possibility.

    As part of the negotiations with the Germans, the British Government
    conceded that English spelling had some room for improvement and has
    accepted a 5-year phase-in plan that would become known as "Euro-English".

    In the first year, "s" will replace the soft "c". Sertainly, this will make
    the sivil servants jump with joy.

    The hard "c" will be dropped in favour of "k". This should klear up
    konfusion, and keyboards kan have one less letter.
    */
    for (unsigned int i = 0; i < txt.length(); i++)
        if (txt[i] == 'c' || txt[i] == 'C')
        {
            switch (tolower(txt[i+1]))
            {
            case 'h': case 'z':
                break;
            case 'e': case 'i': case 'y':
                txt[i] = isalower(txt[i]) ? 's' : 'S';
                break;
            default:
                txt[i] = isalower(txt[i]) ? 'k' : 'K';
            }
        }
    /*
    There will be growing publik enthusiasm in the sekond year when the
    troublesome "ph" will be replaced with "f". This will make words like
    fotograf 20% shorter.
    */
    _replace_cap_variants(txt, "ph", "f");
    /*
    In the 3rd year, publik akseptanse of the new spelling kan be expekted to
    reach the stage where more komplikated changes are possible.

    Governments will enkourage the removal of double letters which have always
    ben a deterent to akurate speling.
    */
    for (int i = txt.length() - 2; i > 0; i--)
        if (txt[i] == txt[i + 1])
            txt.erase(i, 1);
    /*
    Also, al wil agre that the horibl mes of the silent "e" in the languag is
    disgrasful and it should go away.
    */
    for (int i = txt.length() - 2; i > 0; i--)
        if (txt[i]=='e' || txt[i]=='E')
        {
            if (i < 3)
                continue;
            if (isaalpha(txt[i + 1]))
                continue;
            if (txt[i-1]=='e' || txt[i-1]=='E')
                continue;
            if (!isaalpha(txt[i-1])
              || !isaalpha(txt[i-2])
              || !isaalpha(txt[i-3]))
            {
                continue;
            }
            txt.erase(i, 1);
        }
    /*
    By the 4th yer people wil be reseptiv to steps such as replasing "th" with
    "z" and "w" with "v".
    */
    _replace_cap_variants(txt, "th", "z");
    _replace_cap_variants(txt, "w", "v");
    /*
    During ze fifz yer, ze unesesary "o" kan be dropd from vords containing "ou"
    and after ziz fifz yer, ve vil hav a reil sensibl riten styl.
    */
    _replace_cap_variants(txt, "ou", "u");
    /*
    Zer vil be no mor trubl or difikultis and evrivun vil find it ezi tu
    understand ech oza. Ze drem of a united urop vil finali kum tru.

    Und efter ze fifz yer, ve vil al be speking German like zey vunted in ze
    forst plas.
    */
    // TODO :p
}

static const char* german[][4] =
{
  {"sh",    "sch"},
  {"^is$",  "ist"},
  {"an$",   "ann"},
  {"^and$", "und"},
  {"^one$", "ein"},
  {0}
};

static void _wide(string &txt)
{
    string out;

    for (char ch : txt)
    {
        if (ch == ' ')
            out += "　"; // U+3000 rather than U+FF00
        else if (ch > 32 && ch < 127)
        {
            char buf[4];
            int r = wctoutf8(buf, ch + 0xFF00 - 32);
            for (int j = 0; j < r; j++)
                out.push_back(buf[j]);
        }
        else
            out.push_back(ch);
    }

    txt = out;
}

static void _grunt(string &txt)
{
    static const char* exact_grunt[][2] =
    {
        {"battlesphere", "BATTLESPHERE"},
        {"Battlesphere", "BATTLESPHERE"},
        {"battlemage", "BATTLEMAGE"},
        {"Battlemage", "BATTLEMAGE"},
        {"Battle Magician", "BATTLEMAGE"},
        {"battleaxe", "BATTLEAXE"},
        {"book of Battle", "BATTLEBOOK"},
        {"Battlelust", "BATTLELUST"},
        {"Kill them all", "RIP AND TEAR"},
        {"accepts your kill", "roars: ANNIHILATED"},
        {"appreciates your killing of a heretic priest",
         "smash puny heretic"},
        {"appreciates your killing of a magic user",
         "smash puny caster"},
        {"appreciates your killing of a holy being",
         "smash puny angel"},
        {"is honoured by your kill", "screams: ANNIHILATED"},
        {"You pass out from exhaustion.", "POWER NAP!!!"},
        {"You die...", "rip"},
        {"LOW HITPOINT WARNING", "don't die"},
        {"Ouch! That really hurt!", "DANG"},
        {0}
    };

    const char* (*repl)[2] = exact_grunt;
    for (; **repl; repl++)
    {
        size_t pos = 0;
        string a = (*repl)[0];
        string b = (*repl)[1];
        while ((pos = txt.find(a, pos)) != string::npos)
        {
            txt.erase(pos, a.length());
            txt.insert(pos, b);
            pos += b.length();
        }
    }
}

/**
 * Greedily look for a token consisting of only letters, starting at the
 * given index into the given string.
 *
 * @return      The index into the string at which the token ends.
 *              For example, if passed ("bird bee", 5), return 8.
 *              If passed ("bird bee, 4"), return 4 (empty token)
 */
static size_t _token_starting(const string &str, size_t start)
{
    size_t end;
    for (end = start; end < str.size() && isaalpha(str[end]); ++end)
        ;
    return end;
}

/**
 * Is the given string token too boring to be replaced with 'butt'?
 *
 * @param token     The string in question. Should be lowercased.
 * @return          Whether the token should be skipped in buttification.
 */
static bool _too_boring_to_butt(const string &token)
{
    static const unordered_set<string> boring_words = {
        "a", "an", "the", // articles
        "and", "or", "of", "on", "in", "if", "into", "with", // etc
        // , "but" <- this is actually very funny to replace.
    };
    return boring_words.count(token);
}

/**
 * Return an appropriate string to replace the given token with.
 *
 * @param token     The token to be replaced.
 * @return          Presumably, some variant on 'butt'.
 */
static string _replacement_butt(const string &token)
{
    string butt = "butt";
    size_t size = token.size();
    if (isupper(token[0]))
        butt = uppercase_first(butt); // foo -> butt
    if (token[size - 1] == 's')
        butt += 's'; // Foos -> Butts
    else if (size > 2 && token.substr(size - 2, 2) == "ly") // adv?
        butt += "-ly"; // carefully -> butt-ly

    return butt;
}

/**
 * Reference https://code.google.com/p/buttbot/ for vision statement & details.
 *
 * @param str[in,out]   The string to be improved.
 */
static void _butt(string &str)
{
    // iter along the string, tokenizing & potentially replacing as we go.
    for (size_t start = 0; start < str.size(); ++start)
    {
        const size_t end = _token_starting(str, start);

        if (end == start) // empty token (non-alpha index)
            continue;

        const string token = str.substr(start, end - start);
        // butt sparingly.
        if (!one_chance_in(20) || _too_boring_to_butt(lowercase_string(token)))
        {
            start = end; // will increment again, past the token-ending char
            continue;
        }

        // here's where the magic happens!!!
        const string butt = _replacement_butt(token);
        str.erase(start, end - start);
        str.insert(start, butt);
        start += butt.size(); // will incr again, past the token-ending char
    }
}

static void _mist(string &txt)
{
    //TODO list:
    // - provide custom databases
    // - settings to mute all multiturn messages, visuals, etc from code
    static const char* exact_mist[][2] =
    {
      // Prompts and other annoying capitalization
      {"Ctrl-A", ""},
      {"Enter Wizard Command", "Wizmode"},
      {"Ouch! That really hurt!", "Huge Dmg."},
      {"Send 'em back where they came from", "Abjure"},
      {"You don't know any spells", "No spells"},
      {"You aren't carrying or standing over any spellbooks", "No books"},
      {"Sorry, you're not good enough to have a special ability", "No abilities"},
      {"You aren't wearing any", "No"},
      {"You aren't wearing",     "No"},
      {"You aren't carrying any", "No"},
      {"You aren't carrying",     "No"},
      {"You don't have any", "No"},
      {"You don't have",     "No"},
      {"The Sword of Cerebov burns away your fire resistance", "-rF"},
      {"You feel the power of the Abyss delaying your translocation", "Delayed Tele"},
      {"You are cast into the Abyss", "Banish"},
      {"You are cast deeper into the Abyss", "Banish"},
      {"You resist the pull of the Abyss", "Max Banish"},
      {"You are suddenly pulled into a different region of the Abyss!", "Tele done."},
      {"gateway leading deeper into the Abyss", "downhatch"},
      {"You plunge deeper into the Abyss", "Go down"},
      {"You open a gate to Pandemonium!", "Summon demon."},
      {"You beseech Elyvilon to protect your life", "Prot"},
      {"You feel the effects of Trog's Hand fading", "Regen expire"},
      {"You feel the power of Trog in you as your rage grows", "Trog: boost Rage"},
      {"You offer yourself to Yredelemnul, and fill with unholy energy", "Mirror"},
      {"is turned into a pillar of salt by the wrath of Zin!", "now salt."},
      {"Press: ? - help, v - describe*", ""}, // xv
      {"Press: ? - help, . - travel*", ""}, // xv
      {" (i - inventory. (,) - cycle)", ""}, // f
      {"Press: ? - help, Shift-Dir - *", ""}, // f
      {"Press: ? - help, Dir - *", ""}, // jump, leap, etc
      {" (- for none, * to show all)", ""}, // w
      {"Xom*", "Xom."},
      {"Welcome back, *", "Hi."},
      {"Welcome, ", "Hi. "},
      //TODO mute god welcome messages (database hijack code required)
    //{"O*", "O"},
      //   t - <shout>!
      {"Orders for allies: a - Attack new target.", ""},
      {"                   r - Retreat!             s - Stop attacking.", ""},
      {"                   w - Wait.           f - Follow me.", ""},
      {"w - Wait.           f - Follow me.", ""}, //// TODO why this no worky
    //{"                   ", ""},
      {" Anything else - Stay silent.", ""},
      // Enemy renames
      /*
      {"catoblepas", "catlobe"},
      {"demonspawn", "DS"},
      {"torpor snail", "torpedo"},
      {"inotaur", "imotaur"},
      */
      {"deep el", "el"},
      {"draconian", "dr"},
      {"ironbrand ", ""},
      {"ironheart ", ""},
      {"-headed hydra", "-hydra"},
      {"Brimstone Fiend", "brimstone fiend"},
      {"Ice Fiend", "ice fiend"},
      {"Shadow Fiend", "shadow fiend"},
      {"Hell Sentinel", "hell sentinel"},
      {"Executioner", "executioner"},
      {"Orb Guardian", "orb guardian"},
      {"Killer Klown", "killer klown"},
      {"Blork the orc", "Blork"},
      // soh / enchantress / tlh??
      // Item renames
      {"pieces of ", ""},
      {"piece of ",  ""},
      {"god gift", "gift"},
      {"pair of ", ""},
      {"chunks of flesh", "chunk"},
      {"chunk of flesh",  "chunk"},
      {"chunks of ", ""},
      {"chunk of ",  ""},
      {"slices of ", ""},
      {"slice of ",  ""},
      {"beef jerk", "jerk"},
      {" rations", ""},
      {" ration",  ""},
      {"gold pieces", "gold"},
      {"gold piece",  "gold"},
      {"large rock", "rock"},
      {"sling bullet", "bullet"},
      {"poisoned needle", "needle"},
      {"curare-tipped needle", "curare needle"},
      {"hunting sling", "sling"},
      {"berserk rage", "rage"},
      {"Orb of Zot", "Orb"},
      /*
      {"Lugonu", "Lucy"},
      {"Trog", "Lugafu the Hairy"},
      {"giant spiked club", "gsc"},
      {"executioner's axe", "exec axe"},
      //{"tomahawk", "dart"},
      {" armour", ""},
      {"scale mail", "scale"}, // not "ring"
      {"chain mail", "chain"},
      {"fruit", "choko"},
      {"of might", "of str"},
      {"of agility", "of dex"},
      {"of brilliance", "of int"},
      {"invisibility", "invis"},
      {"restore abilities", "restab"},
      {"lignification", "wood"},
      {"beneficial mutation", "benemut"},
      {"cure mutation", "cmut"},
      {"curing", "cure"},
      {"heal wounds", "heal"},
      {"flight", "fly"},
      {"flying", "fly"},
      {"identify", "id"},
      {"remove curse", "rc"},
      {"teleportation", "tele"},
      {"magic mapping", "map"},
      {"enchant armour", "EA"},
      {"enchant weapon", "EW"},
      {"brand weapon", "brand"},
      {"recharging", "charge"},
      {"blinking", "blink"},
      {"acquirement", "gift"},
      {"vulnerability", "vuln"},
      {"holy word", "holy"},
      {"potions of ", "!"},
      {"potion of ", "!"},
      {"scrolls of ", "?"},
      {"scroll of ", "?"},
      {"scrolls labeled ", "?"},
      {"scroll labeled ", "?"},
      {"poison resistance", "rPois"},
      {"fire resistance", "rF"},
      {"cold resistance", "rC"},
      {"magic resistance", "MR"},
      {"life protection", "rN"},
      {"positive energy", "rN"},
      {"protection from fire", "rF"},
      {"protection from cold", "rC"},
      {"protection from magic", "MR"},
      {"teleport control", "ctele"},
      {"of magical power", "(+MP)"},
      {"resist mutation", "rMut"},
      {"resist corrosion", "rCorr"},
      {"of freezing", "(freeze)"},
      {"of flaming", "(flame)"},
      {"of holy wrath", "(holy)"},
      {"of electrocution", "(elec)"},
      {"of returning", "(ret)"},
      {"of penetration", "(pen)"},
      {"of dispersal", "(disp)"},
      {"of protection", "(+AC)"},
      {"of ponderousness", "(bad)"},
      {"protection", "+AC"},
      */
      {" of holy wrath", " holy"},
      {" of electrocution", " elec"},
      {"vampiric", "vamp"},
//    {" of penetration", " penet"},
      {" of ponderousness", " bad"},
//    {" of flam", " flam"}, // puff of flame etc
//    {" of frost",    " freeze"},
      {" of freezing", " freeze"},
      {" of dispersal", " dispersal"},
      {" of returning", " return"},
      {0}
    };

    const char* (*repl)[2] = exact_mist;
    for (; **repl; repl++)
    {
        size_t pos = 0;
        bool erase_tail = false;
        string a = (*repl)[0];
        string b = (*repl)[1];
        if (*a.rbegin() == '*')
        {
            a.erase(a.length() - 1);
            erase_tail = true;
        }
        while ((pos = txt.find(a, pos)) != string::npos)
        {
            txt.erase(pos, a.length());
            txt.insert(pos, b);
            pos += b.length();
            if (erase_tail)
            {
                txt.resize(pos);
                return;
            }
        }
    }
}

void filter_lang(string &str)
{
    for (auto fake_lang : Options.fake_langs)
    {
        const char* (*repl)[4];

        switch (fake_lang)
        {
            case FLANG_DWARVEN:
                repl = dwarven;
                break;
            case FLANG_JAGERKIN:
                repl = jager;
                break;
            case FLANG_KRAUT:
                _german(str), repl = german;
                break;
            case FLANG_WIDE:
                return _wide(str);
            case FLANG_FUTHARK:
                repl = runes;
                break;
            case FLANG_GRUNT:
                _grunt(str); repl = grunt;
                break;
            case FLANG_BUTT:
                _butt(str);
                continue;
            case FLANG_MIST:
                _mist(str); repl = mist;
                break;
            default:
                continue;
        }

        for (; **repl; repl++)
            _replace_cap_variants(str, (*repl)[0], (*repl)[1], (*repl)[2], (*repl)[3]);
    }
}

string filtered_lang(string str)
{
    filter_lang(str);
    return str;
}
