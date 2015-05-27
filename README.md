# Experiments related to CVE-2015-3456

There is:

 - `exploit/` is an "exploit" (it just crashes QEMU).
 - `mock/` contains a stripped down version of QEMU. Only the
   vulnerability remains.
 - `patch/` contains a program to patch a running instance of
   QEMU. The main point is to not need debug symbols, nor the original
   executable on disk. Therefore, some information have to be provided
   for this program to work as expected.

The main idea around all the live patching is to modify the
`command_to_handler` array to make each command point to the "not
implemented" function. While the process is partially stopped by
ptrace (only the main thread), this seems a safe way to fix the
vulnerability (as long as you don't care the floppy controller not
working anymore).

This kind of patching works because `command_to_handler` is a static
uninitialized variable and therefore ends up in the BSS segment which
is writable. Also, except for PIE, the position of this segment in
memory is known at compile time. Unfortunately, QEMU is usually
compiled as PIE and therefore, that's not the case.

Of course, use at your own risk: modifying a running process could
wreak havoc on the world!

More detailed information is available here: http://vincent.bernat.im/en/blog/2015-hotfix-qemu-venom.html

To better understand live patching, here are some links:

 - http://duartes.org/gustavo/blog/post/anatomy-of-a-program-in-memory/
 - https://lwn.net/Articles/631631/
 - http://www.openbsd.org/papers/nycbsdcon08-pie/index.html
 - https://www.technovelty.org/linux/plt-and-got-the-key-to-code-sharing-and-dynamic-libraries.html
 - http://phrack.org/issues/59/8.html
 - http://www.hick.org/code/skape/papers/needle.txt
