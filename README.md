# iseforth

iseforth is a fancier REPL for [SEForth](https://github.com/Arkaeriit/SEForth).

![Usage illustration](https://github.com/Arkaeriit/iseforth/blob/master/iseforth.png?raw=true)

## Features

* Line editing with readline.
* Non volatile history.
* Configuration.

## Installation

To compile iseforth, you need to have SEForth installed with the options `SEF_STORE_NAME` and `SEF_PROGRAMMING_TOOLS` enabled.

Then simply run `make && sudo make install`.

## Configuration

The configuration is done in Forth by reading a rc files.

Here are the available options:

|Option name              |Type                                            |Description                                            |Default value          |
|-------------------------|------------------------------------------------|-------------------------------------------------------|-----------------------|
|`isef_prompt`            |null-terminated string                          |The prompt showed most time                            |` ok `                 |
|`isef_compilation_prompt`|null-terminated string                          |The prompt showed whilst defining a word               |`... `                 |
|`isef_prompt_color`      |ANSI escape sequence in a null-terminated string|The color of the prompt                                |`isef_blue`            |
|`isef_code_color`        |ANSI escape sequence in a null-terminated string|The color of what you write                            |`isef_yellow`          |
|`isef_history_file`      |null-terminated string                          |The path to the history file                           |`/tmp/iseforth-history`|
|`isef_history_file_size` |integer                                         |The maximum allowed number of lines in the history file|`1000`                 |

To help you define colors prompt, you can use the pre-defined colors:
* `isef_red`
* `isef_green`
* `isef_blue`
* `isef_yellow`
* `isef_magenta`
* `isef_cyan`
* `isef_black`
* `isef_white`

Note that string literals and strings made with `<# #>` are already terminated in SEForth, so the null-terminated sequences are easy to do.

Here is a configuration example:

```forth
: isef_prompt s" OK " drop ;
isef_green constant isef_prompt_color
150 constant isef_history_file_size
```

The rc files are:

* `$XDG_CONFIG_HOME/iseforthrc.frt`
* `$HOME/.iseforthrc.frt`
* `$PWD/iseforthrc.frt`

They are read in that order, the latest configuration defined taking priority. 

