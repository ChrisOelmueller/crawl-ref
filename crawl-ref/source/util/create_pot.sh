#!/bin/sh

set -e

#TODO Obtain this from anywhere.
VERSION=`git describe`
# Where to write the message catalog template.
OUTPUT_DIR=dat/po
OUTPUT_FILE=$OUTPUT_DIR/messages.pot
# Template containing strings to *not* extract.
EXCLUDE_FILE=$OUTPUT_DIR/exclude.pot
# Include comments starting with this keyword (and located either on the
# previous or the same line as the string that will be extracted) as help
# for translators. Useful to grasp string context, the meaning of format
# string placeholders, etc.
COMMENT_KEYWORD='i18n'

# As for files exclude, the strings presented there are mostly technical
# mprf() with not much more than format-string specs or simply better left
# untranslated (like dumps, which should be readable regardless of locale).
# Of course right now strings in those files still run through the global
# gettext calls in mpr() and mprf(), so if they are present elsewhere and
# thus extracted, they *will* end up translated.

# The multiple mprf specs lead to several warnings along these lines:
# >>> ambiguous argument specification for keyword 'mprf'
# They are, however, both required (mprf can hold channel as 1st arg).

(find . -mindepth 1 -maxdepth 1 \( -name '*.cc' -or -name '*.h' \) -not \( \
      -name 'wiz-*.cc' -or -name 'chardump.cc'       -or -name 'ctest.cc'   \
  -or -name 'tile*.cc' -or -name 'skills2.cc'        -or -name 'files.cc'   \
  -or -name 'dbg-*.cc' -or -name 'mon-pick.cc'       -or -name 'zotdef.cc'  \
  -or -name 'l_*.cc'   -or -name 'mon-movetarget.cc' -or -name 'mapdef.cc'  \
  -or -name 'tags.cc'  -or -name 'mon-pathfind.cc'   -or -name 'mapmark.cc' \
\)
) | xgettext --files-from=- --output="$OUTPUT_FILE" \
             --from-code=UTF-8 --add-comments="$COMMENT_KEYWORD" \
             --no-wrap --sort-by-file --exclude-file="$EXCLUDE_FILE" \
             --copyright-holder='DCSS Team' \
             --package-name='DCSS' \
             --package-version="$VERSION" \
             --msgid-bugs-address='mail@example.com' \
             \
             --add-comments=/ \
             --keyword=_ \
             --keyword=no_ \
             --keyword=mpr \
             --keyword=mprf --keyword=mprf:2 --keyword=mprf:3 \
             --keyword=mprf_plural:2,3 --keyword=mprf_plural:3,4 --keyword=mprf_plural:4,5 \
             \
             --keyword=armour_prompt \
             --keyword=_annotate_form_based \
             --keyword=choose_inventory_deck \
             --keyword=_dragon_abil \
             --keyword=god_speaks:2 \
             --keyword=increase_duration:4 \
             --keyword=notify_fail \
             --keyword=_output_expiring_message:2 \
             --keyword=prompt_invent_item \
             --keyword=prompt_invent_items \
             --keyword=simple_god_message \
             --keyword=simple_monster_message:2 \
             --keyword=yesno \
             --keyword=zotdef_danger_msg \


# Now add user-facing messages from vaults and various other lua sources
# (troves, autofight, sprints etc.) to the existing file generated above.
(find dat -mindepth 1 \( -name '*.des' -or -name '*.lua' \) -not \( \
       -name 'gearset.lua' -or -name 'test.des' \
\)
) | xgettext --files-from=- --output="$OUTPUT_FILE" \
             --from-code=UTF-8 --add-comments="$COMMENT_KEYWORD" \
             --no-wrap --sort-by-file --exclude-file="$EXCLUDE_FILE" \
             --copyright-holder='DCSS Team' \
             --package-name='DCSS' \
             --package-version="$VERSION" \
             --msgid-bugs-address='mail@example.com' \
             \
             --join-existing \
             --language=Lua \
             --add-comments=/ \
             --keyword \
             --keyword=crawl.mpr \
             --keyword=crawl.yesno \
             --keyword=crawl.yesnoquit \
             --keyword=message_at_spot \


# Print some stats (amount of strings).
msgfmt -o /dev/null --statistics "$OUTPUT_FILE"

# Update language files
for LANG in `cd $OUTPUT_DIR; ls -d ??_??`
    do msgmerge -U $OUTPUT_DIR/$LANG/messages.po $OUTPUT_DIR/messages.pot
done
