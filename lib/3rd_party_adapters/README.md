\dir .
\brief Package \ref third_party_adapters

\page third_party_adapters 3rd Party Adapters
\brief Servers as a bridge to external code.

This package is a special kind of interface adapters.
It contains components which act as a bridge between software's own code and external code.

"Own" code is authored and fully controlled by the organization. 
"External" code is code which is provided by other parties.
That code is used by the "own" code.
External code may be incorporated as source code or (pre-)compiled code.

3rd party adapters shield the own code from the external code.
That is, any changes to external code shall only impact the 3rd party adapters.
That includes for example if one library is exchanged by another library which servers the same purpose.

Which 3rd party code may be directly used without adapter is appointed by \ref flexible_structure.
