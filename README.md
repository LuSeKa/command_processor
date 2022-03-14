# command_processor

A lightweight command processor to help accessing Arduino functions through the Serial interface. Each function needs to either conform to this prototype or be wrapped in a wrapper of the form
```
void foo(float a, float b){}
```

See the examples folder for a minimal example.
