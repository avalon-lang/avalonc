# Avalon - A classical-quantum programming language

[![License](https://img.shields.io/badge/LICENSE-Apache%202.0-blue.svg?style=for-the-badge)](https://www.apache.org/licenses/LICENSE-2.0)
[![Unitary Fund](https://img.shields.io/badge/Supported%20by-Unitary%20Fund-brightgreen.svg?style=for-the-badge)](http://unitary.fund)

This repository contains a rewrite of the Avalon programming language in C. I have abandoned development in C++ because I like the "simplicity" of C (compared to C++), which I am used to. I still plan on playing with the interpreter written in C++ as playground so not totally abandoned.

## Why a new programming language

Because we (at least I) need a language that offers support for quantum computers without jumping through hoops. Yes, one can program a quantum computer in Python (or C, C++, etc) but I find _most_ libraries built on top of existing classical languages to be awkward to use.
So I decided to go ahead and make a new one that will evolve with new quantum hardware being built.

Right now I am targeting only discrete variables quantum computers but I do plan on adding continuous variables and adiabatic (realistically annealing) quantum computers later.

## What's on the menu

The main things to look forward to in this rewrite are the following:

- I am working hard to make sure we have executables for most operating systems, mainly Linux based operating systems, Windows and MacOS.

- The target for code generation is QUIL. QUIL is an instruction set (architecture) meant for quantum computers being researched and manufactured by Rigetti Computing. In fact, it is quite general and I hope to use it as an intermediate language so I hope it is adopted throughout the industry.
In other words, what this means is that once the code generation module is done, you will be able to run your programs on the Rigetti stack.

- The language integrates linear and dependent types to help with hardware constraints. Because of the limited capabilities of the hardware we are targeting,
we have to catch as many errors as possible at compile time. Linear types and dependent types help us statically take into account the resources of the computer we are targeting. Not all possible errors will be caught but most.

Watch this README file for updates as they come.

## Contact

You can reach me at <a href="mailto:nbashige@gmail.com">nbashige@gmail.com</a>

## License

This code is licensed under the Apache 2.0 license. Please see the LICENSE file for the terms.

## Copyright

Copyright (c) 2019 Ntwali Bashige Toussaint
