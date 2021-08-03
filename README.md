# MetaSpline - UE4 Plugin
**Still a WIP**

The idea of this plugin is to make it super easy to add new metadata to spline points, instead of requiring hundreds of lines of code just to add a simple float. MetaSpline uses any UObject as a template for what data to add, so adding new metadata is as simple as creating a new BP that inherits from *Object*, adding the variables you want, and specifying that class as the meta class. It currently only supports floats and vector variables, but more types will come. Everything is exposed to BP, so no C++ is required to use a MetaSpline :)
