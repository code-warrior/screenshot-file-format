# Screenshot File Format for Mac OS X [![Build Status](https://travis-ci.org/code-warrior/screenshot-file-format.svg?branch=v1.0.0)](https://travis-ci.org/code-warrior/screenshot-file-format)

A Mac OS X command line utility that allows users to choose between PNG, PDF, PSD, JPG, TIF, or GIF as file formats for screenshots.

Lint with Splint:

      splint main.c +skip-sys-headers -unrecog -compdef

`-unrecog` skips over warnings to do the process open (`popen`) and process close (`pclose`) functions.

`-compdef` skips over the potential for a memory problem.

Compile with Make:

      make
