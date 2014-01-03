Slalom
======

Slalom is a simple functional language based on the [SKI combinator calculus](http://en.wikipedia.org/wiki/SKI_combinator_calculus). It is inspired somewhat by [Haskell](http://www.haskell.org/haskellwiki/Haskell) (which you may have heard of) but moreso by [Unlambda](www.madore.org/~david/programs/unlambda/) and [Iota](http://webcache.googleusercontent.com/search?q=cache:ScjNltvRk70J:http://semarch.linguistics.fas.nyu.edu/barker/Iota/%2Biota+jot&hl=en&gbv=2&ct=clnk) (which you probably haven't). The compiler is (or will be) written in [LLVM](http://llvm.org).

Dependencies
------------

* [Rake](http://rake.rubyforge.org)
* [Clang](http://clang.llvm.org)

Installing & Running
--------------------

In your terminal of choice, visit the base directory for the project, and run Rake. This will build 2 executables, _runtests_ and _repl_, and will execute _runtests_.

    $ rake

_runtests_ runs the unit tests. They should all pass; if they don't, then I did something wrong! The source code for the tests is in _slalom\_test.cpp_.

_repl_ starts a simple command line shell for the language. The source code for the REPL is in _slalom\_repl.cpp_. Right now, the REPL simply accepts a line of text and parses it. If the input has mismatched parentheses, it will give you a syntax error; otherwise, it will print the parse tree. You can exit the REPL by typing Ctrl-D on a blank line.

Syntax
------

Slalom is a purely functional programming language, in the sense that everything in the language is a function.

The Slalom REPL accepts a line of input, parses it, and returns a textual representation of the parse tree. In the examples below, user input is indicated by lines beginning with "`>> `" and the resulting output is displayed on the following line.

    >> _example input_
    _example output_

### Names

Names are the most basic elements of Slalom syntax. A name is any string of characters that does not contain spaces, tabs, or parentheses. Since Slalom is a purely functional language, every name is the name of a function.

In the REPL, a name evaluates to itself. All of the following are examples of names.

    >> A
    A
    >> theNameOfAFunction
    theNameOfAFunction
    >> 5+8
    5+8
    >> "This_looks_very_complicated!{But-it's-still-just-a-name.}"
    "This_looks_very_complicated!{But-it's-still-just-a-name.}"

### Function Application

When two functions are separated by whitespace (any number of spaces or tabs), the first is applied to the second. To put it another way, the second is passed as an argument to the first.

Every function in Slalom can be applied to another function. Every function application results in a new function.

When one function is applied to another, the REPL will wrap them in helpful parentheses.

    >> Applicator Input
    (Applicator Input)

If several functions are separated by whitespace, function application occurs left-to-right.

    >> one two three four
    (((one two) three) four)

### Grouping

You can change the order in which functions are applied by wrapping them in parentheses.

    >> one (two three four)
    (one ((two three) four))
    >> (A B) (C D)
    ((A B) (C D))

You can wrap a single function in parentheses if you want, but it doesn't really do anything.

    >> (A) (B)
    (A B)
    >> (((((stuckInTheMiddle)))))
    stuckInTheMiddle

If a parenthetical statement is involved in function application, the whitespace isn't necessary.

    >> outside(inside)
    (outside inside)
    >> (good)(neighbors)
    (good neighbors)

### The Empty Statement

The REPL interprets an empty line (or a line with just whitespace) as the function named "I".

    >> 
    I

An empty set of parentheses is also interpreted as "I".

    >> ()
    I
    >> AE () OU
    ((AE I) OU)
