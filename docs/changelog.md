# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Breaking changes

### Added

- Describe myself decomposition
- Add components logging
- Update clang-format
- Agents for init, search, install commands
- Documentation of CLion cmake-configuration
- Download components from a separate directory of a repository, not to clone all repository 
- Add repository url parser to get information from GitHub repository link
- Add installation of kb/ps/interface components to the relevant directories
- Add quiet installation mode
- Add SCn documentation
- Add SCn documentation environment
- Add contributing document
- Add codestyle document
- Add git workflow
- Add changelog
- Add license
- Add utils
- Add loading for downloaded scs files
- Add logging
- Add components install command
- Add components search command
- Add components init command
- Add storage config and config parser
- Create base environment to run commands
- Add commands redaction 

### Changed

- Make sc-component-manager as shared library
- Separated envs from envs of sc-machine
- Replace ExecutionResult with bool and update logging.
- Decompose install command to functions
- Move storage configs to kb

### Fixed

- Processing commands with multiple string spaces
- Install a component while running `components install [name]` with no `--idtf` flag
- Exception while running `components search --explanation` with an empty substring
- Use nrel_authors instead of nrel_author

### Removed

- Remove svn dependency
- Remove trunk folder when download git repository
- Remove storage parser
