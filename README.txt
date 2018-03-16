
Любительский проект для изучения всего что может потребоваться для разработки собственной игр.

Рабочие версии, для Windows 10 и OSX.

- Для генерации проекта под Windows (debug), требуется Make, CMake, Visual Studio 17. Команда в консоли "make WIN_GLES".

- Для генерации проекта под OSX (debug), требуется Make, CMake, XCode 9. Команда в консоли "make OSX". Для запуска приложения не из Xcode нужно скопировать файл "Multi/Project/ThirdParty/GLFW/OSX/libglfw.3.dylib" в директорию "/usr/local/lib/".