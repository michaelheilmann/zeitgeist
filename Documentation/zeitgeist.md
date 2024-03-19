# Zeitgeist Built-In Types
*Zeitgeist* knows the following types: *Boolean*, *ForeignFunction*, *ForeignObject*, *Integer*, *List*, *Map*, *Real32*, *String*, and *Void*.

## List
The *List* type is represented by `Zeitgeist_List`.
The *List* type has the following complexities:
- O(1) for adding to the front, removing fron the front, adding to the back, removing from the back, and get.
- O(n) for insert and remove.

**Warning** The current list implementation (due to time constraints) is still O(n) for adding to the front and removing from the front.
A deque implementation is the solution.

## Map
The *Map* type is represented by `Zeitgeist_Map`.
The *Map* type has the following complexities:
- O(1) for insert and remove.

## Void
The *Void* type has only one value, the value *void*.
*void* has semantics with aggregate types *List* and *Map* and with the interpreters stack are as follows:
- If *void* is inserted into a *List*, then nothing is inserted ("nothing is inserted into a list").
- If *void* is pushed on the stack, then nothing is pushed ("nothing is pushed").
- If *void* is used as key in a map, then no entry is added ("nothing refers to the specified value").
- If *void* is used as a value in a map, then no entry is added ("the specified key refers to nothing").

## The environment-rendition model

*UpstreamRequests* are sent from Renditions to Zeitgeist environment.

For example, a Rendition has a window and wants the Zeitgeist process to exit when the window is closed.
```
Zeitgeist_UpstreamRequest* request = Zeitgeist_UpstreamRequest_createExitProcessRequest(state);
Zeitgeist_sendUpstreamRequest(state, request);
```

## Lifecycles of Renditions
Renditions are basically the programs the *Zeitgeist* interpreter executes.
An rendition implements four functions:
When a rendition is loaded, the function `Zeitgeist_String* Zeitgeist_Rendition_getName(Zeitgeist_State*)`
might be invoked by *Zeitgeist* in order to acquire the name of the rendition. If the rendition executed
for example via `./Zeitgeist-Interpreter --rendition <name>` then *Zeitgeist* will link the following
functions:
- `void Zeitgeist_Rendition_startup(Zeitgeist_State*)`
- `void Zeitgeist_Rendition_shutdown(Zeitgeist_State*)`
- `void Zeitgeist_Rendition_update(Zeitgeist_State*)`

These functions are invoked in a particular sequences:
- `Zeitgeist_Rendition_startup` is invoked. `Zeitgeist_Rendition_startup` fails, then `Zeitgeist_Rendition_startup` is reinvoked a until the *Zeitgeist* process terminates or a call to `Zeitgeist_Rendition_startup` succeeds.
- If a call to `Zeitgeist_Rendition_startup` succeeds, zero or more calls to `Zeitgeist_Rendition_update` follow. If a call to `Zeitgeist_Rendition_update` fails, then this is the last call to `Zeitgeist_Rendition_update` before a call to `Zeitgeist_Rendition_shutdown`.
- After the last call to `Zeitgeist_rendition_update` a single call to `Zeitgeist_Rendition_shutdown` follows.
  If `Zeitgeist_rendition_shutdown` raises an exception is ignored.
