Emotionless
===========

Emotionless is a PS2 emulator with similar ideals to Dolphin.

It is very incomplete, though, so even though PCSX2 may be incompatible with some of your games,
try that first, then Play!, then hpsx64, and then MAYBE in a few months, Emotionless.

I originally had something here where I was trying to be inspirational and shit. It obviously didn't work,
and now lots of people think I hate PCSX2 and its developers. Let me repeat, I have nothing but respect for them.
They're doing the best with what they have, and while they have made some stupid decisions (e.g. AVG bundle), I
don't blame the hard-working programmers for this. However, let's face it, PCSX2's codebase is unmaintainable.
It would be easier to start over than to try to continually fix PCSX2's code.

Tasks
================

- Get all EE opcodes implemented in the Interpreter.
- Emulate EE exceptions.
- Create a scheduling system, one that's hopefully somewhat generic, like Dolphin's CoreTiming.
- Emulate the EE's TLB. I haven't found docs yet, so we might have to resort to docs on base MIPS or PCSX2 code.
- Create a basic Qt UI
- Create a basic Qt debugger
- Start working on an IOP core.

Goals
=====

- Clean code
- As small an amount of hacks and kludges as possible
- Accuracy as long it is still playable on modern computers.

Contribution guide
==================

- Write code
- Run astyle over changes
- PR code
