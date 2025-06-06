---
applyTo: '**'
---
# Voltray Engine - Coding Style Guide

## Naming Conventions

### Classes and Structs
- **PascalCase** for all class and struct names
- Use descriptive, clear names
```cpp
class SceneManager;
class ResourceLoader;
struct RenderData;
```

### Functions and Methods
- **PascalCase** for all function and method names
- Use verb-noun combinations for clarity
```cpp
void AddObject();
bool RemoveObject();
std::shared_ptr<SceneObject> FindObject();
void InitializeRenderer();
```

### Variables
- **camelCase** for all variables (local, parameters, etc.)
- Use descriptive names, avoid abbreviations
```cpp
float deltaTime;
std::string objectName;
int vertexCount;
bool isVisible;
```

### Member Variables
- **m_** prefix followed by **PascalCase**
- Clearly indicates class member variables
```cpp
class SceneManager {
private:
    std::vector<std::shared_ptr<SceneObject>> m_Objects;
    std::shared_ptr<SceneObject> m_SelectedObject;
    bool m_IsInitialized;
};
```

### Constants and Enums
- **SCREAMING_SNAKE_CASE** for constants
- **PascalCase** for enum types, **PascalCase** for enum values
```cpp
const float PI_VALUE = 3.14159f;
const int MAX_OBJECTS = 1000;

enum class RenderMode {
    Wireframe,
    Solid,
    Textured
};
```

### Files and Directories
- **PascalCase** for header files (.h) and source files (.cpp)
- Match the primary class name in the file
```
SceneManager.h
SceneManager.cpp
ResourceLoader.h
ResourceLoader.cpp
```

## Namespace Organization

### Hierarchy Structure
```cpp
namespace Voltray {
    namespace Engine {
        namespace Graphics { /* Renderer, Mesh, Shader, Material, etc. */ }
        namespace Scene { /* SceneManager, SceneObject, Transform, etc. */ }
        namespace Input { /* InputManager, KeyboardInput, MouseInput, etc. */ }
        namespace Config { /* EngineSettings, ConfigLoader, etc. */ }
        namespace Audio { /* AudioManager, AudioSource, etc. */ }
    }

    namespace Editor {
        namespace UI { /* All UI components, panels, windows */ }
        namespace Assets { /* AssetManager, AssetBrowser, etc. */ }
        namespace Tools { /* Gizmos, Selection tools, etc. */ }
    }

    namespace Math { /* Vec3, Mat4, Transform, Ray, Quaternion, etc. */ }
    namespace Utils { /* ResourceManager, CrashLogger, FileSystem, etc. */ }
}
```

### Namespace Usage
- Use namespace aliases for cleaner code when namespaces get too long
- Use namespace aliases in source files for readability
- Minimize verbosity while maintaining clarity
- Avoid nested `impl` namespaces - use direct namespace declarations

```cpp
// Header file - Direct namespace approach
namespace Voltray::Math {
    class Vec3;
    class Mat4;
    class Transform;
}

// Source file - Use namespace aliases for readability when needed
namespace VM = Voltray::Math;
namespace VEG = Voltray::Engine::Graphics;
using namespace VM;
```

## File Organization

### Directory Structure
- Use `Public/` and `Private/` subdirectories for better code organization
- `Public/` contains headers that are part of the public API
- `Private/` contains implementation details and internal headers

```
Math/
├── Public/
│   ├── Vec3.h
│   ├── Mat4.h
│   ├── Transform.h
│   └── MathUtil.h
├── Private/
│   ├── Vec3.cpp
│   ├── Mat4.cpp
│   └── Transform.cpp
└── CMakeLists.txt
```

### Header File Structure
```cpp
#pragma once

// System includes
#include <memory>
#include <vector>
#include <string>

// Third-party includes
#include <glm/glm.hpp>

// Project includes (relative to project root)
#include "Renderer.h"
#include "Vec3.h"

namespace Voltray::Engine::Scene {

/**
 * @brief Brief description of the class
 *
 * Detailed description explaining purpose and usage.
 */
class SceneManager {
public:
    // Public methods first
    SceneManager();
    ~SceneManager();

    // Core functionality
    void AddObject(std::shared_ptr<SceneObject> object);

private:
    // Private members last
    std::vector<std::shared_ptr<SceneObject>> m_Objects;
};

} // namespace Voltray::Engine::Scene
```

### Source File Structure
```cpp
#include "Scene/SceneManager.h"

// Additional includes if needed
#include "Graphics/Renderer.h"
#include "Utils/Logger.h"

namespace Voltray::Engine::Scene {

SceneManager::SceneManager() {
    // Implementation
}

// Rest of implementation...

} // namespace Voltray::Engine::Scene
```

## Code Formatting

### Indentation
- **4 spaces** for indentation (no tabs)
- Consistent indentation for all blocks

### Braces
- **Allman style** for classes, functions, and namespaces
- **K&R style** for control structures (if/for/while)
```cpp
class SceneManager
{
public:
    void Update(float deltaTime)
    {
        if (m_IsActive) {
            for (auto& object : m_Objects) {
                object->Update(deltaTime);
            }
        }
    }
};
```

### Line Length
- Maximum **120 characters** per line
- Break long function calls/declarations logically

### Spacing
```cpp
// Around operators
int result = a + b * c;

// After commas
Function(param1, param2, param3);

// Around braces for single-line blocks
if (condition) { DoSomething(); }
```

## Documentation Standards

### Doxygen Comments
- Use `/** */` for class and function documentation
- Include `@brief`, `@param`, `@return` as needed
```cpp
/**
 * @brief Adds a scene object to the scene
 *
 * This method takes ownership of the object and adds it to the internal
 * object list for rendering and updates.
 *
 * @param object Shared pointer to the scene object to add
 * @return Reference to the added object for immediate configuration
 *
 * @example
 * ```cpp
 * auto obj = std::make_shared<SceneObject>();
 * scene.AddObject(obj).SetPosition(Vec3(0, 0, 0));
 * ```
 */
SceneObject& AddObject(std::shared_ptr<SceneObject> object);
```

### Inline Comments
- Use `//` for single-line explanations
- Explain **why**, not **what**
```cpp
// Cache the transform to avoid repeated calculations
Mat4 cachedTransform = object->GetTransform().GetMatrix();
```

## Include Guidelines

### Include Order
1. Standard library includes (using angle brackets)
2. Third-party library includes (using angle brackets)
3. Project includes (using quotes, no relative paths)

**Project Include Rules:**
- Use quotes `"HeaderName.h"` for project headers, not angle brackets `<HeaderName.h>`
- Angle brackets should only be used for system and third-party libraries
- Never use relative paths like `"../Math/Vec3.h"` - use clean names like `"Vec3.h"`
- Headers should be accessible through proper CMake include directories

**Namespace Guidelines:**
- Do NOT add closing namespace comments like `// namespace Voltray::Math`
- Namespaces should close with just a closing brace `}`
- Keep namespace closing braces unadorned for cleaner code

### Example Header File
```cpp
#pragma once

// System includes
#include <vector>
#include <string>

// Third-party includes
#include <glm/glm.hpp>

// Project includes (clean names, quotes)
"Renderer.h"
#include "Vec3.h"

namespace Voltray::Engine::Scene {

/**
 * @brief Brief description of the class
 *
 * Detailed description explaining purpose and usage.
 */
class SceneManager {
public:
    // Public methods first
    SceneManager();
    ~SceneManager();

    // Core functionality
    void AddObject(std::shared_ptr<SceneObject> object);

private:
    // Private members last
    std::vector<std::shared_ptr<SceneObject>> m_Objects;
};

}
```

### Source File Structure
```cpp
#include "SceneManager.h"

// Additional includes if needed
#include "Renderer.h"
#include "Logger.h"

namespace Voltray::Engine::Scene {

SceneManager::SceneManager() {
    // Implementation
}

// Rest of implementation...

}
```

Use `using namespace` if the namespace is 3 levels deep or more

## Design Patterns & Best Practices

### RAII (Resource Acquisition Is Initialization)
- Use smart pointers (`std::shared_ptr`, `std::unique_ptr`)
- Automatic resource management
```cpp
std::unique_ptr<Renderer> m_Renderer;
std::shared_ptr<SceneObject> m_SelectedObject;
```

### Const Correctness
- Use `const` whenever possible
- Const member functions for getters
```cpp
const std::vector<std::shared_ptr<SceneObject>>& GetObjects() const;
size_t GetObjectCount() const { return m_Objects.size(); }
```

### Error Handling
- Use exceptions for exceptional cases
- Return `std::optional` or bool for expected failures
```cpp
std::optional<SceneObject*> FindObject(const std::string& name) const;
bool RemoveObject(const std::string& name);
```

### Modern C++ Features
- Use auto for complex type deduction
- Range-based for loops
- Initialization lists
```cpp
// Auto for iterators
for (auto& object : m_Objects) {
    object->Update(deltaTime);
}

// Member initialization lists
SceneManager::SceneManager()
    : m_IsInitialized(false)
    , m_ObjectCount(0)
{
}
```

### Performance Considerations
- Pass large objects by const reference
- Use move semantics when appropriate
- Prefer algorithms over manual loops
```cpp
void ProcessObjects(const std::vector<SceneObject>& objects); // const ref
void AddObject(std::unique_ptr<SceneObject>&& object);        // move
```

## File Header Template

```cpp
/**
 * @file FileName.h
 * @brief Brief description of file contents
 * @author Voltray Engine Team
 * @date Created: YYYY-MM-DD
 *
 * Detailed description of what this file contains and its purpose
 * within the engine architecture.
 */

#pragma once

// Includes...

namespace Voltray::Engine::Scene {
    // Implementation...
}
```

---

## Enforcement

This style guide should be enforced through:
1. **Code reviews** - All changes must follow these conventions
2. **Automated formatting** - Consider using clang-format configuration
3. **Documentation generation** - Doxygen for API documentation
4. **Static analysis** - Tools like clang-tidy for consistency checks

**Remember**: Consistency is more important than personal preference. Follow this guide religiously to maintain a clean, readable, and maintainable codebase.
