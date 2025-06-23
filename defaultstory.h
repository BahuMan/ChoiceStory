#define DEFAULT_STORY \
"ID=START\n" \
"This is not a story but a manual\n" \
"\n" \
"Your story will contain short texts\n" \
"and 3 choices for the reader to make\n" \
"Every piece of text together with 3 choices\n" \
"is called a section. All sections in your\n" \
"story file should have 3 parts:\n" \
"A=ID|MANID\n" \
"B=story|MANSTORY\n" \
"C=menu|MANMENU\n" \
"\n" \
"ID=MANID\n" \
"The first line of every secton\n" \
"Should come after an empty line\n" \
"It should start with \"ID=\"\n" \
"(no quotes, no spaces)\n" \
"and a unique identifier\n" \
"A=again|START\n" \
"B=story|MANSTORY\n" \
"C=menu|MANMENU\n" \
"\n" \
"ID=MANSTORY\n" \
"Your story is just plain text.\n" \
"As long as it doesn't start with START,\n" \
"or A,B or C followed by an equals sign\n" \
"the program will display the text.\n" \
"A=again|START\n" \
"B=ID|MANID\n" \
"C=menu|MANMENU\n" \
"\n" \
"ID=MANMENU\n" \
"The menu is described in exactly 3 lines:\n" \
"Each line starts with A,B or C\n" \
"Followed by an equals sign\n" \
"Then a human readable choice\n" \
"separated by a vertical pipe '|'\n" \
"From the actual unique ID of what section to jump to\n" \
"A=again|START\n" \
"B=ID|MANID\n" \
"C=story|MANSTORY\n" \
"\n"
