What is this
------------
* new file [`util/create_pot`] (crawl-ref/source/util/create_pot.sh) to generate gettext message catalog templates
* massaging many strings into more translation-friendly ones
  - `mpr()` combined with ternary operator building are mostly split
  - Get rid of many `%s` that were used to build message content by substituting
    in verbs etc. - not all languages follow English grammar rules
  - Occasionally rewrite some pieces of code for easier string extraction
* finally toss gettext into the mix and translate all the things (if only)
  - Right now, `mpr` and `mprf` are translated unconditionally, but some strings
    are exempt from extraction.
  - Additionally, these are run through gettext as well:
    `_output_expiring_message` `_annotate_form_based`
    `_decrement_a_duration` `prompt_invent_item[s]`
    `notify_fail` `yesno` `yes_or_no`
    [lua/vaults: `crawl.mpr` `crawl.yesno` `crawl.yesnoquit` `message_at_spot`]
* exclusion file for strings to not be extracted: [exclude.pot] (crawl-ref/source/dat/po/exclude.pot)
* ngettext support (plural-aware translations) right now only exists for the
  newly-introduced `mprf_plural` but should be expanded on.


How to test?
------------
To test the pseudo-localization that is [shipped with this repo] (crawl-ref/source/dat/po/):
* Export `LC_ALL=en_US.UTF-8` unless already set
  (if you do not have that locale installed, generate it with locale-gen)
* Run ./crawl from `source/` dir

Else, if you want to see manual translations in-game, in `dat/po/`:
* Generate a translation: `man msginit`
* Pseudo-localize it: `podebug --rewrite unicode <file.po>`
* Compile the translation: `man msgfmt`
  (should be output to [`source/dat/lang/<lang>/LC_MESSAGES/messages.mo`] (crawl-ref/source/dat/lang/))

To test template generation yourself, run `./util/create_pot.sh` in `source/` dir.


List of TODO items
------------------

### Exclude from extraction
* All `mprf` to `MSGCH_ERROR` and `MSGCH_DIAGNOSTICS` (automatically, not via `exclude.pot`)
* dlua errors
* Vault messages:
  - ancient_champ errors
  - twisted.des `dpeg_entry_water_fire` veto message
  - trove debug messages
* Make sure character notes and dumps in general never end up translated

### Keywords and string extraction
* `snprintf` (`Really %s%s%s?` and friends also belong to String building)
* `cprintf` (several gui and message writes)
* `fprintf` (mostly stash.cc)
* `msgwin_get_line`: only valid use in notes.cc and then one in zotdef, rest `wiz-`
* extract both strings from `_decrement_a_duration`, not just one
* `mpr_en` and `mprf_en` for forcibly untranslated and unextracted messages
  (should allow getting rid of most remaining layout-building strings)
* Webtiles
* Individual issues:
  - directn `get_square_desc`
  - religion `_sacrifice_message`

### String building
* Mutation-data: `"Your thick and shaggy fur keeps you warm (AC %1$+d, %2$s)."`
  instead of hardcoding `(AC +3, rC+)` (`%s` part debatable, numbers not)
* Lua in general (troves, sprints, ...)
  - string replacements, needed before going to mpr! mprf interface?
    [Check with wesnoth, widelands]
  - automagic, autofight
  - pan.des who lives here
* `held_status`
* `mpr_comma_separated_list`
* Individual strings:
  - `You fall through a shaft%s!`

### Questions to be discussed
* What to do with titles, or the main HUD in general?
* godabil: "You can now" -> "You can" change (messaging)

The `zotdef_danger_msg` keyword was added to create_pot for sake of completeness.
I thoroughly hate zotdef and crawl would be vastly better off with it not existing.
Because of this, zotdef.cc is excluded from string extraction.
When and if somebody believes that adding `zotdef_danger_msg` and all zotdef messages
to the translation burden is a great idea, go ahead and remove it from the exclusion list.
I won't.
