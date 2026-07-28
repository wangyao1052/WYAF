# WYAF — WY Application Framework

**WYAF** is a C++ application framework for building interactive 2D/3D design, drawing, modeling, and editing software. It provides a complete foundation for applications in CAD, BIM, GIS, vector illustration, 3D modeling, parametric design, and any domain requiring interactive graphical content creation and manipulation.

## Architecture

WYAF is organized in three layers, from bottom to top:

| Layer | Namespace | Responsibility |
|-------|-----------|----------------|
| **WYRX** (Runtime) | `wyrx` | Runtime type system: `Object` base class, `ClassInfo` RTTI, class registration and dynamic creation, `ClassExtension` mechanism |
| **WYDB** (Database) | `wydb` | Data layer: `Database` element container, `Element` model, `Transaction` system (undo/redo), dependency-driven chain update engine, file I/O |
| **WYAP** (Application Platform) | `wyap` | Application layer: `Application`, `Document`, `Scene`/`View`, `Command` system, `Selection` management, `Gizmo` interactors, `Environment` state machine, `Clipboard` |

## Core Capabilities

### Data Model
- **Element System** — Runtime-typed elements built on `wyrx::Object`, with field tracking, parameter system, parent-child hierarchy, erase/hide states, and health-state feedback
- **Transaction & Undo/Redo** — Full transactional system supporting commit/abort/undo/redo, transaction groups with merge, and undo/redo record inspection
- **Dependency-Driven Chain Update** — The core parametric modeling engine: when an element's data piece (Shape, Transform, Appearance, etc.) becomes dirty, dependent elements are automatically regenerated in dependency order
- **File Persistence** — Read/write support for binary and text formats with file version management

### Application Architecture
- **Document Model** — Document-database binding with status tracking (newly created / modified), multi-document management via `DocManager`
- **Scene / View Separation** — `Scene` handles interaction logic and behavioral responses; `View` handles visual rendering. Multiple views per document are supported
- **Environment State Machine** — Three-level nested environments: Gateway (application-wide) → Document → Task, each independently managing command and event subscriptions through enter/exit/suspend/resume lifecycles
- **Reactor Pattern** — Database, Transaction, Selection, Gizmo, Document, Command, and Environment all use reactor interfaces for event-driven observer notifications

### Interaction
- **Command System** — Supports both Modal and Transparent command types (similar to AutoCAD's command model), with PickFirst pre-selection and command history control
- **Selection Management** — Whole-element and sub-element selection with pick position and custom `SelectionData`, plus set-level operations
- **Gizmo Interactors** — Activatable/deactivatable interactive manipulators with refresh synchronization

### Foundation
- **Math Library** — `wy::Vector2`, `wy::Vector3`, `wy::Math`
- **Error Codes** — Comprehensive `wy::ErrorStatus` enumeration covering null pointers, parameters, elements, transactions, files, documents, commands, environments, and more
- **Iterators** — `wy::Iterator<T>` generic traversal interface

## Tech Specs

- **Language**: C++
- **Platform**: Windows x64 (MSVC 2019), with cross-platform compilation provisions
- **Build Artifacts**: Dynamic-link libraries (DLL), export controlled via `__declspec(dllexport/dllimport)`
- **SDK Version**: 1.0.0

## License

WYAF is available under a dual-licensing model:

- **Free Use** — For educational purposes, non-commercial research, open-source projects, individuals and companies with total annual revenue under CNY 1,000,000, and evaluation/testing. See [LICENSE](LICENSE) for full details.
- **Commercial Use** — A commercial license is required for production deployment, commercial projects, or client-facing work.

For commercial licensing inquiries, contact: [wangyao1052@163.com](mailto:wangyao1052@163.com)

## Getting Started

<!-- TODO: Add installation and quick-start instructions -->

## Documentation

<!-- TODO: Link to documentation -->

## Attribution

If you publish, present, or distribute work that uses WYAF, please include the following attribution:

> Built with WY Application Framework

---

© 2024-2026 Wang Yao. All rights reserved.
