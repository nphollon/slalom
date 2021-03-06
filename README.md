Slalom
======

Slalom is a simple functional language based on the [SKI combinator calculus](http://en.wikipedia.org/wiki/SKI_combinator_calculus). It is inspired somewhat by [Haskell](http://www.haskell.org/haskellwiki/Haskell) (which you may have heard of) but moreso by [Unlambda](http://www.madore.org/~david/programs/unlambda/) and [Iota](http://webcache.googleusercontent.com/search?q=cache:ScjNltvRk70J:http://semarch.linguistics.fas.nyu.edu/barker/Iota/%2Biota+jot&hl=en&gbv=2&ct=clnk) (which you probably haven't). The compiler is (or will be) written in [LLVM](http://llvm.org).

Dependencies
------------

### Runtime Dependencies

* [Rake](http://rake.rubyforge.org) v 10.1 or later
* [Clang & LLVM](http://llvm.org/releases/download.html#3.4) v 3.4

### Test Dependencies

* [Boost Unit Test Framework](http://sourceforge.net/projects/boost/files/boost/1.55.0/) v 1.55.0
* [Hippo Mocks](https://www.assembla.com/wiki/show/hippomocks) v 3.1 (You don't need to install this separately. It's included in the Slalom repository.)

License
-------

Copyright (C) 2014  Nicholas Hollon

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the [GNU General Public License](https://www.gnu.org/licenses/gpl.html), for more details.

The full text of the GPLv3 is available in the LICENSE file of this project.

All dependencies are subject to free software licenses compatible with the GPLv3. Rake is licensed under the [MIT "Expat" License](http://rake.rubyforge.org/MIT-LICENSE.html). Clang and LLVM are license under the [University of Illinois/NCSA Open Source License](http://opensource.org/licenses/UoI-NCSA.php). Boost is licensed under the [Boost Software License](http://www.boost.org/users/license.html). Hippo Mocks is licensed under the [GNU Lesser General Public License](https://www.gnu.org/licenses/lgpl.html).

Installing & Running
--------------------

In your terminal of choice, visit the base directory for the project, and run Rake.

    $ rake

This will build 2 executables, `runtests` and `repl`. It will also execute `runtests`, which runs the unit tests. The source code for the tests is in `slalom_test.cpp`.

`repl` starts a simple command line shell for the language. The source code for the REPL is in `slalom_repl.cpp`. Right now, the REPL simply accepts a line of text and parses it. If the input has mismatched parentheses, it will give you a syntax error; otherwise, it will print the parse tree. You can exit the REPL by typing Ctrl-D on a blank line.

Syntax
------

Slalom is a purely functional programming language. Everything in the language is a function. Nothing is not a function.

The Slalom REPL accepts a line of input, parses it, and returns a textual representation of the parse tree. In the examples below, user input is indicated by lines beginning with "`>> `", and the resulting output is displayed on the following line.

    >> example input
    example output

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

When two functions are separated by whitespace (any number of spaces or tabs), the first is applied to the second. In other words, the second is passed as an argument to the first.

Every function in Slalom can be applied to another function. Every function application produces a new function.

When one function is applied to another, the REPL will wrap them in helpful parentheses.

    >> Applicator Input
    (Applicator Input)

If several functions are separated by whitespace, function application occurs from left to right.

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
    >> AE ( ) OU
    ((AE I) OU)
