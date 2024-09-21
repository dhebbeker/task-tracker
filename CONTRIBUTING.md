*Thank you for considering to contribute to this project! 👍*

# How to contribute

This document shall help you to have a positive experience when contributing to this project. ☺️

## General

* In general the language for communication and documentation in this project is US English.
* We enforce [our Code of Conduct](CODE_OF_CONDUCT.md) so that all contributors feel safe and valued. 🛡️
* Don't be shy. Start to contribute *today*!

## Options for contributing

* If you have general questions or ideas, the [discussions space][] may be the right place to start.
  You can open a new topic and we can try to answer your question or develop an idea with you.
* If you are using the system and you found an error, please [report an issue](https://github.com/Task-Tracker-Systems/Task-Tracker-Device/issues/new/choose).
* If you would like to contribute content to the source base (code, schematics, designs, documentation, ...):
  * It is a good idea to contact us before you start working.
    This way we can inform you of implicit requirements and current work in progress.
    Just to make sure your efforts will not be in vain.
  * If you are sure your proposition is an improvement (for example you want to fix a typo), go ahead, fork the repository and issue a pull request.

To get in touch with us you may contact a [contributor](https://github.com/Task-Tracker-Systems/Task-Tracker-Device/graphs/contributors) directly (if they have contact information available) or create a topic in the [discussions space][].

[discussions space]: https://github.com/Task-Tracker-Systems/Task-Tracker-Device/discussions "discussions on GitHub"

## Contributing Code

Before submitting to this repository, please: 

* Familiarize yourself with the 
  [software architecture documentation](<\ref software_architecture> "software architecture documentation")
  and the
  [design decisions](<\ref doc/decisions> "design decisions").
* Test your contribution.
* Make sure your code does not introduce warnings or errors.
* Your proposition may be reviewed based on state of the art coding standards (for example [C++ Core Guidelines](https://isocpp.github.io/CppCoreGuidelines/) or [C++ Super FAQ](https://isocpp.org/faq)).

### Commit messages

Type a short, meaningful commit message that describes the change you made to the file.

Please consider [the recommendation of the Git community for commit messages](https://git-scm.com/docs/git-commit/2.38.0#_discussion):

> Begin the commit message with a single short (less than 50 character) line summarizing the change, followed by a blank line and then a more thorough description. The text up to the first blank line in a commit message is treated as the commit title, and that title is used throughout Git.

### Pull requests

Use self-reviews to speed up the review process.
This includes:

- Review the content for technical accuracy.
- If there are any failing checks in your PR, troubleshoot them until they're all passing.

### Document your code

Please document your code in Doxygen compatible syntax.

### C/C++ source code

Please format your changes using [ClangFormat](https://clang.llvm.org/docs/ClangFormat.html).
A style configuration file is given: [`.clang-format`](.clang-format)

Although the standalone tool `clang-format` may be used, we recommend to integrate the formatting using a [plugin](https://firefox-source-docs.mozilla.org/code-quality/coding-style/format_cpp_code_with_clang-format.html#editor-plugins) for the editor of your choice.

### Doxygen configuration

If you change [`Doxyfile`](Doxyfile), please do a ['cleanup'](https://stackoverflow.com/questions/71157463/what-part-of-the-doxygen-configuration-doxyfile-is-recommended-for-version-con#comment125783393_71157463) by using the output of `doxygen -x`.
For example

    doxygen -x > Doxyfile.tmp && mv Doxyfile.tmp Doxyfile

### PlatformIO configuration

If you change [`platformio.ini`](platformio.ini) using [PlatformIO Home](https://docs.platformio.org/en/latest/home/index.html#platformio-home), please note that [comments may be deleted unsolicitedly](https://github.com/platformio/platformio-core/issues/3862) by that tool.
In general we want to preserve useful comments in the project configuration file.
