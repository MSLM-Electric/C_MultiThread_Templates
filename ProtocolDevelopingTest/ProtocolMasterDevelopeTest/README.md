do next (at beggining):
<br /> 
<br /> mkdir build
<br /> cd build
<br /> cmake ..
<br /> 
<br /> and after generated buildings
<br /> go to the .sln file to see the project trees
<br /> and debug/run it
<br /> 
<br /> #cmake --trace .
<br /> #cmake --trace-expand
<br /> #cmake --debug-output
<br /> #cmake --trace-source=<file>        = Trace only this CMake file/module.  Multiple options allowed.