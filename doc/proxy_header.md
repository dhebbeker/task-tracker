# Proxy header {#proxy_header}

Proxy headers serve as hint to the PlatformIO Library Dependency Finder (LDF) to find 
the corresponding source file (see LDF [mode][] `chain`).
The actual interface is defined in the package where it is used.
This is necessary for cases where the PlatformIO build environment does not use `deep` 
as LDF mode.
Typically unit tests can not use LDF mode `deep` as this would prevent mocking source 
code which is not part of the unit under test.

This is used in case an interface is not inherited but instead forwarded to the actual 
implementation.

[mode]: https://docs.platformio.org/en/latest/librarymanager/ldf.html#dependency-finder-mode "PlatformIO Dependency Finder Modes"
