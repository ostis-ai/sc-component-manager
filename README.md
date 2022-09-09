# sc-component-manager

- Put this repository in **sc-machine/sc-tools**
- Update sc-tools CMakeLists.txt with ```add_subdirectory(sc-component-manager)```

### sc-machine.ini sc-component-manager config example
```
[sc-component-manager]
repos_path = ../sc-tools/sc-component-manager/repos.txt
specifications_path = ../sc-tools/sc-component-manager/src/manager/specifications

log_type = Console
log_level = Debug
```

