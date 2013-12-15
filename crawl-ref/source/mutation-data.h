// mutation definitions:
// first  number  = probability (0 means it doesn't appear naturally
//                  unless you have some level of it already)
// second number  = maximum levels
// first  boolean = is mutation mostly bad?
// second boolean = is mutation physical, i.e. external only?
// third  boolean = is mutation suppressed when shapechanged?
// first  string  = what to show in '%'
// second strings = what to show in 'A'
// third  strings = message given when gaining the mutation
// fourth strings = message given when losing the mutation
// fifth  string  = wizard-mode name of mutation

{ MUT_TOUGH_SKIN,                     0,  3, false,  true,  true,
  no_("tough skin"),

  {no_("You have tough skin (AC +1)."),
   no_("You have very tough skin (AC +2)."),
   no_("You have extremely tough skin (AC +3).")},

  {no_("Your skin toughens."),
   no_("Your skin toughens."),
   no_("Your skin toughens.")},

  {no_("Your skin feels delicate."),
   no_("Your skin feels delicate."),
   no_("Your skin feels delicate.")},

  no_("tough skin")
},

{ MUT_STRONG,                         7, 2, false,  true, false,
  no_("strong"),

  {no_("Your muscles are strong. (Str +2)"),
   no_("Your muscles are very strong. (Str +4)"), ""},
  {"", "", ""},
  {"", "", ""},

  no_("strong")
},

{ MUT_CLEVER,                         7, 2, false,  true, false,
  no_("clever"),

  {no_("Your mind is acute. (Int +2)"),
   no_("Your mind is very acute. (Int +4)"), ""},
  {"", "", ""},
  {"", "", ""},

  no_("clever")
},

{ MUT_AGILE,                          7, 2, false,  true, false,
  no_("agile"),

  {no_("You are agile. (Dex +2)"),
   no_("You are very agile. (Dex +4)"), ""},
  {"", "", ""},
  {"", "", ""},

  no_("agile")
},

{ MUT_POISON_RESISTANCE,              4,  1, false, false,  true,
  no_("poison resistance"),

  {no_("Your system is resistant to poisons."), "", ""},
  {no_("You feel healthy."), "",  ""},
  {no_("You feel a little less healthy."), "", ""},

  no_("poison resistance")
},

{ MUT_CARNIVOROUS,                    5,  3, false, false, false,
  no_("carnivore"),

  {no_("Your digestive system is specialised to digest meat."),
   no_("Your digestive system is highly specialised to digest meat."),
   no_("You are carnivorous and can eat meat at any time.")},

  {no_("You hunger for flesh."),
   no_("You hunger for flesh."),
   no_("You hunger for flesh.")},

  {no_("You feel able to eat a more balanced diet."),
   no_("You feel able to eat a more balanced diet."),
   no_("You feel able to eat a more balanced diet.")},

  no_("carnivorous")
},

{ MUT_HERBIVOROUS,                    5,  3,  true, false, false,
  no_("herbivore"),

  {no_("You digest meat inefficiently."),
   no_("You digest meat very inefficiently."),
   no_("You are a herbivore.")},

  {no_("You hunger for vegetation."),
   no_("You hunger for vegetation."),
   no_("You hunger for vegetation.")},

  {no_("You feel able to eat a more balanced diet."),
   no_("You feel able to eat a more balanced diet."),
   no_("You feel able to eat a more balanced diet.")},

  no_("herbivorous")
},

{ MUT_HEAT_RESISTANCE,                4,  3, false, false,  true,
  no_("fire resistance"),

  {no_("Your flesh is heat resistant."),
   no_("Your flesh is very heat resistant."),
   no_("Your flesh is almost immune to the effects of heat.")},

  {no_("You feel a sudden chill."),
   no_("You feel a sudden chill."),
   no_("You feel a sudden chill.")},

  {no_("You feel hot for a moment."),
   no_("You feel hot for a moment."),
   no_("You feel hot for a moment.")},

  no_("heat resistance")
},

{ MUT_COLD_RESISTANCE,                4,  3, false, false,  true,
  no_("cold resistance"),

  {no_("Your flesh is cold resistant."),
   no_("Your flesh is very cold resistant."),
   no_("Your flesh is almost immune to the effects of cold.")},

  {no_("You feel hot for a moment."),
   no_("You feel hot for a moment."),
   no_("You feel hot for a moment.")},

  {no_("You feel a sudden chill."),
   no_("You feel a sudden chill."),
   no_("You feel a sudden chill.")},

  no_("cold resistance")
},

{ MUT_DEMONIC_GUARDIAN,               0,  3, false, false, false,
  no_("demonic guardian"),

  {no_("A weak demonic guardian rushes to your aid."),
   no_("A demonic guardian rushes to your aid."),
   no_("A powerful demonic guardian rushes to your aid.")},

  {no_("You feel the presence of a demonic guardian."),
   no_("Your guardian grows in power."),
   no_("Your guardian grows in power.")},

  {no_("Your demonic guardian is gone."),
   no_("Your demonic guardian is weakened."),
   no_("Your demonic guardian is weakened.")},

  no_("demonic guardian")
},

{ MUT_SHOCK_RESISTANCE,               2,  1, false, false,  true,
  no_("electricity resistance"),

  {no_("You are resistant to electric shocks."), "", ""},
  {no_("You feel insulated."), "", ""},
  {no_("You feel conductive."), "", ""},

  no_("shock resistance")
},

{ MUT_REGENERATION,                   3,  3, false, false, false,
  no_("regeneration"),

  {no_("Your natural rate of healing is unusually fast."),
   no_("You heal very quickly."),
   no_("You regenerate.")},

  {no_("You begin to heal more quickly."),
   no_("You begin to heal more quickly."),
   no_("You begin to regenerate.")},

  {no_("Your rate of healing slows."),
   no_("Your rate of healing slows."),
   no_("Your rate of healing slows.")},

  no_("regeneration")
},

{ MUT_SLOW_HEALING,                   3,  3,  true, false, false,
  no_("slow healing"),

  {no_("You heal slowly when monsters are visible."),
   no_("You do not heal naturally when monsters are visible."),
   no_("You do not heal naturally.")},

  {no_("Your natural healing is weakened."),
   no_("Your natural healing is weakened."),
   no_("You stop healing.")},

  {no_("Your natural healing is strengthened."),
   no_("Your natural healing is strengthened."),
   no_("Your natural healing is strengthened.")},

  no_("slow healing")
},

{ MUT_FAST_METABOLISM,               10,  3,  true, false, false,
  no_("fast metabolism"),

  {no_("You have a fast metabolism."),
   no_("You have a very fast metabolism."),
   no_("Your metabolism is lightning-fast.")},

  {no_("You feel a little hungry."),
   no_("You feel a little hungry."),
   no_("You feel a little hungry.")},

  {no_("Your metabolism slows."),
   no_("Your metabolism slows."),
   no_("Your metabolism slows.")},

  no_("fast metabolism")
},

{ MUT_SLOW_METABOLISM,                7,  2, false, false, false,
  no_("slow metabolism"),

  {no_("You have a slow metabolism."),
   no_("You need consume almost no food."),
   ""},

  {no_("Your metabolism slows."),
   no_("Your metabolism slows."),
   ""},

  {no_("You feel a little hungry."),
   no_("You feel a little hungry."),
   ""},

  no_("slow metabolism")
},

{ MUT_WEAK,                          8, 2,  true,  true, false,
  no_("weak"),
  {no_("You are weak. (Str -2)"),
   no_("You are very weak. (Str -4)"), ""},
  {"", "", ""},
  {"", "", ""},
  no_("weak")
},

{ MUT_DOPEY,                         8, 2,  true,  true, false,
  no_("dopey"),
  {no_("You are dopey. (Int -2)"),
   no_("You are very dopey. (Int -4)"), ""},
  {"", "", ""},
  {"", "", ""},
  no_("dopey"),
},

{ MUT_CLUMSY,                        8, 2,  true,  true, false,
  no_("clumsy"),
  {no_("You are clumsy. (Dex -2)"),
   no_("You are very clumsy. (Dex -4)"), ""},
  {"", "", ""},
  {"", "", ""},
  no_("clumsy")
},

#if TAG_MAJOR_VERSION == 34
{ MUT_TELEPORT_CONTROL,               0,  1, false, false, false,
  "teleport control",

  {"You can control translocations.", "", ""},
  {"You feel controlled.", "", ""},
  {"You feel random.", "", ""},

  "teleport control"
},
#endif

{ MUT_TELEPORT,                       3,  3,  true, false, false,
  no_("teleportitis"),

  {no_("Space occasionally distorts in your vicinity."),
   no_("Space sometimes distorts in your vicinity."),
   no_("Space frequently distorts in your vicinity.")},

  {no_("You feel weirdly uncertain."),
   no_("You feel even more weirdly uncertain."),
   no_("You feel even more weirdly uncertain.")},

  {no_("You feel stable."),
   no_("You feel stable."),
   no_("You feel stable.")},

  no_("teleport")
},

{ MUT_MAGIC_RESISTANCE,               5,  3, false, false, false,
  no_("magic resistance"),

  {no_("You are resistant to hostile enchantments."),
   no_("You are highly resistant to hostile enchantments."),
   no_("You are extremely resistant to the effects of hostile enchantments.")},

  {no_("You feel resistant to hostile enchantments."),
   no_("You feel more resistant to hostile enchantments."),
   no_("You feel almost impervious to the effects of hostile enchantments.")},

  {no_("You feel less resistant to hostile enchantments."),
   no_("You feel less resistant to hostile enchantments."),
   no_("You feel vulnerable to magic hostile enchantments.")},

  no_("magic resistance")
},

{ MUT_FAST,                           0,  3, false, false,  true,
  "speed",

  {"You cover ground quickly.",
   "You cover ground very quickly.",
   "You cover ground extremely quickly."},

  {"You feel quick.",
   "You feel quick.",
   "You feel quick."},

  {"You feel sluggish.",
   "You feel sluggish.",
   "You feel sluggish."},

  "fast"
},

{ MUT_SLOW,                           0,  3,  true, false,  true,
  "slowness",

  {"You cover ground slowly.",
   "You cover ground very slowly.",
   "You cover ground extremely slowly."},

  {"You feel sluggish.",
   "You feel sluggish.",
   "You feel sluggish."},

  {"You feel quick.",
   "You feel quick.",
   "You feel quick."},

  "slow"
},

{ MUT_ACUTE_VISION,                   2,  1, false, false, false,
  no_("see invisible"),

  {no_("You have supernaturally acute eyesight."), "", ""},

  {no_("Your vision sharpens."),
   no_("Your vision sharpens."),
   no_("Your vision sharpens.")},

  {no_("Your vision seems duller."),
   no_("Your vision seems duller."),
   no_("Your vision seems duller.")},

  no_("acute vision")
},

{ MUT_DEFORMED,                       8,  1,  true,  true,  true,
  no_("deformed body"),

  {no_("Armour fits poorly on your strangely shaped body."), "", ""},
  {no_("Your body twists and deforms."), "", ""},
  {no_("Your body's shape seems more normal."), "", ""},

  no_("deformed")
},

{ MUT_SPIT_POISON,                    8,  3, false, false, false,
  no_("spit poison"),

  {no_("You can spit poison."),
   no_("You can spit moderately strong poison."),
   no_("You can spit strong poison.")},

  {no_("There is a nasty taste in your mouth for a moment."),
   no_("There is a nasty taste in your mouth for a moment."),
   no_("There is a nasty taste in your mouth for a moment.")},

  {no_("You feel an ache in your throat."),
   no_("You feel an ache in your throat."),
   no_("You feel an ache in your throat.")},

  no_("spit poison")
},

{ MUT_BREATHE_FLAMES,                 4,  3, false, false, false,
  no_("breathe flames"),

  {no_("You can breathe flames."),
   no_("You can breathe fire."),
   no_("You can breathe blasts of fire.")},

  {no_("Your throat feels hot."),
   no_("Your throat feels hot."),
   no_("Your throat feels hot.")},

  {no_("A chill runs up and down your throat."),
   no_("A chill runs up and down your throat."),
   no_("A chill runs up and down your throat.")},

  no_("breathe flames")
},

{ MUT_JUMP,                 0,  3, false, false, false,
  no_("jump"),

  {no_("You can jump attack at a short distance."),
   no_("You can jump attack at a medium distance."),
   no_("You can jump attack at a long distance.")},

  {no_("You feel more sure on your feet."),
   no_("You feel more sure on your feet."),
   no_("You feel more sure on your feet.")},

  {no_("You feel less sure on your feet."),
   no_("You feel less sure on your feet."),
   no_("You feel less sure on your feet.")},

  no_("jump")
},

{ MUT_BLINK,                          3,  3, false, false, false,
  no_("blink"),

  {no_("You can translocate small distances at will."),
   no_("You are good at translocating small distances at will."),
   no_("You can easily translocate small distances at will.")},

  {no_("You feel jittery."),
   no_("You feel more jittery."),
   no_("You feel even more jittery.")},

  {no_("You feel a little less jittery."),
   no_("You feel less jittery."),
   no_("You feel less jittery.")},

  no_("blink")
},

#if TAG_MAJOR_VERSION == 34
{ MUT_STRONG_STIFF,                  0,  3, false,  true, false,
  "strong stiff",

  {"Your muscles are strong, but stiff (Str +1, Dex -1).",
   "Your muscles are very strong, but stiff (Str +2, Dex -2).",
   "Your muscles are extremely strong, but stiff (Str +3, Dex -3)."},

  {"Your muscles feel sore.",
   "Your muscles feel sore.",
   "Your muscles feel sore."},

  {"Your muscles feel loose.",
   "Your muscles feel loose.",
   "Your muscles feel loose."},

  "strong stiff"
},

{ MUT_FLEXIBLE_WEAK,                 0,  3, false,  true, false,
  "flexible weak",

  {"Your muscles are flexible, but weak (Str -1, Dex +1).",
   "Your muscles are very flexible, but weak (Str -2, Dex +2).",
   "Your muscles are extremely flexible, but weak (Str -3, Dex +3)."},

  {"Your muscles feel loose.",
   "Your muscles feel loose.",
   "Your muscles feel loose."},

  {"Your muscles feel sore.",
   "Your muscles feel sore.",
   "Your muscles feel sore."},

  "flexible weak"
},
#endif

{ MUT_SCREAM,                         6,  3,  true, false, false,
  no_("screaming"),

  {no_("You occasionally shout uncontrollably."),
   no_("You sometimes yell uncontrollably."),
   no_("You frequently scream uncontrollably.")},

  {no_("You feel the urge to shout."),
   no_("You feel a strong urge to yell."),
   no_("You feel a strong urge to scream.")},

  {no_("Your urge to shout disappears."),
   no_("Your urge to yell lessens."),
   no_("Your urge to scream lessens.")},

  no_("scream")
},

{ MUT_CLARITY,                        6,  1, false, false, false,
  no_("clarity"),

  {no_("You possess an exceptional clarity of mind."), "", ""},
  {no_("Your thoughts seem clearer."), "", ""},
  {no_("Your thinking seems confused."), "", ""},

  no_("clarity")
},

{ MUT_BERSERK,                        7,  3,  true, false, false,
  no_("berserk"),

  {no_("You tend to lose your temper in combat."),
   no_("You often lose your temper in combat."),
   no_("You have an uncontrollable temper.")},

  {no_("You feel a little pissed off."),
   no_("You feel angry."),
   no_("You feel extremely angry at everything!")},

  {no_("You feel a little more calm."),
   no_("You feel a little less angry."),
   no_("You feel a little less angry.")},

  no_("berserk")
},

{ MUT_DETERIORATION,                 10,  3,  true, false, false,
  no_("deterioration"),

  {no_("Your body is slowly deteriorating."),
   no_("Your body is deteriorating."),
   no_("Your body is rapidly deteriorating.")},

  {no_("You feel yourself wasting away."),
   no_("You feel yourself wasting away."),
   no_("You feel your body start to fall apart.")},

  {no_("You feel healthier."),
   no_("You feel a little healthier."),
   no_("You feel a little healthier.")},

  no_("deterioration")
},

{ MUT_BLURRY_VISION,                 10,  3,  true, false, false,
  no_("blurry vision"),

  {no_("Your vision is a little blurry."),
   no_("Your vision is quite blurry."),
   no_("Your vision is extremely blurry.")},

  {no_("Your vision blurs."),
   no_("Your vision blurs."),
   no_("Your vision blurs.")},

  {no_("Your vision sharpens."),
   no_("Your vision sharpens a little."),
   no_("Your vision sharpens a little.")},

  no_("blurry vision")
},

{ MUT_MUTATION_RESISTANCE,            4,  3, false, false, false,
  no_("mutation resistance"),

  {no_("You are somewhat resistant to further mutation."),
   no_("You are somewhat resistant to both further mutation and mutation removal."),
   no_("You are almost entirely resistant to further mutation and mutation removal.")},

  {no_("You feel genetically stable."),
   no_("You feel genetically stable."),
   no_("You feel genetically immutable.")},

  {no_("You feel genetically unstable."),
   no_("You feel genetically unstable."),
   no_("You feel genetically unstable.")},

  no_("mutation resistance")
},

{ MUT_EVOLUTION,                      4,  2, false, false, false,
  no_("evolution"),

  {no_("You evolve."),
   no_("You rapidly evolve."),
   ""},

  {no_("You feel nature experimenting on you. Don't worry, failures die fast."),
   no_("Your genes go into a fast flux."),
   ""},

  {no_("You feel genetically stable."),
   no_("Your wild genetic ride slows down."),
   ""},

  no_("evolution")
},

{ MUT_FRAIL,                         10,  3,  true,  true, false,
  no_("frail"),

  {no_("You are frail (-10% HP)."),
   no_("You are very frail (-20% HP)."),
   no_("You are extremely frail (-30% HP).")},

  {no_("You feel frail."),
   no_("You feel frail."),
   no_("You feel frail.")},

  {no_("You feel robust."),
   no_("You feel robust."),
   no_("You feel robust.")},

  no_("frail")
},

{ MUT_ROBUST,                         5,  3, false,  true, false,
  no_("robust"),

  {no_("You are robust (+10% HP)."),
   no_("You are very robust (+20% HP)."),
   no_("You are extremely robust (+30% HP).")},

  {no_("You feel robust."),
   no_("You feel robust."),
   no_("You feel robust.")},

  {no_("You feel frail."),
   no_("You feel frail."),
   no_("You feel frail.")},

  no_("robust")
},

{ MUT_UNBREATHING,                    0,  1, false, false,  true,
  no_("unbreathing"),

  {no_("You can survive without breathing."), "", ""},
  {no_("You feel breathless."), "", ""},
  {"", "", ""},

  no_("unbreathing")
},

{ MUT_TORMENT_RESISTANCE,             0,  1, false, false, false,
  no_("torment resistance"),

  {no_("You are immune to unholy pain and torment."), "", ""},
  {no_("You feel a strange anaesthesia."), "", ""},
  {"", "", ""},

  no_("torment resistance")
},

{ MUT_NEGATIVE_ENERGY_RESISTANCE,     0,  3, false, false, false,
  no_("life protection"),

  {no_("You resist negative energy."),
   no_("You are quite resistant to negative energy."),
   no_("You are immune to negative energy.")},

  {no_("You feel negative."),
   no_("You feel negative."),
   no_("You feel negative.")},

  {"", "", ""},

  no_("negative energy resistance")
},

{ MUT_HURL_HELLFIRE,                  0,  1, false, false, false,
  no_("hurl hellfire"),

  {no_("You can hurl blasts of hellfire."), "", ""},
  {no_("You smell fire and brimstone."), "", ""},
  {"", "", ""},

  no_("hurl hellfire")
},

// body-slot facets
{ MUT_HORNS,                          7,  3, false,  true,  true,
  no_("horns"),

  {no_("You have a pair of small horns on your head."),
   no_("You have a pair of horns on your head."),
   no_("You have a pair of large horns on your head.")},

  {no_("A pair of horns grows on your head!"),
   no_("The horns on your head grow some more."),
   no_("The horns on your head grow some more.")},

  {no_("The horns on your head shrink away."),
   no_("The horns on your head shrink a bit."),
   no_("The horns on your head shrink a bit.")},

  no_("horns")
},

{ MUT_BEAK,                           1,  1, false,  true,  true,
  no_("beak"),

  {no_("You have a beak for a mouth."), "", ""},
  {no_("Your mouth lengthens and hardens into a beak!"), "", ""},
  {no_("Your beak shortens and softens into a mouth."), "", ""},

  no_("beak")
},

{ MUT_CLAWS,                          2,  3, false,  true,  true,
  no_("claws"),

  {no_("You have sharp fingernails."),
   no_("You have very sharp fingernails."),
   no_("You have claws for hands.")},

  {no_("Your fingernails lengthen."),
   no_("Your fingernails sharpen."),
   no_("Your hands twist into claws.")},

  {no_("Your fingernails shrink to normal size."),
   no_("Your fingernails look duller."),
   no_("Your hands feel fleshier.")},

  no_("claws")
},

{ MUT_FANGS,                          1,  3, false,  true,  true,
  no_("fangs"),

  {no_("You have very sharp teeth."),
   no_("You have extremely sharp teeth."),
   no_("You have razor-sharp teeth.")},

  {no_("Your teeth lengthen and sharpen."),
   no_("Your teeth lengthen and sharpen some more."),
   no_("Your teeth are very long and razor-sharp.")},

  {no_("Your teeth shrink to normal size."),
   no_("Your teeth shrink and become duller."),
   no_("Your teeth shrink and become duller.")},

  no_("fangs")
},

{ MUT_HOOVES,                         5,  3, false,  true,  true,
  no_("hooves"),

  {no_("You have large cloven feet."),
   no_("You have hoof-like feet."),
   no_("You have hooves in place of feet.")},

  {no_("Your feet thicken and deform."),
   no_("Your feet thicken and deform."),
   no_("Your feet have mutated into hooves.")},

  {no_("Your hooves expand and flesh out into feet!"),
   no_("Your hooves look more like feet."),
   no_("Your hooves look more like feet.")},

  no_("hooves")
},

{ MUT_ANTENNAE,                       4,  3, false,  true,  true,
  no_("antennae"),

  {no_("You have a pair of small antennae on your head."),
   no_("You have a pair of antennae on your head."),
   no_("You have a pair of large antennae on your head (SInv).")},

  {no_("A pair of antennae grows on your head!"),
   no_("The antennae on your head grow some more."),
   no_("The antennae on your head grow some more.")},

  {no_("The antennae on your head shrink away."),
   no_("The antennae on your head shrink a bit."),
   no_("The antennae on your head shrink a bit.")},

  no_("antennae")
},

{ MUT_TALONS,                         5,  3, false,  true,  true,
  no_("talons"),

  {no_("You have sharp toenails."),
   no_("You have razor-sharp toenails."),
   no_("You have claws for feet.")},

  {no_("Your toenails lengthen and sharpen."),
   no_("Your toenails lengthen and sharpen."),
   no_("Your feet stretch into talons.")},

  {no_("Your talons dull and shrink into feet."),
   no_("Your talons look more like feet."),
   no_("Your talons look more like feet.")},

  no_("talons")
},

// Octopode only
{ MUT_TENTACLE_SPIKE,                10,  3, false,  true,  true,
  no_("spike"),

  {no_("One of your tentacles bears a spike."),
   no_("One of your tentacles bears a nasty spike."),
   no_("One of your tentacles bears a large vicious spike.")},

  {no_("One of your lower tentacles grows a sharp spike."),
   no_("Your tentacle spike grows bigger."),
   no_("Your tentacle spike grows even bigger.")},

  {no_("Your tentacle spike disappears."),
   no_("Your tentacle spike becomes smaller."),
   no_("Your tentacle spike recedes somewhat.")},

  no_("tentacle spike")
},

// Naga only
{ MUT_BREATHE_POISON,                 4,  1, false, false,  false,
  no_("breathe poison"),

  {no_("You can exhale a cloud of poison."), "", ""},
  {no_("You taste something nasty."), "", ""},
  {no_("Your breath is less nasty."), "", ""},

  no_("breathe poison")
},

// Naga and Draconian only
{ MUT_STINGER,                        8,  3, false,  true,  true,
  no_("stinger"),

  {no_("Your tail ends in a poisonous barb."),
   no_("Your tail ends in a sharp poisonous barb."),
   no_("Your tail ends in a wickedly sharp and poisonous barb.")},

  {no_("A poisonous barb forms on the end of your tail."),
   no_("The barb on your tail looks sharper."),
   no_("The barb on your tail looks very sharp.")},

  {no_("The barb on your tail disappears."),
   no_("The barb on your tail seems less sharp."),
   no_("The barb on your tail seems less sharp.")},

  no_("stinger")
},

// Draconian only
{ MUT_BIG_WINGS,                      4,  1, false,  true,  true,
  no_("large and strong wings"),

  {no_("Your wings are large and strong."), "", ""},
  {no_("Your wings grow larger and stronger."), "", ""},
  {no_("Your wings shrivel and weaken."), "", ""},

  no_("big wings")
},

// species-dependent innate mutations
{ MUT_SAPROVOROUS,                    0,  3, false, false, false,
  no_("saprovore"),

  {no_("You can tolerate rotten meat."),
   no_("You can eat rotten meat."),
   no_("You thrive on rotten meat.")},

  {no_("You hunger for rotting flesh."),
   no_("You hunger for rotting flesh."),
   no_("You hunger for rotting flesh.")},

  {"", "", ""},

  no_("saprovorous")
},

{ MUT_GOURMAND,                       0,  1, false, false, false,
  no_("gourmand"),

  {no_("You like to eat raw meat."), "", ""},
  {"", "", ""},
  {"", "", ""},

  no_("gourmand")
},

{ MUT_SHAGGY_FUR,                     2,  3, false,  true,  true,
  no_("shaggy fur"),

  {no_("You are covered in fur (AC +1)."),
   no_("You are covered in thick fur (AC +2)."),
   no_("Your thick and shaggy fur keeps you warm (AC +3, rC+).")},

  {no_("Fur sprouts all over your body."),
   no_("Your fur grows into a thick mane."),
   no_("Your thick fur grows shaggy and warm.")},

  {no_("You shed all your fur."),
   no_("Your thick fur recedes somewhat."),
   no_("Your shaggy fur recedes somewhat.")},

  no_("shaggy fur")
},

{ MUT_HIGH_MAGIC,                     2,  3, false, false, false,
  no_("high mp"),

  {no_("You have an increased reservoir of magic (+10% MP)."),
   no_("You have a considerably increased reservoir of magic (+20% MP)."),
   no_("You have a greatly increased reservoir of magic (+30% MP).")},

  {no_("You feel more energetic."),
   no_("You feel more energetic."),
   no_("You feel more energetic.")},

  {no_("You feel less energetic."),
   no_("You feel less energetic."),
   no_("You feel less energetic.")},

  no_("high mp")
},

{ MUT_LOW_MAGIC,                      9,  3,  true, false, false,
  no_("low mp"),

  {no_("Your magical capacity is low (-10% MP)."),
   no_("Your magical capacity is very low (-20% MP)."),
   no_("Your magical capacity is extremely low (-30% MP).")},

  {no_("You feel less energetic."),
   no_("You feel less energetic."),
   no_("You feel less energetic.")},

  {no_("You feel more energetic."),
   no_("You feel more energetic."),
   no_("You feel more energetic.")},

  no_("low mp")
},

{ MUT_WILD_MAGIC,                     6,  3, false, false, false,
  no_("wild magic"),

  {no_("Your spells are a little harder to cast, but a little more powerful."),
   no_("Your spells are harder to cast, but more powerful."),
   no_("Your spells are much harder to cast, but much more powerful.")},

  {no_("You feel less in control of your magic."),
   no_("You feel less in control of your magic."),
   no_("You feel your magical power running wild!")},

  {no_("You regain control of your magic."),
   no_("You feel more in control of your magic."),
   no_("You feel more in control of your magic.")},

  no_("wild magic")
},

{ MUT_STOCHASTIC_TORMENT_RESISTANCE,  0,  1, false, false, false,
  "50% torment resistance",

  {no_("You are somewhat able to resist unholy torments (1 in 2 success)."),"",""},
  {no_("You feel a strange anaesthesia."),"",""},
  {"","",""},

  no_("stochastic torment resistance")
},

{ MUT_PASSIVE_MAPPING,                3,  3, false, false, false,
  no_("sense surroundings"),

  {no_("You passively map a small area around you."),
   no_("You passively map the area around you."),
   no_("You passively map a large area around you.")},

  {no_("You feel a strange attunement to the structure of the dungeons."),
   no_("Your attunement to dungeon structure grows."),
   no_("Your attunement to dungeon structure grows further.")},

  {no_("You feel slightly disoriented."),
   no_("You feel slightly disoriented."),
   no_("You feel slightly disoriented.")},

  no_("passive mapping")
},

{ MUT_ICEMAIL,                        0,  1, false, false, false,
  no_("icemail"),

  {no_("A meltable icy envelope protects you from harm and freezing vapours (AC +"), "", ""},
  {no_("An icy envelope takes form around you."), "", ""},
  {"", "", ""},

  no_("icemail")
},

{ MUT_CONSERVE_SCROLLS,               0,  1, false, false, false,
  no_("conserve scrolls"),

  {no_("You are very good at protecting items from fire."), "", ""},
  {no_("You feel less concerned about heat."), "", ""},
  {"", "", ""},

  no_("conserve scrolls"),
},

{ MUT_CONSERVE_POTIONS,               0,  1, false, false, false,
  no_("conserve potions"),

  {no_("You are very good at protecting items from cold."), "", ""},
  {no_("You feel less concerned about cold."), "", ""},
  {"", "", ""},
  no_("conserve potions"),
},

{ MUT_PASSIVE_FREEZE,                 0,  1, false, false, false,
  no_("passive freeze"),

  {no_("A frigid envelope surrounds you and freezes all who hurt you."), "", ""},
  {no_("Your skin feels very cold."), "", ""},
  {"", "", ""},

  no_("passive freeze"),
},

{ MUT_NIGHTSTALKER,                   0,  3, false,  true, false,
  no_("nightstalker"),

  {no_("You are slightly more attuned to the shadows."),
   no_("You are significantly more attuned to the shadows."),
   no_("You are completely attuned to the shadows.")},

  {no_("You slip into the darkness of the dungeon."),
   no_("You slip further into the darkness."),
   no_("You are surrounded by darkness.")},

  {no_("Your affinity for the darkness vanishes."),
   no_("Your affinity for the darkness weakens."),
   no_("Your affinity for the darkness weakens.")},

  no_("nightstalker")
},

{ MUT_SPINY,                          0,  3, false,  true,  true,
  no_("spiny"),

  {no_("You are partially covered in sharp spines."),
   no_("You are mostly covered in sharp spines."),
   no_("You are completely covered in sharp spines.")},

  {no_("Sharp spines emerge from parts of your body."),
   no_("Sharp spines emerge from more of your body."),
   no_("Sharp spines emerge from your entire body.")},

  {no_("Your sharp spines disappear entirely."),
   no_("Your sharp spines retract somewhat."),
   no_("Your sharp spines retract somewhat.")},

  no_("spiny")
},

{ MUT_POWERED_BY_DEATH,               0,  3, false, false, false,
  no_("powered by death"),

  {no_("You can steal the life force of nearby defeated enemies."),
   no_("You can steal the life force of defeated enemies."),
   no_("You can steal the life force of all defeated enemies in sight.")},

  {no_("A wave of death washes over you."),
   no_("The wave of death grows in power."),
   no_("The wave of death grows in power.")},

  {no_("Your control of surrounding life forces is gone."),
   no_("Your control of surrounding life forces weakens."),
   no_("Your control of surrounding life forces weakens.")},

  no_("powered by death")
},

{ MUT_POWERED_BY_PAIN,                0,  3, false, false, false,
  no_("powered by pain"),

  {no_("You sometimes gain a little power by taking damage."),
   no_("You sometimes gain power by taking damage."),
   no_("You are powered by pain.")},

  {no_("You feel energised by your suffering."),
   no_("You feel even more energised by your suffering."),
   no_("You feel completely energised by your suffering.")},

  {"", "", ""},

  no_("powered by pain")
},

{ MUT_AUGMENTATION,                   0,  3, false, false, false,
  no_("augmentation"),

  {no_("Your magical and physical power is slightly enhanced at high health."),
   no_("Your magical and physical power is enhanced at high health."),
   no_("Your magical and physical power is greatly enhanced at high health.")},

  {no_("You feel power flowing into your body."),
   no_("You feel power rushing into your body."),
   no_("You feel saturated with power.")},

  {"", "", ""},

  no_("augmentation")
},

{ MUT_MANA_SHIELD,                    0,  1, false, false, false,
  no_("magic shield"),

  {no_("When hurt, damage is shared between your health and your magic reserves."), "", ""},
  {no_("You feel your magical essence form a protective shroud around your flesh."), "", ""},
  {"", "", ""},

  no_("magic shield")
},

{ MUT_MANA_REGENERATION,              0,  1, false, false, false,
  no_("magic regeneration"),

  {no_("You regenerate magic rapidly."), "", ""},
  {no_("You feel your magic shroud grow more resilient."), "", ""},
  {"", "", ""},

  no_("magic regeneration")
},

{ MUT_MANA_LINK,                      0,  1, false, false, false,
  no_("magic link"),

  {no_("When low on magic, you restore magic in place of health."), "", ""},
  {no_("You feel your life force and your magical essence meld."), "", ""},
  {"", "", ""},

  no_("magic link")
},

// Jiyva only mutations
{ MUT_GELATINOUS_BODY,                0,  3, false,  true,  true,
  no_("gelatinous body"),

  {no_("Your rubbery body absorbs attacks (AC +1)."),
   no_("Your pliable body absorbs attacks (AC +1, EV +1)."),
   no_("Your gelatinous body deflects attacks (AC +2, EV +2).")},

  {no_("Your body becomes stretchy."),
   no_("Your body becomes more malleable."),
   no_("Your body becomes viscous.")},

  {no_("Your body returns to its normal consistency."),
   no_("Your body becomes less malleable."),
   no_("Your body becomes less viscous.")},

  no_("gelatinous body")
},

{ MUT_EYEBALLS,                       0,  3, false,  true,  true,
  no_("eyeballs"),

  {no_("Your body is partially covered in golden eyeballs (Acc +3)."),
   no_("Your body is mostly covered in golden eyeballs (Acc +5)."),
   no_("Your body is completely covered in golden eyeballs (Acc +7, SInv).")},

  {no_("Eyeballs grow over part of your body."),
   no_("Eyeballs cover a large portion of your body."),
   no_("Eyeballs cover you completely.")},

  {no_("The eyeballs on your body disappear."),
   no_("The eyeballs on your body recede somewhat."),
   no_("The eyeballs on your body recede somewhat.")},

  no_("eyeballs")
},

{ MUT_TRANSLUCENT_SKIN,               0,  3, false,  true,  true,
  no_("translucent skin"),

  {no_("Your skin is partially translucent."),
   no_("Your skin is mostly translucent (Stlth)."),
   no_("Your transparent skin reduces the accuracy of your foes (Stlth).")},

  {no_("Your skin becomes partially translucent."),
   no_("Your skin becomes more translucent."),
   no_("Your skin becomes completely transparent.")},

  {no_("Your skin returns to its normal opacity."),
   no_("Your skin's translucency fades."),
   no_("Your skin's transparency fades.")},

  no_("translucent skin")
},

{ MUT_PSEUDOPODS,                     0,  3, false,  true,  true,
  no_("pseudopods"),

  {no_("Armour fits poorly on your pseudopods."),
   no_("Armour fits poorly on your large pseudopods."),
   no_("Armour fits poorly on your massive pseudopods.")},

  {no_("Pseudopods emerge from your body."),
   no_("Your pseudopods grow in size."),
   no_("Your pseudopods grow in size.")},

  {no_("Your pseudopods retract into your body."),
   no_("Your pseudopods become smaller."),
   no_("Your pseudopods become smaller.")},

  no_("pseudopods")
},

#if TAG_MAJOR_VERSION == 34
{ MUT_FOOD_JELLY,                     0,  1, false,  true, false,
  "spawn jellies when eating",

  {"You occasionally spawn a jelly by eating.", "", ""},
  {"You feel more connected to the slimes.", "", ""},
  {"Your connection to the slimes vanishes.", "", ""},

  "jelly spawner"
},
#endif

{ MUT_ACIDIC_BITE,                    0,  1, false,  true,  true,
  no_("acidic bite"),

  {no_("You have acidic saliva."), "", ""},
  {no_("Acid begins to drip from your mouth."), "", ""},
  {no_("Your mouth feels dry."), "", ""},

  no_("acidic bite")
},

// Scale mutations
{ MUT_DISTORTION_FIELD,               2,  3, false, false, false,
  no_("repulsion field"),

  {no_("You are surrounded by a mild repulsion field (EV +2)."),
   no_("You are surrounded by a moderate repulsion field (EV +3)."),
   no_("You are surrounded by a strong repulsion field (EV +4, rMsl).")},

  {no_("You begin to radiate repulsive energy."),
   no_("Your repulsive radiation grows stronger."),
   no_("Your repulsive radiation grows stronger.")},

  {no_("You feel less repulsive."),
   no_("You feel less repulsive."),
   no_("You feel less repulsive.")},

  no_("repulsion field")
},

{ MUT_ICY_BLUE_SCALES,                2,  3, false,  true,  true,
  no_("icy blue scales"),

  {no_("You are partially covered in icy blue scales (AC +1)."),
   no_("You are mostly covered in icy blue scales (AC +3, EV -1)."),
   no_("You are completely covered in icy blue scales (AC +4, EV -1, rC+).")},

  {no_("Icy blue scales grow over part of your body."),
   no_("Icy blue scales spread over more of your body."),
   no_("Icy blue scales cover your body completely.")},

  {no_("Your icy blue scales disappear."),
   no_("Your icy blue scales recede somewhat."),
   no_("Your icy blue scales recede somewhat.")},

  no_("icy blue scales")
},

{ MUT_IRIDESCENT_SCALES,              2,  3, false,  true,  true,
  no_("iridescent scales"),

  {no_("You are partially covered in iridescent scales (AC +4)."),
   no_("You are mostly covered in iridescent scales (AC +6)."),
   no_("You are completely covered in iridescent scales (AC +8).")},

  {no_("Iridescent scales grow over part of your body."),
   no_("Iridescent scales spread over more of your body."),
   no_("Iridescent scales cover you completely.")},

  {no_("Your iridescent scales disappear."),
   no_("Your iridescent scales recede somewhat."),
   no_("Your iridescent scales recede somewhat.")},

  no_("iridescent scales")
},

{ MUT_LARGE_BONE_PLATES,              2,  3, false,  true,  true,
  no_("large bone plates"),

  {no_("You are partially covered in large bone plates (AC +2, SH +2)."),
   no_("You are mostly covered in large bone plates (AC +3, SH +4)."),
   no_("You are completely covered in large bone plates (AC +4, SH +6).")},

  {no_("Large bone plates grow over parts of your arms."),
   no_("Large bone plates spread over more of your arms."),
   no_("Large bone plates cover your arms completely.")},

  {no_("Your large bone plates disappear."),
   no_("Your large bone plates recede somewhat."),
   no_("Your large bone plates recede somewhat.")},

  no_("large bone plates")
},

{ MUT_MOLTEN_SCALES,                  2,  3, false,  true,  true,
  no_("molten scales"),

  {no_("You are partially covered in molten scales (AC +1)."),
   no_("You are mostly covered in molten scales (AC +3, EV -1)."),
   no_("You are completely covered in molten scales (AC +4, EV -1, rF+).")},

  {no_("Molten scales grow over part of your body."),
   no_("Molten scales spread over more of your body."),
   no_("Molten scales cover your body completely.")},

  {no_("Your molten scales disappear."),
   no_("Your molten scales recede somewhat."),
   no_("Your molten scales recede somewhat.")},

  no_("molten scales")
},

{ MUT_ROUGH_BLACK_SCALES,             2,  3, false,  true,  true,
  no_("rough black scales"),

  {no_("You are partially covered in rough black scales (AC +4, Dex -1)."),
   no_("You are mostly covered in rough black scales (AC +7, Dex -2)."),
   no_("You are completely covered in rough black scales (AC +10, Dex -3).")},

  {no_("Rough black scales grow over part of your body."),
   no_("Rough black scales spread over more of your body."),
   no_("Rough black scales cover you completely.")},

  {no_("Your rough black scales disappear."),
   no_("Your rough black scales recede somewhat."),
   no_("Your rough black scales recede somewhat.")},

  no_("rough black scales")
},

{ MUT_RUGGED_BROWN_SCALES,            2,  3, false,  true,  true,
  no_("rugged brown scales"),

  {no_("You are partially covered in rugged brown scales (AC +1, +3% HP)."),
   no_("You are mostly covered in rugged brown scales (AC +2, +5% HP)."),
   no_("You are completely covered in rugged brown scales (AC +3, +7% HP).")},

  {no_("Rugged brown scales grow over part of your body."),
   no_("Rugged brown scales spread over more of your body."),
   no_("Rugged brown scales cover you completely.")},

  {no_("Your rugged brown scales disappear."),
   no_("Your rugged brown scales recede somewhat."),
   no_("Your rugged brown scales recede somewhat.")},

  no_("rugged brown scales")
},

{ MUT_SLIMY_GREEN_SCALES,             2,  3, false,  true,  true,
  no_("slimy green scales"),

  {no_("You are partially covered in slimy green scales (AC +2)."),
   no_("You are mostly covered in slimy green scales (AC +3)."),
   no_("You are completely covered in slimy green scales (AC +4, rPois).")},

  {no_("Slimy green scales grow over part of your body."),
   no_("Slimy green scales spread over more of your body."),
   no_("Slimy green scales cover your body completely.")},

  {no_("Your slimy green scales disappear."),
   no_("Your slimy green scales recede somewhat."),
   no_("Your slimy green scales recede somewhat.")},

  no_("slimy green scales")
},

{ MUT_THIN_METALLIC_SCALES,           2,  3, false,  true,  true,
  no_("thin metallic scales"),

  {no_("You are partially covered in thin metallic scales (AC +2)."),
   no_("You are mostly covered in thin metallic scales (AC +3)."),
   no_("You are completely covered in thin metallic scales (AC +4, rElec).")},

  {no_("Thin metallic scales grow over part of your body."),
   no_("Thin metallic scales spread over more of your body."),
   no_("Thin metallic scales cover your body completely.")},

  {no_("Your thin metallic scales disappear."),
   no_("Your thin metallic scales recede somewhat."),
   no_("Your thin metallic scales recede somewhat.")},

  no_("thin metallic scales")
},

{ MUT_THIN_SKELETAL_STRUCTURE,        2,  3, false,  true, false,
  no_("thin skeletal structure"),

  {no_("You have a somewhat thin skeletal structure (Dex +2, Stlth)."),
   no_("You have a moderately thin skeletal structure (Dex +4, Str -1, Stlth)."),
   no_("You have an unnaturally thin skeletal structure (Dex +6, Str -2, Stlth).")},

  {no_("Your bones become slightly less dense."),
   no_("Your bones become somewhat less dense."),
   no_("Your bones become less dense.")},

  {no_("Your skeletal structure returns to normal."),
   no_("Your skeletal structure densifies."),
   no_("Your skeletal structure densifies.")},

  no_("thin skeletal structure")
},

{ MUT_YELLOW_SCALES,                  2,  3, false,  true,  true,
  no_("yellow scales"),

  {no_("You are partially covered in yellow scales (AC +2)."),
   no_("You are mostly covered in yellow scales (AC +3)."),
   no_("You are completely covered in yellow scales (AC +4, rCorr).")},

  {no_("Yellow scales grow over part of your body."),
   no_("Yellow scales spread over more of your body."),
   no_("Yellow scales cover you completely.")},

  {no_("Your yellow scales disappear."),
   no_("Your yellow scales recede somewhat."),
   no_("Your yellow scales recede somewhat.")},

  no_("yellow scales")
},

{ MUT_CAMOUFLAGE,                     1,  3, false,  true,  true,
  no_("camouflage"),

  {no_("Your skin changes colour to match your surroundings (Stlth)."),
   no_("Your skin blends seamlessly with your surroundings (Stlth)."),
   no_("Your skin perfectly mimics your surroundings (Stlth).")},

  {no_("Your skin functions as natural camouflage."),
   no_("Your natural camouflage becomes more effective."),
   no_("Your natural camouflage becomes more effective.")},

  {no_("Your skin no longer functions as natural camouflage."),
   no_("Your natural camouflage becomes less effective."),
   no_("Your natural camouflage becomes less effective.")},

   no_("camouflage")
},

{ MUT_IGNITE_BLOOD,                   0,  1, false, false, false,
  no_("ignite blood"),

  {no_("Your demonic aura causes spilled blood to erupt in flames."), "", ""},
  {no_("Your blood runs red-hot!"), "", ""},
  {"", "", ""},

  no_("ignite blood")
},

{ MUT_FOUL_STENCH,                    0,  2, false, false, false,
  no_("foul stench"),

  {no_("You emit a foul stench, and are resistant to rotting and decay."),
   no_("You radiate miasma, and are resistant to rotting and decay."),
   ""},

  {no_("You begin to emit a foul stench of rot and decay."),
   no_("You begin to radiate miasma."),
   ""},

  {"", "", ""},

  no_("foul stench")
},

{ MUT_TENDRILS,                       0,  1, false,  true, true,
  no_("tendrils"),

  {no_("Thin tendrils of slime have grown from your body."), "", ""},
  {no_("Thin, slimy tendrils emerge from your body."), "", ""},
  {no_("Your tendrils retract into your body."), "", ""},

  no_("tendrils")
},

{ MUT_JELLY_GROWTH,                       0,  1, false,  true, true,
  no_("a jelly is attached to you"),

  {no_("You have a small jelly attached to you that senses nearby items."), "", ""},
  {no_("Your body partially splits into a small jelly."), "", ""},
  {no_("The jelly growth is reabsorbed into your body."), "", ""},

  no_("jelly growth")
},

{ MUT_JELLY_MISSILE,                       0,  1, false,  true, true,
  no_("absorbing missiles"),

  {no_("You have a small jelly attached to you that may absorb incoming projectiles."), "", ""},
  {no_("Your body partially splits into a small jelly."), "", ""},
  {no_("The jelly growth is reabsorbed into your body."), "", ""},

  no_("jelly missile")
},

{ MUT_PETRIFICATION_RESISTANCE,            0,  1, false, false, false,
  no_("petrification resistance"),

  {no_("You are immune to petrification."), "", ""},
  {no_("Your body vibrates."), "", ""},
  {no_("You briefly stop moving."), "", ""},

  no_("petrification resistance")
},

#if TAG_MAJOR_VERSION == 34
{ MUT_TRAMPLE_RESISTANCE,                  0,  1, false, false, false,
  "trample resistance",

  {"You are resistant to trampling.", "", ""},
  {"You feel steady.", "", ""},
  {"You feel unsteady.", "", ""},

  "trample resistance"
},

{ MUT_CLING,                               0,  1, false, false, true,
  "cling",

  {"You can cling to walls.", "", ""},
  {"You feel sticky.", "", ""},
  {"You feel slippery.", "", ""},

  "cling"
},
#endif

{ MUT_EXOSKELETON,                         0,  2, false,  true,  true,
  no_("exoskeleton"),

  {no_("Your body is surrounded by an exoskeleton. (HP +5, AC +1)"),
   no_("Your body is surrounded by a tough exoskeleton. (HP +20, AC +2)"),
   ""},

  {no_("Your exoskeleton hardens."),
   no_("Your exoskeleton becomes even harder."),
   ""},

  {no_("Your exoskeleton softens."),
   no_("Your exoskeleton softens."),
   ""},

  no_("exoskeleton")
},

{ MUT_FUMES,            0,  2, false, false, false,
  no_("fuming"),

  {no_("You emit clouds of smoke."),
   no_("You frequently emit clouds of smoke."),
   ""},

  {no_("You fume."),
   no_("You fume more."),
   ""},

  {no_("You stop fuming."),
   no_("You fume less."),
   ""},

  no_("fumes")
}
