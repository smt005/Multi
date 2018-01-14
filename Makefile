
POST_CMAKE_COMMAND_VS?= echo Done.
POST_CMAKE_COMMAND_XCODE?= open Project.xcodeproj && echo Done.

DIR_PROJECT_WIN_GLES="Build/WIN32_GLES"
DIR_PROJECT_WIN_GLFW="Build/WIN32_GLFW"
DIR_PROJECT_OSX="Build/OSX"

WIN_GLES:
	rm -rf ${DIR_PROJECT_WIN_GLES} && \
	mkdir -p ${DIR_PROJECT_WIN_GLES} && cd ${DIR_PROJECT_WIN_GLES} && \
	cmake -G "Visual Studio 15 2017" \
	-DBUILD_PALTFORM=WIN_GLES \
	../../ && \
	${POST_CMAKE_COMMAND_VS}
	
WIN_GLFW:
	rm -rf ${DIR_PROJECT_WIN_GLFW} && \
	mkdir -p ${DIR_PROJECT_WIN_GLFW} && cd ${DIR_PROJECT_WIN_GLFW} && \
	cmake -G "Visual Studio 15 2017" \
	-DBUILD_PALTFORM=WIN_GLFW \
	../../ && \
	${POST_CMAKE_COMMAND_VS}

OSX:
	rm -rf ${DIR_PROJECT_OSX} && \
	mkdir -p ${DIR_PROJECT_OSX} && cd ${DIR_PROJECT_OSX} && \
	cmake -G Xcode \
	-DBUILD_PALTFORM=OSX \
	../../ && \
	${POST_CMAKE_COMMAND_XCODE}
