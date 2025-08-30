# Contributing to VoxLux

Thank you for your interest in contributing to VoxLux! We appreciate your time and effort in making this project better.

## Table of Contents

- [Code of Conduct](#code-of-conduct)
- [Getting Started](#getting-started)
- [Development Workflow](#development-workflow)
- [Code Style](#code-style)
- [Commit Message Guidelines](#commit-message-guidelines)
- [Pull Request Process](#pull-request-process)
- [Reporting Bugs](#reporting-bugs)
- [Feature Requests](#feature-requests)
- [License](#license)

## Code of Conduct

This project and everyone participating in it is governed by our [Code of Conduct](CODE_OF_CONDUCT.md). By participating, you are expected to uphold this code.

## Getting Started

1. Fork the repository on GitHub
2. Clone your fork locally
   ```bash
   git clone https://github.com/your-username/VoxLux.git
   cd VoxLux
   ```
3. Set up the development environment (see README.md for details)
4. Create a new branch for your changes
   ```bash
   git checkout -b feature/your-feature-name
   ```

## Development Workflow

1. Make your changes
2. Run tests (if available)
3. Ensure your code follows the style guide
4. Commit your changes with a descriptive message
5. Push to your fork and submit a pull request

## Code Style

### C++ (Firmware)
- Follow the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html)
- Use 2 spaces for indentation
- Maximum line length of 100 characters
- Use `camelCase` for variables and functions
- Use `PascalCase` for classes and structs
- Use `UPPER_CASE` for constants

### Python
- Follow [PEP 8](https://www.python.org/dev/peps/pep-0008/)
- Use 4 spaces for indentation
- Maximum line length of 88 characters (Black formatter standard)

### Documentation
- Use [Google Style](https://google.github.io/styleguide/pyguide.html#38-comments-and-docstrings) for docstrings
- Keep documentation up-to-date with code changes

## Commit Message Guidelines

We follow the [Conventional Commits](https://www.conventionalcommits.org/) specification:

```
<type>[optional scope]: <description>

[optional body]

[optional footer(s)]
```

### Types

- `feat`: A new feature
- `fix`: A bug fix
- `docs`: Documentation only changes
- `style`: Changes that do not affect the meaning of the code (white-space, formatting, etc.)
- `refactor`: A code change that neither fixes a bug nor adds a feature
- `perf`: A code change that improves performance
- `test`: Adding missing tests or correcting existing tests
- `chore`: Changes to the build process or auxiliary tools and libraries

### Examples

```
feat: add user authentication

description of the feature

Closes #123
```

```
fix: resolve issue with LED timing

- Fixed timing calculation in led_controller.cpp
- Updated related tests

Fixes #456
```

## Pull Request Process

1. Ensure any install or build dependencies are removed before the end of the layer when doing a build
2. Update the README.md with details of changes to the interface, this includes new environment variables, exposed ports, useful file locations and container parameters
3. Increase the version numbers in any examples files and the README.md to the new version that this Pull Request would represent. The versioning scheme we use is [SemVer](http://semver.org/)
4. You may merge the Pull Request in once you have the sign-off of two other developers, or if you do not have permission to do that, you may request the second reviewer to merge it for you

## Reporting Bugs

Bugs are tracked as [GitHub issues](https://github.com/your-username/VoxLux/issues).

When reporting a bug, please include:
- A clear and descriptive title
- Steps to reproduce the issue
- Expected behavior
- Actual behavior
- Screenshots if applicable
- Your environment (OS, hardware, firmware version, etc.)

## Feature Requests

Feature requests are welcome! Please open an issue to discuss your idea before submitting a pull request.

## License

By contributing to VoxLux, you agree that your contributions will be licensed under its MIT License.
