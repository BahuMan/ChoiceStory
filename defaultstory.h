#define DEFAULT_STORY \
"ID=START\n" \
"This is not a story but a manual. " \
"Your story will contain short texts " \
"and 3 choices for the reader to make. " \
"Every piece of text in your " \
"story file should have 3 parts:\n" \
"A=ID|MANID\n" \
"B=story|MANSTORY\n" \
"C=menu|MANMENU\n" \
"\n" \
"ID=MANID\n" \
"The first line of every section " \
"should start with \"ID=\" " \
"(no quotes, no spaces) " \
"and a unique identifier\n" \
"A=again|START\n" \
"B=story|MANSTORY\n" \
"C=menu|MANMENU\n" \
"\n" \
"ID=MANSTORY\n" \
"Your story is just plain text.\n" \
"No line should start with ID, " \
"A,B or C followed by an equals sign.\n" \
"A=again|START\n" \
"B=ID|MANID\n" \
"C=menu|MANMENU\n" \
"\n" \
"ID=MANMENU\n" \
"The menu is described in exactly 3 lines. " \
"Each line starts with A,B or C " \
"followed by an equals sign, " \
"a human readable choice " \
"separated by a vertical pipe '|' " \
"from the actual unique ID of what section to jump to\n" \
"A=again|START\n" \
"B=ID|MANID\n" \
"C=story|MANSTORY\n" \
"\n"
