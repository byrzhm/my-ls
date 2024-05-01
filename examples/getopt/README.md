# Key Points

## `getopt()` may change `argv`

By default, `getopt()` permutes the contents of `argv` as it scans,
so that eventually all the nonoptions are at the end.

## `optind` is useful

If there are no more option characters, `getopt()` returns `-1`. Then `optind` is the index in `argv` of the first `argv`-element that is not an option.

## `optarg` is useful

If such a character is followed by a colon, the option requires an argument, so `getopt()`
places a pointer to the following text in the same `argv`-element, or the text of the following
 `argv`-element, in `optarg`. Two colons mean an option takes an optional arg; if there is text in the
current `argv`-element (i.e., in the same word as the option name itself, for example, "`-oarg`"), 
then it is returned in `optarg`, otherwise `optarg` is set to zero.

## `--` is special

The special argument "--" forces an end of option-scanning regardless of the
scanning mode.

## Error handling

While processing the option list, `getopt()` can detect two kinds
of errors: (1) an option character that was not specified in
`optstring` and (2) a missing option argument (i.e., an option at
the end of the command line without an expected argument).  Such
errors are handled and reported as follows:

-  By default, `getopt()` prints an error message on standard
    error, places the erroneous option character in optopt, and
    returns '?' as the function result.

-  If the caller has set the global variable opterr to zero, then
    `getopt()` does not print an error message.  The caller can
    determine that there was an error by testing whether the
    function return value is '?'.  (By default, opterr has a
    nonzero value.)

-  If the first character (following any optional '+' or '-'
    described above) of optstring is a colon (':'), then `getopt()`
    likewise does not print an error message.  In addition, it
    returns ':' instead of '?' to indicate a missing option
    argument.  This allows the caller to distinguish the two
    different types of errors.

