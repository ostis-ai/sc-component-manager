# Developers instruction
To develop sc-component-manager in __Clion IDE__, you should configure cmake.

## Sources configuration
1. In __File | Settings | Build, Execution, Deployment | CMake__ in __CMake options__ set __-DSC_BIN_PATH__ variable as below:
    ```angular2html
    -DSC_BIN_PATH="~/ostis-metasystem/bin"
    ```
   * __Note__: Instead of "~" write your own absolute path to a certain project (ostis-metasystem or ostis-web-platform).
   * __Example__:
   ![](../images/cmake_options.png)
2. In __Run/Debug Configuration__ in __Program arguments__ field insert flag -c and path to ini-file:
   ```angular2html
   -c ostis-metasystem.ini
   ```
   * __Note__: ini-file can be different.
   * __Example__:
   ![](../images/program_args.png)

## Tests Configuration
1. Remove __cmake-build-debug__, if exists.
2. In the root __CMakeLists.txt__ set on __SC_BUILD_TESTS__ variable as below:
    ```angular2html
    option(SC_BUILD_TESTS "Flag to build unit tests" ON)
    ```
3. Reload cmake project.
