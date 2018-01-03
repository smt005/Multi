
POST_CMAKE_COMMAND_WIN?= echo Done.

DIR_PROJECT_WIN="Build/WIN"

WIN32:
	mkdir -p ${DIR_PROJECT_WIN} && cd ${DIR_PROJECT_WIN} && \
	cmake -G "Visual Studio 15 2017" \
	-DBUILD_PALTFORM=WIN32 \
	../../ && \
	${POST_CMAKE_COMMAND_WIN}
