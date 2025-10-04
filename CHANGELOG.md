# Changelog
<!-- Order of Entries: Added -> Fixed -> Changed -> Removed -->
All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

## [1.2.0] - 2025-10-04

### Added

- Benchmarking harness for measuring frame latency and memory usage
- .gitkeep files to maintain build and logs directories in version control

### Fixed

- Corrected location of cube executable to be in build/ directory

### Changed

- Updated .gitignore to exclude build artifacts and benchmark logs
- Enhanced Makefile with benchmark target and modular object compilation
- Refactored clean target in Makefile to ignore the gitkeep
- Added benchmarking section to README with usage instructions

## [1.1.0] - 2025-08-05

### Added

- Using Keep a Changelog format!

### Changed

- Modularized codebase into separate modules
- Replaced magic numbers with constants
- Adjusted documentation
- Enhanced error handling
- Allowing more of a dynamic buffer management than before
- Completely rewrote the readme

## [1.0.0] - 2024-03-05

### Added

- Initial release of the project
- Extensive in-code documentation for clarity
- Basic 3D cube rendering and rotation
- Solid makefile as build system

[1.2.0]: https://github.com/D-Heger/cube/compare/v1.1.0...v1.2.0
[1.1.0]: https://github.com/D-Heger/cube/compare/v1.0.0...v1.1.0
[1.0.0]: https://github.com/D-Heger/cube/releases/tag/v1.0.0
