# Quick Start

## Use sc-component-manager as an extension to sc-machine

### GitHub Releases

Note: currently, using sc-component-manager natively on Windows isn't supported.

1. Download pre-built artifacts of sc-machine from [GitHub Releases](https://github.com/ostis-ai/sc-machine/releases) and extract it to any location.
  To learn more about how to install and use sc-machine artifacts, see [sc-machine quick start](https://ostis-ai.github.io/sc-machine/quick_start/).
2. Download pre-built artifacts of sc-component-manager from [GitHub Releases](https://github.com/ostis-ai/sc-component-manager/releases) and extract it to any location.
3. Build component specifications located in `knowledge-base` directory at the root of the project:
  ```sh
  ./path/to/sc-builder/binary -i path/to/knowledge-base -o path/to/kb.bin --clear
  ```
4. Then specify the folder path to `lib/extensions` from extracted sc-component-manager folder when running the sc-machine binary:
  ```sh
  ./path/to/sc-machine/binary -s path/to/kb.bin \
      -e "path/to/extracted/sc-component-manager/lib/extensions;path/to/sc-machine/lib/extensions" \
      -c path/to/sc-component-manager.ini
  # if several paths to extensions are provided then they should be separated 
  # by semicolon and wrapped in double quotes
  ```
5. Initialize component specifications. Execute the following command in a running terminal:
  ```sh
  components init
  ```
6. Install components. For example, run:
  ```sh
  components install part_ui
  ```

To learn more about how to use sc-component-manager and to manage components with it, see [this docs](usage/usage.md).

In case you want to make changes to the project sources, please refer to the [build system docs](build/build_system.md).
