# Contributing to Voltray

Thank you for your interest in contributing to Voltray! We welcome contributions from everyone. By following these guidelines, you help ensure that the development process is smooth and the codebase remains high-quality.

## Table of Contents

- [Development Workflow](#development-workflow)
  - [Branch Naming](#branch-naming)
  - [Commit Messages](#commit-messages)
- [Pull Request Process](#pull-request-process)
- [Code Style](#code-style)
- [Reporting Issues](#reporting-issues)

## Development Workflow

### Branch Naming

We use a strict branch naming convention to keep our repository organized. Please use one of the following prefixes for your branch names, followed by the issue number and a short description:

**Format:**
`prefix/[Issue Number]-[Description]`

**Prefixes:**
*   `feature/` - For new features
*   `bugfix/` - For bug fixes
*   `refactor/` - For code refactoring without behavior changes

**Examples:**
*   `feature/42-add-shadow-mapping`
*   `bugfix/101-fix-camera-jitter`
*   `refactor/7-renderer-cleanup`

### Commit Messages

Commit messages must follow a specific format to ensure we can track changes back to issues.

**Format:**
```
#[Issue Number]: [Commit message]
```

**Rules:**
*   **[Issue Number]**: The GitHub issue number this commit addresses.
*   **[Commit message]**: A concise description of the change. Use the imperative mood (e.g., "Add feature" not "Added feature").

**Examples:**
*   `#42: Implement basic shadow mapping`
*   `#101: Fix memory leak in texture loader`
*   `#7: Refactor scene graph traversal`

## Pull Request Process

1.  Ensure your code builds and runs correctly on your local machine.
2.  Update documentation if you are changing or adding features.
3.  Push your branch to your fork
4.  Open a **Pull Request** on GitHub targeting the `dev` branch.
5.  Fill out the Pull Request template with details about your changes.
6.  Wait for review. Address any feedback provided by the maintainers.

## Code Style

Voltray follows specific coding standards to ensure readability and maintainability:

*   **Language Standard**: C++17.
*   **Naming**:
    *   Classes: `PascalCase` (e.g., `SceneObject`)
    *   Functions: `PascalCase` (e.g., `GetPosition`)
    *   Variables: `camelCase` (e.g., `cameraPosition`)
    *   Member Variables: `m_camelCase` (e.g., `m_width`)
    *   Constants: `UPPER_SNAKE_CASE` (e.g., `MAX_LIGHTS`)
*   **Memory Management**: Use RAII and smart pointers (`std::shared_ptr`, `std::unique_ptr`) whenever possible. Avoid raw `new` and `delete`.
*   **const Correctness**: Mark methods and arguments as `const` wherever appropriate.

## Reporting Issues

*   **Bugs**: distinct description of the issue, steps to reproduce, and expected vs. actual behavior.
*   **Features**: Clear description of the proposed feature and why it would be useful.

Thank you for contributing!
